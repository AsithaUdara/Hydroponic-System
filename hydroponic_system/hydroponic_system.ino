/*
 * Arduino Controlled Smart Hydroponic Modular System
 * IS1901 Microcontroller based ICT Project 
 * Description: Automated hydroponic system with pH, TDS, temperature, 
 * humidity, and light control using Arduino Mega 2560
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "DHT.h"

// ==================== LCD SETUP ====================
LiquidCrystal_I2C lcd(0x27, 16, 4);

// ==================== DHT22 SENSOR SETUP ====================
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ==================== KEYPAD SETUP ====================
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ==================== SENSOR PINS ====================
const int TDS_PIN = A1;
const int pH_PIN = A3;
const int LDR_PIN = A2;

// ==================== PUMP CONTROL PINS ====================
// TDS Control (Fertilizer)
#define PUMP_TDS_PIN1 11
#define PUMP_TDS_PIN2 10

// pH Control (Acid)
#define PUMP_ACID_PIN1 3
#define PUMP_ACID_PIN2 4

// pH Control (Base)
#define PUMP_BASE_PIN1 5
#define PUMP_BASE_PIN2 6

// ==================== FAN CONTROL PINS ====================
#define FAN_PIN1 31
#define FAN_PIN2 32

// ==================== RELAY PIN (LIGHTS) ====================
const int RELAY_PIN = 12;

// ==================== DEHUMIDIFIER PIN ====================
#define DEHUMIDIFIER_PIN 33

// ==================== SENSOR VARIABLES ====================
// TDS
float TDS_act = 0.0;
float desiredTDSMin = 800.0;
float desiredTDSMax = 1200.0;

// pH
float buffer_arr[10];
float ph_act = 7.0;
float avgval = 0;
float calibration_value = 21.34 + 1.18 - 0.39 + 0.5;
float desiredPHMin = 6.5;
float desiredPHMax = 7.5;

// Temperature & Humidity
float targetTemp = 28.0;
float targetHumidity = 70.0;
float currentTemp = 0.0;
float currentHumidity = 0.0;

// Light
int ldrValue = 0;
int lightThreshold = 500;

// ==================== TIMING VARIABLES ====================
unsigned long previousMillis = 0;
const long interval = 2000; // 2 seconds

// ==================== SYSTEM MODE ====================
enum SystemMode {
  AUTO_MODE,
  MANUAL_MODE,
  SETUP_MODE
};
SystemMode currentMode = AUTO_MODE;

// ==================== SETUP ====================
void setup() {
  Serial.begin(9600);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Initialize pump pins
  pinMode(PUMP_TDS_PIN1, OUTPUT);
  pinMode(PUMP_TDS_PIN2, OUTPUT);
  pinMode(PUMP_ACID_PIN1, OUTPUT);
  pinMode(PUMP_ACID_PIN2, OUTPUT);
  pinMode(PUMP_BASE_PIN1, OUTPUT);
  pinMode(PUMP_BASE_PIN2, OUTPUT);
  
  // Initialize fan pins
  pinMode(FAN_PIN1, OUTPUT);
  pinMode(FAN_PIN2, OUTPUT);
  
  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);
  
  // Initialize dehumidifier pin
  pinMode(DEHUMIDIFIER_PIN, OUTPUT);
  
  // Initialize sensor pins
  pinMode(pH_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(TDS_PIN, INPUT);
  
  // Turn off all actuators initially
  stopAllPumps();
  stopFans();
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(DEHUMIDIFIER_PIN, LOW);
  
  // Welcome screen
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("WELCOME");
  lcd.setCursor(2, 2);
  lcd.print("Hydroponic");
  lcd.setCursor(4, 3);
  lcd.print("System");
  delay(3000);
  lcd.clear();
  
  Serial.println("Smart Hydroponic System Initialized");
  Serial.println("Press 'A' for Auto Mode, 'B' for Manual Mode, 'C' for Setup");
}

// ==================== MAIN LOOP ====================
void loop() {
  unsigned long currentMillis = millis();
  
  // Check for keypad input
  char key = keypad.getKey();
  if (key) {
    handleKeypadInput(key);
  }
  
  // Execute based on current mode
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    switch (currentMode) {
      case AUTO_MODE:
        runAutoMode();
        break;
      case MANUAL_MODE:
        runManualMode();
        break;
      case SETUP_MODE:
        runSetupMode();
        break;
    }
  }
}

// ==================== AUTO MODE ====================
void runAutoMode() {
  // Read all sensors
  readTDSValue();
  readPHValue();
  readTemperatureHumidity();
  readLDR();
  
  // Adjust parameters automatically
  adjustTDS();
  adjustPH();
  adjustTemperature();
  adjustHumidity();
  adjustLighting();
  
  // Display on LCD
  displayAutoMode();
  
  // Print to Serial
  printSensorData();
}

// ==================== MANUAL MODE ====================
void runManualMode() {
  // Read all sensors but don't adjust automatically
  readTDSValue();
  readPHValue();
  readTemperatureHumidity();
  readLDR();
  
  // Display on LCD
  displayManualMode();
  
  // Print to Serial
  printSensorData();
}

// ==================== SETUP MODE ====================
void runSetupMode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setup Mode");
  lcd.setCursor(0, 1);
  lcd.print("1:TDS 2:pH");
  lcd.setCursor(0, 2);
  lcd.print("3:Temp 4:Hum");
  lcd.setCursor(0, 3);
  lcd.print("*:Back #:Save");
}

// ==================== KEYPAD HANDLER ====================
void handleKeypadInput(char key) {
  Serial.print("Key pressed: ");
  Serial.println(key);
  
  switch (key) {
    case 'A':
      currentMode = AUTO_MODE;
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("AUTO MODE");
      delay(1000);
      break;
    case 'B':
      currentMode = MANUAL_MODE;
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.print("MANUAL MODE");
      delay(1000);
      break;
    case 'C':
      currentMode = SETUP_MODE;
      break;
    case '*':
      if (currentMode == SETUP_MODE) {
        currentMode = AUTO_MODE;
      }
      break;
  }
}

// ==================== TDS FUNCTIONS ====================
void readTDSValue() {
  int tdsValue = analogRead(TDS_PIN);
  float voltage = (tdsValue / 1023.0) * 5.0;
  TDS_act = (133.42 * voltage * voltage * voltage - 
             255.86 * voltage * voltage + 857.39 * voltage) * 0.5;
}

void adjustTDS() {
  if (TDS_act < desiredTDSMin) {
    // Add fertilizer
    digitalWrite(PUMP_TDS_PIN1, HIGH);
    digitalWrite(PUMP_TDS_PIN2, LOW);
    delay(1000);
    digitalWrite(PUMP_TDS_PIN1, LOW);
    digitalWrite(PUMP_TDS_PIN2, LOW);
    Serial.println("Adding fertilizer...");
  } else if (TDS_act > desiredTDSMax) {
    // TDS too high - need to dilute (add water)
    Serial.println("TDS too high - dilution needed");
  } else {
    digitalWrite(PUMP_TDS_PIN1, LOW);
    digitalWrite(PUMP_TDS_PIN2, LOW);
  }
}

// ==================== pH FUNCTIONS ====================
void readPHValue() {
  for (int i = 0; i < 10; i++) {
    buffer_arr[i] = analogRead(pH_PIN);
    delay(30);
  }
  
  avgval = 0;
  for (int i = 0; i < 10; i++) {
    avgval += buffer_arr[i];
  }
  avgval /= 10;
  
  float voltage = avgval * 5.0 / 1024;
  ph_act = -5.70 * voltage + calibration_value;
}

void adjustPH() {
  if (ph_act < desiredPHMin) {
    // Add base (increase pH)
    digitalWrite(PUMP_BASE_PIN1, HIGH);
    digitalWrite(PUMP_BASE_PIN2, LOW);
    delay(1000);
    digitalWrite(PUMP_BASE_PIN1, LOW);
    digitalWrite(PUMP_BASE_PIN2, LOW);
    Serial.println("Adding base...");
  } else if (ph_act > desiredPHMax) {
    // Add acid (decrease pH)
    digitalWrite(PUMP_ACID_PIN1, HIGH);
    digitalWrite(PUMP_ACID_PIN2, LOW);
    delay(1000);
    digitalWrite(PUMP_ACID_PIN1, LOW);
    digitalWrite(PUMP_ACID_PIN2, LOW);
    Serial.println("Adding acid...");
  } else {
    digitalWrite(PUMP_BASE_PIN1, LOW);
    digitalWrite(PUMP_BASE_PIN2, LOW);
    digitalWrite(PUMP_ACID_PIN1, LOW);
    digitalWrite(PUMP_ACID_PIN2, LOW);
  }
}

// ==================== TEMPERATURE & HUMIDITY FUNCTIONS ====================
void readTemperatureHumidity() {
  currentTemp = dht.readTemperature();
  currentHumidity = dht.readHumidity();
  
  // Check if readings failed
  if (isnan(currentTemp) || isnan(currentHumidity)) {
    Serial.println("Failed to read from DHT sensor!");
    currentTemp = 0;
    currentHumidity = 0;
  }
}

void adjustTemperature() {
  if (currentTemp > targetTemp) {
    // Turn on fans to cool down
    digitalWrite(FAN_PIN1, HIGH);
    digitalWrite(FAN_PIN2, LOW);
    Serial.println("Cooling - Fans ON");
  } else {
    // Turn off fans
    digitalWrite(FAN_PIN1, LOW);
    digitalWrite(FAN_PIN2, LOW);
  }
}

void adjustHumidity() {
  if (currentHumidity > targetHumidity) {
    // Turn on dehumidifier
    digitalWrite(DEHUMIDIFIER_PIN, HIGH);
    Serial.println("Dehumidifier ON");
  } else {
    // Turn off dehumidifier
    digitalWrite(DEHUMIDIFIER_PIN, LOW);
  }
}

// ==================== LIGHT FUNCTIONS ====================
void readLDR() {
  ldrValue = analogRead(LDR_PIN);
}

void adjustLighting() {
  if (ldrValue < lightThreshold) {
    // Too dark, turn lights on
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Lights ON");
  } else {
    // Bright enough, turn lights off
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Lights OFF");
  }
}

// ==================== DISPLAY FUNCTIONS ====================
void displayAutoMode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TDS:");
  lcd.print(TDS_act, 1);
  lcd.print(" pH:");
  lcd.print(ph_act, 1);
  
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(currentTemp, 1);
  lcd.print("C H:");
  lcd.print(currentHumidity, 1);
  lcd.print("%");
  
  lcd.setCursor(0, 2);
  lcd.print("Light:");
  lcd.print(ldrValue);
  
  lcd.setCursor(0, 3);
  lcd.print("Mode: AUTO");
}

void displayManualMode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TDS:");
  lcd.print(TDS_act, 1);
  lcd.print(" pH:");
  lcd.print(ph_act, 1);
  
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(currentTemp, 1);
  lcd.print("C H:");
  lcd.print(currentHumidity, 1);
  lcd.print("%");
  
  lcd.setCursor(0, 2);
  lcd.print("Light:");
  lcd.print(ldrValue);
  
  lcd.setCursor(0, 3);
  lcd.print("Mode: MANUAL");
}

// ==================== UTILITY FUNCTIONS ====================
void stopAllPumps() {
  digitalWrite(PUMP_TDS_PIN1, LOW);
  digitalWrite(PUMP_TDS_PIN2, LOW);
  digitalWrite(PUMP_ACID_PIN1, LOW);
  digitalWrite(PUMP_ACID_PIN2, LOW);
  digitalWrite(PUMP_BASE_PIN1, LOW);
  digitalWrite(PUMP_BASE_PIN2, LOW);
}

void stopFans() {
  digitalWrite(FAN_PIN1, LOW);
  digitalWrite(FAN_PIN2, LOW);
}

void printSensorData() {
  Serial.println("=== Sensor Readings ===");
  Serial.print("TDS: ");
  Serial.print(TDS_act);
  Serial.println(" ppm");
  
  Serial.print("pH: ");
  Serial.println(ph_act);
  
  Serial.print("Temperature: ");
  Serial.print(currentTemp);
  Serial.println(" C");
  
  Serial.print("Humidity: ");
  Serial.print(currentHumidity);
  Serial.println(" %");
  
  Serial.print("Light Level: ");
  Serial.println(ldrValue);
  Serial.println("=======================");
}
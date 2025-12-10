# 🌱 Arduino Controlled Smart Hydroponic Modular System

[![Arduino](https://img.shields.io/badge/Arduino-Mega%202560-00979D?style=flat&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Active-success.svg)]()

An intelligent, automated hydroponic cultivation system powered by Arduino Mega 2560. This project automates environmental monitoring and control to optimize plant growth while reducing manual intervention and resource waste.

![System Overview](hydroponic_system/images/System%20Overview.jpeg)
*Complete Smart Hydroponic System*

![System Full View](hydroponic_system/docs/System%20Full%20View.jpeg)
*Detailed System Components*

![PCB Design](hydroponic_system/docs/PCB%20Design.jpeg)
*Custom PCB Design*

## 📋 Table of Contents

- [About The Project](#about-the-project)
- [Problem Statement](#problem-statement)
- [Solution](#solution)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Software Requirements](#software-requirements)
- [Circuit Diagram](#circuit-diagram)
- [Installation](#installation)
- [Usage](#usage)
- [System Architecture](#system-architecture)
- [Team Members](#team-members)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## 🎯 About The Project

This smart hydroponic system uses Arduino microcontrollers to automate the growing process, making it easier and more efficient for both experienced and novice gardeners to grow a wide variety of plants indoors year-round.

**Course:** IS1901 - Microcontroller based ICT Project  
**Group:** 13 (ITM)  
**Institution:** University of Moratuwa

## 🔍 Problem Statement

Traditional farming and manual hydroponics face significant challenges:

### Traditional Farming Issues:
- 💧 **Water Scarcity** - Inefficient irrigation in water-stressed regions
- 🌡️ **Climate Change** - Unpredictable weather disrupting farming cycles
- 🐛 **Pest Management** - Increased pest resistance and disease control challenges
- 🌍 **Land Limitations** - Limited arable land and soil degradation

### Manual Hydroponics Issues:
- ⏰ **Labor Intensive** - Constant monitoring of pH, TDS, and nutrients required
- ⚖️ **Human Error** - Manual adjustments lead to imbalances
- 🔧 **High Maintenance** - Regular cleaning and equipment maintenance
- 📈 **Scalability Problems** - Difficult to expand operations efficiently

## 💡 Solution

Our automated hydroponic system addresses these challenges through:

1. **Automated Monitoring** - Arduino sensors continuously monitor critical parameters
2. **Precision Control** - Automatic adjustment of water pumps, lighting, and climate
3. **Modular Design** - Easy expansion and customization for different plant types
4. **Resource Efficiency** - Optimized use of water, nutrients, and energy

## ✨ Features

### 🤖 Automated Systems

- **pH Control** - Automatic acid/base dosing to maintain optimal pH (6.5-7.5)
- **Nutrient Management** - TDS monitoring and fertilizer dosing (800-1200 ppm)
- **Temperature Regulation** - Fan control for optimal growing temperature
- **Humidity Control** - Dehumidifier automation for ideal moisture levels
- **Lighting Control** - Automatic grow light switching based on ambient light

### 📊 Monitoring & Display

- Real-time sensor data on 16x4 LCD display
- Serial monitor output for debugging
- Multi-parameter tracking (pH, TDS, temperature, humidity, light)

### 🎮 Operation Modes

- **Auto Mode** - Fully automated environmental control
- **Manual Mode** - Monitoring only, manual intervention required
- **Setup Mode** - Configure system parameters via keypad

## 🔧 Hardware Components

| Component | Model/Type | Quantity | Purpose |
|-----------|-----------|----------|---------|
| Microcontroller | Arduino Mega 2560 | 1 | Main controller |
| Temperature/Humidity Sensor | DHT22 | 1 | Climate monitoring |
| TDS Sensor | D105 | 1 | Nutrient concentration |
| pH Sensor | Analog pH Sensor | 1 | Acidity monitoring |
| Light Sensor | LDR | 1 | Ambient light detection |
| Display | 16x4 LCD with I2C | 1 | System status display |
| Keypad | 4x4 Matrix Keypad | 1 | User input |
| Motor Driver | L298N | 3 | Pump control |
| Relay Module | 5V 1-Channel | 1 | Light switching |
| Water Pump | R-385 12V DC Diaphragm | 3 | Acid/Base/Fertilizer dosing |
| Submersible Pump | Type 3 Ultra-quiet | 1 | Water circulation |
| Step-Down Converter | XL4015 | 1 | Power regulation |
| Buck Converter | LM2596 | 1 | Power regulation |
| Cooling Fans | 12V DC | 2 | Temperature control |
| Dehumidifier | TEC-based | 1 | Humidity control |

## 💻 Software Requirements

### Arduino IDE
- Version 1.8.x or higher
- Arduino Mega 2560 board support

### Required Libraries

```cpp
#include <Wire.h>                  // I2C communication
#include <LiquidCrystal_I2C.h>    // LCD display control
#include <Keypad.h>                // Keypad input
#include <DHT.h>                   // DHT22 sensor
```

### Library Installation

1. Open Arduino IDE
2. Go to `Sketch` → `Include Library` → `Manage Libraries`
3. Search and install:
   - **LiquidCrystal I2C** by Frank de Brabander
   - **Keypad** by Mark Stanley
   - **DHT sensor library** by Adafruit

## 📐 Circuit Diagram

### Pin Configuration

#### Sensors
```
DHT22 Sensor    → Pin 2
TDS Sensor      → Pin A1
pH Sensor       → Pin A3
LDR Sensor      → Pin A2
```

#### Actuators
```
TDS Pump (Motor 1)      → Pins 11, 10
pH Acid Pump (Motor 2)  → Pins 3, 4
pH Base Pump (Motor 3)  → Pins 5, 6
Cooling Fans            → Pins 31, 32
Light Relay             → Pin 12
Dehumidifier           → Pin 33
```

#### Display & Input
```
LCD I2C      → SDA (Pin 20), SCL (Pin 21)
Keypad Rows  → Pins 22, 23, 24, 25
Keypad Cols  → Pins 26, 27, 28, 29
```

### Power Supply
- 12V DC main power supply
- XL4015 step-down for pumps and fans
- LM2596 buck converter for Arduino (5V)

## 🚀 Installation

### 1. Clone the Repository

```bash
git clone https://github.com/AsithaUdara/Hydroponic-System.git
cd hydroponic-system
```

### 2. Hardware Setup

1. Assemble the circuit according to the pin configuration
2. Connect all sensors to designated analog/digital pins
3. Wire motor drivers to pump motors
4. Connect power supplies with proper voltage regulation
5. Mount LCD and keypad for user interface

### 3. Software Setup

1. Open `hydroponic_system.ino` in Arduino IDE
2. Select **Board**: Arduino Mega 2560
3. Select correct **Port**
4. Install required libraries (see Software Requirements)
5. Upload the code to Arduino Mega

### 4. Calibration

#### pH Sensor Calibration
```cpp
// Adjust this value based on pH 7.0 buffer solution
float calibration_value = 21.34 + 1.18 - 0.39 + 0.5;
```

#### TDS Sensor Calibration
```cpp
// Test with known TDS solution and adjust formula if needed
TDS_act = (133.42 * voltage³ - 255.86 * voltage² + 857.39 * voltage) * 0.5;
```

#### Light Threshold
```cpp
int lightThreshold = 500; // Adjust based on environment
```

## 📖 Usage

### Initial Startup

1. Power on the system
2. Wait for "WELCOME" screen
3. System initializes in **Auto Mode**

### Operation Modes

#### Auto Mode (Press 'A')
- Fully automated control
- System monitors all parameters
- Automatic adjustments when values out of range
- LCD shows: `Mode: AUTO`

#### Manual Mode (Press 'B')
- Monitoring only
- No automatic adjustments
- Manual intervention required
- LCD shows: `Mode: MANUAL`

#### Setup Mode (Press 'C')
- Configure system parameters
- Adjust target values
- Press '*' to return to Auto Mode

### Keypad Controls

```
[1] [2] [3] [A]  →  A: Auto Mode
[4] [5] [6] [B]  →  B: Manual Mode
[7] [8] [9] [C]  →  C: Setup Mode
[*] [0] [#] [D]  →  *: Back  #: Save
```

### LCD Display Layout

```
Row 1: TDS:xxx.x pH:x.x
Row 2: T:xx.xC H:xx.x%
Row 3: Light:xxx
Row 4: Mode: [AUTO/MANUAL]
```

### Serial Monitor

Baud Rate: **9600**

Example Output:
```
=== Sensor Readings ===
TDS: 950.3 ppm
pH: 6.8
Temperature: 24.5 C
Humidity: 65.2 %
Light Level: 320
=======================
```

## 🏗️ System Architecture

### Control Flow

```
┌─────────────┐
│   Sensors   │
│ (Read Data) │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  Arduino    │
│   (Logic)   │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  Actuators  │
│  (Control)  │
└─────────────┘
```

### Decision Logic

**pH Control:**
- If pH < 6.5 → Add base (KOH)
- If pH > 7.5 → Add acid (H₃PO₄)
- Else → No action

**TDS Control:**
- If TDS < 800 ppm → Add fertilizer
- If TDS > 1200 ppm → Dilute solution
- Else → No action

**Temperature Control:**
- If Temp > target → Turn on fans
- Else → Turn off fans

**Humidity Control:**
- If Humidity > target → Turn on dehumidifier
- Else → Turn off dehumidifier

**Light Control:**
- If LDR < 500 → Turn lights ON
- Else → Turn lights OFF

## 👥 Team Members

| Name | Registration No. | Responsibilities |
|------|-----------------|------------------|
| **Deshapriya B.G.A.U** (Leader) | 225128K | PCB Design, Power Analysis, Code Integration, LCD & Keypad Programming, Structure Building |
| **Peligedara S.P.G.M.L** | 225079J | TDS Sensor, Fertilizer Management, Pumping Mechanism, Motor Driver, Structure Building |
| **Rohana K.S** | 225125A | DHT22 Sensor, Temperature/Humidity Control, Dehumidifier Design, Structure Building |
| **Jayasundara K.G.K.G.D** | 225127G | pH Sensor, Acid/Base Solutions, pH Pumping Mechanism, Motor Driver, Structure Building |
| **Magammana I.N** | 225126D | LDR Sensor, Relay Module, Lighting Control, Structure Building |

## 🎯 Project Objectives

- ✅ Develop Arduino-based automated monitoring and control system
- ✅ Design modular, user-friendly solution for all skill levels
- ✅ Promote sustainable agriculture practices
- ✅ Reduce environmental impact of traditional farming
- ✅ Optimize resource usage (water, nutrients, energy)
- ✅ Increase crop yields through precision control

## 📊 Benefits

- 🎯 **Precision Control** - Exact environmental parameters
- 💰 **Cost Reduction** - Less manual labor and resource waste
- 🌱 **Higher Yields** - Optimal growing conditions
- 💧 **Water Efficiency** - 90% less water than traditional farming
- 🌍 **Sustainability** - Year-round indoor cultivation
- 📈 **Scalability** - Modular design for easy expansion

## 🔮 Future Enhancements

- [ ] Mobile app integration for remote monitoring
- [ ] IoT connectivity for cloud data logging
- [ ] Machine learning for predictive maintenance
- [ ] Multi-zone control for different plant types
- [ ] Solar power integration
- [ ] Automated harvesting notifications
- [ ] Integration with weather APIs
- [ ] Camera system for plant health monitoring

## 🐛 Troubleshooting

### Common Issues

**LCD not displaying:**
- Check I2C address (default: 0x27)
- Verify SDA/SCL connections
- Adjust contrast potentiometer

**Sensor reading errors:**
- Check sensor connections and power supply
- Verify analog pin assignments
- Calibrate sensors with known standards

**Pumps not activating:**
- Check motor driver connections
- Verify power supply voltage (12V)
- Test motors independently

**Serial monitor not responding:**
- Verify baud rate (9600)
- Check USB connection
- Restart Arduino IDE

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- University of Moratuwa - Department of Information Technology
- Course Instructor: IS1901 - Microcontroller based ICT Project
- Arduino Community for extensive documentation
- Open-source library contributors

## 📞 Contact

For questions or collaboration:

- **Project Repository:** [GitHub](https://github.com/AsithaUdara/Hydroponic-System)
- **Email:** udarauni001@gmail.com
- **University:** University of Moratuwa, Sri Lanka

## 🌟 Star History

If you find this project useful, please consider giving it a star ⭐

---

**Made with 💚 by Group 13 (ITM) - University of Moratuwa**

*Cultivating the future, one plant at a time* 🌱

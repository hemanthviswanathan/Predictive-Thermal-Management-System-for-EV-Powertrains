# Predictive Thermal Management System for EV Powertrains

An edge-computed, predictive thermal management system designed for high-density Electric Vehicle (EV) battery packs and power electronics. Built on an ESP32 microcontroller, this project uses discrete derivative calculus ($\frac{dT}{dt}$) and Exponential Moving Average (EMA) signal filtering to anticipate thermal runaway and activate solid-state cooling 12 to 15 seconds earlier than traditional thermostats.

---

## Key Features

- **Predictive Derivative Control ($\frac{dT}{dt}$):** Evaluates thermal velocity every 1.0 second inside a hardware timer interrupt.
- **Signal Noise Suppression:** Applies an Exponential Moving Average (EMA, $\alpha=0.3$) filter to eliminate high-frequency EMI noise from power converters.
- **Galvanic Electrical Protection:** Interfaces the 3.3V ESP32 microcontroller with high-current 12V cooling fans using an opto-isolated solid-state relay circuit.
- **Fail-Safe Robustness:** Features open-circuit and NaN fault detection that automatically forces 100% continuous cooling during sensor failure.

---

## Hardware Architecture & Pinout

| Component | ESP32 Pin / Rail | Function |
| :--- | :--- | :--- |
| **BMP280 VCC** | 3.3V | Sensor Power Supply |
| **BMP280 GND** | GND | Common Ground Reference |
| **BMP280 SDA** | GPIO 21 | I2C Serial Data Line |
| **BMP280 SCL** | GPIO 22 | I2C Serial Clock Line |
| **Relay VCC** | VIN (5V) | Relay Coil Electromagnet Power |
| **Relay GND** | GND | Common Ground Reference |
| **Relay IN** | GPIO 5 | 3.3V Logic Trigger Output |
| **Relay COM / NO**| External 12V Supply | Isolated 12V Cooling Fan Circuit Loop |

---

## Mathematical Model

### 1. Digital Noise Filtering (EMA)
$$T_{\text{filtered}}[k] = \alpha \cdot T_{\text{raw}}[k] + (1 - \alpha) \cdot T_{\text{filtered}}[k-1]$$

### 2. First-Order Derivative Calculus
$$\frac{dT}{dt} \approx T_{\text{filtered}}[k] - T_{\text{filtered}}[k-1]$$

### 3. Actuation Condition
The cooling relay activates if:
$$\left(\frac{dT}{dt} \ge 1.0^\circ\text{C/s}\right) \lor \left(T_{\text{filtered}} \ge 45.0^\circ\text{C}\right)$$

---

## How to Build & Flash

1. Install [VS Code](https://code.visualstudio.com/) and the **PlatformIO** extension (or use the Arduino IDE with the ESP32 v3.0 core).
2. Clone this repository.
3. Open the repository folder in PlatformIO / Arduino IDE.
4. Install required libraries: `Adafruit BMP280 Library` and `Adafruit Unified Sensor`.
5. Connect your ESP32 board via USB, set baud rate to `115200`, compile, and flash the firmware.

---

## Authors

- **Hemanth V** (Reg. No: 26MEA0021)
- **Praveen Kumar** (Reg. No: 26MEA0027)
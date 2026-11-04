# Predictive Thermal Management System for EV Powertrains

An edge-computed, predictive thermal management system designed for high-density Electric Vehicle (EV) battery packs and power electronics. Built on an ESP32 microcontroller, this project uses discrete derivative calculus and Exponential Moving Average (EMA) signal filtering to anticipate thermal runaway and activate solid-state cooling 12 to 15 seconds earlier than traditional reactive thermostats.

---

## 🚀 Key Features

* **Predictive Derivative Control:** Evaluates thermal velocity every 1.0 second inside a non-blocking hardware timer interrupt.
* **Signal Noise Suppression:** Applies an Exponential Moving Average (EMA, $\alpha=0.3$) filter to eliminate high-frequency EMI noise from power converters.
* **Galvanic Electrical Protection:** Interfaces the 3.3V ESP32 microcontroller with high-current 12V cooling fans using an opto-isolated solid-state relay circuit.
* **Fail-Safe Robustness:** Features open-circuit and NaN fault detection that automatically forces 100% continuous cooling during sensor hardware failure.

---

## ⚡ Hardware Architecture & Pinout

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

*Note: The sensor is powered strictly via 3.3V to protect the MCU's GPIO pins, while the relay coil utilizes the 5V VIN rail to guarantee physical actuation.*

---

## 🧮 Mathematical Model

**1. Digital Noise Filtering (EMA)**
To prevent the fan from chattering due to electrical noise, raw data is smoothed:
$$T_{\text{filtered}}[k] = \alpha \cdot T_{\text{raw}}[k] + (1 - \alpha) \cdot T_{\text{filtered}}[k-1]$$
*(Where smoothing factor $\alpha = 0.3$)*

**2. First-Order Derivative Calculus**
The ESP32 calculates the continuous rate of temperature change:
$$\frac{dT}{dt} \approx T_{\text{filtered}}[k] - T_{\text{filtered}}[k-1]$$

**3. Actuation Condition**
The cooling relay activates preemptively if:
$$\left(\frac{dT}{dt} \ge 1.0^\circ\text{C/s}\right) \lor \left(T_{\text{filtered}} \ge 45.0^\circ\text{C}\right)$$

---

## 🛠️ How to Build & Flash

1. Install [VS Code](https://code.visualstudio.com/) and the **PlatformIO** extension (or use the Arduino IDE with the ESP32 v3.0 core).
2. Clone this repository:
   ```bash
   git clone [https://github.com/hemanthviswanathan/Predictive-Thermal-Management-System-for-EV-Powertrains.git](https://github.com/hemanthviswanathan/Predictive-Thermal-Management-System-for-EV-Powertrains.git)

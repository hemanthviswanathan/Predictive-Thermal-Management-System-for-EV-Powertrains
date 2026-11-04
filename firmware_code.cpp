#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// Hardware Pin Definitions
#define RELAY_PIN 5        
Adafruit_BMP280 bmp;       

// Predictive & Signal Conditioning Parameters
const float EMA_ALPHA = 0.3;          // Smoothing factor for noise suppression
const float SLOPE_THRESHOLD = 1.0;    // Trigger pre-cooling if dT/dt >= 1.0 °C/s
const float STATIC_MAX_TEMP = 45.0;   // Hard ceiling fallback temperature in °C

// Telemetry & ISR Variables
float emaTemp = 0.0;
float previousTemp = 0.0;
volatile bool sampleFlag = false;     
hw_timer_t * timer = NULL;

// Non-blocking Hardware Timer Interrupt Handler (1 Hz)
void IRAM_ATTR onTimer() {
  sampleFlag = true; 
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Fan default OFF

  // Verify I2C Sensor Hardware Connection
  if (!bmp.begin(0x76) && !bmp.begin(0x77)) { 
    Serial.println("CRITICAL FAULT: BMP280 disconnected. Actuating Failsafe Cooling.");
    digitalWrite(RELAY_PIN, HIGH); // Force 100% cooling duty on sensor loss
    while (1); 
  }

  emaTemp = bmp.readTemperature();
  previousTemp = emaTemp;

  // Initialize Hardware Timer at 1 MHz (1 tick = 1 microsecond)
  timer = timerBegin(1000000);               
  timerAttachInterrupt(timer, &onTimer);     
  timerAlarm(timer, 1000000, true, 0);       // Trigger alarm every 1.0 second
}

void loop() {
  if (sampleFlag) {
    sampleFlag = false; 
    float rawTemp = bmp.readTemperature();

    // Hardware Open-Circuit / Fault Detection
    if (isnan(rawTemp)) {
      digitalWrite(RELAY_PIN, HIGH); 
      return; 
    }

    // 1. Exponential Moving Average (EMA) Digital Low-Pass Filter
    emaTemp = (EMA_ALPHA * rawTemp) + ((1.0 - EMA_ALPHA) * emaTemp);
    
    // 2. Discrete First-Order Derivative Calculus (dT/dt)
    float rateOfChange = emaTemp - previousTemp; 

    // 3. Boolean Dual-Condition Actuation Logic
    bool predictiveTrigger = (rateOfChange >= SLOPE_THRESHOLD);
    bool staticTrigger = (emaTemp >= STATIC_MAX_TEMP);

    if (predictiveTrigger || staticTrigger) {
      digitalWrite(RELAY_PIN, HIGH); // Preemptive Cooling Actuated
    } else {
      digitalWrite(RELAY_PIN, LOW);  // Nominal Operation
    }
    
    previousTemp = emaTemp;
  }
}
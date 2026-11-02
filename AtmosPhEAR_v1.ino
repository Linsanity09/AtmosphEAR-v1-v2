// AtmosphEAR version 1 
// Last modified: 03/15/2026
// Need install or download the header file Adafruit_VL53L0X.h for the displacement sensor.

#include <Wire.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const int fanPin = 9;
const int threshold = 40;           // Fan starts responding below this
const int fullSpeedDistance = 20;   // Full speed at this distance or closer
const int minPWM = 200;             // Minimum spin value

void setup() {
  Serial.begin(115200);
  pinMode(fanPin, OUTPUT);

  if (!lox.begin()) {
    Serial.println("VL53L0X not detected.");
    while (1);
  }

  Serial.println("System Ready.");
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus == 0) {
    int distance = measure.RangeMilliMeter;
    int pwmValue = 0;

    if (distance <= fullSpeedDistance) {
      pwmValue = 255;  // 🔥 FULL SPEED
    }
    else if (distance < threshold) {
            // Map distance between fullSpeedDistance and threshold
      pwmValue = map(distance,
                     threshold,
                     fullSpeedDistance,
                     minPWM,
                     255);
    }
    else {
      pwmValue = 0;  // Outside range → off
    }

    analogWrite(fanPin, pwmValue);
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" mm | PWM: ");
    Serial.println(pwmValue);
    } 
    else {
        analogWrite(fanPin, 0);
        Serial.println("Out of range");
    }
  delay(50);
}


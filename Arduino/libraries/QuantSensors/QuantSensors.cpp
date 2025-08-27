#include "QuantSensors.h"
#include <Arduino.h>

QuantLine::QuantLine() {

}

void QuantLine::begin() {
  initLineSensors(false);
}

void QuantLine::setLevel(uint8_t level) {
  _level = level;
}

void QuantLine::begin(bool debug) {
  initLineSensors(debug);
}

void QuantLine::initLineSensors(bool debug) {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);

  if (debug) {

    bool errors[] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t errorCounter = 0;

    for (uint8_t i = 0; i < 8; i++) {
      if (readLineSensor(i) >= 900) {
        errors[i] = true;
        errorCounter++;
      }
    }

    if (errorCounter == 0) {
      Serial.println("Line Tracker is ready to use!");
    } else if (errorCounter == 8) {
      Serial.println("Line Tracker is not working. Check the switch on the Quant.");
    } else {
      Serial.print("Line Tracker has problems with the following sensors: ");
      uint8_t temp = errorCounter;
      for (uint8_t i = 0; i < 8; i++) {
        if (errors[i]) {
          Serial.print(i);
          if (temp == 1) {
            Serial.println(".");
          } else {
            Serial.print(", ");
          }
          temp--;
        }
      }
    }
  }
}

uint16_t QuantLine::readLineSensor(uint8_t channel) {
  if (channel >= 0 && channel <= 7) {
    digitalWrite(S0, HIGH && (channel & B00000001));
    digitalWrite(S1, HIGH && (channel & B00000010));
    digitalWrite(S2, HIGH && (channel & B00000100));

    return analogRead(A0);
  }
  return 0;
}

bool QuantLine::readLineSensorBool(uint8_t channel) {
  if (channel >= 0 && channel <= 7) {
    if (readLineSensor(channel) > _level)
		return 1;
	else
		return 0;
	
  }
  return 0;
}
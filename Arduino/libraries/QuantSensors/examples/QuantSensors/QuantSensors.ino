#include <QuantSensors.h>

QuantLine line;

void setup() {
  Serial.begin(9600);
  
  line.begin(true);    // Запуск датчиків лінії з Debug в монітор порта
  line.setLevel(100);  // Встановлюємо рівень на датчиках, якщо сигнал менше ніж 100, то буде 0 відправляти  команда line.readLineSensorBool(), якщо більше 100, то line.readLineSensorBool() відправляє 1.
}

void loop() {
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print(line.readLineSensor(i));
    Serial.print("\t");
  }
  Serial.println();
  
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print(line.readLineSensorBool(i));
    Serial.print("\t");
  }
  Serial.println();
  
  delay(1000);
}

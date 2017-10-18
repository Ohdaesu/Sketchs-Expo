#include <Wire.h>

void setup() {
  Wire.begin(0,2); // join i2c bus (address optional for master)
  Serial.begin(115200);
  delay(5000);
  Serial.println("");
  Serial.println("Iniciando...");
  delay(1000);
}

byte operation = 'a';
byte pins = 4;
byte state = true;
byte level = 255;

void loop() {
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(operation);              // sends one byte
  Wire.write(pins);              // sends one byte
  Wire.write(state);              // sends one byte
  Wire.write(level);              // sends one byte
  state = false;
  Wire.endTransmission();    // stop transmitting
  Serial.println("Se envió correctamente");
  delay(2000);
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(operation);              // sends one byte
  Wire.write(pins);              // sends one byte
  Wire.write(state);              // sends one byte
  Wire.write(level);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  state = true;
  Serial.println("Se envió correctamente");
}

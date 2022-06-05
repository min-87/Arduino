#include <EEPROM.h>
int data = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter your year");
}

void loop() {
  if(Serial.available() > 0){
    data = Serial.parseInt();
    byte hi = highByte(data);
    byte low = lowByte(data);
    EEPROM.update(1, hi);
    EEPROM.update(2, low);
    Serial.println("Year saved");
  }
  byte hi = EEPROM.read(1);
  byte low = EEPROM.read(2);
  int year = word(hi, low);
  if(year != 0){
    Serial.println("Your year: " + String(year));
  }
}

#include <EEPROM.h>
String name;
int address = 0;
String savedName;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0){
    name = Serial.readString();
    EEPROM.put(address, name);
  }
  EEPROM.get(address, savedName);
  Serial.println("Your name is: " + savedName);
}

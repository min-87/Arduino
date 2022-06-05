#include <EEPROM.h>
#define RED 9
#define YELLOW 10
#define GREEN 11
#define BUT 2
#define POT A0
int brightness = 0;
volatile int switcher = 0;
int savedData = 0;
int address = 0;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  attachInterrupt(0, BUT_ISR, RISING);
}

void loop() {
  brightness = map(analogRead(POT), 0, 1023, 0, 255);
  address = switcher;
  switch(switcher){
    case 1: {
      analogWrite(RED, brightness);
      EEPROM.write(address, brightness);
    }
      break;
      case 2: {
      analogWrite(YELLOW, brightness);
      EEPROM.write(address, brightness);
    }
      break;
      case 3: {
      analogWrite(GREEN, brightness);
      EEPROM.write(address, brightness);
    }
      break;
      case 4: switcher = 0;
  }
}
void BUT_ISR(){
  switcher++;
}

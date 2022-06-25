#include <IRremote.h>
#define RECEIVE_PIN 12
#define LED1 11
#define LED2 10
#define LED3 9
#define LED4 8
#define LED5 7
#define LED6 6
#define LED7 5
#define LED8 4
#define LED9 3
#define LED10 2
IRrecv irrecv(RECEIVE_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(LED9, OUTPUT);
  pinMode(LED10, OUTPUT);
  irrecv.enableIRIn();
}

void loop()
{
 /* if(irrecv.decode(&results)){
    Serial.print("0x");
    Serial.println(results.value, HEX);
    delay(50);
    irrecv.resume();
  }*/
  if(irrecv.decode(&results)){
    switch(results.value){
      case 0xFD30CF:
          digitalWrite(LED1, !digitalRead(LED1));
          break;
        case 0xFD08F7:
          digitalWrite(LED2, !digitalRead(LED2));
          break;
        case 0xFD8877:
          digitalWrite(LED3, !digitalRead(LED3));
          break;
        case 0xFD48B7:
          digitalWrite(LED4, !digitalRead(LED4));
          break;
        case 0xFD28D7:
          digitalWrite(LED5, !digitalRead(LED5));
          break;
        case 0xFDA857:
          digitalWrite(LED6, !digitalRead(LED6));
          break;
        case 0xFD6897:
          digitalWrite(LED7, !digitalRead(LED7));
          break;
        case 0xFD18E7:
          digitalWrite(LED8, !digitalRead(LED8));
          break;
        case 0xFD9867:
          digitalWrite(LED9, !digitalRead(LED9));
          break;
        case 0xFD58A7:
          digitalWrite(LED10, !digitalRead(LED10));
          break;
    }
    irrecv.resume();
  }
}

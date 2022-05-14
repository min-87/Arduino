#include <Adafruit_NeoPixel.h>
#define led_count 24
#define led_pin 3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(led_count, led_pin, NEO_GRB + NEO_KHZ800);
bool state = false;
int counter = 0;
uint32_t red, green, blue;

void setup()
{
  strip.begin();
  Serial.begin(9600);
  red = strip.Color(255, 0, 0);
  green = strip.Color(0, 255, 0);
  blue = strip.Color(0, 0, 255);
   attachInterrupt(0, isr, CHANGE);
}

void loop()
{
  
}

void isr(){
  state = digitalRead(2);
  Serial.println(state);
  if(state == true){
    counter++;
  }
  switch(counter){
    case 1: runningLight(red); break;
    case 2: runningLight(green); break;
    case 3: runningLight(blue); break;
    case 4: counter = 0;
  }
}

void runningLight(uint32_t color){
  for(int i = 0; i < strip.numPixels()+4; i++){
      strip.setPixelColor(i, color);
      strip.setPixelColor(i - 4, 0);
      strip.show();
      delay(1000);
    }
}

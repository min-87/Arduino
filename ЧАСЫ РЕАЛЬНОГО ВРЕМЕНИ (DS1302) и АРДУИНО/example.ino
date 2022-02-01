#include <iarduino_RTC.h>

iarduino_RTC clock(RTC_DS1302, 4, 2, 3);

void setup(){                                              

  Serial.begin(9600);
  delay(300);
  clock.begin();
  clock.settime(15, 4, 6, 23, 1, 22, 6);

}    

void loop(){

  if(millis()%1000==0){
    Serial.println(clock.gettime("d-m-Y, H:i:s, D"));
    delay(1);
  }

}
#include <Servo.h>              

Servo myservo1;
Servo myservo2;

const int XAxis = A1;             
const int YAxis = A0;             

int X,Y;                          

void setup() {
 Serial.begin(9600);              
 pinMode(XAxis, INPUT);           
 myservo1.attach(2);
 pinMode(YAxis, INPUT); 
 myservo2.attach(3);

}
void loop() {

 X=map(analogRead(XAxis), 0, 1023, 180, 0);
 myservo1.write(X);               
 
 Y=map(analogRead(YAxis), 0, 1023, 0, 180);
 myservo2.write(Y);              

 Serial.print("X ");
 Serial.println(X);
 Serial.print("Y ");
 Serial.println(Y);
delay(500);
}

#include <Servo.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <FastLED.h>

#define A 32
#define B 33
#define C 34
#define D 35
#define E 36
#define F_SEG 37
#define G 38
#define H 39

// Pins driving common anodes
#define CA1 40
#define CA2 41

#define redLed 28
#define greenLed 29
#define blueLed 30

#define ledLeft 26
#define ledRight 27

#define leftLineA A4
#define rightLineA A5

#define leftLineD 3
#define rightLineD 4

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     13  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define UPDATES_PER_SECOND 100

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

LiquidCrystal_I2C lcd_3F(0x3F, 16, 2);
LiquidCrystal_I2C lcd_27(0x27, 16, 2);

Servo wheels;
Servo serv22;
Servo serv23;
Servo serv24;
Servo serv25;
  

int distance = 0;
int spd = 255;
char cmd = ' ';
int appSpd = 10;
byte testNumber = 0    ;
byte prevTestNumber = 0;

int del = 1000;
long prevTime = 0;
byte state = 0;
byte counter = 0;

int piezoPin = 31;
bool buzzerOn = false;

const int MPU_addr = 0x69; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ;

// Pins for A B C D E F G, in sequence
const int segs[7] = { A, B, C, D, E, F_SEG, G };

// Segments that make each number
const byte numbers[10] = { 0b1000000, //0
                           0b1111001, //1
                           0b0100100, //2
                           0b0110000, //3
                           0b0011001, //4
                           0b0010010, //5
                           0b0000010, //6
                           0b1111000, //7
                           0b0000000, //8
                           0b0010000  //9
                         };

#define LED_PIN     42
#define NUM_LEDS    8
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup()
{
  //LCD Display
  lcd_3F.begin();
  lcd_3F.backlight();
  lcd_27.begin();
  lcd_27.backlight();

  lcd_3F.setCursor(0, 0);
  lcd_3F.print("My address is 3F");
  lcd_27.setCursor(0, 0);
  lcd_27.print("My address is 27");

  digitalWrite( 12 , LOW );

  //Serial monitor
  Serial.begin(9600);

  //Bluetooth
  Serial3.begin(9600);

  //Steering wheel
  wheels.attach(11);

  //Servos
  serv22.attach(22);
  serv23.attach(23);
  serv24.attach(24);
  serv25.attach(25);

  //Motor driver
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

  analogWrite(5, spd);
  analogWrite(10, spd);

  //7-segment display
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F_SEG, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(H, OUTPUT);
  pinMode(CA1, OUTPUT);
  pinMode(CA2, OUTPUT);

  //Buttons
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(A7, INPUT);

  //LEDs
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

  //headlights
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);

  //buzzer
  pinMode(piezoPin, OUTPUT);

  pinMode(leftLineD, INPUT);
  pinMode(rightLineD, INPUT);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  delay(1000);
  //restoreAll();
}

void loop()
{

if (digitalRead(16)) {
    testNumber++;
    delay(500);
  }

  if (testNumber > 12) testNumber = 1;

  if (testNumber != prevTestNumber) {
    restoreAll();
  }

  switch (testNumber) {
    case 1:
      moveTest();
      break;
    case 2:
      sonarTest();
      break;
    case 3:
      tempTest();
      break;
    case 4:
      ledsTest();
      break;
    case 5:
      potentiometersTest();
      break;
    case 6:
      buttonsTest();
      break;
    case 7:
      segmentTest();
      break;
    case 8:
      ledLineTest();
      break;
    case 9:
      wheelsTest();
      break;
    case 10:
      buzzerTest();
      break;
    case 11:
      lineTest();
      break;
    case 12:
      bluetoothTest();
      break;
  }

  prevTestNumber = testNumber;

  //potentiometersTest();

}

void forward() {
  wheels.write(90);
  analogWrite(5, 255);
  analogWrite(10, 255);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
}

void backward() {
  wheels.write(90);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
}

void backwardRight() {
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  wheels.write(150);
}

void backwardLeft() {
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  wheels.write(30);
}

void forwardRight() {
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  wheels.write(150);
}

void forwardLeft() {
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  wheels.write(30);
}

void right() {
  wheels.write(160);
}

void left() {
  wheels.write(20);
}

void stp() {
  wheels.write(90);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}

void lightDigit1(byte number) {
  digitalWrite(CA1, LOW);
  digitalWrite(CA2, HIGH);
  lightSegments(number);
}

void lightDigit2(byte number) {
  digitalWrite(CA1, HIGH);
  digitalWrite(CA2, LOW);
  lightSegments(number);
}

void lightSegments(byte number) {
  for (int i = 0; i < 7; i++) {
    int bit = bitRead(number, i);
    digitalWrite(segs[i], !bit);
  }
}

void bluetoothTest() {
  if (prevTestNumber != 13) {
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("Bluetooth Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("Bluetooth Test:");
  }
  if (Serial3.available()) {
    cmd = Serial3.read();

    if (cmd == 'F') {
      forward();
    }
    else if (cmd == 'B') {
      backward();
    }
    else if (cmd == 'R') {
      right();
    }
    else if (cmd == 'L') {
      left();
    }
    else if (cmd == 'G') {
      forwardLeft();
    }
    else if (cmd == 'I') {
      forwardRight();
    }
    else if (cmd == 'H') {
      backwardLeft();
    }
    else if (cmd == 'J') {
      backwardRight();
    }
    else if (cmd == 'S') {
      stp();
    }
    else if (cmd == '1') {
      appSpd = 1;
      spd = map(1, 0, 10, 0, 255);
      analogWrite(5, spd);
      analogWrite(10, spd);
    }
    else if (cmd == '2') {
      appSpd = 2;
      spd = map(2, 0, 10, 0, 255);
      analogWrite(5, spd);
      analogWrite(10, spd);
    }
    else if (cmd == '3') {
      appSpd = 3;
      spd = map(3, 0, 10, 0, 255);
      analogWrite(5, spd);
      analogWrite(10, spd);
    }
    else if (cmd == '4') {
      appSpd = 4;
      spd = map(4, 0, 10, 0, 255);
      analogWrite(5, spd);
      analogWrite(10, spd);
    }
    else if (cmd == '5') {
      appSpd = 5;
      spd = map(5, 0, 10, 0, 255);
      analogWrite(5, spd);
      analogWrite(10, spd);
    }
    else if (cmd == '6') {
      appSpd = 6;
      spd = map(6, 0, 10, 0, 255);
      analogWrite(5, spd);
      analogWrite(10, spd);
    }
    else if (cmd == '7') {
      appSpd = 7;
      spd = map(7, 0, 10, 0, 255);
      analogWrite(5, spd);
      analogWrite(10, spd);
    }
    else if (cmd == '8') {
      appSpd = 8;
      spd = map(8, 0, 10, 0, 255);
      analogWrite(5, spd);
      analogWrite(10, spd);
    }
    else if (cmd == '9') {
      appSpd = 9;
      spd = map(9, 0, 10, 0, 255);
      analogWrite(5, spd);
      analogWrite(10, spd);
    }
    else if (cmd == 'q') {
      appSpd = 10;
      spd = map(10, 0, 10, 0, 255);
      analogWrite(5, spd);
      analogWrite(10, spd);
    }
  }

  if (appSpd < 10) {
    lightDigit2(numbers[appSpd]);
  } else {
    lightDigit1(numbers[1]);
    delay(5);
    lightDigit2(numbers[0]);
    delay(5);
  }
}

void sonarTest() {
  lcd_3F.setCursor(0, 0);
  lcd_3F.print("Sonar Test:");
  lcd_27.setCursor(0, 0);
  lcd_27.print("Sonar Test:");

  byte dist = sonar.ping_cm();
  Serial.print("Distance: ");
  Serial.print(dist); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

  lcd_3F.setCursor(0, 1);
  lcd_3F.print("Distance: " + (String)dist + " cm ");
  lcd_27.setCursor(0, 1);
  lcd_27.print("Distance: " + (String)dist + " cm ");

  delay(50);
}

void ledsTest() {
  if (state == 0) {
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("LEDs Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("LEDs Test:");

    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, LOW);
    digitalWrite(ledLeft, LOW);
    digitalWrite(ledRight, LOW);

    lcd_3F.setCursor(0, 1);
    lcd_3F.print("Red ON");
    lcd_27.setCursor(0, 1);
    lcd_27.print("Red ON");
  }
  else if (state == 1) {
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("LEDs Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("LEDs Test:");

    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(blueLed, LOW);
    digitalWrite(ledLeft, LOW);
    digitalWrite(ledRight, LOW);

    lcd_3F.setCursor(0, 1);
    lcd_3F.print("Green ON");
    lcd_27.setCursor(0, 1);
    lcd_27.print("Green ON");
  }
  else if (state == 2) {
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("LEDs Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("LEDs Test:");

    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, HIGH);
    digitalWrite(ledLeft, LOW);
    digitalWrite(ledRight, LOW);

    lcd_3F.setCursor(0, 1);
    lcd_3F.print("Blue ON");
    lcd_27.setCursor(0, 1);
    lcd_27.print("Blue ON");
  }
  else if (state == 3) {
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("LEDs Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("LEDs Test:");

    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, LOW);
    digitalWrite(ledLeft, HIGH);
    digitalWrite(ledRight, HIGH);


    lcd_3F.setCursor(0, 1);
    lcd_3F.print("Headlights ON");
    lcd_27.setCursor(0, 1);
    lcd_27.print("Headlights ON");
  }

  if ((millis() - prevTime) > del) {
    if (state < 3) {
      state++;
      lcd_3F.clear();
      lcd_27.clear();
    } else {
      state = 0;
      lcd_3F.clear();
      lcd_27.clear();
    }
    prevTime = millis();
  }
}

void tempTest() {
  lcd_3F.setCursor(0, 0);
  lcd_3F.print("Temperature Test");
  lcd_27.setCursor(0, 0);
  lcd_27.print("Temperature Test");

  int raw = analogRead(A6);
  int tem = ( raw / 1023.0 ) * 5.0 * 1000 / 10;

  lcd_3F.setCursor(0, 1);
  lcd_3F.print("Temp: " + (String)tem + "*C ");
  lcd_27.setCursor(0, 1);
  lcd_27.print("Temp: " + (String)tem + "*C ");

  delay(20);
}

void buttonsTest() {
  lcd_3F.setCursor(0, 0);
  lcd_3F.print("Buttons Test:");
  lcd_27.setCursor(0, 0);
  lcd_27.print("Buttons Test:");

  lcd_3F.setCursor(0, 1);
  lcd_3F.print("L:" + (String)digitalRead(16) + " M:" + (String)digitalRead(17) + " R:" + (String)digitalRead(A7));
  lcd_27.setCursor(0, 1);
  lcd_27.print("L:" + (String)digitalRead(16) + " M:" + (String)digitalRead(17) + " R:" + (String)digitalRead(A7));

  delay(20);
}

void potentiometersTest() {
  lcd_3F.setCursor(0, 0);
  lcd_3F.print("VarRes Test:");
  lcd_27.setCursor(0, 0);
  lcd_27.print("VarRes Test:");

  lcd_3F.setCursor(0, 1);
  lcd_3F.print((String)analogRead(A0) + "|" + (String)analogRead(A1) + "|" + (String)analogRead(A2) + "|" + (String)analogRead(A3) + "           ");
  lcd_27.setCursor(0, 1);
  lcd_27.print((String)analogRead(A0) + "|" + (String)analogRead(A1) + "|" + (String)analogRead(A2) + "|" + (String)analogRead(A3) + "           ");
}

void segmentTest() {
  if (digitalRead(17)) {
    state++;
    delay(500);
  }

  if (state > 1) {
    state = 0;
  }

  if (state == 0) {
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("7 Segment Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("7 Segment Test:");

    lcd_3F.setCursor(0, 1);
    lcd_3F.print("All leds ON");
    lcd_27.setCursor(0, 1);
    lcd_27.print("All leds ON");

    digitalWrite(40, LOW);
    digitalWrite(41, LOW);
    digitalWrite(32, HIGH);
    digitalWrite(33, HIGH);
    digitalWrite(34, HIGH);
    digitalWrite(35, HIGH);
    digitalWrite(36, HIGH);
    digitalWrite(37, HIGH);
    digitalWrite(38, HIGH);
    digitalWrite(39, HIGH);
  }
  else if (state == 1) {
    digitalWrite(39, LOW);
    if ((millis() - prevTime) > del) {
      counter++;
      prevTime = millis();

      lcd_3F.setCursor(0, 0);
      lcd_3F.print("7 Segment Test:");
      lcd_27.setCursor(0, 0);
      lcd_27.print("7 Segment Test:");

      lcd_3F.setCursor(0, 1);
      lcd_3F.print("Counter: " + (String)counter + " ");
      lcd_27.setCursor(0, 1);
      lcd_27.print("Counter: " + (String)counter + " ");
    }

    lightDigit1(numbers[counter / 10]);
    delay(5);
    lightDigit2(numbers[counter % 10]);
    delay(5);
  }

}

void moveTest() {
  if (digitalRead(17)) {
    state++;
    delay(500);
  }

  if (state > 2) {
    state = 0;
  }

  if (state == 0) {
    stp();
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("Move Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("Move Test:");
    lcd_3F.setCursor(0, 1);
    lcd_3F.print("Stop      ");
    lcd_27.setCursor(0, 1);
    lcd_27.print("Stop      ");
  }
  else if (state == 1) {
    forward();
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("Move Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("Move Test:");
    lcd_3F.setCursor(0, 1);
    lcd_3F.print("Forward");
    lcd_27.setCursor(0, 1);
    lcd_27.print("Forward");
  }
  else if (state == 2) {
    backward();
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("Move Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("Move Test:");
    lcd_3F.setCursor(0, 1);
    lcd_3F.print("Backward");
    lcd_27.setCursor(0, 1);
    lcd_27.print("Backward");
  }
}

void restoreAll() {
  for (int i = 0; i < 54; i++) {
    digitalWrite(i, LOW);
  }

  del = 1000;
  prevTime = millis();
  state = 0;
  counter = 0;

  lcd_3F.clear();
  lcd_27.clear();

  uint8_t brightness = 0;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = 0;
  }
  FastLED.show();

  wheels.write(90);
}

void ledLineTest() {
  lcd_3F.setCursor(0, 0);
  lcd_3F.print("LED Line Test:");
  lcd_27.setCursor(0, 0);
  lcd_27.print("LED Line Test:");

  ChangePalettePeriodically();

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 10)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 15)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 20)  {
      SetupPurpleAndGreenPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 25)  {
      SetupTotallyRandomPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 30)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = NOBLEND;
    }
    if ( secondHand == 35)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 40)  {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 45)  {
      currentPalette = PartyColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 50)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 55)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = LINEARBLEND;
    }
  }
}

void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

void SetupBlackAndWhiteStripedPalette()
{
  fill_solid( currentPalette, 16, CRGB::Black);

  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}


const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

void wheelsTest() {
  if (state == 0) {
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("Wheels Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("Wheels Test:");
    lcd_3F.setCursor(0, 1);
    lcd_3F.print("Forward");
    lcd_27.setCursor(0, 1);
    lcd_27.print("Forward");

    wheels.write(90);
  }
  else if (state == 1) {
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("Wheels Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("Wheels Test:");
    lcd_3F.setCursor(0, 1);
    lcd_3F.print("Left");
    lcd_27.setCursor(0, 1);
    lcd_27.print("Left");

    wheels.write(10);
  }
  else if (state == 2) {
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("Wheels Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("Wheels Test:");
    lcd_3F.setCursor(0, 1);
    lcd_3F.print("Right");
    lcd_27.setCursor(0, 1);
    lcd_27.print("Right");

    wheels.write(170);
  }

  if ((millis() - prevTime) > del) {
    if (state < 2) {
      state++;
      lcd_3F.clear();
      lcd_27.clear();
    } else {
      state = 0;
      lcd_3F.clear();
      lcd_27.clear();
    }
    prevTime = millis();
  }
}

void buzzerTest() {
  if (digitalRead(17)) {
    if (!buzzerOn) {
      lcd_3F.setCursor(0, 0);
      lcd_3F.print("Buzzer Test:");
      lcd_27.setCursor(0, 0);
      lcd_27.print("Buzzer Test:");
      lcd_3F.setCursor(0, 1);
      lcd_3F.print("Buzzer ON ");
      lcd_27.setCursor(0, 1);
      lcd_27.print("Buzzer ON ");
      buzzerOn = true;
    }

    digitalWrite(piezoPin, HIGH);
    delay(1);
    digitalWrite(piezoPin, LOW);
    delay(1);
  } else {
    lcd_3F.setCursor(0, 0);
    lcd_3F.print("Buzzer Test:");
    lcd_27.setCursor(0, 0);
    lcd_27.print("Buzzer Test:");
    lcd_3F.setCursor(0, 1);
    lcd_3F.print("Buzzer OFF");
    lcd_27.setCursor(0, 1);
    lcd_27.print("Buzzer OFF");
    buzzerOn = false;
  }
}

void gyroTest() {
  lcd_3F.setCursor(0, 0);
  lcd_3F.print("Gyro Test:");
  lcd_27.setCursor(0, 0);
  lcd_27.print("Gyro Test:");

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)

  lcd_3F.setCursor(0, 1);
  lcd_3F.print("X:" + (String)map(AcX, -16200, 16200, -180, 180) + "  ");
  lcd_3F.setCursor(8, 1);
  lcd_3F.print("Y:" + (String)map(AcY, -16200, 16200, -180, 180) + "  ");
  lcd_27.setCursor(0, 1);
  lcd_27.print("X:" + (String)map(AcX, -16200, 16200, -180, 180) + "  ");
  lcd_27.setCursor(8, 1);
  lcd_27.print("Y:" + (String)map(AcY, -16200, 16200, -180, 180) + "  ");

  delay(20);
}

void lineTest() {
  lcd_3F.setCursor(0, 0);
  lcd_3F.print("LineTrack Test:");
  lcd_27.setCursor(0, 0);
  lcd_27.print("LineTrack Test:");

  lcd_27.setCursor(0, 1);
  lcd_27.print("L:" + (String)analogRead(leftLineA));
  lcd_27.setCursor(5, 1);
  lcd_27.print("(" + (String)digitalRead(leftLineD) + ")");
  lcd_27.setCursor(8, 1);
  lcd_27.print("L:" + (String)analogRead(rightLineA));
  lcd_27.setCursor(13, 1);
  lcd_27.print("(" + (String)digitalRead(rightLineD) + ")");

  lcd_3F.setCursor(0, 1);
  lcd_3F.print("R:" + (String)analogRead(leftLineA));
  lcd_3F.setCursor(5, 1);
  lcd_3F.print("(" + (String)digitalRead(leftLineD) + ")");
  lcd_3F.setCursor(8, 1);
  lcd_3F.print("R:" + (String)analogRead(rightLineA));
  lcd_3F.setCursor(13, 1);
  lcd_3F.print("(" + (String)digitalRead(rightLineD) + ")");

  delay(20);
}

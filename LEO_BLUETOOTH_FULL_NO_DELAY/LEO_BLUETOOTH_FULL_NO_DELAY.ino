#include <Servo.h>
#include <FastLED.h>
#include <TimerFreeTone.h>
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

int distance = 0;
int spd = 255;
char cmd = ' ';
int appSpd;

Servo serv;

#define A 32
#define B 33
#define C 34
#define D 35
#define E 36
#define F_SEG 37
#define G 38

// Pins driving common anodes
#define CA1 40
#define CA2 41

// Pins for A B C D E F G, in sequence
const int segs[7] = { A, B, C, D, E, F_SEG, G };

// Segments that make each number
const byte numbers[10] = { 0b1000000, 0b1111001, 0b0100100, 0b0110000, 0b0011001, 0b0010010,
                           0b0000010, 0b1111000, 0b0000000, 0b0010000
                         };
bool state = false;
int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int duration[] = { 250, 125, 125, 250, 250, 250, 250, 250 };
void setup()
{
  digitalWrite( 12 , LOW );
  Serial.begin(9600);
  Serial3.begin(9600);

  serv.attach(11);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(26, OUTPUT);//left
  pinMode(27, OUTPUT);//right

  pinMode(28, OUTPUT);//r
  pinMode(29, OUTPUT);//g
  pinMode(30, OUTPUT);//b

  pinMode(31, OUTPUT);//zoomer

  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F_SEG, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(CA1, OUTPUT);
  pinMode(CA2, OUTPUT);

  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

  analogWrite(5, spd);
  analogWrite(10, spd);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}

void loop()
{

  if (Serial3.available()) {
    cmd = Serial3.read();
    Serial.println(cmd);
    switch (cmd) {
      case 'S': {
          stp();
        } break;
      case 'F': {
          forward();
          backwardlight(1);
        } break;
      case 'B': {
          backward();
          backwardlight(2);
        } break;
      case 'R': {
          right();
          rightlight();

        } break;
      case 'L': {
          left();
          leftlight();
        } break;
      case 'G': {
          forwardLeft();
        } break;
      case 'I': {
          forwardRight();
        } break;
      case 'H': {
          backwardLeft();
          backwardlight(2);
        } break;
      case 'J': {
          backwardRight();
          backwardlight(2);
        } break;
      case 'W': {
          forwardlight(true);
        } break;
      case 'w': {
          forwardlight(false);
        } break;
      case 'U': {
          backwardlight(2);
        } break;
      case 'u': {
          backwardlight(1);
        } break;
      case 'V': {
          alert(true);
        } break;
      case 'v': {
          alert(false);
        } break;
      case 'X': {
          state = true;
        } break;
      case 'x': {
          state = false;
        } break;
      case '1': {
          appSpd = 1;
          spd = map(1, 0, 10, 0, 255);
          analogWrite(5, spd);
          analogWrite(10, spd);
        } break;
      case '2': {
          appSpd = 2;
          spd = map(2, 0, 10, 0, 255);
          analogWrite(5, spd);
          analogWrite(10, spd);
        } break;
      case '3': {
          appSpd = 3;
          spd = map(3, 0, 10, 0, 255);
          analogWrite(5, spd);
          analogWrite(10, spd);
        } break;
      case '4': {
          appSpd = 4;
          spd = map(4, 0, 10, 0, 255);
          analogWrite(5, spd);
          analogWrite(10, spd);
        } break;
      case '5': {
          appSpd = 5;
          spd = map(5, 0, 10, 0, 255);
          analogWrite(5, spd);
          analogWrite(10, spd);
        } break;
      case '6': {
          appSpd = 6;
          spd = map(6, 0, 10, 0, 255);
          analogWrite(5, spd);
          analogWrite(10, spd);
        } break;
      case '7': {
          appSpd = 7;
          spd = map(7, 0, 10, 0, 255);
          analogWrite(5, spd);
          analogWrite(10, spd);
        } break;
      case '8': {
          appSpd = 8;
          spd = map(8, 0, 10, 0, 255);
          analogWrite(5, spd);
          analogWrite(10, spd);
        } break;
      case '9': {
          appSpd = 9;
          spd = map(9, 0, 10, 0, 255);
          analogWrite(5, spd);
          analogWrite(10, spd);
        } break;
      case 'q': {
          appSpd = 10;
          spd = map(10, 0, 10, 0, 255);
          analogWrite(5, spd);
          analogWrite(10, spd);
        } break;
    }
    avarijka(state);
    if (appSpd < 10) {
      lightDigit2(numbers[appSpd]);
    } else {
      lightDigit1(numbers[1]);
      delay(5);
      lightDigit2(numbers[0]);
      delay(5);
    }
  }
}

void leftlight()
{
  for ( int i = 4; i < NUM_LEDS; i++) {
    leds[i] = CRGB( 139, 23, 0);
    FastLED.show();
  }
  for ( int i = 0; i < NUM_LEDS / 2; i++) {
    leds[i] = CRGB( 0, 0, 0);
    FastLED.show();
  }
}

void rightlight()
{
  for ( int i = 0; i < NUM_LEDS / 2; i++) {
    leds[i] = CRGB( 252, 69, 8);
    FastLED.show();
  }
  for ( int i = 4; i < NUM_LEDS; i++) {
    leds[i] = CRGB( 0, 0, 0);
    FastLED.show();
  }
}
void forwardlight(bool s) {
  digitalWrite(27, s);
  digitalWrite(26, s);
}

void backwardlight(int color) {
  for ( int i = 0; i < NUM_LEDS; i++) {
    if (color == 2) {
      leds[i] = CRGB::Red;
      FastLED.show();
      //      FastLED.delay(10);
    }
    else {
      leds[i] = CRGB::Black;
      FastLED.show();
    }

  }
}

void alert(bool s) {
  if (s == true) {
    // TimerFreeTone(31, 1000, 500);
    // delay(500);
    for (int thisNote = 0; thisNote < 8; thisNote++) { // Loop through the notes in the array.
      TimerFreeTone(31, melody[thisNote], duration[thisNote]); // Play thisNote for duration.
      //    delay(50); // Short delay between notes.
    }
  }
  else {
    noTone(31);
  }
}

unsigned long time;
bool st;
void avarijka(bool s) {
  int cont = 27;
  if (s == true) {
    if (millis() - time > 500)
    {
      time = millis();
      st = !st;
      cont++;
    }
    if (cont > 30) {
      cont = 27;
    }
    digitalWrite(cont, st);
  }
  else {
    digitalWrite(28, 0);
    digitalWrite(29, 0);
    digitalWrite(30, 0);
  }
}

void forward() {
  serv.write(90);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
}

void backward() {
  serv.write(90);
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
  serv.write(150);
}

void backwardLeft() {
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  serv.write(30);
}

void forwardRight() {

  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  serv.write(150);
}

void forwardLeft() {
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  serv.write(30);
}

void right() {
  serv.write(160);
}

void left() {
  serv.write(20);
}

void stp() {
  serv.write(90);
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

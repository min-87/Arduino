#include <LiquidCrystal_I2C.h>
#include <FastLED.h>
#include <DFMiniMp3.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define NUM_LEDS 2
#define DATA_PIN 42

#define TRIGGER_PIN  12
#define ECHO_PIN     13
#define MAX_DISTANCE 200

#define DHTPIN 4
#define DHTTYPE DHT22

#define MOTOR_A_IN_1 9
#define MOTOR_A_IN_2 8
#define MOTOR_B_IN_1 7
#define MOTOR_B_IN_2 6

Servo myservo;

int pos = 130;
int flag = 0;

int leftSpeed = 100,
    rightSpeed = 100;

char cmd;

CRGB leds[NUM_LEDS];

LiquidCrystal_I2C lcd(0x20, 16, 2);

DHT_Unified dht(DHTPIN, DHTTYPE);

class Mp3Notify;
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3;
DfMp3 dfmp3(Serial1);

class Mp3Notify
{
  public:
    static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
    {
      if (source & DfMp3_PlaySources_Sd)
      {
        Serial.print("SD Card, ");
      }
      if (source & DfMp3_PlaySources_Usb)
      {
        Serial.print("USB Disk, ");
      }
      if (source & DfMp3_PlaySources_Flash)
      {
        Serial.print("Flash, ");
      }
      Serial.println(action);
    }
    static void OnError(DfMp3& mp3, uint16_t errorCode)
    {
      // see DfMp3_Error for code meaning
      Serial.println();
      Serial.print("Com Error ");
      Serial.println(errorCode);
    }
    static void OnPlayFinished(DfMp3& mp3, DfMp3_PlaySources source, uint16_t track)
    {
      Serial.print("Play finished for #");
      Serial.println(track);
    }
    static void OnPlaySourceOnline(DfMp3& mp3, DfMp3_PlaySources source)
    {
      PrintlnSourceAction(source, "online");
    }
    static void OnPlaySourceInserted(DfMp3& mp3, DfMp3_PlaySources source)
    {
      PrintlnSourceAction(source, "inserted");
    }
    static void OnPlaySourceRemoved(DfMp3& mp3, DfMp3_PlaySources source)
    {
      PrintlnSourceAction(source, "removed");
    }
};

void setup() {
  initAllPeriphery();
  greetingWalle();
  delay(2000);
}
String ip = "";

void loop() {
  if (Serial3.available()) {
    char temp = Serial3.read();
    Serial.println(temp);
    if (temp == 'I' && ip == "") {
      while (true) {
        temp = Serial3.read();
        if (temp != 'P' && (temp > 47 && temp < 58 || temp == '.')) {
          ip += temp;
        } else {
          break;
        }
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Plese, setup me:");
      lcd.setCursor(0, 1);
      lcd.print(ip);
      Serial.println(ip);
      delay(1000);
    }
    if (temp == 'L') {
      lcd.clear();
      lcd.print("Okey");
      delay(1000);
      digitalWrite(34, HIGH);
      delay(1000);
    } else if (temp == 'Q') {
      lcd.clear();
      lcd.print("No problems");
      delay(1000);
      digitalWrite(34, LOW);
      delay(1000);
    } else if (temp == 'S') {
      lcd.clear();
      lcd.print("Speak");
    } else if (temp == 'R') {
      lcd.clear();
      lcd.print("Few seconds, ");
      lcd.setCursor(0, 1);
      lcd.print("please");
    }
    else if (temp == 'T') {
      int temperature;
      sensors_event_t event;

      dht.temperature().getEvent(&event);
      if (isnan(event.temperature)) {
        lcd.clear();
        lcd.print("Temperature error");
      }
      else {
        temperature = event.temperature;
        dfmp3.playFolderTrack16(1, 5);
        delay(2500);
        sayNumber(temperature);
        dfmp3.playFolderTrack16(1, 2);
        delay(1500);
        dfmp3.playFolderTrack16(1, 6);
        delay(2300);
        lcd.clear();
        dfmp3.playFolderTrack16(1, 7);
        lcdPrint("Temp: " + String(temperature) + "*C");
        delay(5000);
      }
    } else if (temp == 'F') {
      setMotorsSpeed(leftSpeed, rightSpeed);
      delay(1000);
      setMotorsSpeed(0, 0);
    } else if (temp == 'B') {
      setMotorsSpeed(-leftSpeed, -rightSpeed);
      delay(1000);
      setMotorsSpeed(0, 0);
    } else if (temp == 'r') {
      setMotorsSpeed(leftSpeed, -rightSpeed);
      delay(1000);
      setMotorsSpeed(0, 0);
    } else if (temp == 'l') {
      setMotorsSpeed(-leftSpeed, rightSpeed);
      delay(1000);
      setMotorsSpeed(0, 0);
    } else if (temp == 'N') {
      dfmp3.playFolderTrack16(3, 1);
    } else if (temp == 'f') {
      dfmp3.playFolderTrack16(3, 2);
      delay(5000);
    } else if (temp == 't') {
      humor();
    } else if (temp == 'n') {
      dfmp3.playFolderTrack16(3, 6);
      delay(2000);
    } else if (temp == 'w') {
      dfmp3.playFolderTrack16(3, 3);
      delay(4000);
    }
  }

  if (Serial2.available()) {
    cmd = Serial2.read();
    Serial.println(cmd);
    if (cmd == 'S') {
      setMotorsSpeed(0, 0);
    }
    else if (cmd == 'F') {
      setMotorsSpeed(leftSpeed, rightSpeed);
    }
    else if (cmd == 'B') {
      setMotorsSpeed(-leftSpeed, -rightSpeed);
    }
    else if (cmd == 'L') {
      setMotorsSpeed(-leftSpeed, rightSpeed);
    }
    else if (cmd == 'R') {
      setMotorsSpeed(leftSpeed, -rightSpeed);
    }
    else if (cmd == 'I') {
      setMotorsSpeed(leftSpeed, 0);
    }
    else if (cmd == 'G') {
      setMotorsSpeed(0, rightSpeed);
    }
    else if (cmd == 'H') {
      setMotorsSpeed(0, -rightSpeed);
    }
    else if (cmd == 'J') {
      setMotorsSpeed(-leftSpeed, 0);
    }
    else if (cmd == 'X') {

    }
    else if (cmd == 'x') {

    }
    else if (cmd == 'w') {
      digitalWrite(34, LOW);
    }
    else if (cmd == 'W') {
      digitalWrite(34, HIGH);
    }
    else if (cmd == 'u') {

    }
    else if (cmd == 'U') {

    }
    else if (cmd == 'v') {
      dfmp3.playFolderTrack16(1, 1);
      delay(700);
      for (int i = 0; i < 16; i++) {
        if (i % 3 == 0 && i != 15) {
          myservo.write(random(110, 150));
        }

        if (i == 15) {
          myservo.write(130);
        }

        leds[0] = CRGB(0, 255 - map(i, 0, 15, 0, 255), map(i, 0, 15, 0, 255));
        leds[1] = CRGB(0, 255 - map(i, 0, 15, 0, 255), map(i, 0, 15, 0, 255));
        FastLED.show();
        delay(60);
      }
    }
    else if (cmd == 'V') {
      dfmp3.playFolderTrack16(1, 1);
      delay(700);
      for (int i = 0; i < 16; i++) {
        if (i % 3 == 0 && i != 15) {
          myservo.write(random(110, 150));
        }

        if (i == 15) {
          myservo.write(130);
        }

        leds[0] = CRGB(0, 255 - map(i, 0, 15, 0, 255), map(i, 0, 15, 0, 255));
        leds[1] = CRGB(0, 255 - map(i, 0, 15, 0, 255), map(i, 0, 15, 0, 255));
        FastLED.show();
        delay(60);
      }
    }
    else if (cmd == '1') {
      leftSpeed = map(1, 1, 10, 0, 255);
      rightSpeed = map(1, 1, 10, 0, 255);
    }
    else if (cmd == '2') {
      leftSpeed = map(2, 1, 10, 0, 255);
      rightSpeed = map(2, 1, 10, 0, 255);
    }
    else if (cmd == '3') {
      leftSpeed = map(3, 1, 10, 0, 255);
      rightSpeed = map(3, 1, 10, 0, 255);
    }
    else if (cmd == '4') {
      leftSpeed = map(4, 1, 10, 0, 255);
      rightSpeed = map(4, 1, 10, 0, 255);
    }
    else if (cmd == '5') {
      leftSpeed = map(5, 1, 10, 0, 255);
      rightSpeed = map(5, 1, 10, 0, 255);
    }
    else if (cmd == '6') {
      leftSpeed = map(6, 1, 10, 0, 255);
      rightSpeed = map(6, 1, 10, 0, 255);
    }
    else if (cmd == '7') {
      leftSpeed = map(7, 1, 10, 0, 255);
      rightSpeed = map(7, 1, 10, 0, 255);
    }
    else if (cmd == '8') {
      leftSpeed = map(8, 1, 10, 0, 255);
      rightSpeed = map(8, 1, 10, 0, 255);
    }
    else if (cmd == '9') {
      leftSpeed = map(9, 1, 10, 0, 255);
      rightSpeed = map(9, 1, 10, 0, 255);
    }
    else if (cmd == 'q') {
      leftSpeed = map(10, 1, 10, 0, 255);
      rightSpeed = map(10, 1, 10, 0, 255);
    }
  }
}

void humor() {
  dfmp3.playFolderTrack16(3, 4);
  delay(1500);
  for (int i = 0; i < 10; i++) {
    leds[0] = CRGB(0, 255 - random(0, 200), 0);
    leds[1] = CRGB(0, 255 - random(0, 200), 0);
    FastLED.show();
    delay(random(20, 100));
    leds[0] = CRGB(0, 0, 0);
    leds[1] = CRGB(0, 0, 0);
    FastLED.show();
    delay(random(20, 100));
  }
  delay(2000);
  leds[0] = CRGB(255, 0, 0);
  leds[1] = CRGB(255, 0, 0);
  FastLED.show();
  delay(100);
  dfmp3.playFolderTrack16(3, 5);
  delay(2000);
}

void lcdPrint(String str) {
  for (int i = 0; i < str.length(); i++) {
    lcd.setCursor(i, 0);
    lcd.print(str.charAt(i));
    delay(100);
  }
}

void sayNumber(int num) {
  int num1 = 0;
  int num2 = 0;
  if (num < 20) {
    num2 = num;
  } else {
    num2 = num % 10;
    num1 = num / 10;
  }
  if (num1 != 0) {
    dfmp3.playFolderTrack16(2, num1 * 10);
    delay(600);
  }

  dfmp3.playFolderTrack16(2, num2);
  delay(700);
}

void initAllPeriphery() {
  randomSeed(analogRead(0));
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  myservo.attach(2);
  delay(2000);
  dfmp3.begin();
  dfmp3.setVolume(25);

  pinMode (34, OUTPUT);

  pinMode (6, OUTPUT);
  pinMode (7, OUTPUT);
  pinMode (8, OUTPUT);
  pinMode (9, OUTPUT);

  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void greetingWalle() {
  dfmp3.playFolderTrack16(1, 1);
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 0);
    lcd.print("#");
    lcd.setCursor(i, 1);
    lcd.print("#");

    if (i % 3 == 0 && i != 15) {
      myservo.write(random(110, 150));
    }

    if (i == 15) {
      myservo.write(130);
    }

    leds[0] = CRGB(0, 255 - map(i, 0, 15, 0, 255), map(i, 0, 15, 0, 255));
    leds[1] = CRGB(0, 255 - map(i, 0, 15, 0, 255), map(i, 0, 15, 0, 255));
    FastLED.show();
    delay(60);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Hi! My name is");
  lcd.setCursor(5, 1);
  lcd.print("Wall-e");
}

void setMotorsSpeed(int left, int right) {
  if (left > 255) left = 255;
  if (left < -255) left = -255;
  if (right > 255) right = 255;
  if (right < -255) right = -255;

  if (left < 0) {
    left *= -1;
    analogWrite(MOTOR_A_IN_1, left);
    analogWrite(MOTOR_A_IN_2, 0);
  } else {
    analogWrite(MOTOR_A_IN_1, 0);
    analogWrite(MOTOR_A_IN_2, left);
  }

  if (right < 0) {
    right *= -1;
    analogWrite(MOTOR_B_IN_1, right);
    analogWrite(MOTOR_B_IN_2, 0);
  } else {
    analogWrite(MOTOR_B_IN_1, 0);
    analogWrite(MOTOR_B_IN_2, right);
  }
}

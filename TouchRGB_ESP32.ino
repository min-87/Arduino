int  threshold = 40;
volatile unsigned int color = 1;
volatile int brightness = 255;
volatile unsigned long timer = 0;
volatile unsigned long changeLastTouch = 0;

void changeColor() {
  timer = millis ();

  if (timer - changeLastTouch > 200) {
    color++;
    if (color > 4) color = 1;
    changeLastTouch = timer;
  }
}
void brightnessHigh() {
  brightness += 5;
  if (brightness > 255) brightness = 255;
}
void brightnessLow() {
  brightness -= 5;
  if (brightness < 0) brightness = 0;
}
void setup() {
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  touchAttachInterrupt(4, changeColor, threshold );
  touchAttachInterrupt(13, brightnessHigh, threshold);
  touchAttachInterrupt(15, brightnessLow, threshold );
  ledcSetup(0, 5000, 8);
  ledcSetup(1, 5000, 8);
  ledcSetup(2, 5000, 8);
  ledcAttachPin(25, 0);
  ledcAttachPin(26, 1);
  ledcAttachPin(27, 2);


}

void loop() {
  selectColor();

}
void  selectColor() {
  if (color == 1) {
    ledcWrite(0, brightness);
    ledcWrite(1, brightness);
    ledcWrite(2, brightness);
  } else if (color == 2) {
    ledcWrite(0, brightness);
    ledcWrite(1, 0);
    ledcWrite(2, 0);
  } else if (color == 3) {
    ledcWrite(0, 0);
    ledcWrite(1, brightness);
    ledcWrite(2, 0);
  } else if (color == 4) {
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, brightness);
  }
}

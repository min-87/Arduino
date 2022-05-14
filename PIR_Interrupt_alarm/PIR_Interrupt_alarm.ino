volatile bool state = false;
volatile bool alarm = false;
  
void setup()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  attachInterrupt(0, PIR_ISR, CHANGE);
  attachInterrupt(1, BUT_ISR, RISING);
  Serial.begin(9600);
}

void loop()
{
  if(alarm == true && state == true){
    buzzer();
  }
  alarm = false;
}

void BUT_ISR(){
  state = !state;
}
void PIR_ISR(){
  alarm = true;
  Serial.println(alarm);
}
void buzzer(){
  for(int i = 0; i < 13; i++){
    digitalWrite(13, HIGH);
    tone(12, 800, 100);
    delay(300);
    noTone(12);
    digitalWrite(13, LOW);
    delay(200);
  }
}

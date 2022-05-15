long nowTime = 0;
long previousTime = 0;
long previousTime2 = 0;
int delayTime = 1000;
int delayTime2 = 100;

void setup()
{
  pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
}

void loop()
{
  nowTime = millis();
  if(nowTime - previousTime > delayTime){
    previousTime = millis();
    if(digitalRead(13) == HIGH){
      digitalWrite(13, LOW);
    }else if(digitalRead(13) == LOW){
      digitalWrite(13, HIGH);
    }
  }
  
  if(nowTime - previousTime2 > delayTime2){
    previousTime2 = millis();
    if(digitalRead(12) == HIGH){
      digitalWrite(12, LOW);
    }else if(digitalRead(12) == LOW){
      digitalWrite(12, HIGH);
    }
  }
}

#define TRIG 10
#define ECHO 9
#define RELE 5
int duration = 0;
int distance = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(RELE, OUTPUT);
}

void loop()
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(2);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if(distance < 50){
    digitalWrite(RELE, HIGH);
  }else{
    digitalWrite(RELE, LOW);
  }
}

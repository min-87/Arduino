int value;
void setup()
{
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop()
{
  int gas = analogRead(1);
  Serial.println(gas);
  value = map(gas, 300, 750, 0, 100);
  if(value <= 30){
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  }else if(value <= 60){
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
  }else{
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
  }
}

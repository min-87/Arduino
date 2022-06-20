int input;
int remainder;
int result[7];
int index;
int pins[7] = {3, 4, 5, 6, 7, 8, 9};
void setup() {
  for(int i = 0; i < 7; i++){
    pinMode(pins[i], OUTPUT);
  }
  Serial.begin(9600);
  Serial.println("Enter your decimal number");
}

void loop() {
  if(Serial.available() > 0){
    input = Serial.parseInt();
    Serial.print("You entered decimal number: ");
    Serial.println(input);
    Serial.print("Your result in bin: ");
    index = 6;
    while(input > 0){
      remainder = input % 2;
      input = input / 2;
      result[index] = remainder;
      index--;
    }
    for(int i = 0; i < 7; i++){
      Serial.print(result[i]);
      digitalWrite(pins[i], result[i]);
    }
  }
}

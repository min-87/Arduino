int count = 0;
void setup() {
  Serial.begin(9600);

}

void loop() {
  if(Serial.available()){
    count = Serial.parseInt();
    int numbers[count];
    for(int i =0; i < count; i++){
      numbers[i] = random(0, 1001);
      Serial.print(numbers[i]);
      Serial.print(" ");
    }
    Serial.println();

    for(int i = 0; i < count; i++){
      for(int j = 0; j < count - 1; j++){
        if(numbers[j] > numbers[j+1]){
          int temp = numbers[j];
          numbers[j] = numbers[j+1];
          numbers[j+1] = temp;
        }
      }
    }
    for(int i = 0; i < count; i++){
      Serial.print(numbers[i]);
      Serial.print(" ");
    }

    Serial.println();

    for(int i = 0; i < count; i++){
      for(int j = 0; j < count - i; j++){
        if(numbers[j] < numbers[j+1]){
          int temp = numbers[j];
          numbers[j] = numbers[j+1];
          numbers[j+1] = temp;
        }
      }
    }
    for(int i = 0; i < count; i++){
      Serial.print(numbers[i]);
      Serial.print(" ");
    }
    
  }

}

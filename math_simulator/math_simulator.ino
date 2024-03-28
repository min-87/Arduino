int first_number, operation, second_number, answer, score;
void setup() {
randomSeed(analogRead(0));
score =0;
Serial.begin(9600);
}
int game() {
  first_number =random(1,101);
  operation = random(1,5);
  second_number =random(1,101);
if (operation == 1){
  Serial.print(first_number);
  Serial.print("+");
  Serial.print(second_number);
  Serial.println("=?");
  do {
    answer = Serial.parseInt();
  }while(answer != first_number + second_number);
  return 20; 
}
if (operation == 2){
  Serial.print(first_number);
  Serial.print("-");
  Serial.print(second_number);
  Serial.println("=?");
  do {
    answer = Serial.parseInt();
  }while(answer != first_number - second_number);
  return 20; 
}
if (operation == 3){
  Serial.print(first_number);
  Serial.print("*");
  Serial.print(second_number);
  Serial.println("=?");
  do {
    answer = Serial.parseInt();
  }while(answer != first_number * second_number);
  return 20; 
}
if (operation == 4){
  Serial.print(first_number);
  Serial.print(":");
  Serial.print(second_number);
  Serial.println("=?");
  do {
    answer = Serial.parseInt();
  }while(answer != first_number / second_number);
  return 20; 
}
}
void loop() {
score += game();
Serial.print("Score:");
Serial.println(score);
}

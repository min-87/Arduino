#include <Servo.h> 

Servo myservo1;
Servo myservo2;

  int a,b;                          // Переменные для хранения текущего положения
  int X,Y;                          // Переменные для хранения значений по оси X и Y
  const int XAxis = A1;             // Нижний серво ось X
  const int YAxis = A0;             // Верхний серво ось Y
  const int Button = 4;             // Объявление pin кнопки
  const int Led = 13;               // Объявление pin светодиод
  int flag = 1;
 
void setup() {
 Serial.begin(9600);              
 pinMode(XAxis, INPUT);           
 myservo1.attach(2);
 pinMode(YAxis, INPUT); 
 myservo2.attach(3);
 pinMode (Button,INPUT_PULLUP);
 pinMode(Led, OUTPUT);
}

void loop() {
 
   a=myservo1.read();               // Считываем текущее положение по оси X
   b=myservo2.read();               // Считываем текущее положение по оси Y
   
   X=map(analogRead(XAxis), 0, 1023, 0, 180);           // Переводим значение из 0-1023 в градусы от 0 до 180 по оси X
   if(X>95){                                            // Если значение больше середины по оси X
   a=a-1;                                               // Уменьшаем значение на 1 градус
   myservo1.write(a); 
   delay(100);                                          // Пауза чтобы мотор успел переместиться. Скорость и плавность движения
   }
   if(X<90){                                            // Если значение меньше середины по оси X
   a=a+1;                                               // Увеличиваем значение на 1 градус
   myservo1.write(a);
   delay(100);                                          // Пауза чтобы мотор успел переместиться. Скорость и плавность движения
   }
   
   Y=map(analogRead(YAxis), 0, 1023, 0, 180);           // Переводим значение из 0-1023 в градусы от 0 до 180 по оси X
   if(Y>90){ 
   b=b+1;
   myservo2.write(b);
   delay(100);                                          // Пауза чтобы мотор успел переместиться. Скорость и плавность движения
   }
   if(Y<85){
   b=b-1;
   myservo2.write(b);
   delay(100);                                          // Пауза чтобы мотор успел переместиться. Скорость и плавность движения
   }
   
 // Кнопка
     if(!digitalRead(Button) == HIGH && flag == 0) {    // если кнопка нажата и flag=0                  
       digitalWrite(Led, HIGH);                         // Зажигаем светодиод
       flag=1;                                          // Флаг только одно действие 
       delay(500);                                     // Задержка. Чтобы увидеть выстрел
       digitalWrite(Led, LOW);                          // Гасим светодиод
     } 
        
      if(!digitalRead(Button) == LOW && flag == 1) {    // если кнопка НЕ нажата и flag=1                 
        flag=0;                                         // обнуляем переменную flag 
     }
}

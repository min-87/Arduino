  #include "RotaryEncoder.h"          // библиотека для энкодера
  RotaryEncoder encoder(3, 2);        // пины подключение энкодера (DT, CLK)
  
  const int motor = 9;                // пин родключения мотора
  int pos = 0;                        // задаем начальную позицию энкодера
  int BUTTON = 4;                     // Кнопка
  const int value1 = 50;              // Первая скорость
  const int value2 = 70;              // Вторая скорость
  const int value3 = 80;              // Третья скорость
  const int value4 = 90;              // Четвёртая скорость
  const int value5 = 100;             // Пятая скорость
  const int value6 = 120;             // Шестая скорость
  const int stopping = 0;              // Остановка


int newPos;

void setup() {
  Serial.begin(9600);
  pinMode(motor, OUTPUT);             // D9 как выход
  pinMode(BUTTON, INPUT_PULLUP); 
  Serial.println(pos);                // выводим на монитор начальное значение
}

void loop() {
   
   encoder.tick();                    // проверяем положение ручки энкодера
   newPos = encoder.getPosition();    // Присваиваем значение энкодера переменной
   

  if  (!(digitalRead(BUTTON))){       // Остановка энкодера
     encoder.setPosition(0);
     delay (100);
    }  
     
     if (pos != newPos) {             // если положение изменилось     
        pos = newPos;                 // Сохраняем текущее положение 
        switch(pos) {
           case 1:  
              Serial.println("Скорость-1");
              analogWrite(motor, value1); 
              break;      
           case 2:  
              Serial.println("Скорость-2");
              analogWrite(motor, value2); 
              break;      
            case 3:  
              Serial.println("Скорость-3");
              analogWrite(motor, value3); 
              break;
            case 4:  
              Serial.println("Скорость-4");
              analogWrite(motor, value4); 
              break;
            case 5:  
              Serial.println("Скорость-5");
              analogWrite(motor, value5); 
              break;
            case 6:  
              Serial.println("Скорость-6");
              analogWrite(motor, value6); 
              break;
            default: 
              Serial.print("Стоп - ");
              Serial.println(pos);
              analogWrite(motor, stopping); 
              return 0;
          }
      }
}

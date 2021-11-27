//Пример для подключения дисплея по шине I2C
//
//Тип подключения дисплея: 1 - по шине I2C, 2 - десятиконтактное. Обязательно указывать ДО подключения библиотеки
//Если этого не сделать, при компиляции возникнет ошибка: "LCD type connect has not been declared"
#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>

LCD_1602_RUS <LiquidCrystal_I2C> lcd(0x27, 20, 4);

void setup()
{
  lcd.init();                       // Инициализируем дисплей
  lcd.backlight();                  // Включаем 
  lcd.setCursor(0, 0); 
  lcd.print("А Б В Г Д Е Ё Ж З И");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 1);  
  lcd.print("Й К Л М Н О П Р С Т");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("У Ф Х Ц Ч Ш Щ Ъ Ы Ь");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 3); 
  lcd.print("Э Ю Я");
  delay(5000);
  lcd.clear();
  
  lcd.setCursor(0, 0); 
  lcd.print("а б в г д е ё ж з и");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 1);  
  lcd.print("й к л м н о п р с т");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("у ф х ц ч ш ш ъ ы ь");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 3); 
  lcd.print("э ю я");
  delay(5000);
  lcd.clear();
  
  lcd.setCursor(6, 0); 
  lcd.print("МАКСИМУМ");
  lcd.setCursor(9, 1); 
  lcd.print("8");
  lcd.setCursor(0, 2); 
  lcd.print("НЕ СТАНДАРТНЫХ БУКВ");
  lcd.setCursor(8, 3); 
  lcd.print("БУКВ");
}


void loop()
{
  

}

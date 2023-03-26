#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte connectorStart[8] = {
0b01010,
0b01010,
0b11111,
0b11111,
0b11111,
0b01110,
0b00100,
0b00100
};

byte connectorMiddle[8] = {
0b01010,
0b11111,
0b11111,
0b11111,
0b01110,
0b00100,
0b00100,
0b00000
};


byte connectorEnd[8] = {
0b11111,
0b11111,
0b11111,
0b01110,
0b00100,
0b00100,
0b00000,
0b00000
};

byte socketStart[8] = {
0b00000,
0b00000,
0b01110,
0b11111,
0b11111,
0b11111,
0b10101,
0b10101
};

byte socketMiddle[8] = {
0b00000,
0b00000,
0b01110,
0b11111,
0b11111,
0b11111,
0b10101,
0b11111
};
byte socketEnd[8] = {
0b00000,
0b00000,
0b01110,
0b11111,
0b11111,
0b11111,
0b11111,
0b11111
};

int i = 2;

void setup() {
  lcd.begin();

  lcd.createChar(0, connectorStart);
  lcd.createChar(1, connectorMiddle);
  lcd.createChar(2, connectorEnd);
  
  lcd.createChar(3, socketStart);
  lcd.createChar(4, socketMiddle);
  lcd.createChar(5, socketEnd);
}

void loop() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Earth");
  lcd.setCursor(5, 1);
  lcd.print("Hour");
  
  lcd.setCursor(11, 0);
  lcd.write(i + 3);
  
  lcd.setCursor(11, 1);
  lcd.write(i);

  delay(300);
  
  i--;
  if (i < 0) {
    i = 2;
  }
}

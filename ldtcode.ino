#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>
#include <Keypad.h>

Servo myservo;

LiquidCrystal_I2C lcd(0x27,20,4);

#define Password_Length 7

int pos = 0;

char Data[Password_Length]; 
char Master[Password_Length] = "102027"; 
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 3;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
bool door = true;

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};

Keypad customKeypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  myservo.attach(10);
  ServoClose();
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
}

void loop()
{
  if (door == 0)
  {
    customKey = customKeypad.getKey();
    
    if (customKey == '#')

    {

    lcd.clear();
  	  lcd.setCursor(3,0);
  	   lcd.print("LOCKING");
       delay(700);
      lcd.setCursor(10,0);
       lcd.print(".");
       delay(800);
      lcd.setCursor(11,0);
       lcd.print(".");
       delay(800);
      lcd.setCursor(12,0);
       lcd.print(".");
       delay(500);
       lcd.clear();
       delay(500);
       ServoClose();
     lcd.setCursor(5,0);
     lcd.print("LOCKED");
     delay(1700);
      
     door = 1;
    }
  }
  
  else Open();
}
  
void clearData()
{
  while (data_count != 0) 
  {
    Data[data_count--] = 0;
  }
  return;
}

void ServoOpen()
{
  for (pos = 0; pos <= 90; pos += 5)
  {
    myservo.write(pos);
    delay(15);
  }
}

void ServoClose()
{
  for (pos = 90; pos >= 0; pos -= 5)
  {
    myservo.write(pos);
    delay(15);
  }
}

void Open()
{
  lcd.setCursor(1, 0);
  lcd.print("INPUT PASSWORD");
  
  customKey = customKeypad.getKey();
  if (customKey)
  {
    lcd.setCursor(0, 1);
     lcd.print("******");
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    lcd.print(Data[data_count]);
    data_count++;
  }
  
  if (data_count == Password_Length - 1)
  {
    if (!strcmp(Data, Master))
    {
      lcd.clear();
      ServoOpen();
      lcd.print("ACCESS GRANTED");
      lcd.setCursor(0,1);
      lcd.print("Press # To Close");
      door = 0;
    }
    else
    {
      lcd.clear();
      
     lcd.setCursor(1,0);
      lcd.print("ACCESS DENIED!");
      delay(1500);
      door = 1;
    }
    clearData();
  }
}
#include <Servo.h>
#include <Wire.h>
#include <DHT11.h>
#include <LiquidCrystal_I2C.h>

int button = 4;
int dial = A5;
int dht = 2;

int fanAdir = 12;
int fanApower = 3;
int fanAbrake = 9;

int fanBdir = 13;
int fanBpower = 11;
int fanBbrake = 8;

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT11 dht11(2);
Servo motor1;

float temperature = 0;
float humidity = 0;

int desiredtemp = 70;
int desiredhumid = 70;

bool buttondown = false;
bool editing = false;

int tick = 0;
int page = 0;

// util functions
int convtofreedomunits(int cel){
  float freedom = (cel * 1.8) + 32;
  return round(freedom); // round() for some reason isn't documented anywhere, but it exists for some reason
}

// check functions
bool checkifclimatechanged(){
  int newtemperature = 0;
  int newhumidity = 0;
  int result = dht11.readTemperatureHumidity(newtemperature, newhumidity);

  if (temperature != convtofreedomunits(newtemperature) || humidity != newhumidity){
    temperature = convtofreedomunits(newtemperature);
    humidity = newhumidity;
    return true;
  } else {
    return false;
  }
}

bool checkifbuttonpressed(){
  bool newbuttondown = digitalRead(button);

  if (buttondown != newbuttondown && newbuttondown == HIGH){
    buttondown = newbuttondown;
    return true;
  } else {
    buttondown = newbuttondown;
    return false;
  }
}

// lcd-affecting functions 
void printclimate(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(temperature);
  lcd.print("F");
  lcd.setCursor(8, 0);
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("TARGET: ");
  lcd.print(desiredtemp);
  lcd.print(" / ");
  lcd.print(desiredhumid);
}

// editing functions
void tempset(){
  int dialreading = analogRead(dial);
  dialreading = map(dialreading, 0, 1023, 50, 90);
  desiredtemp = dialreading;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("set target temp");
  lcd.setCursor(0, 1);
  lcd.print(desiredtemp);
  lcd.print("F");
}

void humidset(){
  int dialreading = analogRead(dial);
  dialreading = map(dialreading, 0, 1023, 0, 100);
  desiredhumid = dialreading;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("set target humid");
  lcd.setCursor(0, 1);
  lcd.print(desiredhumid);
  lcd.print("%");
}

void paginate(){
  page += 1;
  if (page > 2){page = 0;}
  return;
}

// subsystem functions
void fanset(bool act){
  if(act){
    digitalWrite(fanAdir, HIGH);
    digitalWrite(fanAbrake, LOW);
    analogWrite(fanApower, 255);

    digitalWrite(fanBdir, HIGH);
    digitalWrite(fanBbrake, LOW);
    analogWrite(fanBpower, 255);

  } else {
    digitalWrite(fanAdir, HIGH);
    digitalWrite(fanAbrake, HIGH);
    analogWrite(fanApower, 0);

    digitalWrite(fanBdir, HIGH);
    digitalWrite(fanBbrake, HIGH);
    analogWrite(fanBpower, 0);
  }
}

void windowset(bool act){
  return;
}

// system autorun functions
void setup() {
  Serial.begin(19200);
  lcd.init();                // initialize the lcd
  lcd.backlight();           // Turn on backlight

  pinMode(button, INPUT);
  pinMode(dht, INPUT);

  pinMode(fanAdir, OUTPUT);
  pinMode(fanAbrake, OUTPUT);
  pinMode(fanApower, OUTPUT);
}

void loop() {
  motor1.attach(6);
  motor1.write(179);
  motor1.detach();
  bool climatechanged = false;
    if (tick%10 == 0){climatechanged = checkifclimatechanged();}
  
  bool buttonpressed = checkifbuttonpressed();

  if (buttonpressed){paginate();} // cycles through all pages

  switch (page){
    case 0:
      if (climatechanged || buttonpressed){printclimate();} 
      break;

    case 1:
      tempset();
      break;

    case 2:
      humidset();
      break;

    default:
      page = 0;
      break;
  }

  if (humidity > desiredhumid + 5 || temperature > desiredtemp + 15){
    fanset(true);
  } else {
    fanset(false);
  }
  
    

  tick++;
  if (tick==10){tick=0;}

  delay(100);
}

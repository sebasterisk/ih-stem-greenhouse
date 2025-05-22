int moist = 0;
int trigger = 90;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  moist = analogRead(A0);
  Serial.println(moist);
  if (moist > trigger)
  {
    digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(5,LOW);
    delay(150);
  }
  else
  {
    digitalWrite(5, LOW);
  }
}

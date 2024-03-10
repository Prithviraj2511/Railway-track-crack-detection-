#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);
SoftwareSerial gpsSerial(9, 8);
SoftwareSerial sim800l(11, 10);
float lat = 28.5458, lon = 77.1703;
int irleft = 3;
int irright = 4;
int motor = 7;
int buzzer = 5;
int irleftt = 0;
int irrightt = 0;
int pirs = 2;
int pirv = 0;
TinyGPS gps;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sim800l.begin(9600);
  gpsSerial.begin(9600);
  Serial.println("Started");
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Rail Way Track ");
  lcd.setCursor(0, 1);
  lcd.print("  Crack Finder  ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Project Made By-");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ms. Perfect");
  lcd.setCursor(0, 1);
  lcd.print("Harshu");
  delay(2000);
  pinMode(irleft, INPUT);
  pinMode(irright, INPUT);
  pinMode(motor, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pirs, INPUT);
  digitalWrite(motor, HIGH);
  digitalWrite(buzzer, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  pirv = digitalRead(pirs);
  irleftt = digitalRead(irleft);
  irrightt = digitalRead(irright);
  while (gpsSerial.available()) {
    if (gps.encode(gpsSerial.read()))
    {
      gps.f_get_position(&lat, &lon);
    }
  }
  String latitude = String(lat, 6);
  String longitude = String(lon, 6);
  Serial.println(latitude + ";" + longitude);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cheking Track...");
  lcd.setCursor(0,1);
  lcd.print(latitude+","+longitude);
  delay(500);

  if (irrightt == 0 && irleftt == 0 && pirv == LOW)
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(motor, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Track is Clear");
    delay(1000);
  }
  if (pirv == HIGH) {
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Motion detected");
    digitalWrite(motor, LOW);
    sendSMS("Motion Detected");
    delay(500);
  }
  else if (irrightt == 1 && irleftt == 1)
  {
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Track is Cracked");
    lcd.setCursor(0, 1);
    lcd.print("  at Both Side  ");
    digitalWrite(motor, LOW);
    sendSMS("Track is cracked at both sides");
    delay(1000);
  }
  else if (irleftt == 1)
  {
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Track is Cracked");
    lcd.setCursor(0, 1);
    lcd.print("    at  LEFT    ");
    digitalWrite(motor, LOW);
    sendSMS("Track is cracked at left");
    delay(1000);
  }
  else if (irrightt == 1)
  {
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Track is Cracked");
    lcd.setCursor(0, 1);
    lcd.print("    at RIGHT    ");
    digitalWrite(motor, LOW);
    sendSMS("Track is cracked at right");
    delay(1000);
  }
}

void sendSMS(String msg)
{
  Serial.println("Sending SMS...");               //Show this message on serial monitor
  sim800l.print("AT+CMGF=1\r");                   //Set the module to SMS mode
  delay(100);
  sim800l.print("AT+CMGS=\"+917021821421\"\r");  //Your phone number don't forget to include your country code, example +212123456789"
  delay(500);
  sim800l.print(msg);       //This is the text to send to the phone number, don't make it too long or you have to modify the SoftwareSerial buffer
  delay(500);
  sim800l.print((char)26);// (required according to the datasheet)
  delay(500);
  sim800l.println();
  Serial.println("Text Sent.");
  delay(500);

}

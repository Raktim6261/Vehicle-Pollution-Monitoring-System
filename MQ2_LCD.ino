
#include <MQ2.h>
#include <Wire.h>
#include<LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
SoftwareSerial mySerial(3,2);

 // CHANGE 0X3F TO 0X27 AS LCD ADDRESSk
 LiquidCrystal_I2C lcd(0x27,16,2);
int Analog_Input = A0;
float ppm_conc= 0.000;
int lpg, co, smoke;
MQ2 mq2(Analog_Input);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  mySerial.begin(9600);
 
 
  mq2.begin();
}
void loop() {
  float* values = mq2.read(true); //set it false if you don't want to print the values in the Serial
  //lpg = values[0];
  lpg = mq2.readLPG();
  lcd.setCursor(0,0);
  lcd.print("LPG: ");
  lcd.print(lpg);

  //co = values[1];
  co = mq2.readCO();
  
  lcd.print(" CO:");
  lcd.print(co);
  
  
  lcd.setCursor(0,1);
  //smoke = values[2];
  smoke = mq2.readSmoke();
  lcd.print("SMOKE: ");
  lcd.print(smoke);
  lcd.print("PPM");

  
  ppm_conc=10*log(co);
  Serial.print(ppm_conc);
  if(ppm_conc>38){
  Serial.println("You are unlucky");
 
  delay(1000);
  mySerial.println("AT+CGMF=1");
  updateSerial();
     delay(1000);
  mySerial.println("AT+CMGS=\"+918962737050\"");
  updateSerial();
  mySerial.println("Vehicular emimmision is above standard level");
 delay(1000);
  
  }
 
  
 
  delay(1000);
}
 void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

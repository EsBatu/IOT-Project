//-------------------------------------------------Maling Sendal-----------------------------------------------------

#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header (LCD I2C)
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include <hd44780.h>
#include <DS3231.h> 

hd44780_I2Cexp lcd;
DS3231 rtc(SDA, SCL);
Time t;

int output1 = 2; // output
int input1 = 3; // Button Auto
int input2 = 4; // Button SendTest
int input3 = 5; // CB Serial
int input4 = 6; // Board Boomgate
int input5 = 7; // Motor Boomgate
int input6 = 8; // Loop 1
int input7 = 9; // Loop 2
int input8 = 10; // Testpoin Loop 1
int input9 = 11; // Testpoin Loop 2
int output2 = 12; // BG Up
int output3 = 13; // BG Down

const int act = 2020;
const int ex = 2021;
//int ON = 255;
//int OFF = 0;

//-------------------------------------------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(9600);
  rtc.begin();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  pinMode(input1, INPUT);
  pinMode(input2, INPUT);
  pinMode(input3, INPUT);
  pinMode(input4, INPUT);
  pinMode(input5, INPUT);
  pinMode(input6, INPUT);
  pinMode(input7, INPUT);
  pinMode(input8, INPUT);
  pinMode(input9, INPUT);
  
  digitalWrite(output1, HIGH);
  digitalWrite(output2, HIGH);
  digitalWrite(output3, HIGH);

  rtc.setTime(23, 0, 0);     // Set the time to HH:MM:SS (24hr format)
  rtc.setDate(31, 12, 2020);   // Set the date to DD:MM:YYYY
}

//-------------------------------------------------------------------------------------------------------------------
void loop() 
{
  t = rtc.getTime(); 
  
  delay(500);
  
  Serial.println(rtc.getTimeStr());
  delay(1000);
  
  if(t.year == act)
  {
    active();
  }
  else
  {
    expired();
  }  
}

//------------------------------------------------------------------------------------------------------------------
void active()
{
  int automode = digitalRead(input1);
  int testmode = digitalRead(input2);
  t = rtc.getTime(); 
  
  if (automode == HIGH)
  {
    sendtest();
    cbresult();
    automatic();
  }
  else if (testmode == HIGH)
  {
    sendtest2();
    tesgate();
    tesboard();
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(4,0);
    lcd.print("Stand By");
    lcd.setCursor(4,1);
    lcd.print(rtc.getTimeStr());
    //Serial.println(rtc.getTimeStr());
    //delay(1000);
  }
}
//------------------------------------------------
void expired()
{
  lcd.setCursor(0,0);
  lcd.print("              ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(4,0);
  lcd.print("Expired!");
  Serial.println("Expired");
  delay(1000);
}

//-----------------------------------------------------------------------------------------------------------------
void automatic()
{
  int motor = digitalRead(input5);
  int loop1 = digitalRead(input6);
  int loop2 = digitalRead(input7);
  int testloop1 = digitalRead(input8);
  int testloop2 = digitalRead(input9);
  
  
  if (motor == HIGH)
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Wire 1");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(7,1);
    lcd.print("OK");
    delay(2000);
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Wire 1");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(6,1);
    lcd.print("Fail");
    delay(2000);
  }


  if (loop1 == HIGH)
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Wire 2");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(7,1);
    lcd.print("OK");
    delay(2000);
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Wire 2");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(6,1);
    lcd.print("Fail");
    delay(2000);
  }


  if (loop2 == HIGH)
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Wire 3");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(7,1);
    lcd.print("OK");
    delay(2000);
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Wire 3");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(6,1);
    lcd.print("Fail");
    delay(2000);
  }


  if (testloop1 == HIGH)
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Wire 4");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(7,1);
    lcd.print("OK");
    delay(2000);
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Wire 4");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(6,1);
    lcd.print("Fail");
    delay(2000);
  }


  if (testloop2 == HIGH)
  {
   lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Wire 5");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(7,1);
    lcd.print("OK");
    delay(2000);
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Wire 5");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(6,1);
    lcd.print("Fail");
    delay(2000);
  }
   //digitalWrite(output1, LOW);  
}

//------------------------------------------------------------------------------------------------------------------
void sendtest()
{
  int cb = digitalRead(input3);

  lcd.setCursor(0,0);
  lcd.print("              ");
  lcd.setCursor(3,0);
  lcd.print("Automatic");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(5,1);
  lcd.print("Check");
  delay(2000);
  
  lcd.setCursor(0,0);
  lcd.print("              ");
  lcd.setCursor(2,0);
  lcd.print("Checking CB");
  lcd.setCursor(0,1);
  lcd.print("              ");
  delay(2000);
  //Serial.println("SAP");
  Serial.write("SAP");
  delay(100);
}

//------------------------------------------------------------------------------------------------------------------
void sendtest2()
{ 
  lcd.setCursor(0,0);
  lcd.print("              ");
  lcd.setCursor(1,0);
  lcd.print("CB Send Data");
  lcd.setCursor(0,1);
  lcd.print("              ");
  delay(2000);
  digitalWrite(output2, LOW);
  Serial.write("SAP");
  delay(100);
  digitalWrite(output2, HIGH);
  lcd.setCursor(0,0);
  lcd.print("              ");
  lcd.setCursor(1,0);
  lcd.print("Boomgate Test");
  lcd.setCursor(0,1);
  lcd.print("              ");
  delay(100);
}

//------------------------------------------------------------------------------------------------------------------
void tesgate()
{
  digitalWrite(output1 , LOW);
  delay(300);
  digitalWrite(output1 , HIGH);
  delay(300);
  digitalWrite(output1 , LOW);
  delay(300);
  digitalWrite(output1 , HIGH);
  delay(300);
  digitalWrite(output1 , LOW);
  delay(300);
  digitalWrite(output1 , HIGH);
  delay(300);
  digitalWrite(output1 , LOW);
  delay(300);
  digitalWrite(output1 , HIGH);
  delay(300);
  digitalWrite(output1 , LOW);
  delay(300);
  digitalWrite(output1 , HIGH);
  delay(300);
  digitalWrite(output1 , LOW);
  delay(300);
  digitalWrite(output1 , HIGH);
  delay(300);
  digitalWrite(output1 , LOW);
  delay(300);
}

//------------------------------------------------------------------------------------------------------------------
void tesboard()
{
  int board = digitalRead(input4);
  
  if (board == HIGH)
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Board");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(7,1);
    lcd.print("OK");
    delay(2000);
    digitalWrite(output1 , HIGH);
    digitalWrite(output3, LOW);
    delay(500);
    digitalWrite(output3, HIGH);
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(5,0);
    lcd.print("Board");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(6,1);
    lcd.print("Fail");
    delay(2000);
    digitalWrite(output1 , HIGH);
    digitalWrite(output3, LOW);
    delay(500);
    digitalWrite(output3, HIGH);
  }
}

//------------------------------------------------------------------------------------------------------------------
void cbresult()
{
  int cb = digitalRead(input3);
  
  if (cb == HIGH)
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(3,0);
    lcd.print("CB Serial");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(7,1);
    lcd.print("OK");
    delay(2000);
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("              ");
    lcd.setCursor(3,0);
    lcd.print("CB Serial");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(6,1);
    lcd.print("Fail");
    delay(2000);
  }
}

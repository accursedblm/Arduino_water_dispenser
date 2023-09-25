#include <Wire.h>
const int buttonPin1 = 2;   //Номер пина кнопки 1
const int buttonPin2 = 3;   //Номер пина кнопки 2
const int buttonPin3 = 4;   //Номер пина кнопки 3
const int ledPin =  13;     //Номер пина встроенного СД
const int relePin = 10;     //Номер пина реле
int flag = 0;               //Флаг случая нажатия
bool btnState1 = 0;         //Статус кнопки 1
bool btnState2 = 0;         //Статус кнопки 2
bool btnState3 = 0;         //Статус кнопки 3
uint32_t cup = 7000ul;      //200 мл
uint32_t tea = 35000ul;     //1000 мл
uint32_t delta = 0ul;       //Таймер останова
uint32_t TimeStart = 0ul;   //Время начала
uint32_t TimeClick = 0ul;  //Врямя нажати кнопки

void OnRele()
{
  digitalWrite(ledPin, HIGH); // зажигаем светодиод
  digitalWrite(relePin, HIGH); // включаем реле
}

void OffRele()
{
  digitalWrite(ledPin, LOW); // зажигаем светодиод
  digitalWrite(relePin, LOW); // включаем реле
}

void ButtonState()
{
  btnState1 = !digitalRead(buttonPin1);
  if (millis()- TimeClick > 500)
  {
    btnState2 = !digitalRead(buttonPin2);
    btnState3 = !digitalRead(buttonPin3); 
  }
  else 
  {
    btnState2 = 0; btnState3 =0;
  }
}

void PumpOn(int f, uint32_t d)
{
  OnRele();
  flag = f;
  delta = d;
  TimeClick = TimeStart = millis();
}

void PumpOff()
{
  OffRele();
  flag = 0;
  delta = 0ul;
  TimeStart = 0ul; 
}

void setup() {
  //________________________
  // Настройка кнопки
  pinMode(ledPin, OUTPUT); //Выход на пин светодиода
  pinMode(buttonPin1, INPUT); //Вход на пин кнопки
  pinMode(buttonPin2, INPUT); //Вход на пин кнопки
  pinMode(buttonPin3, INPUT); //Вход на пин кнопки
  //________________________
  // Настройка реле
  pinMode(relePin, OUTPUT); //Выход на пин реле
}

void loop() {
  // put your main code here, to run repeatedly:
  ButtonState();
  //Serial.print("Flag=" + String(flag)+ " " + "delta=" + String(delta) + " " + "millis-timestart =" + String(millis()-TimeStart) + "\n");
  if(!flag)
    {
      if (btnState1 && !btnState2 && !btnState3)
        {PumpOn(1,0ul); return;}
      if (btnState2 && !btnState1 && !btnState3)
        {PumpOn(2,cup); return;}
      if (btnState3 && !btnState1 && !btnState2)
        {PumpOn(2,tea); return;}
    }
  else
    {
      if (btnState1 && flag > 1 && !btnState2 && !btnState3)
        {PumpOff(); return;}
      if (!btnState1 && flag == 1 && !btnState2 && !btnState3)
        {PumpOff(); return;}
      if (btnState2 && !btnState1 && !btnState3)
        {delta += cup; TimeClick = millis() ; return;}
      if (btnState3 && !btnState1 && !btnState2)
        {delta += cup; TimeClick = millis(); return;}
      if (TimeStart != 0ul && delta != 0ul && (millis()-TimeStart >= delta))
        {PumpOff(); return;}
    } 
}

//**************************
//програма управления помпой дн-250
//Выводы:
// А0 - потенциометр (датчик уровня)
// А4 - SDA вывод дисплея 
// A5 - SCL вывод дисплея 
// D3 - датчик потока жидкости (внешние прерывания)
// D5 - выход (вкл/выкл реле)
//**************************
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

//состояние конченого автомата
#define START 0
#define MAX 1
#define MIN 2


float Result = 0.0;
int analog = 0;
int status = START;


int counter = 0;
double Flow = 0.0;
double Volume = 0.0;

void btnIsr() {
  counter++;  // + нажатие
}

void setup() {
  pinMode(5, OUTPUT);
  Serial.begin(115200);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, btnIsr, FALLING);
  InitDisplay(2);
}
void InitDisplay(int mode)
{
  oled.init();        // инициализация
  oled.clear();       // очистка
  oled.setScale(mode);   // масштаб текста (1..4)
}




void loop() {
    

  delay(1000);          // имитация загруженной программы
  Flow = counter/450.0;
  Volume = Volume + Flow;
  // вывод реультата на экран
  PrintDisplay("Q=","V=",Flow,Volume);
  // функция PrintDisplay()
   Serial.println(Flow);
   Serial.println(Volume);
   counter = 0;
   // проверка уровня жидкости 
   analog = analogRead(A0);
    Result = (analog * 5.0) / 1024.0;
    Selector(START, Result);

}

void Selector(float result, int status)
{
  switch(status)
  {
    case START:
    {
      if(result > 0.5 && result < 4.5)
      {
         status = START;
         return;
      }
      if(result >= 4.5)
      {
        status = MAX;
        return;
      }
       if(result <= 0.5)
      {
        status = MIN;
        return;
      }
    }
    case MAX:
    {
      Serial.println("MAX");
      digitalWrite(5, HIGH); //включить помпу, одаем "1" на вывод D5
      if(result < 4.5)
      {
        status = START;
      }
       return;
    }
    case MIN:
    {
    Serial.println("MIN");
    digitalWrite(5, LOW); //выключить помпу, подаем "0" на вывод D5
    if(result > 0.5)
      {
        status = START;
      }
       return;
    }
  }
}
void PrintDisplay(const String s1, const String s2, double v1, double v2)
{
  oled.setCursorXY(5, 5);
  oled.print(s1);
  oled.print(v1);
  oled.setCursorXY(5, 35);
  oled.print(s2);
  oled.print(v2);
}

#include <Arduino.h>

#define d0Pin 0
#define d1Pin 1
#define d2Pin 2
#define d3Pin 3
#define sw0Pin 5
#define sw1Pin 6

// Global variables
byte count;

void setup()
{
  // put your setup code here, to run once:
  pinMode(d0Pin, OUTPUT);
  pinMode(d1Pin, OUTPUT);
  pinMode(d2Pin, OUTPUT);
  pinMode(d3Pin, OUTPUT);
  pinMode(sw0Pin, INPUT);
  count = 0;
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (digitalRead(sw1Pin) == 0)
  {
    while (digitalRead(sw1Pin) == 0)
      ;
    delay(300);
  }

  if (digitalRead(sw0Pin) == 1)
  { //digitalRead(sw0Pin) == HIGH
    digitalWrite(d0Pin, bitRead(count, 0));
    digitalWrite(d1Pin, bitRead(count, 1));
    digitalWrite(d2Pin, bitRead(count, 2));
    digitalWrite(d3Pin, bitRead(count, 3));
    delay(450);
    count++;
  }
}

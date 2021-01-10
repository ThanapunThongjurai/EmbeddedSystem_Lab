#include <Arduino.h>
#include <String.h>
#include <string.h>
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop()
{

  if (Serial.available() > 0)
  {

    String rxBuffer = Serial.readString();
    //Serial.println(rxBuffer);
    int numDigit[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < rxBuffer.length(); i++)
    {
      if (rxBuffer[i] >= 48 && rxBuffer[i] <= 57)
        numDigit[rxBuffer[i] - 48]++;
      if (rxBuffer[i] == '#')
        break;
      //Serial.println(rxBuffer[i]);
    }

    Serial.print("Number of character digits\n");
    for (int i = 0; i < 10; i++)
    {
      Serial.print(i);
      Serial.print(" = ");
      Serial.print(numDigit[i]);
      Serial.print("\n");
    }
  }  
}
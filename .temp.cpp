#include <Arduino.h>
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

int count = 0;
int incomedata = 0;
char rxBuffer[100];
void loop()
{

  while(Serial.available() > 0)
  {
    rxBuffer[count] = Serial.read();
    //Serial.println(rxBuffer);
    count++;
    //Serial.println(count);
    incomedata = 1;
  }
  if (incomedata == 1)
  {
    Serial.print("data");
    for (int i = 0; i < count; i++)
    {
      Serial.print(rxBuffer[i]);
    }
    Serial.print("count ");
    Serial.println(count);

    incomedata = 0;
    count = 0;
  }
}
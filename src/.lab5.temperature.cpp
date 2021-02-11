#include <Arduino.h>

#include "DHT.h"

//!dth22 setting
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302) 
DHT dht(DHTPIN, DHTTYPE);

float LM35(char type)
{
    
    float mv = ((1.1/1024.0)*(float)analogRead(A0))/0.01; 
    float cel = mv/10;
    float farh = (cel*9)/5 + 32;
    if(type == 'c')
    {
      return cel;
    }
    else if(type == 'f')
    {
      return farh;
    }
    else
    {
      return cel;
    }
} 

void setup() { 
  Serial.begin(9600); 
  Serial.println("Status\tHumidity (%)\tTemperature (C)\tLM35");
  dht.begin();
  analogReference(INTERNAL);

}

void loop() {  
  float h = dht.readHumidity(); 
  float t = dht.readTemperature();
  float LM35C = LM35('c');

  if (isnan(h) || isnan(t)) 
    Serial.println("!OK"); 
  else
    Serial.print("OK");
  
  Serial.print("\t");
  Serial.print(h, 1);
  Serial.print("\t\t");
  Serial.print(t, 1);
  Serial.print("\t\t");
  Serial.println(LM35('c'));

}
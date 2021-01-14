#include <Arduino.h>
#include "TimerOne.h"
#include <string.h>
#include <EEPROM.h>
//Ref : https://create.arduino.cc/projecthub/BitteristSquash/how-to-get-string-data-from-computer-via-serial-port-2ca228

//!GET String
#define LF 0x0A //END BIT //! \n //Ref https://en.wikipedia.org/wiki/Newline
int index;      //START BIT
char str_receive[100];
bool endtransmit = false;
byte str_receive_length = 0;

//! EEPROM
int addressOfEEPROM = 0;

//!struct
typedef struct
{
    char pvname[35];
    long male;
    long female;
} ProVinces;

ProVinces data[20];
int data_point = 0;
byte Count = 0;
//TODO เลือกจำนวนจังหวัดที่ต้องการก่อร
void setup()
{
    Serial.begin(9600);
    //!GET String
    index = 0;

    addressOfEEPROM = 0;
    while (Serial.available() == 0)
    {
        Count = Serial.parseInt();
        Serial.println(Count);
    }
}

void loop()
{
    //!GET String
    if (Serial.available() > 0)
    {
        Serial.readBytesUntil('\t', data[data_point].pvname, 35);
        data[data_point].male = Serial.parseInt();
        data[data_point].female = Serial.parseInt();
        // Serial.println(data[data_point].pvname);
        // Serial.println(data[data_point].male);
        // Serial.println(data[data_point].female);
        // Serial.println(data[data_point].male + data[data_point].female);
        EEPROM.put(addressOfEEPROM, data[data_point]);
        data_point++;
        addressOfEEPROM = addressOfEEPROM + sizeof(ProVinces);
    }
    if (data_point == Count)
    {
        addressOfEEPROM = 0;
        Serial.println("Read custom object from EEPROM: ");
        for (int i = 0; i < data_point; i++)
        {
            ProVinces ProVincesGet;
            EEPROM.get(addressOfEEPROM, ProVincesGet);
            Serial.print(i + 1);
            Serial.print("PVNAME : ");
            Serial.println(ProVincesGet.pvname);
            Serial.print("male : ");
            Serial.print(ProVincesGet.male);
            Serial.print("female : ");
            Serial.print(ProVincesGet.female);
            Serial.println();
            addressOfEEPROM = addressOfEEPROM + sizeof(ProVinces);
        }
        Count = 0;
    }
}

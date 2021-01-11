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

void setup()
{
    Serial.begin(9600); 
    //!GET String
    index = 0;

    Serial.println("Read custom object from EEPROM: ");
    for (int i = 0; i < 20; i++)
    {
        ProVinces ProVincesGet;
        EEPROM.get(addressOfEEPROM, ProVincesGet);
        Serial.print(i+1);
        Serial.print("\tPVNAME : ");
        Serial.print(ProVincesGet.pvname);
        Serial.print("\t\t\tmale : ");
        Serial.print(ProVincesGet.male);
        Serial.print("\tfemale : ");
        Serial.println(ProVincesGet.female);
        addressOfEEPROM = addressOfEEPROM + sizeof(ProVinces);
    }
    addressOfEEPROM = 0;
}
void loop()
{
    //!GET String
    if (Serial.available() > 0)
    {
        str_receive[index] = Serial.read();
        if ((str_receive[index] == '\n')) // (|| str_receive[index] == ',' //! คิดว่าทำได้นะ) //!LF
        {
            //Serial.print("Received new angle: ");
            str_receive_length = index;
            str_receive[index - 1] = 0;
            //Serial.println(str_receive);
            index = -1;
            endtransmit = true;
        }
        index++;
    }
    if (endtransmit == true)
    {
        endtransmit = false;
        Serial.println(str_receive);
        // for (int i = 0; i < str_receive_length; i++)
        //     Serial.print(str_receive[i]);
        byte name_index = 0; //Name OF Provinces
        byte between_Number = 0;
        byte man_index = 0;
        byte woman_index = 0;
        for (int i = 1; i < str_receive_length - 1; i++)
        {
            //Name OF Provinces
            if ((str_receive[i] == ' ') && (str_receive[i - 1] >= 'A') && (str_receive[i - 1] <= 'z') && (str_receive[i + 1] >= '0') && (str_receive[i + 1] <= '9'))
                name_index = i;

            if ((str_receive[i] == ' ') && (str_receive[i - 1] >= '0') && (str_receive[i - 1] <= '9') && (str_receive[i + 1] >= '0') && (str_receive[i + 1] <= '9'))
                between_Number = i;
            // if ((str_receive[i] == ' '))
            //     Serial.print(" ");
            // if ((str_receive[i] >= 'A') && (str_receive[i] <= 'z'))
            //     Serial.print("C");
            // if ((str_receive[i] >= '0') && (str_receive[i] <= '9'))
            //     Serial.print("N");
        }
        //NameProvince
        char NameProvince[name_index];
        for (int i = 0; i < name_index; i++)
        {
            NameProvince[i] = str_receive[i];
        }
        String NameProvince_str = NameProvince;
        Serial.println(NameProvince_str);

        //ManNumber
        man_index = name_index + 1;
        char man_Number[between_Number - man_index + 1];
        long Man = 0;
        for (int i = man_index, j = 0; i < between_Number; i++, j++)
        {
            man_Number[j] = str_receive[i];
        }
        Serial.print("Man :");
        Serial.println(Man = atol(man_Number));

        //WomanNumber
        woman_index = between_Number + 1;
        char woman_Number[str_receive_length - between_Number + 1];
        long Woman = 0;
        for (int i = between_Number, j = 0; i < str_receive_length; i++, j++)
        {
            woman_Number[j] = str_receive[i];
        }
        Serial.print("Woman :");
        Serial.println(Woman = atol(woman_Number));

        //EEPROM WRITE
        for (int i = 0; i < name_index; i++)
        {
            data[data_point].pvname[i] = NameProvince_str[i];
        }

        data[data_point].male = Man;
        data[data_point].female = Woman;
        EEPROM.put(addressOfEEPROM, data[data_point]);
        data_point++;
        addressOfEEPROM = addressOfEEPROM + sizeof(ProVinces);
    }
}

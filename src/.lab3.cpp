#include <Arduino.h>
#include "TimerOne.h"
#include <Debounce.h>

//! PINMAP
#define SW0 A0
#define S2 A1
#define S3 A2
byte BITpin[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
byte InputSW[] = {SW0, S2, S3};
long TimeStep[] = {500000, 250000, 1000000};
//! Debounce
Debounce Button2pin(2, 250, false, false);
Debounce ButtonS2(S2, 250, false, false);
Debounce ButtonS3(S3, 250, false, false);
//!GOBAL
volatile int Count = 0;
byte TimeStep_Count = 0;
byte CountType_Count = 0;
byte RunOrNot = 0;
void bin2bcd_3digit()
{
    for (int i = 0; i < sizeof(BITpin) / sizeof(BITpin[0]); i++)
    {
        digitalWrite(BITpin[i], bitRead(Count, i));
    }
}
void CountTimeWithInterrupt()
{
    // digitalWrite(13, digitalRead(13) ^ 1);
    if (CountType_Count >= 3)
    {
        CountType_Count = 0;
    }
    if (RunOrNot == 1)
    {

        if (CountType_Count == 0)
        {
            Count = Count + 1;
        }
        else if (CountType_Count == 1)
        {
            if (Count % 2 == 1)
                Count = Count + 2;
            else
                Count = Count + 1;
        }
        else if (CountType_Count == 2)
        {
            if (Count % 2 == 0)
                Count = Count + 2;
            else
                Count = Count + 1;
        }

        if (Count >= 1000)
            Count = 0;

        bin2bcd_3digit();
        Serial.println(Count);
    }
}
void NewTimeStep()
{
    Serial.print(TimeStep[TimeStep_Count]);
    Serial.print(" >>> ");
    TimeStep_Count++;
    if (TimeStep_Count >= sizeof(TimeStep) / sizeof(TimeStep[0]))
        TimeStep_Count = 0;
    Timer1.initialize(TimeStep[TimeStep_Count]);
    Serial.println(TimeStep[TimeStep_Count]);
}

void setup()
{

    for (int i = 0; i < sizeof(BITpin) / sizeof(BITpin[0]); i++)
    {
        pinMode(BITpin[i], OUTPUT);
    }

    for (int i = 0; i < sizeof(InputSW) / sizeof(InputSW[0]); i++)
    {
        pinMode(InputSW[i], INPUT);
    }

    pinMode(13, OUTPUT);
    Timer1.initialize(TimeStep[0]); // initialize timer1, and set a 1/2 second period
    //Timer1.pwm(9, 512);                             // setup pwm on pin 9, 50% duty cycle
    ////Timer1.initialize(2000000);
    ////Timer1.pwm(9, 512);

    Timer1.attachInterrupt(CountTimeWithInterrupt); // attaches CountTimeWithInterrupt() as a timer overflow interrupt

    //!DEBUG
    Serial.begin(9600);
    pinMode(2, INPUT_PULLUP);
}

void loop()
{
    if (Serial.available() > 0)
    {
        int receive = Serial.parseInt();
        Serial.println(receive);
        if (receive == 1)
        {
            RunOrNot = 1;
        }
        if (receive == 2)
        {
            RunOrNot = 0;
        }
        if (receive == 3)
        {
            NewTimeStep();
        }
        if (receive == 4)
        {
            CountType_Count++;
        }
    }
    // if (Button2pin.read() == 1)
    // {
    //     while (Button2pin.read() == 1)
    //         ;
    //     //NewTimeStep(); //<<WORK
    // }
}
#include <Arduino.h>
#define S7 A0
#define SW0 A1
#define SW1 A2

volatile byte stateStartStop = 1;
volatile byte stateFastSlow = 0;
byte pinLEDsize = 8;
byte pinLED[] = {5, 6, 7, 8, 9, 10, 11, 12};
// //!stateStartStop  = 1  เริ่มต้นไฟวิ่งตามรูปแบบ
//!stateStartStop  = 0  หยุดไฟวิ่ง
//!stateFastSlow  =  1  ลดความเร็วไฟวิ่ง
//!stateFastSlow  =  2  ไม่เพิ่มไม่ลด
//!stateFastSlow  =  3  เพิ่มความเร็วไฟวิ่ง

typedef struct Led
{
    int time = 100;
    byte led[8] = {0, 0, 0, 0, 0, 0, 0, 0};
} PatternLed;
PatternLed patternData[4];

void pin_2_function()
{
    if (digitalRead(S7) == LOW)
        stateStartStop = 0;
    else
        stateStartStop = 1;
}
void pin_3_function()
{
    if (digitalRead(S7) == LOW)
        stateFastSlow = 3;
    else
        stateFastSlow = 1;
}
byte ReadSW()
{
    //biteread to dec
    byte pattern = 0;
    pattern = digitalRead(SW0);//fist bit
    pattern = (digitalRead(SW1) *10) + pattern;
    
    return pattern;
}
void speed(byte inputPattern)
{
    if (stateFastSlow == 3)
    {
        patternData[inputPattern].time = patternData[inputPattern].time - 50;
        if (patternData[inputPattern].time <= 100)
            patternData[inputPattern].time = 100;
    }
    else if (stateFastSlow == 1)
    {
        patternData[inputPattern].time = patternData[inputPattern].time + 50;
        if (patternData[inputPattern].time >= 1000)
            patternData[inputPattern].time = 1000;
    }
    stateFastSlow = 2;
}
void PatternRun(byte inputPattern)
{
    for (int i = 0; i < pinLEDsize; i++)
    {
        digitalWrite(pinLED[i], patternData[inputPattern].led[i]);
    }

    if (inputPattern == 0)
    {
        byte light_Current = 0;
        for (int i = 0; i < pinLEDsize; i++)
        {
            if (patternData[inputPattern].led[i] == 1)
            {
                light_Current = i;
                break;
            }
        }
        if (light_Current >= 7)
            patternData[inputPattern].led[0] == 1;
        else
            patternData[inputPattern].led[light_Current + 1] == 1;

        patternData[inputPattern].led[light_Current] == 0;
    }
    if (inputPattern == 1)
    {
        byte light_Current = 0;
        for (int i = 8; i >= 0; i--)
        {
            if (patternData[inputPattern].led[i] == 1)
            {
                light_Current = i;
                break;
            }
        }
        if (light_Current <= 0)
            patternData[inputPattern].led[8] == 1;
        else
            patternData[inputPattern].led[light_Current - 1] == 1;

        patternData[inputPattern].led[light_Current] == 0;
        digitalWrite(light_Current, HIGH);
    }
    if (inputPattern == 2)
    {
    }
    if (inputPattern == 3)
    {
    }

    //delay(patternData[inputPattern].time);
}
void runOrStop(byte inputPattern)
{
    if (stateStartStop == 1)
    {
        PatternRun(inputPattern);
    }
}
void setup()
{
    for (int i = 0; i <= pinLEDsize; i++)
    {
        pinMode(pinLED[i], OUTPUT);
    }

    patternData[0].led[0] = 1;
    patternData[1].led[7] = 1;
    patternData[2].led[0] = 1;
    patternData[2].led[3] = 1;
    patternData[3].led[4] = 1;
    patternData[3].led[7] = 1;

    attachInterrupt(digitalPinToInterrupt(2), pin_2_function, RISING);
    attachInterrupt(digitalPinToInterrupt(3), pin_3_function, FALLING);
}
void loop()
{
    byte inputPattern = 0;//ReadSW();
    speed(inputPattern);
    runOrStop(inputPattern);
}

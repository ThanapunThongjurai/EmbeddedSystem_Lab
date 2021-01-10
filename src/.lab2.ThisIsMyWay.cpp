#include <Arduino.h>
#define S7 A0
#define SW0 A1
#define SW1 A2
volatile byte stateStartStop = 0;
volatile byte stateFastSlow = 0;
byte pinLEDsize = 8;
byte pinLED[] = {5, 6, 7, 8, 9, 10, 11, 12};
byte patRemember = 0;
unsigned long INTERVAL = 1000;
unsigned long GETTIME = 0;
//!stateStartStop  = 1  เริ่มต้นไฟวิ่งตามรูปแบบ
//!stateStartStop  = 0  หยุดไฟวิ่ง
//!stateFastSlow  =  1  ลดความเร็วไฟวิ่ง
//!stateFastSlow  =  2  ไม่เพิ่มไม่ลด
//!stateFastSlow  =  3  เพิ่มความเร็วไฟวิ่ง

typedef struct Led
{
    int time = 100;
    byte led[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    byte countPattern = 0;
} PatternLed;
PatternLed pat[4];
void ReadSW()
{
    byte pattern = 0;
    pattern = digitalRead(SW0) * pow(2, 0);             //fist bit
    pattern = (digitalRead(SW1) * pow(2, 1)) + pattern; //seconde bit

    if (patRemember != pattern)
    {
        patRemember = pattern;
        //stateStartStop = 0;
    }

    //return pattern;
}
void pin_2_function()
{
    if (digitalRead(S7) == LOW)
    {
        Serial.println("PIN2LOW state STOP");
        stateStartStop = 0;
    }

    else
    {
        Serial.println("PIN2HIGH state RUN");
        stateStartStop = 1;
        ReadSW();
    }
}
void pin_3_function()
{
    if (digitalRead(S7) == LOW)
    {
        stateFastSlow = 1;
        Serial.println("PIN3LOW state FAST");
    }

    else
    {
        stateFastSlow = 3;
        Serial.println("PIN3HIGH sate SLOW");
    }
}

void speed(PatternLed *pat)
{
    if (stateFastSlow == 3)
    {
        pat->time = pat->time - 50;
        if (pat->time <= 100)
            pat->time = 100;
    }
    else if (stateFastSlow == 1)
    {
        pat->time = pat->time + 50;
        if (pat->time >= 1000)
            pat->time = 1000;
    }
    stateFastSlow = 2;
}

void printPat(PatternLed *pat, byte getPat)
{
    Serial.print("PATTERN NO :");
    Serial.print(getPat);
    Serial.print("\t\tCOUNT : ");
    Serial.print(pat->countPattern);
    Serial.print("\t\tTIME : ");
    Serial.print(pat->time);
    Serial.print("\t\tRUN OR NOT : ");
    Serial.print(stateStartStop);
    Serial.print("\t\tTIME GEAR : ");
    Serial.println(stateFastSlow);
} 
void patternDisplay2(PatternLed *pat)
{

    for (int i = 0; i <= 8; i++)
    {
        digitalWrite(pinLED[i], pat->led[pat->countPattern] >> i & 0B0000001);
        //bitread(patData[getPat][pat->countPattern],i);
    }
    pat->countPattern++;
    if (pat->countPattern > pinLEDsize - 1)
        pat->countPattern = 0;
}
void setup()
{
    Serial.begin(9600);
    int patData[4][8] = {{0B00000001, 0B00000010, 0B00000100, 0B00001000, 0B00010000, 0B00100000, 0B01000000, 0B10000000},
                         {0B10000000, 0B01000000, 0B00100000, 0B00010000, 0B00001000, 0B00000100, 0B00000010, 0B00000001},
                         {0B10000001, 0B01000010, 0B00100100, 0B00011000, 0B10000001, 0B01000010, 0B00100100, 0B00011000},
                         {0B00011000, 0B00100100, 0B01000010, 0B10000001, 0B00011000, 0B00100100, 0B01000010, 0B10000001}};

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 8; j++)
            pat[i].led[j] = patData[i][j];

    // for(int i = 0 ; i < 8 ;i++)
    //     Serial.println(pat[0].led[i],BIN);
    pinMode(SW0, INPUT);
    pinMode(SW1, INPUT);
    pinMode(S7, INPUT);

    for (int i = 0; i <= pinLEDsize; i++)
    {
        pinMode(pinLED[i], OUTPUT);
    }
    attachInterrupt(digitalPinToInterrupt(2), pin_2_function, RISING);
    attachInterrupt(digitalPinToInterrupt(3), pin_3_function, FALLING);
}

void loop()
{
    byte inputPattern = patRemember; //ReadSW(); //ReadSW();
    //Serial.println(ReadSW());
    speed(&pat[inputPattern]);
    //dispay(&pat[inputPattern], inputPattern);

    //TODO Make it to function
    INTERVAL = pat[inputPattern].time;
    if (millis() - GETTIME >= INTERVAL)
    {
        GETTIME = millis();
        if (stateStartStop == 1)
        {
            //patternDisplay(&pat[inputPattern], inputPattern);
            patternDisplay2(&pat[inputPattern]);
        }
        printPat(&pat[inputPattern], inputPattern);
        //stateFastSlow = 1;
    }
    //delay(pat[inputPattern].time);
}

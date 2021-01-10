#include <Arduino.h>
#define S7 A0
#define SW0 A1
#define SW1 A2
#define Pulse0 2
#define Pulse1 3
int flag0 = 0;
int swPin[] = {A1, A2};
int ledPin[] = {5, 6, 7, 8, 9, 10, 11, 12};
int t = 300;
typedef struct
{
    int Pattern[10];
    byte LastTime;
    byte Step;
} Running_Light;
int count = 0;
unsigned long last_time = 0;
Running_Light pat1, pat2, pat3, pat4;

void Run()
{
    if (SelStep() == "00")
    {
        int pat[] = {0B00000001, 0B00000010, 0B00000100, 0B00001000, 0B00010000, 0B00100000, 0B01000000, 0B10000000};
        memcpy(pat1.Pattern, pat, sizeof(pat));
        disp_rn1(&pat1);
        pat1.Step = 8;
    }
    if (SelStep() == "01")
    {
        int pat[8] = {0B10000000, 0B01000000, 0B00100000, 0B00010000, 0B00001000, 0B00000100, 0B00000010, 0B00000001};
        memcpy(pat2.Pattern, pat, sizeof(pat));
        disp_rn1(&pat2);
        pat2.Step = 8;
    }
    if (SelStep() == "10")
    {
        int pat[4] = {0B10000001, 0B01000010, 0B00100100, 0B00011000};
        memcpy(pat3.Pattern, pat, sizeof(pat));
        disp_rn1(&pat3);
        pat3.Step = 4;
    }
    if (SelStep() == "11")
    {
        int pat[4] = {0B00011000, 0B00100100, 0B01000010, 0B10000001};
        memcpy(pat4.Pattern, pat, sizeof(pat));
        disp_rn1(&pat4);
        pat4.Step = 4;
    }
}
void Pul0()
{
    Serial.println("AAA");
    flag0 = 1;
    if (digitalRead(A0) == 0)
        flag0 = 0;
}
void Pul1()
{
    if (digitalRead(A0) == 0)
        t += 50;
    else
        t -= 50;
    if (t <= 100)
        t = 100;
    if (t >= 1000)
        t = 1000;
    Serial.println(t);
}
void disp_rn1(Running_Light *p)
{
    count = p->LastTime;
    if (millis() - last_time > t)
    {
        for (int i = 0; i < 8; i++)
            digitalWrite(ledPin[i], bitRead(p->Pattern[count], i));
        count++;
        if (count > p->Step)
            count = 0;
        p->LastTime = count;
        last_time = millis();
    }
}
String SelStep(void)
{
    return String(digitalRead(SW0)) + String(digitalRead(SW1));
}
void setup()
{
    Serial.begin(9600);
    for (int i = 0; i < 8; i++)
        pinMode(ledPin[i], OUTPUT);
    for (int i = 0; i < 2; i++)
        pinMode(swPin[i], INPUT_PULLUP);
    pinMode(Pulse0, INPUT_PULLUP);
    pinMode(Pulse1, INPUT_PULLUP);
    pinMode(A0, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(Pulse0), Pul0, FALLING);
    attachInterrupt(digitalPinToInterrupt(Pulse1), Pul1, FALLING);
}
void loop()
{
    Run();
}
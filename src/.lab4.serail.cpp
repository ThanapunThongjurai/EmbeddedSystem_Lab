#include <Arduino.h>
int Receive[20];
long int sum = 0;
void setup()
{
    Serial.begin(9600); // ใช้งาน serial โดนใช้ function begin
}
void loop()
{
    if (Serial.available() > 0) // มีข้อมูลเข้ามาใหม่
    {
        //รับค่าเข้าไปไว้ใน Array
        for (int i = 0; i < 20; i++)
        {
            Receive[i] = Serial.parseInt(); //เอาค่าไปไส่ใน Receive ตัวที่ i
        }
        int min = Receive[0]; //เก็บค่า receive ตัวที่ 0 เพื่อใช้เทียบ
        int max = Receive[0]; //เก็บค่า receive ตัวที่ 0 เพื่อใช้เทียบ
        //วนลูปหาค่า  max และเก็บผลลัพของ Sum
        for (int i = 0; i < 20; i++)
        {
            sum = sum + Receive[i];
            //เช็คค่าน้อย
            if (min > Receive[i])
            {
                min = Receive[i];
            }
            //เช็คค่ามาก
            if (max < Receive[i])
            {
                max = Receive[i];
            }
        }
        //แสดงผล Sum Min Max
        Serial.print("Sum = ");
        Serial.println(sum);
        Serial.print("Min = ");
        Serial.println(min);
        Serial.print("Max = ");
        Serial.println(max);
        Serial.println("DATA = ");

        //เรียงเลขจากน้อยไปมาก
        //ปิดใช้งานก็ได้
        for (int i = 0; i < 19; i++)
        {
            for (int j = i + 1; j < 20; j++)
            {
                if (Receive[i] >= Receive[j])
                {
                    int temp = Receive[i];
                    Receive[i] = Receive[j];
                    Receive[j] = temp;
                }
            }
        }

        //แสดงผล DATA
        int count = 1; //ใช้นับการขึ้นบรรทัดใหม่
        for (int j = 0; j < 20; j++)
        {

            Serial.print(Receive[j]);
            Serial.print("\t");

            //ตรวจว่าแสดงผลครับ 5 ตัวหรือยัง ถ้าครับ ให้ขึ้นบรรทัดใหม่
            if (count == 5)
            {
                count = 0;
                Serial.println();
            }
            count++;
        }

        //Debouce Serail Commu
        while (Serial.available() > 0)
        {
            Serial.read();
        }
    }
}
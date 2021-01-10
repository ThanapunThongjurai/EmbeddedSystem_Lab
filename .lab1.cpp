#include <Arduino.h>
#include <math.h>
byte secment[] = { 
    0x3F, 0x06, 0x5B, 0x4F,
    0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x77, 0x7C,
    0x58, 0x5E, 0x79, 0x71};
void setup()
{
  for (int i = 2; i <= 5; i++){
    pinMode(i, INPUT);//ประกาศ INPUT PIN
  }
  for (int i = 6; i <= 12; i++){
    pinMode(i, OUTPUT);//ประกาศ OUTPUT PIN
  }
}
int read_input_digital(){
  int read_input = 0;
  for (int i = 2, j = 0; i <= 5; i++, j++){
    read_input = read_input + (pow(2, j) * digitalRead(i));
  }
  return read_input;
}
void write_output_7secment(int receive)
{
  for (int i = 0; i <= 6; i++){
    digitalWrite(i + 6, secment[receive] >> i & 0B0000001);
  }
}
void loop(){
  write_output_7secment(read_input_digital());
}
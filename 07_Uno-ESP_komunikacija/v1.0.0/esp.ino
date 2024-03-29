#include<SoftwareSerial.h>
SoftwareSerial SUART(4, 5); //SRX=Dpin-D2; STX-DPin-D1
//-------------------------
int temperature;
int humidity;
//--------------------------------------------

void setup()
{
  Serial.begin(115200); //enable Serial Monitor
  SUART.begin(115200); //enable SUART Port

}

void loop()
{
  byte n = SUART.available(); //n != 0 means a character has arrived
  if (n != 0)
  {
    char x = SUART.read();  //read character
    Serial.print(x);        //show character on Serial Monitor
  }
}

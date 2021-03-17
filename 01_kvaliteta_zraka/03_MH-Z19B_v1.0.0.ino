// Vcc - 5V
// GND - GND
// RX (Blue) - A0
// TX (Green) - A1

#include <SoftwareSerial.h>

SoftwareSerial co2_Serial(A0, A1); // RX, TX

byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
unsigned char response[9]; 
unsigned long ppm;


void setup() {
  Serial.begin(9600); 
  co2_Serial.begin(9600); 
}

void loop() {
  co2_Serial.write(cmd,9);
  co2_Serial.readBytes(response, 9);
  unsigned int responseHigh = (unsigned int) response[2];
  unsigned int responseLow = (unsigned int) response[3];
  ppm = (256*responseHigh)+responseLow;


  Serial.println(ppm);
  Serial.println("-----------");
  delay(10000);
}

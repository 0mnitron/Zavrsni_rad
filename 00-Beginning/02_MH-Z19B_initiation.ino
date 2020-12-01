// PWM - pin 10
// Vcc - 5V
// GND - GND
// RX (Blue) - A0
// TX (Green) - A1

#include <SoftwareSerial.h>
#define pwmPin 10
SoftwareSerial mySerial(A0, A1); // RX, TX

byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
unsigned char response[9]; 
unsigned long th, tl,ppm, ppm2, ppm3 = 0;



void setup() {
  Serial.begin(9600); 
  mySerial.begin(9600); 
  pinMode(pwmPin, INPUT);
}

void loop() {
  mySerial.write(cmd,9);
  mySerial.readBytes(response, 9);
  unsigned int responseHigh = (unsigned int) response[2];
  unsigned int responseLow = (unsigned int) response[3];
  ppm = (256*responseHigh)+responseLow;


  //CO2 via pwm
  do {
    th = pulseIn(pwmPin, HIGH, 1004000) / 1000;
    tl = 1004 - th;
    ppm2 = 2000 * (th-2)/(th+tl-4);
    ppm3 = 5000 * (th-2)/(th+tl-4);
  } while (th == 0);

  Serial.println(ppm);
  Serial.println(th);
  Serial.println(ppm2);
  Serial.println(ppm3);
  Serial.println("-----------");
  delay(5000);
}
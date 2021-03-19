/* Test sketch for Adafruit PM2.5 sensor with UART */

// pin1 - +5V
// pin2 - GND
// pin5 - D5

// https://learn.adafruit.com/pm25-air-quality-sensor?view=all
 
#include "Adafruit_PM25AQI.h"
#include <SoftwareSerial.h>

SoftwareSerial pm25_Serial(5, 6);
 
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();  // aqi = Air Quality Index
 
void setup() {
  // Wait for serial monitor to open
  Serial.begin(115200);
  while (!Serial) delay(10);
 
  Serial.println("Adafruit PMSA003I Air Quality Sensor");
 
  // Wait one second for sensor to boot up!
  delay(1000);
 
  // If using serial, initialize it and set baudrate before starting!
  pm25_Serial.begin(9600);
 
  // connect to the sensor over software serial
  if (! aqi.begin_UART(&pm25_Serial)) {  
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }
 
  Serial.println("PM25 found!");
}
 
void loop() {
  PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    delay(500);  // try again in a bit!
    return;
  }
  
  Serial.println("AQI reading success");

  
  
  Serial.println(F("Concentration Units (environmental)"));  // koncentracija u ug/m3
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
  
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);  // broj čestica u 0.1L zraka
  Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
  Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
  Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
  Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
  Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
  Serial.println(F("---------------------------------------"));
 
  delay(10000);
}

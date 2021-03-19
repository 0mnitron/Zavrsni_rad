// NodeMCUv3
// uno pin 2 - NodeMCU pin D1
// uno pin 3 - preko otpornika na NodeMCU pin D2 

// PM2.5
// pin1 - +5V
// pin2 - GND
// pin5 - D5 (Tx)

// CO2
// Vcc - 5V
// GND - GND
// RX (Blue) - A1 - preko otpornika 
// TX (Green) - A0

#include "Adafruit_PM25AQI.h"
#include <SoftwareSerial.h>
#include "ArduinoJson.h"

SoftwareSerial pm25_Serial(5, 6);  // Rx, Tx  - Rx ne koristimo
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();  // aqi = Air Quality Index

SoftwareSerial co2_Serial(A0, A1); // RX, TX
byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
unsigned char response[9]; 
unsigned long ppm;

SoftwareSerial NodeMCU(2,3);  // RX-2; TX-3

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
 
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

  co2_Serial.begin(9600);
  NodeMCU.begin(115200);
}

void loop(){
  
  // PM 2.5
  pm25_Serial.begin(9600);
  PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    delay(500);  // try again in a bit!
    return;
  }
  Serial.println("AQI reading success");
  pm25_Serial.end();
  
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


  // CO2
  co2_Serial.begin(9600);
  co2_Serial.write(cmd,9);
  co2_Serial.readBytes(response, 9);
  co2_Serial.end();
  unsigned int responseHigh = (unsigned int) response[2];
  unsigned int responseLow = (unsigned int) response[3];
  ppm = (256*responseHigh)+responseLow;

  Serial.println(ppm);
  Serial.println("-----------");


  NodeMCU.begin(115200);
  StaticJsonDocument<100> doc;
  doc["pm10"] = data.pm10_env;
  doc["pm25"] = data.pm25_env;
  doc["pm100"] = data.pm100_env;
  doc["pm_p03"] = data.particles_03um;
  doc["pm_p05"] = data.particles_05um;
  doc["pm_p10"] = data.particles_10um;
  doc["pm_p25"] = data.particles_25um;
  doc["pm_p50"] = data.particles_50um;
  doc["pm_p100"] = data.particles_100um;
  doc["ppm"] = ppm;
  serializeJson(doc, NodeMCU);
  NodeMCU.end();
    
  delay(10000); 
}

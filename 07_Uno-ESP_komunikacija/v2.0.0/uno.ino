// A0 - 3.3V - GND
// 2(rx) - 3(tx)

#include <SoftwareSerial.h>
#include "ArduinoJson.h"

SoftwareSerial NodeMCU(2,3);  // RX-2; TX-3

#include <DHT.h>
#define DHTPIN A0     
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  Serial.begin(9600);
  while (!Serial) continue;
  
  NodeMCU.begin(115200);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print("\t Vlaga: ");
  Serial.print(h);
  Serial.print("\n");

  
  StaticJsonDocument<100> doc;  
  doc ["temperature"] = t;
  doc ["humidity"] = h;
  serializeJson(doc, NodeMCU);

  
  delay(5000);
}

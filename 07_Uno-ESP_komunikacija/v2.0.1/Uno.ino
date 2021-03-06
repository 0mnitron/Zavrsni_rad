#include <SoftwareSerial.h>
#include "ArduinoJson.h"

SoftwareSerial NodeMCU(2,3);  // RX-2; TX-3

// DHT senzor
#include <DHT.h>
#define DHTPIN A0     
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

 
void setup() {
  // inicijalizacija HardwareSerial komunikacije
  Serial.begin(9600);
  while (!Serial) continue;

  // inicijalizacija SoftwareSerial komunikacije
  NodeMCU.begin(115200);

  // inicijalizacija DHT senzora
  dht.begin();
}

void loop() {
  // Spremanje podataka sa senzora
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // ispis podataka sa senzora
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print("\t Vlaga: ");
  Serial.print(h);
  Serial.print("\n");

  // serijalizacija podataka i slanje na NodeMCU
  StaticJsonDocument<100> doc;  
  doc ["temperature"] = t;
  doc ["humidity"] = h;
  serializeJson(doc, NodeMCU);

  
  delay(5000);
}

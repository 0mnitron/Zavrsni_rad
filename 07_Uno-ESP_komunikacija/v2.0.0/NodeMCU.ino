#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial Uno(4,5);  //Rx; Tx
 
void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  
  Uno.begin(115200);
}

void loop() {
  if(Uno.available()){
    StaticJsonDocument<300> doc;
    deserializeJson(doc, Uno);  
    float t = doc["temperature"];
    float h = doc["humidity"];

    Serial.println("Temperatura \t vlaga");
    Serial.print(t);
    Serial.print("\t");
    Serial.print(h);
    Serial.println("\n----------------------------");
  }
}

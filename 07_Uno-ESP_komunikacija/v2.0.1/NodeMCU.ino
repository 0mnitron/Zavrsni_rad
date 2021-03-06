// https://arduinojson.org/v6/how-to/do-serial-communication-between-two-boards/

#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial Uno(4,5);  //Rx; Tx
 
void setup() {
  Serial.begin(115200);
  while (!Serial) continue;

  // komunikacija s Arduino Uno
  Uno.begin(115200);
}

void loop() {
  // ako je Arduino Uno poslao neke podatke
  if(Uno.available()){
    // osigurana veličina prostora za podatke je 300 byta  -> prilagoditi poslije
    StaticJsonDocument<300> doc;
    // provjeriti da li je deserijalizacija uspješno obavljena
    DeserializationError err = deserializeJson(doc, Uno);  

    // ako je uspješno obavljena:
    if(err == DeserializationError::Ok){
      float t = doc["temperature"];
      float h = doc["humidity"];

      Serial.println("Temperatura \t vlaga");
      Serial.print(t);
      Serial.print("\t");
      Serial.print(h);
      Serial.println("\n----------------------------");
    }
    // ako nije:
    else{
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());

      // Flush all bytes in the "Uno" serial port buffer
      // zapravo ovaj dio koda isprazni buffer tako što pročita podatke, pa novi podatci mogu doći
      while (Uno.available() > 0){
        Uno.read();
      }
    }
  }
}

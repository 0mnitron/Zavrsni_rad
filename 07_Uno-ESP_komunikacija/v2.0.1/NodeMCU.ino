// https://arduinojson.org/v6/how-to/do-serial-communication-between-two-boards/

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
    DeserializationError err = deserializeJson(doc, Uno);  
    
    if(err == DeserializationError::Ok){
      float t = doc["temperature"];
      float h = doc["humidity"];

      Serial.println("Temperatura \t vlaga");
      Serial.print(t);
      Serial.print("\t");
      Serial.print(h);
      Serial.println("\n----------------------------");
    }
    else{
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());

      // Flush all bytes in the "link" serial port buffer
      while (Uno.available() > 0)
        Uno.read();
    }
  }
}

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include "ArduinoJson.h"

const char* ssid = "ZeInternet_2";
const char* password = "sifrarutera";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://65.21.1.49:5000/";  // <--------------


SoftwareSerial Uno(4,5);  //Rx; Tx

void setup() {
  // pokretanje Serial Monitora
  Serial.begin(115200);
  while (!Serial) continue;

  // komunikacija s Arduino Uno
  Uno.begin(115200);

  // pokretanje WiFi-a
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
}


void loop() {
  // ako je Arduino Uno poslao neke podatke
  if(Uno.available()){
    // osigurana veličina prostora za podatke je 400 byta  -> prilagoditi poslije
    StaticJsonDocument<400> doc;
    // provjeriti da li je deserijalizacija uspješno obavljena
    DeserializationError err = deserializeJson(doc, Uno);  

    // ako je uspješno obavljena:
    if(err == DeserializationError::Ok){

      // prazni string
      String json;
      // serijaliziramo doc u string u varijablu koja se zove json  --> PROMIJENITI u neko bolje ime
      serializeJson(doc, json);

      
      //Provjerava povezanost s WiFi-om
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
      
        // Your Domain name with URL path or IP address with path
        http.begin(serverName);

        // HTTP request with a content type: application/json
        http.addHeader("Content-Type", "application/json");
        // Send HTTP POST request        
        int httpResponseCode = http.POST(json);

        // Serial Monitor
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);  
        // Read response
        Serial.print(http.getString());
  
        // Free resources
        http.end();
      }
      else {
        Serial.println("WiFi Disconnected");
      }

/*
      // Serial Monitor
      
      int pm10 = doc["pm10"];
      int pm25 = doc["pm25"]; 
      int pm100 = doc["pm100"]; 
      int pm_p03 = doc["pm_p03"]; 
      int pm_p05 = doc["pm_p05"]; 
      int pm_p10 = doc["pm_p10"]; 
      int pm_p25 = doc["pm_p25"]; 
      int pm_p50 = doc["pm_p50"];
      int pm_p100 = doc["pm_p100"];
      int ppm = doc["ppm"];
      float temp = doc["temperature"];
      float pressure = doc["pressure"];
      float hum = doc["humidity"];
      float gas = doc["gas"];
      float altitude_ = doc["altitude"];
      int svjetlina = doc["svjetlina"];

      Serial.println(F("Concentration Units (environmental)"));  // koncentracija u ug/m3
      Serial.println(F("---------------------------------------"));
      Serial.print(F("PM 1.0: ")); Serial.print(pm10);
      Serial.print(F("\t\tPM 2.5: ")); Serial.print(pm25);
      Serial.print(F("\t\tPM 10: ")); Serial.println(pm100);
  
      Serial.println(F("---------------------------------------"));
      Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(pm_p03);  // broj čestica u 0.1L zraka
      Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(pm_p05);
      Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(pm_p10);
      Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(pm_p25);
      Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(pm_p50);
      Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(pm_p100);
      Serial.println(F("---------------------------------------"));

      Serial.println(ppm);
      Serial.println("-----------");

      Serial.print("Temperature = ");
      Serial.print(temp);
      Serial.println(" *C");

      Serial.print("Pressure = ");
      Serial.print(pressure);
      Serial.println(" hPa");

      Serial.print("Humidity = ");
      Serial.print(hum);
      Serial.println(" %");

      Serial.print("Gas = ");
      Serial.print(gas);
      Serial.println(" KOhms");

      Serial.print("Approx. Altitude = ");
      Serial.print(altitude_);
      Serial.println(" m");

      Serial.print("Svjetlina:\t");
      Serial.print(svjetlina);
      Serial.println();
*/
      
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

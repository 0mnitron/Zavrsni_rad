#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include "ArduinoJson.h"

// naziv i lozinka WiFi mreže
const char* ssid = "ZeInternet_2";
const char* password = "sifrarutera";

//Your Domain name with URL path or IP address with path
// IP Adresa servera
const char* serverName = "http://65.21.1.49:5000/";  


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
      String podatci;
      // serijaliziramo "doc" u string u varijablu koja se zove "podatci"  
      serializeJson(doc, podatci);

      
      //Provjerava povezanost s WiFi-om
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
      
        http.begin(serverName);

        // HTTP request with a content type: application/json
        // HTTP zahtjev s podatcima tipa json
        http.addHeader("Content-Type", "application/json");
        // Šalje HTTP POST zahtjev         
        int httpResponseCode = http.POST(podatci);

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
    }
    // ako deserijalizacija nije uspješno obavljena:
    else{
      // Ispisuje grešku na Serial Monitoru
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());

      // Brišu se svi podatci iz akumulatora za serijsku komunikaciju, kako bi mogli novi, ispravni doći
      while (Uno.available() > 0){
        Uno.read();
      }
    }
  }
}

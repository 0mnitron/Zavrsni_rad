#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include "ArduinoJson.h"
#include <MD_CirQueue.h>

// naziv i lozinka WiFi mreže
#define ssid "ZeInternet_2"
#define password "sifrarutera"

//Your Domain name with URL path or IP address with path
// IP Adresa servera 
#define serverName "http://65.21.1.49:5000/"

// Veličina datoteke - definirana u (Uno)NodeMCU programu (StaticJsonDocument<DATA_SIZE> doc;)
// Veličina - Bytes
#define DATA_SIZE 600

// Broj kopija u bufferu
#define QUEUE_SIZE 20

// Define a queue.
MD_CirQueue Q(QUEUE_SIZE, DATA_SIZE);


SoftwareSerial Uno(4,5);  //Rx, Tx

void setup() {
  // pokretanje Serial Monitora
  //Serial.begin(115200);
  //while (!Serial) continue;

  // komunikacija s Arduino Uno
  Uno.begin(115200);

  // pokretanje WiFi-a
  WiFi.begin(ssid, password);
  //Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.print("Connected to WiFi network with IP Address: ");
  //Serial.println(WiFi.localIP());

  // Buffer
  Q.begin();
  //  push() with a full queue will overwrite the oldest item in the queue
  Q.setFullOverwrite(true);
}


void loop() {
  // ako postoji povezanost s internetom i buffer nije prazan, onda ga isprazni
  if(WiFi.status()== WL_CONNECTED && !Q.isEmpty()){
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    while(!Q.isEmpty()){
      char buffer_data[DATA_SIZE];
      Q.pop((uint8_t *)&buffer_data);
      //Serial.print("\nPopped ");
      http.POST(buffer_data);
    }
    http.end();
  }
      
  // ako je Arduino Uno poslao neke podatke
  if(Uno.available()){
    
    // osigurana veličina prostora za podatke je 550 byta  -> prilagoditi poslije
    StaticJsonDocument<DATA_SIZE> doc;        //-->  doc size i DATA_SIZE bi možda trebali biti drugačije veličine
    
    // provjeriti da li je deserijalizacija uspješno obavljena
    DeserializationError err = deserializeJson(doc, Uno);  

    // ako je uspješno obavljena:
    if(err == DeserializationError::Ok){

      // varijabla char tipa u koju se spremaju podatci s Una
      char sensor_data[DATA_SIZE];
      // serijaliziramo "doc" u char varijablu sensor_data  
      serializeJson(doc, sensor_data);

         
      //Provjerava povezanost s WiFi-om
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
      
        http.begin(serverName);

        // HTTP request with a content type: application/json
        // HTTP zahtjev s podatcima tipa json
        http.addHeader("Content-Type", "application/json");
        // Šalje HTTP POST zahtjev         
        int httpResponseCode = http.POST(sensor_data);

        // Serial Monitor
       // Serial.print("HTTP Response code: ");
        //Serial.println(httpResponseCode);  
        // Read response
        //Serial.print(http.getString());
  
        // Free resources
        http.end();
      }
      // Ukoliko je pukla povezanost s WiFi-jem
      else{
        //Serial.println("WiFi Disconnected");
        
        bool b;
        b = Q.push((uint8_t *)&sensor_data);
        //Serial.print("\nPush ");
        //Serial.print(b ? " ok" : " fail");
      }     
    }
    // ako deserijalizacija nije uspješno obavljena:
    else{
      // Ispisuje grešku na Serial Monitoru
      //Serial.print("deserializeJson() returned ");
      //Serial.println(err.c_str());

      // Brišu se svi podatci iz akumulatora za serijsku komunikaciju, kako bi mogli novi, ispravni doći
      while (Uno.available() > 0){
        Uno.read();
      }
    }
  }
}

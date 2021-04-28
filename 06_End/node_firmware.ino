#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include "ArduinoJson.h"
#include <MD_CirQueue.h>

// naziv i lozinka WiFi mreže
#define ssid "ZeInternet"
#define password "sifrarutera"

// IP Adresa servera 
#define serverName "http://65.21.1.49:5000/"

// Veličina podataka pristiglih s Arduina
#define DATA_SIZE 600

// Broj kopija u akumulatoru
#define QUEUE_SIZE 20

// Definira akumulator
MD_CirQueue Q(QUEUE_SIZE, DATA_SIZE);


SoftwareSerial Uno(4,5);  //Rx, Tx

void setup() {
  
  // Komunikacija s Arduino Uno
  Uno.begin(115200);

  // Pokretanje WiFi-a
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Inicijalizacija akumulatora
  Q.begin();
  //  push() funkcija će prebrisati najstariju vrijednost u akumulatoru novom
  Q.setFullOverwrite(true);
}


void loop() {
  // Ako postoji povezanost s internetom i akumulator nije prazan, onda ga isprazni
  if(WiFi.status()== WL_CONNECTED && !Q.isEmpty()){
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    while(!Q.isEmpty()){
      char buffer_data[DATA_SIZE];
      Q.pop((uint8_t *)&buffer_data);
      http.POST(buffer_data);
    }
    http.end();
  }
      
  // Ako je Arduino Uno poslao neke podatke
  if(Uno.available()){
    
    // Osigurana veličina prostora za podatke
    StaticJsonDocument<DATA_SIZE> doc;        
    
    // Provjeriti da li je deserijalizacija uspješno obavljena
    DeserializationError err = deserializeJson(doc, Uno);  

    // Ako je uspješno obavljena:
    if(err == DeserializationError::Ok){

      // Varijabla char tipa u koju se spremaju podatci s Una
      char sensor_data[DATA_SIZE];
      // Serijaliziramo "doc" u char varijablu sensor_data  
      serializeJson(doc, sensor_data);

         
      //Provjerava povezanost s WiFi uređajem
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
      
        http.begin(serverName);

        // HTTP zahtjev s podatcima tipa json
        http.addHeader("Content-Type", "application/json");
        // Šalje HTTP POST zahtjev         
        http.POST(sensor_data);

        // Oslobađaju se resursi
        http.end();
      }
      // Ukoliko je pukla povezanost s internetom
      else{
        // Podatci se spremaju u akumulator    
        Q.push((uint8_t *)&sensor_data);
      }     
    }
    // Ako deserijalizacija nije uspješno obavljena:
    else{
      // Brišu se svi podatci iz memorije za serijsku komunikaciju, kako bi novi
      // mogli doći
      while (Uno.available() > 0){
        Uno.read();
      }
    }
  }
}

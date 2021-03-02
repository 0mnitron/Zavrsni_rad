#include<SoftwareSerial.h>
SoftwareSerial SUART(2, 3); //SRX=Dpin-2; STX-DPin-3

#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN A0

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  SUART.begin(115200); //enable SUART Port
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print("\t Vlaga: ");
  Serial.print(humidity);
  Serial.print("\n");

  //----Send Temp and Humidity signal to NodeMCU via SUART port----
  SUART.print('<');                   //<(start mark)
  SUART.print((int)temperature,DEC);
  SUART.print(',');                   //,(data seperator)
  SUART.print((int)humidity, DEC);
  SUART.print('>'); 

  delay(2000);
}

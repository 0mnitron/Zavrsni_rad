// NodeMCUv3
// uno pin 2 - NodeMCU pin D1
// uno pin 3 - preko otpornika na NodeMCU pin D2 

// PM2.5
// pin1 - +5V
// pin2 - GND
// pin5 - D5 (Tx)

// CO2
// Vcc - 5V
// GND - GND
// RX (Blue) - A1 - preko otpornika 
// TX (Green) - A0

// BME680
// Vcc - 3.3v
// GND - GND
// A5 - SCL
// A4 - SDA

// BME 280 - SPI communication protocol
// Vcc = 3.3V
// GND = GND
// SCL (SCK) = 13
// SDA (SDI) = 11
// SCB (CS) = 10
// SD0 = 12

// Clock
// Vcc - 3.3-5.5V
// GND -GND
// SCL - SCL (1.->)
// SDA - SDA (2.->)

// fotodioda
// A2
// 1kohm na gnd; 5v

#include "Adafruit_PM25AQI.h"
#include <SoftwareSerial.h>
#include "ArduinoJson.h"
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BME680.h"
#include "Adafruit_BME280.h"
#include "RTClib.h"

// PMS5003
SoftwareSerial pm25_Serial(5, 6);  // Rx, Tx  - Rx ne koristimo
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();  // aqi = Air Quality Index

// MH-Z19B
SoftwareSerial co2_Serial(A0, A1); // RX, TX
byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
unsigned char response[9]; 
unsigned long ppm;

// BME680
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME680 bme680; // I2C

// BME280
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
Adafruit_BME280 bme280(BME_CS);

// NodeMCU
SoftwareSerial NodeMCU(2,3);  // RX-2; TX-3

// Clock
RTC_DS3231 rtc;


void setup() {
  //Serial.begin(115200);
  //while (!Serial) continue;

  // PM2.5
//  Serial.println("Adafruit PMSA003I Air Quality Sensor");
  // jedna sekunda da se senzor "upali"
  delay(1000);
 
  pm25_Serial.begin(9600);
 
  if (! aqi.begin_UART(&pm25_Serial)) {  
//    Serial.println("Could not find PM 2.5 sensor!");
    while (1);
  }
//  Serial.println("PM25 found!");

  // CO2
  co2_Serial.begin(9600);
  
  // ESP
  NodeMCU.begin(115200);

  // BME680
  if (!bme680.begin()) {
//    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while(1);
  }
  
  bme680.setTemperatureOversampling(BME680_OS_8X);
  bme680.setHumidityOversampling(BME680_OS_2X);
  bme680.setPressureOversampling(BME680_OS_4X);
  bme680.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme680.setGasHeater(320, 150); // 320*C za 150 ms

  // BME280
  bme280.begin();
  
  // svjetlina
  pinMode(A2, INPUT);
  
  // Clock
  if (! rtc.begin()) {
//    Serial.println("Couldn't find RTC");
    while (1);
  }

  // lostPower() funkcija isčitava vrijednost unutarnjeg registra i provjerava da li je izgubio
  // vrijednost vremena
  if (rtc.lostPower()) {
//    Serial.println("RTC lost power, let's set the time!");
    // postavlja vrijednost vremena kada je program strojno preveden
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop(){
  // PM 2.5
  unsigned long pm25_millis = millis();
  bool aqi_reading = true;
  pm25_Serial.begin(9600);
  PM25_AQI_Data data;
  do{
//    Serial.println("Could not read from AQI");
    delay(500);
    if(millis() >= pm25_millis+5000){
//      Serial.print("\nAbort AQI reading.\n");
      aqi_reading = false;
      break;
    }
  }while(!aqi.read(&data));
//  Serial.println("AQI reading success");
  pm25_Serial.end();
  

  // CO2
  co2_Serial.begin(9600);
  co2_Serial.write(cmd,9);
  co2_Serial.readBytes(response, 9);
  co2_Serial.end();
  unsigned int responseHigh = (unsigned int) response[2];
  unsigned int responseLow = (unsigned int) response[3];
  ppm = (256*responseHigh)+responseLow;


  // BME680  
  if (! bme680.performReading()) {
//    Serial.println("Failed to perform BME680 reading.");
  }

  // fotodioda
  int svjetlina = analogRead(A2);

  // Clock 
  DateTime now = rtc.now();

   
  // NodeMCU
  NodeMCU.begin(115200);
  StaticJsonDocument<300> doc;
  doc["wsid"] = "hr.10000.0";
  
  doc["time"] = now.unixtime();
  
  if(aqi_reading){
    doc["pm10"] = data.pm10_env;
    doc["pm25"] = data.pm25_env;
    doc["pm100"] = data.pm100_env;
    doc["pm_p03"] = data.particles_03um;
    doc["pm_p05"] = data.particles_05um;
    doc["pm_p10"] = data.particles_10um;
    doc["pm_p25"] = data.particles_25um;
    doc["pm_p50"] = data.particles_50um;
    doc["pm_p100"] = data.particles_100um;
  }
  
  doc["ppm"] = ppm;
  
  doc["temperature"] = bme680.temperature;
  doc["pressure"] = bme680.pressure / 100.0F;
  doc["humidity"] = bme680.humidity;
  doc["gas"] = bme680.gas_resistance / 1000.0F;
  
  doc["brightness"] = svjetlina;

  doc["s_temp"] = bme280.readTemperature();
  doc["s_pressure"] = bme280.readPressure() / 100.0F;
  doc["s_hum"] = bme280.readHumidity();
  serializeJson(doc, NodeMCU);
  NodeMCU.end();


/*
  // Serial Monitor
  // PM2.5
  if(aqi_reading){
    Serial.println(F("Concentration Units (environmental)"));  // koncentracija u ug/m3
    Serial.println(F("---------------------------------------"));
    Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
    Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
    Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
  
    Serial.println(F("---------------------------------------"));
    Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);  // broj čestica u 0.1L zraka
    Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
    Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
    Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
    Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
    Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
    Serial.println(F("---------------------------------------"));
  }
  
  // BME280
  Serial.print("Temperature = ");
  Serial.print(bme280.readTemperature());
  Serial.println(" *C");
  Serial.print("Pressure = ");
  Serial.print(bme280.readPressure() / 100.0F);
  Serial.println(" hPa");
  Serial.print("Approx. Altitude = ");
  Serial.print(bme280.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  Serial.print("Humidity = ");
  Serial.print(bme280.readHumidity());
  Serial.println(" %");
  Serial.println();

  
  // CO2
  Serial.println(ppm);
  Serial.println("-----------");

  // BME680
  Serial.print("Temperature = ");
  Serial.print(bme680.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme680.pressure / 100.0F);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme680.humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme680.gas_resistance / 1000.0F);
  Serial.println(" KOhms");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme680.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  
  // Svjetlina
  Serial.print("Svjetlina:\t");
  Serial.print(svjetlina);
  Serial.println();
  
  // Clock 
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print("  ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  Serial.println();
  */
  
  delay(30000);
}

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
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

// MH-Z19B
SoftwareSerial co2_Serial(A0, A1); // RX, TX
byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
unsigned char response[9]; 
unsigned long ppm;

// BME680 - I2C komunikacija
Adafruit_BME680 bme680;

// BME280 - SPI komunikacija
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
Adafruit_BME280 bme280(BME_CS);

// NodeMCU
SoftwareSerial NodeMCU(2,3);  // RX, TX

// Modul stvarnog vremena 
RTC_DS3231 rtc;


void setup() {
  // PMS5003
  pm25_Serial.begin(9600);
  while(! aqi.begin_UART(&pm25_Serial)){
    delay(100);
  }

  // MH-Z19B
  co2_Serial.begin(9600);
  
  // NodeMCU
  NodeMCU.begin(115200);

  // BME680
  while(!bme680.begin()){
    delay(100);
  }
  // Konfiguracija senzora
  bme680.setTemperatureOversampling(BME680_OS_8X);
  bme680.setHumidityOversampling(BME680_OS_2X);
  bme680.setPressureOversampling(BME680_OS_4X);
  bme680.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme680.setGasHeater(320, 150); // 320*C na 150 ms

  // BME280
  bme280.begin();
  
  // Svjetlina
  pinMode(A2, INPUT);
  
  // Sat
  while(! rtc.begin()){
    delay(100);
  }

  // lostPower() funkcija isčitava vrijednost unutarnjeg registra i provjerava da li je izgubio
  // pojam vremena
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop(){
  // PMS5003
  unsigned long pm25_millis = millis();
  bool aqi_reading = true;
  pm25_Serial.begin(9600);
  PM25_AQI_Data data;
  do{
    delay(500);
    if(millis() >= pm25_millis+5000){
      aqi_reading = false;
      break;
    }
  }while(!aqi.read(&data));
  pm25_Serial.end();
  

  // MH-Z19B
  co2_Serial.begin(9600);
  co2_Serial.write(cmd,9);
  co2_Serial.readBytes(response, 9);
  co2_Serial.end();
  unsigned int responseHigh = (unsigned int) response[2];
  unsigned int responseLow = (unsigned int) response[3];
  ppm = (256*responseHigh)+responseLow;


  // BME680  
  bme680.performReading();

  // Fotootpornik
  int svjetlina = analogRead(A2);

  // Sat 
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
  
  delay(30000);
}

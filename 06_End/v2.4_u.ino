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
#include "RTClib.h"

SoftwareSerial pm25_Serial(5, 6);  // Rx, Tx  - Rx ne koristimo
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();  // aqi = Air Quality Index

SoftwareSerial co2_Serial(A0, A1); // RX, TX
byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
unsigned char response[9]; 
unsigned long ppm;

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME680 bme; // I2C

SoftwareSerial NodeMCU(2,3);  // RX-2; TX-3

// Clock
RTC_DS3231 rtc;


void setup() {
  Serial.begin(115200);
  while (!Serial) continue;

  // PM2.5
  Serial.println("Adafruit PMSA003I Air Quality Sensor");
  // Wait one second for sensor to boot up!
  delay(1000);
 
  // If using serial, initialize it and set baudrate before starting!
  pm25_Serial.begin(9600);
 
  // connect to the sensor over software serial
  if (! aqi.begin_UART(&pm25_Serial)) {  
    Serial.println("Could not find PM 2.5 sensor!");
    while (1);
  }
  Serial.println("PM25 found!");

  // CO2
  co2_Serial.begin(9600);
  
  // ESP
  NodeMCU.begin(115200);

  // BME680
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  // svjetlina
  pinMode(A2, INPUT);
  
  // Clock
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  // lostPower() function reads the DS3231’s internal I2C registers to check if the chip has lost track 
  // of time. If the function returns true, we can then set the date & time.
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop(){
  // fotodioda
  int svjetlina = analogRead(A2);
  
  // PM 2.5
  pm25_Serial.begin(9600);
  PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    delay(500);  // try again in a bit!
    return;
  }
  Serial.println("AQI reading success");
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
  if (! bme.performReading()) {
    Serial.println("Failed to perform BME680 reading.");
    return;
  }
  
  float temp = bme.temperature;
  float pressure = bme.pressure / 100.0;
  float hum = bme.humidity;
  float gas = bme.gas_resistance / 1000.0;
  float altitude_ = bme.readAltitude(SEALEVELPRESSURE_HPA);
  

  // Clock 
  DateTime now = rtc.now();

    
  // NodeMCU
  NodeMCU.begin(115200);
  StaticJsonDocument<240> doc;
  doc["time"] = now.toString("YYYY-MM-DDThh:mm:ss");
  
  doc["pm10"] = data.pm10_env;
  doc["pm25"] = data.pm25_env;
  doc["pm100"] = data.pm100_env;
  doc["pm_p03"] = data.particles_03um;
  doc["pm_p05"] = data.particles_05um;
  doc["pm_p10"] = data.particles_10um;
  doc["pm_p25"] = data.particles_25um;
  doc["pm_p50"] = data.particles_50um;
  doc["pm_p100"] = data.particles_100um;
  
  doc["ppm"] = ppm;
  
  doc["temperature"] = temp;
  doc["pressure"] = pressure;
  doc["humidity"] = hum;
  doc["gas"] = gas;
  doc["altitude"] = altitude_;

  doc["brightness"] = svjetlina;
  serializeJson(doc, NodeMCU);
  NodeMCU.end();



  // Serial Monitor

  // PM2.5
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

  // CO2
  Serial.println(ppm);
  Serial.println("-----------");

  // BME680
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
 
  delay(30000); 
}

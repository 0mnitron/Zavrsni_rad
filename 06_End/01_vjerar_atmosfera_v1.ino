// BME 280 - I2C communication protocol
// A5 - SCL
// A4 - SDA
// Vcc - 3.3V
// GND - GND

//5v - GND - 3 

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

#define pin_mag 3
unsigned long previousMillis = 0;
#define wind_interval 5000  // vrijeme koliko dugo će se očitavati brzina vjetra
#define radius 0.002  // ubaciti točnu veličinu radiusa u metrima

float wind_speed = 0;

Adafruit_BME280 bme;

void setup() {
  pinMode(pin_mag, INPUT_PULLUP);
  Serial.begin(9600);
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() { 
  temp_hum_pres_f();
  wind_speedf();
  //wind_direction();
  // __padaline();__
  //air_quailty();
}


void wind_speedf(){
  int n = 0;
  unsigned long time_beg=0, time_end=0;

  previousMillis = millis();
  
  while(millis() - previousMillis <= wind_interval){  // dok traje interval mjerenja brzine vjetra (5 sek)
    if(digitalRead(pin_mag) == 0 && (millis() - previousMillis) > (wind_interval-1000) ){  // megnet prvi put dođe u početni položaj i ima još barem 4 sekunde vremena
      time_beg = millis();
    
      while(millis() - time_beg <= (wind_interval-1000)){  // period = 4 sekunde
        if(digitalRead(pin_mag) == 0){  // ako se magnet nađe u početnom položaju      
          n++;
          time_end = millis();
        }
      }
    }
    else{
      wind_speed = 0;
    }  
  }

  if(n != 0){
    int period = time_end - time_beg;  // ms
    float frequency = n / period * 1000;  // ms -> s (Hz)
    wind_speed = radius * 2 * 3.1416 * frequency;  // m/s
    wind_speed = wind_speed * 3.6;  // km/h 
  }

  Serial.print("Wind speed is: ");
  Serial.print(wind_speed);
  Serial.print(" km/h \n");
  Serial.println();
}

void temp_hum_pres_f(){
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println("*C");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println("hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println("m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println("%");
  Serial.println();
}

#define pin_mag 3
unsigned long previousMillis = 0;
#define wind_interval 7000  // vrijeme koliko dugo će se očitavati brzina vjetra
#define radius 0.002  // ubaciti točnu veličinu radiusa u metrima

float wind_speed = 0;

void setup() {
  pinMode(pin_mag, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() { 
  //temp_hum_pres();
  wind_speedf();
  //wind_direction();
  // __padaline();__
  //air_quailty();
}


void wind_speedf(){
  int n = 0;
  unsigned long time0=0, time_end=0;
                                                        
  unsigned long currentMillis = millis();
  
  while(currentMillis - previousMillis <= wind_interval){  // dok traje interval mjerenja brzine vjetra (7 sek)
    if(digitalRead(pin_mag) == 0 && (currentMillis - previousMillis) > 5000 ){  // megnet prvi put dođe u početni položaj i ima još barem 5 sekundi vremena
      time0 = millis();
    
      while(currentMillis - time0 <= 5000){  // period = 5 sekundi
        if(digitalRead(pin_mag) == 0){  // ako se magnet nađe u početnom položaju      
          n++;
          time_end = millis();
        }
        currentMillis = millis();
      }
    }
    else{
      wind_speed = 0;
    }
    
    currentMillis = millis();  
  }

  if(n != 0){
    int period = time_end - time0;  // ms
    float frequency = n / period * 1000;  // ms -> s (Hz)
    wind_speed = radius * 2 * 3.1416 * frequency;  // m/s
    wind_speed = wind_speed * 3.6;  // km/h
  }

  Serial.print("Wind speed is: ");
  Serial.print(wind_speed);
  Serial.print(" km/h \n");
  
  previousMillis = currentMillis;
}

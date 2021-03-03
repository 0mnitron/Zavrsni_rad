int pin_mag = A0;

void setup() {
  pinMode(pin_mag, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int AD_value  = analogRead(pin_mag);  // 0-1023
  
  int int_voltage = map(AD_value, 0, 1023, 0, 50);
  
  float voltage = int_voltage / 10.0;

  Serial.print("Vrijednost = ");
  Serial.print(voltage);
  Serial.print("\n");
  delay(300);
}

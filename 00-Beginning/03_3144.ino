int pin_mag = 3;

void setup() {
  pinMode(pin_mag, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int value  = digitalRead(pin_mag);

  Serial.print("Vrijednost = ");
  Serial.print(!value);
  Serial.print("\n");
  //delay(100);
}

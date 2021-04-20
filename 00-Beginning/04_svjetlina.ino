void setup() {
  pinMode(A2, INPUT);
  Serial.begin(9600);
  Serial.println("Svjetlina:\t");
}

void loop() {
  int svjetlina = analogRead(A2);
  Serial.println(svjetlina);
  delay(2000);
}

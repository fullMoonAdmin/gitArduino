const int voltPin = 36;

// variable for storing the potentiometer value
int voltValue = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Voltage Reader Online");
  delay(1000);
}

void loop() {
  // Reading potentiometer value
  voltValue = analogRead(voltPin);
  
  Serial.println(voltValue);
  delay(1000);
}

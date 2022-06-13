const int sensorPin = 36;
 
void setup() {
  Serial.begin(115200);
}
 
void loop() {
   
  int value = analogRead(sensorPin);
  Serial.println(value);
   
  delay(10);
}

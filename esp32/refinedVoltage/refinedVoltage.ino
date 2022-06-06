
#include <WiFi.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

const char ssid[] = SECRET_SSID;
const char password[] = SECRET_WIFIPASS;
const char mqtt_server[] = SECRET_MQTTSERVER;
const char mqtt_user[] = SECRET_MQTTUSER;
const char mqtt_password[] = SECRET_MQTTPASS;



//triggerPin
const int triggerPin = 18;
// Volt Pin
const int voltPin = 36;


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  // default settings
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);  
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
   Serial.println();
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    String finalVoltage;
    
    lastMsg = now;
    analogVoltage = analogRead(voltPin);
    
    char voltString[8];
    dtostrf(analogVoltage, 4, 1, voltString);

    //Publish voltage to MQTT server
    Serial.print("Voltage: ");
    Serial.println(voltString);
    //Change value of the string to whatever topic you want
    //Example: "puzzle 1"
    if (analogVoltage <= 0.0){
    client.publish("testRoom/testPuzzle", "cured" );
    }
  }
}

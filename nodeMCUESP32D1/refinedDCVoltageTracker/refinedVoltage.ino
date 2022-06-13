/*
This sketch is to measure the release of magnets for escape rooms using a ESP32 WROOM32 module with an analogue voltage sensor and an external trigger device like the PicoBoo +. 
Just hook in the analog sensor to the ground and hot wires of the magnet
*/
#include <WiFi.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

//values to connect to wifi and MQTT server
const char ssid[] = SECRET_SSID;
const char password[] = SECRET_WIFIPASS;
const char mqtt_server[] = SECRET_MQTTSERVER;
const char mqtt_user[] = SECRET_MQTTUSER;
const char mqtt_password[] = SECRET_MQTTPASS;

// Volt Pin
const int voltPin = 34;


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
    lastMsg = now;
    float analogVoltage; 
    analogVoltage = analogRead(voltPin);

    //set analog voltage value to a char array
    char voltString[8];
    dtostrf(analogVoltage, 4, 1, voltString);

    //Publish voltage to MQTT server
    Serial.print("Voltage: ");
    Serial.println(voltString);
    
    if (analogVoltage <= 0.0){

      //The first function parameter is the topic and the second is the message
      //Change these values to what ever you wish
      client.publish("testRoom/testPuzzle/cured", "true" );
      
    } else {
        client.publish("testRoom/testPuzzle/cured", "false" );
      }
  }
}

//DC Voltage Tracking using ESP32 and MQTT

#include <WiFi.h>
#include <PubSubClient.h>

// Replace the next variables with your SSID/Password combination

//enter WiFi Name 
const char* ssid = "";
const char* password = "";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// Volt Pin
const int voltPin = 36;

void setup() {
  // Open serial Port for local monitoring 
  Serial.begin(115200);
  delay(1000);
  //setup the wifi network
  setup_wifi();
  //Connect to MQTT server 
  client.setServer(mqtt_server, 1883);   
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

  //Serial Messages to confirm WiFi connection
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
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

    int voltage = 0;
    voltage = analogRead(voltPin);   
 
    // (and comment the previous temperature line)
    //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit
    
    // Convert the value to a char array
    char voltString[8];
    dtostrf(voltage, 4, 0, voltString);

    //Publish voltage to MQTT server
    Serial.print("Voltage: ");
    Serial.println(voltString);
    client.publish("voltage", voltString);
  }
}

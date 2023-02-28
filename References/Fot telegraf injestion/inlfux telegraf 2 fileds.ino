#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>




// Replace the next variables with your SSID/Password combination
const char* ssid = "WORKSTATION";
const char* password = "Superman_27";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.1.19";

WiFiClient espClient;
PubSubClient client(espClient);

// Create variable to hold mqtt messages
#define MSG_BUFFER_SIZE  (100)
char msg[MSG_BUFFER_SIZE];

// Connecting to the WIFI network
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
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

void reconnect() {
  
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = "ESP8266Client-"; clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str(), "mqtt", "")) 
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } 
    
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}


void loop() {
  // Connect to the mqtt client
  if (!client.connected()) 
  {
    reconnect();
  }
  
  client.loop();

  // Get the current object temperatur of the sensor
  float objt = 25;
  float objt2 = 156;
  
  
  // Create the message that will be send using mqtt
  String message = String("weather,location=us temperature="+String(objt));
  message.toCharArray(msg, message.length());
  Serial.println(msg);
  
  
  
  // Send the message on the sensors topic
  client.publish("philips/temperature", msg);

  delay(500);
}

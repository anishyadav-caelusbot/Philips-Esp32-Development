#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <Preferences.h>
Preferences preferences;

const char* ssid     = "WORKSTATION";
const char* password = "Superman_27";
// Add your MQTT Broker IP address, example:
const char* mqtt_broker = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
long lastMsg = 0;
char msg[50];
int value = 0;




void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "s7/reset") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
    preferences.clear();
    }
    else if(messageTemp == "off"){
      Serial.println("off");
     // digitalWrite(ledPin, LOW);
    }
  }
}





void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
    if (client.connect(client_id.c_str())) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("s7/reset");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" tryING again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

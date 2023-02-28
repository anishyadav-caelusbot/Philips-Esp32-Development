
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
StaticJsonDocument<200> doc;


const char* ssid = "WORKSTATION";
const char* password = "Superman_27";
const char* mqtt_server = "192.168.1.19";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int value = 0;


float temperature = 0;
float humidity = 0;

// LED Pin
const int ledPin = 4;

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;

  //
  // doc["air"] = "223";
  // doc["time"] = 1655371268;
  // doc["humidity"] = 35;
  // doc["region"] = "baddi";
  // doc["temperature"] = 34;


  doc["Time"] = "223";
  doc["Panel-ID"] = 1655371268;
  doc["Station-ID"] = 35;
  doc["Operation-ON-Cycle"] = 35;
  doc["Voltage"] = 35;
  doc["Current"] = 35;
  doc["Power"] = "baddi";
  doc["Temperature"] = 34;
  doc["Cycle-of-Thermostat"] = 34;
  doc["Test-Status"] = 34;
  doc["Test-Fault"] = 34;
  doc["Temp-Alert"] = 34;

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);
}
//


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
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "philips/output") {
    Serial.print("Changing output to ");
    if (messageTemp == "on") {
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if (messageTemp == "off") {
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("philips");
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
  // Connect to the mqtt client
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  char buffer[256];
  size_t n = serializeJson(doc, buffer);
  client.publish("philips/sensor", buffer, n);
}

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
StaticJsonDocument<200> doc;
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PZEM004Tv30.h>



const char* ssid = "WORKSTATION";
const char* password = "Superman_27";
const char* mqtt_server = "192.168.1.19";
String formattedDate;
char buffer[2048];
float voltage;
float current;
float power;
//multiply
float VMF_Factor = 2.0;
float WMF_Factor = 2.0;
//substract
float VSF_Factor = 0.20;
float WSF_Factor = 0.30;



WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
PZEM004Tv30 pzem3(&Serial2,27,14,0xE4);




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
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("philips/sensor");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Function that gets current epoch time
unsigned long getTime() 
{
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}


void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  setup_wifi();
  timeClient.begin();
  timeClient.setTimeOffset(19800);
  client.setServer(mqtt_server, 1883);
  doc["Panel-ID"] = "Panel-001";
  doc["Station-ID"] = "ST-23";
  client.setCallback(callback);
}




void getElectricalPrameters()
{
  voltage  = VMF_Factor * pzem3.voltage();
  voltage=  voltage - VSF_Factor;
   if(!isnan(voltage)){
       Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
   } else {
       Serial.println("Error reading voltage");
   }

  current = pzem3.current();
   if(!isnan(current)){
       Serial.print("Current: "); Serial.print(current); Serial.println("A");
   } else {
       Serial.println("Error reading current");
   }

   power = WMF_Factor * pzem3.power();
   power = power + VSF_Factor;
   
   if(!isnan(power)){
       Serial.print("Power: "); Serial.print(power); Serial.println("W");
   } else {
       Serial.println("Error reading power");
   }
  
  }

void getTemperaturePrameters()
{
  
  
  
  
  
  
  
  }






void loop() {

  if (!client.connected()) 
  {
    reconnect();
  }
  
  client.loop();

  while (!timeClient.update()) 
  {
    timeClient.forceUpdate();
  }

getElectricalPrameters();


  
 

   Serial.println();

  // Variables to save date and time
  doc["Time"] = getTime();
  doc["Operation-ON-Cycle"] = 0;
  doc["Voltage"] = voltage;
  doc["Current"] = current;
  doc["Power"] = power;
  doc["Temperature"] = 34.24;
  doc["Cycle-of-Thermostat"] = 0;
  doc["Test-Status"] = 0;
  doc["Test-Fault"] = 0;
  doc["Temp-Alert"] = 0;


  Serial.println(formattedDate);
  size_t n = serializeJson(doc, buffer);
  client.publish("philips/sensor", buffer, n);

  delay(1000);
}

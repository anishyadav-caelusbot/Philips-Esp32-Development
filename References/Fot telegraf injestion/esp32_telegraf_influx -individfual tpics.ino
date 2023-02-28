/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <WiFi.h>
#include <PubSubClient.h>
//#include <Wire.h>
//#include <Adafruit_BME280.h>
//#include <Adafruit_Sensor.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "WORKSTATION";
const char* password = "Superman_27";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.1.19";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int value = 0;

//uncomment the following lines if you're using SPI
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/
//
//Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
float temperature = 0;
float humidity = 0;

// LED Pin
const int ledPin = 4;

void setup() {
  Serial.begin(115200);
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  //status = bme.begin();  
//  if (!bme.begin(0x76)) {
//    Serial.println("Could not find a valid BME280 sensor, check wiring!");
//    while (1);
//  }
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
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
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "philips/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
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

  // Get the current object temperatur of the sensor
//  float objt = 1243;
//double objt2 = 1243;
//double objt3 = 457;
//double objt4 = 234.67;
//double objt5 = 1.34;

//String message = String("weather,location=us temperature="+String(objt)+",humidity="+String(objt2)+",power="+String(objt3)+",voltage="+String(objt4)+",current="+String(objt5));
 
  // Create the message that will be send using mqtt
// String message = String("weather,location=us temperature="+String(objt)+",humidity="+String(objt2));
//  message.toCharArray(msg, message.length());
//  Serial.println(msg);
//
//  // Send the message on the sensors topic
//  client.publish("philips/Panel1", msg);



//  char msg1[50];
//  String message1 = String("Panel-001,station=001 Voltage="+String(objt4)+",Current="+String(objt3));
//  message1.toCharArray(msg1, message1.length());
//  Serial.println(msg1);
//  client.publish("philips/Panel-001/Station001", msg1);
//
//  char msg2[50];
//  String message2 = String("Panel-001,station=001 temperature="+String(objt)+",count="+String(objt2));
//  message2.toCharArray(msg2, message2.length());
//  Serial.println(msg2);
//  client.publish("philips/Panel-001/Station001", msg2);
//
//  char msg3[50];
//  String message3 = String("Panel-001,station=001 Power="+String(objt)+",fault="+String(objt2));
//  message3.toCharArray(msg3, message3.length());
//  Serial.println(msg3);
//  client.publish("philips/Panel-001/Station001", msg3);


float ivoltage= 240.5;
float icurrent =5.2;
float ipower =2005.4;
int s7_testStatus= 1;
int s7_faultStatus= 2;
int TestON = 1;
int count_value = 3;

//  // Convert the value to a char array
//  char voltString[8];
//  dtostrf(ivoltage, 1, 2, voltString);
//  Serial.print("voltage: ");
//  Serial.println(voltString);
//  client.publish("philips/Panel-001/Station001/voltage", voltString);
//
//
//
//  // Convert the value to a char array
//  char currString[8];
//  dtostrf(icurrent, 1, 2, currString);
//  Serial.print("Current: ");
//  Serial.println(currString);
//  client.publish("philips/Panel-001/Station001/current", currString);
//
//
//
//  // Convert the value to a char array
//  char poweString[8];
//  dtostrf(ipower, 1, 2, poweString);
//      Serial.print("Power: ");
//      Serial.println(poweString);
//  client.publish("philips/Panel-001/Station001/power", poweString);
//
////
////  // Convert the value to a char array
//  char testStatusString[8];
//  dtostrf(s7_testStatus, 1, 2, testStatusString);
//  Serial.print("Teststatus: ");
//  Serial.println(testStatusString);
//  client.publish("philips/Panel-001/Station001/teststatus", testStatusString);
////
//
//
//
//
//  // Convert the value to a char array
//  char TestFaultString[8];
//  dtostrf(s7_faultStatus, 1, 2, TestFaultString);
//  Serial.print("TestFault: ");
//  Serial.println(TestFaultString);
//  client.publish("philips/Panel-001/Station001/fault", TestFaultString);
//
//  // Convert the value to a char array
//  char TestONString[8];
//  dtostrf(TestON, 1, 2, TestONString);
//  Serial.print("TestON: ");
//  Serial.println(TestONString);
//  client.publish("philips/Panel-001/Station001/testOn", TestONString);
//
// // Convert the value to a char array
//  char count_valueString[8];
//  dtostrf(count_value, 1, 2, count_valueString);
//  Serial.print("count_value: ");
//  Serial.println(count_value);
//  client.publish("philips/Panel-001/Station001/count", count_valueString);

 // Convert the value to a char array
  char voltString[8];
  dtostrf(ivoltage, 1, 2, voltString);
  Serial.print("voltage: ");
  Serial.println(voltString);
  client.publish("/sensors/CLE/v1/device5/temp", voltString);



  // Convert the value to a char array
  char currString[8];
  dtostrf(icurrent, 1, 2, currString);
  Serial.print("Current: ");
  Serial.println(currString);
  client.publish("/sensors/CLE/v1/device5/rpm", currString);



  // Convert the value to a char array
  char poweString[8];
  dtostrf(ipower, 1, 2, poweString);
      Serial.print("Power: ");
      Serial.println(poweString);
  client.publish("/sensors/CLE/v1/device5/ph", poweString);

//
//  // Convert the value to a char array
  char testStatusString[8];
  dtostrf(s7_testStatus, 1, 2, testStatusString);
  Serial.print("Teststatus: ");
  Serial.println(testStatusString);
  client.publish("/sensors/CLE/v1/device5/spin", testStatusString);
//




//  // Convert the value to a char array
//  char TestFaultString[8];
//  dtostrf(s7_faultStatus, 1, 2, TestFaultString);
//  Serial.print("TestFault: ");
//  Serial.println(TestFaultString);
//  client.publish("philips/Panel-001/Station001/fault", TestFaultString);
//
//  // Convert the value to a char array
//  char TestONString[8];
//  dtostrf(TestON, 1, 2, TestONString);
//  Serial.print("TestON: ");
//  Serial.println(TestONString);
//  client.publish("philips/Panel-001/Station001/testOn", TestONString);
//
// // Convert the value to a char array
//  char count_valueString[8];
//  dtostrf(count_value, 1, 2, count_valueString);
//  Serial.print("count_value: ");
//  Serial.println(count_value);
//  client.publish("philips/Panel-001/Station001/count", count_valueString);


  

  delay(1000); 
}

//RoadRoller,field=temperature value=32.2
//RoadRoller,field=Speed value=20
//RoadRoller,field=Vibration value=1.0752

//
//#include <ArduinoJson.h>
//
//void setup() {
//
//  Serial.begin(9600);
//  while (!Serial) continue;
//
//  StaticJsonDocument<200> doc;
//  doc["sensor"] = "lab";
//  doc["time"] = 1351824120;
//  doc["power"] = 489.58;
//  doc["voltage"] = 256.2;
//  doc["current"] = 2.41;
//  doc["temperature"] = 1351824120;
//
//
//
//
//  serializeJson(doc, Serial);
//  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
//
//  Serial.println();
//
//
//  serializeJsonPretty(doc, Serial);
// 
//}
//
//void loop () {
//  }

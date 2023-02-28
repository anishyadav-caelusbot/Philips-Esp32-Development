 
#include <PZEM004Tv30.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"

#define I2C_ADDRESS (0x66)

Adafruit_MCP9600 mcp;
//#include <PZEM004Tv30.h>

/* Use software serial for the PZEM
 * Pin 5 Rx (Connects to the Tx pin on the PZEM)
 * Pin 4 Tx (Connects to the Rx pin on the PZEM)
*/
PZEM004Tv30 pzem(&Serial2,27,14,0xE4);


//multiply
float VMF_Factor = 2.0;
float WMF_Factor = 2.0;
//substract
float VSF_Factor = 0.20;
float WSF_Factor = 0.30;


#define INTERVAL_MESSAGE1 1000
unsigned long time_1 = 0;


#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"


#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"

#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>




 
// WiFi AP SSID
#define WIFI_SSID "WORKSTATION"
// WiFi password
#define WIFI_PASSWORD "Superman_27"
//InfluxDB v2 server url, e.g. https://eu-central-1-1.aws.cloud2.influxdata.com (Use: InfluxDB UI -> Load Data -> Client Libraries)
////#define INFLUXDB_URL "http://192.168.1.19:8086"
//// InfluxDB v2 server or cloud API token (Use: InfluxDB UI -> Data -> API Tokens -> Generate API Token)
////#define INFLUXDB_TOKEN "8971090763"
//#define INFLUXDB_TOKEN "yGzrJhXaxxLX63yhjvLG-usoU8cpb4ZzFXFkWfyC1rqm53wvF5p2OjygGacbdhC1km4B_FEFjI4VYmbWB-S8Pg=="

// InfluxDB v2 organization id (Use: InfluxDB UI -> User -> About -> Common Ids )
//#define INFLUXDB_ORG "starwatt"
// InfluxDB v2 bucket name (Use: InfluxDB UI ->  Data -> Buckets)
//#define INFLUXDB_BUCKET "Philips"


  #define INFLUXDB_URL "https://europe-west1-1.gcp.cloud2.influxdata.com"
  #define INFLUXDB_TOKEN "19mghuraK5ug91XxAt-99xUfYk0dE5DrM-1mzpb-hL9SNG0SHJthF5me4uv8x9At1Tbk1TkfW0tqxltXrtElEQ=="
  #define INFLUXDB_ORG "667a0f15f6035107"
  #define INFLUXDB_BUCKET "Philips"

// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
// Examples:
//  Pacific Time: "PST8PDT"
//  Eastern: "EST5EDT"
//  Japanesse: "JST-9"
//  Central Europe: "CET-1CEST,M3.5.0,M10.5.0/3"
#define TZ_INFO "Asia/Kolkata"

// InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
// InfluxDB client instance without preconfigured InfluxCloud certificate for insecure connection
//InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

// Data point
Point sensorReadings("measurement");


float voltage ;
float current;
float power ;
float energy ;
float frequency ;
float pf ;
float temp;


void setup()
{
 Serial.begin(115200);

  // Connect WiFi
  Serial.println("Connecting to WiFi");

  WiFi.mode(WIFI_STA);

  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  if (! mcp.begin(I2C_ADDRESS)) {
        Serial.println("Sensor not found. Check wiring!");
        while (1);
    }
  
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
  mcp.setThermocoupleType(MCP9600_TYPE_K);
  mcp.setFilterCoefficient(3);
  mcp.setAlertTemperature(1, 30);
  mcp.configureAlert(1, true, true);  // alert 1 enabled, rising temp
  mcp.enable(true);
 
  // Add tags
  sensorReadings.addTag("Device", "Station-001");
  sensorReadings.addTag("Location", "Baddi");
  sensorReadings.addTag("Sensor", "Panel-001");

  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

}

void loop()
{
    Serial.print("Hot Junction: "); Serial.println(mcp.readThermocouple());
  Serial.print("Cold Junction: "); Serial.println(mcp.readAmbient());
  Serial.print("ADC: "); Serial.print(mcp.readADC() * 2); Serial.println(" uV");
  Serial.print("Custom Address:");
  Serial.println(pzem.readAddress(), HEX);
  // Read the data from the sensor
//  voltage = pzem.voltage();
 voltage  = VMF_Factor * pzem.voltage();

  voltage=  voltage - VSF_Factor;
  current = pzem.current();
     power = WMF_Factor * pzem.power();

   power = power + VSF_Factor;
//  power = pzem.power();
  energy = pzem.energy();
  frequency = pzem.frequency();
  pf = pzem.pf();
  temp = mcp.readThermocouple();

    Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
    Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
    Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
    Serial.print("Energy: ");       Serial.print(energy, 3);     Serial.println("kWh");
    Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
    Serial.print("PF: ");           Serial.println(pf);
    Serial.print("Temperature: ");           Serial.println(mcp.readThermocouple());



  // Check if the data is valid
  if (isnan(voltage)) {
    Serial.println("Error reading voltage");
    voltage = 0;
  } else if (isnan(current)) {
    Serial.println("Error reading current");
    current = 0;
  } else if (isnan(power)) {
    Serial.println("Error reading power");
    power = 0;
  } else if (isnan(energy)) {
    Serial.println("Error reading energy");
    energy = 0;

  } else if (isnan(frequency)) {
    Serial.println("Error reading frequency");
    frequency = 0;
  } else if (isnan(pf)) {
    Serial.println("Error reading power factor");
    pf = 0;

  } 
  
  else   
  {

    // Print the values to the Serial console
    Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
    Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
    Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
    Serial.print("Energy: ");       Serial.print(energy, 3);     Serial.println("kWh");
    Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
    Serial.print("PF: ");           Serial.println(pf);

  }
  

  if (millis() > time_1 + INTERVAL_MESSAGE1) 
  {
    time_1 = millis();
    sendData();
  }
//delay(500);
Serial.println("");
Serial.println("");

}



void sendData()
{
  // Add readings as fields to point

  sensorReadings.addField("Voltage", voltage);
  sensorReadings.addField("Current", current);
  sensorReadings.addField("Power", power);
  sensorReadings.addField("Temperature", temp);
  
//  sensorReadings.addField("Energy", energy);
//  sensorReadings.addField("Frequency", frequency);
//  sensorReadings.addField("PowerFactor", pf);

  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(client.pointToLineProtocol(sensorReadings));

  // Write point into buffer
  client.writePoint(sensorReadings);

  // Clear fields for next usage. Tags remain the same.
  sensorReadings.clearFields();

  // If no Wifi signal, try to reconnect it
  if (wifiMulti.run() != WL_CONNECTED) 
  {
    Serial.println("WiFi Connection Lost");
  }

  // Wait 10s
  Serial.println("Wait 10s");
}

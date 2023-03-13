#include <PZEM004Tv30.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"
PZEM004Tv30 pzem2(&Serial2, 27, 14, 0xE3);

//MCP9600
#define I2C_ADDRESS (0x66)
Adafruit_MCP9600 mcp;



// Test On and Test Reset button with counters
const int TestStartButtom = 2;
const int ResetCountersAndTestButon = 4;


// input Sensor Pin
const int inputSensorPin = 15;
// the number of the LED pin


// Button's Default State
int TestStartButtomState = 0;         // variable for reading the pushbutTestResult status
int ResetCountersAndTestButonState = 0;


// Test's Default State xhnage as
int TestResult = 0;


// Input's Default State xhnage as
int inputVoltageBit = 0;         // variable for reading the pushbutTestResult status
int inputSensor = 0;



//Variable for data
float voltage;
float current;
float power;
float temperature;





// variables will change:
int s7_prestate = 0;



// Variables for Fault  Detection
int s7_FaultPreviousState = LOW;                     // previousstate of the switch
unsigned long s7_FaultConfirmDuration = 420000;      // Time we wait before we see the press as a long press
unsigned long s7_FaultConfirmDurationMillis;       // Time in ms when we the button was pressed
bool s7_FaultConfirmState = false;                // True if it is a long press
const int s7_FaultStateChangeInterval = 50;      // Time between two readings of the button state
unsigned long s7_previousFaultStateMillis;      // Timestamp of the latest reading
unsigned long s7_FaultDuration;                // Time the button is pressed in ms
unsigned long s7_FaultcurrentMillis;          // Variabele to store the number of milleseconds since the Arduino has started
int   s7_faultStatus ;





// Test Status Buttons


int TestStatus()
{
  TestStartButtomState = digitalRead(TestStartButtom);
  ResetCountersAndTestButonState = digitalRead(ResetCountersAndTestButon);
  if (TestStartButtomState == HIGH) {
    TestResult = 1;
  }
  if (ResetCountersAndTestButonState == HIGH) {
    TestResult = 0;
  }
  //Serial.println(TestResult);
  return TestResult;
}



// Test Status Buttons

int inputStatus()
{
  inputVoltageBit = digitalRead(inputSensorPin);

  if (inputVoltageBit == HIGH) {
    inputSensor = 1;
  }
  else {
    inputSensor = 0;
  }
  //Serial.println(TestResult);
  return inputSensor;
}








//=================Setup_functions====================

void setup_Thermocouple(){
  
   Serial.println("MCP9600 HW test");

    /* Initialise the driver with I2C_ADDRESS and the default I2C bus. */
    if (! mcp.begin(I2C_ADDRESS)) {
        Serial.println("Sensor not found. Check wiring!");
        while (1);
    }
  Serial.println("Found MCP9600!");

  mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
  Serial.print("ADC resolution set to ");
  switch (mcp.getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:   Serial.print("18"); break;
    case MCP9600_ADCRESOLUTION_16:   Serial.print("16"); break;
    case MCP9600_ADCRESOLUTION_14:   Serial.print("14"); break;
    case MCP9600_ADCRESOLUTION_12:   Serial.print("12"); break;
  }
  Serial.println(" bits");

  mcp.setThermocoupleType(MCP9600_TYPE_K);
  Serial.print("Thermocouple type set to ");
  switch (mcp.getThermocoupleType()) {
    case MCP9600_TYPE_K:  Serial.print("K"); break;
    case MCP9600_TYPE_J:  Serial.print("J"); break;
    case MCP9600_TYPE_T:  Serial.print("T"); break;
    case MCP9600_TYPE_N:  Serial.print("N"); break;
    case MCP9600_TYPE_S:  Serial.print("S"); break;
    case MCP9600_TYPE_E:  Serial.print("E"); break;
    case MCP9600_TYPE_B:  Serial.print("B"); break;
    case MCP9600_TYPE_R:  Serial.print("R"); break;
  }
  Serial.println(" type");
  mcp.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp.getFilterCoefficient());
  mcp.setAlertTemperature(1, 30);
  Serial.print("Alert #1 temperature set to ");
  Serial.println(mcp.getAlertTemperature(1));
  mcp.configureAlert(1, true, true);  // alert 1 enabled, rising temp
  mcp.enable(true);
  Serial.println(F("------------------------------"));
  }


void setup_WiFI_MQTT() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_broker, 1883);
  client.setCallback(callback);
   
  }


void setup_Time()
{
  timeClient.begin();
  timeClient.setTimeOffset(19800);
  
 }


void setup_PanelButtons(){
   pinMode(TestStartButtom, INPUT);
  pinMode(ResetCountersAndTestButon, INPUT);
  
  }

void setup_inputSensorRelay(){
    pinMode(inputSensorPin, INPUT);
  }





  
  

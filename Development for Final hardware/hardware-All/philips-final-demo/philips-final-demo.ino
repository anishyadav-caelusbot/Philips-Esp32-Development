// Esp32 working example https://github.com/mandulaj/PZEM-004T-v30/issues/64

#include "sensor.h"
  //unsigned long FaultcurrentMillis;          // Variabele to store the number of milleseconds since the Arduino has started


void setup()
{
// remove wehn configured
  //pinMode(buttonPin, INPUT); 
  
  // Initialize Serial Monitor
  Serial.begin(115200);
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

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(19800);
}

void loop()
{
  while (!timeClient.update())
  {
    timeClient.forceUpdate();
  }


  if (!client.connected())
  {
    reconnect();
  }

s7_FaultcurrentMillis = millis();    // store the current time



  client.loop();

  long now = millis();
  
  if (now - lastMsg > 1000)
  {
    lastMsg = now;
    //readButtonState();
    
    S7outputPrams();

  }
}

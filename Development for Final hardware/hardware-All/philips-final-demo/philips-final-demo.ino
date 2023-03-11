#include "sensor.h"

void setup()
{
  // Initialize Serial Monitor
  Serial.begin(115200);
  setup_WiFI_MQTT(); 
  setup_Thermocouple();
  setup_Time();
  setup_PanelButtons();
  setup_inputSensorRelay();
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

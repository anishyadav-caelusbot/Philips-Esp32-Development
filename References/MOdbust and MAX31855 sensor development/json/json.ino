
#include <ArduinoJson.h>

void setup() {

  Serial.begin(9600);
  while (!Serial) continue;

  StaticJsonDocument<200> doc;
  doc["sensor"] = "lab";
  doc["time"] = 1351824120;
  doc["power"] = 489.58;
  doc["voltage"] = 256.2;
  doc["current"] = 2.41;
  doc["temperature"] = 1351824120;




  serializeJson(doc, Serial);
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  Serial.println();


  serializeJsonPretty(doc, Serial);
 
}

void loop() 
{

}

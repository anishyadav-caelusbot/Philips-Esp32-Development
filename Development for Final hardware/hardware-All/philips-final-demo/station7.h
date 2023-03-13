#include "stationfunctions.h"









void S7outputPrams()
{
  //========Calibration factors for PZEM==========
  float testCurrent = 0.20;
  float PTRatioMFv = 1.93;
  float PTRatioMFa = 2.0;
  float PTRatioMFw = 233.0;
  float overcurrent = 6.0;
  float realCurrent = pzem2.current();

  // Test status Variables
  int TestON ;
  int TestFault;
  int buttonState;
  int s7_testStatus;
  // variable for counter/Cycles
  unsigned int cycles_value;








  //====================Parameters=========================================
  
  Serial.println();
  Serial.println("====================Output Socket : Station 7=====================");

  //-------Time-----------------------------------------------------------------------
  
  Serial.print("Time: ");  Serial.println(timeClient.getFormattedDate());

  //-------voltage-----------------------------------------------------------------------
  
  voltage = PTRatioMFv * pzem2.voltage();
  if (!isnan(voltage))
  {
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
  }
  else
  {
    Serial.println("Error reading voltage");
  }
  
  //-------current-----------------------------------------------------------------------
  
  current = pzem2.current();
  if (!isnan(current))
  {
    Serial.print("Current: "); Serial.print(current); Serial.println("A");
  }
  else
  {
    Serial.println("Error reading current");
  }
  
  //-------power-----------------------------------------------------------------------
  
  power = voltage * current;

  if (!isnan(power))
  {
    Serial.print("Power: "); Serial.print(power); Serial.println("W");
  }
  else
  {
    Serial.println("Error reading power");
  }

  //-------Temperature-----------------------------------------------------------------------
  
  Serial.print("Thermocouple: "); Serial.println(mcp.readThermocouple());
  temperature = mcp.readThermocouple();

  //-------Panel-Buttons----------------------------------------------------------------------- Optimise it later
  
  int x = TestStatus();
  Serial.println(x);

  //------input-sensor-relay-----------------------------------------------------------------------

  if (inputStatus() ==1)
{
//  int x = inputStatus();
  Serial.println("input high");}

  else{
    Serial.println("input Low");
    }


//====================END==of==Parameters=========================

  preferences.begin("station", false);
  cycles_value = preferences.getUInt("cycles", 0);



  // TestStatus  ==> ON = 1, OFF = 0
  // Start button pressed test status remains high till our reset button is not pressed. Our reset button also reset counters.






  // FAULT  => Type =


//  if (!isnan(pzem1.voltage()) && !isnan(pzem2.voltage()))

if (!TestStatus() ==0){

  
  if ((inputStatus() ==1) && !isnan(pzem2.voltage()))

  //if (!isnan(pzem2.voltage()))
  {
    Serial.print("Test Status: "); Serial.println("ON");
    TestON = HIGH;

    //if (realCurrent > testCurrent )
    if (realCurrent > testCurrent && realCurrent < overcurrent)

    {
      Serial.print("Fault: "); Serial.println("No");
      TestFault = LOW;
    }

    else
    {
      Serial.print("Fault: "); Serial.println("Yes");
      TestFault = HIGH;
    }

    Serial.print("Cycles: "); Serial.println(cycles_value);
  }

  else
  {
    Serial.print("Test Status: "); Serial.println("OFF");
    Serial.print("Fault: "); Serial.println("No");
    TestON = LOW;
    TestFault = LOW;
    Serial.print("Cycles: "); Serial.println(cycles_value);
  }
}








  // Station Fault Detection
  if (s7_FaultcurrentMillis - s7_previousFaultStateMillis > s7_FaultStateChangeInterval) {
    int buttonState =  TestFault;
    if (buttonState == HIGH && s7_FaultPreviousState == LOW && !s7_FaultConfirmState) {
      s7_FaultConfirmDurationMillis = s7_FaultcurrentMillis;
      s7_FaultPreviousState = HIGH;
      Serial.println("UnConfirmed Fault Detected");
      s7_faultStatus = LOW;
    }
    s7_FaultDuration = s7_FaultcurrentMillis - s7_FaultConfirmDurationMillis;

    // If the button is pressed AND
    // If there is no measurement running to determine how long the button is pressed AND
    // If the time the button has been pressed is larger or equal to the time needed for a long press
    if (buttonState == HIGH && !s7_FaultConfirmState && s7_FaultDuration >= s7_FaultConfirmDuration) {
      s7_FaultConfirmState = true;
      Serial.println("Fault Confirmed");
      s7_faultStatus = HIGH;
    }

    // If the button is released AND
    // If the button was pressed before
    if (buttonState == LOW && s7_FaultPreviousState == HIGH) {
      s7_FaultPreviousState = LOW;
      s7_FaultConfirmState = false;
      Serial.println("Fault Cleared");
      s7_faultStatus = LOW;

      // If there is no measurement running to determine how long the button was pressed AND
      // If the time the button has been pressed is smaller than the minimal time needed for a long press
      // Note: The video shows:
      //       if (!s7_FaultConfirmState && s7_FaultDuration < s7_FaultConfirmDuration) {
      //       since s7_FaultConfirmState is set to FALSE on line 75, !s7_FaultConfirmState is always TRUE
      //       and can be removed.
      if (s7_FaultDuration < s7_FaultConfirmDuration) {
        Serial.println("Thermostat Trip Detected ");
        //        s7_faultStatus =4;
      }
    }

    // store the current timestamp in s7_previousFaultStateMillis
    s7_previousFaultStateMillis = s7_FaultcurrentMillis;
  }


  if (TestON == HIGH & s7_faultStatus == LOW) {

    s7_testStatus = 1;
  }

  else if (TestON == HIGH & s7_faultStatus == HIGH)
  {
    s7_testStatus = 2;

  }

  else {
    s7_testStatus = 0;

  }

  //
  //s7_faultStatus
  //
  //TestON
  //
  //














  //  Counter
  if (TestON == HIGH && s7_prestate == 0)
  {
    cycles_value++;
    Serial.printf("Current counter value: %u\n", cycles_value);
    preferences.putUInt("cycles", cycles_value);
    preferences.end();
    s7_prestate = 1;
  }

  else if (TestON == LOW)
  {
    s7_prestate = 0;
    Serial.printf("Current counter value: %u\n", cycles_value);
  }

  else
  {
    preferences.putUInt("cycles", cycles_value);
    preferences.end();
  }















  // Voltage into array
  float ivoltage = voltage;
  float icurrent = current;
  float ipower = power;



  // Convert the value to a char array
  char voltString[8];
  dtostrf(ivoltage, 1, 2, voltString);
  //  Serial.print("voltage: ");
  //  Serial.println(voltString);
  client.publish("esp32/s7volt", voltString);



  // Convert the value to a char array
  char currString[8];
  dtostrf(icurrent, 1, 2, currString);
  //  Serial.print("Current: ");
  //  Serial.println(currString);
  client.publish("esp32/s7curr", currString);



  // Convert the value to a char array
  char poweString[8];
  dtostrf(ipower, 1, 2, poweString);
  //    Serial.print("Humidity: ");
  //    Serial.println(currString);
  client.publish("esp32/s7power", poweString);

  //
  //  // Convert the value to a char array
  char testStatusString[8];
  dtostrf(s7_testStatus, 1, 2, testStatusString);
  //  Serial.print("TestFault: ");
  //  Serial.println(TestFaultString);
  client.publish("esp32/s7_testStatus", testStatusString);
  //




  // Convert the value to a char array
  char TestFaultString[8];
  dtostrf(s7_faultStatus, 1, 2, TestFaultString);
  //  Serial.print("TestFault: ");
  //  Serial.println(TestFaultString);
  client.publish("esp32/testfault", TestFaultString);

  // Convert the value to a char array
  char TestONString[8];
  dtostrf(TestON, 1, 2, TestONString);
  //  Serial.print("TestON: ");
  //  Serial.println(TestONString);
  client.publish("esp32/teston", TestONString);

  // Convert the value to a char array
  char cycles_valueString[8];
  dtostrf(cycles_value, 1, 2, cycles_valueString);
  //  Serial.print("cycles_value: ");
  //  Serial.println(cycles_value);
  client.publish("esp32/cycles_value", cycles_valueString);



}

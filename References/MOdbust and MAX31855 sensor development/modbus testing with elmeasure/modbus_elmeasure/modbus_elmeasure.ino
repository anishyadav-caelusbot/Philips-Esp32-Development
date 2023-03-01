#include <ModbusMaster.h>


#define MAX485_DE      3
#define MAX485_RE_NEG  2

// instantiate ModbusMaster object
ModbusMaster node;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode

  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  // Modbus communication runs at 9600 baud
  Serial1.begin(9600);
Serial.begin(9600,SERIAL_8N1);
  // Modbus slave ID 1
  node.begin(1, Serial1);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop()
{
  float result;

  result = node.readInputRegisters(151, 2);
  if (result == node.ku8MBSuccess)
  {

    
    Serial.println(node.getResponseBuffer(0x01));

   
    Serial.println(node.getResponseBuffer(0x02));
    
    
  }
  delay(1000);
}

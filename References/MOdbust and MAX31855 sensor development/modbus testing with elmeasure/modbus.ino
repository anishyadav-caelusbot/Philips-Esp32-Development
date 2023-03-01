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
  // Modbus communication runs at 115200 baud
    Serial.begin(9600);
  Serial2.begin(9600);
  // Modbus slave ID 1
  node.begin(2, Serial2);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}
bool state = true;
void loop()
{
  uint8_t result;
  uint16_t data[6];
  

  // Read 16 registers starting at 0x3100)
  result = node.readInputRegisters(0151, 10);
  if (result == node.ku8MBSuccess)
  {
    Serial.print("Vbatt: ");
    Serial.println(node.getResponseBuffer(0x04)/100.0f);
    Serial.print("Vload: ");
    Serial.println(node.getResponseBuffer(0xC0)/100.0f);
    Serial.print("Pload: ");
    Serial.println((node.getResponseBuffer(0x0D) +
                    node.getResponseBuffer(0x0E) << 16)/100.0f);
  }
  delay(1000);
  Serial.println(getResultMsg(result));
}


bool getResultMsg(uint8_t result)
{
  String tmpstr2;

  switch (result) {
    case node.ku8MBSuccess:
      return true;
      break;
    case node.ku8MBIllegalFunction:
      tmpstr2 = "Illegal Function";
      break;
    case node.ku8MBIllegalDataAddress:
      tmpstr2 = "Illegal Data Address";
      break;
    case node.ku8MBIllegalDataValue:
      tmpstr2 = "Illegal Data Value";
      break;
    case node.ku8MBSlaveDeviceFailure:
      tmpstr2 = "Slave Device Failure";
      break;
    case node.ku8MBInvalidSlaveID:
      tmpstr2 = "Invalid Slave ID";
      break;
    case node.ku8MBInvalidFunction:
      tmpstr2 = "Invalid Function";
      break;
    case node.ku8MBResponseTimedOut:
      tmpstr2 = "Response Timed Out";
      break;
    case node.ku8MBInvalidCRC:
      tmpstr2 = "Invalid CRC";
      break;
    default:
      tmpstr2 = "Unknown error: " + String(result);
      break;
  }
  Serial.println(tmpstr2);
  return false;
  
}

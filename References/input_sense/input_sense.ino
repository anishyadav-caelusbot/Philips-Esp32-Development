
// constants won't change. They're used here to set pin numbers:
const int inputSensorPin = 15;
    // the number of the LED pin



// variables will change:
int inputVoltageBit = 0;         // variable for reading the pushbutTestResult status


int inputSensor = 0;


void setup() 
{
  Serial.begin(115200);
  pinMode(inputSensorPin, INPUT);

}



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



void loop() {

if (inputStatus() ==1)
{
//  int x = inputStatus();
  Serial.println("input high");}

  else{
    Serial.println("input Low");
    }
}


// constants won't change. They're used here to set pin numbers:
const int TestStartButtom = 2;
const int ResetCountersButTestResult = 4;
const int ledPin =  32;      // the number of the LED pin



// variables will change:
int TestStartButtomState = 0;         // variable for reading the pushbutTestResult status
int ResetCountersButTestResultState = 0;

int TestResult = 0;


void setup() 
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(TestStartButtom, INPUT);
  pinMode(ResetCountersButTestResult, INPUT);
}



int TestStatus() 
{
  TestStartButtomState = digitalRead(TestStartButtom);
  ResetCountersButTestResultState = digitalRead(ResetCountersButTestResult);
  if (TestStartButtomState == HIGH) {
    TestResult = 1;
  }
  if (ResetCountersButTestResultState == HIGH) {
    TestResult = 0;
  }
  //Serial.println(TestResult);
  return TestResult;
}



void loop() {

  int x = TestStatus();
  Serial.println(x);
}

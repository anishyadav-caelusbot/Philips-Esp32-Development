
// Test On and Test Reset button with counters
const int TestStartButtom = 2;
const int ResetCountersButTestResult = 4;



// Button's Default State
int TestStartButtomState = 0;         // variable for reading the pushbutTestResult status
int ResetCountersButTestResultState = 0;


// Test's Default State xhnage as 
int TestResult = 0;


void setup() 
{
  Serial.begin(9600);
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

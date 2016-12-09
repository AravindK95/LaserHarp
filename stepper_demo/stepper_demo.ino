#define EN   A0
#define MS1  A1
#define MS2  A2
#define MS3  A3
#define STEP A4
#define DIR  A5

char user_input;
int x;
int step_dur = 650;
int step_num = 34;

void setup() {
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  resetBEDPins(); //Set step, direction, microstep and enable pins to default states
  Serial.begin(9600); //Open Serial connection for debugging
  Serial.println("Begin motor control");
  Serial.println();
  //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. Turn at default microstep mode.");
  Serial.println("2. Reverse direction at default microstep mode.");
  Serial.println("3. Continuous back and forth at default mode.");
  // Serial.println("4. Step forward and reverse directions.");
  Serial.println();
}

void loop() {
  while(Serial.available()){
    user_input = Serial.read(); //Read user input and trigger appropriate function
    digitalWrite(EN, LOW); //Pull enable pin low to set FETs active and allow motor control
    if (user_input =='1')
    {
       StepForwardDefault();
    }
    else if(user_input =='2')
    {
      ReverseStepDefault();
    }
    else if(user_input =='3')
    {
      ContStepMode();
    }
    // else if(user_input =='4')
    // {
    //   ForwardBackwardStep();
    // }
    else
    {
      Serial.println("Invalid option entered.");
    }
    resetBEDPins();
    Serial.println("Enter new option");
    Serial.println();
  }
}

void resetBEDPins()
{
  digitalWrite(STEP, LOW);
  digitalWrite(DIR, LOW);
  digitalWrite(EN, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
}

//Default microstep mode function
void StepForwardDefault()
{
  //  Serial.println("Moving forward at default step mode.");
  digitalWrite(DIR, LOW); //Pull direction pin low to move "forward"
  for(x= 1; x<step_num; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(STEP, HIGH); //Trigger one step forward
    delayMicroseconds(step_dur);
    digitalWrite(STEP, LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(step_dur);
  }
}

//Reverse default microstep mode function
void ReverseStepDefault()
{
  //  Serial.println("Moving in reverse at default step mode.");
  digitalWrite(DIR, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 1; x<step_num; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(STEP,HIGH); //Trigger one step
    delayMicroseconds(step_dur);
    digitalWrite(STEP,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(step_dur);
  }
}

void ContStepMode()
{
  Serial.println("Stepping continuously...");
  while(1) {
    StepForwardDefault();
    ReverseStepDefault();
  }
}

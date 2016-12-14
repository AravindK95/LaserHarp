//#define EN   A0
//#define MS1  A1
//#define MS2  A2
//#define MS3  A3
//#define STEP A4
//#define DIR  A5

#define EN   13
#define MS1  12
#define MS2  11
#define MS3  10
#define STEP 9
#define DIR  8

#include "TimerOne.h"

char user_input;
int x;
int step_dur = 850;
int step_num = 70;

//int stepper_state = LOW;
int dir_state = LOW;
int step_count = 0;

#define WAIT_TICKS 10

#define MOVE_TICKS 3

#define POS_MIN 0

#define POS_MAX 7

#define LASER 5

#define magic 0

int pos = 3;

int ticks = 1;

enum direction_t {LEFT=0, RIGHT=1};

direction_t direction = LEFT;

enum stepper_state_t {MOVING=0, WAITING=1};

stepper_state_t stepper_state = MOVING;

uint8_t data = 0xFF;

uint8_t readings[] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(LASER, OUTPUT);
  pinMode(6, INPUT_PULLUP);
  analogReference(EXTERNAL);
  resetBEDPins(); //Set step, direction, microstep and enable pins to default states

  //Timer1.initialize(step_dur);
  //Timer1.attachInterrupt(stepMotor); // blinkLED to run every 0.15 seconds
  Serial.begin(115200); //Open Serial connection for debugging
  // Serial.println("Begin motor control");
  digitalWrite(LASER, HIGH);
  
  
  while(digitalRead(6)!=LOW){delay(1);}
  
  
  
  // Serial.println();
  // //Print function list for user selection
  // Serial.println("Enter number for control option:");
  // Serial.println("1. Turn at default microstep mode.");
  // Serial.println("2. Reverse direction at default microstep mode.");
  // Serial.println("3. Continuous back and forth at default mode.");
  // // Serial.println("4. Step forward and reverse directions.");
  // Serial.println();
}

void stepMotor(void) {

  switch (stepper_state) {
    case MOVING:
      if (ticks == MOVE_TICKS) {
        if (direction == LEFT) {
          pos--;
        } else {
          pos++;
        }
        digitalWrite(STEP, 0);
        
        stepper_state = WAITING;
        
        ticks = 0;
      } else {
        digitalWrite(STEP, 1-(ticks%2));
        ticks++;
      }
      break;


    case WAITING:
      if (ticks == WAIT_TICKS) {
        if (pos == POS_MAX || pos == POS_MIN) {
          if (direction == RIGHT) {
            direction=LEFT;
          } else {
            direction=RIGHT;  
          }
          //direction = RIGHT - direction;
          digitalWrite(DIR, direction);
        }
        stepper_state = MOVING;

        //Serial.print(pos);
        //Serial.print(" ");
        
        int x = 0;
        for (int i = 0; i < 7; i++) {
          //while(stepper_state==MOVING){}
          int temp = analogRead(A0);
          //Serial.print(temp);
          //Serial.print(" ");
          if (temp > x) {
            x = temp;
          }
        }
        digitalWrite(LASER, LOW);
        //Serial.println();
//       uint8_t new_data = data;
//       if (x > 300) {
//           new_data = data | (1 << pos);
//       } else {
//           new_data = data & ~(1 << pos);  
//       }
//       if (new_data != data) {
//          //Serial.write(new_data); samples
//       }
//       data = new_data;






       if (x > 1024) {
          Serial.write(pos);
          Serial.write(10 + 4); 
       } else if (x > 600) {
          Serial.write(pos);
          Serial.write(10 + 1); 
       } else if (x > 200) {
          Serial.write(pos);
          Serial.write(10 + 2); 
       } else {
          Serial.write(pos);
          Serial.write(10 + 0); 
       }
//       
       
       
       
       
       
       //        if (pos == 0)
//          Serial.println(x);




        
        
        ticks = 0;
      } else {
         if (ticks > 9)
            digitalWrite(LASER, HIGH);
        ticks++;
      }
      break;
    }
}


















void loop() {
  int small_read_delay = 0;//4000;
  int big_read_delay = 0;//4000;
  int motor_delay = 1250;

  digitalWrite(DIR, HIGH);

  for (int i = 0; i < 7; i++) {
    delayMicroseconds(small_read_delay + 1000);

    if (i==4 || true)
        digitalWrite(LASER, HIGH);
    int x = 0;
    for (int j = 0; j < 12; j++) {
      //while(stepper_state==MOVING){}
      int temp = analogRead(A0);
      //Serial.print(temp);
      //Serial.print(" ");
      if (temp > x) {
        x = temp;
      }
    }
    digitalWrite(LASER, LOW);


    if (x > 1024) {
       Serial.write(i);
       Serial.write(10 + 4); 
    } else if (x > 600) {
       Serial.write(i);
       Serial.write(10 + 1); 
    } else if (x > 200) {
       Serial.write(i);
       Serial.write(10 + 2); 
    } else {
       Serial.write(i);
       Serial.write(10 + 0); 
    }
    delayMicroseconds(big_read_delay);

    for (int j = 0; j < 2; j++) {
      digitalWrite(STEP, HIGH);
      delayMicroseconds(motor_delay);
      digitalWrite(STEP, LOW);
      delayMicroseconds(motor_delay);
    }
  }

  digitalWrite(DIR, LOW);












  
  for (int i = 7; i > 0; i--) {
    delayMicroseconds(big_read_delay + 1000);

    if (i == 4 || true)
        digitalWrite(LASER, HIGH);
    int x = 0;
    for (int j = 0; j < 12; j++) {
      //while(stepper_state==MOVING){}
      int temp = analogRead(A0);
      //Serial.print(temp);
      //Serial.print(" ");
      if (temp > x) {
        x = temp;
      }
    }
    digitalWrite(LASER, LOW);


    if (x > 1024) {
       Serial.write(i);
       Serial.write(10 + 4); 
    } else if (x > 600) {
       Serial.write(i);
       Serial.write(10 + 1); 
    } else if (x > 200) {
       Serial.write(i);
       Serial.write(10 + 2); 
    } else {
       Serial.write(i);
       Serial.write(10 + 0); 
    }
    delayMicroseconds(small_read_delay);

    for (int j = 0; j < 2; j++) {
      digitalWrite(STEP, HIGH);
      delayMicroseconds(motor_delay);
      digitalWrite(STEP, LOW);
      delayMicroseconds(motor_delay);
    }
  }




  // while(Serial.available()){
  //   user_input = Serial.read(); //Read user input and trigger appropriate function
  //   digitalWrite(EN, LOW); //Pull enable pin low to set FETs active and allow motor control
  //   if (user_input =='1')
  //   {println
  //      StepForwardDefault();
  //   }
  //   else if(user_input =='2')
  //   {
  //     ReverseStepDefault();
  //   }
  //   else if(user_input =='3')
  //   {
  //     ContStepMode();
  //   }
  //   // else if(user_input =='4')
  //   // {
  //   //   ForwardBackwardStep();
  //   // }
  //   else
  //   {
  //     Serial.println("Invalid option entered.");
  //   }
  //   resetBEDPins();
  //   Serial.println("Enter new option");
  //   Serial.println();
  // }

  // stepMotor();
  // delayMicroseconds(step_dur);




  
//  int x = 0;
//  for (int i = 0; i < 5; i++) {
//    //while(stepper_state==MOVING){}
//    int temp = analogRead(A0);
//    //Serial.println(temp);
//    if (temp > x) {
//      x = temp;
//    }
//  }
//  
//  if (x > 81) {
//    data = data | (1 << pos);
//  } else {
//    data = data & ~(1 << pos);  
//  }
//  Serial.println(x);




  
  //Serial.write(data);
  //delay(100);
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




#define EN   13
#define MS1  12
#define MS2  11
#define MS3  10
#define STEP 9
#define DIR  8
#define LASER 5
#define BUTTON 6



void setup() {
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(LASER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  analogReference(EXTERNAL);


  resetBEDPins(); //Set step, direction, microstep and enable pins to default states

  Serial.begin(115200);

  // turn on laser
  digitalWrite(LASER, HIGH);
  
  // wait for user to position laser and press button to start harp
  while(digitalRead(BUTTON)!=LOW){delay(1);}

}

void loop() {

  int motor_delay = 1250;

  // set direction to left
  digitalWrite(DIR, HIGH);

  // sweep left
  for (int i = 0; i < 7; i++) {

    // delay due to inertia of mirror
    delayMicroseconds(1000);

    // collect and publish sensor data over serial
    // checks if laser beam is broken
    publish_sample(i);
    
    // move the stepper motor two steps
    for (int j = 0; j < 2; j++) {
      digitalWrite(STEP, HIGH);
      delayMicroseconds(motor_delay);
      digitalWrite(STEP, LOW);
      delayMicroseconds(motor_delay);
    }
  }

  // set direction to right
  digitalWrite(DIR, LOW);

  // sweep right
  for (int i = 7; i > 0; i--) {
    delayMicroseconds(1000);
    publish_sample(i);
    for (int j = 0; j < 2; j++) {
      digitalWrite(STEP, HIGH);
      delayMicroseconds(motor_delay);
      digitalWrite(STEP, LOW);
      delayMicroseconds(motor_delay);
    }
  }
}

void publish_sample(int pos) {

 // turn on the laser only when collecting data
  digitalWrite(LASER, HIGH);

  // take the max of 12 samples
  int x = 0;
  for (int j = 0; j < 12; j++) {
    int temp = analogRead(A0);
    if (temp > x) {
      x = temp;
    }
  }

  // turn off the laser after collecting data
  digitalWrite(LASER, LOW);

  // publish the mirror position and highest reading over serial
  uint8_t packet0 = (pos << 4) | ((x >> 7) & 0xF);
  uint8_t packet1 = (1 << 7) | (x & 0x7F);
  Serial.write(packet0);
  Serial.write(packet1); 
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


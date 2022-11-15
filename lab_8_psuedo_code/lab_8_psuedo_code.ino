
//Fernando Perez Zavala, Jacob Navarro 
//Lab 8: Psuedo Code

// Sensors to implement: Ultrasonic Sensor, Tracking sensor, 

#include <NewPing.h>

#define TRIGGER_PIN 4
#define ECHO_PIN 5
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

#define PWM_A_Pin 9 // PWM A
#define DIR_A_Pin 8 // DIR A

#define PWM_B_Pin 11 // PWM B
#define DIR_B_Pin 4 // DIR B

const int trackingPin1 = 7;
const int trackingPin2 = 8; 

int speedA;
int speedB;

const bool forwardA = HIGH; // to indicate HIGH as a forward motion for motor A
const bool backwardA = LOW; // to indicate LOW as a backward motion for motor A

const bool forwardB = HIGH; // to indicate HIGH as a forward motion for motor B
const bool backwardB = LOW; // to indicate HIGH as a backward motion for motor B



void setup() {
  
  //Setup Channel A
  pinMode(DIR_A_Pin, OUTPUT); //Initiates Motor Channel A pin
  pinMode(DIR_B_Pin, OUTPUT); //Initiates Motor Channel B pin
  pinMode(trackingPin1, INPUT); // set trackingPin1 as INPUT
  pinMode(trackingPin2, INPUT);  //set trackingPin2 as INPUT


  Serial.begin(9600);
  
}

void loop() {




  boolean val1 = digitalRead(trackingPin1); // Left Motor
  boolean val2 = digitalRead(trackingPin2); // Right Motor
  
  if(val1 == HIGH)
  {
    if(val2 == HIGH) // Both wheels go forward full speed
    {
      goStraight(forwardA, 200);
      Serial.print("Going Straight!");
      Serial.print('\n');
    }
    else if(val2 == LOW) // Right Motor Off Track: Turning left
    {
      turnLeft();
      Serial.print("Turning Left");
      Serial.print('\n');
    }   
  }
  else if(val2 == HIGH) 
  {
    if(val1 == LOW) // Left Motor Off Track: Turning Right
    {
      turnRight();
      Serial.print("Turning Right");  
      Serial.print('\n');
    }
  }
  else if(val1 == LOW)
  {
    if(val2 == LOW) // Stopping both wheels
    {
      stopMotion();
      Serial.print("Stopping");
      Serial.print('\n');
    }
  }
  else
  {
   stopMotion(); // Stopping both wheels
  }

  

  //goStraight(forwardA,150);
  //motionA(forwardA, 255);
  //motionB(forwardB, 255);
  //turnLeft();

  //delay(2000);

  //turnRight();

}

void motionA(int directionA, int speedA){

   //forward @ full speed
  digitalWrite(DIR_A_Pin, directionA); //Establishes forward direction of Channel A
  analogWrite(PWM_A_Pin, speedA);   //Spins the motor on Channel A at full speed
  
  
}

void motionB(int directionB, int speedB)
{
    digitalWrite(DIR_B_Pin, directionB);
    analogWrite(PWM_B_Pin, speedB);

}

void goStraight(int direction1, int speed1)
{
  digitalWrite(DIR_A_Pin, direction1); //Establishes forward direction of Channel A
  analogWrite(PWM_A_Pin, speed1);   //Spins the motor on Channel A at full speed

  digitalWrite(DIR_B_Pin, direction1);  //motion B
  analogWrite(PWM_B_Pin, speed1);
  
}

void turnLeft()
{
  motionA(forwardA, 90); // motorA left wheel goes half speed
  motionB(forwardB, 180); // motorB right wheel goes full speed to initiate left turn
  
}

void turnRight()
{
  motionA(forwardA, 180); // motorA left wheel goes full speed to initiate right turn
  motionB(forwardB, 90); // motorB right wheel goes half speed

}

void stopMotion()
{
    // set both channel PWM pins to 0
    analogWrite(PWM_A_Pin, 0);
    analogWrite(PWM_B_Pin, 0);  
    
    //analogWrite(PWM_A_Pin, speedA);
   
    //analogWrite(PWM_B_Pin, speedB);
}

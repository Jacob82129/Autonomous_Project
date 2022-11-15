
//Fernando Perez Zavala, Jacob Navarro 
//Lab 8: Psuedo Code

// Sensors to implement: Ultrasonic Sensor, Tracking sensor, 

#include <NewPing.h>

//#define TRIGGER_PIN 4
//#define ECHO_PIN 5
//#define MAX_DISTANCE 400

//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

#define PWM_A_Pin 9 // PWM A
#define DIR_A_Pin 8 // DIR A

#define PWM_B_Pin 11 // PWM B
#define DIR_B_Pin 4 // DIR B

#define s0 2        //Module pins wiring
#define s1 3
#define s2 6
#define s3 7
#define out 12

const int trackingPin1 = 7;
const int trackingPin2 = 8; 

int speedA;
int speedB;

const bool forwardA = HIGH; // to indicate HIGH as a forward motion for motor A
const bool backwardA = LOW; // to indicate LOW as a backward motion for motor A

const bool forwardB = HIGH; // to indicate HIGH as a forward motion for motor B
const bool backwardB = LOW; // to indicate HIGH as a backward motion for motor B

int red = 0, blue = 0, green = 0; // RGB values

void setup() {
  
  //Setup Channel A
  pinMode(DIR_A_Pin, OUTPUT); //Initiates Motor Channel A pin
  pinMode(DIR_B_Pin, OUTPUT); //Initiates Motor Channel B pin
  pinMode(trackingPin1, INPUT); // set trackingPin1 as INPUT
  pinMode(trackingPin2, INPUT);  //set trackingPin2 as INPUT

  pinMode(s0,OUTPUT);    //pin modes
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);


  Serial.begin(9600);

  digitalWrite(s0,HIGH); //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100% (recommended)
  digitalWrite(s1,HIGH); //LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is  2%
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

  GetColors();

  if (red <=15 && green <=15 && blue <=15){         //If the values are low it's likely the white color (all the colors are present)
      Serial.println("White");                    
  }
  else if (red<blue && red<=green && red<23){      //if Red value is the lowest one and smaller thant 23 it's likely Red
      Serial.println("Red");
      stopMotion();
      Serial.print("Stopping");
      Serial.print('\n');
      delay(2000);
      goStraight(forwardA, 150);
  }
  else if (blue<green && blue<red && blue<20){    //Same thing for Blue
      Serial.println("Blue");
  }
  else if (green<red && green-blue<= 8){           
      Serial.println("Green");                    
  }
  else{
     Serial.println("Unknown");                  //if the color is not recognized, you can add as many as you want
  }

  delay(2000);                                   //2s delay you can modify if you want
  

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

void GetColors()  
{    
  digitalWrite(s2, LOW);                                           //S2/S3 levels define which set of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH is for green 
  digitalWrite(s3, LOW);                                           
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);       //here we wait until "out" go LOW, we start measuring the duration and stops when "out" is HIGH again, if you have trouble with this expression check the bottom of the code
  delay(20);  
  digitalWrite(s3, HIGH);                                         //Here we select the other color (set of photodiodes) and measure the other colors value using the same techinque
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
  digitalWrite(s2, HIGH);  
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
}

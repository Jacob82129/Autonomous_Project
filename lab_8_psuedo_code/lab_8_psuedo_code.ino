
//Fernando Perez Zavala, Jacob Navarro 
//Tracking Car Project

// Sensors to implement: Color Sensor, Tracking sensor, and IR Object Detector


#define PWM_A_Pin 9 // PWM A
#define DIR_A_Pin 8 // DIR A

#define PWM_B_Pin 11 // PWM B
#define DIR_B_Pin 4 // DIR B

#define s0 4        //Module pins wiring
#define s1 5
#define s2 6
#define s3 10
#define out 12  

// initializing tracking pins as numerical input ports
const int trackingPin1 = 7;
const int trackingPin2 = 8; 

bool leftFlag = false;
bool rightFlag = false;
bool straightFlag = false;
bool stopFlag = false;
//bool colorFlag = false;

int speedA;
int speedB;

const bool forwardA = HIGH; // to indicate HIGH as a forward motion for motor A
const bool backwardA = LOW; // to indicate LOW as a backward motion for motor A

const bool forwardB = HIGH; // to indicate HIGH as a forward motion for motor B
const bool backwardB = LOW; // to indicate LOW as a backward motion for motor B

int frequencyR = 0;
int frequencyG = 0;
int frequencyB = 0;

//int interrupt_pin = s2;

void setup() {
  
  //Setup Channel A
  pinMode(DIR_A_Pin, OUTPUT); //Initiates Motor Channel A pin
  pinMode(DIR_B_Pin, OUTPUT); //Initiates Motor Channel B pin
  pinMode(trackingPin1, INPUT); // set trackingPin1 as INPUT
  pinMode(trackingPin2, INPUT);  //set trackingPin2 as INPUT

  //pinMode(interrupt_pin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(interrupt_pin), stopColor, CHANGE);
  
  
  pinMode(s0,OUTPUT);    //pin modes
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);


  Serial.begin(9600);

  digitalWrite(s0,HIGH); //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100% (recommended)
  digitalWrite(s1,HIGH); //LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is  2%
 
}


void loop() 
{

  boolean val1 = digitalRead(trackingPin1); // Left Motor
  boolean val2 = digitalRead(trackingPin2); // Right Motor

  
  if(val1 == HIGH && val2 == HIGH && (straightFlag == false))
  {
    goStraight(forwardA, 230);
    straightFlag == true;
  }
  else if(val1 == LOW && val2 == HIGH && (leftFlag == false))
  {
    turnLeft();
    leftFlag == true;
    
  }
  else if(val1 == HIGH && val2 == LOW && (rightFlag == false))
  {
    turnRight();
    rightFlag == true;
  }
  else if(val1 == LOW && val2 == LOW && (stopFlag == false))
  {
    stopMotion();
    stopFlag == true;
  }    

  stopColor();
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
  motionA(forwardA, 100); // motorA left wheel goes half speed
  motionB(forwardB, 255); // motorB right wheel goes full speed to initiate left turn
  
}

void turnRight()
{
  motionA(forwardA, 255); // motorA left wheel goes full speed to initiate right turn
  motionB(forwardB, 100); // motorB right wheel goes half speed

}

void stopMotion()
{
    // set both channel PWM pins to 0 which will stop both motors
    analogWrite(PWM_A_Pin, 0);
    analogWrite(PWM_B_Pin, 0);  
     
}

void stopColor()
{
  
  //set red  to be read
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  //Reading the output frequency
  frequencyR = pulseIn(out, LOW);
  frequencyR = map(frequencyR, 25, 70, 255, 0);
  //Printing the value on the serial monitor
  /*
  Serial.print("R = ");
  Serial.print(frequencyR);
  Serial.print(" ");
  delay(100);
*/
  //Setting Green to be read
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  frequencyG = pulseIn(out, LOW);
  frequencyG = map(frequencyG, 25, 70, 255, 0);
/*  
  Serial.print("G = ");
  Serial.print(frequencyG);
  Serial.print(" ");
  delay(100);
*/
  //Setting Blue to be read
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  frequencyB = pulseIn(out, LOW);
  frequencyB = map(frequencyB, 25, 70, 255, 0);
  /*
  Serial.print("B = ");
  Serial.print(frequencyB);
  Serial.print(" ");
  Serial.println("");
  delay(100);
*/
  if(frequencyR >= 345 && frequencyG <= 210 && frequencyB <= 255) 
  {      
     Serial.println("Red");
      stopMotion();
      Serial.print("Stopping");
      Serial.print('\n');
      delay(5000);
      goStraight(forwardA, 255);
      delay(3000);
      //colorFlag == true;
  }

}

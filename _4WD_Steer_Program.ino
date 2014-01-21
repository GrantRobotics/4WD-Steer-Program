#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);

boolean fullTurn = false;

int RJX = 6; //Right Joystick X
int LJY = 5; //Left Joystick Y

float RJXVal;
float LJYVal;

float RMult; // Right wheels multiplyer
float LMult; // Left wheels multipler

float totalSpeedRight;
float totalSpeedLeft;


//Max right joystick X = 504
//Low right joystick X = - 484

//Max left joystick Y = 475
//Low left joystick Y = - 550

void setup(){
  AFMS.begin();
  pinMode(RJX, INPUT); // Set RSX as an input
  pinMode(LJY, INPUT); // Set LSY as an input
  Serial.begin(9600); // Startup serial
}
void loop(){
   
  RJXVal= pulseIn(RJX, HIGH, 25000); // Read the pulse width of right joystick X
  LJYVal = pulseIn(LJY, HIGH, 25000); // Read the pulse width of left joystick Y
  
  RJXVal = RJXVal - 1300; // Centers RSXVal
  LJYVal = LJYVal - 1523; // Centers LSYVal
  
  if(abs(RJXVal) < 12){
    RJXVal = 0; // Set RJXVal to 0
  }
  if(abs(LJYVal) < 12){ //Checks if 
    LJYVal = 0; // Set LJYVal to 0
  }
  
  Serial.print("RJXVal (Before): ");  
  Serial.println(RJXVal);  
  Serial.print("LJYVal (Before): ");
  Serial.println(LJYVal);
  
  if (RJXVal > 0) {
    RJXVal =  RJXVal / 504;
  }
  if (RJXVal < 0) {
    RJXVal = RJXVal / 484;
  }  
  if(LJYVal > 0){
    LJYVal = LJYVal / 4.75;
  }
  if(LJYVal < 0){
    LJYVal = LJYVal / 5.50;
  }
  
  Serial.print("RJXVal (After): ");  
  Serial.println(RJXVal);  
  Serial.print("LJYVal (After): ");
  Serial.println(LJYVal);
  
  if(abs(LJYVal) < 5){
    if(abs(RJXVal) > .5){
      fullTurn = true;
      
      if(RJX > 0){
        motor1->run(FORWARD);
        motor2->run(FORWARD);
        motor3->run(BACKWARD);
        motor4->run(BACKWARD);
        
        motor1->setSpeed(100);
        motor2->setSpeed(100);
        motor3->setSpeed(100);
        motor4->setSpeed(100);
      }
      else if(RJX < 0){
        motor1->run(BACKWARD);
        motor2->run(BACKWARD);
        motor3->run(FORWARD);
        motor4->run(FORWARD);
        
        motor1->setSpeed(100);
        motor2->setSpeed(100);
        motor3->setSpeed(100);
        motor4->setSpeed(100);
        
      }  
    }
  }
  if(!fullTurn){ 
    if(RJXVal == 0) {
      RMult = 1;
      LMult = 1;
    }
    if(RJXVal > 0){
     LMult = 1;
     RMult = (RJXVal * - 2) + 1; 
    }
    if(RJXVal < 0){
     LMult = (RJXVal * 2) + 1;
     RMult = 1; 
    }
  
    totalSpeedLeft = LMult * LJYVal;
    totalSpeedRight = RMult * LJYVal;
  
  
    if(totalSpeedLeft > 0 ){
      motor1->run(FORWARD);
      motor2->run(FORWARD);
    
      motor1->setSpeed(totalSpeedLeft); 
      motor2->setSpeed(totalSpeedLeft);
    }
    if(totalSpeedLeft < 0){    
      motor1->run(BACKWARD);
      motor2->run(BACKWARD);
    
      motor1->setSpeed(-totalSpeedLeft);
      motor2->setSpeed(-totalSpeedLeft); 
    }
  
    if(totalSpeedRight > 0 ){    
      motor3->run(FORWARD);
      motor4->run(FORWARD);
       
      motor3->setSpeed(totalSpeedRight); 
      motor4->setSpeed(totalSpeedRight);
    }
    if(totalSpeedRight < 0){   
      motor3->run(BACKWARD);
      motor4->run(BACKWARD);

      motor3->setSpeed(-totalSpeedRight); 
      motor4->setSpeed(-totalSpeedRight);
    }
    

    //Serial.println(map(RJXVal, 1000,2000,-500,500)); // center at 0
    //Serial.println(map(LSYVal, 1000,2000,-500,500)); // center at 0
  }

    Serial.println(); //Prints a line into the serial monitor.
    
    delay(10);
    fullTurn = false;
  
}

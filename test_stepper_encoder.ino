#include <AccelStepper.h>

#include <Wire.h>
#include "rgb_lcd.h"

#define encoderPinA  3 // Channel A must be on interrupt 0
#define encoderPinB  4 // Channel B can be on any pin
 
rgb_lcd lcd;
 
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

////// STEPPER DIRECTION AND STEP PINS:
const int dirPin1 = 5; //// X on CNC shield
const int stepPin1 = 2;


const int enablePin = 8;


// Define motor interface type (type 1 means an external stepper driver with Step and Direction pins)
#define motorInterfaceType 1

// Define steppers and the pins they use
AccelStepper stepper1(motorInterfaceType, stepPin1, dirPin1);

// Global variable
volatile int encoder_count = 0;

int distance = 5000;
String str="0";

unsigned long previousTimeLed1 = millis();
long timeIntervalLed1 = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(1); 

  // other stuff
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(1, doEncoderA, RISING);

   // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
 
    lcd.setRGB(colorR, colorG, colorB);
 
    // Print a message to the LCD.
    lcd.print("Maker Tutor!");


  pinMode(enablePin, OUTPUT); // CNC shield enable pin
  digitalWrite(enablePin, HIGH);


  /////////// STEPPER PROPERTIES:

  stepper1.setMaxSpeed(4000); //// From Accelstepper: "Max possible speed depends on clock speed. Caution: Speeds that exceed the maximum speed supported by the processor may Result in non-linear accelerations and decelerations"
  stepper1.setAcceleration(4000);

  stepper1.moveTo(distance);

  delay(3000);
} // Setup End

void loop() {
  
  if( stepper1.distanceToGo() == 0 ){
          distance = distance * -1;
          stepper1.moveTo( distance * -1 );
          delay(3000);
  }
  stepper1.run();
  
  

   unsigned long currentTime = millis();
  // task 1
  if(currentTime - previousTimeLed1 > timeIntervalLed1) {
    previousTimeLed1 = currentTime;
    str = String(encoder_count);
    lcd.clear();
  lcd.print(str);
  }
}

void doEncoderA() { // Rising edge of wave A initiates pin B state reading to determine direction
  // If pin B is low when A trips, then A precedes B
  if(digitalRead(encoderPinB) == LOW) {
    encoder_count = encoder_count + 1; //Positive Rotation
  }

  if(digitalRead(encoderPinB) == HIGH) {
    encoder_count = encoder_count - 1; // Negative Rotation
  }
 // Serial.print("Encoder count:  ");
 // Serial.println (encoder_count);
  
  //lcd.clear();
  //lcd.print(str);
  delay(10);
  // use for debugging - remember to comment out
}

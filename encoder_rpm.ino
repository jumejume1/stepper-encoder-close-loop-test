// use this for scara robot 101
#include <AccelStepper.h>

#include <Wire.h>
#include "rgb_lcd.h"

#define encoderPinA  3 // Channel A must be on interrupt 0
#define encoderPinB  4 // Channel B can be on any pin
 
rgb_lcd lcd;
 
const int colorR = 100;
const int colorG = 100;
const int colorB = 100;

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

int distance = 800000;
String str="0";

char buf [6];


unsigned long previousTimeLed1 = millis();
long timeIntervalLed1 = 300;
unsigned long currentTime;

unsigned long start;
volatile long pulse;
volatile bool pinB, pinA, dir;
const byte ppr = 1000, upDatesPerSec = 2;
const int fin = 1000 / upDatesPerSec;
const float konstant = 60.0 * upDatesPerSec / (ppr * 2);
int rpm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(1); 

  // other stuff
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(1, readEncoder, CHANGE);

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
  lcd.clear();
} // Setup End

void loop() {

  currentTime = millis();
  
  if( stepper1.distanceToGo() == 0 ){
          distance = distance * -1;
          stepper1.moveTo( distance * -1 );
          delay(3000);
  }
  stepper1.run();

  if(millis() - start > fin)
  {
    start = millis();
    rpm = abs(pulse * konstant);
    //Serial.println(rpm);
    pulse = 0;
    lcd.clear();
    //lcd.setCursor(0,0);
    lcd.print(rpm);
    lcd.print(" RPM");
  }
    
  
}

void readEncoder()
{
  pinA = bitRead(PIND,encoderPinA);
  pinB = bitRead(PIND,encoderPinB);
  dir = pinA ^ pinB;          // if pinA & pinB are the same
  dir ? --pulse : ++pulse;    // dir is CW, else CCW
}


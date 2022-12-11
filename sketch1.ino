
#include <AccelStepper.h>

////// STEPPER DIRECTION AND STEP PINS:
const int dirPin1  = 5; 
const int stepPin1 = 2;


const int enablePin = 8;

// Define motor interface type (type 1 means an external stepper driver with Step and Direction pins)
#define motorInterfaceType 1

// Define steppers and the pins they use
AccelStepper stepper1(motorInterfaceType, stepPin1, dirPin1);


int distance = 5000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(1); 


  pinMode(enablePin, OUTPUT); 
  digitalWrite(enablePin, HIGH); // If not working set to LOW for other drivers.


  /////////// STEPPER PROPERTIES:

  stepper1.setMaxSpeed(4000); //// From Accelstepper: "Max possible speed depends on clock speed. Caution: Speeds that exceed the maximum speed supported by the processor may Result in non-linear accelerations and decelerations"
  stepper1.setAcceleration(4000);

  stepper1.moveTo(distance);
} // Setup End






void loop() {
  
  if( stepper1.distanceToGo() == 0 ){
          distance = distance * -1;
          stepper1.moveTo( distance * -1 );
          delay(3000);
  }
  stepper1.run();
}

/* RC Receiver
   Jose Tovar and Keren Elvir
   Trevecca Nazarene University
   SP2019
*/

// NECESSARY LIBRARIES
#include <Servo.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

//  DEFINITIONS NRF24 MODULE
#define CE 7
#define CSN 8

// SERVO PINS
const byte steeringPin = 9;
const byte grippPin = XXXX;
const byte liftPin = xxxxx;

// SERVOS BEING USED
Servo steering;
Servo gripp;
Servo lift;

// NRF24 MISC
int address[6] = "00119";
RF24 radio(CE, CSN);

// DC MOTOR PINS
const byte rightMotor = 3; // ENABLE FOR RIGHT MOTOR
const byte rightMotor_forward = 2;
const byte rightMotor_backward = 4
const byte leftMotor = 5; //  ENABLE FOR LEFT MOTOR
const byte leftMotor_forward = 10;
const byte leftMotor_backward = 12;



void setup()
{
  //  SERIAL SETUP
  Serial.begin();
  while (!Serial);

  //  RF24 SETUP
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  //  SERVOS SETUP
  steering.attach(steeringPin);
  gripp.attach(grippPin);
  lift.attach(liftPin);

  //  DC MOTORS SETUP
  pinMode(rightMotor, OUTPUT);
  pinMode(rightMotor_forward, OUTPUT);
  pinMode(rightMotor_backward, OUTPUT);
  pinMode(leftMotor, OUTPUT);
  pinMode(leftMotor_forward, OUTPUT);
  pinMode(leftMotor_backward, OUTPUT);
}

void loop()
{


}

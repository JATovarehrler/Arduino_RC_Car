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
const byte grippPin = 2;
const byte liftPin = 4;

// SERVOS BEING USED
Servo steering;
Servo gripper;
Servo lift;

// NRF24 MISC
const byte address[6] = "00119";
RF24 radio(CE, CSN);

// DC MOTOR PINS
const byte rightMotor = 3; // ENABLE FOR RIGHT MOTOR
const byte rightMotor_forward = 2;
const byte rightMotor_backward = 4;
const byte leftMotor = 5; //  ENABLE FOR LEFT MOTOR
const byte leftMotor_forward = 10;
const byte leftMotor_backward = 12;

//  STRUCTURE TO BE RECEIVED
struct robot_pkt {
  int speed;
  int steering;
  int gripper_grip;
  int gripper_height;
  int left_forward;
  int right_forward;
  int left_backward;
  int right_backward;
} ;
struct robot_pkt pkt;


void setup()
{
  //  SERIAL SETUP
  Serial.begin(9600);
  while (!Serial);

  //  RF24 SETUP
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  
  //  SERVOS SETUP
  steering.attach(steeringPin);
  gripper.attach(grippPin);
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
  if (radio.available()) {
    radio.read(&pkt, sizeof(pkt));
    
    //  TRANSMIT INFORMATION TO SERVOS
    steering.write(pkt.steering);
    gripper.write(pkt.gripper_grip);
    lift.write(pkt.gripper_height);

    //  TRANSMIT INFORMATION TO DC MOTORS
    analogWrite(rightMotor, pkt.speed);
    analogWrite(leftMotor, pkt.speed);
    digitalWrite(rightMotor_forward, pkt.right_forward);
    digitalWrite(rightMotor_backward, pkt.right_backward);
    digitalWrite(leftMotor_forward, pkt.left_forward); // IF PKT.LEFT_FORWARD=HIGH, LEFT_BACKWARD=LOW
    digitalWrite(leftMotor_backward, pkt.left_backward);
  }
}

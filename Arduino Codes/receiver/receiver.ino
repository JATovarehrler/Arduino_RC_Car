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
RF24 radio(7, 8);

// DC MOTOR PINS
const byte rightMotor = 5; // ENABLE FOR RIGHT MOTOR
const byte rightMotor_forward = A3;
const byte rightMotor_backward = A2;
const byte leftMotor = 3; //  ENABLE FOR LEFT MOTOR
const byte leftMotor_forward = A1;
const byte leftMotor_backward = A0;

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
  // Serial.begin(9600);
  delay(1000);
  //  RF24 SETUP
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);      // set data rate through the air
  radio.startListening();


  //  SERVOS SETUP
  steering.attach(steeringPin);
  gripper.attach(grippPin);
  lift.attach(liftPin);
  steering.write(90);
  lift.write(90);
  gripper.write(120);

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
    digitalWrite(6, HIGH);
    radio.read(&pkt, sizeof(pkt));
    //Serial.println(pkt.steering);

    //  TRANSMIT INFORMATION TO SERVOS
    steering.write(pkt.steering);
    gripper.write(pkt.gripper_grip);
    lift.write(pkt.gripper_height);

    //  TRANSMIT INFORMATION TO DC MOTORS
    digitalWrite(rightMotor_forward, pkt.right_forward);
    digitalWrite(rightMotor_backward, pkt.right_backward);
    digitalWrite(leftMotor_forward, pkt.left_forward); // IF PKT.LEFT_FORWARD=HIGH, LEFT_BACKWARD=LOW
    digitalWrite(leftMotor_backward, pkt.left_backward);

    //  SPEED RAMP
    
      analogWrite(5, pkt.speed);
      analogWrite(3, pkt.speed);
    
  }

}

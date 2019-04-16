/* RC Transmitter
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

//  STRUCTURE TO BE RECEIVED
struct robot_pkt {
  unsigned int sequence;
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

//  NRF24 MISC
const byte address[6] = "00119";
RF24 radio(CE, CSN);

//  POT PINS
const byte gripPin = 0;
const byte heightPin = 1;

//  GLOBAL VARIABLES
int seq = 0;


void setup()
{
  // SERIAL SETUP
  Serial.begin(9600);
  while (!Serial);

  // RF24 SETUP
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  //  INITIAL VALUES
  pkt.gripper_grip = 90;
  pkt.gripper_height = 90;

  //  PIN DECLARATIONS
  pinMode(gripPin, INPUT);
  pinMode(heightPin, INPUT);
}

void loop()
{
  seq++;
  pkt.sequence = seq;
  pkt.gripper_grip = analogRead(gripPin);
  pkt.gripper_grip = map(pkt.gripper_grip, 0, 1023, 0, 170);
  pkt.gripper_height = analogRead(heightPin);
  pkt.gripper_height = map(pkt.gripper_height, 0, 1023, 0, 170);

  radio.write(&pkt, sizeof(pkt));
  delay(20);
  //Serial.println(pkt.gripper_grip);
  //Serial.println(pkt.gripper_height);
}

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
  int gripper_height=90;
  int left_forward;
  int right_forward;
  int left_backward;
  int right_backward;
} ;
struct robot_pkt pkt;

//  NRF24 MISC
const byte address[6] = "00119";
RF24 radio(CE, CSN);

//  BUTTON PINS
const byte upPin = 4;
const byte downPin = 5;
const byte fullGrip_pin = 6;
const byte downGrip_pin = 3;

//  JOYSTICK PINS
const byte Vx = 0;
const byte Vy = 3;
const byte Sw = 2;

//  LED PIN
const byte LED = 9;

//  GLOBAL VARIABLES
int seq = 0;

//  DEBOUNCING STATES
byte upStateOld;
byte downStateOld;

//  INIT
int bs = 0;

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

  


  //  PIN DECLARATIONS
  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(fullGrip_pin, INPUT_PULLUP);
  pinMode(Sw, INPUT_PULLUP);
}

void loop()
{
  seq++;
  pkt.sequence = seq;

  //  CURRENT STATES FOR BUTTONS
  int upState;
  upState = digitalRead(upPin);
  int downState;
  downState = digitalRead(downPin);

  //  LOGIC
  if ((upState == LOW) && (upStateOld == HIGH)) {
    pkt.gripper_height = (pkt.gripper_height) + 10;
    bs=bs++;
    if (pkt.gripper_height > 180) {
      pkt.gripper_height = 180;
    }
  }
   upStateOld = digitalRead(upPin);

  int height=analogRead(Vy);
int   y=map(height,0,1023,0,180);
  pkt.gripper_height=y;
 Serial.println(y);
  radio.write(&pkt, sizeof(pkt));
  delay(20);
}

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

//  BUTTON PINS
const byte upPin = 4;
const byte downPin = 5;
const byte fullGrip_pin = 3;
const byte downGrip_pin = 6;

//  JOYSTICK PINS
const byte Vx = 0;
const byte Vy = 1;
const byte Sw = 2;

//  DEBOUNCING STATES
byte upStateOld;
byte downStateOld;

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
  //  GET RETURNS FROM FUNCTIONS
  pkt.gripper_height=height_control();
  //  TRANSMIT RADIO SIGNALS
  radio.write(&pkt, sizeof(pkt));
  delay(20);
}

int height_control()  //  FUNCTION TO CONTROL THE HEIGHT OF THE GRIPPER
{
  //  INITIAL VARIABLES
  int gripper_height = 90;
  //  CURRENT STATES FOR BUTTONS
  int upState;
  upState = digitalRead(upPin);
  int downState;
  downState = digitalRead(downPin);

  //  UP BUTTON LOGIC AND DEBOUNCE
  if ((upState == LOW) && (upStateOld == HIGH)) {
    gripper_height = gripper_height + 10; //  ADD TEN DEGREES

    if (gripper_height > 180) {
      gripper_height = 180;
    }
  }
  upStateOld = digitalRead(upPin); //  DEBOUNCE

  //  DOWN BUTTON LOGIC AND DEBOUNCE
  if ((downState == LOW) && (downStateOld == HIGH)) {
    gripper_height = gripper_height - 10; //  SUBTRACT 10 DEGREES

    if (gripper_height > 180) {
      gripper_height = 180;
    }
  }
  downStateOld = digitalRead(downPin); //  DEBOUNCE

  return gripper_height;
}

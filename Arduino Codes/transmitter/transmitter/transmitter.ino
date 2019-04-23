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
const byte noGrip_pin = 6;

//  JOYSTICK PINS
const byte Xpin = A0;
const byte Ypin = A1;
const byte Sw = A2;

//  DEBOUNCING STATES
byte upStateOld;
byte downStateOld;
byte leftStateOld;
byte rightStateOld;

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

  //  BUTTON PIN DECLARATIONS
  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(fullGrip_pin, INPUT_PULLUP);
  pinMode(noGrip_pin, INPUT_PULLUP);

  //  JOYSTICK PIN DECLARATIONS
  pinMode(Ypin, INPUT);
  pinMode(Xpin, INPUT);
  pinMode(Sw, INPUT_PULLUP);
}

void loop()
{
  //  GET RETURNS FROM FUNCTIONS
  pkt.gripper_height = height_control();
  pkt.gripper_grip = grip_control();
  pkt.steering = steering_control();
  //  TRANSMIT RADIO SIGNALS
  radio.write(&pkt, sizeof(pkt));
  delay(20);
}

//  FUNCTIONS
int height_control()  //  FUNCTION TO CONTROL THE HEIGHT OF THE GRIPPER
{
  //  CURRENT STATES FOR BUTTONS
  int upState;
  upState = digitalRead(upPin);
  int downState;
  downState = digitalRead(downPin);

  //  UP BUTTON LOGIC AND DEBOUNCE
  if ((upState == LOW) && (upStateOld == HIGH)) {
    pkt.gripper_height = pkt.gripper_height + 10; //  ADD TEN DEGREES

    if (pkt.gripper_height > 180) {
      pkt.gripper_height = 180;
    }
  }
  upStateOld = digitalRead(upPin); //  DEBOUNCE

  //  DOWN BUTTON LOGIC AND DEBOUNCE
  if ((downState == LOW) && (downStateOld == HIGH)) {
    pkt.gripper_height = pkt.gripper_height - 10; //  SUBTRACT 10 DEGREES

    if (pkt.gripper_height > 180) {
      pkt.gripper_height = 180;
    }
  }
  downStateOld = digitalRead(downPin); //  DEBOUNCE

  return pkt.gripper_height;
}

int grip_control() //  FUNCTION TO CONTROL THE GRIPP LEVEL
{
  //  CURRENT STATE FOR BUTTONS
  int rightState;
  rightState = digitalRead(fullGrip_pin);
  int leftState;
  leftState = digitalRead(noGrip_pin);

  //  FULL GRIP LOGIC AND DEBOUNCE
  if ((rightState == LOW) && (rightStateOld == HIGH)) {
    pkt.gripper_grip = 160;
  }
  rightStateOld = digitalRead(fullGrip_pin); //  DEBOUNCE

  //  NO GRIP LOGIC AND DEBOUNCE
  if ((leftState == LOW) && (leftStateOld == HIGH)) {
    pkt.gripper_grip = 0;
  }
  leftStateOld = digitalRead(noGrip_pin); //  DEBOUNCE
}

int Xval_readout()  //  FUNCTION TO READ X VALUES FROM JOYSTICK
{
  //  USED VARIABLES
  int Xval;
  Xval = analogRead(Xpin);
  return Xval;

}

int Yval_readout()  //  FUNCTION TO READ Y VALUES FROM JOYSTICK
{
  int Yval;
  Yval = analogRead(Ypin);
  return Yval;
}

int steering_control()
{
  //  VARIABLES BEING USED
  int Xval = Xval_readout();
  int Yval = Yval_readout();

  //  STEERING LOGIC
  if ((Xval > 516) && (Yval > 516)) { //  FIRST QUADRANT
    pkt.steering = atan(Yval / Xval);
    pkt.steering = map(pkt.steering, 0, 180, 80, 103);
  }
  if ((Xval < 516) && (Yval > 516)) { //  SECOND QUADRANT
    pkt.steering = atan(Yval / Xval);
    pkt.steering = map(pkt.steering, 0, 180, 103, 120);
  }
  if ((Xval < 516) && (Yval < 516)) { //  THIRD QUADRANT
    pkt.steering = atan(Yval / Xval);
    pkt.steering = map(pkt.steering, 0, 180,103,120);
  }
  if ((Xval > 516) && (Yval < 516)) { //  FOURTH QUADRANT
    pkt.steering = atan(Yval / Xval);
    pkt.steering = map(pkt.steering, 0, 180,80,103);
  }
  return pkt.steering;
}

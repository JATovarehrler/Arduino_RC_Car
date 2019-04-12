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

//  DEFINITIONS
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
RF24 radio(CE,CSN);



void setup()
{
  // SERIAL SETUP
  Serial.begin();
  while (!Serial);
  
  //RF24 SETUP
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  // SERVOS SETUP
  steering.attach(steeringPin);
  gripp.attach(grippPin);
  lift.attach(liftPin);
  
}

void loop()
{


}

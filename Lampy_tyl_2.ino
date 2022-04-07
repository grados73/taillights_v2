  /*
  ******************************************************************************
  ******************************************************************************
  * @file       : Lampy_tyl_2
  * @project		: Electric Car
  * @author			: grados73 - https://github.com/grados73
  * @purpose		: File with program to control rear lamp
  * @TODO			  : New logic of state machine
  ******************************************************************************
  ******************************************************************************
  */
#include <Adafruit_NeoPixel.h>
//
// Structure of LAMPS current state
//
typedef enum
{
LAMP_STATE_POZYCJA,             //1
LAMP_STATE_STOP,                //2
LAMP_STATE_TURN_L_STOP,         //3
LAMP_STATE_TURN_R_STOP,         //4
LAMP_STATE_TURN_L_REVERSE,      //5
LAMP_STATE_TURN_R_REVERSE,      //6
LAMP_STATE_HAZARD_LIGHTS,       //7
LAMP_STATE_HAZARD_REVEERSE,     //8
LAMP_STATE_TURN_L_HEADLIGHTS,   //9
LAMP_STATE_TURN_P_HEADLIGHTS,   //10
LAMP_STATE_REVERSE,             //11
LAMP_STATE_STOP_REVERSE,        //12
LAMP_STATE_HAZARD_REVERSE,      //13
LAMP_STATE_LIGHTS_OFF,          //14
LAMP_STATE_ACTIVATION,          //15     
LAMP_STATE_SHUTDOWN             //16


} LampState;

void setup() {
  // put your setup code here, to run once:
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  
  //
  //Code to check if arduino work correctly
  //
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                        // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(100); 
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);  
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);                         // wait for a second
}

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

typedef int receivedCommand;
receivedCommand NewCommand;

//
// Structure of LAMPS current state
//
typedef enum
{
LAMP_STATE_HEADLIGHTS,          //1
LAMP_STATE_STOP,                //2
LAMP_STATE_TURN_L_STOP,         //3
LAMP_STATE_TURN_R_STOP,         //4
LAMP_STATE_TURN_L_REVERSE,      //5
LAMP_STATE_TURN_R_REVERSE,      //6
LAMP_STATE_HAZARD_LIGHTS,       //7
LAMP_STATE_HAZARD_REVERSE,      //8
LAMP_STATE_TURN_L_HEADLIGHTS,   //9
LAMP_STATE_TURN_R_HEADLIGHTS,   //10
LAMP_STATE_REVERSE,             //11
LAMP_STATE_STOP_REVERSE,        //12
LAMP_STATE_HAZARD_STOP,         //13
LAMP_STATE_LIGHTS_OFF,          //14
LAMP_STATE_ACTIVATION,          //15     
LAMP_STATE_SHUTDOWN             //16
} RearLampState;

RearLampState lampState;

// Prototype of function routine made in every state
void HeadlightsRoutine(receivedCommand NewCommand);
void StopRoutine(receivedCommand NewCommand);
void TurnLStopRoutine(receivedCommand NewCommand);
void TurnRStopRoutine(receivedCommand NewCommand);
void TurnLReverseRoutine(receivedCommand NewCommand);
void TurnRReverseRoutine(receivedCommand NewCommand);
void HazardLightsRoutine(receivedCommand NewCommand);
void HazardReverseRoutine(receivedCommand NewCommand);
void TurnLHeadlightsRoutine(receivedCommand NewCommand);
void TurnRHeadlightsRoutine(receivedCommand NewCommand);
void ReverseRoutine(receivedCommand NewCommand);
void StopReverseRoutine(receivedCommand NewCommand);
void HazardStopRoutine(receivedCommand NewCommand);
void LightsOffRoutine(receivedCommand NewCommand);
void ActivationsRoutine(receivedCommand NewCommand);
void ShutdownRoutine(receivedCommand NewCommand);



void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize variable enum which will hold current state of rear lamp
  lampState = LAMP_STATE_ACTIVATION;
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

void SwitchLampRegular()
{
  switch(lampState)
  {
    case LAMP_STATE_HEADLIGHTS:
      HeadlightsRoutine(NewCommand);
      break;

    case LAMP_STATE_STOP:
      StopRoutine(NewCommand);
      break;

    case LAMP_STATE_TURN_L_STOP:
      TurnLStopRoutine(NewCommand);
      break;

    case LAMP_STATE_TURN_R_STOP:
      TurnRStopRoutine(NewCommand);
      break;

    case LAMP_STATE_TURN_L_REVERSE:
      TurnLReverseRoutine(NewCommand);
      break;

    case LAMP_STATE_TURN_R_REVERSE:
      TurnRReverseRoutine(NewCommand);
      break;

    case LAMP_STATE_HAZARD_LIGHTS:
      HazardLightsRoutine(NewCommand);
      break;

    case LAMP_STATE_HAZARD_REVERSE:
      HazardReverseRoutine(NewCommand);
      break;

    case LAMP_STATE_TURN_L_HEADLIGHTS:
      TurnLHeadlightsRoutine(NewCommand);
      break;
    
    case LAMP_STATE_TURN_R_HEADLIGHTS:
      TurnRHeadlightsRoutine(NewCommand);
      break;
    
    case LAMP_STATE_REVERSE:
      ReverseRoutine(NewCommand);
      break;

    case LAMP_STATE_STOP_REVERSE:
      StopReverseRoutine(NewCommand);
      break;

    case LAMP_STATE_HAZARD_STOP:
      HazardStopRoutine(NewCommand);
      break;

    case LAMP_STATE_LIGHTS_OFF:
      LightsOffRoutine(NewCommand);
      break;

    case LAMP_STATE_ACTIVATION:
      ActivationsRoutine(NewCommand);
      break;

    case LAMP_STATE_SHUTDOWN:
      ShutdownRoutine(NewCommand);
      break;
      
    default:
      break;
  }
}

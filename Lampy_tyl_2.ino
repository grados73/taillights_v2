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


//Control commands to receive
#define STOP 1  
#define HEADLIGHTS 5 
#define REVERSE 7
#define HAZARD 2 
#define RIGHT_INDICATOR 9
#define LEFT_INDICATOR 6
#define ACTIVATION_SYSTEM 4
#define SHUTDOWN_SYSTEM 14
#define TURN_OFF_INDICATORS 12
#define TURN_OFF_STOP 11
#define TURN_OFF_REVRESE 17
#define TURN_OF_HEADLIGHTS 15

#define ledStripPin1t 12                  //Left strip connection Pin
#define ledStripPin2t 11                  //Right strip connection Pin

#define ledCount 10                       //Number of led in each strip
#define Brightness 5                      //0 to 255
#define lengthOfReverseLight 3            //How many Led shine white during reversing
#define indicatorTimeDurationInMs 500     //How long indicators and hazard will blink


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

// Variable to hold current state of lamps
RearLampState lampState;

// Flag signalizing after get new command
bool ChangeStateFlag = false;

typedef int receivedCommand;
receivedCommand NewCommand;

String dataReceivedString = ""; //Empty string to hold received data

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

// Prototype of function to change string to int TODO!
int StoI_f(String daneString);

void clearLed12t();

// Initialize of two strips with WS18B20
Adafruit_NeoPixel strip1t(ledCount, ledStripPin1t, NEO_GRB + NEO_KHZ800); // rear left
Adafruit_NeoPixel strip2t(ledCount, ledStripPin2t, NEO_GRB + NEO_KHZ800); // rear rignt


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize variable enum which will hold current state of rear lamp
  lampState = LAMP_STATE_ACTIVATION;

  Serial.begin(115200); //uart initialization, SET SPEED = !!!115200!!!
  strip1t.begin();
  strip2t.begin();
  strip1t.setBrightness(Brightness); //set brightness
  strip2t.setBrightness(Brightness); //set brightness
  clearLed12t();
}

//
// Main program loop
//
void loop() {

}


//
// Main function to change state of lamps after get new command
//
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



void HeadlightsRoutine(receivedCommand NewCommand)
{

}

void StopRoutine(receivedCommand NewCommand)
{

}

void TurnLStopRoutine(receivedCommand NewCommand)
{

}

void TurnRStopRoutine(receivedCommand NewCommand)
{

}

void TurnLReverseRoutine(receivedCommand NewCommand)
{

}

void TurnRReverseRoutine(receivedCommand NewCommand)
{

}

void HazardLightsRoutine(receivedCommand NewCommand)
{

}

void HazardReverseRoutine(receivedCommand NewCommand)
{

}

void TurnLHeadlightsRoutine(receivedCommand NewCommand)
{

}

void TurnRHeadlightsRoutine(receivedCommand NewCommand)
{

}

void ReverseRoutine(receivedCommand NewCommand)
{

}

void StopReverseRoutine(receivedCommand NewCommand)
{

}

void HazardStopRoutine(receivedCommand NewCommand)
{

}

void LightsOffRoutine(receivedCommand NewCommand)
{

}

void ActivationsRoutine(receivedCommand NewCommand)
{

}

void ShutdownRoutine(receivedCommand NewCommand)
{

}




void clearLed12t()
{
  
}







int StoI_f(String daneString){
  int odebraneDaneInt = 0;
  if( daneString == "1") odebraneDaneInt = 1;
  else if( daneString == "2") odebraneDaneInt = 2;
  else if( daneString == "3") odebraneDaneInt = 3;
  else if( daneString == "4") odebraneDaneInt = 4;
  else if( daneString == "5") odebraneDaneInt = 5;
  else if( daneString == "6") odebraneDaneInt = 6;
  else if( daneString == "7") odebraneDaneInt = 7;
  else if( daneString == "8") odebraneDaneInt = 8;
  else if( daneString == "9") odebraneDaneInt = 9;
  else if( daneString == "10") odebraneDaneInt = 10;
  else if( daneString == "11") odebraneDaneInt = 11;
  else if( daneString == "12") odebraneDaneInt = 12;
  else if( daneString == "13") odebraneDaneInt = 13;
  else if( daneString == "14") odebraneDaneInt = 14;
  else if( daneString == "15") odebraneDaneInt = 15;
  else if( daneString == "16") odebraneDaneInt = 16;
  else if( daneString == "17") odebraneDaneInt = 17;
  else if( daneString == "18") odebraneDaneInt = 18;
  else if( daneString == "19") odebraneDaneInt = 19;
  //else odebraneDaneInt = 0;
  return odebraneDaneInt;
}
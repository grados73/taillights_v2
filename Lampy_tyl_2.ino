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

#define DEBUG_MODE 1 					// in debuging mode uC answeraing by serial about current state or received command - made to develop program
#define HIDING_INDICATOR 0				// in initialization LEDs that light up move away the strip and hide
#define NO_AUTOMATIC_DAYLIGHT 0			// after initialization headlights turn off automatically, 0 - turn headlight


//Control commands to receive
#define STOP_CMD 1  
#define HEADLIGHTS_CMD 5 
#define REVERSE_CMD 7
#define HAZARD_CMD 2 
#define RIGHT_INDICATOR_CMD 9
#define LEFT_INDICATOR_CMD 6
#define ACTIVATION_SYSTEM_CMD 4
#define SHUTDOWN_SYSTEM_CMD 14
#define TURN_OFF_INDICATORS_CMD 12
#define TURN_OFF_STOP_CMD 11
#define TURN_OFF_REVRESE_CMD 17
#define TURN_OFF_HEADLIGHTS_CMD 15

#define ledStripPin1t 12                  //Left strip connection Pin
#define ledStripPin2t 11                  //Right strip connection Pin

#define ledCount 10                       //Number of led in each strip
#define reverseCount 3					  //Number of reverse light size
#define initLedCount 3					  //Number of led in initialization animation
#define Brightness 50                     //0 to 255
#define lengthOfReverseLight 3            //How many Led shine white during reversing
#define indicatorTimeDurationInMs 500     //How long indicators and hazard will blink

#define delayInitChangeInMs 100			  //Time in [ms] after that change lights strip in initialization

// RGB color each symbol
#define RGB_STOP_R 255
#define RGB_STOP_G 0
#define RGB_STOP_B 0

#define RGB_HEADLIGHT_R 140
#define RGB_HEADLIGHT_G 0
#define RGB_HEADLIGHT_B 0

#define RGB_INDICATOR_R 255
#define RGB_INDICATOR_G 215
#define RGB_INDICATOR_B 0

#define RGB_REVERSE_R 255
#define RGB_REVERSE_G 255
#define RGB_REVERSE_B 255

#define RGB_OFF_R 0
#define RGB_OFF_G 0
#define RGB_OFF_B 0

//
// Structure of LAMPS current state
//
typedef enum
{
LAMP_STATE_HEADLIGHTS = 1,          //1
LAMP_STATE_STOP = 2,                //2
LAMP_STATE_TURN_L_STOP = 3,         //3
LAMP_STATE_TURN_R_STOP = 4,         //4
LAMP_STATE_TURN_L_REVERSE = 5,      //5
LAMP_STATE_TURN_R_REVERSE = 6,      //6
LAMP_STATE_HAZARD_LIGHTS = 7,       //7
LAMP_STATE_HAZARD_REVERSE = 8,      //8
LAMP_STATE_TURN_L_HEADLIGHTS = 9,   //9
LAMP_STATE_TURN_R_HEADLIGHTS = 10,   //10
LAMP_STATE_REVERSE = 11,             //11
LAMP_STATE_STOP_REVERSE = 12,        //12
LAMP_STATE_HAZARD_STOP = 13,         //13
LAMP_STATE_LIGHTS_OFF = 14,          //14
LAMP_STATE_ACTIVATION = 15,          //15     
LAMP_STATE_SHUTDOWN = 16             //16
} RearLampState;

// Variable to hold current state of lamps
RearLampState lampState;

// Flag signalizing after get new command
bool ChangeStateFlag = false;

bool LeftIndicatorCurrentState = false;
bool RightIndicatorCurrentState = false;

typedef int receivedCommand;
receivedCommand NewCommand;

unsigned long CurrentTimeInMs = 0; 		// Current time useing to blink turn lights etc
unsigned long TimeOfLaskBlink = 0;	

String dataReceivedString = ""; //Empty string to hold received data

void MakeCurrentLampsStateAcion(void);

// Prototype of function routine made in every change state command received
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

//Prototype of function with activity after each change to new state
void HeadlightsAcion(void);
void StopAcion(void);
void TurnLStopAcion(void);
void TurnRStopAcion(void);
void TurnLReverseAcion(void);
void TurnRReverseAcion(void);
void HazardLightsAcion(void);
void HazardReverseAcion(void);
void TurnLHeadlightsAcion(void);
void TurnRHeadlightsAcion(void);
void ReverseAcion(void);
void StopReverseAcion(void);
void HazardStopAcion(void);
void LightsOffAcion(void);
void ActivationsAcion(void);
void ShutdownAcion(void);


// Prototype of function to change string to int TODO!
int StoI_f(String daneString);

void clearLed12t();

// Initialize of two strips with WS18B20
Adafruit_NeoPixel strip1t(ledCount, ledStripPin1t, NEO_GRB + NEO_KHZ800); // rear left
Adafruit_NeoPixel strip2t(ledCount, ledStripPin2t, NEO_GRB + NEO_KHZ800); // rear rignt


void setup() 
{
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
void loop() 
{
  // if new comand received
	if(Serial.available() > 0) 
	{ 
			dataReceivedString = Serial.readStringUntil('\n'); 
			NewCommand = StoI_f(dataReceivedString); // String to int
			#if DEBUG_MODE
				Serial.print("\nNew Comand: ");
				Serial.println(NewCommand, DEC);
			#endif

			SwitchLampRegular();

			#if DEBUG_MODE
				Serial.print("New Lamp State: ");
				Serial.println(lampState);
			#endif

			ChangeStateFlag = true;

	}

	 CurrentTimeInMs = millis();

 	 MakeCurrentLampsStateAcion();

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


//
//Function to manage action to do in current state
//
void MakeCurrentLampsStateAcion(void)
{
	switch(lampState)
	{
		case LAMP_STATE_HEADLIGHTS:
		HeadlightsAcion();
		break;

		case LAMP_STATE_STOP:
		StopAcion();
		break;

		case LAMP_STATE_TURN_L_STOP:
		TurnLStopAcion();
		break;

		case LAMP_STATE_TURN_R_STOP:
		TurnRStopAcion();
		break;

		case LAMP_STATE_TURN_L_REVERSE:
		TurnLReverseAcion();
		break;

		case LAMP_STATE_TURN_R_REVERSE:
		TurnRReverseAcion();
		break;

		case LAMP_STATE_HAZARD_LIGHTS:
		HazardLightsAcion();
		break;

		case LAMP_STATE_HAZARD_REVERSE:
		HazardReverseAcion();
		break;

		case LAMP_STATE_TURN_L_HEADLIGHTS:
		TurnLHeadlightsAcion();
		break;
		
		case LAMP_STATE_TURN_R_HEADLIGHTS:
		TurnRHeadlightsAcion();
		break;
		
		case LAMP_STATE_REVERSE:
		ReverseAcion();
		break;

		case LAMP_STATE_STOP_REVERSE:
		StopReverseAcion();
		break;

		case LAMP_STATE_HAZARD_STOP:
		HazardStopAcion();
		break;

		case LAMP_STATE_LIGHTS_OFF:
		LightsOffAcion();
		break;

		case LAMP_STATE_ACTIVATION:
		ActivationsAcion();
		break;

		case LAMP_STATE_SHUTDOWN:
		ShutdownAcion();
		break;
		
		default:
		break;
	}
}


//
// Function to change state of lamps deepending on new command and current state
//
void HeadlightsRoutine(receivedCommand NewCommand) // 6 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(RIGHT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_R_HEADLIGHTS;
	else if(LEFT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_L_HEADLIGHTS;
	else if(STOP_CMD == NewCommand) lampState = LAMP_STATE_STOP;
	else if(REVERSE_CMD == NewCommand) lampState = LAMP_STATE_REVERSE;
	else if(HAZARD_CMD == NewCommand) lampState = LAMP_STATE_HAZARD_LIGHTS;
}

void StopRoutine(receivedCommand NewCommand) // 6 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(RIGHT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_R_STOP;
	else if(LEFT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_L_STOP;
	else if(HAZARD_CMD == NewCommand) lampState = LAMP_STATE_HAZARD_STOP;
	else if(REVERSE_CMD == NewCommand) lampState = LAMP_STATE_STOP_REVERSE;
	else if(TURN_OFF_STOP_CMD == NewCommand) lampState = LAMP_STATE_HEADLIGHTS;
}

void TurnLStopRoutine(receivedCommand NewCommand) // 4 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(TURN_OFF_INDICATORS_CMD == NewCommand) lampState = LAMP_STATE_STOP;
	else if(TURN_OFF_STOP_CMD == NewCommand) lampState = LAMP_STATE_TURN_L_HEADLIGHTS;
	else if(RIGHT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_R_STOP;
}

void TurnRStopRoutine(receivedCommand NewCommand) // 4 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(TURN_OFF_INDICATORS_CMD == NewCommand) lampState = LAMP_STATE_STOP;
	else if(TURN_OFF_STOP_CMD == NewCommand) lampState = LAMP_STATE_TURN_R_HEADLIGHTS;
	else if(LEFT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_L_STOP;
}

void TurnLReverseRoutine(receivedCommand NewCommand) // 4 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(TURN_OFF_REVRESE_CMD == NewCommand) lampState = LAMP_STATE_TURN_L_HEADLIGHTS;
	else if(TURN_OFF_INDICATORS_CMD == NewCommand) lampState = LAMP_STATE_REVERSE;
	else if(RIGHT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_R_REVERSE;
}

void TurnRReverseRoutine(receivedCommand NewCommand) // 4 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(TURN_OFF_REVRESE_CMD == NewCommand) lampState = LAMP_STATE_TURN_R_HEADLIGHTS;
	else if(TURN_OFF_INDICATORS_CMD == NewCommand) lampState = LAMP_STATE_REVERSE;
	else if(LEFT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_L_REVERSE;
}

void HazardLightsRoutine(receivedCommand NewCommand) // 4 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(TURN_OFF_INDICATORS_CMD == NewCommand) lampState = LAMP_STATE_HEADLIGHTS;
	else if(REVERSE_CMD == NewCommand) lampState = LAMP_STATE_HAZARD_REVERSE;
	else if(STOP_CMD == NewCommand) lampState = LAMP_STATE_HAZARD_STOP;
}

void HazardReverseRoutine(receivedCommand NewCommand) // 3 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(TURN_OFF_REVRESE_CMD == NewCommand) lampState = LAMP_STATE_HAZARD_LIGHTS;
	else if(TURN_OFF_INDICATORS_CMD == NewCommand) lampState = LAMP_STATE_REVERSE;
}

void TurnLHeadlightsRoutine(receivedCommand NewCommand) // 5 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(TURN_OFF_INDICATORS_CMD == NewCommand) lampState = LAMP_STATE_HEADLIGHTS;
	else if(STOP_CMD == NewCommand) lampState = LAMP_STATE_TURN_L_STOP;
	else if(RIGHT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_R_HEADLIGHTS;
	else if(REVERSE_CMD == NewCommand) lampState = LAMP_STATE_TURN_L_REVERSE;
}

void TurnRHeadlightsRoutine(receivedCommand NewCommand) // 5 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(TURN_OFF_INDICATORS_CMD == NewCommand) lampState = LAMP_STATE_HEADLIGHTS;
	else if(STOP_CMD == NewCommand) lampState = LAMP_STATE_TURN_R_STOP;
	else if(LEFT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_L_HEADLIGHTS;
	else if(REVERSE_CMD == NewCommand) lampState = LAMP_STATE_TURN_R_REVERSE;
}

void ReverseRoutine(receivedCommand NewCommand) // 6 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(TURN_OFF_REVRESE_CMD == NewCommand) lampState = LAMP_STATE_HEADLIGHTS;
	else if(LEFT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_L_REVERSE;
	else if(RIGHT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_R_REVERSE;
	else if(STOP_CMD == NewCommand) lampState = LAMP_STATE_STOP_REVERSE;
	else if(HAZARD_CMD == NewCommand) lampState = LAMP_STATE_HAZARD_REVERSE;
}

void StopReverseRoutine(receivedCommand NewCommand) // 5 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(TURN_OFF_REVRESE_CMD == NewCommand) lampState = LAMP_STATE_STOP;
	else if(TURN_OFF_STOP_CMD == NewCommand) lampState = LAMP_STATE_REVERSE;
	else if(RIGHT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_R_REVERSE;
	else if(LEFT_INDICATOR_CMD == NewCommand) lampState = LAMP_STATE_TURN_L_REVERSE;
}

void HazardStopRoutine(receivedCommand NewCommand) // 3 possibilities
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(TURN_OFF_STOP_CMD == NewCommand) lampState = LAMP_STATE_HAZARD_LIGHTS;
	else if(TURN_OFF_INDICATORS_CMD == NewCommand) lampState = LAMP_STATE_STOP;
	else if(REVERSE_CMD == NewCommand) lampState = LAMP_STATE_HAZARD_REVERSE;
}

void LightsOffRoutine(receivedCommand NewCommand)
{
		if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
		else if(ACTIVATION_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_ACTIVATION;
		else if(STOP_CMD == NewCommand) lampState = LAMP_STATE_STOP;
		else if(REVERSE_CMD == NewCommand) lampState = LAMP_STATE_REVERSE;

}

void ActivationsRoutine(receivedCommand NewCommand)
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
}

void ShutdownRoutine(receivedCommand NewCommand)
{
	if(TURN_OFF_HEADLIGHTS_CMD == NewCommand) lampState = LAMP_STATE_LIGHTS_OFF;
	else if(SHUTDOWN_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_SHUTDOWN;
	else if(ACTIVATION_SYSTEM_CMD == NewCommand) lampState = LAMP_STATE_ACTIVATION;
}




//
// Function with activities in every state
//
void HeadlightsAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
		Serial.println("Headlights action");
		#endif
	}
	for (int i = 0; i < ledCount; i++) 
	{
		strip1t.setPixelColor(i, strip1t.Color(RGB_HEADLIGHT_R, RGB_HEADLIGHT_G, RGB_HEADLIGHT_B));
    	strip2t.setPixelColor(i, strip2t.Color(RGB_HEADLIGHT_R, RGB_HEADLIGHT_G, RGB_HEADLIGHT_B));
  	}
  	strip1t.show();
  	strip2t.show();
}

//******************************************************//
void StopAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
		Serial.println("Stop action");
		#endif
	}
  	for (int i = 0; i < ledCount; i++) 
 	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
    	strip2t.setPixelColor(i, strip2t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
  	}
  	strip1t.show();
  	strip2t.show();
}

//******************************************************//
void TurnLStopAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
			Serial.println("Turn left Stop");
		#endif

	for (int i = 0; i < ledCount; i++) 
 	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
  	}
  	strip1t.show();
	LeftIndicatorCurrentState = true;
	TimeOfLaskBlink = CurrentTimeInMs;	
	}

	if((CurrentTimeInMs - TimeOfLaskBlink) >= indicatorTimeDurationInMs)
	{
		if(true == LeftIndicatorCurrentState) // If ON -> turn OFF
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
			}
		}

		else	// If OFF -> turn ON
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
			}
		}

		LeftIndicatorCurrentState = !LeftIndicatorCurrentState;
		strip1t.show();	
		TimeOfLaskBlink = CurrentTimeInMs;
	}
}

//******************************************************//
void TurnRStopAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
			Serial.println("Turn right Stop");
		#endif

	for (int i = 0; i < ledCount; i++) 
 	{
    	strip2t.setPixelColor(i, strip2t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
  	}
  	strip2t.show();
	RightIndicatorCurrentState = true;
	TimeOfLaskBlink = CurrentTimeInMs;	
	}

	if((CurrentTimeInMs - TimeOfLaskBlink) >= indicatorTimeDurationInMs)
	{
		if(true == RightIndicatorCurrentState) // If ON -> turn OFF
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip2t.setPixelColor(i, strip2t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
			}
		}

		else	// If OFF -> turn ON
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip2t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
			}
		}
		
		RightIndicatorCurrentState = !RightIndicatorCurrentState;
		strip2t.show();	
		TimeOfLaskBlink = CurrentTimeInMs;
	}
}

//******************************************************//
void TurnLReverseAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
			Serial.println("Turn left reverse Headlight");
		#endif

	for (int i = 0; i < reverseCount; i++) 
 	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_REVERSE_R, RGB_REVERSE_G, RGB_REVERSE_B));
  	}
	for (int i = reverseCount; i < ledCount; i++) 
 	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
  	}
  	strip1t.show();

	LeftIndicatorCurrentState = true;
	TimeOfLaskBlink = CurrentTimeInMs;	
	}

	if((CurrentTimeInMs - TimeOfLaskBlink) >= indicatorTimeDurationInMs)
	{
		if(true == LeftIndicatorCurrentState) // If ON -> turn OFF
		{
			for (int i = 0; i < reverseCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_REVERSE_R, RGB_REVERSE_G, RGB_REVERSE_B));
			}
			for (int i = reverseCount; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_HEADLIGHT_R, RGB_HEADLIGHT_G, RGB_HEADLIGHT_B));
			}
		}

		else	// If OFF -> turn ON
		{
			for (int i = 0; i < reverseCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_REVERSE_R, RGB_REVERSE_G, RGB_REVERSE_B));
			}
			for (int i = reverseCount; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
			}
		}

		LeftIndicatorCurrentState = !LeftIndicatorCurrentState;
		strip1t.show();	
		TimeOfLaskBlink = CurrentTimeInMs;
	}
}

//******************************************************//
void TurnRReverseAcion(void)
{
	TurnRHeadlightsAcion();
}

//******************************************************//
void HazardLightsAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
			Serial.println("Hazard Light actions");
		#endif

	for (int i = 0; i < ledCount; i++) 
 	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
		strip2t.setPixelColor(i, strip2t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
  	}
  	strip1t.show();
	strip2t.show();

	LeftIndicatorCurrentState = true;
	RightIndicatorCurrentState = true;
	TimeOfLaskBlink = CurrentTimeInMs;	
	}

	if((CurrentTimeInMs - TimeOfLaskBlink) >= indicatorTimeDurationInMs)
	{
		if(true == LeftIndicatorCurrentState) // If ON -> turn OFF
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_HEADLIGHT_R, RGB_HEADLIGHT_G, RGB_HEADLIGHT_B));
				strip2t.setPixelColor(i, strip2t.Color(RGB_HEADLIGHT_R, RGB_HEADLIGHT_G, RGB_HEADLIGHT_B));
			}
		}

		else	// If OFF -> turn ON
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
				strip2t.setPixelColor(i, strip2t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
			}
		}

		LeftIndicatorCurrentState = !LeftIndicatorCurrentState;
		RightIndicatorCurrentState = !RightIndicatorCurrentState;
		strip1t.show();	
		strip2t.show();	
		TimeOfLaskBlink = CurrentTimeInMs;
	}

}

//******************************************************//
void HazardReverseAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
			Serial.println("Hazard Reverse Action");
		#endif

	for (int i = 0; i < reverseCount; i++) 
 	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_REVERSE_R, RGB_REVERSE_G, RGB_REVERSE_B));
  	}
	for (int i = reverseCount; i < ledCount; i++) 
 	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
  	}
	  	for (int i = 0; i < ledCount; i++) 
 	{
    	strip2t.setPixelColor(i, strip2t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
  	}
  	strip1t.show();
	strip2t.show();

	LeftIndicatorCurrentState = true;
	RightIndicatorCurrentState = true;
	TimeOfLaskBlink = CurrentTimeInMs;	
	}

	if((CurrentTimeInMs - TimeOfLaskBlink) >= indicatorTimeDurationInMs)
	{
		if(true == LeftIndicatorCurrentState) // If ON -> turn OFF
		{
			for (int i = 0; i < reverseCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_REVERSE_R, RGB_REVERSE_G, RGB_REVERSE_B));
			}
			for (int i = reverseCount; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_HEADLIGHT_R, RGB_HEADLIGHT_G, RGB_HEADLIGHT_B));
			}

			for (int i = 0; i < ledCount; i++) 
			{
				strip2t.setPixelColor(i, strip2t.Color(RGB_HEADLIGHT_R, RGB_HEADLIGHT_G, RGB_HEADLIGHT_B));
			}
		}

		else	// If OFF -> turn ON
		{
			for (int i = 0; i < reverseCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_REVERSE_R, RGB_REVERSE_G, RGB_REVERSE_B));
			}
			for (int i = reverseCount; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
			}
			for (int i = 0; i < ledCount; i++) 
			{
				strip2t.setPixelColor(i, strip2t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
			}
		}

		LeftIndicatorCurrentState = !LeftIndicatorCurrentState;
		RightIndicatorCurrentState = !RightIndicatorCurrentState;
		strip1t.show();	
		strip2t.show();
		TimeOfLaskBlink = CurrentTimeInMs;
	}

}

//******************************************************//
void TurnLHeadlightsAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
			Serial.println("Turn left Headlight");
		#endif

	for (int i = 0; i < ledCount; i++) 
 	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
  	}
  	strip1t.show();
	LeftIndicatorCurrentState = true;
	TimeOfLaskBlink = CurrentTimeInMs;	
	}

	if((CurrentTimeInMs - TimeOfLaskBlink) >= indicatorTimeDurationInMs)
	{
		if(true == LeftIndicatorCurrentState) // If ON -> turn OFF
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_HEADLIGHT_R, RGB_HEADLIGHT_G, RGB_HEADLIGHT_B));
			}
		}

		else	// If OFF -> turn ON
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
			}
		}

		LeftIndicatorCurrentState = !LeftIndicatorCurrentState;
		strip1t.show();	
		TimeOfLaskBlink = CurrentTimeInMs;
	}

}

//******************************************************//
void TurnRHeadlightsAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
			Serial.println("Turn right headlight");
		#endif

	for (int i = 0; i < ledCount; i++) 
 	{
    	strip2t.setPixelColor(i, strip2t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
  	}
  	strip2t.show();
	RightIndicatorCurrentState = true;
	TimeOfLaskBlink = CurrentTimeInMs;	
	}

	if((CurrentTimeInMs - TimeOfLaskBlink) >= indicatorTimeDurationInMs)
	{
		if(true == RightIndicatorCurrentState) // If ON -> turn OFF
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip2t.setPixelColor(i, strip2t.Color(RGB_HEADLIGHT_R, RGB_HEADLIGHT_G, RGB_HEADLIGHT_B));
			}
		}

		else	// If OFF -> turn ON
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip2t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
			}
		}
		
		RightIndicatorCurrentState = !RightIndicatorCurrentState;
		strip2t.show();	
		TimeOfLaskBlink = CurrentTimeInMs;
	}

}

//******************************************************//
void ReverseAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
		Serial.println("Reverse action");
		#endif
	}

	for (int i = 0; i < ledCount; i++) 
	{
		strip1t.setPixelColor(i, strip1t.Color(RGB_HEADLIGHT_R, RGB_HEADLIGHT_G, RGB_HEADLIGHT_B));
    	strip2t.setPixelColor(i, strip2t.Color(RGB_HEADLIGHT_R, RGB_HEADLIGHT_G, RGB_HEADLIGHT_B));
	}
	for (int i = 0; i < reverseCount; i++) 
	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_REVERSE_R, RGB_REVERSE_G, RGB_REVERSE_B));
	}
  	strip1t.show();
  	strip2t.show();
}

//******************************************************//
void StopReverseAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
		Serial.println("Stop Reverse action");
		#endif
	}

	// STOP
	for (int i = 0; i < ledCount; i++) 
 	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
    	strip2t.setPixelColor(i, strip2t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
  	}
	// REVERSE
	for (int i = 0; i < reverseCount; i++) 
	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_REVERSE_R, RGB_REVERSE_G, RGB_REVERSE_B));
	}
  	strip1t.show();
  	strip2t.show();

}

//******************************************************//
void HazardStopAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
			Serial.println("Hazard Light actions");
		#endif

	for (int i = 0; i < ledCount; i++) 
 	{
    	strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
		strip2t.setPixelColor(i, strip2t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
  	}
  	strip1t.show();
	strip2t.show();

	LeftIndicatorCurrentState = true;
	RightIndicatorCurrentState = true;
	TimeOfLaskBlink = CurrentTimeInMs;	
	}

	if((CurrentTimeInMs - TimeOfLaskBlink) >= indicatorTimeDurationInMs)
	{
		if(true == LeftIndicatorCurrentState) // If ON -> turn OFF
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
				strip2t.setPixelColor(i, strip2t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
			}
		}

		else	// If OFF -> turn ON
		{
			for (int i = 0; i < ledCount; i++) 
			{
				strip1t.setPixelColor(i, strip1t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
				strip2t.setPixelColor(i, strip2t.Color(RGB_INDICATOR_R, RGB_INDICATOR_G, RGB_INDICATOR_B));
			}
		}

		LeftIndicatorCurrentState = !LeftIndicatorCurrentState;
		RightIndicatorCurrentState = !RightIndicatorCurrentState;
		strip1t.show();	
		strip2t.show();	
		TimeOfLaskBlink = CurrentTimeInMs;
	}
}

//******************************************************//
void LightsOffAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
		Serial.println("Lights off action");
		#endif
	}

	clearLed12t();

}

//******************************************************//
void ActivationsAcion(void)
{
	if(ChangeStateFlag)
	{
		ChangeStateFlag = false;
		#if DEBUG_MODE
		Serial.println("Lights init action");
		#endif
	}

	//// LEDS MOVE UP
    for( int i = 0; i < ledCount; i++)
	{  
    	strip1t.setPixelColor(i, strip1t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
       	strip2t.setPixelColor(i, strip2t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
       	delay(delayInitChangeInMs);
     
         	if( i >= initLedCount) //turning of led from start
			{ 
          		strip1t.setPixelColor(i-initLedCount, strip1t.Color(RGB_OFF_R, RGB_OFF_G, RGB_OFF_B));
          		strip2t.setPixelColor(i-initLedCount, strip2t.Color(RGB_OFF_R, RGB_OFF_G, RGB_OFF_B));
         	}
       		strip1t.show();
       		strip2t.show();
    
	} 
	/// if should hide all
	#if HIDING_INDICATOR
        for(int k = (ledCount-initLedCount); k <= ledCount ; k++)
		{  
        	strip1.setPixelColor(k, strip1.Color(RGB_OFF_R, RGB_OFF_G, RGB_OFF_B));
          	strip2.setPixelColor(k, strip2.Color(RGB_OFF_R, RGB_OFF_G, RGB_OFF_B));
          	strip1.show();
          	strip2.show();
          	delay(delayInitChangeInMs);   
        }
  	#endif
  
	// LEDS MOVE DOWN
    for( int i = ledCount-(initLedCount-1); i >= 0; i--) 
	{  
       	strip1t.setPixelColor(i, strip1t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
       	strip2t.setPixelColor(i, strip2t.Color(RGB_STOP_R, RGB_STOP_G, RGB_STOP_B));
       	delay(delayInitChangeInMs);
  
       	if( i< ledCount-initLedCount)
	   	{ 
       	   	strip1t.setPixelColor(i+initLedCount+2, strip1t.Color(RGB_OFF_R, RGB_OFF_G, RGB_OFF_B));
          	strip2t.setPixelColor(i+initLedCount+2, strip2t.Color(RGB_OFF_R, RGB_OFF_G, RGB_OFF_B));
        }
       strip1t.show();
       strip2t.show();

       } 

	#if NO_AUTOMATIC_DAYLIGHT
        for(int k = initLedCount+1; k >= 0 ; k--) 
		{ 
                  strip1t.setPixelColor(k, strip1t.Color(RGB_OFF_R, RGB_OFF_G, RGB_OFF_B));
                  strip2t.setPixelColor(k, strip2t.Color(RGB_OFF_R, RGB_OFF_G, RGB_OFF_B));
                  strip1t.show();
                  strip2t.show();
                  delay(delayInitChangeInMs);   
        }
		lampState = LAMP_STATE_SHUTDOWN;
	#else 
		lampState = LAMP_STATE_HEADLIGHTS;	  
	#endif

	#if DEBUG_MODE
		Serial.print("Curren state: ");
		Serial.println(lampState);
	#endif

	MakeCurrentLampsStateAcion();	
}

//******************************************************//
void ShutdownAcion(void)
{
	for( int i = (0+initLedCount); i < ledCount; i++)
	{  // indicators move up ^
         strip1t.setPixelColor(i, strip1t.Color(255, 0, 0));
         strip2t.setPixelColor(i, strip2t.Color(255, 0, 0));
         delay(delayInitChangeInMs);
       
             if( i >= initLedCount){ //turning off indicators from begining
              strip1t.setPixelColor(i-initLedCount, strip1t.Color(0, 0, 0));
              strip2t.setPixelColor(i-initLedCount, strip2t.Color(0, 0, 0));
             }
         strip1t.show();
         strip2t.show();
     }
     
     for( int i = ledCount-initLedCount; i >= 0; i--){  // indicators move down
           strip1t.setPixelColor(i, strip1t.Color(255, 0, 0));
           strip2t.setPixelColor(i, strip2t.Color(255, 0, 0));
           delay(delayInitChangeInMs);
         
               if( i< ledCount-initLedCount){ //turning off indicators from begining
                  strip1t.setPixelColor(i+initLedCount, strip1t.Color(0, 0, 0));
                  strip2t.setPixelColor(i+initLedCount, strip2t.Color(0, 0, 0));
               }
           strip1t.show();
           strip2t.show();
           }
              for(int k = initLedCount; k >= 0 ; k--){  //hiding indicators
                    strip1t.setPixelColor(k, strip1t.Color(0, 0, 0));
                    strip2t.setPixelColor(k, strip2t.Color(0, 0, 0));
                    strip1t.show();
                    strip2t.show();
                    delay(delayInitChangeInMs);   
              }
	lampState = LAMP_STATE_LIGHTS_OFF;
	#if DEBUG_MODE
		Serial.print("Curren state: ");
		Serial.println(lampState);
	#endif

	MakeCurrentLampsStateAcion();	
}


//Set both strip OFF
void clearLed12t()
{
	for (int i = 0; i < ledCount; i++) 
	{
		strip1t.setPixelColor(i, 0, 0, 0);
		strip2t.setPixelColor(i, 0, 0, 0);
	} 
		strip1t.show();
		strip2t.show();
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

  return odebraneDaneInt;
}
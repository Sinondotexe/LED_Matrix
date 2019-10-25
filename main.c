/***************************************
 @autor Lionardo Wernli
 @name LedMatrix_Rev.1.c
 @bugs GUI Touch funktioniert nicht mehr
 @todo 
***************************************/

//---------------------------------------------------------------------
//Includes
//---------------------------------------------------------------------

 #include "TouchP0P1.h"

//---------------------------------------------------------------------
//Defines
//---------------------------------------------------------------------

 #define btn_debug !PC_13 //Schaltet Debug_mode an

 //---------------------------------------------------------------------
 //Funktionsprototypen
 //---------------------------------------------------------------------

 void Matrix_Clock(void);
 void Matrix_LEDTest(void);
 void Matrix_Initialize_Pins(void);
 void Matrix_Matrix_Control(void);
 void Matrix_Debug_Mode(void);
 void Matrix_GUI_Touch(void);
 void Matrix_GUI_Touch_Control(void);
 void Matrix_GUI_DrawClockAnalog(void);
 void Matrix_GUI_DrawClockDigital(void);
 void Matrix_GUI_DrawBtn(void);
 void Matrix_Gui_Touch_Countdown(void);

//---------------------------------------------------------------------
//Globale Variabeln
//---------------------------------------------------------------------

 int config_debugging = 0; //ob im Debug Modus
 int config_LedTest = 0;   //Überschreibt programm mit LED test
 int MiliSeconds, Seconds, Minutes, Hours; //Rechenvariablen Zeit    ***HIER mit wird UHR gesetz***
 int state_btn_startstop = 0, state_btn_startstop_old = 0;
 int state_btn_reset = 0, state_btn_reset_old = 0;
 int state_btn_clock = 2, state_btn_clock_old = 0;
 int state_btn_countdown = 1, state_btn_countdown_old = 0;
 int state_btn_stopwatch = 1, state_btn_stopwatch_old = 0;
 int clocktype_active = 0, clocktype_old = 0;
 int oldTouch = 0, isTouched = 0;
 int btn_pressed;


//*********************************************************************

int main (void)
{
	Matrix_Initialize_Pins();
	InitTouchScreen(); //Display Initialisieren
	while(1) //Endlos Betrieb
	{
		if(config_LedTest == 1)
		{
			Matrix_LEDTest();
		}
		else
		{
			if(btn_debug)
			{
				config_debugging = 1; //schaltet Debugmode an			
			}
			Matrix_GUI_DrawBtn();
			Matrix_GUI_Touch();
			if(config_debugging == 1) //Debug Modus
			{
				Matrix_Debug_Mode();	
			}
			Matrix_GUI_Touch_Control();
			Matrix_Clock();
			//Matrix_Control();
		}
	}
}

void Matrix_Clock()
{
	char TimerOverrun = TimerGetFlag(5);
	int TimerValue = TimerGetTime(5);

	if(TimerOverrun == 1)
	{
		Seconds++;
		if(Seconds == 60)
		{
			Seconds = 0;
			Minutes++;			
		}
		if(Minutes == 60)
		{
			Minutes = 0;
			Hours++;			
		}
		if(Hours == 24)
		{
			Hours = 0;
		}		
	}
	if(TimerValue == 0)
	{
      //TimerInit(5,10,0); //1ms
		TimerInit(5,10000,0); //1sek
	}	
}

void Matrix_Stopwatch_Timer() //Stopuhr mit 10ms genauigkeit
{
	char TimerOverrun = TimerGetFlag(5);
	int TimerValue = TimerGetTime(5);

	if(TimerOverrun == 1)
	{
		MiliSeconds += 10;
		if((MiliSeconds%1000) == 0)
		{
			MiliSeconds = 0;
			Seconds++;			
		}
		if((Seconds%60) == 0)
		{
			Seconds = 0;
			Minutes++;			
		}		
	}
	if(TimerValue == 0)
	{
		TimerInit(5,100,0); //10ms
	}
}

void Matrix_Countdown_Timer(void);

void Matrix_LEDTest()
{
	while(1)
	{
	}
}

void Matrix_Initialize_Pins(void)
{
	GPIOInit("PEH",00000000);
	GPIOE->CRH   &= 0x00000000;                  // Konfiguriere GPIOE für
	GPIOE->CRH   |= 0x22222222;                  // General purpose output push-pull, 2MHz
	GPIOPutByte("PEH",0xAA);
}

void Matrix_Debug_Mode()
{
	rectan(0, 0, 100, 0, 1, BRIGHT_BLUE, 1); //X Achse
	rectan(0, 0, 0, 100, 1, BRIGHT_YELLOW, 1); //Y Achse
	printAt(8, "TSC :");   


	textxy("Debugmode On", 0, 100, BRIGHT_RED, BLACK); //Text DebugModus

	switch(btn_pressed) //last button pressed
	{
		case 1: //Clock Pressed
			textxy("clock is pressed    ", 0, 122, BRIGHT_CYAN, BLACK);
		break;

		case 2: //Stopwatch Pressed
			textxy("Stopwatch is pressed", 0, 122, BRIGHT_CYAN, BLACK);
		break;

		case 3: //Countdown Pressed
			textxy("Countdown is pressed", 0, 122, BRIGHT_CYAN, BLACK);
		break;

		case 4: //Reset Pressed
			textxy("Reset is pressed    ", 0, 122, BRIGHT_CYAN, BLACK);
		break;

		case 5: //StartStop Pressed
			textxy("Startstop is pressed", 0, 122, BRIGHT_CYAN, BLACK);
		break;
	}
		if(getTSCx() <= 320)      
         {
           printDec(5, getTSCx());        //schreibe x aus (zum test) 
   
         }     // grenze Bereich für Rückgabewerte ein und gib sie aus.
			if(getTSCy() <= 320)
         {
           printDec(5, getTSCy());          //schreibe y aus, (zum test)
         }
         printAt(9, "");
         printDec(2, Hours);
         printDec(3, Minutes);
         printDec(3, Seconds);
}

void Matrix_GUI_Touch(void)	//Touch erkennung
{
	GPIOPutByte("PEH",getTSCtouched());
		isTouched = getTSCtouched();	//wenn berührt
	if(isTouched && !oldTouch)	//
	{
		oldTouch = 1;	//Touchdown
		getTSCxy();	//get Cursor X Y position
		unsigned int touch_xPos = getTSCx(); //save X Position
		unsigned int touch_yPos = getTSCy(); //save Y Position

		//btn Clock Pressed
		if((touch_xPos > 0) && (touch_yPos > 0) && (touch_xPos < 71) && (touch_yPos < 20))
		{
			btn_pressed = 1; 
		}

		//btn Stopwatch Pressed
		if((touch_xPos > 80) && (touch_yPos > 0) && (touch_xPos < 154) && (touch_yPos < 20))
		{
			btn_pressed = 2;
		}

		//btn Countdown Pressed
		if((touch_xPos > 163) && (touch_yPos > 0) && (touch_xPos < 240) && (touch_yPos < 20))
		{
			btn_pressed = 3;
		}

		//btn Reset Pressed	
		if((touch_xPos > 0) && (touch_yPos > 300) && (touch_xPos < 117) && (touch_yPos < 320))
		{
			btn_pressed = 4;	
		}

		//btn StartStop Pressed	
		if((touch_xPos > 123) && (touch_yPos > 300) && (touch_xPos < 240) && (touch_yPos < 320))
		{
			btn_pressed = 5;
		}
		touch_xPos = 0; //reset Last Pressed X
		touch_yPos = 0; //reset Last Pressed Y	
	}
   if(!isTouched && oldTouch)
	{
		oldTouch = 0; //Reset Pressed			
	}
}

void Matrix_GUI_Touch_Control() //Auswertung Touch
{
	switch(btn_pressed)
	{
		case 1: //clock pressed
			state_btn_reset = 2;
			state_btn_startstop = 3;

			if(state_btn_stopwatch_old > 1)
			{
				state_btn_stopwatch = 1;				
			}
			if(state_btn_countdown_old > 1)
			{
				state_btn_countdown = 1;				
			}

			if((state_btn_clock_old == 1) || (state_btn_clock_old == 3))
			{
				state_btn_clock = 2;  //set Analog Mode
				clocktype_active = 1;	
			}
			if(state_btn_clock_old == 2)
			{
				state_btn_clock = 3; //set Digital Mode
				clocktype_active = 2;
			}
		break;

		case 2: //Stopwatch pressed
			if(state_btn_stopwatch_old == 1)
			{
				state_btn_stopwatch = 2;
				state_btn_reset = 1;
				state_btn_startstop = 1;
				clocktype_active = 3;
				if(state_btn_clock_old > 1)
				{
					state_btn_clock = 1;					
				}
				if(state_btn_countdown_old > 1)
				{
					state_btn_countdown = 1;					
				}
			}
		break;

		case 3: //Countdown pressed
			if(state_btn_countdown_old == 1)
			{
				state_btn_countdown = 2;
				state_btn_reset = 1;
				state_btn_startstop = 1;
				clocktype_active = 4;
				if(state_btn_clock_old > 1)
				{
					state_btn_clock = 1;					
				}
				if(state_btn_stopwatch_old > 1)
				{
					state_btn_stopwatch = 1;
				}				
			}
		break;

		case 4: //Reset pressed
			//Reset Timer Variable
		break;

		case 5: //StartStop pressed
			if(state_btn_startstop_old == 1)
			{
				state_btn_startstop = 2;
			}
			if(state_btn_startstop_old == 2)
			{
				state_btn_startstop = 1;
			}
		break;		
	}
	btn_pressed = 0;

}
void Matrix_GUI_DrawClockAnalog()
{

}

void Matrix_GUI_DrawClockDigital()
{
	switch(clocktype_active)
	{
		case 2: //Digital Clock     Timer
			break;
		case 3: //Digital Stopwatch Timer
			break;
		case 4: //Digital Countdown Timer + Touchfelder
			break;
	}	
}

void Matrix_GUI_DrawBtn()
{
	if(state_btn_clock == 1) //set not Clockmode
	{
		rectan(0 , 0, 71, 20, 1, BRIGHT_YELLOW, 1); //Yellor Rectan
		textxy("Clock", 16 ,15 , BLACK, BRIGHT_YELLOW);	//Text Clock
		state_btn_clock = 1; //renew btn state
		state_btn_clock = 0; //dont redraw
	}
	if(state_btn_clock == 2) //set Clock Analog Mode
	{
		rectan(0, 0, 71, 20, 1, BRIGHT_GREEN, 1); //Green Rectan
		textxy("Analog", 13, 15, BLACK, BRIGHT_GREEN); //Text Analog
		state_btn_clock_old = 2; //renew btn state
		state_btn_clock = 0; //dont redraw
	}
	if(state_btn_clock == 3) //set Clock Digital Mode
	{
		rectan(0, 0, 71, 20, 1, BRIGHT_RED, 1); //RED Rectan
		textxy("Digital", 9 ,15 , BLACK, BRIGHT_RED); //Text Digital
		state_btn_clock_old = 3; //renew btn state
		state_btn_clock = 0; //dont redraw
	}
	if(state_btn_stopwatch == 1) //set Stopwatch Inactiv
	{
		rectan(80 , 0, 154, 20, 1, BRIGHT_YELLOW, 1); //Yellow Rectan
     	textxy("Stopwatch", 82 ,15 , BLACK, BRIGHT_YELLOW); //Text Stopwatch
     	state_btn_stopwatch_old = 1; //renew btn state
     	state_btn_stopwatch = 0; //dont redraw

	}
	if(state_btn_stopwatch == 2) //set Stopwatch Activ
	{
		rectan(80 , 0, 154, 20, 1, BRIGHT_GREEN, 1); //Green Rectan
   	    textxy("Stopwatch", 82 ,15 , BLACK, BRIGHT_GREEN); //Text Stopwatch
   	    state_btn_stopwatch_old = 2; //renew btn state
   	    state_btn_stopwatch = 0; //dont redraw

	}
	if(state_btn_countdown == 1) //Countdown Inactiv
	{
		rectan(163 , 0, 240, 20, 1, BRIGHT_YELLOW, 1); //Yellow Rectan
     	textxy("Countdown", 166 ,15 , BLACK, BRIGHT_YELLOW); //Text Countdown
     	state_btn_countdown_old = 1; //renew btn state
   	    state_btn_countdown = 0; //dont redraw

	}
	if(state_btn_countdown == 2) //Countdown Activ
	{
		rectan(163 , 0, 240, 20, 1, BRIGHT_GREEN, 1); //Green Rectan
     	textxy("Countdown", 166 ,15 , BLACK, BRIGHT_GREEN); //Text Countdown
     	state_btn_countdown_old = 2; //renew btn state
   	    state_btn_countdown = 0; //dont redraw

	}
	if(state_btn_countdown == 3) //Countdown Timed Out
	{
		rectan(163 , 0, 240, 20, 1, BRIGHT_RED, 1); //Red Rectan
     	textxy("Countdown", 166 ,15 , BLACK, BRIGHT_RED); //Text Countdown
     	state_btn_countdown_old = 3; //renew btn state
     	state_btn_countdown = 0; //dont redraw
	}
	if(state_btn_reset == 1) //Reset Drawn
	{
		rectan(0, 300, 117, 320, 1, BRIGHT_YELLOW, 1); //Yellow Rectan
		textxy("Reset", 40 ,315 , BLACK, BRIGHT_YELLOW); //Text Reset
		state_btn_reset_old = 1; //
		state_btn_reset = 0;
	}
	if(state_btn_reset == 2) //Reset Hidden
	{
		rectan(0, 300, 117, 320, 1, BLACK, 1);
		state_btn_reset_old = 2;
		state_btn_reset = 0;
	}
	if(state_btn_startstop == 1) //Start
	{
		rectan(123, 300, 240, 320, 1, BRIGHT_GREEN, 1);
		textxy("Start", 165 ,315 , BLACK, BRIGHT_GREEN);
		state_btn_startstop_old = 1;
		state_btn_startstop = 0;
	}
	if(state_btn_startstop == 2) //Stop
	{
		rectan(123, 300, 240, 320, 1, BRIGHT_RED, 1);
		textxy("Stop", 165 ,315 , BLACK, BRIGHT_RED);
		state_btn_startstop_old = 2;
		state_btn_startstop = 0;
	}
	if(state_btn_startstop == 3) //StartStop Hidden
	{
		rectan(123, 300, 240, 320, 1, BLACK, 1);
		state_btn_startstop_old = 3;
		state_btn_startstop = 0;
	}
}

//liest stellenänderung Zeit
void Matrix_GUI_Touch_Countdown(void);


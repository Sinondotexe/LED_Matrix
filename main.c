/***
 @autor Lionardo Wernli
 @name LedMatrix_Rev.1.c
 ***/
 //TODO
 /*
 GUI Analog, Digital Uhr, Touchbare Einstellbare Stopuhr settings
 DEBUG MODE, eingestellte zeit anzeigen
 LED TESTING erstellen
 MATRIX CONTROLL erstellen
*/


 //---------------------------------------------------------------------
 //Includes
 //---------------------------------------------------------------------
 #include "TouchP0P1.h"
 #include "Matrix_GUI.h" 
 #include "Matrix_Debug.h"

 //---------------------------------------------------------------------
 //Defines
 //---------------------------------------------------------------------
 #define btn_debug !PC_13 //Schaltet Debug_mode an
 int config_debugging = 0; //Ob Im Debugmode
 int config_LedTest = 0; //Überschreibt programm mit LED test
 int Clock_Seconds, Clock_Minutes, Clock_Hours;
 int StopWatch_Seconds, StopWatch_Minutes, StopWatch_MiliSeconds;
 //---------------------------------------------------------------------
 //Funktionsprototypen
 //---------------------------------------------------------------------
void Matrix_Clock(void);
void Matrix_Control(void);

int main (void)
{
	InitTouchScreen(); //Display Initialisieren
	while(1) //Endlos Betrieb
	{
		if(config_LedTest == 1)
		{
			//Matrix_LEDTest();
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
				//Matrix_Debug_Menu(Matrix_GUI_Touch());	
			}
			Matrix_GUI_Touch_Control();
			Matrix_Clock();
			// Matrix_Control();
		}
	}
}
//der echte Timer
void Matrix_Clock()
{
	char Clock_TimerOverrun = TimerGetFlag(5);
	int Clock_TimerValue = TimerGetTime(5);
	if(Clock_TimerOverrun == 1)
	{
		Clock_Seconds++;
		if((Clock_Seconds%60) == 0)
		{
			Clock_Seconds = 0;
			Clock_Minutes++;			
		}
		if((Clock_Minutes%60) == 0)
		{
			Clock_Minutes = 0;
			Clock_Hours++;			
		}
		if((Clock_Hours%24) == 0)
		{
			Clock_Hours = 0;
		}		
	}
	if(Clock_TimerValue == 0)
	{
		TimerInit(5,10000,0); //1sek
	}	
}

void Matrix_Stopwatch_Timer() //Stopuhr mit 10ms genauigkeit
{
	char StopWatch_TimerOverrun = TimerGetFlag(6);
	int StopWatch_TimerValue = TimerGetTime(6);

	if(StopWatch_TimerOverrun == 1)
	{
		StopWatch_MiliSeconds += 10;
		if((StopWatch_MiliSeconds%1000) == 0)
		{
			StopWatch_MiliSeconds = 0;
			StopWatch_Seconds++;			
		}
		if((StopWatch_Seconds%60) == 0)
		{
			StopWatch_Seconds = 0;
			StopWatch_Minutes++;			
		}		
	}
	if(StopWatch_TimerValue == 0)
	{
		TimerInit(6,100,0); //10ms
	}
}

void Matrix_Countdown_Timer(void);


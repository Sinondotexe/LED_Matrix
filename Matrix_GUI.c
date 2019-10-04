#include "TouchP0P1.h"
int state_btn_startstop = 0, state_btn_startstop_old = 0;
int state_btn_reset = 0, state_btn_reset_old = 0;
int state_btn_clock = 0, state_btn_clock_old = 0;
int state_btn_countdown = 0, state_btn_countdown_old = 0;
int state_btn_stopwatch = 0, state_btn_stopwatch_old = 0;
int clocktype_active = 0, clocktype_old = 0;
int btn_pressed = 0;


int Matrix_GUI_Touch()
{
	int isTouched = getTSCtouched();
	int oldTouch = 0;
	if(isTouched && !oldTouch)
	{
		oldTouch = 1; //Touchdown
		getTSCxy();
		unsigned int touch_xPos = getTSCx();
		unsigned int touch_yPos = getTSCy();

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

		if(!isTouched && oldTouch)
		{
			oldTouch = 0; //Reset Pressed			
		}
	}
	return btn_pressed;		
}

void Matrix_GUI_Touch_Control()
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
				if(state_btn_startstop_old > 1)
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
		rectan(80 , 0, 154, 20, 1, BRIGHT_YELLOW, 1);
     	textxy("Stopwatch", 82 ,15 , BLACK, BRIGHT_YELLOW);
     	state_btn_stopwatch_old = 1;
     	state_btn_stopwatch = 0;

	}
	if(state_btn_stopwatch == 2) //set Stopwatch Activ
	{
		rectan(80 , 0, 154, 20, 1, BRIGHT_GREEN, 1);
   	    textxy("Stopwatch", 82 ,15 , BLACK, BRIGHT_GREEN);
   	    state_btn_stopwatch_old = 2;
   	    state_btn_stopwatch = 0;

	}
	if(state_btn_countdown == 1) //Countdown Inactiv
	{
		rectan(163 , 0, 240, 20, 1, BRIGHT_YELLOW, 1);
     	textxy("Countdown", 166 ,15 , BLACK, BRIGHT_YELLOW);
     	state_btn_countdown_old = 1;
   	    state_btn_countdown = 0;

	}
	if(state_btn_countdown == 2) //Countdown Activ
	{
		rectan(163 , 0, 240, 20, 1, BRIGHT_GREEN, 1);
     	textxy("Countdown", 166 ,15 , BLACK, BRIGHT_GREEN);
     	state_btn_countdown_old = 2;
   	    state_btn_countdown = 0;

	}
	if(state_btn_countdown == 3) //Countdown Timed Out
	{
		rectan(163 , 0, 240, 20, 1, BRIGHT_RED, 1);
     	textxy("Countdown", 166 ,15 , BLACK, BRIGHT_RED);
     	state_btn_countdown_old = 3;
     	state_btn_countdown = 0;
	}
	if(state_btn_reset == 1) //Reset Drawn
	{
		rectan(0, 300, 117, 320, 1, BRIGHT_YELLOW, 1);
		textxy("Reset", 40 ,315 , BLACK, BRIGHT_YELLOW);
		state_btn_reset_old = 1;
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




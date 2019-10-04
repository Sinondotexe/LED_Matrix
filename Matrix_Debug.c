#include "TouchP0P1.h"

void Matrix_Debug_Menu(int btn_pressed)
{
	rectan(0, 0, 100, 0, 1, BRIGHT_BLUE, 1); //X Achse
	rectan(0, 0, 0, 100, 1, BRIGHT_YELLOW, 1); //Y Achse

	textxy("Debugmode On", 0, 100, BRIGHT_RED, BLACK); //Text DebugModus

	switch(btn_pressed)
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

}
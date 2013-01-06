#include "console.h"

#ifndef LINUX
	using namespace std;

	void gotoXY(int x, int y)
	{
			//Initialize the coordinates
			COORD coord = {x, y};
			//Set the position
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			return;
	}

	COORD getXY()
	{ 
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		COORD coord = {0, 0};
		if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
			coord = csbi.dwCursorPosition;
		return coord;
	}

	void Green(bool highlite) {
		if (highlite) 
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
		else
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
	}
	void Red(bool highlite) {
		if (highlite) 
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
		else
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED);
	}
	void White(bool highlite) {
		if (highlite) 
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
		else
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
	}
#endif


#ifdef LINUX
	void gotoXY(int x, int y) {}

	COORD getXY()
	{
		return 0;
	}

	void Green(bool highlite = false) {
	}

	void Red(bool highlite = false) {
	}

	void White(bool highlite = false) {
	}
#endif

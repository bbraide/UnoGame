#define NOMINMAX
#define WIN32_LEAN_AND_MEAN  
#include <windows.h>
#include "Game.h"


int main() {
	// enable ANSI color codes on Windows
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

	Game game("uno_deck.txt");
	game.WelcomeMenu();
	return 0;
}
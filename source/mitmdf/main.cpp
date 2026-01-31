#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {

	initscr();

	nl();
	noecho();
	curs_set(0);
	timeout(0);
	keypad(stdscr, 1);

	addstr("Hello world!");
	getch();

	nocbreak();
	keypad(stdscr, 0);
	echo();
	endwin();

	return 0;
} 
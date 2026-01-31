#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#include "texts.h"
#include "game.h"

int main(int argc, char* argv[]) {
	char inp[60];

	initscr();
	resize_term(60, 134);
	start_color();
    scrollok(stdscr, true);

	PDC_set_title("Murder In The Masked Dancefloor");

	nl();
	noecho();
	curs_set(0);
	timeout(0);
	keypad(stdscr, true);

	addstr(mitmdf::S_TITLE);
	getnstr(inp, 59);

	

	nocbreak();
	keypad(stdscr, false);
	echo();
	endwin();

	return 0;
} 
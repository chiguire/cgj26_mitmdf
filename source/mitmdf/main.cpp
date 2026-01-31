#include <curses.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {

	initscr();

	nl();
	noecho();
	curs_set(0);
	timeout(0);
	keypad(stdscr, true);

	addstr("Hello world!");

	nocbreak();
	keypad(stdscr, false);
	echo();
	endwin();
} 
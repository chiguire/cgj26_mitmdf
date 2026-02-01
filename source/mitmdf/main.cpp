#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "game.h"

#define BUFFER_SIZE 60
char text_buffer[BUFFER_SIZE];

int read_option() {
	addstr(mitmdf::S_PROMPT.c_str());
	getnstr(text_buffer, BUFFER_SIZE - 1);

	try {
		return std::stoi(std::string(text_buffer));
	} catch (const std::invalid_argument&) {
		return -1; // Invalid input
	} catch (const std::out_of_range&) {
		return -1; // Out of range
	}
}

void playGame(mitmdf::ProgramState& programState) {
	mitmdf::GameState& gameState = programState.GetGameState();
	switch (gameState.GetCurrentState()) {
		case mitmdf::GameStateEnum::INTRO: {
			
			break;
		}
		case mitmdf::GameStateEnum::QUESTION:
			//addstr(mitmdf::S_QUESTION.c_str());
			gameState.SwitchToState(mitmdf::GameStateEnum::ANSWER);
			break;
	}
}

int main(int argc, char* argv[]) {
	mitmdf::ProgramState programState(std::time(nullptr));

	initscr();
	resize_term(60, 134);
	start_color();
    scrollok(stdscr, true);

	PDC_set_title("Murder In The Masked Dancefloor");

	nl();
	//noecho();
	curs_set(0);
	timeout(0);
	keypad(stdscr, true);

	addstr(mitmdf::S_TITLE.c_str());
	
	bool exitRequested = false;
	while (!exitRequested) {
		switch (programState.GetState()) {
			case mitmdf::ProgramStateEnum::MENU: {
				addstr(mitmdf::S_MENU.c_str());
				int opt = read_option();
				if (opt == 1) {
					auto intro = mitmdf::S_INTRODUCTION;
					size_t pos = intro.find("{{PRESIDENT_NAME}}");
					if (pos != std::string::npos) {
						intro.replace(pos, std::string("{{PRESIDENT_NAME}}").length(), programState.GetGameState().GetPresidentName());
					}
					pos = intro.find("{{PRESIDENT_PRONOUN}}");
					if (pos != std::string::npos) {
						intro.replace(pos, std::string("{{PRESIDENT_PRONOUN}}").length(), mitmdf::PRONOUNS[static_cast<int>(programState.GetGameState().GetPresidentGender())]);
					}
					addstr(intro.c_str());
				} else if (opt == 0) {
					exitRequested = true;
				} else {
					addstr(mitmdf::S_UNRECOGNISED_OPTION.c_str());
				}
				break;
			}
			case mitmdf::ProgramStateEnum::PLAYING:
				playGame(programState);
				break;
			case mitmdf::ProgramStateEnum::GAMEOVER_WIN:
				//addstr(mitmdf::S_WIN.c_str());
				programState.SwitchToState(mitmdf::ProgramStateEnum::MENU);
				break;
			case mitmdf::ProgramStateEnum::GAMEOVER_LOSE:
				//addstr(mitmdf::S_LOSE.c_str());
				programState.SwitchToState(mitmdf::ProgramStateEnum::MENU);
				break;
		}
	}

	nocbreak();
	keypad(stdscr, false);
	echo();
	endwin();

	return 0;
} 
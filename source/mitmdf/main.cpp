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

void press_any_key() {
	addstr("\n\n   Press enter to continue...");
	getnstr(text_buffer, BUFFER_SIZE - 1);
}

void playGame(mitmdf::ProgramState& programState) {
	mitmdf::GameState& gameState = programState.GetGameState();
	switch (gameState.GetCurrentState()) {
		case mitmdf::GameStateEnum::INTRO: {
			addstr(mitmdf::S_PLAY_OPTIONS.c_str());
			int opt = read_option();
			if (opt == 1) {
				gameState.SwitchToState(mitmdf::GameStateEnum::FACT);
			} else if (opt == 2) {
				gameState.SwitchToState(mitmdf::GameStateEnum::DOSSIER);
			} else if (opt == 3) {
				gameState.SwitchToState(mitmdf::GameStateEnum::ACCUSATION);
			} else if (opt == 4) {
				gameState.SwitchToState(mitmdf::GameStateEnum::GUIDE);
			} else if (opt == 0) {
				addstr(mitmdf::S_LOSE.c_str());
				press_any_key();
				programState.SwitchToState(mitmdf::ProgramStateEnum::GAMEOVER_LOSE);
			} else {
				addstr(mitmdf::S_UNRECOGNISED_OPTION.c_str());
			}
			break;
		}
		case mitmdf::GameStateEnum::FACT:
			switch (gameState.GetWeaponUsed()) {
				case mitmdf::WEAPON::KILLING_SPELL:
					addstr("\n   The victim's pupils seem to be dilated.\n");
					break;
				case mitmdf::WEAPON::POISON_THORN:
					addstr("\n   The victim's eyes seem reddened.\n");
					break;
				case mitmdf::WEAPON::HEX_PATCH:
					addstr("\n   The victim's eyes seem normal, but their nails look blue-ish, and it's not nail paint.\n");
					break;
			}

			press_any_key();
			gameState.SwitchToState(mitmdf::GameStateEnum::INTRO);
			break;
		case mitmdf::GameStateEnum::DOSSIER: {
			addstr(mitmdf::S_DOSSIER_HEADER.c_str());
			for (size_t i = 0; i < gameState.GetDancers().size(); ++i) {
				const mitmdf::Dancer& dancer = gameState.GetDancers().at(i);
				std::string name = "    " + std::to_string(i + 1) + ") " + dancer.GetName() + " (" + mitmdf::GENDER_NAMES[static_cast<int>(dancer.GetGender())] + ")\n";
				addstr(name.c_str());
			}
			addstr("    0) Back\n");

			int opt = read_option();

			if (opt == 0) {
				gameState.SwitchToState(mitmdf::GameStateEnum::INTRO);
			}
			else if (opt > 0 && opt <= gameState.GetDancers().size()) {
				// Show details for the selected dancer
				const mitmdf::Dancer& selectedDancer = gameState.GetDancers().at(opt - 1);
				std::string details = selectedDancer.GetSubjectDescription();
				addstr(details.c_str());
				press_any_key();
			}
			else {
				addstr(mitmdf::S_UNRECOGNISED_OPTION.c_str());
			}
			break;
		}
		case mitmdf::GameStateEnum::GUIDE:
			addstr(mitmdf::S_GUIDE.c_str());
			press_any_key();
			gameState.SwitchToState(mitmdf::GameStateEnum::INTRO);
			break;
		case mitmdf::GameStateEnum::ACCUSATION: {
			addstr(mitmdf::S_ACCUSATION_PROMPT.c_str());
			for (size_t i = 0; i < gameState.GetDancers().size(); ++i) {
				const mitmdf::Dancer& dancer = gameState.GetDancers().at(i);
				std::string name = "    " + std::to_string(i + 1) + ") " + dancer.GetName() + " (" + mitmdf::GENDER_NAMES[static_cast<int>(dancer.GetGender())] + ")\n";
				addstr(name.c_str());
			}
			addstr("    0) Back\n");
			int accusationOpt = read_option();
			if (accusationOpt == 0) {
				gameState.SwitchToState(mitmdf::GameStateEnum::INTRO);
			}
			else if (accusationOpt > 0 && accusationOpt <= gameState.GetDancers().size()) {
				const mitmdf::Dancer& accusedDancer = gameState.GetDancers().at(accusationOpt - 1);
				if (&accusedDancer == gameState.GetKiller()) {
					addstr(mitmdf::S_WIN.c_str());
					press_any_key();
					programState.SwitchToState(mitmdf::ProgramStateEnum::GAMEOVER_WIN);
				}
				else {
					auto loseMsg = mitmdf::S_LOSE;
					size_t pos = loseMsg.find("{{KILLER_NAME}}");
					if (pos != std::string::npos) {
						loseMsg.replace(pos, std::string("{{KILLER_NAME}}").length(), gameState.GetKiller()->GetName());
					}
					addstr(loseMsg.c_str());
					press_any_key();
					programState.SwitchToState(mitmdf::ProgramStateEnum::GAMEOVER_LOSE);
				}
			}
			else {
				addstr(mitmdf::S_UNRECOGNISED_OPTION.c_str());
			}
			break;
		}
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
					pos = intro.find("{{PRESIDENT_POSSESSIVE_PRONOUN}}");
					if (pos != std::string::npos) {
						intro.replace(pos, std::string("{{PRESIDENT_POSSESSIVE_PRONOUN}}").length(), mitmdf::POSSESSIVE_PRONOUNS[static_cast<int>(programState.GetGameState().GetPresidentGender())]);
					}
					addstr(intro.c_str());
				} else if (opt == 2) {
					programState.SwitchToState(mitmdf::ProgramStateEnum::PLAYING);
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
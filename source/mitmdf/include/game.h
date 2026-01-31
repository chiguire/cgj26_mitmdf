#ifndef GAME_H
#define GAME_H

namespace mitmdf {

    class Position {
    public:
        Position(int x, int y) : m_x(x), m_y(y) {}
        ~Position() {}

        int GetX() const { return m_x; }
        int GetY() const { return m_y; }
    private:
        int m_x;
        int m_y;
    };

    enum Gender {
        GENDER_MAN,
        GENDER_WOMAN,
        GENDER_NON_BINARY,
    };

    class Dancer {
    public:
        Dancer();
        ~Dancer();

    private:
        const char *m_name;
        const Gender m_gender;
        const Position *m_last_positions;
    };

    enum GameStateEnum {
        STATE_INTRO,
        STATE_QUESTION,
        STATE_ANSWER,
        STATE_ACCUSATION,
    };

    class GameState {
    public:
        GameState();
        ~GameState();

        GameStateEnum GetCurrentState() const {
            return m_currentState;
        }

        void StartGame();
    private:
        

        GameStateEnum m_currentState;
    };

    enum ProgramStateEnum {
        STATE_MENU,
        STATE_PLAYING,
        STATE_GAMEOVER_WIN,
        STATE_GAMEOVER_LOSE,
    };

    class ProgramState {
    public:
        ProgramState();
        ~ProgramState();

        GameState &GetGameState();

    private:
    void SwitchToState(GameStateEnum newState);
        GameState m_gameState;
    };
}

#endif GAME_H
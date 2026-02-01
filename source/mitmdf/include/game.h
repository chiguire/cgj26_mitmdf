#ifndef GAME_H
#define GAME_H

#include <vector>
#include <cstdlib>
#include <random>

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

    enum class Gender {
        MAN,
        WOMAN,
        NON_BINARY,
        ___last,
    };

    class Dancer {
    public:
        Dancer();
        ~Dancer();

    private:
        const bool m_generic; // If true, this dancer is not a suspect
        const char *m_name;
        const Gender m_gender;
        const Position *m_last_positions;
    };

    enum class GameStateEnum {
        INTRO,
        QUESTION,
        ANSWER,
        ACCUSATION,
    };

    class GameState {
    public:
        GameState();
        ~GameState();

        GameStateEnum GetCurrentState() const {
            return m_currentState;
        }

        void StartGame();
        void SwitchToState(GameStateEnum newState);
        const char* GetPresidentName() const {
            return m_presidentName;
        }
        Gender GetPresidentGender() const {
            return m_presidentGender;
        }
    private:
        GameStateEnum m_currentState;
        
        const char *m_presidentName;
        Gender m_presidentGender;

        std::vector<Dancer> m_dancers;
        //std::vector<BallState> m_ball;
    };

    enum class ProgramStateEnum {
        MENU,
        PLAYING,
        GAMEOVER_WIN,
        GAMEOVER_LOSE,
    };

    class ProgramState {
    public:
        ProgramState(int seed = std::time(nullptr));
        ~ProgramState();

        ProgramStateEnum GetState() const;
        GameState &GetGameState();
        void SwitchToState(ProgramStateEnum newState);
        
        unsigned int GetRandomInt();

    private:
        GameState *m_gameState;
        std::mt19937 m_randomDevice;
        ProgramStateEnum m_programState;
    };

    static ProgramState *g_programState = nullptr;

    // Texts
    extern std::string S_TITLE;
    extern std::string S_INTRODUCTION;
    extern std::string S_MENU;
    extern std::string S_PROMPT;
    extern std::string S_GUIDE;
    extern std::string S_UNRECOGNISED_OPTION;

    extern std::vector<std::string> MAN_NAMES;
    extern std::vector<std::string> WOMAN_NAMES;
    extern std::vector<std::string> NON_BINARY_NAMES;

    enum class WEAPON {
        KILLING_SPELL, // People will say that hands were moved in a specific pattern. Takes 3 turns after hand moved to take effect.
        POISON_THORN, // People might point towards a very specific type of flower weared by the killer. Takes 2 turns to take effect.
        HEX_PATCH,    // People might mention that hands were covered in a particular colour. Takes 1 turn to take effect.
        ___last,
    };

    extern std::vector<std::string> WEAPON_NAMES;

    enum class COLOUR {
        RED,
        BLUE,
        GREEN,
        YELLOW,
        PURPLE,
        ORANGE,
        BLACK,
        WHITE,
        PINK,
        BROWN,
        GREY,
        SILVER,
        GOLD,
        ___last,
    };

    extern std::vector<std::string> COLOUR_NAMES;

    enum class MASK_TYPE {
        FULL,
        HALF,
        ___last,
    };

    extern std::vector<std::string> MASK_TYPE_NAMES;

    extern std::vector<std::string> PRONOUNS;
    extern std::vector<std::string> POSSESSIVE_PRONOUNS;

    const char* randomName(const mitmdf::Gender g);
}

#endif GAME_H
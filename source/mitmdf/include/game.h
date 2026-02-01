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

    enum class WEAPON {
        KILLING_SPELL, // People will say that hands were moved in a specific pattern. Takes 3 turns after hand moved to take effect.
        POISON_THORN, // People might point towards a very specific type of flower weared by the killer. Takes 2 turns to take effect.
        HEX_PATCH,    // People might mention that hands were covered in a particular colour. Takes 1 turn to take effect.
        ___last,
    };

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

    enum class MASK_TYPE {
        FULL,
        HALF,
        ___last,
    };

    class Dancer {
    public:
        Dancer();
        ~Dancer();

        std::string GetSubjectDescription() const;
        std::vector<Position> &GetLastPositions();
        const char* GetName() const {
            return m_name;
        }
        Gender GetGender() const {
            return m_gender;
        }
    private:
        const bool m_generic; // If true, this dancer is not a suspect
        const char *m_name;
        const Gender m_gender;
        const mitmdf::COLOUR m_eyeColour;
        const MASK_TYPE m_maskType;
        const COLOUR m_maskBaseColour;
        const COLOUR m_maskLipsColour;
        const COLOUR m_maskEyeshadowColour;
        const COLOUR m_capeColour;
        std::vector<Position> m_last_positions;
    };

    enum class GameStateEnum {
        START,
        INTRO,
        FACT,
        DOSSIER,
        GUIDE,
        ACCUSATION,
        END,
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
        const std::vector<Dancer>& GetDancers() const {
            return m_dancers;
        }
        const Dancer* GetKiller() const {
            return m_killer;
        }
        const WEAPON GetWeaponUsed() const {
            return m_weaponUsed;
        }
    private:
        void InitializeGame();
        GameStateEnum m_currentState;
        
        const char *m_presidentName;
        Gender m_presidentGender;

        std::vector<Dancer> m_dancers;
        Dancer *m_killer;
        WEAPON m_weaponUsed;
        const int m_numDancers = 6;
        const int m_dancefloorWidth = 15;
        const int m_numberOfTurns = 6;
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
    extern std::string S_PLAY_OPTIONS;
    extern std::string S_DOSSIER_HEADER;
    extern std::string S_ACCUSATION_PROMPT;
    extern std::string S_WIN;
    extern std::string S_LOSE;
    extern std::string S_PROMPT;
    extern std::string S_GUIDE;
    extern std::string S_UNRECOGNISED_OPTION;

    extern std::vector<std::string> GENDER_NAMES;
    extern std::vector<std::string> MAN_NAMES;
    extern std::vector<std::string> WOMAN_NAMES;
    extern std::vector<std::string> NON_BINARY_NAMES;

    extern std::vector<std::string> WEAPON_NAMES;

    extern std::vector<std::string> COLOUR_NAMES;

    extern std::vector<std::string> MASK_TYPE_NAMES;

    extern std::vector<std::string> PRONOUNS;
    extern std::vector<std::string> POSSESSIVE_PRONOUNS;

    extern std::string S_SUBJECT_DESCRIPTION;

    const char* randomName(const mitmdf::Gender g);
}

#endif GAME_H
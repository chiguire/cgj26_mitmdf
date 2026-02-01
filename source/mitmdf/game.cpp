#include <random>
#include <ctime>

#include "include/game.h"
#include "include/util.h"

namespace mitmdf {

    // ProgramState implementation
    ProgramState::ProgramState(int seed)
        : m_randomDevice(seed)
        , m_programState(ProgramStateEnum::MENU)
    {
        g_programState = this;
        m_gameState = new GameState();
    }

    ProgramState::~ProgramState() {
        m_gameState->~GameState();
        m_gameState = nullptr;
    }

    unsigned int ProgramState::GetRandomInt() {
        return m_randomDevice();
    }

    GameState &ProgramState::GetGameState() {
        return *m_gameState;
    }

    ProgramStateEnum ProgramState::GetState() const {
        return m_programState;
    }

    void ProgramState::SwitchToState(ProgramStateEnum newState) {
        m_programState = newState;
        if (newState == ProgramStateEnum::MENU) {
            m_gameState->StartGame();
        }
    }

    // GameState implementation
    GameState::GameState() 
        : m_presidentGender(randomEnum<Gender>())
        , m_presidentName(mitmdf::randomName(m_presidentGender))
        , m_currentState(GameStateEnum::INTRO) {
    }

    GameState::~GameState() {
        // TODO: Clean up GameState
    }

    void GameState::StartGame() {
        m_currentState = GameStateEnum::INTRO;
    }

    void GameState::SwitchToState(GameStateEnum newState) {
        m_currentState = newState;
    }

    Dancer::Dancer()
    : m_gender(randomEnum<Gender>())
    , m_generic(false)
    , m_name(randomName(m_gender)) {

    }

    Dancer::~Dancer() {

    }


    // Texts
    extern std::string S_TITLE = ""
    "*************************************************************************************************************************88******\n"
    "*===============================================================================================================================*\n"
    "*=  =====  =====================  =============================================  ===============================================*\n"
    "*=   ===   =====================  =============================================  =============       ===================       =*\n"
    "*=  =   =  =====================  ========================================  ===  =============           ===========           =*\n"
    "*=  == ==  ==  =  ==  =   ======  ===   ===  =   ========  ==  = ========    ==  ======   =====    =====      =      =====    ==*\n"
    "*=  =====  ==  =  ==    =  ===    ==  =  ==    =  ===========     ========  ===    ===  =  =====    =======       =======    ===*\n"
    "*=  =====  ==  =  ==  =======  =  ==     ==  ============  ==  =  ========  ===  =  ==     =====      ======     ======      ===*\n"
    "*=  =====  ==  =  ==  =======  =  ==  =====  ============  ==  =  ========  ===  =  ==  ========  =                       =  ===*\n"
    "*=  =====  ==  =  ==  =======  =  ==  =  ==  ============  ==  =  ========  ===  =  ==  =  ====  ========           ========   =*\n"
    "*=  =====  ===    ==  ========    ===   ===  ============  ==  =  ========   ==  =  ===   ===== ========   ==   ==   ========= =*\n"
    "*============================================================================================== =======  ===== =====  ======== =*\n"
    "*============================================================================================================================= =*\n"
    "*=  =====  ================  ===============  =======       =================================    ===  ======================== =*\n"
    "*=   ===   ================  ===============  =======  ====  ===============================  ==  ==  ======================== =*\n"
    "*=  =   =  ================  ===============  =======  ====  ===============================  ======  ======================== =*\n"
    "*=  == ==  ===   ====   ===  =  ===   ======  =======  ====  ===   ===  = ====   ====   ===    =====  ===   ====   ===  =   == =*\n"
    "*=  =====  ==  =  ==  =  ==    ===  =  ===    =======  ====  ==  =  ==     ==  =  ==  =  ===  ======  ==     ==     ==    =  = =*\n"
    "*=  =====  =====  ===  ====   ====     ==  =  =======  ====  =====  ==  =  ==  =====     ===  ======  ==  =  ==  =  ==  ====== =*\n"
    "*=  =====  ===    ====  ===    ===  =====  =  =======  ====  ===    ==  =  ==  =====  ======  ======  ==  =  ==  =  ==  ====== =*\n"
    "*=  =====  ==  =  ==  =  ==  =  ==  =  ==  =  =======  ====  ==  =  ==  =  ==  =  ==  =  ===  ======  ==  =  ==  =  ==  ====== =*\n"
    "*=  =====  ===    ===   ===  =  ===   ====    =======       ====    ==  =  ===   ====   ====  ======  ===   ====   ===  ====== =*\n"
    "*===============================================================================================================================*\n"
    "*********************************************************************************************************************************\n"
    "\n"
    "   A Caracas Game Jam/Global Game Jam 2026 Procedurally Generated Murdery Mystery.\n"
    "   Programming and writing: Ciro Durán (ciro.duran@gmail.com)\n"
    "   January/February 2026. Brighton and Hove, UK.\n"
    "\n"
    "";

    extern std::string S_MENU = ""
    "   Welcome! Choose an option:\n"
    "       1) Introduction\n"
    "       2) Start Game\n"
    "       0) Exit\n";

    extern std::string S_PROMPT = "    Your option: ";

    extern std::string S_UNRECOGNISED_OPTION = "    Unrecognised option. Please try again.\n";

    extern std::string S_INTRODUCTION = ""
    "At the strike of midnight, the New Year's Eve Masked Ball was interrupted by\n"
    "a callous murder right in the middle of the dancefloor. {{PRESIDENT_NAME}}, the president of\n"
    "the Magic Society and organiser of the NYE Masked Ball fell down to the floor,\n"
    "lifeless, while the rest of the dancing group were celebrating the fireworks\n"
    "that were exploding right above them in the ceiling.\n"
    "\n"
    "It was a few seconds later when the horror was quite apparent. The president's\n"
    "body lied on the floor and {{PRESIDENT_PRONOUN}} current dancing partner\n"
    "screamed uncontrollably as they were realising what had just happened.\n"
    "\n"
    "You are a detective called on emergency to figure out who killed the president.\n"
    "\n"
    "In this ball, everyone dances standing in a checkered floor, one person per\n"
    "tile. Every 5 seconds, the partners switch tiles with a random partner. The\n"
    "room has some memory and will let you know some things that happened in the\n"
    "previous 30 seconds to the murder.\n"
    "\n"
    "You can also interview 6 suspects, among which the killer is one for sure. The\n"
    "killer will attempt to lie and misguide you, the rest do not have reason to lie.\n"
    "\n"
    "You can consult the magician's cheat sheet to figure out facts that you might\n"
    "need to know in order to solve the case.\n"
    "\n"
    "Good luck!\n";

    extern std::string S_GUIDE = ""
    "Magician's Guide to Murder\n"
    "==========================\n"
    "\n"
    "The most common ways magicians murder people are:\n"
    "\n"
    "1) Killing spell: an elegant way to murder. The spellcaster must move one of\n"
    "their hands. From a closed fist they open their hand finger by finger slowly\n"
    "starting from the index, keeping the pinky curled; then they softly twist their\n"
    "hand. While doing this they recite some words in silence while looking at the\n"
    "victim. 15 seconds later, the victim will drop dead instantly, like a ragdoll.\n"
    "You can tell a killing spell was used because the victim's pupils are dilated.\n"
    "\n"
    "2) Poison thorn: magicians usually wear flowers with thorns in important\n"
    "occasions. This makes a flower with a poisonous thorn a perfect weapon. The\n"
    "murderer must be standing next to the victim when they apply the thorn to\n"
    "any part of the body. The thorn does not become poisonous until it is applied\n"
    "to the victim. 15 seconds later, the victim starts coughing and then suddenly\n"
    "falls down. The thorn is dissolved at the moment, leaving no trace of it. The\n"
    "victims eyes look reddened.\n"
    "\n"
    "3) Hex patch: risky and gruesome, magicians tend not to use it. The murderer\n"
    "needs to apply the patch to the victim, and the patch acts only after 5 seconds.\n"
    "The murderer hands are usually a bit red after handling this patch. The patch\n"
    "dissolves, leaving no trace of it. The victim's nails look blue-ish after the\n"
    "patch does its effect.\n"
    "\n";

    extern std::vector<std::string> MAN_NAMES = {
        "Roscoe",
        "Logan",
        "John",
        "Spencer",
        "Matthew",
        "Bruce",
    };

    extern std::vector<std::string> WOMAN_NAMES = {
        "Aria",
        "Eve",
        "Luna",
        "Peyton",
        "Tarja",
        "Bess",
        "Maria",
    };

    extern std::vector<std::string> NON_BINARY_NAMES = {
        "Hunter",
        "Alex",
        "Jessie",
        "Taylor",
        "Jordan",
        "Quinn",
        "Avery",
        "Morgan",
    };

    extern std::vector<std::string> WEAPON_NAMES = {
        "killing spell",
        "poison thorn",
        "hex patch",
    };
    
    extern std::vector<std::string> COLOUR_NAMES = {
        "red",
        "blue",
        "green",
        "yellow",
        "purple",
        "orange",
        "black",
        "white",
        "pink",
        "brown",
        "grey",
        "silver",
        "gold",
    };

    extern std::vector<std::string> MASK_TYPE_NAMES = {
        "full face",
        "half face",
    };

    extern std::vector<std::string> PRONOUNS = {
        "he",
        "she",
        "they",
    };

    extern std::vector<std::string> POSSESSIVE_PRONOUNS = {
        "his",
        "her",
        "their",
    };

    const char* randomName(const mitmdf::Gender g) {
        switch (g) {
            case mitmdf::Gender::MAN:
                return stringVectorRand(mitmdf::MAN_NAMES);
            case mitmdf::Gender::WOMAN:
                return stringVectorRand(mitmdf::WOMAN_NAMES);
            case mitmdf::Gender::NON_BINARY:
                return stringVectorRand(mitmdf::NON_BINARY_NAMES);
            default:
                return "Unknown";
        }
    }
}
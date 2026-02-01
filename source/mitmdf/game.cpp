#include <random>
#include <ctime>

#include "include/game.h"
#include "include/util.h"

namespace mitmdf
{

    // ProgramState implementation
    ProgramState::ProgramState(int seed)
        : m_randomDevice(seed), m_programState(ProgramStateEnum::MENU)
    {
        g_programState = this;
        m_gameState = new GameState();
        m_gameState->StartGame();
    }

    ProgramState::~ProgramState()
    {
        m_gameState->~GameState();
        m_gameState = nullptr;
    }

    unsigned int ProgramState::GetRandomInt()
    {
        return m_randomDevice();
    }

    GameState &ProgramState::GetGameState()
    {
        return *m_gameState;
    }

    ProgramStateEnum ProgramState::GetState() const
    {
        return m_programState;
    }

    void ProgramState::SwitchToState(ProgramStateEnum newState)
    {
        m_programState = newState;
        if (newState == ProgramStateEnum::MENU)
        {
            m_gameState->StartGame();
        }
    }

    // GameState implementation
    GameState::GameState()
        : m_presidentGender(randomEnum<Gender>())
        , m_presidentName(mitmdf::randomName(m_presidentGender))
        , m_currentState(GameStateEnum::START)
        , m_dancers()
        , m_killer(nullptr)
        , m_weaponUsed(WEAPON::HEX_PATCH) // For now, default weapon
    {
    }

    GameState::~GameState()
    {
        // TODO: Clean up GameState
    }

    void GameState::StartGame()
    {
        InitializeGame();
        m_currentState = GameStateEnum::START;
        SwitchToState(GameStateEnum::INTRO);
    }

    void GameState::SwitchToState(GameStateEnum newState)
    {
        m_currentState = newState;
    }

    void GameState::InitializeGame()
    {
        // Initialize dancers
            m_dancers.clear();
            for (int i = 0; i < m_numDancers; ++i)
            {
                auto dancer = Dancer();
                m_dancers.push_back(dancer);
            }
            // Select a killer randomly
            int killerIndex = g_programState->GetRandomInt() % 4; // Among first 4 dancers
            m_killer = &m_dancers[killerIndex];
            // Select a weapon randomly
            m_weaponUsed = randomEnum<WEAPON>();
            
            std::vector<std::vector<Dancer *>> dancefloor(m_dancefloorWidth, std::vector<Dancer *>(m_dancefloorWidth, nullptr));
            // Select 4 dancers for putting around center of the dancefloor
            // The rest are a bit further away
            std::vector<Dancer *> selectedDancers;
            std::vector<Dancer *> remainingDancers;
            std::vector<int> indices;
            while (selectedDancers.size() < 4) {
                int index = g_programState->GetRandomInt() % m_dancers.size();
                if (std::find(indices.begin(), indices.end(), index) == indices.end()) {
                    indices.push_back(index);
                    selectedDancers.push_back(&m_dancers[index]);
                }
            }
            for (unsigned int i = 0; i < m_dancers.size(); ++i) {
                if (std::find(indices.begin(), indices.end(), i) == indices.end()) {
                    remainingDancers.push_back(&m_dancers[i]);
                }
            }

            // Place selected dancers around center
            int centerX = m_dancefloorWidth / 2;
            int centerY = m_dancefloorWidth / 2;
            dancefloor[centerX - 1][centerY] = selectedDancers[0];
            selectedDancers[0]->GetLastPositions().push_back(Position(centerX - 1, centerY));
            dancefloor[centerX + 1][centerY] = selectedDancers[1];
            selectedDancers[1]->GetLastPositions().push_back(Position(centerX + 1, centerY));
            dancefloor[centerX][centerY - 1] = selectedDancers[2]; 
            selectedDancers[2]->GetLastPositions().push_back(Position(centerX, centerY - 1));
            dancefloor[centerX][centerY + 1] = selectedDancers[3];
            selectedDancers[3]->GetLastPositions().push_back(Position(centerX, centerY + 1));

            // Place remaining dancers randomly
            std::vector<Position> availablePositions = {
                Position(-2, -1), Position(-2, 0), Position(-2, 1), // Left side
                Position(-1, -2), Position(0, -2), Position(1, -2), // Top side
                Position(2, -1), Position(2, 0), Position(2, 1),    // Right side
                Position(-1, 2), Position(0, 2), Position(1, 2),    // Bottom side
            };

            for (auto &dancer : remainingDancers) {
                int posIndex = g_programState->GetRandomInt() % availablePositions.size();
                Position pos = availablePositions[posIndex];
                dancefloor[centerX + pos.GetX()][centerY + pos.GetY()] = dancer;
                dancer->GetLastPositions().push_back(Position(centerX + pos.GetX(), centerY + pos.GetY()));
                availablePositions.erase(availablePositions.begin() + posIndex);
            }

            // For m_numberOfTurns, move dancers in a random position
            // and store their last positions
            for (int turn = 0; turn < m_numberOfTurns; ++turn) {
                if (turn == 0) {
                    // In first turn, selected dancers have a fixed direction
                    selectedDancers[0]->GetLastPositions().push_back(Position(centerX - 2, centerY));
                    dancefloor[centerX - 2][centerY] = selectedDancers[0];
                    dancefloor[centerX - 1][centerY] = nullptr;
                    selectedDancers[1]->GetLastPositions().push_back(Position(centerX + 2, centerY));
                    dancefloor[centerX + 2][centerY] = selectedDancers[1];
                    dancefloor[centerX + 1][centerY] = nullptr;
                    selectedDancers[2]->GetLastPositions().push_back(Position(centerX, centerY - 2));
                    dancefloor[centerX][centerY - 2] = selectedDancers[2];
                    dancefloor[centerX][centerY - 1] = nullptr;
                    selectedDancers[3]->GetLastPositions().push_back(Position(centerX, centerY + 2));
                    dancefloor[centerX][centerY + 2] = selectedDancers[3];
                    dancefloor[centerX][centerY + 1] = nullptr;
                } else {
                    // From second turn onwards, move randomly. Try all directions, leave in place if no move possible
                    for (auto &dancer : m_dancers) {
                        Position lastPos = dancer.GetLastPositions().back();
                        Position newPos = lastPos;
                        std::vector<Position> possibleMoves = {
                            Position(-1, 0),
                            Position(1, 0),
                            Position(0, -1),
                            Position(0, 1),
                        };
                        while (!possibleMoves.empty()) {
                            int moveIndex = g_programState->GetRandomInt() % possibleMoves.size();
                            Position move = possibleMoves.at(moveIndex);
                            possibleMoves.erase(possibleMoves.begin() + moveIndex);
                            Position newPos2(lastPos.GetX() + move.GetX(), lastPos.GetY() + move.GetY());

                            if (newPos2.GetX() >= 0 && newPos2.GetX() < m_dancefloorWidth && newPos2.GetY() >= 0 && newPos2.GetY() < m_dancefloorWidth) {
                                if (dancefloor[newPos2.GetX()][newPos2.GetY()] == nullptr) {
                                    dancefloor[lastPos.GetX()][lastPos.GetY()] = nullptr;
                                    dancefloor[newPos2.GetX()][newPos2.GetY()] = &dancer;
                                    newPos = newPos2;
                                    break;
                                }
                            }
                        }
                        dancer.GetLastPositions().push_back(newPos);
                    }
                }
            }
    }

    Dancer::Dancer()
        : m_gender(randomEnum<Gender>())
        , m_generic(false)
        , m_name(randomName(m_gender))
        , m_eyeColour(randomEnum<COLOUR>())
        , m_maskType(randomEnum<MASK_TYPE>())
        , m_maskBaseColour(randomEnum<COLOUR>())
        , m_maskLipsColour(randomEnum<COLOUR>())
        , m_maskEyeshadowColour(randomEnum<COLOUR>())
        , m_capeColour(randomEnum<COLOUR>())
        , m_last_positions()
    {
    }

    Dancer::~Dancer()
    {
    }

    std::string Dancer::GetSubjectDescription() const
    {
        std::string description = S_SUBJECT_DESCRIPTION;
        // Replace placeholders with actual values
        size_t pos;
        pos = description.find("{{FIRST_NAME}}");
        if (pos != std::string::npos)
        {
            description.replace(pos, std::string("{{FIRST_NAME}}").length(), m_name);
        }
        pos = description.find("{{GENDER}}");
        if (pos != std::string::npos)
        {
            description.replace(pos, std::string("{{GENDER}}").length(), GENDER_NAMES[static_cast<int>(m_gender)]);
        }
        pos = description.find("{{EYE_COLOUR}}");
        if (pos != std::string::npos)
        {
            description.replace(pos, std::string("{{EYE_COLOUR}}").length(), COLOUR_NAMES[static_cast<int>(m_eyeColour)]);
        }
        pos = description.find("{{MASK_TYPE}}");
        if (pos != std::string::npos)
        {
            description.replace(pos, std::string("{{MASK_TYPE}}").length(), MASK_TYPE_NAMES[static_cast<int>(m_maskType)]);
        }
        pos = description.find("{{MASK_BASE_COLOUR}}");
        if (pos != std::string::npos)
        {
            description.replace(pos, std::string("{{MASK_BASE_COLOUR}}").length(), COLOUR_NAMES[static_cast<int>(m_maskBaseColour)]);
        }
        pos = description.find("{{MASK_LIPS_COLOUR}}");
        if (pos != std::string::npos)
        {
            description.replace(pos, std::string("{{MASK_LIPS_COLOUR}}").length(), COLOUR_NAMES[static_cast<int>(m_maskLipsColour)]);
        }
        pos = description.find("{{MASK_EYESHADOW_COLOUR}}");
        if (pos != std::string::npos)
        {
            description.replace(pos, std::string("{{MASK_EYESHADOW_COLOUR}}").length(), COLOUR_NAMES[static_cast<int>(m_maskEyeshadowColour)]);
        }
        pos = description.find("{{CAPE_COLOUR}}");
        if (pos != std::string::npos)
        {
            description.replace(pos, std::string("{{CAPE_COLOUR}}").length(), COLOUR_NAMES[static_cast<int>(m_capeColour)]);
        }

        return description;
    }

    std::vector<Position> &Dancer::GetLastPositions()
    {
        return m_last_positions;
    }

    // Texts
    extern std::string S_TITLE = ""
                                 "*********************************************************************************************************************************\n"
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
                                "\n   Welcome! Choose an option:\n"
                                "       1) Introduction\n"
                                "       2) Start Game\n"
                                "       0) Exit\n";

    extern std::string S_PROMPT = "\n    Your option: ";

    extern std::string S_UNRECOGNISED_OPTION = "\n    Unrecognised option. Please try again.\n";

    extern std::string S_INTRODUCTION = ""
                                        "At the strike of midnight, the New Year's Eve Masked Ball was interrupted by\n"
                                        "a callous murder right in the middle of the dancefloor. {{PRESIDENT_NAME}}, the president of\n"
                                        "the Magic Society and organiser of the NYE Masked Ball fell down to the floor,\n"
                                        "lifeless, while the rest of the dancing group were celebrating the fireworks\n"
                                        "that were exploding right above them in the ceiling.\n"
                                        "\n"
                                        "It was a few seconds later when the horror was quite apparent. The president's\n"
                                        "body lied on the floor and {{PRESIDENT_POSSESSIVE_PRONOUN}} current dancing partner\n"
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

    extern std::vector<std::string> GENDER_NAMES = {
        "man",
        "woman",
        "non-binary",
    };

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

    extern std::string S_PLAY_OPTIONS = ""
        "\n   What would you like to do?\n"
        "       1) Invoke a fact from the ballroom (gives you a hint)\n"
        "       2) See the dossier of suspects\n"
        "       3) Make an accusation\n"
        "       4) Consult the magician's guide to murder\n"
        "       0) Finish game (you lose)\n";

    extern std::string S_ACCUSATION_PROMPT = "\n   Who do you accuse of being the killer? (Enter number): ";
    extern std::string S_DOSSIER_HEADER = "\n   Dossier of suspects:\n";

    extern std::string S_WIN = "\n   Congratulations! You have correctly identified the killer!\n";
    extern std::string S_LOSE = "\n   Sorry, that is incorrect. The killer was {{KILLER_NAME}}.\n";

    extern std::string S_SUBJECT_DESCRIPTION = ""
        "Suspect Description\n"
        "===================\n"
        "\n"
        "First name: {{FIRST_NAME}}\n"
        "Eyes colour: {{EYE_COLOUR}}\n"
        "Gender: {{GENDER}}\n"
        "Mask: {{MASK_TYPE}}, {{MASK_BASE_COLOUR}} base, {{MASK_LIPS_COLOUR}} lips, {{MASK_EYESHADOW_COLOUR}} eyeshadow\n"
        "Cape colour: {{CAPE_COLOUR}}\n";

    const char *randomName(const mitmdf::Gender g)
    {
        switch (g)
        {
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
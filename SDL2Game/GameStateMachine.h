#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include <vector>
#include "GameState.h"

class GameStateMachine
{
public:
    void pushState(GameState* pState);
    void changeState(GameState* pState);
    void popState();

    void update();
    void render();

    void clean();
	std::vector<GameState*>& getGameStates() { return m_gameStates; }

private:
    std::vector<GameState*> m_gameStates;
};

#endif
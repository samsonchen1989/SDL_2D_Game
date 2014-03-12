#include "GameStateMachine.h"
#include <iostream>

void GameStateMachine::pushState(GameState* pState)
{
    m_gameStates.push_back(pState);
    m_gameStates.back()->onEnter();
}

void GameStateMachine::popState()
{
    if (m_gameStates.empty() == false) {
        if (m_gameStates.back()->onExit()) {
            delete m_gameStates.back();
            m_gameStates.pop_back();
        }
    }
}

void GameStateMachine::changeState(GameState* pState)
{
    if (m_gameStates.empty() == false) {
        if (m_gameStates.back()->getStateID() == pState->getStateID()) {
            return;
        }
        std::cout << "state machine change state 1" << std::endl;

        m_gameStates.back()->onExit();
        m_gameStates.pop_back();
    }

    //push back new state
    pState->onEnter();
    m_gameStates.push_back(pState);
}

void GameStateMachine::update()
{
    std::cout << "state machine update" << m_gameStates.back()->getStateID() << std::endl;
    if (m_gameStates.empty() == false) {
        m_gameStates.back()->update();
    }
}

void GameStateMachine::render()
{
    //std::cout << "state machine render" << std::endl;
    if (m_gameStates.empty() == false) {
        m_gameStates.back()->render();
    }
}
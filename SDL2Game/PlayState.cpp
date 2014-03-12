#include "PlayState.h"
#include "TextureManager.h"
#include "Game.h"
#include "Player.h"
#include "InputHandler.h"
#include "PauseState.h"

#include <iostream>

const std::string PlayState::s_playID = "PLAY";

void PlayState::update()
{
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE)) {
        TheGame::Instance()->getStateMachine()->pushState(new PauseState());
    }

    for (size_t i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->update();
    }
}

void PlayState::render()
{
    for (size_t i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->draw();
    }
}

bool PlayState::onEnter()
{
    std::cout << "PlayState enter." << std::endl;
    if (TheTextureManager::Instance()->load("assets/helicopter.png",
        "helicopter", TheGame::Instance()->getRenderer()) == false)
    {
        std::cout << "PlayState read image false." << std::endl;
        return false;
    }

    GameObject* player = new Player(new LoaderParams(100, 100, 128, 55, "helicopter"));
    m_gameObjects.push_back(player);

    std::cout << "PlayState enter return true." << std::endl;
    return true;
}

bool PlayState::onExit()
{
    for (size_t i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->clean();
    }

    m_gameObjects.clear();
    TheTextureManager::Instance()->clearFromTextureMap("helicopter");
    return true;
}
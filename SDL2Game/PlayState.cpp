#include "PlayState.h"
#include "TextureManager.h"
#include "Game.h"
#include "Player.h"
#include "InputHandler.h"
#include "PauseState.h"
#include "Enemy.h"
#include "GameOverState.h"

#include <iostream>

const std::string PlayState::s_playID = "PLAY";

bool PlayState::checkCollision(SDLGameObject* p1, SDLGameObject* p2)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = p1->getPosition().getX();
    rightA = leftA + p1->getWidth();
    topA = p1->getPosition().getY();
    bottomA = topA + p1->getHeight();

    //calculate the sides of rectB
    leftB = p2->getPosition().getX();
    rightB = leftB + p2->getWidth();
    topB = p2->getPosition().getY();
    bottomB = topB + p2->getHeight();

    if (bottomA <= topB) { return false; }
    if (topA >= bottomB) { return false; }
    if (rightA <= leftB) { return false; }
    if (leftA >= rightB) { return false; }

    return true;
}

void PlayState::update()
{
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE)) {
        TheGame::Instance()->getStateMachine()->pushState(new PauseState());
    }

    for (size_t i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->update();
    }

    if (checkCollision(dynamic_cast<SDLGameObject*>(m_gameObjects[0]),
        dynamic_cast<SDLGameObject*>(m_gameObjects[1])))
    {
        TheGame::Instance()->getStateMachine()->pushState(new GameOverState());
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

    if (TheTextureManager::Instance()->load("assets/helicopter2.png",
        "helicopter2", TheGame::Instance()->getRenderer()) == false)
    {
        return false;
    }

    GameObject* player = new Player(new LoaderParams(500, 100, 128, 55, "helicopter"));
    GameObject* enemy = new Enemy(new LoaderParams(100, 100, 128, 55, "helicopter2"));

    m_gameObjects.push_back(player);
    m_gameObjects.push_back(enemy);

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
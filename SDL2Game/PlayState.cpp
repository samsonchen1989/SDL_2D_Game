#include "PlayState.h"
#include "TextureManager.h"
#include "Game.h"
#include "Player.h"
#include "InputHandler.h"
#include "PauseState.h"
#include "Enemy.h"
#include "GameOverState.h"
#include "StateParser.h"
#include "LevelParser.h"
#include "Level.h"

#include <iostream>

const std::string PlayState::s_playID = "PLAY";

bool PlayState::checkCollision(SDLGameObject* p1, SDLGameObject* p2)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = static_cast<int>(p1->getPosition().getX());
    rightA = leftA + p1->getWidth();
    topA = static_cast<int>(p1->getPosition().getY());
    bottomA = topA + p1->getHeight();

    //calculate the sides of rectB
    leftB = static_cast<int>(p2->getPosition().getX());
    rightB = leftB + p2->getWidth();
    topB = static_cast<int>(p2->getPosition().getY());
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

    if(pLevel != 0)
    {
        pLevel->render();
    }
}

void PlayState::render()
{
    for (size_t i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->draw();
    }

    if(pLevel != 0)
    {
        pLevel->render();
    }
}

bool PlayState::onEnter()
{
    StateParser stateParser;
    stateParser.parseState("assets/test.xml", s_playID, &m_gameObjects, &m_textureIDList);

    LevelParser levelParser;
    pLevel = levelParser.parseLevel("assets/map1.tmx");

    std::cout << "entering PlayState\n";
    return true;
}

bool PlayState::onExit()
{
    for (size_t i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->clean();
    }

    m_gameObjects.clear();

    for (unsigned i = 0; i < m_textureIDList.size(); i++) {
        std::cout << "PlayState, textureID:" << m_textureIDList[i] << std::endl;
        TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
    }

    return true;
}
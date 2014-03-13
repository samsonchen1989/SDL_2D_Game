#include "GameOverState.h"
#include "Game.h"
#include "MenuState.h"
#include "PlayState.h"
#include "TextureManager.h"
#include "AnimatedGraphic.h"
#include "MenuButton.h"

const std::string GameOverState::s_gameOverID = "GAMEOVER";

void GameOverState::s_gameOverToMain()
{
    TheGame::Instance()->getStateMachine()->changeState(new MenuState());
}

void GameOverState::s_restartPlay()
{
    TheGame::Instance()->getStateMachine()->changeState(new PlayState());
}

bool GameOverState::onEnter()
{
    if (TheTextureManager::Instance()->load("assets/gameover.png", "gameovertext",
        TheGame::Instance()->getRenderer()) == false)
    {
        return false;
    }

    if (TheTextureManager::Instance()->load("assets/main.png", "mainbutton",
        TheGame::Instance()->getRenderer()) == false)
    {
        return false;
    }

    if (TheTextureManager::Instance()->load("assets/restart.png", "restartbutton",
        TheGame::Instance()->getRenderer()) == false)
    {
        return false;
    }

    GameObject* gameOverText = new AnimatedGraphic(new LoaderParams(200, 100, 190, 30, "gameovertext"), 2);
    GameObject* button1 = new MenuButton(new LoaderParams(200, 200, 200, 80, "mainbutton"), s_gameOverToMain);
    GameObject* button2 = new MenuButton(new LoaderParams(200, 300, 200, 80, "restartbutton"), s_restartPlay);

    m_gameObjects.push_back(gameOverText);
    m_gameObjects.push_back(button1);
    m_gameObjects.push_back(button2);

    return true;
}

bool GameOverState::onExit()
{
    for (size_t i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->clean();
    }

    m_gameObjects.clear();

    TheTextureManager::Instance()->clearFromTextureMap("gameovertext");
    TheTextureManager::Instance()->clearFromTextureMap("mainbutton");
    TheTextureManager::Instance()->clearFromTextureMap("restartbutton");

    return true;
}

void GameOverState::render()
{
    for (size_t i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->draw();
    }
}

void GameOverState::update()
{
    for (size_t i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->update();
    }
}
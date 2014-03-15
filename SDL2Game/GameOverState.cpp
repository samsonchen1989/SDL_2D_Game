#include "GameOverState.h"
#include "Game.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "TextureManager.h"
#include "AnimatedGraphic.h"
#include "MenuButton.h"
#include "StateParser.h"
#include "InputHandler.h"

const std::string GameOverState::s_gameOverID = "GAMEOVER";

void GameOverState::s_gameOverToMain()
{
    TheGame::Instance()->getStateMachine()->changeState(new MainMenuState());
}

void GameOverState::s_restartPlay()
{
    TheGame::Instance()->getStateMachine()->changeState(new PlayState());
}

bool GameOverState::onEnter()
{
    /*
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
    */
    StateParser stateParser;
    stateParser.parseState("assets/test.xml", s_gameOverID, &m_gameObjects, &m_textureIDList);
    m_callbacks.push_back(0);
    m_callbacks.push_back(s_gameOverToMain);
    m_callbacks.push_back(s_restartPlay);

    setCallbacks(m_callbacks);

    std::cout << "entering GameoverState\n";
    return true;
}

void GameOverState::setCallbacks(const std::vector<Callback>& callbacks)
{
    //go through the objects
    for (unsigned i = 0; i < m_gameObjects.size(); i++) {
        //if they are of type MenuButton then assign a callback
        //based on the id passed in from the list
        if (dynamic_cast<MenuButton*>(m_gameObjects[i])) {
            MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
            pButton->setCallback(callbacks[pButton->getCallbackID()]);
        }
    }
}

bool GameOverState::onExit()
{
    for (size_t i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->clean();
    }

    m_gameObjects.clear();

    for(unsigned i = 0; i < m_textureIDList.size(); i++) {
        TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
    }

    TheInputHandler::Instance()->reset();

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
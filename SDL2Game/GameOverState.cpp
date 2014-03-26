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
    // parse the state
    StateParser stateParser;
    stateParser.parseState("assets/conan.xml", s_gameOverID, &m_gameObjects, &m_textureIDList);

    m_callbacks.push_back(0);
    m_callbacks.push_back(s_gameOverToMain);
    m_callbacks.push_back(s_restartPlay);

    // set the callbacks for menu items
    setCallbacks(m_callbacks);

    m_loadingComplete = true;

    std::cout << "entering GameOverState\n";
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
    if(m_loadingComplete && !m_gameObjects.empty())
    {
        for(int i = 0; i < m_gameObjects.size(); i++)
        {
            m_gameObjects[i]->clean();
            delete m_gameObjects[i];
        }

        m_gameObjects.clear();
    }

    std::cout << m_gameObjects.size();

    // clear the texture manager
    for(int i = 0; i < m_textureIDList.size(); i++)
    {
        TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
    }

    TheInputHandler::Instance()->reset();

    std::cout << "exiting GameOverState\n";
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
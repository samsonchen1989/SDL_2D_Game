#include "PauseState.h"
#include "Game.h"
#include "MainMenuState.h"
#include "TextureManager.h"
#include "MenuButton.h"
#include "InputHandler.h"
#include "StateParser.h"

const std::string PauseState::s_pauseID = "PAUSE";

void PauseState::s_pauseToMain()
{
    TheGame::Instance()->getStateMachine()->changeState(new MainMenuState());
}

void PauseState::s_resumePlay()
{
    TheGame::Instance()->getStateMachine()->popState();
}

void PauseState::update()
{
    for (unsigned i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->update();
    }
}

void PauseState::render()
{
    for(unsigned i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->draw();
    }
}

bool PauseState::onEnter()
{
    /*
    if (TheTextureManager::Instance()->load("assets/resume.png", "resumebutton",
        TheGame::Instance()->getRenderer()) == false)
    {
        return false;
    }

    if (TheTextureManager::Instance()->load("assets/main.png", "mainbutton",
        TheGame::Instance()->getRenderer()) == false)
    {
        return false;
    }

    GameObject* button1 = new MenuButton(new LoaderParams(200, 100, 200, 80, "mainbutton"), s_pauseToMain);
    GameObject* button2 = new MenuButton(new LoaderParams(200, 300, 200, 80, "resumebutton"), s_resumePlay);

    m_gameObjects.push_back(button1);
    m_gameObjects.push_back(button2);

    return true;
    */
    StateParser stateParser;
    stateParser.parseState("assets/test.xml", s_pauseID, &m_gameObjects, &m_textureIDList);

    m_callbacks.push_back(0);
    m_callbacks.push_back(s_pauseToMain);
    m_callbacks.push_back(s_resumePlay);

    setCallbacks(m_callbacks);
    return true;
}

void PauseState::setCallbacks(const std::vector<Callback>& callbacks)
{
    //go through the game objects
    for(unsigned i = 0; i < m_gameObjects.size(); i++) {
        if (dynamic_cast<MenuButton*>(m_gameObjects[i])) {
            MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
            pButton->setCallback(callbacks[pButton->getCallbackID()]);
        }
    }
}

bool PauseState::onExit()
{
    for (unsigned i = 0; i < m_gameObjects.size(); i++) {
        m_gameObjects[i]->clean();
    }

    m_gameObjects.clear();

    for (unsigned i = 0; i < m_textureIDList.size(); i++) {
        TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
    }

    TheInputHandler::Instance()->resetMouseButton();

    return true;
}
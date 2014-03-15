#include "MainMenuState.h"
#include "TextureManager.h"
#include "Game.h"
#include "MenuButton.h"
#include "PlayState.h"
#include "StateParser.h"
#include "InputHandler.h"

#include <iostream>

const std::string MainMenuState::s_menuID = "MENU";

void MainMenuState::update()
{
    if (m_menuObjects.empty() == false) {
        for (unsigned i = 0; i < m_menuObjects.size(); i++) {
            if (m_menuObjects[i] != 0) {
                m_menuObjects[i]->update();
            }
        }
    }
}

void MainMenuState::render()
{
    if (m_menuObjects.empty() == true) {
        return;
    }

    for (size_t i = 0; i < m_menuObjects.size(); i++) {
        m_menuObjects[i]->draw();
    }
}

bool MainMenuState::onEnter()
{
    /*
    std::cout << "MenuState onEnter." << std::endl;
    if (TheTextureManager::Instance()->load("assets/button.png",
        "playbutton", TheGame::Instance()->getRenderer()) == false)
    {
        std::cout << "load fail play button." << std::endl;
        return false;
    }

    if (TheTextureManager::Instance()->load("assets/exit.png",
        "exitbutton", TheGame::Instance()->getRenderer()) == false)
    {
        std::cout << "load fail exit button." << std::endl;
        return false;
    }

    GameObject* button1 = new MenuButton(new LoaderParams(100, 100, 400, 100, "playbutton"), s_menuToPlay);
    GameObject* button2 = new MenuButton(new LoaderParams(100, 300, 400, 100, "exitbutton"), s_exitFromMenu);

    m_menuObjects.push_back(button1);
    m_menuObjects.push_back(button2);
    std::cout << "MenuState onEnter return true." << std::endl;
    return true;
    */
    //parse the state
    StateParser stateParser;
    stateParser.parseState("assets/test.xml", s_menuID, &m_menuObjects, &m_textureIDList);

    m_callbacks.push_back(0);
    m_callbacks.push_back(s_menuToPlay);
    m_callbacks.push_back(s_exitFromMenu);

    setCallbacks(m_callbacks);
    std::cout << "entering menustate" << std::endl;
    return true;
}

void MainMenuState::setCallbacks(const std::vector<Callback>& callbacks)
{
    //go through the objects
    for (unsigned i = 0; i < m_menuObjects.size(); i++) {
        //if they are of type MenuButton then assign a callback
        //based on the id passed in from the list
        if (dynamic_cast<MenuButton*>(m_menuObjects[i])) {
            MenuButton* pButton = dynamic_cast<MenuButton*>(m_menuObjects[i]);
            pButton->setCallback(callbacks[pButton->getCallbackID()]);
        }
    }
}

bool MainMenuState::onExit()
{
    std::cout << "MenuState onExit." << std::endl;

    for (size_t i = 0; i < m_menuObjects.size(); i++) {
        m_menuObjects[i]->clean();
    }

    m_menuObjects.clear();

    for(unsigned i = 0; i < m_textureIDList.size(); i++) {
        TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
    }

    // reset the input handler
    TheInputHandler::Instance()->reset();
    std::cout << "menuState onExit, return true." << std::endl; 
    return true;
}

void MainMenuState::s_menuToPlay()
{
    TheGame::Instance()->getStateMachine()->changeState(new PlayState());
}

void MainMenuState::s_exitFromMenu()
{
    TheGame::Instance()->quit();
}

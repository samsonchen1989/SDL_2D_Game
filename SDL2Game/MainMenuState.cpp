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
    if(TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
    {
        s_menuToPlay();
    }

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
    if(m_loadingComplete && !m_menuObjects.empty())
    {
        for(int i = 0; i < m_menuObjects.size(); i++)
        {
            m_menuObjects[i]->draw();
        }
    }
}

bool MainMenuState::onEnter()
{
    // parse the state
    StateParser stateParser;
    stateParser.parseState("assets/conan.xml", s_menuID, &m_menuObjects, &m_textureIDList);

    m_callbacks.push_back(0);
    m_callbacks.push_back(s_menuToPlay);
    m_callbacks.push_back(s_exitFromMenu);

    // set the callbacks for menu items
    setCallbacks(m_callbacks);

    m_loadingComplete = true;
    std::cout << "entering MenuState\n";
    return true;
}

void MainMenuState::setCallbacks(const std::vector<Callback>& callbacks)
{
    // go through the game objects
    if(!m_menuObjects.empty())
    {
        for(int i = 0; i < m_menuObjects.size(); i++)
        {
            // if they are of type MenuButton then assign a callback based on the id passed in from the file
            if(dynamic_cast<MenuButton*>(m_menuObjects[i]))
            {
                MenuButton* pButton = dynamic_cast<MenuButton*>(m_menuObjects[i]);
                pButton->setCallback(callbacks[pButton->getCallbackID()]);
            }
        }
    }
}

bool MainMenuState::onExit()
{
    m_exiting = true;
    
    // clean the game objects
    if(m_loadingComplete && !m_menuObjects.empty())
    {
		m_menuObjects.back()->clean();
		m_menuObjects.pop_back();
    }

	m_menuObjects.clear();
    
    // reset the input handler
    TheInputHandler::Instance()->reset();
    
    std::cout << "exiting MenuState\n";
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

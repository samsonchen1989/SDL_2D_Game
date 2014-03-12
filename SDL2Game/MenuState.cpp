#include "MenuState.h"
#include "TextureManager.h"
#include "Game.h"
#include "MenuButton.h"
#include "PlayState.h"

#include <iostream>

const std::string MenuState::s_menuID = "MENU";

void MenuState::update()
{
    if (m_menuObjects.empty() == false) {
        for (int i = 0; i < m_menuObjects.size(); i++) {
            if (m_menuObjects[i] != 0) {
                m_menuObjects[i]->update();
            }
        }
    }
}

void MenuState::render()
{
    if (m_menuObjects.empty() == true) {
        return;
    }

    for (size_t i = 0; i < m_menuObjects.size(); i++) {
        m_menuObjects[i]->draw();
    }
}

bool MenuState::onEnter()
{
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
}

bool MenuState::onExit()
{
    std::cout << "MenuState onExit." << std::endl;

    for (size_t i = 0; i < m_menuObjects.size(); i++) {
        m_menuObjects[i]->clean();
    }

    m_menuObjects.clear();
    TheTextureManager::Instance()->clearFromTextureMap("playbutton");
    TheTextureManager::Instance()->clearFromTextureMap("exitbutton");

    std::cout << "menuState onExit, return true." << std::endl; 
    return true;
}

void MenuState::s_menuToPlay()
{
    TheGame::Instance()->getStateMachine()->changeState(new PlayState());
}

void MenuState::s_exitFromMenu()
{
    TheGame::Instance()->quit();
}

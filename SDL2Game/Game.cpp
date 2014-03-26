#include <iostream>

#include "Game.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "MainMenuState.h"
#include "GameObjectFactory.h"
#include "MenuButton.h"
#include "AnimatedGraphic.h"
#include "Player.h"
#include "ScrollingBackground.h"
#include "SoundManager.h"
#include "Snail.h"
#include "GameOverState.h"

using namespace std;

Game* Game::s_pInstance = 0;

Game::Game():
m_pWindow(0),
m_pRenderer(0),
m_bRunning(false),
m_pGameStateMachine(0),
m_playerLives(3),
m_scrollSpeed(0.8),
m_bLevelComplete(false),
m_bChangingState(false)
{
    // add some level files to an array
    m_levelFiles.push_back("assets/map1.tmx");

    // start at this level
    m_currentLevel = 1;
}

Game::~Game()
{
    clean();
}

GAME_STATUS_TAG Game::init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
    // store the game width and height
    m_gameWidth = width;
    m_gameHeight = height;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
        //if succeeded create our window
        m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        //If the window creation succeeded create our render
        if (m_pWindow != 0) {
            m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

            if (m_pRenderer != 0)
            {
                SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            } else {
                return GAME_ERROR_RENDERER_INIT_FAIL;
            }
        } else {
            return GAME_ERROR_WINDOW_INIT_FAIL;
        }
    } else {
        return GAME_ERROR_SDL_INIT_FAIL; //SDL could not initialize
    }

    m_bRunning = true;

    TheInputHandler::Instance()->initialiseJoysticks();

    // add some sound effects - TODO move to better place
    TheSoundManager::Instance()->load("assets/DST-Away.ogg", "music1", SOUND_MUSIC);
    TheSoundManager::Instance()->load("assets/jump.wav", "jump", SOUND_SFX);
    //TheSoundManager::Instance()->load("phaser.wav", "shoot", SOUND_SFX);

    TheSoundManager::Instance()->playMusic("music1", -1);


    TheInputHandler::Instance()->initialiseJoysticks();

    // register the types for the game
    TheGameObjectFactory::Instance()->registerType("MenuButton", new MenuButtonCreator());
    TheGameObjectFactory::Instance()->registerType("Player", new PlayerCreator());
    TheGameObjectFactory::Instance()->registerType("AnimatedGraphic", new AnimatedGraphicCreator());
    TheGameObjectFactory::Instance()->registerType("ScrollingBackground", new ScrollingBackgroundCreator());
    TheGameObjectFactory::Instance()->registerType("Snail", new SnailCreator());

    m_pGameStateMachine = new GameStateMachine();
    m_pGameStateMachine->changeState(new MainMenuState());

    return GAME_INIT_SUCCESS;
}

void Game::render()
{
    SDL_RenderClear(m_pRenderer);

    m_pGameStateMachine->render();

    //show the window
    SDL_RenderPresent(m_pRenderer);
}

void Game::clean()
{
    cout << "cleaning game\n";

    TheInputHandler::Instance()->clean();

    m_pGameStateMachine->clean();

    m_pGameStateMachine = 0;
    delete m_pGameStateMachine;

    TheTextureManager::Instance()->clearTextureMap();

    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    SDL_Quit();
}

void Game::quit()
{
    m_bRunning = false;
}

void Game::handleEvents()
{
    TheInputHandler::Instance()->update();
}

void Game::update()
{
    m_pGameStateMachine->update();
}

void Game::setCurrentLevel(int currentLevel)
{
    m_currentLevel = currentLevel;
    m_pGameStateMachine->changeState(new GameOverState());
    m_bLevelComplete = false;
}
#include <iostream>
#include <SDL_image.h>
#include "Game.h"
#include "TextureManager.h"
#include "Player.h"
#include "Enemy.h"
#include "InputHandler.h"
#include "MenuState.h"
#include "PlayState.h"

using namespace std;

Game* Game::s_pInstance = 0;

Game::Game():
    m_bRunning(false), m_pRenderer(0), m_pWindow(0) {}

Game::~Game()
{
    clean();
}

GAME_STATUS_TAG Game::init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
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

    m_pGameStateMachine = new GameStateMachine();
    m_pGameStateMachine->changeState(new MenuState());

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
    TheInputHandler::Instance()->clean();

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

    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN)) {
        m_pGameStateMachine->changeState(new PlayState());
    }
}

void Game::update()
{
    m_pGameStateMachine->update();
}
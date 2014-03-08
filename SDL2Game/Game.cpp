#include <iostream>
#include <SDL_image.h>
#include "Game.h"
#include "TextureManager.h"
#include "Player.h"
#include "Enemy.h"

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

    TextureManager::Instance()->load("assets/animate-alpha.png", "animate", m_pRenderer);

    m_gameObjects.push_back(new Player(new LoaderParams(100, 100, 128, 82, "animate")));
    m_gameObjects.push_back(new Enemy(new LoaderParams(300, 300, 128, 82, "animate")));

    return GAME_INIT_SUCCESS;
}

void Game::render()
{
    SDL_RenderClear(m_pRenderer);

    /*
    //"1" means first row, m_currentFrame means which list, animate img always
    //stores as two dimensional array.
    TextureManager::Instance()->drawFrame("animate", 100, 100, 128, 82, 1,
        m_currentFrame, m_pRenderer);
    */
    for (std::vector<GameObject*>::size_type i = 0; i != m_gameObjects.size(); i++) {
        m_gameObjects[i]->draw();
    }

    //show the window
    SDL_RenderPresent(m_pRenderer);
}

void Game::clean()
{
    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    SDL_Quit();
}

void Game::handleEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            m_bRunning = false;
            break;
        default:
            break;
        }
    }
}

void Game::update()
{
    for (std::vector<GameObject*>::size_type i =0; i != m_gameObjects.size(); i++) {
        m_gameObjects[i]->update();
    }
}
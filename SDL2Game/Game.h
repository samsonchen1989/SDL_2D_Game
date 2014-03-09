#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL.h>
#include "Log.h"
#include "GameObject.h"

class Game
{
public:
    static Game* Instance()
    {
        if (s_pInstance == 0) {
            s_pInstance = new Game();
            return s_pInstance;
        }

        return s_pInstance;
    }

    SDL_Renderer* getRenderer() const { return m_pRenderer; }

    void render();
    void update();
    void handleEvents();
    void quit();
    void clean();

    GAME_STATUS_TAG init(const char* title, int xpos, int ypos,
        int width, int height, int flags);

    bool running() { return m_bRunning; }

private:
    Game();
    virtual ~Game();
    static Game* s_pInstance;

    bool m_bRunning;
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    SDL_Texture* m_pTexture;

    std::vector<GameObject*> m_gameObjects;
};

typedef Game TheGame;

#endif
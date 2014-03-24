#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL.h>
#include "Log.h"
#include "GameObject.h"
#include "GameStateMachine.h"

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

    void render();
    void update();
    void handleEvents();
    void quit();
    void clean();

    GAME_STATUS_TAG init(const char* title, int xpos, int ypos,
        int width, int height, int flags);

    bool running() { return m_bRunning; }

    SDL_Renderer* getRenderer() const { return m_pRenderer; }

    GameStateMachine* getStateMachine() { return m_pGameStateMachine; }

    int getGameWidth() const { return m_gameWidth; }
    int getGameHeight() const { return m_gameHeight; }
    float getScrollSpeed() { return m_scrollSpeed; }

    void setPlayerLives(int lives) { m_playerLives = lives; }
    int getPlayerLives() { return m_playerLives; }

    void setCurrentLevel(int currentLevel);
    const int getCurrentLevel() { return m_currentLevel; }

    void setLevelComplete(bool levelComplete) { m_bLevelComplete = levelComplete; }
    const bool getLevelComplete() { return m_bLevelComplete; }

    std::vector<std::string> getLevelFiles() { return m_levelFiles; }

private:
    Game();
    virtual ~Game();
    static Game* s_pInstance;

    bool m_bRunning;
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    SDL_Texture* m_pTexture;

    int m_gameWidth;
    int m_gameHeight;
    float m_scrollSpeed;

    int m_playerLives;

    int m_currentLevel;

    bool m_bLevelComplete;

    bool m_bChangingState;

    std::vector<GameObject*> m_gameObjects;
    GameStateMachine* m_pGameStateMachine;

    std::vector<std::string> m_levelFiles;
};

typedef Game TheGame;

#endif
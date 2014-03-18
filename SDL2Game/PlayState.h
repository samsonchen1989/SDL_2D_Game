#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include <vector>
#include "GameState.h"

class SDLGameObject;
class GameObject;
class Level;

class PlayState : public GameState
{
public:
    virtual void update();
    virtual void render();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return s_playID; }

private:
    static const std::string s_playID;
    std::vector<GameObject*> m_gameObjects;

    Level* pLevel;

    bool checkCollision(SDLGameObject* p1, SDLGameObject* p2);
};

#endif
#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include <vector>
#include "GameState.h"
#include "GameObject.h"
#include "SDLGameObject.h"

class GameObject;

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

    bool checkCollision(SDLGameObject* p1, SDLGameObject* p2);
};

#endif
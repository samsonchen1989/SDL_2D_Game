#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include <vector>
#include "GameState.h"
#include "CollisionManager.h"

class SDLGameObject;
class GameObject;
class Level;

class PlayState : public GameState
{
public:
    virtual ~PlayState() { delete pLevel; }
    virtual void update();
    virtual void render();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return s_playID; }

private:
    CollisionManager m_collisionManager;
    static const std::string s_playID;
    std::vector<GameObject*> m_gameObjects;

    Level* pLevel;
};

#endif
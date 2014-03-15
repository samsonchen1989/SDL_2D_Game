#ifndef ENEMY_H
#define ENEMY_H

#include "SDLGameObject.h"
#include "GameObjectFactory.h"

class Enemy : public SDLGameObject
{
public:
    Enemy();

    virtual void draw();
    virtual void update();
    virtual void clean();
    virtual void load(const LoaderParams* pParams);
};

class EnemyCreator : public BaseCreator
{
    GameObject* createGameObject() const
    {
        return new Enemy();
    }
};

#endif
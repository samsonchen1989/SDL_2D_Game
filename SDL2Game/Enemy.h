#ifndef ENEMY_H
#define ENEMY_H

#include "PlatformerObject.h"
#include "GameObjectFactory.h"

class Enemy : public PlatformerObject
{
public:
    virtual std::string type() { return "Enemy"; }

protected:
    int m_health;

    Enemy() : PlatformerObject() {}
    virtual ~Enemy() {}
};

#endif
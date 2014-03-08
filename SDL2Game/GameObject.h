#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <SDL.h>

#include "LoaderParams.h"

//abstract base classes
class GameObject
{
public:
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void clean() = 0;

protected:
    GameObject(const LoaderParams* pParams) {}
    virtual ~GameObject() {}
};

#endif
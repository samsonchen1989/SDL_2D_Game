#ifndef ANIMATED_GRAPHIC_H
#define ANIMATED_GRAPHIC_H

#include "PlatformerObject.h"
#include "GameObjectFactory.h"

class AnimatedGraphic : public PlatformerObject
{
public:
    AnimatedGraphic();

    virtual void draw();
    virtual void update();
    virtual void clean();
    //remember the const, or subclass will create a
    //new function but not inherit super function
    virtual void load(std::unique_ptr<LoaderParams> const &pParams);

protected:
    int m_animSpeed;
};

class AnimatedGraphicCreator : public BaseCreator
{
public:
    GameObject* createGameObject() const
    {
        return new AnimatedGraphic();
    }
};

#endif
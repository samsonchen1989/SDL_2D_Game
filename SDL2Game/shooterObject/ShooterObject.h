#ifndef SDL_GAME_OBJECT_H
#define SDL_GAME_OBJECT_H

#include "../GameObject.h"
#include "../Vector2D.h"

class ShooterObject : public GameObject
{
public:
    virtual ~ShooterObject() {}
    virtual void draw();
    virtual void update();
    virtual void load(std::unique_ptr<LoaderParams> const &pParams);
    
    virtual void clean() {} //not implemented in this class
    virtual void collision() {} //not implemented in this class
    virtual std::string type() { return "SDLGameObject"; }

protected:
    ShooterObject();
    
    //draw the animation for the object being destroyed
    void doDyingAnimation();

    //how fast will this object being destroyed
    int m_bulletFiringSpeed;
    int m_bulletCounter;

    //how fast will the object move?
    int m_moveSpeed;

    //how long will the death animation takes? with a counter
    int m_dyingTime;
    int m_dyingCounter;

    //has the explosion sound player?
    bool m_bPlayedDeathSound;
};

#endif
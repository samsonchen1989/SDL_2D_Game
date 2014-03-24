#ifndef PLAYER_H
#define PLAYER_H

#include "ShooterObject.h"
#include "GameObjectFactory.h"

class Player : public ShooterObject
{
public:

    Player();

    virtual void draw();
    virtual void update();
    virtual void clean();
    virtual void load(std::unique_ptr<LoaderParams> const &pParams);

    //when get hit
    virtual void collision();

    virtual std::string type() { return "Player"; }

private:
    // handle any input from the keyboard, mouse, or joystick
    void handleInput();

    //bring the player back if there are lives left
    void ressurect();

    //handle any animation for the player
    void handleAnimation();

    //player can invulnerable for a time
    int m_invulnerable;
    int m_invulnerableTime;
    int m_invulnerableCounter;
};

class PlayerCreator : public BaseCreator
{
    GameObject* createGameObject() const
    {
        return new Player();
    }
};

#endif
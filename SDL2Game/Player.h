#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include "PlatformerObject.h"
#include "GameObjectFactory.h"

class Player : public PlatformerObject
{
public:

    Player();
    virtual ~Player() {}

    virtual void load(std::unique_ptr<LoaderParams> const &pParams);

    virtual void draw();
    virtual void update();
    virtual void clean();

    virtual void collision();

    virtual std::string type() { return "Player"; }

private:

    // bring the player back if there are lives left
    void ressurect();

    // handle any input from the keyboard, mouse, or joystick
    void handleInput();

    // handle any animation for the player
    void handleAnimation();

    void handleMovement(Vector2D velocity);

    // player can be invulnerable for a time
    int m_bInvulnerable;
    int m_invulnerableTime;
    int m_invulnerableCounter;

    bool m_bPressedJump;
    bool m_bJumpFalling;
    int m_gravity;
};

// for the factory
class PlayerCreator : public BaseCreator
{
    GameObject* createGameObject() const
    {
        return new Player();
    }
};

#endif
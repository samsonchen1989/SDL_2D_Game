#include "Player.h"
#include "InputHandler.h"
#include <iostream>

Player::Player(const LoaderParams* pParams) :
    SDLGameObject(pParams)
{
}

void Player::draw()
{
    SDLGameObject::draw();
}

void Player::update()
{
    m_velocity.setX(0);
    m_velocity.setY(0);

    handleInput();

    m_currentFrame = int ((SDL_GetTicks() / 100) % 5);
    SDLGameObject::update();
}

void Player::clean()
{
}

void Player::handleInput()
{
    if (TheInputHandler::Instance()->joystickInitialised()) { 
        if (TheInputHandler::Instance()->xvalue(0, 1) > 0 ||
            TheInputHandler::Instance()->xvalue(0, 1) < 0) {
            m_velocity.setX(1.0f * TheInputHandler::Instance()->xvalue(0, 1));
        }

        if (TheInputHandler::Instance()->yvalue(0, 1) > 0 ||
            TheInputHandler::Instance()->yvalue(0, 1) < 0) {
            m_velocity.setY(1.0f * TheInputHandler::Instance()->yvalue(0, 1));
        }
    } else {
        if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT)) {
            m_velocity.setX(2);
        } else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT)) {
            m_velocity.setX(-2);
        } else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP)) {
            //when move up, m_velocity set to negative value
            m_velocity.setY(-2);
        } else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN)) {
            m_velocity.setY(2);
        }
    }
}
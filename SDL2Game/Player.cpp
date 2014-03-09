#include "Player.h"
#include "InputHandler.h"

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

    m_currentFrame = int ((SDL_GetTicks() / 100) % 6);

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

        if (TheInputHandler::Instance()->xvalue(0, 2) > 0 ||
            TheInputHandler::Instance()->xvalue(0, 2) < 0) {
            m_velocity.setX(1.0f * TheInputHandler::Instance()->xvalue(0, 2));
        }

        if (TheInputHandler::Instance()->yvalue(0, 2) > 0 ||
            TheInputHandler::Instance()->yvalue(0, 2) < 0) {
            m_velocity.setY(1.0f * TheInputHandler::Instance()->yvalue(0, 2));
        }
    }

    Vector2D* vec = TheInputHandler::Instance()->getMousePosition();
    m_velocity = (*vec - m_position) / 100;
}
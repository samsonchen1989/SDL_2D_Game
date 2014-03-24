#include <iostream>

#include "Player.h"
#include "Game.h"
#include "InputHandler.h"
#include "TileLayer.h"
#include "BulletHandler.h"
#include "SoundManager.h"

Player::Player() : ShooterObject(),
m_invulnerable(false),
m_invulnerableTime(200),
m_invulnerableCounter(0)
{
}

void Player::collision()
{
    // if the player is not invulnerable then set to dying and change values for death animation tile sheet
    if(!m_invulnerable && !TheGame::Instance()->getLevelComplete())
    {
        m_textureID = "largeexplosion";
        m_currentFrame = 0;
        m_numFrames = 9;
        m_width = 60;
        m_height = 60;
        m_bDying = true;
    }
}

void Player::draw()
{
    ShooterObject::draw();
}

void Player::update()
{
    //if the level is complete then fly off the screen
    if (TheGame::Instance()->getLevelComplete() == true) {
        if (m_position.getX() >= TheGame::Instance()->getGameWidth()) {
            TheGame::Instance()->setCurrentLevel(TheGame::Instance()->getCurrentLevel() + 1);
        } else {
            m_velocity.setY(0);
            m_velocity.setX(3);
            ShooterObject::update();
            handleAnimation();
        }
    } else {
        //if the player is not doing its death animation then update it normally
        if (m_bDying == false) {
            //reset velocity
            m_velocity.setX(0);
            m_velocity.setY(0);

            //get input
            handleInput();
            // do normal position += velocity update
            ShooterObject::update();
            // update the animation
            handleAnimation();
        } else {
            m_currentFrame = static_cast<int>((SDL_GetTicks() / 100) % m_numFrames);
            //if the death animation has completed
            if (m_dyingCounter == m_dyingTime) {
                //ressurect the player
                ressurect();
            }

            m_dyingCounter++;
        }
    }
}

void Player::clean()
{
    ShooterObject::clean();
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
            m_velocity.setX(m_moveSpeed);
        } else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT)) {
            m_velocity.setX(-m_moveSpeed);
        } else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP)) {
            //when move up, m_velocity set to negative value
            m_velocity.setY(-m_moveSpeed);
        } else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN)) {
            m_velocity.setY(m_moveSpeed);
        }

        if(TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
        {
            if(m_bulletCounter == m_bulletFiringSpeed)
            {
                TheSoundManager::Instance()->playSound("shoot", 0);
                TheBulletHandler::Instance()->addPlayerBullet(m_position.getX() + 90, m_position.getY() + 12, 11, 11, "bullet1", 1, Vector2D(10,0));
                m_bulletCounter = 0;
            }

            m_bulletCounter++;
        }
        else
        {
            m_bulletCounter = m_bulletFiringSpeed;
        }
    }
}

void Player::load(std::unique_ptr<LoaderParams> const &pParams)
{
    ShooterObject::load(pParams);

    // can set up the players inherited values here

    // set up bullets
    m_bulletFiringSpeed = 13;
    m_moveSpeed = 3;

    // we want to be able to fire instantly
    m_bulletCounter = m_bulletFiringSpeed;

    // time it takes for death explosion
    m_dyingTime = 100;
}

void Player::ressurect()
{
    TheGame::Instance()->setPlayerLives(TheGame::Instance()->getPlayerLives() - 1);

    m_position.setX(10);
    m_position.setY(200);
    m_bDying = false;

    m_textureID = "player";

    m_currentFrame = 0;
    m_numFrames = 5;
    m_width = 101;
    m_height = 46;

    m_dyingCounter = 0;
    m_invulnerable = true;
}

void Player::handleAnimation()
{
    // if the player is invulnerable we can flash its alpha to let people know
    if (m_invulnerable == true) {
        //invulnerability is finished, set values back
        if (m_invulnerableCounter == m_invulnerableTime) {
            m_invulnerable = false;
            m_invulnerableCounter = 0;
            m_alpha = 255;
        } else { //otherwise, flash the alpha on and off
            if (m_alpha == 255) {
                m_alpha = 0;
            } else {
                m_alpha = 255;
            }
        }

        m_invulnerableCounter++;
    }

    //if the player is not dead then we can change the angle with the velocity to
    //give the impression of a moving helicopter
    if (m_bDead == false) {
        if (m_velocity.getX() < 0) {
            m_alpha = -10.0;
        } else if (m_velocity.getX() > 0) {
            m_angle = 10.0;
        } else {
            m_angle = 0.0;
        }
    }

    //our standard animation code - for helicopter propellors
    m_currentFrame = static_cast<int>((SDL_GetTicks() / 100) % m_numFrames);
}
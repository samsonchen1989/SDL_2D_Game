#include "SDLGameObject.h"
#include "Game.h"
#include "TextureManager.h"

SDLGameObject::SDLGameObject(const LoaderParams* pParams) :
    GameObject(pParams),
    m_position(static_cast<float>(pParams->getX()),
        static_cast<float>(pParams->getY())),
    m_velocity(0, 0),
    m_acceleration(0, 0)
{
    m_width = pParams->getWidth();
    m_height = pParams->getheight();
    m_textureID = pParams->getTextureID();

    m_currentRow = 1;
    m_currentFrame = 1;
}

void SDLGameObject::draw()
{
    if (m_velocity.getX() > 0) {
        TextureManager::Instance()->drawFrame(m_textureID, (int)m_position.getX(),
            (int)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame,
            Game::Instance()->getRenderer(), SDL_FLIP_HORIZONTAL);
    } else {
        TextureManager::Instance()->drawFrame(m_textureID, (int)m_position.getX(),
            (int)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame,
            Game::Instance()->getRenderer());
    }
}

void SDLGameObject::update()
{
    m_velocity += m_acceleration;
    m_position += m_velocity;
}

void SDLGameObject::clean()
{

}
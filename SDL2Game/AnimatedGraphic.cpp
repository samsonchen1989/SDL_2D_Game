#include <iostream>
#include "AnimatedGraphic.h"

AnimatedGraphic::AnimatedGraphic() : SDLGameObject()
{
}

void AnimatedGraphic::update()
{
    std::cout << "update, m_animSpeed:" << m_animSpeed << std::endl;
    m_currentFrame = int((SDL_GetTicks() / (1000 / m_animSpeed)) % m_numFrames);
}

void AnimatedGraphic::draw()
{
    SDLGameObject::draw();
}

void AnimatedGraphic::clean()
{
    SDLGameObject::clean();
}

void AnimatedGraphic::load(const LoaderParams* pParams)
{
    SDLGameObject::load(pParams);
    m_animSpeed = pParams->getAnimSpeed();
    m_numFrames = pParams->getNumFrames();
    std::cout << "load, m_animSpeed:" << m_animSpeed << std::endl;
}
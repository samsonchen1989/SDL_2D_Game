#include <iostream>
#include "AnimatedGraphic.h"

AnimatedGraphic::AnimatedGraphic() : PlatformerObject()
{
}

void AnimatedGraphic::update()
{
    std::cout << "update, m_animSpeed:" << m_animSpeed << std::endl;
    m_currentFrame = int((SDL_GetTicks() / (1000 / m_animSpeed)) % m_numFrames);
}

void AnimatedGraphic::draw()
{
    PlatformerObject::draw();
}

void AnimatedGraphic::clean()
{
    PlatformerObject::clean();
}

void AnimatedGraphic::load(std::unique_ptr<LoaderParams> const &pParams)
{
    PlatformerObject::load(pParams);
    m_animSpeed = pParams->getAnimSpeed();
    m_numFrames = pParams->getNumFrames();
    std::cout << "load, m_animSpeed:" << m_animSpeed << std::endl;
}
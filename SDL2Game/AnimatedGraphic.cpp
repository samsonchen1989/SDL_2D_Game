#include "AnimatedGraphic.h"

AnimatedGraphic::AnimatedGraphic(LoaderParams* pParams, int animSpeed) :
    SDLGameObject(pParams), m_animSpeed(animSpeed)
{
}

void AnimatedGraphic::update()
{
    m_currentFrame = int((SDL_GetTicks() / (1000 / m_animSpeed)) % 5);
}

void AnimatedGraphic::draw()
{
    SDLGameObject::draw();
}

void AnimatedGraphic::clean()
{
    SDLGameObject::clean();
}
#ifndef ANIMATED_GRAPHIC_H
#define ANIMATED_GRAPHIC_H

#include "SDLGameObject.h"

class AnimatedGraphic : public SDLGameObject
{
public:
    AnimatedGraphic(LoaderParams* pParams, int animSpeed);
    virtual ~AnimatedGraphic() {}

    virtual void draw();
    virtual void update();
    virtual void clean();

protected:
    int m_animSpeed;
};

#endif
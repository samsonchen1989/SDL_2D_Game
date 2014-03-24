#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include "ShooterObject.h"
#include "GameObjectFactory.h"

class MenuButton : public ShooterObject
{
public:

    MenuButton();

    virtual void draw();
    virtual void update();
    virtual void clean();
    virtual void load(std::unique_ptr<LoaderParams> const &pParams);

    void setCallback(void(*callback)()) { m_callback = callback; }
    int getCallbackID() { return m_callbackID; }

private:
    enum button_state
    {
        MOUSE_OUT = 0,
        MOUSE_OVER = 1,
        CLICKED = 2
    };

    int m_callbackID;
    void (*m_callback)();
    bool m_bReleased;
};

class MenuButtonCreator : public BaseCreator
{
    GameObject* createGameObject() const
    {
        return new MenuButton();
    }
};

#endif
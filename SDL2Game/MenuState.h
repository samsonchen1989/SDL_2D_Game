#ifndef MENU_STATE
#define MENU_STATE

#include <vector>
#include "GameState.h"

class MenuState : public GameState
{
protected:
    typedef void(*Callback)();
    virtual void setCallbacks(const std::vector<Callback>& callbacks) = 0;

    std::vector<Callback> m_callbacks;
};

#endif
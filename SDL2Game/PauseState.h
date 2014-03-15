#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include <vector>
#include "GameState.h"
#include "GameObject.h"
#include "MenuState.h"

class PauseState : public MenuState
{
public:
    virtual void update();
    virtual void render();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return s_pauseID; }

private:
    static void s_pauseToMain();
    static void s_resumePlay();
    virtual void setCallbacks(const std::vector<Callback>& callbacks);

    static const std::string s_pauseID;
    std::vector<GameObject*> m_gameObjects;
};

#endif
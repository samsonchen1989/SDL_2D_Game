#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <vector>
#include <SDL.h>

#include "Vector2D.h"

enum mouse_buttons
{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

class InputHandler
{
public:
    static InputHandler* Instance()
    {
        if (s_pInstance == 0) {
            s_pInstance = new InputHandler();
        }

        return s_pInstance;
    }

    void update();
    void clean();
    void reset();

    void initialiseJoysticks();
    bool joystickInitialised() {
        return m_bJoysticksInitialised;
    }

    int xvalue(int joy, int stick);
    int yvalue(int joy, int stick);

    bool getButtonState(int joy, int buttonNumber) {
        return m_buttonStates[joy][buttonNumber];
    }

    bool getMouseButtonState(int buttonNumber) {
        return m_mouseButtonStates[buttonNumber];
    }

    Vector2D* getMousePosition() {
        return m_mousePosition;
    }

    bool isKeyDown(SDL_Scancode key);

    void resetMouseButton();

private:
    InputHandler();
    ~InputHandler();

    //handle keyboard event
    void onKeyDown();
    void onKeyUp();

    //handle mouse events
    void onMouseMove(SDL_Event& event);
    void onMouseButtonDown(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);

    //handle joystick events
    void onJoystickAxisMove(SDL_Event& event);
    void onJoystickButtonDown(SDL_Event& event);
    void onJoystickButtonUp(SDL_Event& event);

    static const int m_joystickDeadZone = 10000;
    static InputHandler* s_pInstance;
    bool m_bJoysticksInitialised;
    std::vector<SDL_Joystick*> m_joysticks;
    //use vector to store whether joystick has moved up/down/left/right
    //stick 1 for left stick, 2 for right stick
    std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues;
    std::vector<std::vector<bool>> m_buttonStates;
    std::vector<bool> m_mouseButtonStates;
    Vector2D* m_mousePosition;
    const Uint8* m_keystate;
};

typedef InputHandler TheInputHandler;

#endif
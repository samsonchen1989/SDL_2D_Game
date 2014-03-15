#include <iostream>
#include "InputHandler.h"
#include "Game.h"

InputHandler* InputHandler::s_pInstance = 0;

InputHandler::InputHandler() :
    m_bJoysticksInitialised(false),
    m_mousePosition(new Vector2D(0, 0)),
    m_keystate(0)
{
    for (int i = 0; i < 3; i++) {
        m_mouseButtonStates.push_back(false);
    }
}

InputHandler::~InputHandler()
{
    delete m_mousePosition;
    delete m_keystate;

    m_joystickValues.clear();
    m_joysticks.clear();
    m_mouseButtonStates.clear();
}

void InputHandler::initialiseJoysticks()
{
    if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) {
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    }

    if (SDL_NumJoysticks() > 0) {
        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            SDL_Joystick* joy = SDL_JoystickOpen(i);
            if (SDL_JoystickOpen(i)) {
                std::cout << "name:" << SDL_JoystickName(joy) << std::endl;
                std::cout << "axes:" << SDL_JoystickNumAxes(joy) << std::endl;
                std::cout << "button:" << SDL_JoystickNumButtons(joy) << std::endl;

                m_joysticks.push_back(joy);
                m_joystickValues.push_back(std::make_pair(new Vector2D(0, 0),
                    new Vector2D(0, 0)));

                std::vector<bool> tempButtons;
                for (int i = 0; i < SDL_JoystickNumButtons(joy); i++) {
                    tempButtons.push_back(false);
                }

                m_buttonStates.push_back(tempButtons);
            } else {
                std::cout << SDL_GetError();
            }
        }

        SDL_JoystickEventState(SDL_ENABLE);
        std::cout << "Initialise JoyStick number:" << SDL_NumJoysticks();
        m_bJoysticksInitialised = true;
    } else {
        m_bJoysticksInitialised = false;
    }
}

void InputHandler::reset()
{
    m_mouseButtonStates[LEFT] = false;
    m_mouseButtonStates[RIGHT] = false;
    m_mouseButtonStates[MIDDLE] = false;
}

void InputHandler::clean()
{
    if (m_bJoysticksInitialised) {
        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            SDL_JoystickClose(m_joysticks[i]);
        }
    }
}

int InputHandler::xvalue(int joy, int stick)
{
    if (m_joystickValues.size() > 0) {
        if (stick == 1) {
            return static_cast<int>(m_joystickValues[joy].first->getX());
        } else if (stick == 2) {
            return static_cast<int>(m_joystickValues[joy].second->getX());
        }
    }

    return 0;
}

int InputHandler::yvalue(int joy, int stick)
{
    if (m_joystickValues.size() > 0) {
        if (stick == 1) {
            return static_cast<int>(m_joystickValues[joy].first->getY());
        } else if (stick == 2) {
            return static_cast<int>(m_joystickValues[joy].second->getY());
        }
    }

    return 0;
}

bool InputHandler::isKeyDown(SDL_Scancode key)
{
    if (m_keystate != 0) {
        if (m_keystate[key] == 1) {
            return true;
        } else {
            return false;
        }
    }

    return false;
}

void InputHandler::update()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            TheGame::Instance()->quit();
            break;
        case SDL_JOYAXISMOTION:
            onJoystickAxisMove(event);
            break;
        case SDL_JOYBUTTONDOWN:
            onJoystickButtonDown(event);
            break;
        case SDL_JOYBUTTONUP:
            onJoystickButtonUp(event);
            break;
        case SDL_MOUSEBUTTONDOWN:
            onMouseButtonDown(event);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseButtonUp(event);
            break;
        case SDL_MOUSEMOTION:
            onMouseMove(event);
            break;
        case SDL_KEYDOWN:
            onKeyDown();
            break;
        case SDL_KEYUP:
            onKeyUp();
            break;
        default:
            break;
        }
    }
}

void InputHandler::onJoystickAxisMove(SDL_Event& event)
{
    int whichOne = event.jaxis.which;
    int axis = static_cast<int>(event.jaxis.axis);
    int value = static_cast<int>(event.jaxis.value);
    std::cout << "jaxis:" << event.jaxis.which << std::endl;
    std::cout << "axis:" << axis << std::endl;
    std::cout << "value:" << value << std::endl;
    //left stick move left or right
    if (axis == 0) {
        if (value > m_joystickDeadZone) {
            m_joystickValues[whichOne].first->setX(1);
        } else if (value < -m_joystickDeadZone) {
            m_joystickValues[whichOne].first->setX(-1);
        } else {
            m_joystickValues[whichOne].first->setX(0);
        }
    }

    //left stick move up or down
    if (axis == 1) {
        if (value > m_joystickDeadZone) {
            m_joystickValues[whichOne].first->setY(1);
        } else if (value < -m_joystickDeadZone) {
            m_joystickValues[whichOne].first->setY(-1);
        } else {
            m_joystickValues[whichOne].first->setY(0);
        }
    }

    //right stick move left or right
    if (axis == 3) {
        if (value > m_joystickDeadZone) {
            m_joystickValues[whichOne].second->setX(1);
        } else if (value < -m_joystickDeadZone) {
            m_joystickValues[whichOne].second->setX(-1);
        } else {
            m_joystickValues[whichOne].first->setX(0);
        }
    }

    //right stick move up or down
    if (axis == 4) {
        if (value > m_joystickDeadZone) {
            m_joystickValues[whichOne].second->setY(1);
        } else if (value < -m_joystickDeadZone) {
            m_joystickValues[whichOne].second->setY(-1);
        } else {
            m_joystickValues[whichOne].second->setY(0);
        }
    }
}

void InputHandler::onJoystickButtonDown(SDL_Event& event)
{
    m_buttonStates[event.jaxis.which][event.jbutton.button] = true;
}

void InputHandler::onJoystickButtonUp(SDL_Event& event)
{
    m_buttonStates[event.jaxis.which][event.jbutton.button] = false;
}

void InputHandler::onMouseButtonDown(SDL_Event& event)
{
    if (event.button.button == SDL_BUTTON_LEFT) {
        m_mouseButtonStates[LEFT] = true;
    }

    if (event.button.button == SDL_BUTTON_MIDDLE) {
        m_mouseButtonStates[MIDDLE] = true;
    }

    if (event.button.button == SDL_BUTTON_RIGHT) {
        m_mouseButtonStates[RIGHT] = true;
    }
}

void InputHandler::onMouseButtonUp(SDL_Event& event)
{
    if (event.button.button == SDL_BUTTON_LEFT) {
        m_mouseButtonStates[LEFT] = false;
    }

    if (event.button.button == SDL_BUTTON_MIDDLE) {
        m_mouseButtonStates[MIDDLE] = false;
    }

    if (event.button.button == SDL_BUTTON_RIGHT) {
        m_mouseButtonStates[RIGHT] = false;
    }
}

void InputHandler::onMouseMove(SDL_Event& event)
{
    m_mousePosition->setX(static_cast<float>(event.motion.x));
    m_mousePosition->setY(static_cast<float>(event.motion.y));
}

void InputHandler::resetMouseButton()
{
    m_mouseButtonStates[LEFT] = false;
    m_mouseButtonStates[RIGHT] = false;
    m_mouseButtonStates[MIDDLE] = false;
}

void InputHandler::onKeyDown()
{
    m_keystate = SDL_GetKeyboardState(0);
}

void InputHandler::onKeyUp()
{
    m_keystate = SDL_GetKeyboardState(0);
}
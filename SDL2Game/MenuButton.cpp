#include "MenuButton.h"
#include "InputHandler.h"

MenuButton::MenuButton() : PlatformerObject()
{

}

void MenuButton::draw()
{

    PlatformerObject::draw();
}

void MenuButton::update()
{
    Vector2D* pMousePos = TheInputHandler::Instance()->getMousePosition();

    if (pMousePos->getX() < (m_position.getX() + m_width)
        && pMousePos->getX() > m_position.getX()
        && pMousePos->getY() < (m_position.getY() + m_height)
        && pMousePos->getY() > m_position.getY())
    {
        if (TheInputHandler::Instance()->getMouseButtonState(LEFT) == true && m_bReleased) {
            m_currentFrame = CLICKED;
            m_callback();
            m_bReleased = false;
        } else if (TheInputHandler::Instance()->getMouseButtonState(LEFT) == false) {
            m_bReleased = true;
            m_currentFrame = MOUSE_OVER;
        }
    } else {
        m_currentFrame = MOUSE_OUT;
    }
}

void MenuButton::clean()
{
    PlatformerObject::clean();
}

void MenuButton::load(std::unique_ptr<LoaderParams> const &pParams)
{
    std::cout << "menubutton load" << std::endl;
    PlatformerObject::load(pParams);
    m_callbackID = pParams->getCallbackID();
    m_currentFrame = MOUSE_OUT;
}
#include "player.h"
#include <algorithm>

bool BreakoutPlayer::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_LEFT && brickPos.x >= -10.0)
    {
        setPos(brickPos + Ogre::Vector3(-1, 0, 0));
    }
    else if (evt.keysym.sym == OgreBites::SDLK_RIGHT && brickPos.x <= 10.0)
    {
        setPos(brickPos + Ogre::Vector3(1, 0, 0));
    }

    return true;
}

bool BreakoutPlayer::axisMoved(const OgreBites::AxisEvent& evt)
{
    // JoyStick min/max is -32768 / 32767
    if (evt.axis == 0)
    {
        xSpeed = evt.value / 32768.0;
    }

    return true;
}

bool BreakoutPlayer::frameStarted(const Ogre::FrameEvent& evt)
{
    if (xSpeed < 0)
    {
        setPos(Ogre::Vector3(std::max(-10.0f, brickPos.x + xSpeed), brickPos.y, brickPos.z));
    }
    else
    {
        setPos(Ogre::Vector3(std::min(10.0f, brickPos.x + xSpeed), brickPos.y, brickPos.z));
    }
    return true;
}
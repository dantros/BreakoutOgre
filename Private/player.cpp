#include "player.h"

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
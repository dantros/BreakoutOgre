#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "brick.h"

// Player class used to move the lower brick
class BreakoutPlayer : public BreakoutBrick , public OgreBites::InputListener
{
public:
    BreakoutPlayer(Ogre::SceneNode* playerNode) : BreakoutBrick(playerNode, 0)
    {
        mBreakable = false;
    }
    virtual ~BreakoutPlayer() {}

    bool keyPressed(const OgreBites::KeyboardEvent& evt);
};
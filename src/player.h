#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "brick.h"

// Player class used to move the lower brick
class BreakoutPlayer : public BreakoutBrick , public OgreBites::InputListener, public Ogre::FrameListener
{
public:
    BreakoutPlayer(Ogre::SceneNode* playerNode) : BreakoutBrick(playerNode, 0)
    {
        mBreakable = false;
    }
    virtual ~BreakoutPlayer() {}

    bool keyPressed(const OgreBites::KeyboardEvent& evt);
    bool axisMoved(const OgreBites::AxisEvent& evt);
    bool frameStarted(const Ogre::FrameEvent& evt); // Called every frame

private:
    float xSpeed = 0.0;
};
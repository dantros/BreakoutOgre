#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "brick.h"

// FrameListener is an interface that allows us to make changes to the scene every frame
class BreakoutBall : public Ogre::FrameListener
{
public:
    BreakoutBall(Ogre::SceneNode* ballNode, BreakoutBrick* bricks[], int numBricks) : mNode(ballNode), mBricks(new BreakoutBrick*[numBricks]), mNumBricks(numBricks)
    {
        for (int i = 0; i < numBricks; i++)
        {
            mBricks[i] = bricks[i];
        }
    }

    virtual ~BreakoutBall()
    {
        delete[] mBricks;
    }

    bool frameStarted(const Ogre::FrameEvent& evt); // Called every frame
    void setDirection(const Ogre::Vector3& dir); // Set the direction of the ball
private:
    Ogre::SceneNode* mNode;
    BreakoutBrick** mBricks;
    int mNumBricks;
    Ogre::Vector3 mDirection;
};
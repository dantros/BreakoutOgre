#include "ball.h"

bool BreakoutBall::frameStarted(const Ogre::FrameEvent& evt)
{
    // Moving the ball
    Ogre::Vector3 newPos = mNode->getPosition() + mDirection * evt.timeSinceLastFrame;

    for (int i = 0; i < mNumBricks; i++)
    {
        if (mBricks[i]->isColliding(newPos))
        {
            Ogre::Vector3 normal = mBricks[i]->getBounce(mNode->getPosition(), mDirection);
            // mDirection = mDirection.reflect(normal);
            mDirection = normal;
            mBricks[i]->breakBrick();
            break;
        }
    }

    mNode->translate(mDirection * evt.timeSinceLastFrame);
    return true;
}

void BreakoutBall::setDirection(const Ogre::Vector3& dir)
{
    mDirection = dir;
}
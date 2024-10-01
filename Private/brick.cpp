#include "brick.h"
#include <cmath>

void BreakoutBrick::setup(float width, float height)
{
    mWidth = width;
    mHeight = height;
}

void BreakoutBrick::setPos(const Ogre::Vector3& pos)
{
    mNode->setPosition(pos);
    brickPos = pos;
}

bool BreakoutBrick::isColliding(const Ogre::Vector3& ballPos)
{
    // Only if the brick is visible
    if (mBroken)
    {
        return false;
    }
    // Check if the ball is within the bounds of the brick
    if (ballPos.x >= brickPos.x - mWidth / 2 && ballPos.x <= brickPos.x + mWidth / 2 &&
        ballPos.y >= brickPos.y - mHeight / 2 && ballPos.y <= brickPos.y + mHeight / 2)
    {
        return true;
    }

    return false;
}

// Given the ball's position and velocity, return the normal of the brick
// The ball's position is outside of the brick but on the next frame it would enter it
Ogre::Vector3 BreakoutBrick::getBounce(const Ogre::Vector3& ballPos, const Ogre::Vector3& ballVel)
{
    Ogre::Vector3 normal = Ogre::Vector3::ZERO;

    if (mType == 0)
    {
        // Brick bounds
        float left = brickPos.x - mWidth / 2;
        float right = brickPos.x + mWidth / 2;
        float top = brickPos.y + mHeight / 2;
        float bottom = brickPos.y - mHeight / 2;

        // Get the closest edge
        float dxLeft = std::abs(ballPos.x - left);
        float dxRight = std::abs(ballPos.x - right);
        float dyTop = std::abs(ballPos.y - top);
        float dyBottom = std::abs(ballPos.y - bottom);

        // If the ball collides with the left or right edge
        if (dxLeft <= dxRight && dxLeft <= dyTop && dxLeft <= dyBottom)
        {
            normal.x = -std::abs(ballVel.x);
            normal.y = ballVel.y;
        }
        else if (dxRight <= dxLeft && dxRight <= dyTop && dxRight <= dyBottom)
        {
            normal.x = std::abs(ballVel.x);
            normal.y = ballVel.y;
        }

        // If the ball collides with the top or bottom edge
        else if (dyTop <= dyBottom && dyTop <= dxLeft && dyTop <= dxRight)
        {
            normal.y = std::abs(ballVel.y);
            normal.x = ballVel.x;
        }
        else
        {
            normal.y = -std::abs(ballVel.y);
            normal.x = ballVel.x;
        }
    }
    else if (mType == 1)
    {
        // Left wall, just set the x-velocity to be positive
        normal.x = std::abs(ballVel.x);
        normal.y = ballVel.y;
    }
    else if (mType == 2)
    {
        // Right wall, just set the x-velocity to be negative
        normal.x = -std::abs(ballVel.x);
        normal.y = ballVel.y;
    }
    else if (mType == 3)
    {
        // Top wall, just set the y-velocity to be negative
        normal.x = ballVel.x;
        normal.y = -std::abs(ballVel.y);
    }

    return normal;
}


void BreakoutBrick::breakBrick()
{
    if (mType == 0 && !mBroken && mBreakable)
    {
        mNode->setVisible(false);
        mBroken = true;
    }
}
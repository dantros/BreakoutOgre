#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"

// Brick class used to change the ball's direction
class BreakoutBrick 
{
public:
    BreakoutBrick(Ogre::SceneNode* brickNode, int type = 0) : mNode(brickNode), mType(type) {}
    virtual ~BreakoutBrick() {}

    void setup(float width, float height);
    void setPos(const Ogre::Vector3& pos);
    bool isColliding(const Ogre::Vector3& ballPos);
    Ogre::Vector3 getBounce(const Ogre::Vector3& ballPos, const Ogre::Vector3& ballVel);
    void breakBrick();
private:
    Ogre::SceneNode* mNode;
    Ogre::Vector3 brickPos;
    float mWidth;
    float mHeight;
    int mType;
    bool mBroken = false;
    bool mBreakable = true;

    friend class BreakoutPlayer;
};
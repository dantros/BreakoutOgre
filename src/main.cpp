#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreFileSystemLayer.h>
//#include <iostream>
#include "ball.h"
#include "brick.h"
#include "player.h"

class Breakout : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
    Breakout();
    virtual ~Breakout() {}

    void setup();
    bool keyPressed(const OgreBites::KeyboardEvent& evt);
private:
    int backgroundMaterialId = 0;
    std::vector<Ogre::String> backgrounds;
    Ogre::Entity* backgroundEntity;
};

Breakout::Breakout() : OgreBites::ApplicationContext("Breakout")
{
}

void Breakout::setup()
{
    // Calling the base first, adding the input listener
    // This will create a file ogre.cfg at your OS user folder/Breakout. Example: My Documents/Breakout.
    // that file contains your initial setup, such as window size, full screen, render backend, etc.
    // This is the way OgreBites work, if we do not want this, we need to implement something equivalent
    // to OgreBites.
    // *Important Note*: OgreBites was only meant to be used with Ogre examples, so it is very incomplete
    // for general purposes. Example: At the time of writting, joystick analog inputs are not properly
    // detected (all axis are showing the same event parameters for some reason).
    OgreBites::ApplicationContext::setup();

    // Adding an input listener
    addInputListener(this);

    // Pointer to the default root
    Ogre::Root* root = getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();
    scnMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));

    // Use Ogre's custom shadow mapping ability
    Ogre::MaterialManager* materialMgr = Ogre::MaterialManager::getSingletonPtr();
    scnMgr->setShadowTexturePixelFormat(Ogre::PF_DEPTH16);
    // only MODULATIVE technique is working for some reason.
    //scnMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_TEXTURE_ADDITIVE);
    //scnMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
    scnMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_TEXTURE_MODULATIVE);
    //scnMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED);

    // default is only 1 shadow map
    scnMgr->setShadowTextureCount(5);
    scnMgr->setShadowTextureCasterMaterial(materialMgr->getByName("PSSM/shadow_caster"));
    scnMgr->setShadowTextureReceiverMaterial(materialMgr->getByName("Ogre/DepthShadowmap/Receiver/RockWall"));
    // for shadow mapping, selkf shadowing is not implemented by default.
    scnMgr->setShadowTextureSelfShadow(false);
    scnMgr->setShadowTextureSize(1024);

    // Registering the scene with the RTShaderSystem
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

//#if 0
    // A light source is needed
    Ogre::Light* light = scnMgr->createLight("MainLight");
    light->setType(Ogre::Light::LightTypes::LT_POINT);
    light->setCastShadows(true);
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setDirection(0,1,-1);
    lightNode->setPosition(0, 0, 20);
    lightNode->attachObject(light);
//#endif

    // Setting up the camera
    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5);
    cam->setAutoAspectRatio(false);
    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 40);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
    camNode->attachObject(cam);

    // Setting up the viewport
    getRenderWindow()->addViewport(cam);

    // 20 bricks + 3 walls + paddle
    Ogre::Entity* bricks[24];
    Ogre::SceneNode* brickNodes[24];
    BreakoutBrick* bricksActors[24];

    std::vector<Ogre::String> meshMaterials{
        "Basics/Red",
        "Basics/Green",
        "Basics/Blue",
        "Basics/Yellow",
        "Basics/Purple",
        "Basics/Aqua"
    };

    backgrounds = {
        "Advanced/ScrollingClouds",
        "Advanced/ScrollingColor",
        "Basics/Gray",
        "Basics/Black",
        "Advanced/MagicChrome",
        "Advanced/UglyWater",
        "Advanced/Rocks"
    };

    // Loop to create the entities and scene nodes
    for (int i = 0; i < 20; i++)
    {
        bricks[i] = scnMgr->createEntity("Brick" + Ogre::StringConverter::toString(i), "brick.obj");
        bricks[i]->getSubEntity(0)->setMaterialName(meshMaterials[i % meshMaterials.size()]);
        bricks[i]->setCastShadows(true); // true is the default anyways.
        brickNodes[i] = scnMgr->getRootSceneNode()->createChildSceneNode("Brick" + Ogre::StringConverter::toString(i) + "Node");
        bricksActors[i] = new BreakoutBrick(brickNodes[i]);
        bricksActors[i]->setup(7.5, 2.0);
        bricksActors[i]->setPos(Ogre::Vector3(-15.0 + 7.5 * (i % 5), 15.0 - 2.0 * (i / 5), 0));
        brickNodes[i]->yaw(Ogre::Degree(90));
        brickNodes[i]->attachObject(bricks[i]);
    }

    // Now we setup the walls, they are just invisible bricks!
    brickNodes[20] = scnMgr->getRootSceneNode()->createChildSceneNode("LeftWallNode");
    bricksActors[20] = new BreakoutBrick(brickNodes[20], 1);
    bricksActors[20]->setup(2.0, 30.0);
    bricksActors[20]->setPos(Ogre::Vector3(-17.0, 0, 0));

    brickNodes[21] = scnMgr->getRootSceneNode()->createChildSceneNode("RightWallNode");
    bricksActors[21] = new BreakoutBrick(brickNodes[21], 2);
    bricksActors[21]->setup(2.0, 30.0);
    bricksActors[21]->setPos(Ogre::Vector3(17.0, 0, 0));

    brickNodes[22] = scnMgr->getRootSceneNode()->createChildSceneNode("TopWallNode");
    bricksActors[22] = new BreakoutBrick(brickNodes[22], 3);
    bricksActors[22]->setup(60.0, 2.0);
    bricksActors[22]->setPos(Ogre::Vector3(0, 17.0, 0));

    // Setting up the player brick
    bricks[23] = scnMgr->createEntity("Player", "brick.obj");
    bricks[23]->getSubEntity(0)->setMaterialName("Advanced/UnlitWhite");
    brickNodes[23] = scnMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
    BreakoutPlayer* player = new BreakoutPlayer(brickNodes[23]);
    bricksActors[23] = player;
    bricksActors[23]->setup(7.5, 2.0);
    bricksActors[23]->setPos(Ogre::Vector3(0, -11.0, 0));
    brickNodes[23]->yaw(Ogre::Degree(90));
    brickNodes[23]->attachObject(bricks[23]);

    // Background
    backgroundEntity = scnMgr->createEntity("Background", "quad.obj");
    backgroundEntity->getSubEntity(0)->setMaterialName("Advanced/ScrollingClouds");
    backgroundEntity->setCastShadows(false);
    Ogre::SceneNode* backgroundNode = scnMgr->getRootSceneNode()->createChildSceneNode("BackgroundNode");
    backgroundNode->setPosition(Ogre::Vector3(0, 0, -5));
    backgroundNode->setOrientation(Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3(1,0,0)));
    backgroundNode->setScale(2.5,1,2.5);
    backgroundNode->attachObject(backgroundEntity);

    // Create the BreakoutPlayer and attach it to the root
    addInputListener(player);
    root->addFrameListener(player);

    // Create a sphere entity and node
    Ogre::Entity* sphereEntity = scnMgr->createEntity("Sphere", "ball.obj");
    sphereEntity->getSubEntity(0)->setMaterialName("Advanced/EnvMappedRustySteel");
    Ogre::SceneNode* sphereNode = scnMgr->getRootSceneNode()->createChildSceneNode("SphereNode");
    sphereNode->setPosition(0, -9.0, 0);
    sphereNode->yaw(Ogre::Degree(90));
    sphereNode->attachObject(sphereEntity);

//#if 0
    // second light source, not all combinations work, no error, so be sure it is working when modifying it.
    Ogre::Light* light2 = scnMgr->createLight("SecondaryLight");
    light2->setType(Ogre::Light::LightTypes::LT_POINT);
    light2->setCastShadows(true);
    light2->setAttenuation(50.0, 0.5, 0.0001, 0.0);
    Ogre::SceneNode* light2Node = scnMgr->getRootSceneNode()->createChildSceneNode();
    //Ogre::SceneNode* light2Node = brickNodes[23]->createChildSceneNode();
    light2Node->setDirection(0, 0, -1);
    light2Node->setPosition(10, 0, 20);
    light2Node->attachObject(light2);
//#endif

    // Create the BreakoutBall and attach it to the root
    BreakoutBall* breakoutBall = new BreakoutBall(sphereNode, bricksActors, 24);
    root->addFrameListener(breakoutBall);

    // Optionally set the direction
    breakoutBall->setDirection(Ogre::Vector3(7, 7, 0)); // Move along the y-axis
}

bool Breakout::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_SPACE)
    {
        backgroundMaterialId = backgroundMaterialId + 1 < backgrounds.size() ? backgroundMaterialId + 1 : 0;
        backgroundEntity->getSubEntity(0)->setMaterialName(backgrounds.at(backgroundMaterialId));
    }
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    return true;
}

int main(int argc, char **argv)
{
    try
    {
        Breakout app;
        app.initApp();
        app.getRoot()->startRendering();
        app.closeApp();
    }
    catch (Ogre::Exception& e)
    {
        Ogre::LogManager::getSingleton().logMessage(e.what());
    }
    return 0;
}
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
};

Breakout::Breakout() : OgreBites::ApplicationContext("Breakout")
{
}

void Breakout::setup()
{
    // Calling the base first, adding the input listener
    OgreBites::ApplicationContext::setup();

    addInputListener(this);

    // Pointer to the default root
    Ogre::Root* root = getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();
    scnMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));

    // Use Ogre's custom shadow mapping ability
    Ogre::MaterialManager* materialMgr = Ogre::MaterialManager::getSingletonPtr();
    scnMgr->setShadowTexturePixelFormat(Ogre::PF_DEPTH16);
    scnMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_TEXTURE_ADDITIVE);
    //scnMgr->setShadowTextureCasterMaterial(materialMgr->getByName("PSSM/shadow_caster"));
    scnMgr->setShadowTextureReceiverMaterial(materialMgr->getByName("Ogre/DepthShadowmap/Receiver/RockWall"));
    scnMgr->setShadowTextureSelfShadow(true);
    scnMgr->setShadowTextureSize(512);

    // Registering the scene with the RTShaderSystem
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // A light source is needed
    Ogre::Light* light = scnMgr->createLight("MainLight");
    light->setType(Ogre::Light::LightTypes::LT_POINT);
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setDirection(0,1,-1);
    lightNode->setPosition(0, 0, 20);
    lightNode->attachObject(light);

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

    // Array of 20 entities
    Ogre::Entity* bricks[22];
    // Array of 20 scene nodes
    Ogre::SceneNode* brickNodes[25];
    // Array of 20 bricks
    BreakoutBrick* bricksActors[25];
    // Array of mesh names
    std::vector<Ogre::String> meshMaterials{ "Basics/Black", "Basics/Green", "Basics/Blue", "Basics/Yellow", "Basics/Purple", "Advanced/ScrollingClouds" };

    // Loop to create the entities and scene nodes
    for (int i = 0; i < 20; i++)
    {
        bricks[i] = scnMgr->createEntity("Brick" + Ogre::StringConverter::toString(i), "brick.obj");
        bricks[i]->getSubEntity(0)->setMaterialName(meshMaterials[i % meshMaterials.size()]);
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
    bricks[20] = scnMgr->createEntity("Player", "brick.obj");
    bricks[20]->getSubEntity(0)->setMaterialName("Advanced/UnlitWhite");
    brickNodes[23] = scnMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
    BreakoutPlayer* player = new BreakoutPlayer(brickNodes[23]);
    bricksActors[23] = player;
    bricksActors[23]->setup(7.5, 2.0);
    bricksActors[23]->setPos(Ogre::Vector3(0, -11.0, 0));
    brickNodes[23]->yaw(Ogre::Degree(90));
    brickNodes[23]->attachObject(bricks[20]);

    // Background
    bricks[21] = scnMgr->createEntity("Background", "quad.obj");
    bricks[21]->getSubEntity(0)->setMaterialName("Advanced/ScrollingColor");
    brickNodes[24] = scnMgr->getRootSceneNode()->createChildSceneNode("BackgroundNode");
    bricksActors[24] = new BreakoutBrick(brickNodes[24], 1);
    bricksActors[24]->setup(7.5, 2.0);
    bricksActors[24]->setPos(Ogre::Vector3(0, 0, -5));
    brickNodes[24]->setOrientation(Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3(1,0,0)));
    brickNodes[24]->setScale(3,1,3);
    brickNodes[24]->attachObject(bricks[21]);

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

    // Create the BreakoutBall and attach it to the root
    BreakoutBall* breakoutBall = new BreakoutBall(sphereNode, bricksActors, 24);
    root->addFrameListener(breakoutBall);

    // Optionally set the direction
    breakoutBall->setDirection(Ogre::Vector3(7, 7, 0)); // Move along the y-axis
}

bool Breakout::keyPressed(const OgreBites::KeyboardEvent& evt)
{
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
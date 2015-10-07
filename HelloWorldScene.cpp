#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

/*Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}*/

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);
    
    
    //PiecesLayer* BGI_Layer = PiecesLayer::create();
    //ReverseLayer* BGI_Layer = ReverseLayer::create();
    //GravityLayer* BGI_Layer = GravityLayer::create();
    //BlacknWhiteLayer* BGI_Layer = BlacknWhiteLayer::create();
    //BackgroundLayer* BGI_Layer = BackgroundLayer::create();
    //PortalLayer* BGI_Layer = PortalLayer::create();
    //ReverseLayer* BGI_Layer = ReverseLayer::create();
    //GrowingBrickLayer* BGI_Layer = GrowingBrickLayer::create();
    //ContraLayer* BGI_Layer = ContraLayer::create();
    //ContraWithOutBossLayer* BGI_Layer = ContraWithOutBossLayer::create();
    //NoisyLayer* BGI_Layer = NoisyLayer::create();
    
    //ElasticDefenseLayer* BGI_Layer = ElasticDefenseLayer::create();
    //BlackholeLayer* BGI_Layer = BlackholeLayer::create();
    DemoLayer* BGI_Layer = DemoLayer::create();
    rootNode->addChild(BGI_Layer,1,1);
    
    //Sprite* bg = (Sprite*)rootNode->getChildByTag(10);
    //bg->setOpacity(0);
    //t_spFontGround->runAction(MoveBy::create(1.0f, Vec2(100, 0)));
    
    

    return true;
}

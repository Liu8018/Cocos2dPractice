#include "VizScene.h"
#include "globals.h"

#include <ui/CocosGUI.h>

USING_NS_CC;

Scene* VizScene::createScene()
{
    return VizScene::create();
}

bool VizScene::init()
{
    //确保super类先init
    if(!Scene::init())
        return false;
    
    //
    g_vizScene = this;
    g_vizItem = &m_vizItem;
    g_world = &m_world;
    
    //窗口尺寸
    m_vizSize = Director::getInstance()->getVisibleSize();
    
    //原点坐标
    m_origin = Director::getInstance()->getVisibleOrigin();
    
    //地图相关
    m_mapCacheHalfW = 48;
    m_mapCacheHalfH = 36;
    loadMap();
    
    //玩家图像
    addChild(m_mainPlayer.getSprite(),2);
    
    //行走相关
    m_isMapMoving = false;
    m_isMovingU = false;
    m_isMovingL = false;
    m_isMovingD = false;
    m_isMovingR = false;
    
    //键盘事件监视
    EventListenerKeyboard* keyBoardListener = EventListenerKeyboard::create();
    keyBoardListener->onKeyPressed = CC_CALLBACK_2(VizScene::onKeyPressed, this);
    keyBoardListener->onKeyReleased = CC_CALLBACK_2(VizScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
    
    //玩家菜单
    addChild(m_playerMenu.getMenu(),99);
    
    //定时器-刷新世界信息
    //schedule(CC_SCHEDULE_SELECTOR(TestScene::updateWorldData),m_turnTimeLen);
    
    //定时器-刷新画面（调用update()）
    scheduleUpdate();
    
    return true;
}

void VizScene::loadMap()
{
    //释放掉之前的map
    if(m_map != nullptr){
        removeChild(m_map);
    }
    
    //从world载入map
    m_map = TMXTiledMap::createWithXML(m_world.getLocalMapTmx(m_mapCacheHalfW,m_mapCacheHalfH),g_dir_mapRes);
    m_map->setScale(Director::getInstance()->getContentScaleFactor());

    //把人物位置放在视图中心
    m_map->setPositionX(m_origin.x-m_map->getMapSize().width*g_mapTileW/2+m_vizSize.width/2);
    m_map->setPositionY(m_origin.y-m_map->getMapSize().height*g_mapTileH/2+m_vizSize.height/2);
    
    addChild(m_map,0);
    
    //更改人物与map的相对坐标
    m_playerPosX_local = m_mapCacheHalfW+1;
    m_playerPosY_local = m_mapCacheHalfH+1;
}

bool VizScene::isTimeToLoadMap()
{
    int diffT = 0;
    int diffB = 0;
    int diffL = 0;
    int diffR = 0;
    
    int cacheW = m_vizSize.width/8;
    int cacheH = m_vizSize.height/8;
    
    int mapH = m_map->getMapSize().height*m_map->getTileSize().height;
    int mapW = m_map->getMapSize().width*m_map->getTileSize().width;
    
    diffT = (m_map->getPositionY()+mapH)-m_vizSize.height;
    if(diffT < cacheH)
        return true;
    
    diffB = -m_map->getPositionY();
    if(diffB < cacheH)
        return true;
    
    diffL = -m_map->getPositionX();
    if(diffL < cacheW)
        return true;
    
    diffR = (m_map->getPositionX()+mapW)-m_vizSize.width;
    if(diffR < cacheW)
        return true;
    
    return false;
}

void VizScene::MoveMap(int dX, int dY)
{
    if(m_isMapMoving)
        return;
    
    m_isMapMoving = true;
    
    //注意runAction单独分了一个线程
    
    //播放行走动画
    m_mainPlayer.walk(-dX);
    
    //播放地图移动动画
    int preBlockX = (m_map->getPositionX()+g_mapTileW/2)/g_mapTileW;
    int preBlockY = (m_map->getPositionY()+g_mapTileH/2)/g_mapTileH;
    int nextBlockX = (preBlockX+dX)*g_mapTileW-g_mapTileW/2;
    int nextBlockY = (preBlockY+dY)*g_mapTileH-g_mapTileH/2;
    MoveTo* move = MoveTo::create(g_turnTimeLen,Vec2(nextBlockX,nextBlockY));
    m_map->runAction(Sequence::create(move,CallFunc::create(([this]() { 
        m_isMapMoving = false; 
        if(isTimeToLoadMap())
            loadMap();
    })), nullptr));
    
    //更新world
    m_world.playerMove(0,-dX,dY);
    
    //更改人物与map的相对坐标
    m_playerPosX_local += -dX;
    m_playerPosY_local += -dY;
}

void VizScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    using KeyCode = EventKeyboard::KeyCode;
    
    switch (keyCode) {
    
    //方向键
    case KeyCode::KEY_UP_ARROW:
    case KeyCode::KEY_W: 
        m_isMovingU = true; break;
    case KeyCode::KEY_LEFT_ARROW:
    case KeyCode::KEY_A: 
        m_isMovingL = true; break;
    case KeyCode::KEY_DOWN_ARROW:
    case KeyCode::KEY_S: 
        m_isMovingD = true; break;
    case KeyCode::KEY_RIGHT_ARROW:
    case KeyCode::KEY_D: 
        m_isMovingR = true; break;
        
    default:break;
    }
}

void VizScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    using KeyCode = EventKeyboard::KeyCode;
    
    switch (keyCode) {
    
    //方向键
    case KeyCode::KEY_UP_ARROW:
    case KeyCode::KEY_W: 
        m_isMovingU = false; break;
    case KeyCode::KEY_LEFT_ARROW:
    case KeyCode::KEY_A: 
        m_isMovingL = false; break;
    case KeyCode::KEY_DOWN_ARROW:
    case KeyCode::KEY_S: 
        m_isMovingD = false; break;
    case KeyCode::KEY_RIGHT_ARROW:
    case KeyCode::KEY_D: 
        m_isMovingR = false; break;
        
    default:break;
    }
}

void VizScene::update(float)
{
    if(m_isMovingU)
        MoveMap(0,-1);
    else if(m_isMovingL)
        MoveMap(1,0);
    else if(m_isMovingD)
        MoveMap(0,1);
    else if(m_isMovingR)
        MoveMap(-1,0);
}

void VizScene::updateWorldData(float)
{
    
}

void VizScene::placeItem(std::pair<std::uint16_t, std::uint8_t> item)
{
    if(item.second < 1){
        log("[error] try to place item with 0 num");
        exit(1);
    }
    
    //从VizItem获取item图像
    auto itemSprite = Sprite::createWithSpriteFrame(g_vizItem->getItemFrame(item.first));
    //map增加子节点
    m_map->addChild(itemSprite);
    //计算放在map的哪个位置
    itemSprite->setPosition(m_playerPosX_local*g_mapTileW-g_mapTileW/2,
                            m_playerPosY_local*g_mapTileH
                            +itemSprite->getTextureRect().size.height/2-g_mapTileH);
}

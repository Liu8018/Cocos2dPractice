#ifndef VIZSCENE_H
#define VIZSCENE_H

#include "cocos2d.h"

#include "World.h"
#include "VizPlayer.h"
#include "VizPlayerMenu.h"
#include "VizItem.h"

class VizScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(VizScene);
    
    //放置item
    void placeItem(std::pair<std::uint16_t,std::uint8_t> item);
    
private:
    //
    World m_world;
    
    //
    cocos2d::Size m_vizSize;
    cocos2d::Vec2 m_origin;
    
    //地图
    cocos2d::TMXTiledMap* m_map;
    void MoveMap(int dX, int dY);
    bool m_isMapMoving;
    bool m_isMovingU;
    bool m_isMovingL;
    bool m_isMovingD;
    bool m_isMovingR;
    void loadMap();
    bool isTimeToLoadMap();
    
    int m_mapCacheHalfW;
    int m_mapCacheHalfH;
    
    //玩家数据
    VizPlayer m_mainPlayer;
    int m_playerPosX_local;
    int m_playerPosY_local;
    
    //按键响应
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    //刷新世界信息
    void updateWorldData(float);
    
    //刷新画面
    void update(float);
    
    //玩家菜单
    VizPlayerMenu m_playerMenu;
    
    //
    VizItem m_vizItem;
    
};

#endif // VIZSCENE_H

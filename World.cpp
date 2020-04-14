#include "World.h"
#include "globals.h"

#include <fstream>
#include <sstream>
#include <iostream>

void getLimitIndex(int limitW, int limitH, int infX, int infY, int &limitX, int &limitY)
{
    while (infX < 0) {
        infX += limitW;
    }
    
    while (infY < 0) {
        infY += limitH;
    }
    
    limitX = infX%limitW;
    limitY = infY%limitH;
}

World::World()
{
    //全地图大小，及人物初始位置
    m_w = g_mapWholeSize;
    m_h = g_mapWholeSize;
    m_playerPosX = 40000;
    m_playerPosY = 50000;
    
    //test
    m_playerItemList.push_back(std::make_pair(0,1));
    m_playerItemList.push_back(std::make_pair(1,1));
    m_playerItemList.push_back(std::make_pair(2,1));
    m_playerItemList.push_back(std::make_pair(3,1));
    m_playerItemList.push_back(std::make_pair(4,1));
    m_playerItemList.push_back(std::make_pair(5,1));
    m_playerItemList.push_back(std::make_pair(6,1));
    m_playerItemList.push_back(std::make_pair(7,1));
    m_playerItemList.push_back(std::make_pair(8,1));
    m_playerItemList.push_back(std::make_pair(9,1));
    m_playerItemList.push_back(std::make_pair(10,1));
    m_playerItemList.push_back(std::make_pair(11,1));
    m_playerItemList.push_back(std::make_pair(12,1));
    m_playerItemList.push_back(std::make_pair(13,1));
    m_playerItemList.push_back(std::make_pair(14,1));
    m_playerItemList.push_back(std::make_pair(15,1));
    m_playerItemList.push_back(std::make_pair(16,1));
    m_playerItemList.push_back(std::make_pair(17,1));
    m_playerItemList.push_back(std::make_pair(18,1));
    m_playerItemList.push_back(std::make_pair(19,1));
    m_playerItemList.push_back(std::make_pair(20,1));
    m_playerItemList.push_back(std::make_pair(21,1));
    m_playerItemList.push_back(std::make_pair(22,1));
    m_playerItemList.push_back(std::make_pair(23,1));
    m_playerItemList.push_back(std::make_pair(24,1));
    m_playerItemList.push_back(std::make_pair(25,1));
    m_playerItemList.push_back(std::make_pair(26,1));
    m_playerItemList.push_back(std::make_pair(27,1));
    m_playerItemList.push_back(std::make_pair(28,1));
    m_playerItemList.push_back(std::make_pair(29,1));
    m_playerItemList.push_back(std::make_pair(30,1));
    m_playerItemList.push_back(std::make_pair(31,1));
}

void World::playerMove(int playerId, int dX, int dY)
{
    //改变全局坐标
    m_playerPosX += dX;
    m_playerPosY += dY;
    int limitX,limitY;
    getLimitIndex(m_w,m_h,m_playerPosX,m_playerPosY,limitX,limitY);
    m_playerPosX = limitX;
    m_playerPosY = limitY;
    
    //debug 输出当前坐标
    std::cout<<"current position: "<<m_playerPosX<<","<<m_playerPosY<<std::endl;
}

std::string World::getLocalMapTmx(int halfW, int halfH)
{
    std::string tmxString;
    
    std::vector<std::vector<std::vector<std::pair<uint16_t, uint8_t>>>> itemMap;
    m_mapGener.generate(m_playerPosX,m_playerPosY,halfW,halfH,tmxString,itemMap);
    
    //printf("%s", tmxString.data());
    
    return tmxString;
}

void World::getPlayerCoordToLocalMap(int &x, int &y)
{
    
}

void World::getPlayerBackpack(int playerId, std::vector<std::pair<uint16_t, uint8_t> > &itemIdList)
{
    if(m_playerItemList.empty()){
        itemIdList.clear();
        return;
    }
    itemIdList.assign(m_playerItemList.begin(),m_playerItemList.end());
}

void World::dropItem(int playerId, int playerItemId, int num)
{
    if(m_playerItemList.empty())
        return;
    
    //背包
    std::uint8_t* p_itemNum = &(m_playerItemList[playerItemId].second);
    if(*p_itemNum > num){
        *p_itemNum -= num;
    }
    else{
        m_playerItemList.erase(m_playerItemList.begin()+playerItemId);
    }
    
    //地图!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //m_itemMap
}

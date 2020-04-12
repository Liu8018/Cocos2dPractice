#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>

#include "MapGenerator.h"

class World
{
public:
    World();
    
    void playerMove(int playerId, int dX, int dY);
    
    std::string getLocalMapTmx(int halfW, int halfH);
    void getPlayerCoordToLocalMap(int &x, int &y);
    
    void getPlayerBackpack(int playerId, std::vector<std::pair<std::uint16_t,std::uint8_t>> &itemIdList);
    
    void dropItem(int playerId, int playerItemId, int num);
    
private:
    int m_w;
    int m_h;
    
    int m_playerPosX;
    int m_playerPosY;
    
    //地形图
    std::vector<std::vector<int>> m_atlas;
    
    //全地图的item信息
    unsigned char* m_itemMap;
    
    //
    MapGenerator m_mapGener;
    
    //
    std::vector<std::pair<std::uint16_t,std::uint8_t>> m_playerItemList;
    
};

#endif // WORLD_H

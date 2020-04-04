#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>

class World
{
public:
    World();
    
    void playerMove(int dX, int dY);
    
    int getWidth();
    int getHeight();
    
    std::string getLocalMap(int halfW, int halfH);
    
private:
    int m_w;
    int m_h;
    
    int m_playerPosX;
    int m_playerPosY;
    
    //地形图
    std::vector<std::vector<int>> m_atlas;
    
    //全地图的item信息
    unsigned char* m_itemMap;
    
    //个体
    
};

#endif // WORLD_H

#include "World.h"

#include <fstream>
#include <sstream>
#include <iostream>

void loadAtlas(std::string csvPath, int &w, int &h, std::vector<std::vector<int>> &atlas)
{
    std::ifstream ifs(csvPath,std::ios::in);
    if(!ifs.is_open()){
        printf("failed to open %s\n",csvPath.data());
        exit(1);
    }
    
    atlas.clear();
    
    std::string line;
    while(getline(ifs,line)){
        //去除换行符
        if(line[line.length()-1] == '\n')
            line.pop_back();
        
        //去除最后一个逗号
        if(line[line.length()-1] == ',')
            line.pop_back();
        
        std::vector<int> tileVec;
        while(1){
            int commaPos = line.find_first_of(",");
            if(commaPos == std::string::npos){
                break;
            }
            int tileId = atoi(line.substr(0,commaPos).data());
            tileVec.push_back(tileId);
            
            line = line.substr(commaPos+1,line.length());
        }
        tileVec.push_back(atoi(line.data()));
        atlas.push_back(tileVec);
    }
    
    ifs.close();
    
    w = atlas[0].size();
    h = atlas.size();
}

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
    //加载地形图数据
    loadAtlas("Resources/map/atlas1.csv",m_w,m_h,m_atlas);
    
    //
    m_playerPosX = m_w/2;
    m_playerPosY = m_h/2;
    
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
}

std::string World::getLocalMapTmx(int halfW, int halfH)
{
    //
    std::vector<std::vector<int>> localMap;
    
    int startX = m_playerPosX - halfW;
    int endX = m_playerPosX + halfW;
    int startY = m_playerPosY - halfH;
    int endY = m_playerPosY + halfH;
    
    for(int y=startY;y<=endY;y++){
        std::vector<int> tileIdVec;
        for(int x=startX;x<=endX;x++){
            int limitX,limitY;
            getLimitIndex(m_w,m_h,x,y,limitX,limitY);
            
            
            //debug
            if(limitX == m_playerPosX && limitY == m_playerPosY){
                tileIdVec.push_back(7);
                continue;
            }
            
            
            tileIdVec.push_back(m_atlas[limitY][limitX]);
        }
        localMap.push_back(tileIdVec);
    }
    
    //
    std::string tmxString = "";
    int localMapW = localMap[0].size();
    int localMapH = localMap.size();
    
    tmxString += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    tmxString += "<map version=\"1.2\" tiledversion=\"1.3.3\" orientation=\"orthogonal\" renderorder=\"right-down\" width=\""+std::to_string(localMapW)+
            "\" height=\""+std::to_string(localMapH)+"\" tilewidth=\"32\" tileheight=\"32\" infinite=\"0\" nextlayerid=\"2\" nextobjectid=\"1\">\n";
    tmxString += "<tileset firstgid=\"1\" source=\"atlas1.tsx\"/>\n";
    tmxString += "<layer id=\"1\" name=\"layer0\" width=\""+std::to_string(localMapW)+"\" height=\""+std::to_string(localMapH)+"\">\n";
    tmxString += "<data encoding=\"csv\">\n";
    
    for(int y=0;y<localMap.size();y++){
        for(int x=0;x<localMap[y].size();x++){
            tmxString += std::to_string(localMap[y][x]) + ",";
        }
    }
    tmxString.pop_back();
    
    tmxString += "</data>\n</layer>\n</map>";
    
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

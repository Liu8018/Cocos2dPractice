#include "MapGenerator.h"
#include "globals.h"

#include <iostream>

MapCache::MapCache()
{
    m_size = 0;
    m_maxSize = 10;
    
    m_first = (mapCacheList*)malloc(sizeof (mapCacheList));
    m_first->next = nullptr;
}

void MapCache::push(const std::pair<int, int> &blockId, 
                    const std::vector<std::vector<int> > &naturalMapBlock, 
                    const std::vector<std::vector<std::vector<std::pair<uint16_t, uint8_t>>>> &itemMapBlock)
{
    mapCacheList* cptr = m_first;
    
    while (cptr->next != nullptr) {
        cptr = cptr->next;
    }
    
    //末尾添加一个元素
    cptr->next = (mapCacheList*)malloc(sizeof (mapCacheList));
    cptr->next->blockId = blockId;
    for(int y=0;y<naturalMapBlock.size();y++){
        for(int x=0;x<naturalMapBlock[y].size();x++){
            cptr->next->naturalMapBlock[y][x] = naturalMapBlock[y][x];
        }
    }
    for(int y=0;y<itemMapBlock.size();y++){
        for(int x=0;x<itemMapBlock[y].size();x++){
            for(int z=0;z<5;z++){
                if(itemMapBlock[y][x].size() > z){
                    cptr->next->itemMapBlock[y][x][z] = itemMapBlock[y][x][z];
                }
                else{
                    cptr->next->itemMapBlock[y][x][z] = std::make_pair(0,0);
                }
            }
        }
    }
    cptr->next->next = nullptr;
    
    if(m_size < m_maxSize){
        m_size++;
    }
    else{
        //释放掉第一个元素
        mapCacheList* tmpPtr = m_first->next;
        m_first->next = m_first->next->next;
        free(tmpPtr);
    }
}

bool MapCache::get(const std::pair<int, int> &blockId, 
                   std::vector<std::vector<int> > &naturalMapBlock, 
                   std::vector<std::vector<std::vector<std::pair<uint16_t, uint8_t>>>> &itemMapBlock)
{
    mapCacheList* cptr = m_first->next;
    
    while (cptr != nullptr) {
        if(cptr->blockId.first == blockId.first &&
           cptr->blockId.second == blockId.second)
        {
            naturalMapBlock.resize(200);
            itemMapBlock.resize(200);
            for(int i=0;i<200;i++){
                naturalMapBlock[i].resize(200);
                itemMapBlock[i].resize(200);
            }
            for(int i=0;i<200;i++){
                for(int j=0;j<200;j++){
                    itemMapBlock[i][j].resize(5);
                }
            }
            
            for(int y=0;y<200;y++){
                for(int x=0;x<200;x++){
                    naturalMapBlock[y][x] = cptr->naturalMapBlock[y][x];
                    
                    for(int z=0;z<5;z++)
                        itemMapBlock[y][x][z] = cptr->itemMapBlock[y][x][z];
                }
            }
            
            return true;
        }
        
        cptr = cptr->next;
    }
    
    return false;
}


//--------------------------------------------------------------------------------
MapGenerator::MapGenerator()
{
    m_wholeSize = g_mapWholeSize;
    m_fan_radius = m_wholeSize/10;
    m_fan2xiu_w = m_fan_radius/10;
    
    m_randomSeed = 123;
    
    m_blockSize = 200;//不能乱改，跟缓存相关的
    m_perlinK_fan = 20;
    m_perlinK_xiu = 2000;
}

void MapGenerator::mySrand(unsigned int r)
{
    m_randomVar = 1103515245*r + 12345;
}

unsigned int MapGenerator::myRand()
{
    m_randomVar = 1103515245*m_randomVar + 12345;
    
    return m_randomVar;
}

float fade(float v)
{
    return v*v*v*(6*v*v-15*v+10);
}

float MapGenerator::PerlinNoise2d(int randomSeed, float x, float y)
{
    int x0 = int(x);
    int x1 = x0+1;
    int y0 = int(y);
    int y1 = y0+1;
    
    float dx0 = x-x0;
    float dy0 = y-y0;
    float dx1 = x-x1;
    float dy1 = y-y1;
    
    mySrand(randomSeed + x0*x0 + y0*y0*y0);
    float g0x = (myRand()%100)/float(50) - 1.0;
    float g0y = (myRand()%100)/float(50) - 1.0;
    mySrand(randomSeed + x1*x1 + y0*y0*y0);
    float g1x = (myRand()%100)/float(50) - 1.0;
    float g1y = (myRand()%100)/float(50) - 1.0;
    mySrand(randomSeed + x0*x0 + y1*y1*y1);
    float g2x = (myRand()%100)/float(50) - 1.0;
    float g2y = (myRand()%100)/float(50) - 1.0;
    mySrand(randomSeed + x1*x1 + y1*y1*y1);
    float g3x = (myRand()%100)/float(50) - 1.0;
    float g3y = (myRand()%100)/float(50) - 1.0;
    
    float i0 = dx0*g0x + dy0*g0y;
    float i1 = dx1*g1x + dy0*g1y;
    float i2 = dx0*g2x + dy1*g2y;
    float i3 = dx1*g3x + dy1*g3y;
    
    float fade_dx0 = fade(dx0);
    float i01 = i0 + fade_dx0*(i1-i0);
    float i23 = i2 + fade_dx0*(i3-i2);
    float i = i01 + fade(dy0)*(i23-i01);
    
    return i;
}

void MapGenerator::generate(int x, int y, int halfW, int halfH, 
                            std::string &naturalMapTmx, 
                            std::vector<std::vector<std::vector<std::pair<uint16_t, uint8_t>>>> &itemMap)
{
    //根据需要生成的区域找到涉及的block
    int startX = x-halfW;
    int endX = x+halfW;
    int startY = y-halfH;
    int endY = y+halfH;
    
    std::vector<std::pair<int,int>> blockIds;
    std::vector<std::vector<std::vector<int>>> naturalMaps;
    std::vector< std::vector<std::vector<std::vector<std::pair<uint16_t, uint8_t>>>> > itemMaps;
    
    int startBlockX = startX/m_blockSize;
    int startBlockY = startY/m_blockSize;
    int endBlockX = endX/m_blockSize;
    int endBlockY = endY/m_blockSize;
    for(int blockY=startBlockY;blockY<=endBlockY;blockY++){
        for(int blockX=startBlockX;blockX<=endBlockX;blockX++){
            std::pair<int,int> block(blockX,blockY);
            blockIds.push_back(block);
            
            std::vector<std::vector<int>> naturalMap;
            std::vector<std::vector<std::vector<std::pair<uint16_t, uint8_t>>>> itemMap;
            
            //尝试从缓存中获取
            bool isCached = m_mapCache.get(block,naturalMap,itemMap);
            
            //若未缓存则生成后加入缓存
            if(!isCached){
                genNaturalMap(block.first,block.second,naturalMap);
                
                m_mapCache.push(block,naturalMap,itemMap);
            }
            
            naturalMaps.push_back(naturalMap);
            itemMaps.push_back(itemMap);
        }
    }
    
    //取所需区域
    std::vector<std::vector<int>> naturalMap;
    naturalMap.resize(halfH*2+1);
    for(int i=0;i<naturalMap.size();i++)
        naturalMap[i].resize(halfW*2+1);
    
    for(int cy=startY;cy<endY;cy++){
        int blockY = cy/m_blockSize;
        int relaY = cy-blockY*m_blockSize;
        for(int cx=startX;cx<=endX;cx++){
            int blockX = cx/m_blockSize;
            
            int naturalMapId = 0;
            for(int i=0;i<blockIds.size();i++){
                if(blockIds[i] == std::make_pair(blockX,blockY)){
                    naturalMapId = i;
                    break;
                }
            }
            
            int relaX = cx-blockX*m_blockSize;
            naturalMap[cy-startY][cx-startX] = naturalMaps[naturalMapId][relaY][relaX];
        }
    }
    
    //自然地形处理
    naturalMapBeautify(x-halfW,y-halfH,naturalMap);
    naturalMap2Tmx(naturalMap,naturalMapTmx);
    
}

void MapGenerator::genNaturalMap(int blockX, int blockY,
                                 std::vector<std::vector<int> > &naturalMap)
{
    int wholeSizeHalf = m_wholeSize/2;
    int fanRadiusPow2 = m_fan_radius*m_fan_radius;
    int f2xRadiusPow2 = (m_fan_radius+m_fan2xiu_w)*(m_fan_radius+m_fan2xiu_w);
    
    int startX = blockX*m_blockSize;
    int endX = startX + m_blockSize;
    int startY = blockY*m_blockSize;
    int endY = startY + m_blockSize;
    
    float waterLine = -0.3;
    float sandLine = -0.2;
    
    float grasslandLine = 0;
    
    naturalMap.clear();
    naturalMap.reserve(m_blockSize);
    for(int y=startY;y<endY;y++){
        std::vector<int> atlasLine;
        atlasLine.reserve(m_blockSize);
        
        for(int x=startX;x<endX;x++){
            
            //计算在fan还是在xiu
            int cDist = (x-wholeSizeHalf)*(x-wholeSizeHalf) + (y-wholeSizeHalf)*(y-wholeSizeHalf);
            //在fan
            if(cDist < fanRadiusPow2){
                float xf = x/float(m_perlinK_fan);
                float yf = y/float(m_perlinK_fan);
                float noise = PerlinNoise2d(m_randomSeed,xf,yf);
                
                //水
                if(noise < waterLine)
                    atlasLine.push_back(0);
                //沙
                else if(noise < sandLine)
                    atlasLine.push_back(64);
                //土
                else if(noise < grasslandLine)
                    atlasLine.push_back(128);
                //草
                else
                    atlasLine.push_back(192);
            }
            //在fan-xiu交界
            else if(cDist < f2xRadiusPow2){
                float xf = x/float(m_perlinK_fan);
                float yf = y/float(m_perlinK_fan);
                float noise = PerlinNoise2d(m_randomSeed,xf,yf);
                
                noise -= (cDist - fanRadiusPow2)/float(m_fan2xiu_w*m_fan2xiu_w);
                
                //水
                if(noise < waterLine)
                    atlasLine.push_back(0);
                //沙
                else if(noise < sandLine)
                    atlasLine.push_back(64);
                //土
                else if(noise < grasslandLine)
                    atlasLine.push_back(128);
                //草
                else
                    atlasLine.push_back(192);
            }
            //在xiu
            else{
                atlasLine.push_back(0);
            }
            
        }
        
        naturalMap.push_back(atlasLine);
    }
}

void MapGenerator::naturalMap2Tmx(const std::vector<std::vector<int> > &naturalMap, std::string &tmx)
{
    if(naturalMap.empty() || naturalMap[0].empty()){
        printf("naturalMap2Tmx error!");
        exit(1);
    }
    
    int h = naturalMap.size();
    int w = naturalMap[0].size();
    
    std::string tmxString = "";
    
    tmxString += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    tmxString += "<map version=\"1.2\" tiledversion=\"1.3.3\" orientation=\"orthogonal\" renderorder=\"right-down\" width=\""+std::to_string(w)+
            "\" height=\""+std::to_string(h)+"\" tilewidth=\"32\" tileheight=\"32\" infinite=\"0\" nextlayerid=\"2\" nextobjectid=\"1\">\n";
    tmxString += "<tileset firstgid=\"1\" source=\"naturalAtlas.tsx\"/>\n";
    tmxString += "<layer id=\"1\" name=\"layer0\" width=\""+std::to_string(w)+"\" height=\""+std::to_string(h)+"\">\n";
    tmxString += "<data encoding=\"csv\">\n";
    
    for(int y=h-1;y>=0;y--){
        for(int x=0;x<w;x++){
            tmxString += std::to_string(naturalMap[y][x]+1) + ",";
        }
    }
    tmxString.pop_back();
    
    tmxString += "</data>\n</layer>\n</map>";
    
    tmx.assign(tmxString);
}

void MapGenerator::naturalMapBeautify(int startX, int startY,
                                      std::vector<std::vector<int> > &naturalMap)
{
    if(naturalMap.empty() || naturalMap[0].empty()){
        printf("naturalMapBeautify error!");
        exit(1);
    }
    
    int h = naturalMap.size();
    int w = naturalMap[0].size();
    
    //地形内随机
    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            int sx = startX+x;
            int sy = startY+y;
            mySrand(sx*sx*sy+sy*sx*6+7);
            
            //每种地形有3种装饰地形，一共4种候选
            int k=0;
            int r = myRand()%100;
            if(r < 94) k = 0;
            else if(r < 96) k = 1;
            else if(r < 98) k = 2;
            else k = 3;
            
            naturalMap[y][x] += k*16;
        }
    }
    
    //处理地形交界
    std::vector<std::vector<int> > newNaturalMap;
    newNaturalMap.assign(naturalMap.begin(),naturalMap.end());
    
    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            int utile = -1;
            if(y < h-1) utile = naturalMap[y+1][x]/64;
            
            int btile = -1;
            if(y > 0) btile = naturalMap[y-1][x]/64;
            
            int ltile = -1;
            if(x > 0) ltile = naturalMap[y][x-1]/64;
            
            int rtile = -1;
            if(x < w-1) rtile = naturalMap[y][x+1]/64;
            
            int ctile_src = naturalMap[y][x];
            int ctile = ctile_src/64;
            
            bool ud = false;
            bool bd = false;
            bool ld = false;
            bool rd = false;
            if(utile != ctile) ud = true;
            if(btile != ctile) bd = true;
            if(ltile != ctile) ld = true;
            if(rtile != ctile) rd = true;
            
            if(!ud && !rd && !bd && !ld){
                newNaturalMap[y][x] = ctile_src;
            }
            else if(ud && !rd && !bd && !ld){
                newNaturalMap[y][x] = ctile_src+1;
            }
            else if(!ud && rd && !bd && !ld){
                newNaturalMap[y][x] = ctile_src+2;
            }
            else if(!ud && !rd && bd && !ld){
                newNaturalMap[y][x] = ctile_src+3;
            }
            else if(!ud && !rd && !bd && ld){
                newNaturalMap[y][x] = ctile_src+4;
            }
            else if(ud && rd && !bd && !ld){
                newNaturalMap[y][x] = ctile_src+5;
            }
            else if(!ud && rd && bd && !ld){
                newNaturalMap[y][x] = ctile_src+6;
            }
            else if(!ud && !rd && bd && ld){
                newNaturalMap[y][x] = ctile_src+7;
            }
            else if(ud && !rd && !bd && ld){
                newNaturalMap[y][x] = ctile_src+8;
            }
            else if(ud && !rd && bd && !ld){
                newNaturalMap[y][x] = ctile_src+9;
            }
            else if(!ud && rd && !bd && ld){
                newNaturalMap[y][x] = ctile_src+10;
            }
            else if(ud && rd && bd && !ld){
                newNaturalMap[y][x] = ctile_src+11;
            }
            else if(!ud && rd && bd && ld){
                newNaturalMap[y][x] = ctile_src+12;
            }
            else if(ud && !rd && bd && ld){
                newNaturalMap[y][x] = ctile_src+13;
            }
            else if(ud && rd && !bd && ld){
                newNaturalMap[y][x] = ctile_src+14;
            }
            else{
                newNaturalMap[y][x] = ctile_src+15;
            }
        }
    }
    
    naturalMap.assign(newNaturalMap.begin(),newNaturalMap.end());
}

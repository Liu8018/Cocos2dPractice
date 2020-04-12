#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <string>
#include <vector>
#include <queue>

class MapGenerator
{
public:
    MapGenerator();
    
    void setRandomSeed(unsigned int);
    
    void generate(int x, int y, int halfW, int halfH,
                  std::string &naturalMapTmx,
                  std::vector<std::vector<std::vector<std::pair<uint16_t,uint8_t>>>> &itemMap);
    
private:
    //随机种子
    unsigned int m_randomSeed;
    
    //随机数生成器
    unsigned int m_randomVar;
    void mySrand(unsigned int);
    unsigned int myRand();
    
    //区块大小
    int m_blockSize;
    //perlin噪声参考单位
    int m_perlinK;
    
    //地图缓存
    std::queue<std::pair<int,int>> m_mapCacheCoord;
    std::queue< std::vector<std::vector<std::vector<int>>> > m_naturalMapCache;
    std::queue< std::vector<std::vector<std::vector<int>>> > m_itemMapCache;
    
    //柏林噪声
    float PerlinNoise2d(int randomSeed, float x, float y);
    
    //生成自然部分（不可更改地图）
    void genNaturalMap(int startX, int startY, int endX, int endY,
                       std::vector<std::vector<int>>&);
};

#endif // MAPGENERATOR_H

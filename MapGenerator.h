#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <string>
#include <vector>

class MapCache
{
public:
    MapCache();
    
    void push(const std::pair<int,int>& blockId,
              const std::vector<std::vector<int>>& naturalMapBlock,
              const std::vector<std::vector<std::vector<std::pair<uint16_t, uint8_t>>>> &itemMapBlock);
    
    bool get(const std::pair<int,int>& blockId,
             std::vector<std::vector<int>>& naturalMapBlock,
             std::vector<std::vector<std::vector<std::pair<uint16_t,uint8_t>>>>& itemMapBlock);
    
private:
    int m_size;
    int m_maxSize;
    
    struct mapCacheList{
        std::pair<int,int> blockId;
        int naturalMapBlock[200][200];
        std::pair<uint16_t, uint8_t> itemMapBlock[200][200][5];
        
        mapCacheList* next;
    };
    
    mapCacheList* m_first;
};

class MapGenerator
{
public:
    MapGenerator();
    
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
    
    //地图尺寸信息
    int m_wholeSize;
    int m_fan_radius;
    int m_fan2xiu_w;
    
    //区块大小
    int m_blockSize;
    //perlin噪声参考单位
    int m_perlinK_fan;
    int m_perlinK_xiu;
    
    //地图缓存
    MapCache m_mapCache;
    
    //柏林噪声
    float PerlinNoise2d(int randomSeed, float x, float y);
    
    //生成自然部分（不可更改地图）
    void genNaturalMap(int blockX, int blockY, std::vector<std::vector<int>>&);
    void naturalMap2Tmx(const std::vector<std::vector<int> > &naturalMap, std::string &tmx);
    void naturalMapBeautify(int startX, int startY,
                            std::vector<std::vector<int> > &naturalMap);
};

#endif // MAPGENERATOR_H

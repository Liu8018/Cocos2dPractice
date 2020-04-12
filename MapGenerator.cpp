#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
    m_perlinK = 20;
    m_blockSize = 100;
}

void MapGenerator::setRandomSeed(unsigned int r)
{
    m_randomSeed = r;
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

void naturalMap2Tmx(const std::vector<std::vector<int> > &naturalMap, std::string &tmx)
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
    
    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            tmxString += std::to_string(naturalMap[y][x]+1) + ",";
        }
    }
    tmxString.pop_back();
    
    tmxString += "</data>\n</layer>\n</map>";
    
    tmx.assign(tmxString);
}

void MapGenerator::generate(int x, int y, int halfW, int halfH, 
                            std::string &naturalMapTmx, 
                            std::vector<std::vector<std::vector<std::pair<uint16_t, uint8_t>>>> &itemMap)
{
    //固定地形--------------------------------------------------------------
    //生成对应区域
    std::vector<std::vector<int>> naturalMap;
    genNaturalMap(x-halfW,y-halfH,x+halfW,y+halfH,naturalMap);
    
    //自然地形美化
    
    //自然地形转tmx
    naturalMap2Tmx(naturalMap,naturalMapTmx);
    
    //可改造地形------------------------------------------------------------
    //根据需要生成的区域找到涉及的block
    
    //生成每个block
    
    //取所需区域
    
}

void MapGenerator::genNaturalMap(int startX, int startY, int endX, int endY,
                                 std::vector<std::vector<int> > &naturalMap)
{
    float waterLine = -0.3;
    float sandLine = -0.2;
    
    float grasslandLine = 0;
    
    naturalMap.clear();
    naturalMap.reserve(m_blockSize);
    unsigned int grassRandomSeed = (m_randomSeed+1)*(m_randomSeed+1);
    for(int y=startY;y<=endY;y++){
        std::vector<int> atlasLine;
        atlasLine.reserve(m_blockSize);
        
        for(int x=startX;x<=endX;x++){
            float xf = x/float(m_perlinK);
            float yf = y/float(m_perlinK);
            float noise1 = PerlinNoise2d(m_randomSeed,xf,yf);
            //float noise2 = PerlinNoise2d(grassRandomSeed,xf,yf);
            
            //水
            if(noise1 < waterLine)
                atlasLine.push_back(0);
            //沙
            else if(noise1 < sandLine)
                atlasLine.push_back(64);
            //土
            else if(noise1 < grasslandLine)
                atlasLine.push_back(128);
            //草
            else
                atlasLine.push_back(192);
        }
        
        naturalMap.push_back(atlasLine);
    }
}

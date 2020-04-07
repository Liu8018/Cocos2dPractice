#include "VizItem.h"
#include "globals.h"

#include <fstream>

USING_NS_CC;

void loadItemTable(std::string path, 
                   std::vector<std::vector<std::string>> &itemTable)
{
    std::ifstream ifs(path,std::ios::in);
    if(!ifs.is_open()){
        printf("failed to open %s\n",path.data());
        exit(1);
    }
    
    std::string line;
    while(getline(ifs,line)){
        //去除换行符
        if(line[line.length()-1] == '\n')
            line.pop_back();
        
        std::vector<std::string> attrVec;
        while(1){
            int commaPos = line.find_first_of(",");
            if(commaPos == std::string::npos){
                break;
            }
            std::string attr = line.substr(0,commaPos);
            attrVec.push_back(attr);
            
            line = line.substr(commaPos+1,line.length());
        }
        attrVec.push_back(line);
        itemTable.push_back(attrVec);
    }
    
    ifs.close();
}

VizItem::VizItem()
{
    //加载itemTable
    loadItemTable("Resources/"+g_txt_itemTable,m_itemTable);
    
    //应该根据itemId排一下序--------------------------------------------------------------------------------
    
    //根据itemTable加载所有item到内存
    m_itemSpriteList.reserve(m_itemTable.size());
    for(int i=0;i<m_itemTable.size();i++){
        std::string itemImgPath = m_itemTable[i][1];
        m_itemSpriteList.pushBack(Sprite::create(itemImgPath));
    }
}

cocos2d::SpriteFrame* VizItem::getItemFrame(int itemId)
{
    return m_itemSpriteList.at(itemId)->getSpriteFrame();
}

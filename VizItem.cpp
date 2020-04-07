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
    //这里应该根据itemList加载所有item到内存
    loadItemTable(g_txt_itemTable,m_itemTable);
}

cocos2d::SpriteFrame* VizItem::getItemFrame(int itemId)
{
    
}

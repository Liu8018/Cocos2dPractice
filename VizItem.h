#ifndef VIZITEM_H
#define VIZITEM_H

#include "cocos2d.h"

class VizItem
{
public:
    VizItem();
    
    cocos2d::SpriteFrame *getItemFrame(int);
    
private:
    //
    std::vector<std::vector<std::string>> m_itemTable;
    
    //
    cocos2d::Vector<cocos2d::Sprite*> m_itemSpriteList;
};

#endif // VIZITEM_H

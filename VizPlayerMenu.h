#ifndef VIZPLAYERMENU_H
#define VIZPLAYERMENU_H

#include "cocos2d.h"

class VizPlayerMenu
{
public:
    VizPlayerMenu();
    
    cocos2d::Menu *getMenu();
    
private:
    cocos2d::Menu* m_menu;
    
    void MenuOpenCallback(cocos2d::Ref *pSender);
    void MenuCloseCallback(cocos2d::Ref* pSender);
};

#endif // VIZPLAYERMENU_H

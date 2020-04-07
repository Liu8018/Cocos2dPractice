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
    
    //
    cocos2d::Menu* m_subMenu_playerInfo;
    cocos2d::Menu* m_subMenu_backpack;
    cocos2d::Menu* m_subMenu_system;
    void loadPlayerInfoUI();
    void loadBackpackUI();
    void loadSystemUI();
    
    void showPlayerInfoUI(cocos2d::Ref*);
    void showBackpackUI(cocos2d::Ref*);
    void showSystemUI(cocos2d::Ref*);
};

#endif // VIZPLAYERMENU_H

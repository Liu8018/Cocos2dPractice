#ifndef VIZPLAYERMENU_H
#define VIZPLAYERMENU_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class VizPlayerMenu
{
public:
    VizPlayerMenu();
    
    cocos2d::ui::Layout *getMenu();
    
private:
    cocos2d::ui::Layout* m_menu;
    
    void MenuOpenCallback(cocos2d::Ref*);
    void MenuCloseCallback(cocos2d::Ref*);
    
    //
    cocos2d::ui::Layout* m_subMenu_playerInfo;
    cocos2d::ui::Layout* m_subMenu_backpack;
    cocos2d::ui::Layout* m_subMenu_system;
    void loadPlayerInfoUI();
    void loadBackpackUI();
    void loadSystemUI();
    
    void showPlayerInfoUI(cocos2d::Ref*);
    void showBackpackUI(cocos2d::Ref*);
    void showSystemUI(cocos2d::Ref*);
    
    //背包界面的变量
    void updateItemBoxes();
    int m_curPageId;
    int m_maxPageId;
    void itemNextPage(cocos2d::Ref*);
    void itemPrevPage(cocos2d::Ref*);
    std::vector<std::pair<std::uint16_t,std::uint8_t>> m_playerItemList;
    int m_curItemId;
    void itemBoxClicked(cocos2d::Ref*);
    void itemDropClicked(cocos2d::Ref*);
    
    //物品信息区域
    cocos2d::Sprite* m_curItemSprite;
    void loadItemInfoUI();
    
};

#endif // VIZPLAYERMENU_H

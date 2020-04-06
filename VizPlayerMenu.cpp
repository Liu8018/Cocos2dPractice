#include "VizPlayerMenu.h"
#include "globals.h"

#include "ui/CocosGUI.h"

USING_NS_CC;

VizPlayerMenu::VizPlayerMenu()
{
    auto vizSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    //
    m_menu = Menu::create();
    m_menu->setPosition(origin);
    
    //menu->菜单图标
    auto menuIcon = cocos2d::ui::Button::create();
    menuIcon->loadTextures("res/menu/backpack.png","res/menu/backpack_selected.png");
    menuIcon->setPosition(Vec2(origin.x+menuIcon->getContentSize().width/2,
                          origin.y+vizSize.height-menuIcon->getContentSize().height/2));
    menuIcon->addClickEventListener(CC_CALLBACK_1(VizPlayerMenu::MenuOpenCallback, this));
    m_menu->addChild(menuIcon,0);
    
    //menu->背景
    auto menuBg = ui::ImageView::create("res/menu/menuBg.png");
    menuBg->setOpacity(220);
    menuBg->setPosition(origin + vizSize/2);
    menuBg->setVisible(false);//初始不可见
    m_menu->addChild(menuBg,1,"menuBg");
    
    //menu->背景->关闭菜单图标
    auto menuCloseIcon = cocos2d::ui::Button::create();
    menuCloseIcon->loadTextures("res/menu/menuClose.png", "res/menu/menuClose_selected.png");
    menuCloseIcon->setPosition(menuBg->getContentSize()-menuCloseIcon->getContentSize()/2);
    menuCloseIcon->addClickEventListener(CC_CALLBACK_1(VizPlayerMenu::MenuCloseCallback, this));
    menuBg->addChild(menuCloseIcon);
    
    //menu->背景->界面选择栏
    
    //menu->背景->人物界面
    
    //menu->背景->背包界面
    
    //menu->背景->系统设置界面
    
}

Menu* VizPlayerMenu::getMenu()
{
    return m_menu;
}

void VizPlayerMenu::MenuOpenCallback(cocos2d::Ref *pSender)
{
    g_vizScene->unscheduleUpdate();
    
    m_menu->getChildByName("menuBg")->setVisible(true);
}

void VizPlayerMenu::MenuCloseCallback(cocos2d::Ref *pSender)
{
    g_vizScene->scheduleUpdate();
    
    m_menu->getChildByName("menuBg")->setVisible(false);
}

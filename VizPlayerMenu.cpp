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
    menuIcon->loadTextures(g_icon_backpack,g_icon_backpackSel);
    menuIcon->setPosition(Vec2(origin.x+menuIcon->getContentSize().width/2,
                          origin.y+vizSize.height-menuIcon->getContentSize().height/2));
    menuIcon->addClickEventListener(CC_CALLBACK_1(VizPlayerMenu::MenuOpenCallback, this));
    m_menu->addChild(menuIcon,0);
    
    //menu->背景
    auto menuBg = ui::ImageView::create(g_img_menuBg);
    menuBg->setOpacity(240);
    menuBg->setPosition(origin + vizSize/2);
    menuBg->setVisible(false);//初始不可见
    m_menu->addChild(menuBg,1,"menuBg");
    
    //menu->背景->关闭菜单图标
    auto menuCloseIcon = ui::Button::create();
    menuCloseIcon->loadTextures(g_icon_menuClose, g_icon_menuCloseSel);
    menuCloseIcon->setPosition(menuBg->getContentSize()-menuCloseIcon->getContentSize()/2+Size(2,2));
    menuCloseIcon->addClickEventListener(CC_CALLBACK_1(VizPlayerMenu::MenuCloseCallback, this));
    menuBg->addChild(menuCloseIcon);
    
    //menu->背景->界面选择栏
    auto playerInfoButton = ui::Button::create();
    playerInfoButton->loadTextures(g_icon_blueButton1,g_icon_blueButton1Sel,g_icon_blueButton1Sel);
    playerInfoButton->setPosition(Vec2(20,580)+playerInfoButton->getContentSize()/2);
    playerInfoButton->setTitleText("player");
    playerInfoButton->setTitleFontSize(20);
    playerInfoButton->addClickEventListener(CC_CALLBACK_1(VizPlayerMenu::showPlayerInfoUI, this));
    menuBg->addChild(playerInfoButton,0,"playerInfoButton");
    
    auto backpackButton = ui::Button::create();
    backpackButton->loadTextures(g_icon_blueButton1,g_icon_blueButton1Sel,g_icon_blueButton1Sel);
    backpackButton->setPosition(Vec2(215,580)+backpackButton->getContentSize()/2);
    backpackButton->setTitleText("backpack");
    backpackButton->setTitleFontSize(20);
    backpackButton->addClickEventListener(CC_CALLBACK_1(VizPlayerMenu::showBackpackUI, this));
    menuBg->addChild(backpackButton,0,"backpackButton");
    
    auto systemSettingButton = ui::Button::create();
    systemSettingButton->loadTextures(g_icon_blueButton1,g_icon_blueButton1Sel,g_icon_blueButton1Sel);
    systemSettingButton->setPosition(Vec2(410,580)+systemSettingButton->getContentSize()/2);
    systemSettingButton->setTitleText("system");
    systemSettingButton->setTitleFontSize(20);
    systemSettingButton->addClickEventListener(CC_CALLBACK_1(VizPlayerMenu::showSystemUI, this));
    menuBg->addChild(systemSettingButton,0,"systemSettingButton");
    
    
    //menu->背景->人物界面
    loadPlayerInfoUI();
    
    //menu->背景->背包界面
    loadBackpackUI();
    
    //menu->背景->系统设置界面
    loadSystemUI();
    
    showPlayerInfoUI(nullptr);
}

void VizPlayerMenu::loadPlayerInfoUI()
{
    //人物信息子菜单
    m_subMenu_playerInfo = Menu::create();
    m_subMenu_playerInfo->setPosition(Vec2::ZERO);
    
    auto menuBg = m_menu->getChildByName("menuBg");
    menuBg->addChild(m_subMenu_playerInfo);
}

void VizPlayerMenu::loadBackpackUI()
{
    //背包子菜单
    m_subMenu_backpack = Menu::create();
    m_subMenu_backpack->setPosition(Vec2::ZERO);
    auto menuBg = m_menu->getChildByName("menuBg");
    menuBg->addChild(m_subMenu_backpack);
    
    //物品详情区
    auto itemInfo = ui::ImageView::create(g_img_panel1);
    m_subMenu_backpack->addChild(itemInfo);
    itemInfo->setPosition(Vec2(30,80)+itemInfo->getContentSize()/2);
    
    //"使用"按钮
    auto itemUseButton = ui::Button::create();
    m_subMenu_backpack->addChild(itemUseButton);
    itemUseButton->loadTextures(g_icon_greenButton1,g_icon_greenButton1Sel);
    itemUseButton->setTitleText("use");
    itemUseButton->setTitleFontSize(18);
    itemUseButton->setPosition(Vec2(40,25)+itemUseButton->getContentSize()/2);
    
    //"丢弃"按钮
    auto itemDropButton = ui::Button::create();
    m_subMenu_backpack->addChild(itemDropButton);
    itemDropButton->loadTextures(g_icon_greenButton1,g_icon_greenButton1Sel);
    itemDropButton->setTitleText("drop");
    itemDropButton->setTitleFontSize(18);
    itemDropButton->setPosition(Vec2(175,25)+itemDropButton->getContentSize()/2);
    
    //物品展示区
    auto itemRegion = ui::ImageView::create(g_img_panel2);
    m_subMenu_backpack->addChild(itemRegion);
    itemRegion->setPosition(Vec2(330,80)+itemRegion->getContentSize()/2);
    
    //下翻页按钮
    auto sliderLeftButton = ui::Button::create();
    m_subMenu_backpack->addChild(sliderLeftButton);
    sliderLeftButton->loadTextures(g_icon_greySliderLeft,g_icon_greySliderLeftSel);
    sliderLeftButton->setPosition(Vec2(740,35)+sliderLeftButton->getContentSize()/2);
    
    //上翻页按钮
    auto sliderRightButton = ui::Button::create();
    m_subMenu_backpack->addChild(sliderRightButton);
    sliderRightButton->loadTextures(g_icon_greySliderRight,g_icon_greySliderRightSel);
    sliderRightButton->setPosition(Vec2(830,35)+sliderRightButton->getContentSize()/2);
}

void VizPlayerMenu::loadSystemUI()
{
    //系统设置子菜单
    m_subMenu_system = Menu::create();
    m_subMenu_system->setPosition(Vec2::ZERO);
    
    auto menuBg = m_menu->getChildByName("menuBg");
    menuBg->addChild(m_subMenu_system);
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

void VizPlayerMenu::showPlayerInfoUI(cocos2d::Ref *)
{
    auto menuBg = m_menu->getChildByName("menuBg");
    ((ui::Button*)menuBg->getChildByName("playerInfoButton"))->setEnabled(false);
    ((ui::Button*)menuBg->getChildByName("backpackButton"))->setEnabled(true);
    ((ui::Button*)menuBg->getChildByName("systemSettingButton"))->setEnabled(true);
    
    m_subMenu_playerInfo->setVisible(true);
    m_subMenu_backpack->setVisible(false);
    m_subMenu_system->setVisible(false);
}

void VizPlayerMenu::showBackpackUI(cocos2d::Ref *)
{
    auto menuBg = m_menu->getChildByName("menuBg");
    ((ui::Button*)menuBg->getChildByName("playerInfoButton"))->setEnabled(true);
    ((ui::Button*)menuBg->getChildByName("backpackButton"))->setEnabled(false);
    ((ui::Button*)menuBg->getChildByName("systemSettingButton"))->setEnabled(true);
    
    m_subMenu_playerInfo->setVisible(false);
    m_subMenu_backpack->setVisible(true);
    m_subMenu_system->setVisible(false);
}

void VizPlayerMenu::showSystemUI(cocos2d::Ref *)
{
    auto menuBg = m_menu->getChildByName("menuBg");
    ((ui::Button*)menuBg->getChildByName("playerInfoButton"))->setEnabled(true);
    ((ui::Button*)menuBg->getChildByName("backpackButton"))->setEnabled(true);
    ((ui::Button*)menuBg->getChildByName("systemSettingButton"))->setEnabled(false);
    
    m_subMenu_playerInfo->setVisible(false);
    m_subMenu_backpack->setVisible(false);
    m_subMenu_system->setVisible(true);
}

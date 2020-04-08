#include "VizPlayerMenu.h"
#include "globals.h"

USING_NS_CC;

VizPlayerMenu::VizPlayerMenu()
{
    auto vizSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    //
    m_menu = ui::Layout::create();
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
    m_subMenu_playerInfo = ui::Layout::create();
    m_subMenu_playerInfo->setPosition(Vec2::ZERO);
    
    auto menuBg = m_menu->getChildByName("menuBg");
    menuBg->addChild(m_subMenu_playerInfo);
}

void VizPlayerMenu::loadBackpackUI()
{
    //背包子菜单
    m_subMenu_backpack = ui::Layout::create();
    m_subMenu_backpack->setPosition(Vec2::ZERO);
    auto menuBg = m_menu->getChildByName("menuBg");
    menuBg->addChild(m_subMenu_backpack);
    
    //物品详情区
    auto itemInfo = ui::ImageView::create(g_img_panel1);
    m_subMenu_backpack->addChild(itemInfo,0,"itemInfoBg");
    itemInfo->setPosition(Vec2(30,140)+itemInfo->getContentSize()/2);
    
    //"使用"按钮
    auto itemUseButton = ui::Button::create();
    m_subMenu_backpack->addChild(itemUseButton);
    itemUseButton->loadTextures(g_icon_greenButton1,g_icon_greenButton1Sel);
    itemUseButton->setTitleText("use");
    itemUseButton->setTitleFontSize(18);
    itemUseButton->setPosition(Vec2(37,77)+itemUseButton->getContentSize()/2);
    
    //"丢弃"按钮
    auto itemDropButton = ui::Button::create();
    m_subMenu_backpack->addChild(itemDropButton);
    itemDropButton->loadTextures(g_icon_greenButton1,g_icon_greenButton1Sel);
    itemDropButton->setTitleText("drop");
    itemDropButton->setTitleFontSize(18);
    itemDropButton->setPosition(Vec2(176,77)+itemDropButton->getContentSize()/2);
    itemDropButton->addClickEventListener(CC_CALLBACK_1(VizPlayerMenu::itemDropClicked, this));
    
    //物品展示区
    auto itemRegion = ui::ImageView::create(g_img_panel2);
    m_subMenu_backpack->addChild(itemRegion);
    itemRegion->setPosition(Vec2(330,80)+itemRegion->getContentSize()/2);
    
    auto equipItemButton = ui::Button::create();
    m_subMenu_backpack->addChild(equipItemButton);
    equipItemButton->loadTextures(g_icon_greyButton1,g_icon_greyButton1Sel);
    equipItemButton->setTitleText("eq");
    equipItemButton->setTitleFontSize(18);
    equipItemButton->setPosition(Vec2(345,527)+equipItemButton->getContentSize()/2);
    
    auto specialItemButton = ui::Button::create();
    m_subMenu_backpack->addChild(specialItemButton);
    specialItemButton->loadTextures(g_icon_greyButton1,g_icon_greyButton1Sel);
    specialItemButton->setTitleText("sp");
    specialItemButton->setTitleFontSize(18);
    specialItemButton->setPosition(Vec2(410,527)+specialItemButton->getContentSize()/2);
    
    int itemBoxIntH = (473.733-95)/7;
    int itemBoxTag = 0;
    for(int i=0;i<8;i++){
        for(int j=0;j<3;j++){
            auto itemBox = ui::Button::create();
            m_subMenu_backpack->addChild(itemBox,0,itemBoxTag);
            itemBox->loadTextures(g_img_itemBox,g_img_itemBoxSel,g_img_itemBoxSel);
            itemBox->setPosition(Vec2(345+j*175,473.733-i*itemBoxIntH)+itemBox->getContentSize()/2);
            itemBox->addClickEventListener(CC_CALLBACK_1(VizPlayerMenu::itemBoxClicked, this));
            itemBoxTag++;
        }
    }
    
    m_curPageId = 0;
    m_maxPageId = 0;
    
    //上翻页按钮
    auto sliderLeftButton = ui::Button::create();
    m_subMenu_backpack->addChild(sliderLeftButton);
    sliderLeftButton->loadTextures(g_icon_greySliderLeft,g_icon_greySliderLeftSel);
    sliderLeftButton->setPosition(Vec2(740,35)+sliderLeftButton->getContentSize()/2);
    sliderLeftButton->addClickEventListener(CC_CALLBACK_1(VizPlayerMenu::itemPrevPage, this));
    
    //下翻页按钮
    auto sliderRightButton = ui::Button::create();
    m_subMenu_backpack->addChild(sliderRightButton);
    sliderRightButton->loadTextures(g_icon_greySliderRight,g_icon_greySliderRightSel);
    sliderRightButton->setPosition(Vec2(830,35)+sliderRightButton->getContentSize()/2);
    sliderRightButton->addClickEventListener(CC_CALLBACK_1(VizPlayerMenu::itemNextPage, this));
}

void VizPlayerMenu::loadSystemUI()
{
    //系统设置子菜单
    m_subMenu_system = ui::Layout::create();
    m_subMenu_system->setPosition(Vec2::ZERO);
    
    auto menuBg = m_menu->getChildByName("menuBg");
    menuBg->addChild(m_subMenu_system);
}

ui::Layout* VizPlayerMenu::getMenu()
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
    //
    auto menuBg = m_menu->getChildByName("menuBg");
    ((ui::Button*)menuBg->getChildByName("playerInfoButton"))->setEnabled(true);
    ((ui::Button*)menuBg->getChildByName("backpackButton"))->setEnabled(false);
    ((ui::Button*)menuBg->getChildByName("systemSettingButton"))->setEnabled(true);
    
    m_subMenu_playerInfo->setVisible(false);
    m_subMenu_backpack->setVisible(true);
    m_subMenu_system->setVisible(false);
    
    //
    m_playerItemList.clear();
    g_world->getPlayerBackpack(0,m_playerItemList);
    
    m_maxPageId = m_playerItemList.size()/24;
    
    //
    updateItemBoxes();
    
    if(m_playerItemList.empty())
        return;
    
    //在左侧显示背包第一个物品信息
    itemBoxClicked(m_subMenu_backpack->getChildByTag(0));
}

void VizPlayerMenu::updateItemBoxes()
{
    for(int i=0;i<24;i++){
        int itemId = m_curPageId*24+i;
        if(itemId < m_playerItemList.size()){
            auto itemBox = (ui::Button*)m_subMenu_backpack->getChildByTag(i);
            itemBox->setTitleText(std::to_string(m_playerItemList[itemId].first));
            
            auto sp = Sprite::createWithSpriteFrame(g_vizItem->getItemFrame(m_playerItemList[itemId].first));
            float longSide = std::max(sp->getSpriteFrame()->getRect().size.width,
                                      sp->getSpriteFrame()->getRect().size.height);
            float scaleRatio = 34/longSide;
            sp->setScale(scaleRatio);
            sp->setPosition(Vec2(22,26));
            
            if(itemBox->getChildByTag(0) != nullptr)
                itemBox->removeChildByTag(0);
            itemBox->addChild(sp,0,0);
        }
        else{
            //清除掉这些box的内容
            auto itemBox = (ui::Button*)m_subMenu_backpack->getChildByTag(i);
            itemBox->setTitleText("");
            
            itemBox->removeChildByTag(0);
        }
    }
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

void VizPlayerMenu::itemBoxClicked(cocos2d::Ref *pSender)
{
    auto cItemBox = ((ui::Button*)pSender);
    
    //点到无物品的栏则返回
    int itemBoxTag = cItemBox->getTag();
    int callItemId = itemBoxTag + m_curPageId*24;
    if(callItemId > int(m_playerItemList.size())-1)
        return;
    
    m_curItemId = callItemId;
    
    //
    for(int i=0;i<24;i++){
        auto itemBox = (ui::Button*)m_subMenu_backpack->getChildByTag(i);
        if(!itemBox->isEnabled())
            itemBox->setEnabled(true);
    }
    cItemBox->setEnabled(false);
    
    //在左侧显示物品信息
    loadItemInfoUI();
}

void VizPlayerMenu::itemDropClicked(cocos2d::Ref*)
{
    if(m_curItemId < 0)
        return;
    
    //map上放置物品
    g_vizScene->placeItem(m_playerItemList[m_curItemId]);
    
    //删除背包里的物品信息
    std::uint8_t* p_itemNum = &(m_playerItemList[m_curItemId].second);
    *p_itemNum -= 1;
    if(*p_itemNum < 1)
        m_playerItemList.erase(m_playerItemList.begin()+m_curItemId);
    
    //更新world信息
    g_world->dropItem(0,m_curItemId,1);
    
    //处理删除物品后本页为空的情况
    if(m_curItemId > int(m_playerItemList.size())-1){
        m_curItemId--;
        
        if(!m_playerItemList.empty())
            itemBoxClicked(m_subMenu_backpack->getChildByTag(m_curItemId%24));
        
        if(m_curPageId*24 > m_curItemId && m_curPageId>0){
            m_curPageId--;
            m_maxPageId--;
            itemBoxClicked(m_subMenu_backpack->getChildByTag(23));
        }
    }
    
    m_maxPageId = (int(m_playerItemList.size())-1)/24;
    
    //
    updateItemBoxes();
    loadItemInfoUI();
    
    //
    if(m_curItemId < 0)
        ((ui::Button*)m_subMenu_backpack->getChildByTag(0))->setEnabled(true);
}

void VizPlayerMenu::loadItemInfoUI()
{
    if(m_curItemSprite != nullptr){
        m_subMenu_backpack->removeChild(m_curItemSprite,true);
    }
    
    if(m_curItemId < 0)
        return;
    
    m_curItemSprite = Sprite::createWithSpriteFrame(
                g_vizItem->getItemFrame(m_playerItemList[m_curItemId].first));
    m_subMenu_backpack->addChild(m_curItemSprite);
    m_curItemSprite->setPosition(
                m_subMenu_backpack->getChildByName("itemInfoBg")->getPosition());
    
}

void VizPlayerMenu::itemNextPage(cocos2d::Ref *)
{
    m_curPageId++;
    
    if(m_curPageId > m_maxPageId){
        m_curPageId = m_maxPageId;
        return;
    }
    
    updateItemBoxes();
    itemBoxClicked(m_subMenu_backpack->getChildByTag(0));
}

void VizPlayerMenu::itemPrevPage(cocos2d::Ref *)
{
    m_curPageId--;
    
    if(m_curPageId < 0){
        m_curPageId = 0;
        return;
    }
    
    updateItemBoxes();
    itemBoxClicked(m_subMenu_backpack->getChildByTag(0));
}

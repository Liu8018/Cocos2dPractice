#ifndef GLOBALS_H
#define GLOBALS_H

#include "cocos2d.h"

//
extern int g_mapWholeSize;

//一些游戏资源路径
extern std::string g_icon_backpack;
extern std::string g_icon_backpackSel;
extern std::string g_icon_blueButton1;
extern std::string g_icon_blueButton1Sel;
extern std::string g_icon_greenButton1;
extern std::string g_icon_greenButton1Sel;
extern std::string g_icon_greySliderLeft;
extern std::string g_icon_greySliderLeftSel;
extern std::string g_icon_greySliderRight;
extern std::string g_icon_greySliderRightSel;
extern std::string g_img_menuBg;
extern std::string g_icon_menuClose;
extern std::string g_icon_menuCloseSel;
extern std::string g_img_panel1;
extern std::string g_img_panel2;
extern std::string g_img_itemBox;
extern std::string g_img_itemBoxSel;
extern std::string g_icon_greyButton1;
extern std::string g_icon_greyButton1Sel;

extern std::string g_imgSet_sprite1;

extern std::string g_dir_mapRes;

extern std::string g_txt_itemTable;

//每回合时间长度
extern float g_turnTimeLen;

//瓦片地图的单个瓦片尺寸
extern int g_mapTileW;
extern int g_mapTileH;

//主游戏界面VizScene的指针
#include "VizScene.h"
extern VizScene* g_vizScene;

//
#include "VizItem.h"
extern VizItem* g_vizItem;

//世界信息World的指针
#include "World.h"
extern World* g_world;

#endif // GLOBALS_H

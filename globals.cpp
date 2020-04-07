#include "globals.h"

std::string g_icon_backpack = "menu/backpack.png";
std::string g_icon_backpackSel = "menu/backpack_selected.png";
std::string g_icon_blueButton1 = "menu/BlueButton1.png";
std::string g_icon_blueButton1Sel = "menu/BlueButton1_selected.png";
std::string g_icon_greenButton1 = "menu/GreenButton1.png";
std::string g_icon_greenButton1Sel = "menu/GreenButton1_selected.png";
std::string g_icon_greySliderLeft = "menu/greySliderLeft.png";
std::string g_icon_greySliderLeftSel = "menu/greySliderLeft_selected.png";
std::string g_icon_greySliderRight = "menu/greySliderRight.png";
std::string g_icon_greySliderRightSel = "menu/greySliderRight_selected.png";
std::string g_img_menuBg = "menu/menuBg.png";
std::string g_icon_menuClose = "menu/menuClose.png";
std::string g_icon_menuCloseSel = "menu/menuClose_selected.png";
std::string g_img_panel1 = "menu/panel1.png";
std::string g_img_panel2 = "menu/panel2.png";

std::string g_imgSet_sprite1 = "player/sprite1.png";

std::string g_dir_mapRes = "map";

std::string g_txt_itemTable = "items/itemTable.csv";

float g_turnTimeLen = 0.2;

//在VizScene的loadMap里赋值
int g_mapTileW = 0;
int g_mapTileH = 0;

cocos2d::Scene* g_vizScene = nullptr;

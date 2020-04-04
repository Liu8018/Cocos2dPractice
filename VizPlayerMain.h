#ifndef VIZPLAYERMAIN_H
#define VIZPLAYERMAIN_H

#include "cocos2d.h"

class VizPlayerMain
{
public:
    VizPlayerMain();
    
    //返回sprite指针
    cocos2d::Sprite* getSprite();
    
    //播放行走动画
    void walk(int dX);
    
    //得到图像
    cocos2d::SpriteFrame* getStatFrame(bool);
    
private:
    cocos2d::Sprite* m_sprite;
    
    //左侧/右侧站立
    bool m_isLeftStat;
    
    //
    cocos2d::SpriteFrame* m_statFrameL;
    cocos2d::SpriteFrame* m_statFrameR;
};

#endif // VIZPLAYERMAIN_H

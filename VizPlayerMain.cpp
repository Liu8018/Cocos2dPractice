#include "VizPlayerMain.h"
#include "globals.h"

USING_NS_CC;

VizPlayerMain::VizPlayerMain()
{
    //读入图像资源集
    Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("res/sprite1.png");
    
    //获取左右帧
    m_statFrameL = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0,64,64,64)));
    m_statFrameR = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0,64*3,64,64)));
    
    //初始状态为左侧
    m_sprite = Sprite::createWithSpriteFrame(m_statFrameL);
    m_isLeftStat = true;
    
    //加载行走动画
    cocos2d::Vector<cocos2d::SpriteFrame*> framesWalkL;
    framesWalkL.reserve(10);
    for(int i=0;i<9;i++){
        auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(64*i, 64*9, 64, 64)));
        framesWalkL.pushBack(frame);
    }
    framesWalkL.pushBack(m_statFrameL);
    auto animationWalkL = Animation::createWithSpriteFrames(framesWalkL, g_turnTimeLen/9);
    AnimationCache::getInstance()->addAnimation(animationWalkL,"mainPlayerWalkL");
    
    cocos2d::Vector<cocos2d::SpriteFrame*> framesWalkR;
    framesWalkR.reserve(10);
    for(int i=0;i<9;i++){
        auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(64*i, 64*11, 64, 64)));
        framesWalkR.pushBack(frame);
    }
    framesWalkR.pushBack(m_statFrameR);
    auto animationWalkR = Animation::createWithSpriteFrames(framesWalkR, g_turnTimeLen/9);
    AnimationCache::getInstance()->addAnimation(animationWalkR, "mainPlayerWalkR");
}

SpriteFrame* VizPlayerMain::getStatFrame(bool side)
{
    if(side)
        return m_statFrameR;
    else
        return m_statFrameL;
}

Sprite* VizPlayerMain::getSprite()
{
    return m_sprite;
}

void VizPlayerMain::walk(int dX)
{
    if(dX > 0)
        m_isLeftStat = false;
    else if(dX < 0)
        m_isLeftStat = true;
    
    std::string walkSide;
    if(m_isLeftStat)
        walkSide = "mainPlayerWalkL";
    else
        walkSide = "mainPlayerWalkR";
    
    m_sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(walkSide)));
}

//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "FreeStuffLayer.h"
#include "Global.h"

FreeStuffLayer::FreeStuffLayer() : BaseMenuLayer() {
    m_Title->setText("free stuff");
    btn_back->setButtonClickSelector(wyTargetSelector::make(this, SEL(FreeStuffLayer::onCloseButtonClick)));
    btn_back->setTouchPriority(2003);
    
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shop_coin.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shop_coin.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    btn_coin = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(FreeStuffLayer::onCoinBtnClicked)), mask, NULL);
    btn_coin->setScaleX(sGlobal->scaleX*0.8f);
    btn_coin->setScaleY(sGlobal->scaleY*0.8f);
    btn_coin->setMultiTouchClickable(false);
    btn_coin->setInterceptTouch(true);
    btn_coin->setTouchPriority(1004);
    addChildLocked(btn_coin);
    btn_coin->setPosition(wyDevice::winWidth/2 - DP(120)*sGlobal->scaleX, wyDevice::winHeight/2+DP(36)*sGlobal->scaleY);
    
    wyBitmapFontLabel *btn_coin_title = wyBitmapFontLabel::make(sGlobal->font_desc, "free coin");
    btn_coin_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    btn_coin_title->setAlignment(wyBitmapFontLabel::LEFT);
    btn_coin_title->setPosition(btn_coin->getPositionX(), btn_coin->getPositionY()-DP(36)*sGlobal->scaleY);
    btn_coin_title->setAnchor(0.5f, 1.0f);
    addChildLocked(btn_coin_title);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shop_cash.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shop_cash.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    btn_cash = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(FreeStuffLayer::onCashBtnClicked)), mask, NULL);
    btn_cash->setScaleX(sGlobal->scaleX*0.8f);
    btn_cash->setScaleY(sGlobal->scaleY*0.8f);
    btn_cash->setMultiTouchClickable(false);
    btn_cash->setInterceptTouch(true);
    btn_cash->setTouchPriority(1004);
    addChildLocked(btn_cash);
    btn_cash->setPosition(wyDevice::winWidth/2 + DP(120)*sGlobal->scaleX, btn_coin->getPositionY());
    
    wyBitmapFontLabel *btn_cash_title = wyBitmapFontLabel::make(sGlobal->font_desc, "free cash");
    btn_cash_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    btn_cash_title->setAlignment(wyBitmapFontLabel::LEFT);
    btn_cash_title->setPosition(btn_cash->getPositionX(), btn_coin_title->getPositionY()-DP(0)*sGlobal->scaleY);
    btn_cash_title->setAnchor(0.5f, 1.0f);
    addChildLocked(btn_cash_title);
    
    this->setTouchEnabled(true);
    this->setTouchPriority(1003);
    this->setInterceptTouch(true);
    
    BlinkStar *star1 = new BlinkStar();
    star1->autoRelease();
    addChildLocked(star1);
    star1->posx1 = btn_coin->getPositionX()-DP(0)*sGlobal->scaleX;
    star1->posy1 = btn_coin->getPositionY()+DP(12)*sGlobal->scaleY;
    star1->posx2 = btn_coin->getPositionX()+DP(35)*sGlobal->scaleX,
    star1->posy2 = btn_coin->getPositionY()+DP(5)*sGlobal->scaleY;
    
    BlinkStar *star2 = new BlinkStar();
    star2->autoRelease();
    addChildLocked(star2);
    star2->posx1 = btn_cash->getPositionX()+DP(30)*sGlobal->scaleX;
    star2->posy1 = btn_cash->getPositionY()+DP(10)*sGlobal->scaleY;
    star2->posx2 = btn_cash->getPositionX()-DP(30)*sGlobal->scaleX,
    star2->posy2 = btn_cash->getPositionY()+DP(15)*sGlobal->scaleY;
}

void FreeStuffLayer::onEnter() {
    wyLayer::onEnter();
    
}

void FreeStuffLayer::onCloseButtonClick(wyTargetSelector* ts) {
    this->getParent()->removeChildLocked(this, true);
}

void FreeStuffLayer::onCoinBtnClicked(wyTargetSelector *ts) {
#if ANDROID
    showOfferWall(0);
#endif
}

void FreeStuffLayer::onCashBtnClicked(wyTargetSelector *ts) {
#if ANDROID
    showOfferWall(1);
#endif
}




BlinkStar::BlinkStar() {
    free_star = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_shine.png");
    free_star->setScaleX(sGlobal->scaleX*0.56f);
    free_star->setScaleY(sGlobal->scaleY*0.56f);
    addChildLocked(free_star,1);
    free_star->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    free_star->setVisible(false);
    starTimer = 0;

    starTimer = 0;
    nextStarTime = randRange(0, 1);
    
    wyTargetSelector* ts = wyTargetSelector::make(this, SEL(BlinkStar::updateTimer));
    wyTimer* t = wyTimer::make(ts);
    scheduleLocked(t);
}

BlinkStar::~BlinkStar() {
    
}

void BlinkStar::updateTimer(wyTargetSelector *ts) {
    float dt = ts->getDelta();
    
    starTimer += dt;
    if (starTimer > nextStarTime) {
        free_star->stopAllActions();
        float dur = randRange(1, 2);
        free_star->runAction(wySequence::make(wyShow::make(), wyRotateBy::make(dur*2, 360*2), wyHide::make(), NULL));
        float tx = sGlobal->scaleX*0.8f;
        free_star->runAction(wySequence::make(wyScaleTo::make(dur, 0,0, tx,tx), wyScaleTo::make(dur, tx,tx, 0,0), NULL));
        
        starTimer = 0;
        nextStarTime = randRange((int)dur*3, (int)dur*4);
        int seed = rand()%2;
        if (seed == 0) {
            free_star->setPosition(posx1, posy1);
        }
        else if (seed == 1) {
            free_star->setPosition(posx2, posy2);
        }
    }
}



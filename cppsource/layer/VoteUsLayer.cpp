//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "VoteUsLayer.h"
#include "Global.h"
#include "MainScene.h"
#include "LoadingScene.h"
#include "UpgradeScene.h"
#if ANDROID
#include "com_wiyun_engine_skeleton_Skeleton.h"
#endif
#if IOS
#import "ObjCCalls.h"
#endif
#include "DialogBgLayer.h"
#include "ShopLayer.h"

VoteUsLayer::VoteUsLayer() {
    wyColorLayer *bg1 = wyColorLayer::make();
    bg1->setColor(wyc4b(0, 0, 0, 100));
    bg1->setPosition(0, 0);
    addChildLocked(bg1);
    
    container = wyNode::make();
    container->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    addChildLocked(container);
    
    DialogBgLayer *bgLayer = DialogBgLayer::make();
    bgLayer = DialogBgLayer::make();
    bgLayer->setScaleX(0.9f);
    bgLayer->setScaleY(0.7f);
    bgLayer->setAnchor(0, 0);
    container->addChildLocked(bgLayer);
    
    
    wyBitmapFontLabel *content = wyBitmapFontLabel::make(sGlobal->font_desc, "resident zombies is an awesome game,\nisn't it?\nwould you please give us a 5-star rating?");
    content->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    content->setAlignment(wyBitmapFontLabel::LEFT);
    content->setLineWidth(DP(400)*sGlobal->scaleX/content->getScale());
    content->setPosition(DP(-180)*sGlobal->scaleX, DP(95)*sGlobal->scaleY);
    content->setAnchor(0, 1.0f);
    container->addChildLocked(content);
    
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    wyBitmapFontLabel *title = wyBitmapFontLabel::make(sGlobal->font_desc, "yes");
    title->setScale(SP(24)/sGlobal->descFontSize);
    yesBtn = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(VoteUsLayer::onYesButtonClick)), mask, title);
    yesBtn->setScaleX(sGlobal->scaleX*0.8f);
    yesBtn->setScaleY(sGlobal->scaleY*0.8f);
    yesBtn->setMultiTouchClickable(false);
    yesBtn->setInterceptTouch(true);
    yesBtn->setTouchPriority(10000);
    container->addChildLocked(yesBtn, 100);
    yesBtn->setPosition(-DP(140)*sGlobal->scaleX, -DP(50)*sGlobal->scaleY);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "later");
    title->setScale(SP(24)/sGlobal->descFontSize);
    laterBtn = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(VoteUsLayer::onLaterButtonClick)), mask, title);
    laterBtn->setScaleX(sGlobal->scaleX*0.8f);
    laterBtn->setScaleY(sGlobal->scaleY*0.8f);
    laterBtn->setMultiTouchClickable(false);
    laterBtn->setInterceptTouch(true);
    laterBtn->setTouchPriority(10000);
    container->addChildLocked(laterBtn, 100);
    laterBtn->setPosition(yesBtn->getPositionX()+DP(152)*sGlobal->scaleX, yesBtn->getPositionY());
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "never");
    title->setScale(SP(24)/sGlobal->descFontSize);
    neverBtn = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(VoteUsLayer::onNeverButtonClick)), mask, title);
    neverBtn->setScaleX(sGlobal->scaleX*0.8f);
    neverBtn->setScaleY(sGlobal->scaleY*0.8f);
    neverBtn->setMultiTouchClickable(false);
    neverBtn->setInterceptTouch(true);
    neverBtn->setTouchPriority(10000);
    container->addChildLocked(neverBtn, 100);
    neverBtn->setPosition(laterBtn->getPositionX()+DP(152)*sGlobal->scaleX, laterBtn->getPositionY());
    
    this->setTouchEnabled(true);
    this->setTouchPriority(1001);
    this->setInterceptTouch(true);
    
    isEnterAnimEnd = false;

    container->setScale(0);
    
    int rateTimes = wyPrefs::getInt(RATEME_TIMES, 0) + 1;
    wyPrefs::setInt(RATEME_TIMES, rateTimes);
#if ANDROID
    sGlobal->reportGameEvent(EVT_RATEME_SHOW, rateTimes);
#endif
}

static void onEnterAnimEnd(wyAction* action, void* data) {
    VoteUsLayer *layer = (VoteUsLayer *)data;
    
    layer->isEnterAnimEnd = true;
}

void VoteUsLayer::onEnter() {
    wyLayer::onEnter();
    
    wyIntervalAction* a2 = wyScaleTo::make(0.2f, 0.8f, 0.8f, 1.0f, 1.0f);
    wyIntervalAction *a21 = wyEaseBackOut::make(a2);

    wyActionCallback callback = {
        NULL,
        onEnterAnimEnd,
        NULL
    };
    a21->setCallback(&callback, this);
    container->runAction(a21);
    container->setScale(0.8f);
    
    isEnterAnimEnd = true;
}


bool VoteUsLayer::touchesBegan(wyMotionEvent& event) {
//    if (isEnterAnimEnd) {
//        this->getParent()->removeChildLocked(this, true);
//    }
    
    return true;
}

bool VoteUsLayer::touchesMoved(wyMotionEvent& event) {
//    this->getParent()->removeChildLocked(this, true);
    return true;
}

bool VoteUsLayer::touchesEnded(wyMotionEvent& event) {
//    this->getParent()->removeChildLocked(this, true);
    return true;
}

void VoteUsLayer::onCloseButtonClick(wyTargetSelector* ts) {
//    sGlobal->playEffect("se_button.mp3");
    
    if (isEnterAnimEnd) {
        this->getParent()->removeChildLocked(this, true);
    }
}

void VoteUsLayer::onYesButtonClick(wyTargetSelector* ts) {
#if ANDROID
    showRateMeGoogle();
    sGlobal->isRatingMe = true;
    
    sGlobal->reportGameEvent(EVT_RATEME_YES, wyPrefs::getInt(RATEME_TIMES, 0));
#endif
    wyPrefs::setInt(RATEME_YES, 1);
    this->getParent()->removeChildLocked(this, true);
}

void VoteUsLayer::onLaterButtonClick(wyTargetSelector* ts) {
#if ANDROID
    sGlobal->reportGameEvent(EVT_RATEME_LATER, wyPrefs::getInt(RATEME_TIMES, 0));
#endif
    wyPrefs::setInt(RATEME_LATER, 1);
    this->getParent()->removeChildLocked(this, true);
}

void VoteUsLayer::onNeverButtonClick(wyTargetSelector* ts) {
#if ANDROID
    sGlobal->reportGameEvent(EVT_RATEME_NEVER, wyPrefs::getInt(RATEME_TIMES, 0));
#endif
    wyPrefs::setInt(RATEME_NEVER, 1);
    this->getParent()->removeChildLocked(this, true);
}





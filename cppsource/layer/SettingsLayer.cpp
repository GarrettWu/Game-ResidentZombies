//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "SettingsLayer.h"
#include "Global.h"

SettingsLayer::SettingsLayer() : BaseMenuLayer() {
    this->setTouchEnabled(true);
    this->setTouchPriority(2000);
    this->setInterceptTouch(true);
    
    m_Title->setText("settings");
    btn_back->setButtonClickSelector(wyTargetSelector::make(this, SEL(SettingsLayer::onCloseButtonClick)));
    btn_back->setTouchPriority(2003);
    
    wySprite *normal;
    normal = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "toggle_se.png");
    se_toggle = wyMenuItemSprite::make(NULL,
                                       wyTargetSelector::make(this, SEL(SettingsLayer::onSEClicked)),
                                       normal,
                                       NULL,
                                       NULL);
    se_toggle->setPosition(wyDevice::winWidth/2-DP(120)*sGlobal->scaleX, wyDevice::winHeight/2+DP(36)*sGlobal->scaleY);
    se_toggle->setClickScale(1.0f);
    se_toggle->setScale(sGlobal->scaleX*0.6);
    
    normal = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "toggle_bgm.png");
    bgm_toggle = wyMenuItemSprite::make(NULL,
                                        wyTargetSelector::make(this, SEL(SettingsLayer::onBGMClicked)),
                                        normal,
                                        NULL,
                                        NULL);
    bgm_toggle->setPosition(wyDevice::winWidth/2+DP(120)*sGlobal->scaleX, se_toggle->getPositionY());
    bgm_toggle->setClickScale(1.0f);
    bgm_toggle->setScale(sGlobal->scaleX*0.6);
    
    menu1 = wyMenu::make(bgm_toggle, se_toggle, NULL);
    menu1->setPosition(0, 0);
    menu1->setAnchor(0, 0);
    menu1->setTouchPriority(99999);
    menu1->setInterceptTouch(true);
    addChildLocked(menu1);
    
    se_disable = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "sound_disable.png");
    se_disable->setPosition(se_toggle->getPositionX(), se_toggle->getPositionY());
    se_disable->setScale(sGlobal->scaleX*0.6);
    addChildLocked(se_disable);
    se_disable->setVisible(!sGlobal->isSFXOn);
    
    bgm_disable = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "sound_disable.png");
    bgm_disable->setPosition(bgm_toggle->getPositionX(), bgm_toggle->getPositionY());
    bgm_disable->setScale(sGlobal->scaleX*0.6);
    addChildLocked(bgm_disable);
    bgm_disable->setVisible(!sGlobal->isBGMOn);
    
    wyBitmapFontLabel *btn_sfx_title = wyBitmapFontLabel::make(sGlobal->font_desc, "sound effects");
    btn_sfx_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    btn_sfx_title->setAlignment(wyBitmapFontLabel::LEFT);
    btn_sfx_title->setPosition(se_toggle->getPositionX(), se_toggle->getPositionY()-DP(44)*sGlobal->scaleY);
    btn_sfx_title->setAnchor(0.5f, 1.0f);
    addChildLocked(btn_sfx_title);
    
    wyBitmapFontLabel *btn_bgm_title = wyBitmapFontLabel::make(sGlobal->font_desc, "background music");
    btn_bgm_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    btn_bgm_title->setAlignment(wyBitmapFontLabel::LEFT);
    btn_bgm_title->setPosition(bgm_toggle->getPositionX(), bgm_toggle->getPositionY()-DP(44)*sGlobal->scaleY);
    btn_bgm_title->setAnchor(0.5f, 1.0f);
    addChildLocked(btn_bgm_title);
}

void SettingsLayer::onEnter() {
    wyLayer::onEnter();
    
}

void SettingsLayer::onCloseButtonClick(wyTargetSelector* ts) {
    this->getParent()->removeChildLocked(this, true);
}

void SettingsLayer::onBGMClicked(wyTargetSelector* ts) {
    sGlobal->playEffect(se_button);
    
    if (sGlobal->isBGMOn) {
        sGlobal->stopBGM();
        sGlobal->isBGMOn = false;
        wyPrefs::setBool(DB_IS_BGM_ON, sGlobal->isBGMOn);
        
        bgm_disable->setVisible(true);
    } else {
        sGlobal->isBGMOn = true;
        int prevId = sGlobal->currentBGMId;
        sGlobal->currentBGMId = -1;
        sGlobal->playBGM(prevId);
        wyPrefs::setBool(DB_IS_BGM_ON, sGlobal->isBGMOn);
        
        bgm_disable->setVisible(false);
    }
}

void SettingsLayer::onSEClicked(wyTargetSelector* ts) {
    if (sGlobal->isSFXOn) {
        sGlobal->isSFXOn = false;
        wyPrefs::setBool(DB_IS_SFX_ON, sGlobal->isSFXOn);
        
        se_disable->setVisible(true);
    } else {
        sGlobal->isSFXOn = true;
        wyPrefs::setBool(DB_IS_SFX_ON, sGlobal->isSFXOn);
        
        se_disable->setVisible(false);
    }
    
    sGlobal->playEffect(se_button);
}

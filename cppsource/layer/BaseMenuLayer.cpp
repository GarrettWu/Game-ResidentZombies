//
//  AllMenusLayer.cpp
//  NinjaCat_ios
//
//  Created by xu songlin on 12-2-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "BaseMenuLayer.h"
#include "Global.h"

BaseMenuLayer::BaseMenuLayer() {
    wySpriteEx *m_bg = wySpriteEx::make(MyTextureManager::makePNG("sc_upgrade_bg.png"));
    m_bg->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    m_bg->setScaleX(wyDevice::winWidth/m_bg->getWidth());
    m_bg->setScaleY(wyDevice::winHeight/m_bg->getHeight());
    addChildLocked(m_bg);
    
    h_line = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "sc_upgrade_bg_line_1.png");//wySpriteEx::make(MyTextureManager::makePNG("sc_upgrade_bg_line_1.png"));
    h_line->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    h_line->setScaleX(m_bg->getScaleX());
    h_line->setScaleY(m_bg->getScaleY());
    h_line->setAnchor(0.5, 0);
    h_line->setPosition(wyDevice::winWidth/2, DP(78)*h_line->getScaleY());
    addChildLocked(h_line);
    
    m_Title = wyBitmapFontLabel::make(sGlobal->font_title, "a");
    m_Title->setScale(SP(20)*sGlobal->scaleX/sGlobal->descFontSize);
    m_Title->setAlignment(wyBitmapFontLabel::LEFT);
    m_Title->setPosition(DP(15)*sGlobal->scaleX, wyDevice::winHeight-DP(28)*sGlobal->scaleY);
    m_Title->setAnchor(0, 0.5f);
    addChildLocked(m_Title);
    
    createMoneyPanel();
    
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "jump.png");//wySpriteEx::make(MyTextureManager::makePNG("jump.png"));
    normal->setFlipX(true);
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "jump.png");//wySpriteEx::make(MyTextureManager::makePNG("jump.png"));
    mask->setFlipX(true);
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    btn_back = ControlButton::make(normal, NULL, NULL, NULL, NULL, NULL, mask, NULL);
    btn_back->setScaleX(sGlobal->scaleX*0.7f);
    btn_back->setScaleY(sGlobal->scaleY*0.7f);
    btn_back->setPosition(DP(55)*sGlobal->scaleX, DP(38)*sGlobal->scaleY);
    btn_back->setMultiTouchClickable(false);
    btn_back->setInterceptTouch(true);
    btn_back->setTouchPriority(1002);
    btn_back->setRotation(-90);
    addChildLocked(btn_back);
    btn_back->needDelay = false;
    
    this->setTouchEnabled(true);
    this->setTouchPriority(1001);
    this->setInterceptTouch(true);
    
    sGlobal->addCoinChangeListener(this);
}

BaseMenuLayer::~BaseMenuLayer() {
    sGlobal->removeCoinChangeListener(this);
}

void BaseMenuLayer::createMoneyPanel() {
    wySpriteEx *coin_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");//wySpriteEx::make(MyTextureManager::makePNG("coin_icon.png"));
    coin_icon->setScaleX(sGlobal->scaleX*0.7f);
    coin_icon->setScaleY(sGlobal->scaleY*0.7f);
    coin_icon->setPosition(wyDevice::winWidth-DP(160)*sGlobal->scaleX, wyDevice::winHeight-DP(15)*sGlobal->scaleY);
    addChildLocked(coin_icon);
    
    coin_num = wyBitmapFontLabel::make(sGlobal->font_desc, "0");
    coin_num->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    coin_num->setAlignment(wyBitmapFontLabel::LEFT);
    coin_num->setPosition(coin_icon->getPositionX()+DP(20)*sGlobal->scaleX, coin_icon->getPositionY());
    coin_num->setAnchor(0, 0.5f);
    addChildLocked(coin_num);
    
    wySpriteEx *cash_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cash_icon.png");//wySpriteEx::make(MyTextureManager::makePNG("cash_icon.png"));
    cash_icon->setScaleX(sGlobal->scaleX*0.7f);
    cash_icon->setScaleY(sGlobal->scaleY*0.7f);
    cash_icon->setPosition(coin_icon->getPositionX()-DP(0)*sGlobal->scaleX, coin_icon->getPositionY()-DP(25)*sGlobal->scaleY);
    addChildLocked(cash_icon);
    
    cash_num = wyBitmapFontLabel::make(sGlobal->font_desc, "0");
    cash_num->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    cash_num->setAlignment(wyBitmapFontLabel::LEFT);
    cash_num->setPosition(cash_icon->getPositionX()+DP(20)*sGlobal->scaleX, cash_icon->getPositionY());
    cash_num->setAnchor(0, 0.5f);
    addChildLocked(cash_num);
    
    updateCoin();
    updateCash();
}

void BaseMenuLayer::updateCoin() {
    char temp[30];
    addComma(sGlobal->getGold(), temp);
    coin_num->setText(temp);
    
    MyPrefs::setInt(DB_GOLD, sGlobal->getGold());
}

void BaseMenuLayer::updateCash() {
    char temp[30];
	addComma(sGlobal->zuanshi.getValue(), temp);
    cash_num->setText(temp);
    
    MyPrefs::setInt(DB_ZUANSHI_NUM, sGlobal->zuanshi.getValue());
}

void BaseMenuLayer::onCoinChanged() {
    char temp[30];
    addComma(sGlobal->getGold(), temp);
    coin_num->setText(temp);
    
	addComma(sGlobal->zuanshi.getValue(), temp);
    cash_num->setText(temp);
}




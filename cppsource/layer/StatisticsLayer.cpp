//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "StatisticsLayer.h"
#include "Global.h"

StatisticsLayer::StatisticsLayer() : BaseMenuLayer() {
    m_Title->setText("statistics");
    btn_back->setButtonClickSelector(wyTargetSelector::make(this, SEL(StatisticsLayer::onCloseButtonClick)));
    btn_back->setTouchPriority(2003);
    
    this->setTouchEnabled(true);
    this->setTouchPriority(2000);
    this->setInterceptTouch(true);
    
    wyColor3B color = wyc3b(220, 220, 220);
    
    storymode_title = wyBitmapFontLabel::make(sGlobal->font_desc, "story mode:");
    storymode_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    storymode_title->setAlignment(wyBitmapFontLabel::LEFT);
    storymode_title->setPosition(wyDevice::winWidth/2-DP(50)*sGlobal->scaleX, DP(330)*sGlobal->scaleY);
    storymode_title->setAnchor(1.0f, 0.5f);
    addChildLocked(storymode_title);
    storymode_title->setColor(color);
    
    storymode_number = wyBitmapFontLabel::make(sGlobal->font_desc, "10/20");
    storymode_number->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    storymode_number->setAlignment(wyBitmapFontLabel::LEFT);
    storymode_number->setPosition(storymode_title->getPositionX()+DP(10)*sGlobal->scaleX, storymode_title->getPositionY());
    storymode_number->setAnchor(0, 0.5f);
    addChildLocked(storymode_number);
    storymode_number->setColor(color);
    
    s_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "score_s_small.png");
    s_icon->setScaleX(sGlobal->scaleX*0.8);
    s_icon->setScaleY(sGlobal->scaleY*0.8);
//    s_icon->setPosition(storymode_number->getPositionX()+DP(80)*sGlobal->scaleX, storymode_number->getPositionY()+DP(2)*sGlobal->scaleX);
    addChildLocked(s_icon);
    
    endlessmode_title = wyBitmapFontLabel::make(sGlobal->font_desc, "endless highest score:");
    endlessmode_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    endlessmode_title->setAlignment(wyBitmapFontLabel::LEFT);
    endlessmode_title->setPosition(storymode_title->getPositionX(), storymode_title->getPositionY()-DP(40)*sGlobal->scaleY);
    endlessmode_title->setAnchor(1.0f, 0.5f);
    addChildLocked(endlessmode_title);
    endlessmode_title->setColor(color);
    
    endlessmode_number = wyBitmapFontLabel::make(sGlobal->font_desc, "1234567");
    endlessmode_number->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    endlessmode_number->setAlignment(wyBitmapFontLabel::LEFT);
    endlessmode_number->setPosition(endlessmode_title->getPositionX()+DP(10)*sGlobal->scaleX, endlessmode_title->getPositionY());
    endlessmode_number->setAnchor(0, 0.5f);
    addChildLocked(endlessmode_number);
    endlessmode_number->setColor(color);
    
    kills_title = wyBitmapFontLabel::make(sGlobal->font_desc, "total kills:");
    kills_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    kills_title->setAlignment(wyBitmapFontLabel::LEFT);
    kills_title->setPosition(storymode_title->getPositionX(), storymode_title->getPositionY()-DP(40)*sGlobal->scaleY*2);
    kills_title->setAnchor(1.0f, 0.5f);
    addChildLocked(kills_title);
    kills_title->setColor(color);
    
    kills_number = wyBitmapFontLabel::make(sGlobal->font_desc, "1200");
    kills_number->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    kills_number->setAlignment(wyBitmapFontLabel::LEFT);
    kills_number->setPosition(kills_title->getPositionX()+DP(10)*sGlobal->scaleX, kills_title->getPositionY());
    kills_number->setAnchor(0, 0.5f);
    addChildLocked(kills_number);
    kills_number->setColor(color);
    
    bosses_title = wyBitmapFontLabel::make(sGlobal->font_desc, "killed bosses:");
    bosses_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    bosses_title->setAlignment(wyBitmapFontLabel::LEFT);
    bosses_title->setPosition(storymode_title->getPositionX(), storymode_title->getPositionY()-DP(40)*sGlobal->scaleY*3);
    bosses_title->setAnchor(1.0f, 0.5f);
    addChildLocked(bosses_title);
    bosses_title->setColor(color);
    
    bosses_number = wyBitmapFontLabel::make(sGlobal->font_desc, "120");
    bosses_number->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    bosses_number->setAlignment(wyBitmapFontLabel::LEFT);
    bosses_number->setPosition(bosses_title->getPositionX()+DP(10)*sGlobal->scaleX, bosses_title->getPositionY());
    bosses_number->setAnchor(0, 0.5f);
    addChildLocked(bosses_number);
    bosses_number->setColor(color);
    
    coins_title = wyBitmapFontLabel::make(sGlobal->font_desc, "collected coins:");
    coins_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    coins_title->setAlignment(wyBitmapFontLabel::LEFT);
    coins_title->setPosition(storymode_title->getPositionX(), storymode_title->getPositionY()-DP(40)*sGlobal->scaleY*4);
    coins_title->setAnchor(1.0f, 0.5f);
    addChildLocked(coins_title);
    coins_title->setColor(color);
    
    coins_number = wyBitmapFontLabel::make(sGlobal->font_desc, "3000");
    coins_number->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    coins_number->setAlignment(wyBitmapFontLabel::LEFT);
    coins_number->setPosition(coins_title->getPositionX()+DP(10)*sGlobal->scaleX, coins_title->getPositionY());
    coins_number->setAnchor(0, 0.5f);
    addChildLocked(coins_number);
    coins_number->setColor(color);
    
    icon_gold = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
    icon_gold->setScaleX(sGlobal->scaleX*0.7f);
    icon_gold->setScaleY(sGlobal->scaleY*0.7f);
    icon_gold->setAnchor(0.0f, 0.5f);
    addChildLocked(icon_gold);
    icon_gold->setColor(color);
    
    distance_title = wyBitmapFontLabel::make(sGlobal->font_desc, "endless distance:");
    distance_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    distance_title->setAlignment(wyBitmapFontLabel::LEFT);
    distance_title->setPosition(storymode_title->getPositionX(), storymode_title->getPositionY()-DP(40)*sGlobal->scaleY*5);
    distance_title->setAnchor(1.0f, 0.5f);
    addChildLocked(distance_title);
    distance_title->setColor(color);
    
    distance_number = wyBitmapFontLabel::make(sGlobal->font_desc, "122200m");
    distance_number->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    distance_number->setAlignment(wyBitmapFontLabel::LEFT);
    distance_number->setPosition(distance_title->getPositionX()+DP(10)*sGlobal->scaleX, distance_title->getPositionY());
    distance_number->setAnchor(0, 0.5f);
    addChildLocked(distance_number);
    distance_number->setColor(color);
    
    calcAll();
}

void StatisticsLayer::onEnter() {
    wyLayer::onEnter();
    
}

void StatisticsLayer::onCloseButtonClick(wyTargetSelector* ts) {
    this->getParent()->removeChildLocked(this, true);
}

void StatisticsLayer::calcAll() {
    int scount = 0;
    for (int i = 1; i <= 3; i++) {
        for (int j = 0; j <= 2; j++) {
            scount += sGlobal->countScount(i, j);
        }
    }
    char temp[50];
    sprintf(temp, "%d/90", scount);
    storymode_number->setText(temp);
    s_icon->setPosition(storymode_number->getPositionX()+storymode_number->getWidth()*storymode_number->getScaleX()+DP(16)*sGlobal->scaleX, storymode_number->getPositionY()-DP(0)*sGlobal->scaleX);
    
    int highscore = MAX(MAX(MAX(sGlobal->distRecNormal, sGlobal->distRecHeroic), sGlobal->distRecInfernal), 0);
    addComma(highscore, temp);
    endlessmode_number->setText(temp);
    
    addComma(sGlobal->totalKilledEnemies, temp);
    kills_number->setText(temp);
    
    addComma(sGlobal->killedBosses, temp);
    bosses_number->setText(temp);
    
    addComma(sGlobal->totalCollectedCoins, temp);
    coins_number->setText(temp);
    icon_gold->setPosition(coins_number->getPositionX()+coins_number->getWidth()*coins_number->getScaleX()+DP(0)*sGlobal->scaleX, coins_number->getPositionY());
    
    addComma(sGlobal->totalRunDistance, temp);
    char temp1[50];
    sprintf(temp1, "%sm", temp);
    distance_number->setText(temp1);
}





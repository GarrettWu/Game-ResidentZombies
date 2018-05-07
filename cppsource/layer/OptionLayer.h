//
//  MainGameLayer.h
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "WiEngine.h"
#include "DialogBgLayer.h"
#include "ControlButton.h"
#include "QuestLayer.h"
#include "CoinChangeListener.h"

class OptionLayer : public wyColorLayer {
public:
    OptionLayer();
    virtual ~OptionLayer();
    
    void onResumeClicked(wyTargetSelector* ts);
    void onQuitClicked(wyTargetSelector* ts);
    void onRestartClicked(wyTargetSelector* ts);
    
    void onSEClicked(wyTargetSelector* ts);
    void onBGMClicked(wyTargetSelector* ts);
    
    void onEnter();
    void onEnterTransitionDidFinish();

    wyNode *container;
    
    wyMenu *menu1, *menu2;
    
    wyMenuItemSprite *se_toggle, *bgm_toggle;
    wySprite *se_disable, *bgm_disable;
    
    DialogBgLayer *bgLayer;

    wyBitmapFontLabel *option_distance, *option_bosses, *option_score;
    wyBitmapFontLabel *number_distance, *number_bosses, *number_score;
    void calcAllForEndless();
    
    wyBitmapFontLabel *option_gold, *option_kills, *option_remain_hp, *option_totalgold;
    wyBitmapFontLabel *number_gold, *number_kills, *number_remain_hp, *number_totalgold;
    void createDataPanel();
    void calcAllForNormal();
    
    wySpriteEx *icon_totalgold, *icon_gold;
    
    ControlButton *btn_resume, *btn_restart, *btn_quit;
    
    QuestLayer *qLayer;
    
    wyBitmapFontLabel *title;
    void initCommonUI();
    void initNormalUI();
    void initEndlessUI();
    
    void clearOneRunMissionStatus();
    
    wyBitmapFontLabel *goldMultiplyer;
    void onWenhaoClicked(wyTargetSelector *ts);
    ControlButton *btn_wenhao;
    
//    void onBuyDBCoin(wyTargetSelector *ts);
//    void onPurchaseEnded(int itemId);
};


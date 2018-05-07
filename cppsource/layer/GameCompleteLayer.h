//
//  MainGameLayer.h
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "WiEngine.h"
#include "ControlButton.h"

class GameCompleteLayer : public wyColorLayer {
public:
    bool needForceNext;
    GameCompleteLayer(bool needForceNext = false, int tabIndex = 1);
    virtual ~GameCompleteLayer();
    
    void onNextClicked(wyTargetSelector* ts);
    void onQuitClicked(wyTargetSelector* ts);
    void onRestartClicked(wyTargetSelector* ts);

    void onEnter();

    void gradeShake();
    wyNode *container;
    
    wySpriteEx *grade;
    
    wyBitmapFontLabel *option_gold, *option_kills, *option_remain_hp, *option_total, *option_score, *option_totalgold, *option_cash;
    wyBitmapFontLabel *number_gold, *number_kills, *number_remain_hp, *number_total, *number_score, *number_totalgold, *number_cash;
    
    wySpriteEx *icon_totalgold, *icon_gold, *icon_cash;
    
    wyTimer *countDownTimer;
    
    virtual bool touchesBegan(wyMotionEvent& e);
    
    void updatePrefs();
    
    wyBitmapFontLabel* title;
    void calcAll();
    int totalGold, gradeLevel, totalScore;
    
    void startAnimation();
    
    ControlButton *btn_restart, *btn_quit, *btn_next;
    
    void onEnterTransitionDidFinish();
    
    void countDownScore();
    
    void changeColor0(wyTargetSelector *ts);
    void changeColor1(wyTargetSelector *ts);
    void changeColor2(wyTargetSelector *ts);
    void changeColor3(wyTargetSelector *ts);
    
    float scoreTimer, totalDur;
    wyTimer* stimer;
    int curScore;
    void updateScoreTimer(wyTargetSelector *ts);
    
    wyBitmapFontLabel *goldMultiplyer;
    void onWenhaoClicked(wyTargetSelector *ts);
    ControlButton *btn_wenhao;
};


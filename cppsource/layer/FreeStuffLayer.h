//
//  MainGameLayer.h
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "WiEngine.h"
#include "BaseMenuLayer.h"

class BlinkStar : public wyNode {
public:
    BlinkStar();
    
    ~BlinkStar();
    
    wySpriteEx *free_star;
    float starTimer, nextStarTime;
    void updateTimer(wyTargetSelector *ts);
    
    float posx1, posy1;
    float posx2, posy2;
    float posx3, posy3;
};

class FreeStuffLayer : public BaseMenuLayer {
public:
    FreeStuffLayer();
    
    ~FreeStuffLayer() {
        
    }
    
    void onEnter();
    
    void onCloseButtonClick(wyTargetSelector* ts);
    
    ControlButton* btn_coin, *btn_cash;
    
    void onCoinBtnClicked(wyTargetSelector *ts);
    void onCashBtnClicked(wyTargetSelector *ts);
};

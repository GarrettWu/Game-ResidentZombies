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

class StatisticsLayer : public BaseMenuLayer {
public:
    StatisticsLayer();
    
    ~StatisticsLayer() {
        
    }
    
    void onEnter();
    
    void onCloseButtonClick(wyTargetSelector* ts);
    
    wyBitmapFontLabel *storymode_title, *endlessmode_title, *kills_title, *bosses_title, *coins_title, *distance_title;
    wyBitmapFontLabel *storymode_number, *endlessmode_number, *kills_number, *bosses_number, *coins_number, *distance_number;
    wySpriteEx *icon_gold, *s_icon;
    
    void calcAll();
};

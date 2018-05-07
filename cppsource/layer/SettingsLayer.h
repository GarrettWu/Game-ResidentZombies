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

class SettingsLayer : public BaseMenuLayer {
public:
    SettingsLayer();
    
    ~SettingsLayer() {
        
    }
    
    void onEnter();
    
    void onCloseButtonClick(wyTargetSelector* ts);
    
    wyMenuItemSprite *se_toggle, *bgm_toggle;
    wySprite *se_disable, *bgm_disable;
    
    wyMenu *menu1;
    
    void onSEClicked(wyTargetSelector* ts);
    void onBGMClicked(wyTargetSelector* ts);
};

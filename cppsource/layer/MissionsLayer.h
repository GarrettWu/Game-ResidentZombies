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
#include "BaseMenuLayer.h"

static const int TAB_MISSION_CURRENT = 0;
static const int TAB_MISSION_COMPLETE = 1;

class MissionsLayer : public BaseMenuLayer, public wyScrollableLayerListener {
public:
    MissionsLayer();
    
    ~MissionsLayer() {
        
    }
    
    void onEnter();
    
    void onCloseButtonClick(wyTargetSelector* ts);
    
    void createTabs();
    void createCurrentMissionsLayer();
    void createCompleteMissionsLayer();
    
    void onTabCurrentClick(wyTargetSelector* ts);
    void onTabCompleteClick(wyTargetSelector* ts);
    
    ControlButton *tab_current, *tab_complete;
    
    int curTabIndex;
    
    wyBitmapFontLabel *test_Title;
    
    wyNode* createMission(int index, int missionId, bool isMid);
    void updateMission(wyNode *container, int mid);
    
    wyNode *currentMissionLayer;
    wyScrollableLayer *completeMissionLayer;
    wyNinePatchSprite* vThumb;
    
    void onScrollOffsetChanged(wyScrollableLayer* layer);
};

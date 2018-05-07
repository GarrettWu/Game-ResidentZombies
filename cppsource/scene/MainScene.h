//
//  MainScene.h
//  NinjaCat
//
//  Created by xu songlin on 12-1-10.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "WiEngine.h"

const static int SC_NORMAL = 0;
const static int SC_OPTION_OPENED = 1;
const static int SC_DEAD_OPENED = 2;
const static int SC_COMPLETE_OPEND = 3;

class MainScene : public wyScene {
public:
    MainScene(int mapIndex, bool isFromCheckPoint = false);
    virtual ~MainScene();

	int mapIndex;
    
    void onEnter();
    void onEnterTransitionDidFinish();
    
    bool isFromCheckPoint;
    void reset(bool isFromCheckPoint);

    wyScene *nextScene;
    
    int sceneState;
    bool keyUp(wyKeyEvent& event);
    void switchSeaon(int seasonId);

    void quit();
    bool isQuiting;
    
    void playbgmWithDelay(wyTargetSelector* ts);
    
    int prevWeapon, prevRifleGrade, prevRifleAmmoNumber, prevSkill;
    void clearTutorialStatus();
};


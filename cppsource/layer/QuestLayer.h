//
//  MainGameLayer.h
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "WiEngine.h"

static const int B_BLACK = 0;
static const int B_WHITE = 1;
static const int B_RED = 2;

static const int TAG_TITLE = 0;
static const int TAG_BADGE = 1;
static const int TAG_PROGRESS_BAR = 2;
static const int TAG_PROGRESS_TITLE = 3;
static const int TAG_CASH_NUM = 4;
static const int TAG_CASH_ICON = 5;

class QuestLayer : public wyColorLayer {
public:
    bool m_isDead;
    QuestLayer(bool isDead);
    virtual ~QuestLayer();
    
    wyNode *container;
    
    virtual bool touchesBegan(wyMotionEvent& e);
    void onEnter();
    void createMission(int index, bool isNext = false);
    void updateMission(int index, int mid);
    void startEnterAnimation();
    void startAnimation();
    void refreshMissionIds();
    
    void startQuestAnim(int index);

    void selectBorder(int index, int status);
    void runProgressEffect();
    
    int oldm1, oldm2, oldm3;
    int curMid, curIndex, curMissionId, nextMissionId;
    int from, to, target;
    wyBitmapFontLabel *progressTitle;
    void updateProgressTitle(float t);
    void onFinishProgressAnim();
    bool isAnimFinished;
    
    bool isQuiting;
    
    void placeAllThings();
    
    wyTargetSelector *onEndSelector;
    void setQuestEndSelector(wyTargetSelector *selector);
};


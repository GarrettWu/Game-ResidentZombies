//
//  MainGameLayer.h
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "WiEngine.h"
#include "QuestLayer.h"
#include "ShopLayer.h"

class DeadNoticeLayer : public wyColorLayer, public ShopListener {
public:
    DeadNoticeLayer(int canSave);
    virtual ~DeadNoticeLayer();

    void onQuitClicked(wyTargetSelector* ts);
    void onRestartClicked(wyTargetSelector* ts);
    void onReviveClicked(wyTargetSelector* ts);

    void onEnter();

    wyNode *container;
    
    virtual bool touchesBegan(wyMotionEvent& e);
    
    wyBitmapFontLabel *title;
    
    bool isHaltDeadNoticeShow;
    wyNode *saveMe;
    ControlButton *saveMeBtn;
    wyProgressTimer *saveme_progress;
    float savemeDeltaTime;
    bool needDestroyTimer;
    bool isSaveMeShowEnd, isDeadNoticeShowed, isSaveMeClicked, isBuyGold;
    bool isReviveBuyGold;
    void onSaveMeClicked(wyTargetSelector* ts);
    void showDeadNotice();
    void runSaveMeTimer();
    void saveMeTimerUpdate(wyTargetSelector* ts);
    
    wyTimer *timer;
    bool isTimerPaused;

	wyBitmapFontLabel *gold_number;
	void updateGold();
    void onShopEnd(int result);

    int saveType;
    
    void onEnterTransitionDidFinish();
    void onCloseButtonClick(wyTargetSelector* ts);
    void updateZuanshi();
    
    void initCommonUI();
    void initNormalDeadUI();
    void initEndlessDeadUI();
    
    void calcAll();
    wyBitmapFontLabel *option_distance, *option_kills, *option_bosses, *option_gold, *option_score;
    wyBitmapFontLabel *number_distance, *number_kills, *number_bosses, *number_gold, *number_score;
    
    void createMissionContainer(int i);
    QuestLayer *qLayer;
    
    void showCountDown(wyTargetSelector *target);
    wyBitmapFontLabel* number1;
//    wyLabel *number1;
    void count2();
    void count1();
    void go();
    void gone();
    
    ControlButton *btn_restart, *btn_quit;
    void onGotoUpgradeBtnClick(wyTargetSelector* ts);
    void onGotoUpgradeOk(wyTargetSelector* ts, void *data);
    
    void onQuestEnded(wyTargetSelector* ts);
    
    bool isHighScore;
    wySpriteEx *highestscore;
    void gradeShake();
    
    int totalScore;
    
    void onGPlusButtonClick(wyTargetSelector *ts);
    
    wyBitmapFontLabel *goldMultiplyer;
    void onWenhaoClicked(wyTargetSelector *ts);
    ControlButton *btn_wenhao;
};


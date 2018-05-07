#pragma once

#include "WiEngine.h"
#include "ControlButton.h"
#include "DailyRewardLayer.h"
#include "VoteUsLayer.h"

class CGScene:	public wyScene {
public:
    CGScene(bool needPlayBGM = false, bool needUnlockNandu = false);
    virtual ~CGScene();
    
    wyNode *sp_homeBg;
    wySPXSprite *dimian_left, *dimian_right;
    wySprite *sp_homeLogo;
    wySprite *sp_touch2Start;
    
    bool touchesBegan(wyMotionEvent& e);
    void onSModeBtnClicked(wyTargetSelector* ts);
    void onEModeBtnClicked(wyTargetSelector* ts);
    
    void onAllMenusBtnClicked(wyTargetSelector* ts);
    
    bool isDestroied;
    bool isEnterAnimationFinished;
    
    virtual bool keyUp(wyKeyEvent& event);
    void onOK(wyTargetSelector* ts);

    void onEnter();
    void onEnterTransitionDidFinish();

    wySprite *coinShineSprite, *coinSprite;
    void updateCoin(wyTargetSelector *ts);

    wyNode *rightContainer;
    wySPXSprite *sprite_mao, *sprite_mao1, *left, *daoguang, *nvkoushui, *langxiaba, *langkoushui, *langxiaba1;
    int testStatus;
    bool isFiring;
    float fireCDTimer;
    int currentLoopCount;
    
    float timer1, timer2, nextTime1, nextTime2;
    bool isNextAnimScheduled, isNextAnimScheduled1;
    void scheduleNextAnim();
    void scheduleNextAnim1();
    
    ControlButton *btn_sMode, *btn_eMode, *btn_allMenu;
    
    wySpriteEx *touch2Start;
    void createTouch2Start();
    void createButtons();
    
    bool isMenuEnterAnimPlayed;
    void playMenuEnterAnim(wyTargetSelector* ts);
    
    void init1();
    
    void onTutorialOk(wyTargetSelector* ts, void *data);
    void onTutorialCancel(wyTargetSelector* ts, void *data);
    
    void showNanduUnlockNotice();
    wySpriteEx *sModeGantanhao, *eModeGantanhao, *menuGantanhao;
    bool needShowSModeGantanhao, needShowEModeGantanhao, needShowMenuGantanhao;
    
    ControlButton *btn_gplus;
    void onGPlusButtonClick(wyTargetSelector* ts);
    
    void onPopupDailogClose(wyTargetSelector *ts, void *data);
    void showDailyRewardAndVote();
};


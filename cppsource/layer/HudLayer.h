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

static const int GUIDE_NONE = -1;
static const int GUIDE_0_1 = 0;
static const int GUIDE_0_2 = 1;
static const int GUIDE_0_3 = 2;
static const int GUIDE_1_1 = 11;
static const int GUIDE_1_2 = 12;
static const int GUIDE_2_1 = 21;
static const int GUIDE_2_2 = 22;
static const int GUIDE_2_3 = 23;
static const int GUIDE_3_1 = 31;
static const int GUIDE_3_2 = 32;
static const int GUIDE_3_3 = 33;
static const int GUIDE_3_4 = 34;
static const int GUIDE_4_1 = 41;
static const int GUIDE_4_2 = 42;
static const int GUIDE_4_3 = 43;
static const int GUIDE_5_1 = 51;
static const int GUIDE_5_2 = 52;
static const int GUIDE_5_3 = 53;
static const int GUIDE_6_1 = 61;
static const int GUIDE_6_2 = 62;
static const int GUIDE_6_3 = 63;

static const int GUIDE_10_1 = 101;
static const int GUIDE_10_2 = 102;
static const int GUIDE_11_1 = 111;
static const int GUIDE_11_2 = 112;
static const int GUIDE_12_1 = 121;
static const int GUIDE_12_2 = 122;

class Intro;

class HudLayer : public wyLayer {
public:
    HudLayer(int mapIndex);
    virtual ~HudLayer();

    void update(float dt);
    void onLeftButtonClick(wyTargetSelector* ts);
    void onRightButtonClick(wyTargetSelector* ts);
    void onSkillButtonClick(wyTargetSelector* ts);
    void onSkillButtonClick1(wyTargetSelector* ts);
    void onPauseBtnClick(wyTargetSelector* ts);
    void onCharacterBtnClick(wyTargetSelector* ts);
    void onToolBtnClick(wyTargetSelector* ts);
    void onJumpButtonDown(wyTargetSelector* ts);
    void onJumpButtonUp(wyTargetSelector* ts);
    
    void onSwitchBtnClick(wyTargetSelector* ts, int needAmmo);
    
    virtual bool touchesBegan(wyMotionEvent& e);
    virtual bool touchesEnded(wyMotionEvent& e);
    bool touchesPointerBegan(wyMotionEvent& e);
    bool touchesPointerEnded(wyMotionEvent& e);
    bool touchesCancelled(wyMotionEvent& e);
    bool touchesMoved(wyMotionEvent& e);
    bool touchesPointerCancelled(wyMotionEvent& e);
    
    bool isJumping;
    float elapsedTime;
    float coolDown;
    bool isClickedInCoolDown;
    bool isShooting;
    bool isLongPress;
    bool isReleased;
    float longPressTime;
    
    float maxScale;
    wyProgressTimer *hpBar, *hpBarLoss;
    void setHPBar(int hp);
    
    float firecoolDown, toolcoolDown;
    bool isFireInCoolDown, isToolInCoolDown;
    wyButton* skillBtn;
    wyButton* atkBtn;
    wyButton* jumpBtn;
    wyButton* toolBtn;
    wyProgressTimer *skillCD, *toolCD;
    wySpriteEx *tool, *jumpSprite;
    wyNode *dart;
    
    int movingStatus;
    float movingTimer;
    
//    wySprite *dialog;
    wyLabel *dialogContent;
    wyDialog *dialog;
    void showIntro(Intro *intro);
    
    bool isDialogShowing;
    void disableMe(bool flag);
    void onOKButton(wyTargetSelector* ts, void* data);
    
    bool isInVillage;
    
    bool isPaused;
    
    wyBitmapFontLabel *notice_txt, *notice_txt1;
    wyColorLayer *notice_bg;
    wyTimer *introTimer;
    void showNotice(const char *noticeStr, int duration = -1, const char *noticeStr1 = NULL, bool isSwitchGun = false);
    void hideNotice(wyTargetSelector *ts);
    void initNotice();
    
    wyButton *pauseBtn;
    
    bool isTouching, isMultiTouching;
    
    void reset();
    void showCountinueBtn();
    
    wyButton* continueBtn;
    void onCountinueBtnClick(wyTargetSelector* ts);
    void showPointer(int x, int y, bool isFlipX, bool isDelay = false, bool isSwitchGun = false);
    void hidePointer();
    void onHandPointerDelayEnd(wyTargetSelector* ts);
    wySpriteEx *handPointer;
    
    void show1stGuide();
    wyColorLayer *guidBg;
    
    int status;
    bool canJump, canDbJump, canTool, canSkill, canAtk, canSwitch;
    void setHudOp(bool canJump, bool canDbJump, bool canTool, bool canSkill, bool canAtk, bool canSwitch = true);
    void setHudStatus(int status);
    void updateGuide();
    
    void onEnter();
    
    bool needDoSkill;
    
    void addGold(int gold);
    
    wyNode *bossContainer;
    wySpriteEx *bossHead, *bossBloodBorder;
    wyProgressTimer *bossBlood;
    void setBossHPBar(float percent);
    void showBossHP();
    void hideBossHP();
    
    wyBitmapFontLabel *tool_number;
    
    void updateTool();
    
    wySpriteEx *gold_title, *cash_title, *ammo_title;
    wyBitmapFontLabel *gold_number, *cash_number, *ammo_number, *meter_number;
    void updateMeter();
    
    void initBossHpBar(int index);
    void addZuanshi(int zuanshi);
    void addZuanshiFake(int zuanshi);
    
    float shootCDNum, skillCDNum;
    void showHealEffect();
    wySpriteEx *hpBarMiddle;
    
    bool needJump;
    
    bool needAutoFire;
    float autoFireTimer;
    void onRightButtonUp(wyTargetSelector* ts);
    
    bool isFired;
    bool hasCollectCoin;
    
//    wyRect atkBoundingBox;
//    int firePid;
    void onRightButtonMoveOut(wyTargetSelector* ts);
    
    void onExit();
    
    void addAmmo(int ammoNum);
    void runGoldDecAnim(int goldNum);
    void runAmmoIncAnim(int ammoNum);
    void runOutofResAnim();
    
    wyNode *switchGunIcon;
    wyButton *switchBtn;
    
    bool isTrainingToLv1;
    void trainToLv1();
    
    void onTipSwitchGunEnd(wyTargetSelector* ts, void *data);
    
    void updatePropPosition();
    
    wyButton* downBtn;
    void onDownButtonDown(wyTargetSelector* ts);
    
    bool canAutoFire(int weaponId);
};


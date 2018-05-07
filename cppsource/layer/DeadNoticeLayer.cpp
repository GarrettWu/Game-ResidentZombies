//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "DeadNoticeLayer.h"
#include "Global.h"
#include "MainScene.h"
#include "LoadingScene.h"
#include "ShopLayer.h"
#include "LevelSelectLayer.h"
#if ANDROID
#include "com_wiyun_engine_skeleton_Skeleton.h"
#endif
#if IOS
#include "ObjCCalls.h"
#endif
#include "DialogBgLayer.h"

static void onAnimEnd(wyAction* action, void* data) {
    DeadNoticeLayer* layer = (DeadNoticeLayer*)data;
    layer->gradeShake();
}

static void onAnimationEnded(wyAction* action, void* data) {
    DeadNoticeLayer *layer = (DeadNoticeLayer *)data;
    layer->isSaveMeShowEnd = true;
    
    layer->runSaveMeTimer();
}

static void onShowDeadNoticeDelayEnded(wyAction* action, void* data) {
    DeadNoticeLayer *layer = (DeadNoticeLayer *)data;
//    layer->isSaveMeShowEnd = true;
    
//    if (!layer->isHaltDeadNoticeShow) {
        layer->showDeadNotice();
//    } else {
//        layer->isHaltDeadNoticeShow = false;
//    }
}

DeadNoticeLayer::DeadNoticeLayer(int saveType) : wyColorLayer(wyc4b(0, 0, 0, 100)) {
//    LOGE("saveType:%d", saveType);
    sGlobal->needPauseGame = false;

	sGlobal->deadNoticeLayer = this;
    this->saveType = saveType;
    
    MyPrefs::setInt(DB_GOLD, sGlobal->getGold());
    MyPrefs::setInt(DB_ZUANSHI_NUM, sGlobal->zuanshi.getValue());

	saveMe = wyNode::make();
	addChildLocked(saveMe);
	
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "saveme_bg.png");
    wySpriteEx *pressed = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "saveme_bg.png");
    pressed->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    wyBitmapFontLabel *title = wyBitmapFontLabel::make(sGlobal->font_desc, "");
    title->setScale(SP(24)/sGlobal->descFontSize);
    saveMeBtn = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(DeadNoticeLayer::onSaveMeClicked)), pressed, title);
    saveMeBtn->setScaleX(0.7f*sGlobal->scaleX);
	saveMeBtn->setScaleY(0.7f*sGlobal->scaleY);
	saveMeBtn->setPosition(wyDevice::winWidth/2, DP(150)*sGlobal->scaleY - wyDevice::winHeight);
    saveMeBtn->setInterceptTouch(true);
	saveMe->addChildLocked(saveMeBtn);
    
    wySpriteEx *progress_border = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "saveme_progress.png");//wySpriteEx::make(MyTextureManager::makePNG("saveme_progress.png"));
    progress_border->setScaleX(4.5f*sGlobal->scaleY);
    progress_border->setScaleY(0.6f*sGlobal->scaleY);
    progress_border->setPosition(saveMeBtn->getPositionX(), saveMeBtn->getPositionY()-DP(0)*sGlobal->scaleY);
    progress_border->setColor(wyc3b(0, 0, 0));
    saveMe->addChildLocked(progress_border);
    
    saveme_progress = wyProgressTimer::make(wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "saveme_progress.png"));
    saveme_progress->setStyle(HORIZONTAL_BAR_LR);
    saveme_progress->setPosition(progress_border->getPositionX(), progress_border->getPositionY());
    saveme_progress->setScaleX(progress_border->getScaleX());
    saveme_progress->setScaleY(0.6f*sGlobal->scaleX);
    saveMe->addChildLocked(saveme_progress);
    saveme_progress->setPercentage(100);
    
    wySpriteEx *cash_title = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cash_icon.png");
    cash_title->setAnchor(1.0f, 0.5f);
    cash_title->setScaleX(sGlobal->scaleX*0.7f);
    cash_title->setScaleY(sGlobal->scaleY*0.7f);
	cash_title->setPosition(saveMeBtn->getPositionX() + DP(0)*sGlobal->scaleX, saveMeBtn->getPositionY() - DP(36)*sGlobal->scaleY);
    saveMe->addChildLocked(cash_title);
    
    wyBitmapFontLabel *cash_number = wyBitmapFontLabel::make(sGlobal->font_desc, "2");
    cash_number->setScale(SP(20)*sGlobal->scaleX/sGlobal->descFontSize);
    cash_number->setAlignment(wyBitmapFontLabel::LEFT);
    cash_number->setAnchor(0, 0.5f);
    cash_number->setPosition(cash_title->getPositionX() + DP(4)*sGlobal->scaleX, cash_title->getPositionY() + DP(0)*sGlobal->scaleY);
    saveMe->addChildLocked(cash_number);

    isSaveMeShowEnd = false;
    isDeadNoticeShowed = false;
    isSaveMeClicked = false;
	isBuyGold = false;
    timer = NULL;
    
    initCommonUI();
    if (sGlobal->isRealEndless) {
        initEndlessDeadUI();
    } else {
        initNormalDeadUI();
        qLayer = NULL;
    }
    
    this->setTouchEnabled(true);
    
    container->setVisible(true);
    saveMe->setVisible(true);
    
    isReviveBuyGold = false;
    isHaltDeadNoticeShow = false;
    
    isTimerPaused = false;
}

DeadNoticeLayer::~DeadNoticeLayer() {
    sGlobal->deadNoticeLayer = NULL;
//    sGlobal->needPauseGame = false;
}

void DeadNoticeLayer::onEnterTransitionDidFinish() {
    wyLayer::onEnterTransitionDidFinish();
}

void DeadNoticeLayer::onEnter() {
    wyLayer::onEnter();
    
    wyIntervalAction *a1 = wyEaseElasticOut::make(0.5f, wyMoveBy::make(1.0f, 0, wyDevice::winHeight));
    wyIntervalAction *a2 = wySequence::make(wyDelayTime::make(0.5f), a1, NULL);
    wyActionCallback callback = {
        NULL,
        onAnimationEnded
    };
    a2->setCallback(&callback, this);
    saveMe->runAction(a2);
    saveMe->setVisible(true);
    
//    timer = wyTimer::make(wyTargetSelector::make(this, SEL(DeadNoticeLayer::showDeadNotice)), 3.5f);
//    timer->setOneShot(true);
//    scheduleLocked(timer);
    
//    this->runAction(wyFadeTo::make(0.1f, 255, 10, true));
}

void DeadNoticeLayer::runSaveMeTimer() {
//    LOGE("runSaveMeTimer...");
    timer = wyTimer::make(wyTargetSelector::make(this, SEL(DeadNoticeLayer::saveMeTimerUpdate)));
    scheduleLocked(timer);
    
    savemeDeltaTime = 0;
//    isTimerPaused = false;
}

void DeadNoticeLayer::saveMeTimerUpdate(wyTargetSelector *ts) {
    if (isTimerPaused) {
        return;
    }
    
    savemeDeltaTime += ts->getDelta();
    if (savemeDeltaTime > 0.03f) {
        savemeDeltaTime = 0;
        float nextPercent = MAX(saveme_progress->getPercentage()-1, 0);
        if ((int) nextPercent == 0) {
            saveme_progress->setPercentage(0);
            
            wyIntervalAction *a2 = wyDelayTime::make(0.2f);
            wyActionCallback callback = {
                NULL,
                onShowDeadNoticeDelayEnded
            };
            a2->setCallback(&callback, this);
            a2->setTag(99);
            saveMe->runAction(a2);
            
            saveMeBtn->setEnabled(false);
            
//            if (timer != NULL) {
//                LOGE("unscheduleLocked123 timer...");
//                unscheduleLocked(timer);
//                timer = NULL;
//            }
//            LOGE("time reached...");
            isTimerPaused = true;
            
            this->setTouchEnabled(false);
        } else {
            saveme_progress->setPercentage(nextPercent);
        }
        
    }
}

void DeadNoticeLayer::onCloseButtonClick(wyTargetSelector* ts) {
    showDeadNotice();
}

bool DeadNoticeLayer::touchesBegan(wyMotionEvent& event) {
    if (isSaveMeShowEnd) {
//        showDeadNotice();
        float nextPercent = MAX(saveme_progress->getPercentage()-25, 0);
        saveme_progress->setPercentage(nextPercent);
    }
    return true;
}

void DeadNoticeLayer::onSaveMeClicked(wyTargetSelector* ts) {
    if (isTimerPaused) {
//        LOGE("time reached, cannot click...");
        return;
    }
    
    isTimerPaused = true;
    if (timer != NULL) {
//        LOGE("unscheduleLocked timer...");
//        unscheduleLocked(timer);
//        timer = NULL;

//        LOGE("saveme clicked..., pause timer...");
        saveMe->stopAction(99);
    }
    
    if (sGlobal->zuanshi.getValue() >= 2) {
        if (!sGlobal->isRealEndless) {
            sGlobal->reportGameEvent(EVT_GAME_SAVEME, sGlobal->index, 0.0f, DIFFICULTY_STR[sGlobal->currentSModeDifficulty]);
        } else {
            sGlobal->reportGameEvent(EVT_GAME_ENDLESS_SAVEME, sGlobal->currentDistance, 0.0f, DIFFICULTY_STR[sGlobal->currentEModeDifficulty]);
        }
        
        isSaveMeClicked = true;
        
        sGlobal->zuanshi.offset(-2);
        MyPrefs::setInt(DB_ZUANSHI_NUM, sGlobal->zuanshi.getValue());
        
        sGlobal->mainGameLayer->hero->saveType = saveType;
        sGlobal->mainGameLayer->hero->needSaveMe = true;
        sGlobal->mainGameLayer->hero->isSavingMe = true;
        sGlobal->mainGameLayer->hero->savingMeTimer = 0;
        sGlobal->mainGameLayer->hero->isSaveMeInited = false;
        
        //更新下hudLayer的钻石
        sGlobal->hudLayer->addZuanshi(0);
        
        if (sGlobal->isRealEndless) {
            for (int i = 0; i < 2; i++) {
                int achvNo = ACHV_SAVEME_1+i;
                if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
                    sGlobal->achv_progress[achvNo]+=1;
                    if (sGlobal->achv_progress[achvNo] >= sGlobal->ACHV_NUM[achvNo*3]) {
                        sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                        sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                        sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
                    }
                }
            }
        }
        
        sGlobal->hudLayer->setHudOp(true, true, true, true, true);
        
        this->getParent()->removeChildLocked(this, true);
        sGlobal->mainGameLayer->onEnter();
        sGlobal->effectLayer->onEnter();
        sGlobal->hudLayer->onEnter();
        
        ((MainScene *)sGlobal->mainGameLayer->getParent())->sceneState = SC_NORMAL;
        sGlobal->needPauseGame = true;
    } else {
		isBuyGold = true;
        ConfirmDialogLayer *layer = new ConfirmDialogLayer(1, -1, 1);
        layer->autoRelease();
        layer->listener = this;
        this->getParent()->addChildLocked(layer, 101, 0);
	}
}

void DeadNoticeLayer::onQuitClicked(wyTargetSelector* ts) {
    sGlobal->saveDataInGame();
    
    bool flag = sGlobal->isRealEndless;

//    sGlobal->isRealEndless = false;
    sGlobal->isBoostOn = false;
    sGlobal->boostMulti = 1.0f;
    sGlobal->heroVelocityX = HERO_VEL_X;

    if (qLayer != NULL) {
        qLayer->isQuiting = true;
    }
    
    sGlobal->stopBGM();
    if (sGlobal->isRealEndless) {
        sGlobal->gotoCGScene();
    } else {
        sGlobal->gotoWorldMapScene(true);
    }
    
    sGlobal->needPauseGame = false;
}

void DeadNoticeLayer::onRestartClicked(wyTargetSelector* ts) {
    sGlobal->saveDataInGame();

    if (qLayer != NULL) {
        qLayer->isQuiting = true;
    }
    
    if (sGlobal->isRealEndless) {
        ((MainScene *)sGlobal->mainGameLayer->getParent())->reset(false);
    } else {
        ((MainScene *)sGlobal->mainGameLayer->getParent())->reset(false);
    }
}

void DeadNoticeLayer::onReviveClicked(wyTargetSelector* ts) {
    if (sGlobal->getGold() >= 500) {
        //mainscene的reset里发送事件了
//        if (!sGlobal->isRealEndless) {
//            sGlobal->reportGameEvent(EVT_GAME_REVIVE, sGlobal->index, 0.0f, DIFFICULTY_STR[sGlobal->currentSModeDifficulty]);
//        }
        
        sGlobal->offsetGold(-500);
        MyPrefs::setInt(DB_GOLD, sGlobal->getGold());

        sGlobal->hudLayer->addGold(0);
        
        sGlobal->saveDataInGame();

        if (qLayer != NULL) {
            qLayer->isQuiting = true;
        }
        
        ((MainScene *)sGlobal->mainGameLayer->getParent())->reset(sGlobal->hasReachCheckPoint);
        ((MainScene *)sGlobal->mainGameLayer->getParent())->sceneState = SC_NORMAL;
    } else {
		isReviveBuyGold = true;
        ConfirmDialogLayer *layer = new ConfirmDialogLayer(1, -1, 0);
        layer->autoRelease();
        layer->listener = this;
        this->getParent()->addChildLocked(layer, 101, 0);
	}
}

void DeadNoticeLayer::showDeadNotice() {
    if (isDeadNoticeShowed || isSaveMeClicked || isBuyGold) {
        return;
    }
    isDeadNoticeShowed = true;
    
    wyIntervalAction *a = wyEaseElasticOut::make(0.8f, wyMoveBy::make(0.8f, 0, -DP(350)*sGlobal->scaleY));
    saveMe->runAction(a);
    
//    container->setVisible(true);
    wyIntervalAction *a0 = wyEaseElasticOut::make(0.5f, wyMoveBy::make(1.0f, 0, -wyDevice::winHeight));
    container->runAction(a0);
    
    if (sGlobal->isRealEndless) {
        calcAll();
        
        if (isHighScore) {
            wyIntervalAction *a1 = wyScaleTo::make(0.5f, sGlobal->scaleX*8.0f, sGlobal->scaleY*8.0f, sGlobal->scaleX*0.6f, sGlobal->scaleY*0.6f);
            wyIntervalAction *a2 = wyFadeTo::make(0.5f, 30, 255);
            wyIntervalAction *a11 = wySequence::make(wyDelayTime::make(1.0f), wyShow::make(), wyEaseOut::make(0.2f, a1), NULL);
            wyIntervalAction *a21 = wySequence::make(wyDelayTime::make(1.0f), wyEaseOut::make(0.2f, a2), NULL);
            wyActionCallback callback = {
                NULL,
                onAnimEnd
            };
            a21->setCallback(&callback, this);
            highestscore->runAction(a11);
            highestscore->runAction(a21);
        }
    }
    
    if (sGlobal->isRealEndless) {
        int reportDist = sGlobal->currentDistance/100;
        sGlobal->reportGameEvent(EVT_GAME_ENDLESS_END, reportDist, 0.0f, DIFFICULTY_STR[sGlobal->currentEModeDifficulty]);
    }
}

void DeadNoticeLayer::updateGold() {
    sGlobal->hudLayer->addGold(0);
    sGlobal->hudLayer->updateTool();
}

void DeadNoticeLayer::updateZuanshi() {
    sGlobal->hudLayer->addZuanshi(0);
}

//void DeadNoticeLayer::buyGoldSuccess() {
//	isSaveMeShowEnd = true;
//	isBuyGold = false;
//    
//    if (!isReviveBuyGold) {
//        runSaveMeTimer();
//    } else {
//        isReviveBuyGold = false;
//    }
//}

void DeadNoticeLayer::onShopEnd(int result) {
//    LOGE("result:%d", result);
	isSaveMeShowEnd = true;
	isBuyGold = false;

    if (result == 0) {
        if (!isReviveBuyGold) {
//            runSaveMeTimer();
//            LOGE("resume timer...");
            isTimerPaused = false;
        } else {
            isReviveBuyGold = false;
        }
    } else {
        sGlobal->hudLayer->addGold(0);
        sGlobal->hudLayer->addZuanshi(0);
        
        if (!isReviveBuyGold) {
            if (sGlobal->zuanshi.getValue() >= 2) {
                saveMe->setVisible(false);
                container->setVisible(false);
                
                //虚假更新hud上的钻石
                sGlobal->hudLayer->addZuanshiFake(-2);
                
                wyTimer *timer1 = wyTimer::make(wyTargetSelector::make(this, SEL(DeadNoticeLayer::showCountDown)), 0.1f);
                timer1->setOneShot(true);
                scheduleLocked(timer1);
            } else {
                isBuyGold = true;
                ConfirmDialogLayer *layer = new ConfirmDialogLayer(1, -1, 1);
                layer->autoRelease();
                layer->listener = this;
                this->getParent()->addChildLocked(layer, 101, 0);
            }
        } else {
            if (sGlobal->getGold() >= 500) {
                isReviveBuyGold = false;
                onReviveClicked(NULL);
            } else {
                isReviveBuyGold = true;
                ConfirmDialogLayer *layer = new ConfirmDialogLayer(1, -1, 0);
                layer->autoRelease();
                layer->listener = this;
                this->getParent()->addChildLocked(layer, 101, 0);
            }
        }
    }
}

static void onAnimationEnded1(wyAction* action, void* data) {
    DeadNoticeLayer* layer = (DeadNoticeLayer*)data;
    wyTargetSelector* ts = wyTargetSelector::make(layer, SEL(DeadNoticeLayer::count2));
    wyTimer* t = wyTimer::make(ts, 0.2f);
    t->setOneShot(true);
    layer->scheduleLocked(t);
}

static void onAnimationEnded2(wyAction* action, void* data) {
    DeadNoticeLayer* layer = (DeadNoticeLayer*)data;
    wyTargetSelector* ts = wyTargetSelector::make(layer, SEL(DeadNoticeLayer::count1));
    wyTimer* t = wyTimer::make(ts, 0.2f);
    t->setOneShot(true);
    layer->scheduleLocked(t);
}

static void onAnimationEnded3(wyAction* action, void* data) {
    DeadNoticeLayer* layer = (DeadNoticeLayer*)data;
    wyTargetSelector* ts = wyTargetSelector::make(layer, SEL(DeadNoticeLayer::go));
    wyTimer* t = wyTimer::make(ts, 0.2f);
    t->setOneShot(true);
    layer->scheduleLocked(t);
}

static void onAnimationEnded4(wyAction* action, void* data) {
    DeadNoticeLayer* layer = (DeadNoticeLayer*)data;
    wyTargetSelector* ts = wyTargetSelector::make(layer, SEL(DeadNoticeLayer::gone));
    wyTimer* t = wyTimer::make(ts, 0.3f);
    t->setOneShot(true);
    layer->scheduleLocked(t);
}

void DeadNoticeLayer::count2() {
    float fsx = SP(300)*sGlobal->scaleX/sGlobal->descFontSize;
    float tsx = SP(60)*sGlobal->scaleX/sGlobal->descFontSize;
    wyIntervalAction *a1 = wyScaleTo::make(0.5f, fsx, fsx, tsx, tsx);
    wyIntervalAction *a2 = wyFadeTo::make(0.5f, 30, 255);
    wyIntervalAction *a11 = (wyEaseBackOut*)(new wyEaseOut(0.2f, a1))->autoRelease();
    wyIntervalAction *a21 = (wyEaseBackOut*)(new wyEaseOut(0.2f, a2))->autoRelease();
    wyActionCallback callback = {
        NULL,
        onAnimationEnded2
    };
    a21->setCallback(&callback, this);
    number1->runAction(a11);
    number1->runAction(a21);
    number1->setText("2");
    number1->setVisible(true);
}

void DeadNoticeLayer::count1() {
    float fsx = SP(300)*sGlobal->scaleX/sGlobal->descFontSize;
    float tsx = SP(60)*sGlobal->scaleX/sGlobal->descFontSize;
    wyIntervalAction *a1 = wyScaleTo::make(0.5f, fsx, fsx, tsx, tsx);
    wyIntervalAction *a2 = wyFadeTo::make(0.5f, 30, 255);
    wyIntervalAction *a11 = (wyEaseBackOut*)(new wyEaseOut(0.2f, a1))->autoRelease();
    wyIntervalAction *a21 = (wyEaseBackOut*)(new wyEaseOut(0.2f, a2))->autoRelease();
    wyActionCallback callback = {
        NULL,
        onAnimationEnded4
    };
    a21->setCallback(&callback, this);
    number1->runAction(a11);
    number1->runAction(a21);
    number1->setText("1");
    number1->setVisible(true);
}

void DeadNoticeLayer::go() {
    float fsx = SP(300)*sGlobal->scaleX/sGlobal->descFontSize;
    float tsx = SP(60)*sGlobal->scaleX/sGlobal->descFontSize;
    wyIntervalAction *a1 = wyScaleTo::make(0.5f, fsx, fsx, tsx, tsx);
    wyIntervalAction *a2 = wyFadeTo::make(0.5f, 30, 255);
    wyIntervalAction *a11 = (wyEaseBackOut*)(new wyEaseOut(0.2f, a1))->autoRelease();
    wyIntervalAction *a21 = (wyEaseBackOut*)(new wyEaseOut(0.2f, a2))->autoRelease();
    wyActionCallback callback = {
        NULL,
        onAnimationEnded4
    };
    a21->setCallback(&callback, this);
    number1->runAction(a11);
    number1->runAction(a21);
    number1->setText("GO!");
    number1->setVisible(true);
}

void DeadNoticeLayer::gone() {
    isTimerPaused = false;
    onSaveMeClicked(NULL);
}

void DeadNoticeLayer::showCountDown(wyTargetSelector *target) {
    saveMe->setVisible(false);

    number1 = wyBitmapFontLabel::make(sGlobal->font_desc, "3");
//    number1 = wyLabel::make("3", SP(60)*sGlobal->scaleX, BOLD);
//    number1->setColor(wyc3b(255, 255, 0));
    number1->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    addChildLocked(number1);
    number1->setVisible(false);
    
    float fsx = SP(300)*sGlobal->scaleX/sGlobal->descFontSize;
    float tsx = SP(60)*sGlobal->scaleX/sGlobal->descFontSize;
    wyIntervalAction *a1 = wyScaleTo::make(0.5f, fsx, fsx, tsx, tsx);
    wyIntervalAction *a2 = wyFadeTo::make(0.5f, 30, 255);
    wyIntervalAction *a11 = (wyEaseBackOut*)(new wyEaseOut(0.2f, a1))->autoRelease();
    wyIntervalAction *a21 = (wyEaseBackOut*)(new wyEaseOut(0.2f, a2))->autoRelease();
    wyActionCallback callback = {
        NULL,
        onAnimationEnded1
    };
    a21->setCallback(&callback, this);
    number1->runAction(a11);
    number1->runAction(a21);
    number1->setVisible(true);
}

void DeadNoticeLayer::initCommonUI() {
    container = wyNode::make();
    container->setPosition(wyDevice::winWidth/2, wyDevice::winHeight*1.5f);
    addChildLocked(container);
    container->setVisible(false);
    
    DialogBgLayer *bgLayer = DialogBgLayer::make();
    container->addChildLocked(bgLayer);
    
    title = wyBitmapFontLabel::make(sGlobal->font_title, "a");
    title->setScale(SP(42)*sGlobal->scaleX/sGlobal->titleFontSize);
    title->setAlignment(wyBitmapFontLabel::LEFT);
    title->setAnchor(0, 0);
    title->setPosition(-DP(215)*sGlobal->scaleX, DP(125)*sGlobal->scaleY);
    container->addChildLocked(title);

    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    wyBitmapFontLabel *title = wyBitmapFontLabel::make(sGlobal->font_desc, "restart");
    title->setScale(SP(22)/sGlobal->descFontSize);
    btn_restart = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(DeadNoticeLayer::onRestartClicked)), mask, title);
    btn_restart->setScaleX(sGlobal->scaleX*0.8f);
    btn_restart->setScaleY(sGlobal->scaleY*0.8f);
    btn_restart->setMultiTouchClickable(false);
    btn_restart->setInterceptTouch(true);
    btn_restart->setTouchPriority(1000);
    container->addChildLocked(btn_restart, 100);
    btn_restart->needDelay = false;
    btn_restart->setPosition(DP(0)*sGlobal->scaleX, -DP(120)*sGlobal->scaleY);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "quit");
    title->setScale(SP(22)/sGlobal->descFontSize);
    btn_quit = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(DeadNoticeLayer::onQuitClicked)), mask, title);
    btn_quit->setScaleX(sGlobal->scaleX*0.8f);
    btn_quit->setScaleY(sGlobal->scaleY*0.8f);
    btn_quit->setMultiTouchClickable(false);
    btn_quit->setInterceptTouch(true);
    btn_quit->setTouchPriority(1000);
    container->addChildLocked(btn_quit, 100);
    btn_quit->needDelay = false;
    btn_quit->setPosition(-DP(165)*sGlobal->scaleX, -DP(120)*sGlobal->scaleY);
}

void DeadNoticeLayer::initNormalDeadUI() {
    char temp[20];
    int seasonId = sGlobal->index / 100;
    int mapId = sGlobal->index % 100;
    
    if (sGlobal->index == 0) {
        sprintf(temp, "%s", sGlobal->getStr(STR_TUTORIAL_TITLE));
    } else {
        sprintf(temp, sGlobal->getStr(STR_LEVEL_TITLE), seasonId, mapId);
    }
    title->setText(temp);
    title->setAnchor(0.5f, 0);
    title->setScale(SP(42)*sGlobal->scaleX/sGlobal->titleFontSize);
    title->setPosition(-DP(140)*sGlobal->scaleX, DP(110)*sGlobal->scaleY);
    title->setVisible(false);
    
    wyBitmapFontLabel *dead_notice;
    dead_notice = wyBitmapFontLabel::make(sGlobal->font_desc, sGlobal->getStr(STR_MISSION_FAILED_TITLE));
    dead_notice->setScale(SP(26)*sGlobal->scaleX/sGlobal->descFontSize);
    dead_notice->setAlignment(wyBitmapFontLabel::LEFT);
    dead_notice->setAnchor(0, 1.0);
    dead_notice->setColor(wyc3b(198, 65, 35));
    dead_notice->setPosition(DP(10)*sGlobal->scaleY, DP(152)*sGlobal->scaleY);
    container->addChildLocked(dead_notice);
    
    wyBitmapFontLabel *dead_notice1;
    dead_notice1 = wyBitmapFontLabel::make(sGlobal->font_desc, sGlobal->getStr(STR_MISSION_FAILED_DESC));
    dead_notice1->setAlignment(wyBitmapFontLabel::LEFT);
    dead_notice1->setLineWidth(DP(380)*sGlobal->scaleX/dead_notice->getScale());
    dead_notice1->setAnchor(0, 1.0f);
    container->addChildLocked(dead_notice1);
    dead_notice1->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    dead_notice1->setPosition(dead_notice->getPositionX()+DP(0)*sGlobal->scaleY, dead_notice->getPositionY()-DP(40)*sGlobal->scaleY);
    
//    wySprite *dead_badge = wySprite::make(MyTextureManager::makePNG("dead_badge.png"));
    wySPXSprite *dead_badge = wySPXSprite::make("s03.sprite", false, MyTextureManager::makePNG("s03.png"), 1);
    dead_badge->setScaleX(sGlobal->scaleX*1.1f);
    dead_badge->setScaleY(sGlobal->scaleY*1.1f);
    dead_badge->setPosition(-DP(126)*sGlobal->scaleX, DP(40)*sGlobal->scaleY);
    container->addChildLocked(dead_badge);
    
    wySpriteEx *sp = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_upgrade_gun.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_upgrade_gun.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    wyBitmapFontLabel *text = wyBitmapFontLabel::make(sGlobal->font_desc, "");
    ControlButton *btn_up = ControlButton::make(sp, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(DeadNoticeLayer::onGotoUpgradeBtnClick)), mask, text);
    btn_up->setScaleX(sGlobal->scaleX*0.6f);
    btn_up->setScaleY(sGlobal->scaleY*0.6f);
    btn_up->setMultiTouchClickable(false);
    btn_up->setInterceptTouch(true);
    btn_up->setTouchPriority(1000);
    container->addChildLocked(btn_up, 100);
    btn_up->setPosition(DP(95)*sGlobal->scaleX, -DP(10)*sGlobal->scaleY);

    if (sGlobal->hasReachCheckPoint) {
        wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
        wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
        mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        ControlButton *btn_revive = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(DeadNoticeLayer::onReviveClicked)), mask, NULL);
        btn_revive->setScaleX(sGlobal->scaleX*0.85f);
        btn_revive->setScaleY(sGlobal->scaleY*1.2f);
        btn_revive->setMultiTouchClickable(false);
        btn_revive->setInterceptTouch(true);
        btn_revive->setTouchPriority(1000);
        container->addChildLocked(btn_revive);
        btn_revive->setPosition(DP(165)*sGlobal->scaleX, -DP(115)*sGlobal->scaleY);
        btn_revive->needDelay = false;

        wyBitmapFontLabel *btn_revive_title = wyBitmapFontLabel::make(sGlobal->font_desc, "checkpoint");
        btn_revive_title->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
        btn_revive_title->setPosition(btn_revive->getPositionX()+DP(2)*sGlobal->scaleX, btn_revive->getPositionY()+DP(12)*sGlobal->scaleX);
        container->addChildLocked(btn_revive_title);
        
        wySpriteEx *coin_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
        coin_icon->setScaleX(sGlobal->scaleX*0.7f);
        coin_icon->setScaleY(sGlobal->scaleY*0.7f);
        coin_icon->setPosition(btn_revive->getPositionX()-DP(17)*sGlobal->scaleY, btn_revive->getPositionY()-DP(12)*sGlobal->scaleY);
        container->addChildLocked(coin_icon);
        
        wyBitmapFontLabel *coin_num = wyBitmapFontLabel::make(sGlobal->font_desc, "500");
        coin_num->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
        coin_num->setAlignment(wyBitmapFontLabel::LEFT);
        coin_num->setPosition(coin_icon->getPositionX()+DP(18)*sGlobal->scaleX, coin_icon->getPositionY());
        coin_num->setAnchor(0, 0.5f);
        container->addChildLocked(coin_num);
    
        mask->setVisible(true);
        mask->stopAllActions();
        mask->runAction(wyRepeatForever::make(wySequence::make(wyFadeTo::make(0.6f, 255, 0), wyFadeTo::make(0.6f, 0, 255), NULL)));
    } else {
        btn_restart->setPosition(DP(165)*sGlobal->scaleX, -DP(120)*sGlobal->scaleY);
    }
}

void DeadNoticeLayer::onGotoUpgradeOk(wyTargetSelector* ts, void *data) {
    PopupDialog *popupDialog = (PopupDialog*) data;
    popupDialog->getParent()->removeChildLocked(popupDialog, true);
    
    sGlobal->gotoUpgradeScene(sGlobal->index);
}

void DeadNoticeLayer::onGotoUpgradeBtnClick(wyTargetSelector* ts) {
    if (sGlobal->hasReachCheckPoint) {
        PopupDialog *popupDialog = new PopupDialog();
        popupDialog->autoRelease();
        popupDialog->setContent(OTHER_STR[STR_LOSS_PROGRESS]);
        popupDialog->setOkButtonSelector(wyTargetSelector::make(this, SEL_p(DeadNoticeLayer::onGotoUpgradeOk), popupDialog));
        addChildLocked(popupDialog, 101, 0);
    } else {
        sGlobal->gotoUpgradeScene(sGlobal->index);
    }
}

void DeadNoticeLayer::initEndlessDeadUI() {
    char temp[20] = {0};
    switch (sGlobal->currentEModeDifficulty) {
        case DIFFICULTY_NORMAL:
            sprintf(temp, "%s", "endless-easy");
            break;
        case DIFFICULTY_HEROIC:
            sprintf(temp, "%s", "endless-hard");
            break;
        case DIFFICULTY_HARD:
            sprintf(temp, "%s", "endless-crazy");
            break;
        default:
            sprintf(temp, "%s", "endless-easy");
            break;
    }
    title->setText(temp);
    title->setScale(SP(36)*sGlobal->scaleX/sGlobal->titleFontSize);
    title->setAnchor(0, 0);
    title->setPosition(-DP(225)*sGlobal->scaleX, DP(132)*sGlobal->scaleY);

    option_distance = wyBitmapFontLabel::make(sGlobal->font_desc, "distance");
    option_distance->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    option_distance->setAlignment(wyBitmapFontLabel::LEFT);
    option_distance->setAnchor(0, 0);
    container->addChildLocked(option_distance);
    
    number_distance = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    number_distance->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    number_distance->setAlignment(wyBitmapFontLabel::LEFT);
    number_distance->setAnchor(0, 0);
    container->addChildLocked(number_distance);

    option_kills = wyBitmapFontLabel::make(sGlobal->font_desc, "kills");
    option_kills->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    option_kills->setAlignment(wyBitmapFontLabel::LEFT);
    option_kills->setAnchor(0, 0);
    container->addChildLocked(option_kills);
    
    number_kills = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    number_kills->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    number_kills->setAlignment(wyBitmapFontLabel::LEFT);
    number_kills->setAnchor(0, 0);
    container->addChildLocked(number_kills);
    
    option_bosses = wyBitmapFontLabel::make(sGlobal->font_desc, "bosses");
    option_bosses->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    option_bosses->setAlignment(wyBitmapFontLabel::LEFT);
    option_bosses->setAnchor(0, 0);
    container->addChildLocked(option_bosses);
    
    number_bosses = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    number_bosses->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    number_bosses->setAlignment(wyBitmapFontLabel::LEFT);
    number_bosses->setAnchor(0, 0);
    container->addChildLocked(number_bosses);

    option_gold= wyBitmapFontLabel::make(sGlobal->font_desc, "gold");
    option_gold->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    option_gold->setAlignment(wyBitmapFontLabel::LEFT);
    option_gold->setAnchor(0, 0);
    container->addChildLocked(option_gold);
    
    number_gold= wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    number_gold->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    number_gold->setAlignment(wyBitmapFontLabel::LEFT);
    number_gold->setAnchor(0, 0);
    container->addChildLocked(number_gold);
    
    option_score= wyBitmapFontLabel::make(sGlobal->font_desc, "score");
    option_score->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    option_score->setAlignment(wyBitmapFontLabel::LEFT);
    option_score->setAnchor(0, 0);
    container->addChildLocked(option_score);
    
    number_score= wyBitmapFontLabel::make(sGlobal->font_desc, "123456");
    number_score->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    number_score->setAlignment(wyBitmapFontLabel::LEFT);
    number_score->setAnchor(0, 0);
    container->addChildLocked(number_score);

    float ofy = DP(20)*sGlobal->scaleY;
    float startY = DP(117)*sGlobal->scaleY;
    float offsetX = -DP(10)*sGlobal->scaleX;
    
    option_distance->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY);
    number_distance->setPosition(-DP(80)*sGlobal->scaleX + offsetX, startY);
    
    option_kills->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY - ofy);
    number_kills->setPosition(-DP(80)*sGlobal->scaleX + offsetX, startY - ofy);
    
    option_bosses->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY - ofy*2);
    number_bosses->setPosition(-DP(80)*sGlobal->scaleX + offsetX, startY - ofy*2);
    
    option_gold->setPosition(DP(10)*sGlobal->scaleX + offsetX, startY);
    number_gold->setPosition(DP(75)*sGlobal->scaleX + offsetX, startY);
    
    option_score->setPosition(DP(10)*sGlobal->scaleX + offsetX, startY-ofy);
    number_score->setPosition(DP(75)*sGlobal->scaleX + offsetX, startY-ofy);

//    calcAll();
    
    wyBitmapFontLabel *title1 = wyBitmapFontLabel::make(sGlobal->font_title, "medals");
    title1->setScale(SP(30)*sGlobal->scaleX/sGlobal->titleFontSize);
    title1->setAlignment(wyBitmapFontLabel::LEFT);
    title1->setAnchor(0, 0);
    title1->setPosition(-DP(225)*sGlobal->scaleX, DP(45)*sGlobal->scaleY);
    title1->setColor(wyc3b(198, 65, 35));
    container->addChildLocked(title1);

//    qLayer = (QuestLayer*) (new QuestLayer(true))->autoRelease();
//    this->addChildLocked(qLayer, 101, 0);
//    qLayer->setQuestEndSelector(wyTargetSelector::make(this, SEL(DeadNoticeLayer::onQuestEnded)));
    
    btn_quit->setVisible(false);
    btn_restart->setVisible(false);
    btn_quit->setPosition(-DP(165)*sGlobal->scaleX, -DP(130)*sGlobal->scaleY);
    btn_restart->setPosition(DP(165)*sGlobal->scaleX, -DP(130)*sGlobal->scaleY);
    
    
    wySpriteEx *sp = wySpriteEx::make(MyTextureManager::makePNG("gplus_icon_64.png"));//wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "btn_upgrade_gun.png");
    wySpriteEx *mask = wySpriteEx::make(MyTextureManager::makePNG("gplus_icon_64.png"));
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    ControlButton *btn_gplus = ControlButton::make(sp, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(DeadNoticeLayer::onGPlusButtonClick)), mask, NULL);
    btn_gplus->setScaleX(sGlobal->scaleX*0.7f);
    btn_gplus->setScaleY(sGlobal->scaleY*0.7f);
    btn_gplus->setMultiTouchClickable(false);
    btn_gplus->setInterceptTouch(true);
    btn_gplus->setTouchPriority(1000);
    container->addChildLocked(btn_gplus, 100);
    btn_gplus->setPosition(DP(200)*sGlobal->scaleX, DP(90)*sGlobal->scaleY);
    
    char tempp[50];
    float multi = (1+sGlobal->skillMulti[SKILL_GREED]);
    if (sGlobal->isDoubleCoin) {
        multi *= 2;
    }
    sprintf(tempp, "coin multiplier: x%.1f", multi);
    goldMultiplyer = wyBitmapFontLabel::make(sGlobal->font_desc, tempp);
    goldMultiplyer->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    goldMultiplyer->setAlignment(wyBitmapFontLabel::LEFT);
    goldMultiplyer->setAnchor(0, 0.5f);
    container->addChildLocked(goldMultiplyer);
    goldMultiplyer->setColor(wyc3b(198, 65, 35));
    
    wySpriteEx* normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "wenhao.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "wenhao.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    btn_wenhao = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(DeadNoticeLayer::onWenhaoClicked)), mask, NULL);
    btn_wenhao->setScaleX(sGlobal->scaleX*0.6f);
    btn_wenhao->setScaleY(sGlobal->scaleY*0.6f);
    btn_wenhao->setMultiTouchClickable(false);
    btn_wenhao->setInterceptTouch(true);
    btn_wenhao->setTouchPriority(1000);
    container->addChildLocked(btn_wenhao, 100);
    btn_wenhao->setPosition(DP(165)*sGlobal->scaleX, -DP(120)*sGlobal->scaleY);
    btn_wenhao->needDelay = false;
    
    goldMultiplyer->setPosition(DP(50)*sGlobal->scaleX, DP(150)*sGlobal->scaleY);
    btn_wenhao->setPosition(DP(240)*sGlobal->scaleX, DP(150)*sGlobal->scaleY);
}

void DeadNoticeLayer::onGPlusButtonClick(wyTargetSelector* ts) {
#if ANDROID
    submitScoreAndShowLB(totalScore);
//    LOGE("submit score:%d", totalScore);
#endif
}


void DeadNoticeLayer::onQuestEnded(wyTargetSelector* ts) {
    btn_quit->setVisible(true);
    btn_restart->setVisible(true);
    
#if ANDROID
    submitScore(totalScore);
//    LOGE("submit score:%d", totalScore);
#endif
}

void DeadNoticeLayer::createMissionContainer(int i) {

}

void DeadNoticeLayer::gradeShake() {
    sGlobal->playEffect(se_menu);
    
    wyIntervalAction* a = wyShake::make(0.2f, 5);
    highestscore->runAction(a);
    wyQuadParticleSystem *emitter = wyParticleLoader::load("particle_star.plist");
    emitter->setPosition(highestscore->getPositionX(), highestscore->getPositionY());
    addChildLocked(emitter, 9);
    emitter->setScaleX(sGlobal->scaleX/0.75);
    emitter->setScaleY(sGlobal->scaleY/0.75);
    emitter->setAutoRemoveOnFinish(true);
}

void DeadNoticeLayer::calcAll() {
    char temp1[30];
    addComma(sGlobal->currentDistance, temp1);
    char temp2[50];
    sprintf(temp2, "%sm", temp1);
    number_distance->setText(temp2);
    
    char temp[20];
    sprintf(temp, "%d", sGlobal->level_enemy_kill);
    number_kills->setText(temp);
    
    sprintf(temp, "%d", sGlobal->level_bosses);
    number_bosses->setText(temp);
    
    sprintf(temp, "%d", sGlobal->level_gold);
    number_gold->setText(temp);
    
    float score = sGlobal->currentDistance/4 + sGlobal->level_enemy_kill*20 + sGlobal->level_gold+sGlobal->level_bosses*1000;
    score = score * 2.71828183f * 10;
    switch (sGlobal->currentEModeDifficulty) {
        case DIFFICULTY_HEROIC:
            score *= 2;
            break;
        case DIFFICULTY_HARD:
            score *= 4;
            break;
        default:
            break;
    }
    totalScore = (int)score;
    sprintf(temp, "%d", totalScore);
    number_score->setText(temp);
    
    if (sGlobal->isRealEndless) {
//        sGlobal->runDistance += sGlobal->currentDistance;
//        MyPrefs::setInt(DB_RUN_DISTANCE, sGlobal->runDistance);
//        sGlobal->totalRunDistance += sGlobal->currentDistance;
//        MyPrefs::setInt(DB_TOTAL_RUN_DISTANCE, sGlobal->totalRunDistance);
        
        isHighScore = false;
        switch (sGlobal->currentEModeDifficulty) {
            case DIFFICULTY_NORMAL:
                if (totalScore > sGlobal->distRecNormal) {
                    sGlobal->distRecNormal = totalScore;
                    MyPrefs::setInt(DB_DIST_REC_NORMAL, sGlobal->distRecNormal);
                    isHighScore = true;
                }
                break;
            case DIFFICULTY_HEROIC:
                if (totalScore > sGlobal->distRecHeroic) {
                    sGlobal->distRecHeroic = totalScore;
                    MyPrefs::setInt(DB_DIST_REC_HEROIC, sGlobal->distRecHeroic);
                    isHighScore = true;
                }
                break;
            case DIFFICULTY_HARD:
                if (totalScore > sGlobal->distRecInfernal) {
                    sGlobal->distRecInfernal = totalScore;
                    MyPrefs::setInt(DB_DIST_REC_INFERNAL, sGlobal->distRecInfernal);
                    isHighScore = true;
                }
                break;
            default:
                break;
        }
        
        if (isHighScore) {
            highestscore = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "endless_highestscore.png");
            highestscore->setScaleX(sGlobal->scaleX*3.6f);
            highestscore->setScaleY(sGlobal->scaleY*3.6f);
            highestscore->setPosition(wyDevice::winWidth/2 - DP(10)*sGlobal->scaleX, DP(295)*sGlobal->scaleY);
            addChildLocked(highestscore, 10);
            highestscore->setVisible(false);
        }
    }
    
    qLayer = (QuestLayer*) (new QuestLayer(true))->autoRelease();
    this->addChildLocked(qLayer, 101, 0);
    qLayer->setQuestEndSelector(wyTargetSelector::make(this, SEL(DeadNoticeLayer::onQuestEnded)));
    if (qLayer != NULL) {
        qLayer->startAnimation();
    }

}

void DeadNoticeLayer::onWenhaoClicked(wyTargetSelector *ts) {
    PopupDialog *popupDialog = new PopupDialog();
    popupDialog->autoRelease();
    popupDialog->setContent("");
    popupDialog->showOnlyOkBtn();
    addChildLocked(popupDialog, 101, 0);
    
    popupDialog->okBtn->setPosition(popupDialog->okBtn->getPositionX(), popupDialog->okBtn->getPositionY()-DP(4)*sGlobal->scaleY);
    
    wySpriteEx *skill = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "skill_3.png");
    skill->setScaleX(sGlobal->scaleX*0.5f);
    skill->setScaleY(sGlobal->scaleY*0.5f);
    skill->setAnchor(0, 0.5f);
    skill->setPosition(-DP(120)*sGlobal->scaleX, DP(60)*sGlobal->scaleY);
    popupDialog->container->addChildLocked(skill);
    
    char tempp[50];
    float multi = (1+sGlobal->skillMulti[SKILL_GREED]);
    sprintf(tempp, "greed: x%.2f", multi);
    wyBitmapFontLabel *skill_title = wyBitmapFontLabel::make(sGlobal->font_desc, tempp);
    skill_title->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    skill_title->setAlignment(wyBitmapFontLabel::LEFT);
    skill_title->setAnchor(0, 0.5f);
    skill_title->setPosition(skill->getPositionX()+DP(60)*sGlobal->scaleY, skill->getPositionY()+DP(-6)*sGlobal->scaleY);
    popupDialog->container->addChildLocked(skill_title);
    
    wyBitmapFontLabel *dbcoin_title = wyBitmapFontLabel::make(sGlobal->font_desc, "");
    if (sGlobal->isDoubleCoin) {
        sprintf(tempp, "double coins: x2");
    } else {
        sprintf(tempp, "double coins: x2     not activated");
        dbcoin_title->setColor(wyc3b(142, 142, 142));
    }
    
    dbcoin_title->setText(tempp);
    dbcoin_title->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    dbcoin_title->setAlignment(wyBitmapFontLabel::LEFT);
    dbcoin_title->setAnchor(0, 0.5f);
    dbcoin_title->setPosition(-DP(115)*sGlobal->scaleX, DP(15)*sGlobal->scaleY);
    popupDialog->container->addChildLocked(dbcoin_title);
    
    if (sGlobal->isDoubleCoin) {
        skill->setPosition(skill->getPositionX()+DP(40)*sGlobal->scaleX, skill->getPositionY());
        skill_title->setPosition(skill_title->getPositionX()+DP(40)*sGlobal->scaleX, skill_title->getPositionY());
        dbcoin_title->setPosition(dbcoin_title->getPositionX()+DP(40)*sGlobal->scaleX, dbcoin_title->getPositionY());
    }
}


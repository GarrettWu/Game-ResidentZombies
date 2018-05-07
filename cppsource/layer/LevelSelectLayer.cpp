//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "LevelSelectLayer.h"
#include "Global.h"
#include "MainScene.h"
#include "LoadingScene.h"
#include "UpgradeScene.h"
#if ANDROID
#include "com_wiyun_engine_skeleton_Skeleton.h"
#endif
#if IOS
#import "ObjCCalls.h"
#endif
#include "DialogBgLayer.h"
#include "ShopLayer.h"


ConfirmDialogLayer::ConfirmDialogLayer(int parentType, int prevId, int type) {
    this->parentType = parentType;
    this->prevId = prevId;
    
    const char* content;
    switch (type) {
        case 0:
            content = OTHER_STR[STR_SHOP_COIN_LACK];
            break;
        case 1:
            content = OTHER_STR[STR_SHOP_CASH_LACK];
            break;
        case 2:
            content = OTHER_STR[STR_SHOP_COIN_LACK];
            break;
        default:
            content = OTHER_STR[STR_SHOP_COIN_LACK];
            break;
    }

    PopupDialog *popupDialog = new PopupDialog();
    popupDialog->autoRelease();
    popupDialog->setContent(content);
    popupDialog->setOkButtonSelector(wyTargetSelector::make(this, SEL_p(ConfirmDialogLayer::onBtn1Click), popupDialog));
    popupDialog->setCancelButtonSelector(wyTargetSelector::make(this, SEL_p(ConfirmDialogLayer::onBtn2Click), popupDialog));
    addChildLocked(popupDialog, 101, 0);
    
    listener = NULL;
}

void ConfirmDialogLayer::onEnter() {
    wyLayer::onEnter();
    
//    wyIntervalAction* a2 = wyScaleTo::make(0.6f, 0.8f, 0.8f, 1.0f, 1.0f);
//    wyIntervalAction *a21 = wyEaseElasticOut::make(0.6f, a2);
//    container->runAction(a21);
//    this->runAction(wyFadeTo::make(0.1f, 255, 50, true));
}

void ConfirmDialogLayer::onBtn1Click(wyTargetSelector* ts) {
    ShopLayer *layer = new ShopLayer();
    layer->autoRelease();
    layer->listener = listener;
    this->getParent()->addChildLocked(layer, 101);
    
    this->getParent()->removeChildLocked(this, true);
    
#if ANDROID
    if (this->parentType == 0) {
        sGlobal->reportGameEvent(EVT_SHOP_UPGRADE_LACK);
    }
    else if (this->parentType == 1) {
        sGlobal->reportGameEvent(EVT_SHOP_INGAME);
    }
#endif
}

void ConfirmDialogLayer::onBtn2Click(wyTargetSelector* ts) {
    if (listener != NULL) {
        listener->onShopEnd(0);
    }
    
    this->getParent()->removeChildLocked(this, true);
}

bool ConfirmDialogLayer::touchesBegan(wyMotionEvent& event) {
    return true;
}



NanDuSelectLayer::NanDuSelectLayer() {
    wyColorLayer *bg1 = wyColorLayer::make();
    bg1->setColor(wyc4b(0, 0, 0, 100));
    bg1->setPosition(0, 0);
    addChildLocked(bg1);
    
    container = wyNode::make();
    container->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    addChildLocked(container);
    
    DialogBgLayer *bgLayer = DialogBgLayer::make();
    bgLayer->setScaleY(0.8f);
    bgLayer->setScaleX(1.0f);
    bgLayer->setAnchor(0, 0);
    bgLayer->setPosition(bgLayer->getPositionX()-DP(4)*sGlobal->scaleX, bgLayer->getPositionY()-DP(10)*sGlobal->scaleX);
    container->addChildLocked(bgLayer);

    wySpriteEx *normal, *mask;
    bool unlocked = true;
    bool needUnlock = false;
    
    if (sGlobal->isRealEndless) {
        if (sGlobal->isEModeUnlockable(DIFFICULTY_NORMAL)) {
            unlocked = false;
            needUnlock = true;
            
            //表示已经解锁了！
            sGlobal->distRecNormal = 0;
            MyPrefs::setInt(DB_DIST_REC_NORMAL, 0);
        } else {
            unlocked = sGlobal->isEModeActive(DIFFICULTY_NORMAL);
        }
    } else {
        unlocked = sGlobal->isSModeActive(DIFFICULTY_NORMAL);
    }
    if (unlocked) {
        normal = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "easy1.png");
        mask = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "easy1.png");
    } else {
        normal = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "easy2.png");
        mask = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "easy2.png");
    }
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    ControlButton *btn_easy = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL_i(NanDuSelectLayer::onNanduBtnClick), DIFFICULTY_NORMAL), mask, NULL);
    container->addChildLocked(btn_easy, 1, 0);
    btn_easy->setPosition(-DP(160)*sGlobal->scaleX, DP(10)*sGlobal->scaleY);
    btn_easy->setScaleX(sGlobal->scaleX*0.7f);
    btn_easy->setScaleY(sGlobal->scaleY*0.7f);
    btn_easy->setTouchPriority(1002);
    btn_easy->setInterceptTouch(true);
    btn_easy->setContentSize(btn_easy->getContentSize().width*1.2, btn_easy->getContentSize().height*3);
    if (needUnlock) {
        showUnlockEffect(btn_easy);
    }
    
//    wyBitmapFontLabel *txt1 = wyBitmapFontLabel::make(sGlobal->font_desc, "easy");
//    txt1->setScale(SP(21)*sGlobal->scaleX/sGlobal->descFontSize);
//    txt1->setPosition(btn_easy->getPositionX(), btn_easy->getPositionY()-DP(95)*sGlobal->scaleY);
//    txt1->setColor(wyc3b(220, 220, 220));
//    container->addChildLocked(txt1);

    needUnlock = false;
    if (sGlobal->isRealEndless) {
        if (sGlobal->isEModeUnlockable(DIFFICULTY_HEROIC)) {
            unlocked = false;
            needUnlock = true;

            sGlobal->distRecHeroic = 0;
            MyPrefs::setInt(DB_DIST_REC_HEROIC, 0);
        } else {
            unlocked = sGlobal->isEModeActive(DIFFICULTY_HEROIC);
        }
    } else {
        if (sGlobal->isSModeUnlockable(DIFFICULTY_HEROIC)) {
            unlocked = false;
            needUnlock = true;
            
            sGlobal->currentProgress[DIFFICULTY_HEROIC] = 101;
            sGlobal->arrayToStr(DB_PROGRESS, sGlobal->currentProgress, 3);
        } else {
            unlocked = sGlobal->isSModeActive(DIFFICULTY_HEROIC);
        }
    }
    if (unlocked) {
        normal = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "hard1.png");
        mask = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "hard1.png");
    } else {
        normal = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "hard2.png");
        mask = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "hard2.png");
    }
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    ControlButton *btn_hard = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL_i(NanDuSelectLayer::onNanduBtnClick), DIFFICULTY_HEROIC), mask, NULL);
    container->addChildLocked(btn_hard, 1, 1);
    btn_hard->setPosition(DP(10)*sGlobal->scaleX, btn_easy->getPositionY());
    btn_hard->setScaleX(sGlobal->scaleX*0.7f);
    btn_hard->setScaleY(sGlobal->scaleY*0.7f);
    btn_hard->setTouchPriority(1002);
    btn_hard->setInterceptTouch(true);
    btn_hard->setContentSize(btn_hard->getContentSize().width*1.2, btn_hard->getContentSize().height*3);
    if (needUnlock) {
        showUnlockEffect(btn_hard);
    }
    
//    wyBitmapFontLabel *txt2 = wyBitmapFontLabel::make(sGlobal->font_desc, "hard");
//    txt2->setScale(SP(21)*sGlobal->scaleX/sGlobal->descFontSize);
//    txt2->setPosition(btn_hard->getPositionX(), txt1->getPositionY());
//    txt2->setColor(wyc3b(220, 220, 220));
//    container->addChildLocked(txt2);
    
    needUnlock = false;
    if (sGlobal->isRealEndless) {
        if (sGlobal->isEModeUnlockable(DIFFICULTY_HARD)) {
            unlocked = false;
            needUnlock = true;
            
            sGlobal->distRecInfernal = 0;
            MyPrefs::setInt(DB_DIST_REC_INFERNAL, 0);
        } else {
            unlocked = sGlobal->isEModeActive(DIFFICULTY_HARD);
        }
    } else {
        if (sGlobal->isSModeUnlockable(DIFFICULTY_HARD)) {
            unlocked = false;
            needUnlock = true;
            
            sGlobal->currentProgress[DIFFICULTY_HARD] = 101;
            sGlobal->arrayToStr(DB_PROGRESS, sGlobal->currentProgress, 3);
        } else {
            unlocked = sGlobal->isSModeActive(DIFFICULTY_HARD);
        }
    }
    if (unlocked) {
        normal = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "crazy1.png");
        mask = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "crazy1.png");
    } else {
        normal = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "crazy2.png");
        mask = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "crazy2.png");
    }
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    ControlButton *btn_crazy = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL_i(NanDuSelectLayer::onNanduBtnClick), DIFFICULTY_HARD), mask, NULL);
    container->addChildLocked(btn_crazy, 1, 2);
    btn_crazy->setPosition(DP(180)*sGlobal->scaleX, btn_easy->getPositionY());
    btn_crazy->setScaleX(sGlobal->scaleX*0.7f);
    btn_crazy->setScaleY(sGlobal->scaleY*0.7f);
    btn_crazy->setTouchPriority(1002);
    btn_crazy->setInterceptTouch(true);
    btn_crazy->setContentSize(btn_crazy->getContentSize().width*1.2, btn_crazy->getContentSize().height*3);
    if (needUnlock) {
        showUnlockEffect(btn_crazy);
    }
    
//    wyBitmapFontLabel *txt3 = wyBitmapFontLabel::make(sGlobal->font_desc, "crazy");
//    txt3->setScale(SP(21)*sGlobal->scaleX/sGlobal->descFontSize);
//    txt3->setPosition(btn_crazy->getPositionX(), txt1->getPositionY());
//    txt3->setColor(wyc3b(220, 220, 220));
//    container->addChildLocked(txt3);
    
    if (sGlobal->isRealEndless) {
        char temp[20];
        
        wyBitmapFontLabel *score1 = wyBitmapFontLabel::make(sGlobal->font_desc, "highest score:");
        score1->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
        score1->setPosition(btn_easy->getPositionX(), btn_easy->getPositionY()-DP(80)*sGlobal->scaleY);
        score1->setColor(wyc3b(220, 220, 220));
        container->addChildLocked(score1);
        
        addComma(MAX(sGlobal->distRecNormal,0), temp);
        wyBitmapFontLabel *score11 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
        score11->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
        score11->setPosition(score1->getPositionX(), score1->getPositionY()-DP(20)*sGlobal->scaleY);
        score11->setColor(wyc3b(220, 220, 220));
        container->addChildLocked(score11);

        wyBitmapFontLabel *score2 = wyBitmapFontLabel::make(sGlobal->font_desc, "highest score:");
        score2->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
        score2->setPosition(btn_hard->getPositionX(), btn_hard->getPositionY()-DP(80)*sGlobal->scaleY);
        score2->setColor(wyc3b(220, 220, 220));
        container->addChildLocked(score2);
        
        addComma(MAX(sGlobal->distRecHeroic,0), temp);
        wyBitmapFontLabel *score22 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
        score22->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
        score22->setPosition(score2->getPositionX(), score2->getPositionY()-DP(20)*sGlobal->scaleY);
        score22->setColor(wyc3b(220, 220, 220));
        container->addChildLocked(score22);
        
        wyBitmapFontLabel *score3 = wyBitmapFontLabel::make(sGlobal->font_desc, "highest score:");
        score3->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
        score3->setPosition(btn_crazy->getPositionX(), btn_crazy->getPositionY()-DP(80)*sGlobal->scaleY);
        score3->setColor(wyc3b(220, 220, 220));
        container->addChildLocked(score3);
        
        addComma(MAX(sGlobal->distRecInfernal,0), temp);
        wyBitmapFontLabel *score33 = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
        score33->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
        score33->setPosition(score3->getPositionX(), score3->getPositionY()-DP(20)*sGlobal->scaleY);
        score33->setColor(wyc3b(220, 220, 220));
        container->addChildLocked(score33);
    } else {
        int scount = sGlobal->countScount(1, DIFFICULTY_NORMAL) + sGlobal->countScount(2, DIFFICULTY_NORMAL) + sGlobal->countScount(3, DIFFICULTY_NORMAL);
        char temp[10];
        sprintf(temp, "%d/30", scount);
        wyBitmapFontLabel *mapInfo = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
        mapInfo->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
        mapInfo->setAlignment(wyBitmapFontLabel::LEFT);
        mapInfo->setPosition(btn_easy->getPositionX()-DP(4)*sGlobal->scaleX, btn_easy->getPositionY()-DP(80)*sGlobal->scaleY);
        mapInfo->setColor(wyc3b(220, 220, 220));
        container->addChildLocked(mapInfo);
        
        wySpriteEx *mapInfoGrade1 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "score_s_small.png");
        mapInfoGrade1->setScaleX(sGlobal->scaleX*0.8);
        mapInfoGrade1->setScaleY(sGlobal->scaleY*0.8);
        mapInfoGrade1->setPosition(mapInfo->getPositionX()+DP(40)*sGlobal->scaleX, mapInfo->getPositionY()+DP(2)*sGlobal->scaleX);
        container->addChildLocked(mapInfoGrade1);
        
        scount = sGlobal->countScount(1, DIFFICULTY_HEROIC) + sGlobal->countScount(2, DIFFICULTY_HEROIC) + sGlobal->countScount(3, DIFFICULTY_HEROIC);
        sprintf(temp, "%d/30", scount);
        mapInfo = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
        mapInfo->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
        mapInfo->setAlignment(wyBitmapFontLabel::LEFT);
        mapInfo->setPosition(btn_hard->getPositionX()-DP(4)*sGlobal->scaleX, btn_hard->getPositionY()-DP(80)*sGlobal->scaleY);
        mapInfo->setColor(wyc3b(220, 220, 220));
        container->addChildLocked(mapInfo);
        
        mapInfoGrade1 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "score_s_small.png");
        mapInfoGrade1->setScaleX(sGlobal->scaleX*0.8);
        mapInfoGrade1->setScaleY(sGlobal->scaleY*0.8);
        mapInfoGrade1->setPosition(mapInfo->getPositionX()+DP(40)*sGlobal->scaleX, mapInfo->getPositionY()+DP(2)*sGlobal->scaleX);
        container->addChildLocked(mapInfoGrade1);
        
        scount = sGlobal->countScount(1, DIFFICULTY_HARD) + sGlobal->countScount(2, DIFFICULTY_HARD) + sGlobal->countScount(3, DIFFICULTY_HARD);
        sprintf(temp, "%d/30", scount);
        mapInfo = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
        mapInfo->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
        mapInfo->setAlignment(wyBitmapFontLabel::LEFT);
        mapInfo->setPosition(btn_crazy->getPositionX()-DP(4)*sGlobal->scaleX, btn_crazy->getPositionY()-DP(80)*sGlobal->scaleY);
        mapInfo->setColor(wyc3b(220, 220, 220));
        container->addChildLocked(mapInfo);
        
        mapInfoGrade1 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "score_s_small.png");
        mapInfoGrade1->setScaleX(sGlobal->scaleX*0.8);
        mapInfoGrade1->setScaleY(sGlobal->scaleY*0.8);
        mapInfoGrade1->setPosition(mapInfo->getPositionX()+DP(40)*sGlobal->scaleX, mapInfo->getPositionY()+DP(2)*sGlobal->scaleX);
        container->addChildLocked(mapInfoGrade1);
    }
    
    char temp[30];
    if (sGlobal->isRealEndless) {
        sprintf(temp, "%s", "endless      mode");
    } else {
        sprintf(temp, "%s", "story      mode");
    }
    wyBitmapFontLabel *content = wyBitmapFontLabel::make(sGlobal->font_title, temp);
    content->setScale(SP(24)*sGlobal->scaleX/sGlobal->descFontSize);
    content->setAlignment(wyBitmapFontLabel::LEFT);
    content->setAnchor(0, 0.5f);
    container->addChildLocked(content);
    content->setPosition(-DP(220)*sGlobal->scaleX, DP(90)*sGlobal->scaleY);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "nandu_x.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "nandu_x.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    ControlButton *btn_close = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(NanDuSelectLayer::onCloseButtonClick)), mask, NULL);
    container->addChildLocked(btn_close);
    btn_close->setPosition(bgLayer->getWidth()*bgLayer->getScaleX()/2-DP(2)*sGlobal->scaleX, bgLayer->getHeight()*bgLayer->getScaleY()/2-DP(4)*sGlobal->scaleX);
    btn_close->setScaleX(sGlobal->scaleX*0.55f);
    btn_close->setScaleY(sGlobal->scaleY*0.55f);
    btn_close->setTouchPriority(1002);
    btn_close->setInterceptTouch(true);

    this->setTouchEnabled(true);
    this->setTouchPriority(1001);
    this->setInterceptTouch(true);
    
    isEnterAnimEnd = false;

    container->setScale(0);
}

static void onEnterAnimEnd(wyAction* action, void* data) {
    NanDuSelectLayer *layer = (NanDuSelectLayer *)data;
    
    layer->isEnterAnimEnd = true;
}

void NanDuSelectLayer::onEnter() {
    wyLayer::onEnter();
    
    wyIntervalAction* a2 = wyScaleTo::make(0.2f, 0.8f, 0.8f, 1.0f, 1.0f);
    wyIntervalAction *a21 = wyEaseBackOut::make(a2);

    wyActionCallback callback = {
        NULL,
        onEnterAnimEnd,
        NULL
    };
    a21->setCallback(&callback, this);
    container->runAction(a21);
    container->setScale(0.8f);
    
    isEnterAnimEnd = true;
}

void NanDuSelectLayer::onUnlockEmitterEnd(wyTargetSelector* ts, void *data) {
    ControlButton *btn = (ControlButton *)data;
    wySpriteEx *mask;
    switch (btn->getTag()) {
        case DIFFICULTY_NORMAL:
            btn->setNormalState(wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "easy1.png"));
            mask = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "easy1.png");
            break;
        case DIFFICULTY_HEROIC:
            btn->setNormalState(wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "hard1.png"));
            mask = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "hard1.png");
            break;
        case DIFFICULTY_HARD:
            btn->setNormalState(wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "crazy1.png"));
            mask = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "crazy1.png");
            break;
        default:
            btn->setNormalState(wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "crazy1.png"));
            mask = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "crazy1.png");
            break;
            break;
    }
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });                       
    btn->setMask(mask);
    wyAction *a = wyFadeOut::make(0.6f);
    mask->runAction(a);
    mask->setVisible(true);
    btn->setContentSize(btn->getContentSize().width*1.2, btn->getContentSize().height*3);
    
    wyDirector* director = wyDirector::getInstance();
    wyNode *node = director->getRunningScene()->getChildByTag(btn->getTag());
    if (node != NULL) {
        node->getParent()->removeChildLocked(node, true);
    }
}

void NanDuSelectLayer::showUnlockEffect(ControlButton *btn) {
    wyParticleSystem *train_skill_emitter = wyParticleLoader::load("particle_train_skill.plist");
    container->addChildLocked(train_skill_emitter, 999);
    
    float scale = sGlobal->scaleX;
    float width = btn->getWidth()*btn->getScaleX();
    float height = btn->getHeight()*btn->getScaleY();
    train_skill_emitter->setStartSizeVariance(12, 5);
    train_skill_emitter->setParticlePositionVariance(width/2/scale, 0, width/2/scale, 0);
    train_skill_emitter->setEmissionRate(100);
    train_skill_emitter->setDuration(1.0f);
    train_skill_emitter->setLifeVariance(0.6, 0.2);
    train_skill_emitter->setScale(scale);
    train_skill_emitter->setPosition(btn->getPositionX()-width/2+DP(0)*sGlobal->scaleY, btn->getPositionY()-height/2+DP(0)*sGlobal->scaleY);
    train_skill_emitter->setAutoRemoveOnFinish(true);
    train_skill_emitter->setPositionType(GROUPED);
    
    btn->runAction(wySequence::make(wyDelayTime::make(0.4f), wyCallFunc::make(wyTargetSelector::make(this, SEL_p(NanDuSelectLayer::onUnlockEmitterEnd), btn)), NULL));
    
    sGlobal->playEffect(se_upgrade);
    
    wyNode *node = TouchLayer::make();
    wyDirector* director = wyDirector::getInstance();
    director->getRunningScene()->addChildLocked(node, 0, btn->getTag());
}

void NanDuSelectLayer::onDifDisabled(wyTargetSelector* ts, void *data) {
    PopupDialog *popupDialog = (PopupDialog*) data;
    popupDialog->getParent()->removeChildLocked(popupDialog, true);
}

void NanDuSelectLayer::onNanduBtnClick(wyTargetSelector* ts, int data) {
//    sGlobal->playEffect("se_button.mp3");
    
    if (sGlobal->isDebug) {
        if (sGlobal->isRealEndless) {
            sGlobal->currentEModeDifficulty = data;
            sGlobal->gotoUpgradeScene(100);
        } else {
            sGlobal->currentSModeDifficulty = data;
            sGlobal->gotoWorldMapScene(false);
        }
        return;
    }
    
    bool canStart = true;
    if (sGlobal->isRealEndless) {
        switch (data) {
            case DIFFICULTY_NORMAL: {
                if (sGlobal->s02_score[10] == 0) {
                    canStart = false;
                    
                    PopupDialog *popupDialog = new PopupDialog();
                    popupDialog->autoRelease();
                    popupDialog->setContent(OTHER_STR[STR_ULK_ENDLESS_EASY]);
                    popupDialog->setOkButtonSelector(wyTargetSelector::make(this, SEL_p(NanDuSelectLayer::onDifDisabled), popupDialog));
                    popupDialog->showOnlyOkBtn();
                    addChildLocked(popupDialog, 101, 0);
                }
                break;
            }
            case DIFFICULTY_HEROIC: {
                if (sGlobal->s02_score[10+DIFFICULTY_HEROIC*10] == 0) {
                    canStart = false;
                    
                    PopupDialog *popupDialog = new PopupDialog();
                    popupDialog->autoRelease();
                    popupDialog->setContent(OTHER_STR[STR_ULK_ENDLESS_HARD]);
                    popupDialog->setOkButtonSelector(wyTargetSelector::make(this, SEL_p(NanDuSelectLayer::onDifDisabled), popupDialog));
                    popupDialog->showOnlyOkBtn();
                    addChildLocked(popupDialog, 101, 0);
                }
                break;
            }
            case DIFFICULTY_HARD: {
                if (sGlobal->s02_score[10+DIFFICULTY_HARD*10] == 0) {
                    canStart = false;
                    
                    PopupDialog *popupDialog = new PopupDialog();
                    popupDialog->autoRelease();
                    popupDialog->setContent(OTHER_STR[STR_ULK_ENDLESS_CRAZY]);
                    popupDialog->setOkButtonSelector(wyTargetSelector::make(this, SEL_p(NanDuSelectLayer::onDifDisabled), popupDialog));
                    popupDialog->showOnlyOkBtn();
                    addChildLocked(popupDialog, 101, 0);
                }
                break;
            }
            default:
                break;
        }
        
        if (canStart) {
            sGlobal->currentEModeDifficulty = data;
            sGlobal->gotoUpgradeScene(100);
        }
    } else {
        switch (data) {
            case DIFFICULTY_HEROIC: {
                if (sGlobal->s03_score[10] == 0) {
                    canStart = false;
                    
                    PopupDialog *popupDialog = new PopupDialog();
                    popupDialog->autoRelease();
                    popupDialog->setContent(OTHER_STR[STR_ULK_SMODE_HARD]);
                    popupDialog->setOkButtonSelector(wyTargetSelector::make(this, SEL_p(NanDuSelectLayer::onDifDisabled), popupDialog));
                    popupDialog->showOnlyOkBtn();
                    addChildLocked(popupDialog, 101, 0);
                }
                break;
            }
            case DIFFICULTY_HARD: {
                if (sGlobal->s03_score[10+DIFFICULTY_HEROIC*10] == 0) {
                    canStart = false;
                    
                    PopupDialog *popupDialog = new PopupDialog();
                    popupDialog->autoRelease();
                    popupDialog->setContent(OTHER_STR[STR_ULK_SMODE_CRAZY]);
                    popupDialog->setOkButtonSelector(wyTargetSelector::make(this, SEL_p(NanDuSelectLayer::onDifDisabled), popupDialog));
                    popupDialog->showOnlyOkBtn();
                    addChildLocked(popupDialog, 101, 0);
                }
                break;
            }
            default:
                break;
        }
        
        if (canStart) {
            sGlobal->currentSModeDifficulty = data;
            sGlobal->gotoWorldMapScene(false);
        }
    }
    
}

bool NanDuSelectLayer::touchesBegan(wyMotionEvent& event) {
//    if (isEnterAnimEnd) {
//        this->getParent()->removeChildLocked(this, true);
//    }
    
    return true;
}

bool NanDuSelectLayer::touchesMoved(wyMotionEvent& event) {
//    this->getParent()->removeChildLocked(this, true);
    return true;
}

bool NanDuSelectLayer::touchesEnded(wyMotionEvent& event) {
//    this->getParent()->removeChildLocked(this, true);
    return true;
}

void NanDuSelectLayer::onCloseButtonClick(wyTargetSelector* ts) {
//    sGlobal->playEffect("se_button.mp3");
    
    if (isEnterAnimEnd) {
        this->getParent()->removeChildLocked(this, true);
    }
}





PopupDialog::PopupDialog() {
    wyColorLayer *bg1 = wyColorLayer::make();
    bg1->setColor(wyc4b(0, 0, 0, 100));
    bg1->setPosition(0, 0);
    addChildLocked(bg1);
    
    container = wyNode::make();
    container->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    addChildLocked(container);
    
    bgLayer = DialogBgLayer::make();
    bgLayer->setScaleX(0.8f);
    bgLayer->setScaleY(0.6f);
    bgLayer->setAnchor(0, 0);
    container->addChildLocked(bgLayer);
    
    content = wyBitmapFontLabel::make(sGlobal->font_desc, "");
    content->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    content->setAlignment(wyBitmapFontLabel::LEFT);
    content->setLineWidth(DP(350)*sGlobal->scaleX/content->getScale());
    content->setPosition(DP(-160)*sGlobal->scaleX, DP(85)*sGlobal->scaleY);
    content->setAnchor(0, 1.0f);
    container->addChildLocked(content);
//    content->setVisible(false);
    
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    wyBitmapFontLabel *title = wyBitmapFontLabel::make(sGlobal->font_desc, "ok");
    title->setScale(SP(24)/sGlobal->descFontSize);
    okBtn = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(PopupDialog::defaultOnOkButtonClick)), mask, title);
    okBtn->setScaleX(sGlobal->scaleX*0.8f);
    okBtn->setScaleY(sGlobal->scaleY*0.8f);
    okBtn->setMultiTouchClickable(false);
    okBtn->setInterceptTouch(true);
    okBtn->setTouchPriority(10000);
    container->addChildLocked(okBtn, 100);
    okBtn->setPosition(DP(-90)*sGlobal->scaleX, -DP(45)*sGlobal->scaleY);
//    okBtn->setVisible(false);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "cancel");
    title->setScale(SP(24)/sGlobal->descFontSize);
    cancelBtn = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(PopupDialog::defaultOnCancelButtonClick)), mask, title);
    cancelBtn->setScaleX(sGlobal->scaleX*0.8f);
    cancelBtn->setScaleY(sGlobal->scaleY*0.8f);
    cancelBtn->setMultiTouchClickable(false);
    cancelBtn->setInterceptTouch(true);
    cancelBtn->setTouchPriority(10000);
    container->addChildLocked(cancelBtn, 100);
    cancelBtn->setPosition(okBtn->getPositionX()+DP(205)*sGlobal->scaleX, okBtn->getPositionY());
//    cancelBtn->setVisible(false);
    
    this->setTouchEnabled(true);
    this->setTouchPriority(9999);
    this->setInterceptTouch(true);
    
    isEnterAnimEnd = false;
}

static void onPopupDialogEnterAnimEnd(wyAction* action, void* data) {
    PopupDialog *layer = (PopupDialog *)data;
    
    layer->isEnterAnimEnd = true;
}

void PopupDialog::onEnter() {
    wyLayer::onEnter();
    
    wyIntervalAction* a2 = wyScaleTo::make(0.2f, 0.8f, 0.8f, 1.0f, 1.0f);
    wyIntervalAction *a21 = wyEaseBackOut::make(a2);
    
    wyActionCallback callback = {
        NULL,
        onEnterAnimEnd,
        NULL
    };
    a21->setCallback(&callback, this);
    container->runAction(a21);
    container->setScale(0.8f);
    
    isEnterAnimEnd = true;
}

bool PopupDialog::touchesBegan(wyMotionEvent& event) {
    return true;
}

void PopupDialog::setContent(const char *text) {
    content->setText(text);
    content->setVisible(true);
}

void PopupDialog::setOkButtonSelector(wyTargetSelector* sel) {
    okBtn->setButtonClickSelector(sel);
    okBtn->setVisible(true);
}

void PopupDialog::setCancelButtonSelector(wyTargetSelector* sel) {
    cancelBtn->setButtonClickSelector(sel);
    cancelBtn->setVisible(true);
}

void PopupDialog::defaultOnCancelButtonClick(wyTargetSelector* ts) {
//    sGlobal->playEffect("se_button.mp3");
    
    this->getParent()->removeChildLocked(this, true);
}

void PopupDialog::defaultOnOkButtonClick(wyTargetSelector* ts) {
//    sGlobal->playEffect("se_button.mp3");
    
    this->getParent()->removeChildLocked(this, true);
}

void PopupDialog::showOnlyOkBtn() {
    cancelBtn->setVisible(false);
    
    okBtn->setPosition(DP(8)*sGlobal->scaleX, okBtn->getPositionY());
}



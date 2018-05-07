//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved. 
//

#include "OptionLayer.h"
#include "Global.h"
#include "MainScene.h"
#include "LoadingScene.h"
#include "LevelSelectLayer.h"

OptionLayer::OptionLayer() : wyColorLayer(wyc4b(0, 0, 0, 100)) {
//    wyBitmapFont* font = wyBitmapFont::loadFont("font_tc_en"), BFF_ANGELCODE_XML);
//    LOGE("targetDistance1:%d", sGlobal->achv_progress[ACHV_RUN_1000]);
	MyPrefs::setInt(DB_GOLD, sGlobal->getGold());
    MyPrefs::setInt(DB_ZUANSHI_NUM, sGlobal->zuanshi.getValue());

    sGlobal->optionLayer = this;

    sGlobal->needPauseGame = false;
    
    initCommonUI();
    if (sGlobal->isRealEndless) {
        initEndlessUI();
    } else {
        initNormalUI();
    }

    this->setTouchEnabled(false);
    
//    sGlobal->addPurchaseEndListener(this);
}

OptionLayer::~OptionLayer() {
//    sGlobal->removePurchaseEndListener(this);
//    LOGE("~OptionLayer...");
    sGlobal->optionLayer = NULL;
//    sGlobal->needPauseGame = true;
}

void OptionLayer::onEnterTransitionDidFinish() {
    wyLayer::onEnterTransitionDidFinish();
    
//    this->runAction(wyFadeTo::make(0.1, 255, 50, true));
}

void OptionLayer::initCommonUI() {
    container = wyNode::make();
    container->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    container->setAnchor(0.5f, 0.5f);
    addChildLocked(container);
    
    bgLayer = DialogBgLayer::make();
    container->addChildLocked(bgLayer);
    
    title = wyBitmapFontLabel::make(sGlobal->font_title, "a");
    title->setScale(SP(40)*sGlobal->scaleX/sGlobal->titleFontSize);
    title->setAlignment(wyBitmapFontLabel::LEFT);
    title->setAnchor(0, 0);
    title->setPosition(-DP(220)*sGlobal->scaleX, DP(130)*sGlobal->scaleY);
    container->addChildLocked(title);
    
    wySprite *normal;
    normal = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "toggle_se.png");
    se_toggle = wyMenuItemSprite::make(NULL,
                                       wyTargetSelector::make(this, SEL(OptionLayer::onSEClicked)),
                                       normal,
                                       NULL,
                                       NULL);
    se_toggle->setPosition(DP(150)*sGlobal->scaleX, DP(80)*sGlobal->scaleY);
    se_toggle->setClickScale(1.0f);
    se_toggle->setScale(sGlobal->scaleX*0.4);
    
    normal = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "toggle_bgm.png");
    bgm_toggle = wyMenuItemSprite::make(NULL,
                                        wyTargetSelector::make(this, SEL(OptionLayer::onBGMClicked)),
                                        normal,
                                        NULL,
                                        NULL);
    bgm_toggle->setPosition(DP(150)*sGlobal->scaleX, -DP(20)*sGlobal->scaleY);
    bgm_toggle->setClickScale(1.0f);
    bgm_toggle->setScale(sGlobal->scaleX*0.4);
    
    menu1 = wyMenu::make(bgm_toggle, se_toggle, NULL);
    menu1->setPosition(0, 0);
    menu1->setAnchor(0, 0);
    menu1->setTouchPriority(99999);
    menu1->setInterceptTouch(true);
    container->addChildLocked(menu1);
    
    se_disable = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "sound_disable.png");
    se_disable->setPosition(se_toggle->getPositionX(), se_toggle->getPositionY());
    se_disable->setScale(sGlobal->scaleX*0.4);
    container->addChildLocked(se_disable);
    se_disable->setVisible(!sGlobal->isSFXOn);
    
    bgm_disable = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "sound_disable.png");
    bgm_disable->setPosition(bgm_toggle->getPositionX(), bgm_toggle->getPositionY());
    bgm_disable->setScale(sGlobal->scaleX*0.4);
    container->addChildLocked(bgm_disable);
    bgm_disable->setVisible(!sGlobal->isBGMOn);
    
    normal = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "btn_common_normal.png");
    wySprite *mask = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    wyBitmapFontLabel* title1 = wyBitmapFontLabel::make(sGlobal->font_desc, "resume");
    title1->setScale(SP(22)/sGlobal->descFontSize);
    btn_resume = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(OptionLayer::onResumeClicked)), mask, title1);
    btn_resume->setScaleX(sGlobal->scaleX*0.8f);
    btn_resume->setScaleY(sGlobal->scaleY*0.8f);
    btn_resume->setMultiTouchClickable(false);
    btn_resume->setInterceptTouch(true);
    btn_resume->setTouchPriority(1000);
    container->addChildLocked(btn_resume, 100);
    btn_resume->setPosition(-DP(165)*sGlobal->scaleX, -DP(120)*sGlobal->scaleY);
    btn_resume->needDelay = true;
    
    normal = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title1 = wyBitmapFontLabel::make(sGlobal->font_desc, "restart");
    title1->setScale(SP(22)/sGlobal->descFontSize);
    btn_restart = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(OptionLayer::onRestartClicked)), mask, title1);
    btn_restart->setScaleX(sGlobal->scaleX*0.8f);
    btn_restart->setScaleY(sGlobal->scaleY*0.8f);
    btn_restart->setMultiTouchClickable(false);
    btn_restart->setInterceptTouch(true);
    btn_restart->setTouchPriority(1000);
    container->addChildLocked(btn_restart, 100);
    btn_restart->setPosition(DP(0)*sGlobal->scaleX, -DP(120)*sGlobal->scaleY);
    btn_restart->needDelay = false;
    
    normal = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title1 = wyBitmapFontLabel::make(sGlobal->font_desc, "quit");
    title1->setScale(SP(22)/sGlobal->descFontSize);
    btn_quit = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(OptionLayer::onQuitClicked)), mask, title1);
    btn_quit->setScaleX(sGlobal->scaleX*0.8f);
    btn_quit->setScaleY(sGlobal->scaleY*0.8f);
    btn_quit->setMultiTouchClickable(false);
    btn_quit->setInterceptTouch(true);
    btn_quit->setTouchPriority(1000);
    container->addChildLocked(btn_quit, 100);
    btn_quit->setPosition(DP(165)*sGlobal->scaleX, -DP(120)*sGlobal->scaleY);
    btn_quit->needDelay = false;
    
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
    
    normal = wyZwoptexManager::getInstance()->makeSprite("ui_en", "wenhao.png");
    mask = wyZwoptexManager::getInstance()->makeSprite("ui_en", "wenhao.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    btn_wenhao = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(OptionLayer::onWenhaoClicked)), mask, NULL);
    btn_wenhao->setScaleX(sGlobal->scaleX*0.6f);
    btn_wenhao->setScaleY(sGlobal->scaleY*0.6f);
    btn_wenhao->setMultiTouchClickable(false);
    btn_wenhao->setInterceptTouch(true);
    btn_wenhao->setTouchPriority(1000);
    container->addChildLocked(btn_wenhao, 100);
    btn_wenhao->setPosition(DP(165)*sGlobal->scaleX, -DP(120)*sGlobal->scaleY);
    btn_wenhao->needDelay = false;
}

void OptionLayer::onWenhaoClicked(wyTargetSelector *ts) {
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

//    sGlobal->isDoubleCoin=0;
    wyBitmapFontLabel *dbcoin_title = wyBitmapFontLabel::make(sGlobal->font_desc, "");
    if (sGlobal->isDoubleCoin) {
        sprintf(tempp, "double coins: x2");
//        dbcoin_title->setColor(wyc3b(142, 142, 142));
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
    
//    wySpriteEx* normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common.png");
//    wySpriteEx* mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common.png");
//    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
//    wyBitmapFontLabel *title = wyBitmapFontLabel::make(sGlobal->font_desc, "get it");
//    title->setScale(SP(26)/sGlobal->descFontSize);
//    ControlButton *btn_buy_dbcoin = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(OptionLayer::onBuyDBCoin)), mask, title);
//    btn_buy_dbcoin->setScaleX(sGlobal->scaleX*0.6f);
//    btn_buy_dbcoin->setScaleY(sGlobal->scaleY*0.6f);
//    btn_buy_dbcoin->setPosition(DP(110)*sGlobal->scaleX, DP(20)*sGlobal->scaleY);
//    btn_buy_dbcoin->setMultiTouchClickable(false);
//    btn_buy_dbcoin->setInterceptTouch(true);
//    btn_buy_dbcoin->setTouchPriority(101);
//    popupDialog->container->addChildLocked(btn_buy_dbcoin);
    
    if (sGlobal->isDoubleCoin) {
//        btn_buy_dbcoin->setVisible(false);
        
        skill->setPosition(skill->getPositionX()+DP(40)*sGlobal->scaleX, skill->getPositionY());
        skill_title->setPosition(skill_title->getPositionX()+DP(40)*sGlobal->scaleX, skill_title->getPositionY());
        dbcoin_title->setPosition(dbcoin_title->getPositionX()+DP(40)*sGlobal->scaleX, dbcoin_title->getPositionY());
    }
    
//    sGlobal->addPurchaseEndListener(popupDialog);
}

//void OptionLayer::onBuyDBCoin(wyTargetSelector *ts) {
//#if ANDROID
//    sGlobal->purchaseGameItemStart(SHOP_DBCOIN_ID);
//#endif
//}
//
//void OptionLayer::onPurchaseEnded(int itemId) {
//    if (itemId != SHOP_DBCOIN_ID) {
//        return;
//    }
//    
//    char tempp[50];
//    float multi = (1+sGlobal->skillMulti[SKILL_GREED]);
//    if (sGlobal->isDoubleCoin) {
//        multi *= 2;
//    }
//    sprintf(tempp, "coin multiplier: x%.1f", multi);
//    goldMultiplyer->setText(tempp);
//}

void OptionLayer::initNormalUI() {
    int seasonId = sGlobal->index / 100;
    int mapId = sGlobal->index % 100;
    
    char temp[20] = {0};
    if (sGlobal->index == 0) {
        sprintf(temp, "%s", sGlobal->getStr(STR_TUTORIAL_TITLE));
    } else {
        sprintf(temp, sGlobal->getStr(STR_LEVEL_TITLE), seasonId, mapId);
    }
    title->setText(temp);
    title->setScale(SP(42)*sGlobal->scaleX/sGlobal->titleFontSize);
    title->setPosition(-DP(215)*sGlobal->scaleX, DP(115)*sGlobal->scaleY);
    
    goldMultiplyer->setPosition(DP(40)*sGlobal->scaleX, DP(135)*sGlobal->scaleY);
    btn_wenhao->setPosition(DP(230)*sGlobal->scaleX, DP(135)*sGlobal->scaleY);
    
    createDataPanel();
}

void OptionLayer::initEndlessUI() {
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
    
    goldMultiplyer->setPosition(DP(50)*sGlobal->scaleX, DP(150)*sGlobal->scaleY);
    btn_wenhao->setPosition(DP(240)*sGlobal->scaleX, DP(150)*sGlobal->scaleY);
    
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
    float offsetX = -DP(50)*sGlobal->scaleX;
    
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
    
    calcAllForEndless();
    
    wyBitmapFontLabel *title1 = wyBitmapFontLabel::make(sGlobal->font_title, "medals");
    title1->setScale(SP(30)*sGlobal->scaleX/sGlobal->titleFontSize);
    title1->setAlignment(wyBitmapFontLabel::LEFT);
    title1->setAnchor(0, 0);
    title1->setPosition(-DP(225)*sGlobal->scaleX, DP(45)*sGlobal->scaleY);
    title1->setColor(wyc3b(198, 65, 35));
    container->addChildLocked(title1);
    
    qLayer = (QuestLayer*) (new QuestLayer(false))->autoRelease();
    this->addChildLocked(qLayer, 101, 0);
    qLayer->placeAllThings();
    
    se_toggle->setPosition(DP(120)*sGlobal->scaleX, DP(95)*sGlobal->scaleY);
    se_disable->setPosition(se_toggle->getPositionX(), se_toggle->getPositionY());
    bgm_toggle->setPosition(DP(200)*sGlobal->scaleX, DP(95)*sGlobal->scaleY);
    bgm_disable->setPosition(bgm_toggle->getPositionX(), bgm_toggle->getPositionY());
    
    btn_resume->setPosition(-DP(165)*sGlobal->scaleX, -DP(130)*sGlobal->scaleY);
    btn_restart->setPosition(DP(0)*sGlobal->scaleX, -DP(130)*sGlobal->scaleY);
    btn_quit->setPosition(DP(165)*sGlobal->scaleX, -DP(130)*sGlobal->scaleY);
}

void OptionLayer::onBGMClicked(wyTargetSelector* ts) {
    sGlobal->playEffect(se_button);

    if (sGlobal->isBGMOn) {
        sGlobal->stopBGM();
        sGlobal->isBGMOn = false;
        wyPrefs::setBool(DB_IS_BGM_ON, sGlobal->isBGMOn);
        
        bgm_disable->setVisible(true);
    } else {
        sGlobal->isBGMOn = true;
        int prevId = sGlobal->currentBGMId;
        sGlobal->currentBGMId = -1;
        sGlobal->playBGM(prevId);
        wyPrefs::setBool(DB_IS_BGM_ON, sGlobal->isBGMOn);
        
        bgm_disable->setVisible(false);
    }
}

void OptionLayer::onSEClicked(wyTargetSelector* ts) {
    if (sGlobal->isSFXOn) {
        sGlobal->isSFXOn = false;
        wyPrefs::setBool(DB_IS_SFX_ON, sGlobal->isSFXOn);
        
        se_disable->setVisible(true);
    } else {
        sGlobal->isSFXOn = true;
        wyPrefs::setBool(DB_IS_SFX_ON, sGlobal->isSFXOn);
        
        se_disable->setVisible(false);
    }
    
    sGlobal->playEffect(se_button);
}

void OptionLayer::onResumeClicked(wyTargetSelector* ts) {
    btn_resume->setEnabled(false);
    btn_restart->setEnabled(false);
    btn_quit->setEnabled(false);
    menu1->setEnabled(false);
    this->getParent()->removeChildLocked(this, true);
    sGlobal->mainGameLayer->onEnter();
    sGlobal->effectLayer->onEnter();
    sGlobal->hudLayer->onEnter();
    ((MainScene *)sGlobal->mainGameLayer->getParent())->sceneState = SC_NORMAL;
    
    sGlobal->needPauseGame = true;
}

void OptionLayer::onQuitClicked(wyTargetSelector* ts) {
    btn_resume->setEnabled(false);
    btn_restart->setEnabled(false);
    btn_quit->setEnabled(false);
    
    sGlobal->saveDataInGame();
    
    bool flag = sGlobal->isRealEndless;
    
//    sGlobal->isRealEndless = false;
    sGlobal->isBoostOn = false;
    sGlobal->boostMulti = 1.0f;
    sGlobal->heroVelocityX = HERO_VEL_X;
    
    sGlobal->stopBGM();
    if (sGlobal->isRealEndless) {
        clearOneRunMissionStatus();
        sGlobal->gotoCGScene();
    } else {
        if (sGlobal->index == 0) {
            if (sGlobal->isTrainingEnd == 0) {
                sGlobal->isTrainingEnd = 1;
                MyPrefs::setInt(DB_IS_TRAINING_END, 1);
            }
            
            sGlobal->gotoCGScene();
        } else {
            sGlobal->gotoWorldMapScene(true);
        }
    }
    
    sGlobal->needPauseGame = false;
}

void OptionLayer::onRestartClicked(wyTargetSelector* ts) {
    btn_resume->setEnabled(false);
    btn_restart->setEnabled(false);
    btn_quit->setEnabled(false);
    
    sGlobal->saveDataInGame();
    if (sGlobal->isRealEndless) {
        clearOneRunMissionStatus();
    }

    sGlobal->stopBGM();
    ((MainScene *)sGlobal->mainGameLayer->getParent())->reset(false);
}

void OptionLayer::onEnter() {
    wyLayer::onEnter();

//    container->runAction(wyFadeTo::make(0.1f, 255, 20, true));
//    bgLayer->runAction(wyFadeTo::make(0.1f, 255, 20, true));
}

void OptionLayer::createDataPanel() {
    float offsetX = 0;//wyDevice::winWidth;
    
    option_gold = wyBitmapFontLabel::make(sGlobal->font_desc, "coin");
    option_gold->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    option_gold->setAlignment(wyBitmapFontLabel::LEFT);
    option_gold->setAnchor(0, 0.5f);
    container->addChildLocked(option_gold);
    
    number_gold = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    number_gold->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    number_gold->setAlignment(wyBitmapFontLabel::LEFT);
    number_gold->setAnchor(0, 0.5f);
    container->addChildLocked(number_gold);
    
    icon_gold = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
    icon_gold->setScaleX(sGlobal->scaleX*0.7f);
    icon_gold->setScaleY(sGlobal->scaleY*0.7f);
    icon_gold->setAnchor(0.0f, 0.5f);
    container->addChildLocked(icon_gold);
    
    option_kills = wyBitmapFontLabel::make(sGlobal->font_desc, "kills");
    option_kills->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    option_kills->setAlignment(wyBitmapFontLabel::LEFT);
    option_kills->setAnchor(0, 0.5f);
    container->addChildLocked(option_kills);
    
    number_kills = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    number_kills->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    number_kills->setAlignment(wyBitmapFontLabel::LEFT);
    number_kills->setAnchor(0, 0.5f);
    container->addChildLocked(number_kills);
    
    option_remain_hp = wyBitmapFontLabel::make(sGlobal->font_desc, "hp");
    option_remain_hp->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    option_remain_hp->setAlignment(wyBitmapFontLabel::LEFT);
    option_remain_hp->setAnchor(0, 0.5f);
    container->addChildLocked(option_remain_hp);
    
    number_remain_hp = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    number_remain_hp->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    number_remain_hp->setAlignment(wyBitmapFontLabel::LEFT);
    number_remain_hp->setAnchor(0, 0.5f);
    container->addChildLocked(number_remain_hp);
    
    option_totalgold = wyBitmapFontLabel::make(sGlobal->font_desc, "total");
    option_totalgold->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    option_totalgold->setAlignment(wyBitmapFontLabel::LEFT);
    option_totalgold->setAnchor(0, 0.5f);
    container->addChildLocked(option_totalgold);
    
    number_totalgold = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    number_totalgold->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    number_totalgold->setAlignment(wyBitmapFontLabel::LEFT);
    number_totalgold->setAnchor(0, 0.5f);
    container->addChildLocked(number_totalgold);
    
    icon_totalgold = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
    icon_totalgold->setScaleX(sGlobal->scaleX*0.7f);
    icon_totalgold->setScaleY(sGlobal->scaleY*0.7f);
    icon_totalgold->setAnchor(0.0f, 0.5f);
    container->addChildLocked(icon_totalgold);
    
    float ofy = DP(26)*sGlobal->scaleY;
    float startY = DP(75)*sGlobal->scaleY;
    
    calcAllForNormal();
    
    option_gold->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY);
    number_gold->setPosition(-DP(60)*sGlobal->scaleX + offsetX, startY);
    icon_gold->setPosition(number_gold->getPositionX()+number_gold->getWidth()*number_gold->getScaleX()+DP(6)*sGlobal->scaleX, number_gold->getPositionY());
    
    option_kills->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY - ofy);
    number_kills->setPosition(-DP(60)*sGlobal->scaleX + offsetX, startY - ofy);
    
    option_remain_hp->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY - ofy*2);
    number_remain_hp->setPosition(-DP(60)*sGlobal->scaleX + offsetX, startY - ofy*2);
    
    option_totalgold->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY - ofy*4.5);
    number_totalgold->setPosition(-DP(60)*sGlobal->scaleX + offsetX, startY - ofy*4.5);
    icon_totalgold->setPosition(number_totalgold->getPositionX()+number_totalgold->getWidth()*number_totalgold->getScaleX()+DP(6)*sGlobal->scaleX, number_totalgold->getPositionY());
    
}

void OptionLayer::calcAllForNormal() {
    char temp[50];
//    int totalgold = sGlobal->total_gold * (1+sGlobal->skillMulti[SKILL_GREED]);
//    if (sGlobal->isDoubleCoin) {
//        totalgold *= 2;
//    }
    sprintf(temp, "%d / %d", sGlobal->level_gold_normal, sGlobal->total_gold);
    number_gold->setText(temp);
    
    sprintf(temp, "%d / %d", sGlobal->level_kills, sGlobal->total_enemies);
    number_kills->setText(temp);
    
    int hp_remain = (MAX((float)sGlobal->curHP,0) / sGlobal->cloth_hp * 100);
    sprintf(temp, "%d%%", hp_remain);
    number_remain_hp->setText(temp);
    
    sprintf(temp, "%d", sGlobal->level_total_gold);
    number_totalgold->setText(temp);
}

void OptionLayer::calcAllForEndless() {
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
    int totalScore = (int)score;
    sprintf(temp, "%d", totalScore);
    number_score->setText(temp);
}

void OptionLayer::clearOneRunMissionStatus() {
    sGlobal->achv_progress_temp[sGlobal->ACHV_SEQ[sGlobal->missionId1]] = 0;
    sGlobal->achv_progress_temp[sGlobal->ACHV_SEQ[sGlobal->missionId2]] = 0;
    sGlobal->achv_progress_temp[sGlobal->ACHV_SEQ[sGlobal->missionId3]] = 0;
}




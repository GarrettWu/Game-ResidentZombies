//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameCompleteLayer.h"
#include "Global.h"
#include "MainScene.h"
#include "LoadingScene.h"
#include "WorldMapScene.h"
#include "DialogBgLayer.h"
#include "VungleVedioController.h"

GameCompleteLayer::GameCompleteLayer(bool needForceNext, int tabIndex) : wyColorLayer(wyc4b(0, 0, 0, 100)) {
    MyPrefs::setInt(DB_GOLD, sGlobal->getGold());
    MyPrefs::setInt(DB_ZUANSHI_NUM, sGlobal->zuanshi.getValue());

    sGlobal->needPauseGame = false;

    this->needForceNext = needForceNext;
    
    container = wyNode::make();
    container->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    addChildLocked(container);
    
    DialogBgLayer *bgLayer = DialogBgLayer::make();
    container->addChildLocked(bgLayer);

    title = wyBitmapFontLabel::make(sGlobal->font_title, "a");
    title->setScale(SP(45)*sGlobal->scaleX/sGlobal->titleFontSize);
    title->setAlignment(wyBitmapFontLabel::LEFT);
    title->setAnchor(0.5f, 0);
    title->setPosition(-DP(140)*sGlobal->scaleX, DP(110)*sGlobal->scaleY);
    container->addChildLocked(title);
    
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

    option_total = wyBitmapFontLabel::make(sGlobal->font_desc, "total");
    option_total->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    option_total->setAlignment(wyBitmapFontLabel::LEFT);
    option_total->setAnchor(0, 0.5f);
    container->addChildLocked(option_total);
    
    number_total = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    number_total->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    number_total->setAlignment(wyBitmapFontLabel::LEFT);
    number_total->setAnchor(0, 0.5f);
    container->addChildLocked(number_total);

    option_score = wyBitmapFontLabel::make(sGlobal->font_desc, "score");
    option_score->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    option_score->setAlignment(wyBitmapFontLabel::LEFT);
    option_score->setAnchor(0, 0.5f);
    container->addChildLocked(option_score);
    
    number_score = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    number_score->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    number_score->setAlignment(wyBitmapFontLabel::LEFT);
    number_score->setAnchor(0, 0.5f);
    container->addChildLocked(number_score);
    
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
    
    option_cash = wyBitmapFontLabel::make(sGlobal->font_desc, "cash");
    option_cash->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    option_cash->setAlignment(wyBitmapFontLabel::LEFT);
    option_cash->setAnchor(0, 0.5f);
    container->addChildLocked(option_cash);
    
    number_cash = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    number_cash->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    number_cash->setAlignment(wyBitmapFontLabel::LEFT);
    number_cash->setAnchor(0, 0.5f);
    container->addChildLocked(number_cash);
    
    icon_cash = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cash_icon.png");
    icon_cash->setScaleX(sGlobal->scaleX*0.7f);
    icon_cash->setScaleY(sGlobal->scaleY*0.7f);
    icon_cash->setAnchor(0.0f, 0.5f);
    container->addChildLocked(icon_cash);
    
    float ofy = DP(26)*sGlobal->scaleY;
    float startY = DP(85)*sGlobal->scaleY;
    float offsetX = wyDevice::winWidth;

    option_gold->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY);
    number_gold->setPosition(-DP(60)*sGlobal->scaleX + offsetX, startY);
    
    option_kills->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY - ofy);
    number_kills->setPosition(-DP(60)*sGlobal->scaleX + offsetX, startY - ofy);
    
    option_total->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY - ofy*2);
    number_total->setPosition(-DP(60)*sGlobal->scaleX + offsetX, startY - ofy*2);
    option_total->setVisible(false);
    number_total->setVisible(false);
    
    option_remain_hp->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY - ofy*2);
    number_remain_hp->setPosition(-DP(60)*sGlobal->scaleX + offsetX, startY - ofy*2);
    
    option_score->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY - ofy*3);
    number_score->setPosition(-DP(60)*sGlobal->scaleX + offsetX, startY - ofy*3);
    
    option_totalgold->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY - ofy*4.5f);
    number_totalgold->setPosition(-DP(60)*sGlobal->scaleX + offsetX, startY - ofy*4.5f);
    
    option_cash->setPosition(-DP(160)*sGlobal->scaleX + offsetX, startY - ofy*5.5f);
    number_cash->setPosition(-DP(60)*sGlobal->scaleX + offsetX, startY - ofy*5.5f);
    
    
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    wyBitmapFontLabel *title = wyBitmapFontLabel::make(sGlobal->font_desc, "restart");
    title->setScale(SP(22)/sGlobal->descFontSize);
    btn_restart = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(GameCompleteLayer::onRestartClicked)), mask, title);
    btn_restart->setScaleX(sGlobal->scaleX*0.8f);
    btn_restart->setScaleY(sGlobal->scaleY*0.8f);
    btn_restart->setMultiTouchClickable(false);
    btn_restart->setInterceptTouch(true);
    btn_restart->setTouchPriority(1000);
    container->addChildLocked(btn_restart, 100);
    btn_restart->setPosition(DP(0)*sGlobal->scaleX, -DP(115)*sGlobal->scaleY);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "quit");
    title->setScale(SP(22)/sGlobal->descFontSize);
    btn_quit = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(GameCompleteLayer::onQuitClicked)), mask, title);
    btn_quit->setScaleX(sGlobal->scaleX*0.8f);
    btn_quit->setScaleY(sGlobal->scaleY*0.8f);
    btn_quit->setMultiTouchClickable(false);
    btn_quit->setInterceptTouch(true);
    btn_quit->setTouchPriority(1000);
    container->addChildLocked(btn_quit, 100);
    btn_quit->setPosition(-DP(165)*sGlobal->scaleX, -DP(115)*sGlobal->scaleY);
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "next");
    title->setScale(SP(22)/sGlobal->descFontSize);
    btn_next = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(GameCompleteLayer::onNextClicked)), mask, title);
    btn_next->setScaleX(sGlobal->scaleX*0.8f);
    btn_next->setScaleY(sGlobal->scaleY*0.8f);
    btn_next->setMultiTouchClickable(false);
    btn_next->setInterceptTouch(true);
    btn_next->setTouchPriority(1000);
    container->addChildLocked(btn_next, 100);
    btn_next->setPosition(DP(165)*sGlobal->scaleX, -DP(115)*sGlobal->scaleY);
    
    btn_restart->setEnabled(false);
    btn_quit->setEnabled(false);
    btn_next->setEnabled(false);
    
    calcAll();
    
    icon_gold->setPosition(number_gold->getPositionX()+number_gold->getWidth()*number_gold->getScaleX()+DP(6)*sGlobal->scaleX, number_gold->getPositionY());
    icon_totalgold->setPosition(number_totalgold->getPositionX()+number_totalgold->getWidth()*number_totalgold->getScaleX()+DP(6)*sGlobal->scaleX, number_totalgold->getPositionY());
    icon_cash->setPosition(number_cash->getPositionX()+number_cash->getWidth()*number_cash->getScaleX()+DP(6)*sGlobal->scaleX, number_cash->getPositionY());
    
    option_gold->setVisible(false);
    number_gold->setVisible(false);
    option_kills->setVisible(false);
    number_kills->setVisible(false);
    option_remain_hp->setVisible(false);
    number_remain_hp->setVisible(false);
    option_total->setVisible(false);
    number_total->setVisible(false);
    option_score->setVisible(false);
    number_score->setVisible(false);
    option_cash->setVisible(false);
    number_cash->setVisible(false);
    option_totalgold->setVisible(false);
    number_totalgold->setVisible(false);
    icon_gold->setVisible(false);
    icon_totalgold->setVisible(false);
    icon_cash->setVisible(false);
    
    this->setTouchEnabled(true);
    
    updatePrefs();
    
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
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "wenhao.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "wenhao.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    btn_wenhao = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(GameCompleteLayer::onWenhaoClicked)), mask, NULL);
    btn_wenhao->setScaleX(sGlobal->scaleX*0.6f);
    btn_wenhao->setScaleY(sGlobal->scaleY*0.6f);
    btn_wenhao->setMultiTouchClickable(false);
    btn_wenhao->setInterceptTouch(true);
    btn_wenhao->setTouchPriority(1000);
    container->addChildLocked(btn_wenhao, 100);
    btn_wenhao->setPosition(DP(165)*sGlobal->scaleX, -DP(120)*sGlobal->scaleY);
    btn_wenhao->needDelay = false;
    
    goldMultiplyer->setPosition(DP(40)*sGlobal->scaleX, DP(135)*sGlobal->scaleY);
    btn_wenhao->setPosition(DP(230)*sGlobal->scaleX, DP(135)*sGlobal->scaleY);
}

GameCompleteLayer::~GameCompleteLayer() {
    sGlobal->needPauseGame = false;
#if ANDROID
    hideAd();
#endif
#if IOS
    ObjCCalls::getInstance()->hideAd();
#endif
}

void GameCompleteLayer::onEnterTransitionDidFinish() {
    wyLayer::onEnterTransitionDidFinish();
}

void GameCompleteLayer::calcAll() {
    int seasonId = sGlobal->index / 100;
    int mapId = sGlobal->index % 100;
    int scoreIndex = mapId+sGlobal->currentSModeDifficulty*10;

    char temp[20] = {0};
    if (sGlobal->index == 0) {
        sprintf(temp, "%s", sGlobal->getStr(STR_TUTORIAL_TITLE));
    } else {
        sprintf(temp, sGlobal->getStr(STR_LEVEL_TITLE), seasonId, mapId);
    }
    title->setText(temp);
    
//    int totalgold = sGlobal->total_gold * (1+sGlobal->skillMulti[SKILL_GREED]);
//    if (sGlobal->isDoubleCoin) {
//        totalgold *= 2;
//    }
    sprintf(temp, "%d / %d", sGlobal->level_gold_normal, sGlobal->total_gold);
//    addComma(sGlobal->level_gold, temp);
    number_gold->setText(temp);
    
    sprintf(temp, "%d", sGlobal->level_total_gold);
    number_totalgold->setText(temp);
    
    sprintf(temp, "%d / %d", sGlobal->level_kills, sGlobal->total_enemies);
    number_kills->setText(temp);
    
    int hp_remain = ((float)sGlobal->curHP / sGlobal->cloth_hp * 100);
    sprintf(temp, "%d%%", hp_remain);
    number_remain_hp->setText(temp);
    
    if (sGlobal->total_enemies == 0) {
        sGlobal->total_enemies = 1;
        sGlobal->level_kills = 1;
    }
    
    if (sGlobal->total_gold == 0) {
        sGlobal->total_gold = 1;
        sGlobal->level_gold = 1;
        sGlobal->level_gold_normal = 1;
    }
    
    float hpPercent = (float)sGlobal->curHP / sGlobal->cloth_hp;
    totalScore = (sGlobal->level_gold + sGlobal->level_enemy_gold)*(1+hpPercent)*2.71828183f*10;
//    LOGE("level_gold:%d, enemy_gold:%d, hpGold:%d", sGlobal->level_gold, sGlobal->level_enemy_gold, (int)(((float)sGlobal->curHP / sGlobal->cloth_hp)*200));
    
    float gradeScore = 1/3.0f * sGlobal->level_gold_normal / sGlobal->total_gold + 1/3.0f * sGlobal->level_kills / sGlobal->total_enemies + 1/3.0f * sGlobal->curHP / sGlobal->cloth_hp;
    
    const char *reportGrade;
    if (gradeScore >= 0.95f) {
//        grade = wySpriteEx::make(MyTextureManager::makePNG("score_s")));
        grade = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "score_s.png");
        totalScore += 15000;
        gradeLevel = GRADE_S;
        reportGrade = "s";
    }
    else if (gradeScore >= 0.85f && gradeScore < 0.95f) {
//        grade = wySpriteEx::make(MyTextureManager::makePNG("score_a")));
        grade = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "score_a.png");
        totalScore += 10000;
        gradeLevel = GRADE_A;
        reportGrade = "a";
    }
    else if (gradeScore >= 0.7f && gradeScore < 0.85f) {
//        grade = wySpriteEx::make(MyTextureManager::makePNG("score_b")));
        grade = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "score_b.png");
        totalScore += 5000;
        gradeLevel = GRADE_B;
        reportGrade = "b";
    }
    else if (gradeScore < 0.7f) {
//        grade = wySpriteEx::make(MyTextureManager::makePNG("score_c")));
        grade = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "score_c.png");
        gradeLevel = GRADE_C;
        reportGrade = "c";
    }
    grade->setScaleX(sGlobal->scaleX*3.6f);
    grade->setScaleY(sGlobal->scaleY*3.6f);
//    if (gradeLevel == GRADE_B) {
//        grade->setPosition(wyDevice::winWidth/2 + DP(220)*sGlobal->scaleX, DP(150)*sGlobal->scaleY);
//    } else {
        grade->setPosition(wyDevice::winWidth/2 + DP(110)*sGlobal->scaleX, DP(220)*sGlobal->scaleY);
//    }
    addChildLocked(grade, 10);
    grade->setVisible(false);

    sprintf(temp, "%d", 0);
    number_score->setText(temp);
    
    int prevGrade;
    switch (seasonId) {
        case 1:
            prevGrade = sGlobal->s01_grade[scoreIndex];
            if (sGlobal->s01_grade[scoreIndex] < gradeLevel) {
                sGlobal->s01_grade[scoreIndex] = gradeLevel;
            }
            if (sGlobal->s01_score[scoreIndex] < totalScore) {
                sGlobal->s01_score[scoreIndex] = totalScore;
            }
            
            sGlobal->arrayToStr(DB_S01_GRADE, sGlobal->s01_grade, ARR_LEN_LEVEL_NUM);
            sGlobal->arrayToStr(DB_S01_SCORE, sGlobal->s01_score, ARR_LEN_LEVEL_NUM);
            break;
        case 2:
            prevGrade = sGlobal->s02_grade[scoreIndex];
            if (sGlobal->s02_grade[scoreIndex] < gradeLevel) {
                sGlobal->s02_grade[scoreIndex] = gradeLevel;
            }
            if (sGlobal->s02_score[scoreIndex] < totalScore) {
                sGlobal->s02_score[scoreIndex] = totalScore;
            }
            
            sGlobal->arrayToStr(DB_S02_GRADE, sGlobal->s02_grade, ARR_LEN_LEVEL_NUM);
            sGlobal->arrayToStr(DB_S02_SCORE, sGlobal->s02_score, ARR_LEN_LEVEL_NUM);
            break;
        case 3:
            prevGrade = sGlobal->s03_grade[scoreIndex];
            if (sGlobal->s03_grade[scoreIndex] < gradeLevel) {
                sGlobal->s03_grade[scoreIndex] = gradeLevel;
            }
            if (sGlobal->s03_score[scoreIndex] < totalScore) {
                sGlobal->s03_score[scoreIndex] = totalScore;
            }

            sGlobal->arrayToStr(DB_S03_GRADE, sGlobal->s03_grade, ARR_LEN_LEVEL_NUM);
            sGlobal->arrayToStr(DB_S03_SCORE, sGlobal->s03_score, ARR_LEN_LEVEL_NUM);
            break;
        default:
            break;
    }
    
    int reportMapIndex = sGlobal->index;
    int nextLevel = sGlobal->index;
    if (sGlobal->index == 0) {
        nextLevel = 101;
    } else {
        if (sGlobal->index % 100 >= 10) {
//            if (sGlobal->index / 100 == 3) {
//                nextLevel = sGlobal->index;
//            } else {
//                nextLevel = (sGlobal->index / 100 + 1) * 100 + 1;
//            }
            nextLevel = sGlobal->index;
        } else {
            nextLevel += 1;
        }
        
        if (nextLevel > sGlobal->currentProgress[sGlobal->currentSModeDifficulty]) {
            sGlobal->currentProgress[sGlobal->currentSModeDifficulty] = nextLevel;
            
#if ANDROID
            sGlobal->reportGameEvent(EVT_GAME_PROGRESS, nextLevel, 0.0f, DIFFICULTY_STR[sGlobal->currentSModeDifficulty], NULL);
#endif
        }
        sGlobal->arrayToStr(DB_PROGRESS, sGlobal->currentProgress, 3);
        
        //add cash...
//        LOGE("prevGrade:%d", prevGrade);
        int cash = 0;
        if (gradeLevel == GRADE_S) {
            if (prevGrade == GRADE_NONE) {
                cash = 2;
            }
            else if (prevGrade == GRADE_S) {
                cash = 0;
            }
            else {
                cash = 1;
            }
        } else {
            if (prevGrade == GRADE_NONE) {
                cash = 1;
            }
            else if (prevGrade == GRADE_S) {
                cash = 0;
            }
            else {
                cash = 0;
            }
        }
        sprintf(temp, "%d", cash);
        number_cash->setText(temp);
        
        sGlobal->zuanshi.offset(cash);
        MyPrefs::setInt(DB_ZUANSHI_NUM, sGlobal->zuanshi.getValue());
    }

    sGlobal->saveDataInGame();
    
#if IOS
    ObjCCalls::getInstance()->reportScore(sGlobal->getTotalScore());
#endif
    
#if ANDROID
    sGlobal->reportGameEvent(EVT_GAME_COMPLETE, reportMapIndex, 0.0f, DIFFICULTY_STR[sGlobal->currentSModeDifficulty], reportGrade);
#endif

}

void GameCompleteLayer::updatePrefs() {

}

bool GameCompleteLayer::touchesBegan(wyMotionEvent& event) {

    return true;
}

void GameCompleteLayer::onNextClicked(wyTargetSelector* ts) {
    btn_quit->setEnabled(false);
    btn_restart->setEnabled(false);
    btn_next->setEnabled(false);

    if (sGlobal->index == 0) {
        sGlobal->index = 101;
        
        sGlobal->currentSModeDifficulty = DIFFICULTY_NORMAL;
    } else {
        if (sGlobal->index % 100 >= 10) {
            if (sGlobal->index / 100 == 3) {
                sGlobal->stopBGM();
                sGlobal->gotoCGScene(true);
            } else {
                if (sGlobal->currentProgress[sGlobal->currentSModeDifficulty] > sGlobal->index) {
                    sGlobal->index = (sGlobal->index / 100 + 1) * 100 + 1;
                } else {
                    sGlobal->stopBGM();
                    sGlobal->gotoWorldMapScene(true);
                    return;
                }
            }
        } else {
            sGlobal->index += 1;
        }
    }

    wyDirector* director = wyDirector::getInstance();
    wyScene *scene = new LoadingScene(SCENE_UPGRADE, sGlobal->index);
    scene->autoRelease();
    director->replaceScene(scene);
}

void GameCompleteLayer::onQuitClicked(wyTargetSelector* ts) {
    btn_quit->setEnabled(false);
    btn_restart->setEnabled(false);
    btn_next->setEnabled(false);

    sGlobal->stopBGM();
    sGlobal->gotoWorldMapScene(true);
    
    sGlobal->needPauseGame = false;
}

void GameCompleteLayer::onRestartClicked(wyTargetSelector* ts) {
    btn_quit->setEnabled(false);
    btn_restart->setEnabled(false);
    btn_next->setEnabled(false);
    
    ((MainScene *)sGlobal->mainGameLayer->getParent())->reset(false);
}

static void onAnimationEnded1(wyAction* action, void* data) {
    GameCompleteLayer* layer = (GameCompleteLayer*)data;
    layer->gradeShake();
}

static void onAnimationEnded2(wyAction* action, void* data) {
    GameCompleteLayer* layer = (GameCompleteLayer*)data;
    layer->startAnimation();
    sGlobal->playEffect(se_menu);
}

void GameCompleteLayer::onEnter() {
    wyLayer::onEnter();

//#if ANDROID
//    if (sGlobal->index == 104 && wyPrefs::getInt("IS_VUNGLE_PLAYED", 0) == 0 && !sGlobal->isAdsRemoved) {
//        playVungleVedio();
//    }
//#endif
    VungleVedioController::playVedioWhenLevelComplete();
    startAnimation();
}

static void onCashEnterEnd(wyAction* action, void* data) {
    GameCompleteLayer* layer = (GameCompleteLayer*)data;
    
    wyQuadParticleSystem *emitter = wyParticleLoader::load("particle_star.plist");
    emitter->setPosition(layer->number_cash->getPositionX()+layer->container->getPositionX(), layer->number_cash->getPositionY()+layer->container->getPositionY());
    layer->addChildLocked(emitter, 9);
    emitter->setScaleX(sGlobal->scaleX*0.4f);
    emitter->setScaleY(sGlobal->scaleY*0.4f);
    emitter->setAutoRemoveOnFinish(true);
}

void GameCompleteLayer::gradeShake() {
    wyIntervalAction* a = wyShake::make(0.2f, 5);
//    wyActionCallback callback = {
//        NULL,
//        onAnimationEnded2
//    };
//    a->setCallback(&callback, this);
    grade->runAction(a);
    wyQuadParticleSystem *emitter = wyParticleLoader::load("particle_star.plist");
    emitter->setPosition(grade->getPositionX(), grade->getPositionY());
    addChildLocked(emitter, 9);
    emitter->setScaleX(sGlobal->scaleX/0.75);
    emitter->setScaleY(sGlobal->scaleY/0.75);
    emitter->setAutoRemoveOnFinish(true);
    
    sGlobal->playEffect(se_menu);
    
    float duration = 0.2f;
    float interval = 0.1f;
	float offsetX = wyDevice::winWidth;
    wyIntervalAction* a2 = wyMoveTo::make(duration, option_cash->getPositionX(), option_cash->getPositionY(), option_cash->getPositionX()-offsetX, option_cash->getPositionY());
    wyIntervalAction*t = wySequence::make(wyDelayTime::make(0.5f), wyShow::make(), a2, NULL);
    option_cash->runAction(t);
    a2 = wyMoveTo::make(duration, number_cash->getPositionX(), number_cash->getPositionY(), number_cash->getPositionX()-offsetX, number_cash->getPositionY());
    t = wySequence::make(wyDelayTime::make(0.5f),wyShow::make(), a2, NULL);
    wyActionCallback callback1 = {
        NULL,
        onCashEnterEnd,
        NULL
    };
    t->setCallback(&callback1, this);
    number_cash->runAction(t);
    a2 = wyMoveTo::make(duration, icon_cash->getPositionX(), icon_cash->getPositionY(), icon_cash->getPositionX()-offsetX, icon_cash->getPositionY());
    t = wySequence::make(wyDelayTime::make(0.5f),wyShow::make(), a2, NULL);
    icon_cash->runAction(t);
}

static void onAnimationEnd(wyAction* action, void* data) {
    GameCompleteLayer* layer = (GameCompleteLayer*)data;
    layer->btn_quit->setEnabled(true);
    layer->btn_restart->setEnabled(true);
    layer->btn_next->setEnabled(true);
    
    layer->countDownScore();
}

void GameCompleteLayer::countDownScore() {
    float dur1 = 0.2f;
    float dur2 = 0.5f;
    totalDur = dur1 + dur2*3;
    this->runAction(wySequence::make(wyDelayTime::make(dur1), wyCallFunc::make(wyTargetSelector::make(this, SEL(GameCompleteLayer::changeColor0))), NULL));
    this->runAction(wySequence::make(wyDelayTime::make(dur1 + dur2*1), wyCallFunc::make(wyTargetSelector::make(this, SEL(GameCompleteLayer::changeColor1))), NULL));
    this->runAction(wySequence::make(wyDelayTime::make(dur1 + dur2*2), wyCallFunc::make(wyTargetSelector::make(this, SEL(GameCompleteLayer::changeColor2))), NULL));
    this->runAction(wySequence::make(wyDelayTime::make(totalDur), wyCallFunc::make(wyTargetSelector::make(this, SEL(GameCompleteLayer::changeColor3))), NULL));
}

void GameCompleteLayer::updateScoreTimer(wyTargetSelector *ts) {
    scoreTimer += ts->getDelta();
    
    if (scoreTimer > 0.02f) {
        curScore += totalScore/totalDur * scoreTimer;
        curScore = MIN(totalScore, curScore);
        char temp[20];
        sprintf(temp, "%d", curScore);
        number_score->setText(temp);
        
        scoreTimer = 0;
        
        if (curScore == totalScore) {
            unscheduleLocked(stimer);
            stimer = NULL;
            
            wyIntervalAction *a1 = wyScaleTo::make(0.5f, sGlobal->scaleX*8.0f, sGlobal->scaleY*8.0f, sGlobal->scaleX*1.0f, sGlobal->scaleY*1.0f);
            wyIntervalAction *a2 = wyFadeTo::make(0.5f, 30, 255);
            wyIntervalAction *a11 = (wyEaseBackOut*)(new wyEaseOut(0.2f, a1))->autoRelease();
            wyIntervalAction *a21 = (wyEaseBackOut*)(new wyEaseOut(0.2f, a2))->autoRelease();
            wyActionCallback callback = {
                NULL,
                onAnimationEnded1
            };
            a21->setCallback(&callback, this);
            grade->runAction(a11);
            grade->runAction(a21);
            grade->setVisible(true);
        }
    }
    
}

void GameCompleteLayer::changeColor0(wyTargetSelector *ts) {
    wyColor3B color = wyc3b(255, 85, 24);
    option_gold->setColor(color);
    number_gold->setColor(color);
    
    wyTargetSelector* tss = wyTargetSelector::make(this, SEL(GameCompleteLayer::updateScoreTimer));
    stimer = wyTimer::make(tss);
    scheduleLocked(stimer);
    
    scoreTimer = 0;
    curScore = 0;
}

void GameCompleteLayer::changeColor1(wyTargetSelector *ts) {
    wyColor3B colorW = wyc3b(255, 255, 255);
    option_gold->setColor(colorW);
    number_gold->setColor(colorW);
    
    wyColor3B color = wyc3b(255, 85, 24);
    option_kills->setColor(color);
    number_kills->setColor(color);
}

void GameCompleteLayer::changeColor2(wyTargetSelector *ts) {
    wyColor3B colorW = wyc3b(255, 255, 255);
    wyColor3B color = wyc3b(255, 85, 24);
    
    option_remain_hp->setColor(color);
    number_remain_hp->setColor(color);
    
    option_kills->setColor(colorW);
    number_kills->setColor(colorW);
}

void GameCompleteLayer::changeColor3(wyTargetSelector *ts) {
    wyColor3B colorW = wyc3b(255, 255, 255);
    
    option_remain_hp->setColor(colorW);
    number_remain_hp->setColor(colorW);
}

void GameCompleteLayer::startAnimation() {
    float duration = 0.2f;
    float interval = 0.1f;
	float offsetX = wyDevice::winWidth;
    wyIntervalAction* a2 = wyMoveTo::make(duration, option_gold->getPositionX(), option_gold->getPositionY(), option_gold->getPositionX()-offsetX, option_gold->getPositionY());
    wyIntervalAction* t = wySequence::make(wyShow::make(), a2, NULL);
    option_gold->runAction(t);
    a2 = wyMoveTo::make(duration, number_gold->getPositionX(), number_gold->getPositionY(), number_gold->getPositionX()-offsetX, number_gold->getPositionY());
    t = wySequence::make(wyShow::make(), a2, NULL);
    number_gold->runAction(t);
    a2 = wyMoveTo::make(duration, icon_gold->getPositionX(), icon_gold->getPositionY(), icon_gold->getPositionX()-offsetX, icon_gold->getPositionY());
    t = wySequence::make(wyShow::make(), a2, NULL);
    icon_gold->runAction(t);
    
    a2 = wyMoveTo::make(duration, option_kills->getPositionX(), option_kills->getPositionY(), option_kills->getPositionX()-offsetX, option_kills->getPositionY());
    t = wySequence::make(wyDelayTime::make(interval), wyShow::make(), a2, NULL);
    option_kills->runAction(t);
    a2 = wyMoveTo::make(duration, number_kills->getPositionX(), number_kills->getPositionY(), number_kills->getPositionX()-offsetX, number_kills->getPositionY());
    t = wySequence::make(wyDelayTime::make(interval), wyShow::make(), a2, NULL);
    number_kills->runAction(t);
    
    a2 = wyMoveTo::make(duration, option_remain_hp->getPositionX(), option_remain_hp->getPositionY(), option_remain_hp->getPositionX()-offsetX, option_remain_hp->getPositionY());
    t = wySequence::make(wyDelayTime::make(interval*3), wyShow::make(), a2, NULL);
    option_remain_hp->runAction(t);
    a2 = wyMoveTo::make(duration, number_remain_hp->getPositionX(), number_remain_hp->getPositionY(), number_remain_hp->getPositionX()-offsetX, number_remain_hp->getPositionY());
    t = wySequence::make(wyDelayTime::make(interval*3), wyShow::make(), a2, NULL);
    number_remain_hp->runAction(t);
    
    a2 = wyMoveTo::make(duration, option_score->getPositionX(), option_score->getPositionY(), option_score->getPositionX()-offsetX, option_score->getPositionY());
    t = wySequence::make(wyDelayTime::make(interval*4), wyShow::make(), a2, NULL);
    option_score->runAction(t);
    a2 = wyMoveTo::make(duration, number_score->getPositionX(), number_score->getPositionY(), number_score->getPositionX()-offsetX, number_score->getPositionY());
    t = wySequence::make(wyDelayTime::make(interval*4), wyShow::make(), a2, NULL);
    number_score->runAction(t);
    
    a2 = wyMoveTo::make(duration, option_totalgold->getPositionX(), option_totalgold->getPositionY(), option_totalgold->getPositionX()-offsetX, option_totalgold->getPositionY());
    t = wySequence::make(wyDelayTime::make(interval*5), wyShow::make(), a2, NULL);
    option_totalgold->runAction(t);
    a2 = wyMoveTo::make(duration, number_totalgold->getPositionX(), number_totalgold->getPositionY(), number_totalgold->getPositionX()-offsetX, number_totalgold->getPositionY());
    t = wySequence::make(wyDelayTime::make(interval*5), wyShow::make(), a2, NULL);
    wyActionCallback callback = {
        NULL,
        onAnimationEnd,
        NULL
    };
    t->setCallback(&callback, this);
    number_totalgold->runAction(t);
    a2 = wyMoveTo::make(duration, icon_totalgold->getPositionX(), icon_totalgold->getPositionY(), icon_totalgold->getPositionX()-offsetX, icon_totalgold->getPositionY());
    t = wySequence::make(wyDelayTime::make(interval*5), wyShow::make(), a2, NULL);
    icon_totalgold->runAction(t);
}


void GameCompleteLayer::onWenhaoClicked(wyTargetSelector *ts) {
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


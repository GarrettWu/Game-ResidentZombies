//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "QuestLayer.h"
#include "Global.h"
#include "MainScene.h"
#include "WorldMapScene.h"

QuestLayer::QuestLayer(bool isDead) : wyColorLayer(wyc4b(0, 0, 0, 0)) {
//    wyBitmapFont* font = wyBitmapFont::loadFont("font_tc_en"), BFF_ANGELCODE_XML);
    
    container = wyNode::make();
    addChildLocked(container);

    for (int i = 0; i < 3; i++) {
        createMission(i);
    }
    for (int i = 3; i < 6; i++) {
        createMission(i, true);
    }
    
    oldm1 = sGlobal->missionId1;
    oldm2 = sGlobal->missionId2;
    oldm3 = sGlobal->missionId3;

    m_isDead = isDead;
    if (isDead) {
        sGlobal->refreshMIds();
    }
    
    curMid = -1;
    curIndex = 0;
    isAnimFinished = false;
    
    this->setTouchEnabled(false);
//    this->setTouchPriority(99999);
//    this->setInterceptTouch(true);
    
//    for (int i = 0; i < ARR_LEN_ACHV_NUM; i++) {
//        LOGE("no:%d, seq:%d, rec:%d, progress:%d, progressold:%d, temp:%d", i, sGlobal->ACHV_SEQ[i], sGlobal->achv_rec[sGlobal->ACHV_SEQ[i]], sGlobal->achv_progress[sGlobal->ACHV_SEQ[i]], sGlobal->achv_progress_old[sGlobal->ACHV_SEQ[i]], sGlobal->achv_progress_temp[sGlobal->ACHV_SEQ[i]]);
//    }
    
    isQuiting = false;
    
    onEndSelector = NULL;
}

QuestLayer::~QuestLayer() {
//    LOGE("quest delete...");
}

static void onDelayEnd(wyAction* action, void* data) {
    QuestLayer* layer = (QuestLayer*)data;
    if (layer->isQuiting)
        return;
    
    layer->startEnterAnimation();
}

static void onEnterAnimationEnd(wyAction* action, void* data) {
    QuestLayer* layer = (QuestLayer*)data;
    if (layer->isQuiting)
        return;
    layer->startQuestAnim(0);
}

static void onProgressEnd(wyAction* action, void* data) {
    QuestLayer* layer = (QuestLayer*)data;
    if (layer->isQuiting)
        return;
    layer->onFinishProgressAnim();
}

static void onProgressUpdate(wyAction* action, float t, void* data) {
    QuestLayer* layer = (QuestLayer*)data;
    if (layer->isQuiting)
        return;
    layer->updateProgressTitle(t);
}

static void onSingleQuestEnd(wyAction* action, void* data) {
    QuestLayer* layer = (QuestLayer*)data;
    if (layer->isQuiting)
        return;
    layer->startQuestAnim(++layer->curIndex);
}

void QuestLayer::onEnter() {
    wyLayer::onEnter();

}

void QuestLayer::placeAllThings() {
    wyNode *mission1 = container->getChildByTag(0);
    mission1->setVisible(true);
    
    wyNode *mission2 = container->getChildByTag(1);
    mission2->setVisible(true);
    
    wyNode *mission3 = container->getChildByTag(2);
    mission3->setVisible(true);

    for (int i = 0; i < 3; i++) {
        switch (i) {
            case 0:
                curMissionId = oldm1;
                nextMissionId = sGlobal->missionId1;
                break;
            case 1:
                curMissionId = oldm2;
                nextMissionId = sGlobal->missionId2;
                break;
            case 2:
                curMissionId = oldm3;
                nextMissionId = sGlobal->missionId3;
                break;
            default:
                break;
        }
        curMid = sGlobal->ACHV_SEQ[curMissionId];
        target = sGlobal->ACHV_NUM[curMid*3];
        from = sGlobal->achv_progress_old[curMid];
        to = sGlobal->achv_progress[curMid];
        
        switch (curMid) {
            case ACHV_KILL_50_ONE:
            case ACHV_KILL_100_ONE:
            case ACHV_KILL_200_ONE:
            case ACHV_KILL_400_ONE:
            case ACHV_KILL_800_ONE:
                
            case ACHV_BOSS_2_ONE:
            case ACHV_BOSS_4_ONE:
            case ACHV_BOSS_8_ONE:
            case ACHV_BOSS_16_ONE:
            case ACHV_BOSS_24_ONE:
                
            case ACHV_RUN_400_ONE:
            case ACHV_RUN_800_ONE:
            case ACHV_RUN_1600_ONE:
            case ACHV_RUN_3200_ONE:
            case ACHV_RUN_4800_ONE:
                
            case ACHV_COIN_400_ONE:
            case ACHV_COIN_800_ONE:
            case ACHV_COIN_1600_ONE:
            case ACHV_COIN_3200_ONE:
            case ACHV_COIN_4800_ONE:
                
                to = sGlobal->achv_progress_temp[curMid];
                break;
                
            default:
                break;
        }

        
//        sGlobal->achv_progress_old[curMid] = sGlobal->achv_progress[curMid];
        
        wyNode *missionContainer = container->getChildByTag(i);
        wyProgressTimer *progress_bg = (wyProgressTimer*)missionContainer->getChildByTag(TAG_PROGRESS_BAR);
        wyBitmapFontLabel *progress = (wyBitmapFontLabel*)missionContainer->getChildByTag(TAG_PROGRESS_TITLE);
        
        progress_bg->setPercentage(to/(float)target * 100);
        char temp[20];
        sprintf(temp, "%d/%d", (int)to, sGlobal->ACHV_NUM[curMid*3]);
        progress->setText(temp);
    }
}

void QuestLayer::startAnimation() {
    if (isQuiting)
        return;
    wyAction *a = wyDelayTime::make(0.6f);
    wyActionCallback callback = {
        NULL,
        onDelayEnd,
        NULL
    };
    a->setCallback(&callback, this);
    this->runAction(a);
}

void QuestLayer::startEnterAnimation() {
    if (isQuiting)
        return;
    
    wyNode *mission1 = container->getChildByTag(0);
    mission1->runAction(wySequence::make(wyPlace::make(mission1->getPositionX()+wyDevice::winWidth, mission1->getPositionY()), wyShow::make(), wyMoveBy::make(0.2f, -wyDevice::winWidth, 0), NULL));
    
    wyNode *mission2 = container->getChildByTag(1);
    mission2->runAction(wySequence::make(wyDelayTime::make(0.1f), wyPlace::make(mission2->getPositionX()+wyDevice::winWidth, mission2->getPositionY()), wyShow::make(), wyMoveBy::make(0.2f, -wyDevice::winWidth, 0), NULL));
    
    wyNode *mission3 = container->getChildByTag(2);
    wyIntervalAction *a = wySequence::make(wyDelayTime::make(0.1f*2), wyPlace::make(mission3->getPositionX()+wyDevice::winWidth, mission3->getPositionY()), wyShow::make(), wyMoveBy::make(0.2f, -wyDevice::winWidth, 0), NULL);
    wyActionCallback callback = {
        NULL,
        onEnterAnimationEnd,
        NULL
    };
    a->setCallback(&callback, this);
    mission3->runAction(a);
}

void QuestLayer::startQuestAnim(int index) {
    if (isQuiting)
        return;
    
    wyNode *missionContainer = container->getChildByTag(0);
//    LOGE("mission pos:%f, %f, %f, %f", missionContainer->getPositionX(), missionContainer->getPositionY(), container->getPositionX(), container->getPositionY());
    if (index == 3) {
        selectBorder(index-1, B_BLACK);
        isAnimFinished = true;
        
        if (onEndSelector != NULL) {
            onEndSelector->invoke();
        }
        return;
    }
    
    selectBorder(index-1, B_BLACK);
    selectBorder(index, B_WHITE);
    
    switch (index) {
        case 0:
            curMissionId = oldm1;
            nextMissionId = sGlobal->missionId1;
            break;
        case 1:
            curMissionId = oldm2;
            nextMissionId = sGlobal->missionId2;
            break;
        case 2:
            curMissionId = oldm3;
            nextMissionId = sGlobal->missionId3;
            break;
        default:
            break;
    }
    curMid = sGlobal->ACHV_SEQ[curMissionId];
    target = sGlobal->ACHV_NUM[curMid*3];
    from = sGlobal->achv_progress_old[curMid];
    to = sGlobal->achv_progress[curMid];
    
    sGlobal->achv_progress_old[curMid] = sGlobal->achv_progress[curMid];

    runProgressEffect();
}

void QuestLayer::runProgressEffect() {
    if (isQuiting)
        return;
    
    wyNode *missionContainer = container->getChildByTag(curIndex);
    wyProgressTimer *progress_bg = (wyProgressTimer*)missionContainer->getChildByTag(TAG_PROGRESS_BAR);
    wyBitmapFontLabel *progress = (wyBitmapFontLabel*)missionContainer->getChildByTag(TAG_PROGRESS_TITLE);
    progressTitle = progress;

    wyIntervalAction *progressTo = wyProgressTo::make(0.5f, from/(float)target * 100, to/(float)target * 100);
    wyActionCallback callback = {
        NULL,
        onProgressEnd,
        onProgressUpdate,
    };
    progressTo->setCallback(&callback, this);
    progress_bg->runAction(progressTo);
}

void QuestLayer::updateProgressTitle(float t) {
    char temp[20];
    sprintf(temp, "%d/%d", (int)((to - from)*t)+from, sGlobal->ACHV_NUM[curMid*3]);
    progressTitle->setText(temp);
}

void QuestLayer::onFinishProgressAnim() {
//    LOGE("mid:%d, %d", curMid, sGlobal->achv_rec[curMid]);
    if (sGlobal->achv_rec[curMid] == ACHV_FINISHED) {
//        LOGE("%d", sGlobal->ACHV_NUM[curMid*3+1]);
        sGlobal->zuanshi.offset(sGlobal->ACHV_NUM[curMid*3+1]);
        MyPrefs::setInt(DB_ZUANSHI_NUM, sGlobal->zuanshi.getValue());
        
        wyNode *missionContainer = container->getChildByTag(curIndex);
        selectBorder(curIndex, B_RED);
        
        wySpriteEx *cash_icon = (wySpriteEx*)missionContainer->getChildByTag(TAG_CASH_ICON);
        wyBitmapFontLabel *cash_num = (wyBitmapFontLabel*)missionContainer->getChildByTag(TAG_CASH_NUM);
        wyIntervalAction *move = wySequence::make(wyDelayTime::make(0.2f), wyMoveBy::make(0.3f, 0, DP(20)*sGlobal->scaleY), NULL);
        wyIntervalAction *fade = wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(0.3f), NULL);
        cash_icon->runAction(move);
        cash_icon->runAction(fade);
        cash_num->runAction(move->copy());
        cash_num->runAction(fade->copy());
        
        updateMission(curIndex+3, nextMissionId);
        
        wyNode *c0 = container->getChildByTag(curIndex);
        wyNode *c1 = container->getChildByTag(curIndex+3);
        
        c0->setPosition(wyDevice::winWidth/2-DP(205)*sGlobal->scaleX, c0->getPositionY());
        c1->setPosition(c0->getPositionX()+wyDevice::winWidth, c0->getPositionY());
        
        wyIntervalAction *a1 = wySequence::make(wyDelayTime::make(0.7f), wyMoveBy::make(0.25f, -wyDevice::winWidth, 0), wyDelayTime::make(0.5f), NULL);
        wyActionCallback callback = {
            NULL,
            onSingleQuestEnd,
            NULL,
        };
        a1->setCallback(&callback, this);
        c0->runAction(a1);
        
        c1->runAction(wySequence::make(wyDelayTime::make(0.7f), wyMoveBy::make(0.25f, -wyDevice::winWidth, 0), NULL));
    } else {
        startQuestAnim(++curIndex);
    }
}

void QuestLayer::selectBorder(int index, int status) {
    if (isQuiting)
        return;
    
//    LOGE("select border:%d", index);
    if (index < 0)
        return;
    
    wyColor4B color;
    switch (status) {
        case B_BLACK:
            color = wyc4b(0, 0, 0, 255);
            break;
        case B_WHITE:
            color = wyc4b(200, 200, 200, 255);
            break;
        case B_RED:
            color = wyc4b(253, 86, 30, 255);
            break;
        default:
            break;
    }
    
    wyNode *node = container->getChildByTag(index);
    for (int i = 0; i < 5; i++) {
        wyNode *n = node->getChildByTag(10+i);
        n->setColor(color);
    }
}

bool QuestLayer::touchesBegan(wyMotionEvent& e) {

}

void QuestLayer::createMission(int index, bool isNext) {
    int missionId;
    switch (index) {
        case 0:
            missionId = sGlobal->missionId1;
            break;
        case 1:
            missionId = sGlobal->missionId2;
            break;
        case 2:
            missionId = sGlobal->missionId3;
            break;
        default:
            missionId = sGlobal->missionId1;
            break;
    }
    
    wyNode *missionContainer = wyNode::make();
    missionContainer->setPosition(wyDevice::winWidth/2-DP(205)*sGlobal->scaleX, DP(218)*sGlobal->scaleY - DP(52)*sGlobal->scaleY*(index%3));
    missionContainer->setVisible(false);
    container->addChildLocked(missionContainer, 0, index);
    if (isNext) {
        missionContainer->setPosition(missionContainer->getPositionX()+wyDevice::winWidth, missionContainer->getPositionY());
        missionContainer->setVisible(true);
    }

    float borderScale = DP(6)*sGlobal->scaleX/DP(10);
    float borderWidth = DP(620)*sGlobal->scaleX*0.8f;
    float borderHeight = DP(84)*sGlobal->scaleY*0.8f;
    wyColor3B blackColor = wyc3b(0, 0, 0);
    
    wySpriteEx *frame_down = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_down_10.png");
    frame_down->setScaleX(sGlobal->scaleX*0.8f);
    frame_down->setScaleY(sGlobal->scaleY*0.6f);
    frame_down->setAnchor(0, 0);
    missionContainer->addChildLocked(frame_down, 0, 10);
    
    wySpriteEx *frame_left = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_left_16.png");
    frame_left->setScaleX(sGlobal->scaleX*0.6f);
    frame_left->setScaleY(sGlobal->scaleY*0.6f);
    frame_left->setAnchor(1.0f, 0);
    missionContainer->addChildLocked(frame_left, 0, 11);
    
    wySpriteEx *frame_right = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_right_16.png");
    frame_right->setScaleX(sGlobal->scaleX*0.6f);
    frame_right->setScaleY(sGlobal->scaleY*0.6f);
    frame_right->setAnchor(0, 0);
    frame_right->setPosition(frame_down->getWidth()*frame_down->getScaleX(), 0);
    missionContainer->addChildLocked(frame_right, 0, 12);
    
    wySpriteEx *frame_up = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_up_10.png");
    frame_up->setScaleX(sGlobal->scaleX*0.8f);
    frame_up->setScaleY(sGlobal->scaleY*0.6f);
    frame_up->setAnchor(0, 1.0f);
    frame_up->setPosition(0, frame_left->getHeight()*frame_left->getScaleY());
    missionContainer->addChildLocked(frame_up, 0, 13);
    
    wyColorLayer *frame_bg = wyColorLayer::make();
    frame_bg->setContentSize(frame_down->getWidth()*frame_down->getScaleX(), frame_left->getHeight()*frame_left->getScaleY()-DP(8)*sGlobal->scaleY);
    frame_bg->setAnchor(0, 0);
    frame_bg->setPosition(0, DP(4)*sGlobal->scaleY);
    missionContainer->addChildLocked(frame_bg, 0, 14);
    
    wyColor4B c = wyc4b(0, 0, 0, 255);
    frame_down->setColor(c);
    frame_up->setColor(c);
    frame_left->setColor(c);
    frame_right->setColor(c);
    frame_bg->setColor(c);

    wySpriteEx *badge;
    LOGE("%d", missionId);
    int mid = sGlobal->ACHV_SEQ[missionId];
    if (sGlobal->achv_rec[mid] == ACHV_FINISHED && !isNext) {
        badge = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "mission_complete.png");
    } else {
        badge = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "mission_uncomplete.png");
    }
    badge->setScaleX(sGlobal->scaleX*0.3f);
    badge->setScaleY(sGlobal->scaleY*0.3f);
//    badge->setAnchor(0, 1.0f);
    badge->setPosition(DP(16)*sGlobal->scaleX, DP(22)*sGlobal->scaleY);
    missionContainer->addChildLocked(badge, 1, TAG_BADGE);

    wyBitmapFontLabel *title = wyBitmapFontLabel::make(sGlobal->font_desc, "collect 10,0000 coin in one run (hard mode)");
    title->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    title->setPosition(DP(38)*sGlobal->scaleX, DP(40)*sGlobal->scaleY);
    title->setAnchor(0, 1.0f);
    missionContainer->addChildLocked(title, 1, TAG_TITLE);
  
    wySpriteEx *progress_border = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "progressbar_bottom.png");
    progress_border->setAnchor(0.0f, 1.0f);
    progress_border->setScaleX(1.54f*sGlobal->scaleX);
    progress_border->setScaleY(0.7f*sGlobal->scaleY);
    progress_border->setPosition(title->getPositionX(), title->getPositionY()-DP(22)*sGlobal->scaleY);
    missionContainer->addChildLocked(progress_border, 1);
    
    wyProgressTimer *atk_progress = wyProgressTimer::make(wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "progressbar_upper.png"));
    atk_progress->setStyle(HORIZONTAL_BAR_LR);
    atk_progress->setAnchor(0, 1.0f);
    atk_progress->setPosition(progress_border->getPositionX(), progress_border->getPositionY());
    atk_progress->setScaleX(1.54f*sGlobal->scaleX);
    atk_progress->setScaleY(0.7f*sGlobal->scaleX);
    missionContainer->addChildLocked(atk_progress, 1, TAG_PROGRESS_BAR);
    atk_progress->setPercentage(80);
    
    wyBitmapFontLabel *progress_title = wyBitmapFontLabel::make(sGlobal->font_desc, "1");
    progress_title->setScale(SP(12)*sGlobal->scaleX/sGlobal->descFontSize);
    progress_title->setPosition(progress_border->getPositionX()+progress_border->getWidth()*progress_border->getScaleX()/2, progress_border->getPositionY()-progress_border->getHeight()*progress_border->getScaleY()/2);
    progress_title->setAnchor(0.5f, 0.5f);
    missionContainer->addChildLocked(progress_title, 1, TAG_PROGRESS_TITLE);
    
    wySpriteEx *cash_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cash_icon.png");
    cash_icon->setScaleX(sGlobal->scaleX*0.6f);
    cash_icon->setScaleY(sGlobal->scaleY*0.6f);
    cash_icon->setAnchor(1.0, 0.5f);
    cash_icon->setPosition(title->getPositionX()+DP(355)*sGlobal->scaleX, title->getPositionY()-DP(18)*sGlobal->scaleY);
    missionContainer->addChildLocked(cash_icon, 1, TAG_CASH_ICON);
    
    wyBitmapFontLabel *cash_num = wyBitmapFontLabel::make(sGlobal->font_desc, "100");
    cash_num->setScale(SP(14)*sGlobal->scaleX/sGlobal->descFontSize);
    cash_num->setAlignment(wyBitmapFontLabel::LEFT);
    cash_num->setPosition(cash_icon->getPositionX() + DP(0)*sGlobal->scaleX, cash_icon->getPositionY()-DP(0)*sGlobal->scaleY);
    cash_num->setAnchor(0.0, 0.5f);
    missionContainer->addChildLocked(cash_num, 1, TAG_CASH_NUM);
    
    if (!isNext) {
        updateMission(index, missionId);
    }
}

void QuestLayer::updateMission(int index, int missionId) {
    wyNode *missionContainer = container->getChildByTag(index);
    wyProgressTimer *progress_bar = (wyProgressTimer*)missionContainer->getChildByTag(TAG_PROGRESS_BAR);
    wyBitmapFontLabel *title = (wyBitmapFontLabel*)missionContainer->getChildByTag(TAG_TITLE);
    wyBitmapFontLabel *progress = (wyBitmapFontLabel*)missionContainer->getChildByTag(TAG_PROGRESS_TITLE);
    wyBitmapFontLabel *cash_num = (wyBitmapFontLabel*)missionContainer->getChildByTag(TAG_CASH_NUM);
    
    int mid = sGlobal->ACHV_SEQ[missionId];
    progress_bar->setPercentage(sGlobal->achv_progress_old[mid]/(float)sGlobal->ACHV_NUM[mid*3]*100);
    
    char temp[50];
    char temp1[20];
    addComma(sGlobal->ACHV_NUM[mid*3], temp1);
    sprintf(temp, ACHV_DESC[mid], temp1);
    title->setText(temp);
    
    sprintf(temp, "%d/%d", sGlobal->achv_progress_old[mid], sGlobal->ACHV_NUM[mid*3]);
    progress->setText(temp);
    
    sprintf(temp, "%d", sGlobal->ACHV_NUM[mid*3+1]);
    cash_num->setText(temp);
    
//    LOGE("updateMission: %d, %d", mid, sGlobal->achv_progress[mid]);
}

void QuestLayer::setQuestEndSelector(wyTargetSelector *selector) {
    if (onEndSelector != NULL) {
        onEndSelector->release();
    }
    if (selector != NULL) {
        onEndSelector = selector;
        onEndSelector->retain();
    }
}

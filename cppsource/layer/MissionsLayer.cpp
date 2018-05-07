//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "MissionsLayer.h"
#include "Global.h"

MissionsLayer::MissionsLayer() {
    m_Title->setText("missions");
    btn_back->setButtonClickSelector(wyTargetSelector::make(this, SEL(MissionsLayer::onCloseButtonClick)));
    btn_back->setTouchPriority(2003);
    
    //current, complete tab
    createTabs();
    
    this->setTouchEnabled(true);
    this->setTouchPriority(2000);
    this->setInterceptTouch(true);
}

void MissionsLayer::onEnter() {
    wyLayer::onEnter();
    
}

void MissionsLayer::onCloseButtonClick(wyTargetSelector* ts) {
    this->getParent()->removeChildLocked(this, true);
}

void MissionsLayer::createTabs() {
    wySpriteEx *normal, *mask;
    wyBitmapFontLabel *title;

    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "current");
    title->setScale(SP(22)/sGlobal->descFontSize);
    
    tab_current = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(MissionsLayer::onTabCurrentClick)), mask, title);
    tab_current->setScaleX(sGlobal->scaleX*0.8f);
    tab_current->setScaleY(sGlobal->scaleY*0.8f);
    tab_current->setPosition(DP(230)*sGlobal->scaleX, wyDevice::winHeight-DP(26)*sGlobal->scaleY);
    tab_current->setMultiTouchClickable(false);
    tab_current->setInterceptTouch(true);
    tab_current->setTouchPriority(10000);
    addChildLocked(tab_current);
    tab_current->needDelay = false;
    
    normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    title = wyBitmapFontLabel::make(sGlobal->font_desc, "complete");
    title->setScale(SP(22)/sGlobal->descFontSize);
    
    tab_complete = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(MissionsLayer::onTabCompleteClick)), mask, title);
    tab_complete->setScaleX(sGlobal->scaleX*0.8f);
    tab_complete->setScaleY(sGlobal->scaleY*0.8f);
    tab_complete->setPosition(tab_current->getPositionX()+DP(120)*sGlobal->scaleX, wyDevice::winHeight-DP(26)*sGlobal->scaleY);
    tab_complete->setMultiTouchClickable(false);
    tab_complete->setInterceptTouch(true);
    tab_complete->setTouchPriority(10000);
    addChildLocked(tab_complete);
    tab_complete->needDelay = false;

    createCurrentMissionsLayer();
    createCompleteMissionsLayer();
    
    curTabIndex = -1;
    onTabCurrentClick(NULL);
}

void MissionsLayer::createCurrentMissionsLayer() {
    currentMissionLayer = wyNode::make();
    addChildLocked(currentMissionLayer);
    
    for (int i = 0; i < 3; i++) {
        int missionId;
        switch (i) {
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
        wyNode *mission = createMission(i, missionId, true);
        currentMissionLayer->addChildLocked(mission, 0, i);
    }
}

void MissionsLayer::onScrollOffsetChanged(wyScrollableLayer* layer) {
//    LOGE("offset:%f", layer->getOffsetY());
}

void MissionsLayer::createCompleteMissionsLayer() {
    float w = wyDevice::winWidth*0.75f;
    
    completeMissionLayer = wyScrollableLayer::make(wyc4b(255, 0, 0, 0));
	completeMissionLayer->setContentSize(wyDevice::winWidth, DP(280)*sGlobal->scaleY);
    completeMissionLayer->setPosition(DP(0)*sGlobal->scaleX, DP(82)*sGlobal->scaleY);
    completeMissionLayer->setAnchor(0, 1.0f);
    completeMissionLayer->setVertical(true);
    completeMissionLayer->setHorizontal(false);
    completeMissionLayer->setLeftMargin((wyDevice::winWidth-w)/2);
    completeMissionLayer->setRightMargin(DP(0));
	completeMissionLayer->setTopMargin(DP(16)*sGlobal->scaleY);
    completeMissionLayer->setBottomMargin(DP(22)*sGlobal->scaleY);
    completeMissionLayer->setTouchPriority(2001);
    addChildLocked(completeMissionLayer);
	completeMissionLayer->setClipRect(wyr(DP(0)*sGlobal->scaleX, DP(80)*sGlobal->scaleY, wyDevice::winWidth, DP(290)*sGlobal->scaleY));
    vThumb = wyNinePatchSprite::make(MyTextureManager::makePNG("vertical_thumb.png"), wyr(DP(5), DP(7), DP(1), DP(1)));
    completeMissionLayer->setVerticalThumb(vThumb);
    completeMissionLayer->setThumbFadeOutTime(1.0f);
    
//    completeMissionLayer->setCallback(this);
    int idx = 0;
    for (int i = 0; i < ARR_LEN_ACHV_NUM; i++) {
//        LOGE("missionId:%d, status:%d", i, sGlobal->achv_rec[i]);
        if (sGlobal->achv_rec[i] == ACHV_FINISHED) {
            wyNode *mission = createMission(idx, i, false);
            idx++;
            completeMissionLayer->addScrollableChildLocked(mission, 0, idx);
        }
    }
    if (idx < 3) {
        wyNode *mission = createMission(2, 1, false);
        completeMissionLayer->addScrollableChildLocked(mission, 0, 0);
        mission->setVisible(false);
    }
    
}

void MissionsLayer::onTabCurrentClick(wyTargetSelector* ts) {
    if (curTabIndex == TAB_MISSION_CURRENT) {
        return;
    }
    
    curTabIndex = TAB_MISSION_CURRENT;
//    sGlobal->playEffect("se_button.mp3");
    
    setPressColor(tab_complete);
    setPressColor(tab_complete->m_Title);
    tab_complete->setScaleX(sGlobal->scaleX*0.7f);
    tab_complete->setScaleY(sGlobal->scaleY*0.7f);
    
    tab_current->setColor(wyc3b(255, 255, 255));
    tab_current->m_Title->setColor(wyc3b(255, 255, 255));
    tab_current->setScaleX(sGlobal->scaleX*0.8f);
    tab_current->setScaleY(sGlobal->scaleY*0.8f);
    
    currentMissionLayer->setVisible(true);
    completeMissionLayer->setVisible(false);
}

void MissionsLayer::onTabCompleteClick(wyTargetSelector* ts) {
    if (curTabIndex == TAB_MISSION_COMPLETE) {
        return;
    }
    
    curTabIndex = TAB_MISSION_COMPLETE;
//    sGlobal->playEffect("se_button.mp3");
    
    setPressColor(tab_current);
    setPressColor(tab_current->m_Title);
    tab_current->setScaleX(sGlobal->scaleX*0.7f);
    tab_current->setScaleY(sGlobal->scaleY*0.7f);
    
    tab_complete->setColor(wyc3b(255, 255, 255));
    tab_complete->m_Title->setColor(wyc3b(255, 255, 255));
    tab_complete->setScaleX(sGlobal->scaleX*0.8f);
    tab_complete->setScaleY(sGlobal->scaleY*0.8f);
    
    currentMissionLayer->setVisible(false);
    completeMissionLayer->setVisible(true);
    
    completeMissionLayer->setOffsetY(0);
//    vThumb->runAction(wySequence::make(wyDelayTime::make(0.1f), wyFadeOut::make(0.5f), NULL));
}

wyNode* MissionsLayer::createMission(int index, int missionId, bool isMid) {
    wySpriteEx *frame_down = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_down_10.png");
    wySpriteEx *frame_left = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_left_16.png");
    
    float w = wyDevice::winWidth*0.75f;
    float sx = w/frame_down->getWidth();
    float sy = sGlobal->scaleY*0.8f;
    
    float h = frame_left->getHeight()*sy;
    
    wyNode *missionContainer = wyNode::make();
    missionContainer->setAnchor(0, 0);
    missionContainer->setContentSize(w, h);
    missionContainer->setPosition((wyDevice::winWidth-w)/2, DP(288)*sGlobal->scaleY - DP(92)*sGlobal->scaleY*index);
//    missionContainer->setPosition(100, 100 + DP(92)*sGlobal->scaleY*index);
//    parent->addChildLocked(missionContainer, 0, index);

//    wySprite *frame_down = wySprite::make(MyTextureManager::makePNG("frame_down_10.png"));
    frame_down->setScaleX(sx);
    frame_down->setScaleY(sy);
    frame_down->setAnchor(0, 0);
    missionContainer->addChildLocked(frame_down, 0, 10);
    
    
    frame_left->setScaleX(sy);
    frame_left->setScaleY(sy);
    frame_left->setAnchor(1.0f, 0);
    missionContainer->addChildLocked(frame_left, 0, 11);
    
    wySpriteEx *frame_right = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_right_16.png");
    frame_right->setScaleX(sy);
    frame_right->setScaleY(sy);
    frame_right->setAnchor(0, 0);
    frame_right->setPosition(frame_down->getWidth()*frame_down->getScaleX(), 0);
    missionContainer->addChildLocked(frame_right, 0, 12);
    
    wySpriteEx *frame_up = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_up_10.png");
    frame_up->setScaleX(sx);
    frame_up->setScaleY(sy);
    frame_up->setAnchor(0, 1.0f);
    frame_up->setPosition(0, frame_left->getHeight()*frame_left->getScaleY());
    missionContainer->addChildLocked(frame_up, 0, 13);
    
    wyColorLayer *frame_bg = wyColorLayer::make();
    frame_bg->setContentSize(frame_down->getWidth()*frame_down->getScaleX(), frame_left->getHeight()*frame_left->getScaleY()-DP(8)*sGlobal->scaleY);
    frame_bg->setAnchor(0, 0);
    frame_bg->setPosition(0, DP(4)*sGlobal->scaleY);
    missionContainer->addChildLocked(frame_bg, 0, 14);
    
    wyColor4B c = wyc4b(80, 80, 80, 255);
    frame_down->setColor(c);
    frame_up->setColor(c);
    frame_left->setColor(c);
    frame_right->setColor(c);
    frame_bg->setColor(c);
    
    wySpriteEx *badge;
    int mid;
    if (isMid) {
        mid = sGlobal->ACHV_SEQ[missionId];
    } else {
        mid = missionId;
    }
//    LOGE("mid:%d", mid);
    if (sGlobal->achv_rec[mid] == ACHV_FINISHED) {
        badge = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "mission_complete.png");
    } else {
        badge = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "mission_uncomplete.png");
    }
    
    badge->setScaleX(sGlobal->scaleX*0.5f);
    badge->setScaleY(sGlobal->scaleY*0.5f);
    badge->setAnchor(0, 0.5f);
    badge->setPosition(DP(15)*sGlobal->scaleX, h/2+DP(0)*sGlobal->scaleY);
    missionContainer->addChildLocked(badge, 1, TAG_BADGE);
    
    wyBitmapFontLabel *title = wyBitmapFontLabel::make(sGlobal->font_desc, "collect 10,0000 coin in one run (hard mode)");
    title->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    title->setPosition(DP(80)*sGlobal->scaleX, badge->getPositionY()+DP(10)*sGlobal->scaleY);
    title->setAnchor(0, 0.5f);
    missionContainer->addChildLocked(title, 1, TAG_TITLE);
    
    wySpriteEx *progress_border = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "progressbar_bottom.png");
    progress_border->setAnchor(0.0f, 0.5f);
    progress_border->setScaleX(1.5f*sGlobal->scaleX);
    progress_border->setScaleY(0.7f*sGlobal->scaleY);
    progress_border->setPosition(title->getPositionX(), title->getPositionY()-DP(22)*sGlobal->scaleY);
    missionContainer->addChildLocked(progress_border, 1);
    
    wyProgressTimer *atk_progress = wyProgressTimer::make(wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "progressbar_upper.png"));
    atk_progress->setStyle(HORIZONTAL_BAR_LR);
    atk_progress->setAnchor(0, 0.5f);
    atk_progress->setPosition(progress_border->getPositionX(), progress_border->getPositionY());
    atk_progress->setScaleX(1.5f*sGlobal->scaleX);
    atk_progress->setScaleY(0.7f*sGlobal->scaleX);
    missionContainer->addChildLocked(atk_progress, 1, TAG_PROGRESS_BAR);
    atk_progress->setPercentage(80);
    
    wyBitmapFontLabel *progress_title = wyBitmapFontLabel::make(sGlobal->font_desc, "1");
    progress_title->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    progress_title->setPosition(progress_border->getPositionX()+progress_border->getWidth()*progress_border->getScaleX()/2, progress_border->getPositionY());
    progress_title->setAnchor(0.5f, 0.5f);
    missionContainer->addChildLocked(progress_title, 1, TAG_PROGRESS_TITLE);
    
    wySpriteEx *cash_icon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cash_icon.png");
    cash_icon->setScaleX(sGlobal->scaleX*0.7f);
    cash_icon->setScaleY(sGlobal->scaleY*0.7f);
    cash_icon->setAnchor(0.5f, 0.5f);
    cash_icon->setPosition(w-DP(60)*sGlobal->scaleX, badge->getPositionY()-DP(0)*sGlobal->scaleY);
    missionContainer->addChildLocked(cash_icon, 1, TAG_CASH_ICON);
    
    wyBitmapFontLabel *cash_num = wyBitmapFontLabel::make(sGlobal->font_desc, "100");
    cash_num->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    cash_num->setAlignment(wyBitmapFontLabel::LEFT);
    cash_num->setPosition(cash_icon->getPositionX() + DP(25)*sGlobal->scaleX, cash_icon->getPositionY()-DP(0)*sGlobal->scaleY);
    cash_num->setAnchor(0.5f, 0.5f);
    missionContainer->addChildLocked(cash_num, 1, TAG_CASH_NUM);
    
    updateMission(missionContainer, mid);
    
    return missionContainer;
}

void MissionsLayer::updateMission(wyNode *container, int missionId) {
    wyNode *missionContainer = container;
    wyProgressTimer *progress_bar = (wyProgressTimer*)missionContainer->getChildByTag(TAG_PROGRESS_BAR);
    wyBitmapFontLabel *title = (wyBitmapFontLabel*)missionContainer->getChildByTag(TAG_TITLE);
    wyBitmapFontLabel *progress = (wyBitmapFontLabel*)missionContainer->getChildByTag(TAG_PROGRESS_TITLE);
    wyBitmapFontLabel *cash_num = (wyBitmapFontLabel*)missionContainer->getChildByTag(TAG_CASH_NUM);
    
    int mid = missionId;
    progress_bar->setPercentage(sGlobal->achv_progress[mid]/(float)sGlobal->ACHV_NUM[mid*3]*100);
    
    char temp[50];
    char temp1[20];
    addComma(sGlobal->ACHV_NUM[mid*3], temp1);
    sprintf(temp, ACHV_DESC[mid], temp1);
    title->setText(temp);
    
    sprintf(temp, "%d/%d", sGlobal->achv_progress[mid], sGlobal->ACHV_NUM[mid*3]);
    progress->setText(temp);
    
    sprintf(temp, "%d", sGlobal->ACHV_NUM[mid*3+1]);
    cash_num->setText(temp);
    
//    LOGE("updateMission: %d, %d", mid, sGlobal->achv_progress[mid]);
}

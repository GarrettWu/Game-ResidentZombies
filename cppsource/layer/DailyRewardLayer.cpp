//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "DailyRewardLayer.h"
#include "Global.h"
#include "MainScene.h"
#include "LoadingScene.h"
#include "UpgradeScene.h"
#include "ControlButton.h"
#if ANDROID
#include "com_wiyun_engine_skeleton_Skeleton.h"
#endif
#if IOS
#import "ObjCCalls.h"
#endif
#include "DialogBgLayer.h"
#include "ShopLayer.h"
#include "CGScene.h"

void DailyRewardLayer::showParticle(wySpriteEx *sprite){
    
}

wySpriteEx* DailyRewardLayer::getRewardSprite(int dayIndex, bool isUnlock){
    wySpriteEx *sprite;
    switch (sGlobal->Reward_Unit[dayIndex]) {
        case 0:
            if (isUnlock) {
                sprite = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shop_coin.png");
            } else {
                sprite = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "shop_coin_gray.png");
            }
            break;
        case 1:
            if (isUnlock) {
                sprite = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shop_cash.png");
            } else {
                sprite = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "shop_cash_gray.png");
            }
            break;
        default:
            break;
    }
    
    sprite->setScaleX(sGlobal->scaleX*0.6f);
    sprite->setScaleY(sGlobal->scaleY*0.6f);
    return sprite;
    
}
DailyRewardLayer::DailyRewardLayer(bool isDay5Reached, bool isPlaced) {
    this->isPlaced = isPlaced;
    
    if (!isPlaced) {
        
    } else {
        isDay5Reached = false;
        
        long long now=getCurrentTimeForDailyReward();
        LOGE("now:%lld, last:%lld, sub:%lld", now, wyPrefs::getInt64("lastTime", 0), (now-wyPrefs::getInt64("lastTime", 0))/1000);
        if(!wyPrefs::getInt64("lastTime")){
            wyPrefs::setInt64("lastTime", now);
            wyPrefs::setInt("loginDay",1);
        }else{
            long long sub=now-wyPrefs::getInt64("lastTime");
            if (sub < 0) {
                wyPrefs::setInt("loginDay",1);
                wyPrefs::setInt64("lastTime", now);
            }
            if(sub>aDay&&sub<=tDay){
                
            }
            else if (sub>tDay) {
                wyPrefs::setInt("loginDay",1);
                wyPrefs::setInt64("lastTime", now);
            }
        }
    }
    lastPlayTime = wyPrefs::getInt64("lastTime");
    
    bool unlocked = true;
    bool needUnlock = false;
    wyColorLayer *bg1 = wyColorLayer::make();
    bg1->setColor(wyc4b(0, 0, 0, 100));
    bg1->setPosition(0, 0);
    addChildLocked(bg1);
    container = wyNode::make();
    container->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/2);
    addChildLocked(container);
    DialogBgLayer *bgLayer = DialogBgLayer::make();
    bgLayer->setScaleY(1.0f);
    bgLayer->setScaleX(1.2f);
    bgLayer->setAnchor(0, 0);
    bgLayer->setPosition(bgLayer->getPositionX()-DP(4)*sGlobal->scaleX, bgLayer->getPositionY()-DP(10)*sGlobal->scaleX);
    container->addChildLocked(bgLayer);
    
    wyBitmapFontLabel *content = wyBitmapFontLabel::make(sGlobal->font_title, "daily reward");
    content->setScale(SP(24)*sGlobal->scaleX/sGlobal->descFontSize);
    content->setAlignment(wyBitmapFontLabel::LEFT);
    content->setAnchor(0, 0.5f);
    container->addChildLocked(content);
    content->setPosition(-DP(260)*sGlobal->scaleX, DP(130)*sGlobal->scaleY);
    
    countDownLabel = wyBitmapFontLabel::make(sGlobal->font_desc, "daily reward");
    countDownLabel->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    countDownLabel->setAlignment(wyBitmapFontLabel::LEFT);
    countDownLabel->setAnchor(0.0f, 0.5f);
    container->addChildLocked(countDownLabel);
    countDownLabel->setPosition(DP(120)*sGlobal->scaleX, DP(132)*sGlobal->scaleY);
    countDownLabel->setColor(wyc3b(220, 220, 220));
    
    wySpriteEx *normal = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    wySpriteEx *mask = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "btn_common_normal.png");
    mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    wyBitmapFontLabel *title = wyBitmapFontLabel::make(sGlobal->font_desc, "ok");
    title->setScale(SP(24)/sGlobal->descFontSize);
    okBtn = ControlButton::make(normal, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL( DailyRewardLayer::onCloseButtonClick)), mask, title);
    okBtn->setScaleX(sGlobal->scaleX*0.8f);
    okBtn->setScaleY(sGlobal->scaleY*0.8f);
    okBtn->setMultiTouchClickable(false);
    okBtn->setInterceptTouch(true);
    okBtn->setTouchPriority(10000);
    container->addChildLocked(okBtn, 100);
    okBtn->setPosition(DP(8)*sGlobal->scaleX, -DP(115)*sGlobal->scaleY);
    okBtn->needDelay = false;
    
    currentLoginDay = wyPrefs::getInt("loginDay");
    for (int i=0; i<5; i++) {
        wySpriteEx* sp_money;
        wySpriteEx *sp_bg=wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "daily_reward_border.png");
        sp_bg->setScaleX(sGlobal->scaleX*0.8f);
        sp_bg->setScaleY(sGlobal->scaleY*0.6f);
        sp_bg->setPosition(DP(-212)*sGlobal->scaleX+DP(110*i)*sGlobal->scaleX,DP(30)*sGlobal->scaleY);
        sp_bg->setAnchor(0.5f, 0.5f);
        container->addChildLocked(sp_bg, 0, i);

        //如果连续登陆一天以上
        bool isUnlock;
        if (i<(currentLoginDay-1)) {
            isUnlock = true;
        } else {
            if (isPlaced) {
                if (i == currentLoginDay-1) {
                    isUnlock = true;
                } else {
                    isUnlock = false;
                }
            } else {
                if (i == currentLoginDay-1 && currentLoginDay == 5 && isDay5Reached) {
                    isUnlock = true;
                } else {
                    isUnlock = false;
                }
            }
        }
        
        if (isUnlock) {
            sp_money = getRewardSprite(i, true);
            sp_bg->setColor(orangeColor);
        } else {
            sp_money = getRewardSprite(i, false);
            sp_bg->setColor(grayColor);
        }
        
        sp_money->setPosition(sp_bg->getPositionX(), sp_bg->getPositionY()+DP(20)*sGlobal->scaleY);
        container->addChildLocked(sp_money, 0, 100+i);
        
        wySpriteEx *currencyIcon;
        switch (sGlobal->Reward_Unit[i]) {
            case 0:
                currencyIcon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
                break;
            case 1:
                currencyIcon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cash_icon.png");
                break;
            default:
                currencyIcon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
                break;
        }
        currencyIcon->setScaleX(sGlobal->scaleX*0.7f);
        currencyIcon->setScaleY(sGlobal->scaleY*0.7f);
        container->addChildLocked(currencyIcon, 0, 200+i);
        
        char buf[100];
        sprintf(buf, "day %d",i+1);
        char buffer[100];
        sprintf(buffer,"%d",sGlobal->Reward_Number[i]);
        
        wyBitmapFontLabel *rewardLabel=wyBitmapFontLabel::make(sGlobal->font_desc,buffer);
        rewardLabel->setScale(DP(16)*sGlobal->scaleX/sGlobal->descFontSize);
        
        wyBitmapFontLabel *dayLabel= wyBitmapFontLabel::make(sGlobal->font_desc,buf);
        dayLabel->setScale(DP(18)*sGlobal->scaleX/sGlobal->descFontSize);
        dayLabel->setPosition(sp_bg->getPositionX(),sp_bg->getPositionY()+DP(-80)*sGlobal->scaleY);
        
        if (isUnlock) {
            dayLabel->setColor(orangeColor);
        } else {
            dayLabel->setColor(grayColor);
        }
        
        float w1 = rewardLabel->getWidth()*rewardLabel->getScaleX()+DP(4)*sGlobal->scaleX;
        float w2 = currencyIcon->getWidth()*currencyIcon->getScaleX()+DP(0)*sGlobal->scaleX;
        float hw = (w1+w2)/2;
        rewardLabel->setPosition(sp_bg->getPositionX()-hw+w1/2, sp_bg->getPositionY()+DP(-36)*sGlobal->scaleY);
        currencyIcon->setPosition(sp_bg->getPositionX()+hw-w2/2, rewardLabel->getPositionY());
        
        container->addChildLocked(rewardLabel, 0, 300+i);
        container->addChildLocked(dayLabel,0,400+i);
        
        wySpriteEx *sp_bg_mask=wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "daily_reward_border.png");
        sp_bg_mask->setScaleX(sGlobal->scaleX*0.8f);
        sp_bg_mask->setScaleY(sGlobal->scaleY*0.6f);
        sp_bg_mask->setPosition(sp_bg->getPositionX(),sp_bg->getPositionY());
        container->addChildLocked(sp_bg_mask, 1, 500+i);
        sp_bg_mask->setColor(orangeColor);
        sp_bg_mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        
        if (i == currentLoginDay) {
            sp_bg_mask->runAction(wyRepeatForever::make(wySequence::make(wyFadeOut::make(1.0f), wyFadeIn::make(1.0f), NULL)));
        } else {
            sp_bg_mask->setVisible(false);
        }
    }
    
    this->setTouchEnabled(true);
    this->setTouchPriority(2000);
    this->setInterceptTouch(true);
    
    isEnterAnimEnd = false;
    
    container->setScale(0);
    
    long long now = getCurrentTimeForDailyReward();
    long long diff = now - lastPlayTime;
    diff = aDay - diff;
    if (diff<0) {
        diff = 0;
    }
    int hour = diff/1000/60/60;
    int min = diff/1000/60-hour*60;
    int sec = diff/1000-hour*60*60-min*60;
    char temp[50];
    sprintf(temp, "next reward time:\n%02dh:%02dm:%02ds", hour, min, sec);
    countDownLabel->setText(temp);
    
    if (!isPlaced) {
        getRealReward();
    }
}

void DailyRewardLayer::getRealReward() {
    //add reward here
    int rewardNum = sGlobal->Reward_Number[currentLoginDay-1];
    switch (sGlobal->Reward_Unit[currentLoginDay-1]) {
        case 0:
            sGlobal->gold.offset(rewardNum);
            MyPrefs::setInt(DB_GOLD, sGlobal->gold.getValue());
            LOGE("reward gold:%d", rewardNum);
            break;
        case 1:
            sGlobal->zuanshi.offset(rewardNum);
            MyPrefs::setInt(DB_ZUANSHI_NUM, sGlobal->zuanshi.getValue());
            LOGE("reward cash:%d", rewardNum);
            break;
        default:
            break;
    }
    
    sGlobal->notifyAllCoinChangeListeners();
    
#if ANDROID
    sGlobal->reportGameEvent(EVT_DAILYREWARD, currentLoginDay-1);
#endif
}

void DailyRewardLayer::updateCountDown(wyTargetSelector *ts) {
    long long now = getCurrentTimeForDailyReward();
    long long sub = now - lastPlayTime;
    long diff = aDay - sub;
    if (diff<0) {
        diff = 0;
    }
//    LOGE("now:%lld, last:%lld, diff:%lld", now, wyPrefs::getInt64("lastTime", 0), diff);
    int hour = diff/1000/60/60;
    int min = diff/1000/60-hour*60;
    int sec = diff/1000-hour*60*60-min*60;
    
    char temp[50];
    sprintf(temp, "next reward time:\n%02dh:%02dm:%02ds", hour, min, sec);
    countDownLabel->setText(temp);

    if(sub>aDay&&sub<=tDay){
        currentLoginDay = MIN(5, wyPrefs::getInt("loginDay")+1);
        
        wyPrefs::setInt("loginDay", currentLoginDay);
        wyPrefs::setInt64("lastTime", now);
        lastPlayTime = now;
        
        getRealReward();
        runUnlockRewardEffect(NULL);
    } else if (sub>tDay) {
        wyPrefs::setInt("loginDay",0);
        wyPrefs::setInt64("lastTime", now);
        lastPlayTime = now;
    }
}

void DailyRewardLayer::runUnlockRewardEffect(wyTargetSelector *ts) {
    int dayIndex = currentLoginDay-1;
    
    wySpriteEx *sp_bg = (wySpriteEx*)container->getChildByTagLocked(dayIndex);
    sp_bg->setColor(orangeColor);
    
    wySpriteEx *sp_bg_mask = (wySpriteEx*)container->getChildByTagLocked(dayIndex+500);
    sp_bg_mask->stopAllActions();
    sp_bg_mask->setVisible(true);
//    sp_bg_mask->setScaleX(sGlobal->scaleX*0.8f);
//    sp_bg_mask->setScaleY(sGlobal->scaleY*0.6f);
//    sp_bg_mask->setPosition(sp_bg->getPositionX(),sp_bg->getPositionY());
//    container->addChildLocked(sp_bg_mask, 1);
//    sp_bg_mask->setColor(orangeColor);
//    sp_bg_mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    
    wySpriteEx *sp_money = (wySpriteEx*)container->getChildByTagLocked(dayIndex+100);
    float xx = sp_money->getPositionX();
    float yy = sp_money->getPositionY();
    container->removeChildLocked(sp_money, true);
    sp_money = getRewardSprite(dayIndex, true);
    container->addChildLocked(sp_money, 0, dayIndex+100);
    sp_money->setPosition(xx, yy);
    
    wySpriteEx *sp_money_mask = getRewardSprite(dayIndex, true);
    container->addChildLocked(sp_money_mask);
    sp_money_mask->setPosition(xx, yy);
    sp_money_mask->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    
    wySpriteEx *currencyIcon = (wySpriteEx*)container->getChildByTagLocked(dayIndex+200);
    
    wySpriteEx *currencyIcon1;
    switch (sGlobal->Reward_Unit[dayIndex]) {
        case 0:
            currencyIcon1 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
            break;
        case 1:
            currencyIcon1 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cash_icon.png");
            break;
        default:
            currencyIcon1 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
            break;
    }
    currencyIcon1->setScaleX(sGlobal->scaleX*0.7f);
    currencyIcon1->setScaleY(sGlobal->scaleY*0.7f);
    currencyIcon1->setPosition(currencyIcon->getPositionX(), currencyIcon->getPositionY());
    container->addChildLocked(currencyIcon1, 1);
    
    wyBitmapFontLabel *rewardLabel = (wyBitmapFontLabel*)container->getChildByTagLocked(dayIndex+300);
    char buffer[100];
    sprintf(buffer,"%d",sGlobal->Reward_Number[dayIndex]);
    wyBitmapFontLabel *rewardLabel1 = wyBitmapFontLabel::make(sGlobal->font_desc,buffer);
    rewardLabel1->setScale(DP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    rewardLabel1->setPosition(rewardLabel->getPositionX(), rewardLabel->getPositionY());
    container->addChildLocked(rewardLabel1, 1);
    
    wyBitmapFontLabel *dayLabel = (wyBitmapFontLabel*)container->getChildByTagLocked(dayIndex+400);
    dayLabel->setColor(orangeColor);
    
    wyParticleSystem *train_skill_emitter = wyParticleLoader::load("particle_train_skill.plist");
    float scale = sGlobal->scaleX;
    float width = sp_bg->getWidth()*sp_bg->getScaleX();
    float height = sp_bg->getHeight()*sp_bg->getScaleY();
    train_skill_emitter->setStartSizeVariance(10.0f, 5.0f);
    train_skill_emitter->setParticlePositionVariance(0, 0, width/2/scale,0);
    train_skill_emitter->setEmissionRate(100);
    train_skill_emitter->setDuration(1.0f);
    train_skill_emitter->setLifeVariance(1.0, 0.2);
    train_skill_emitter->setScale(scale);
    train_skill_emitter->setPosition(sp_bg->getPositionX(), sp_bg->getPositionY()-height/2);
    train_skill_emitter->setAutoRemoveOnFinish(true);
    train_skill_emitter->setPositionType(GROUPED);
    container->addChildLocked(train_skill_emitter);

    currencyIcon1->runAction(wyMoveBy::make(1.0f, 0, DP(40)*sGlobal->scaleY));
    currencyIcon1->runAction(wyFadeOut::make(2.0f));
    rewardLabel1->runAction(wyMoveBy::make(1.0f, 0, DP(40)*sGlobal->scaleY));
    rewardLabel1->runAction(wyFadeOut::make(2.0f));
    
    sp_bg_mask->runAction(wyFadeOut::make(1.0f));
    sp_money_mask->runAction(wyFadeOut::make(1.0f));
    
    this->runAction(wySequence::make(wyDelayTime::make(2.0f), wyCallFunc::make(wyTargetSelector::make(this, SEL_i(DailyRewardLayer::showNextDay), dayIndex)), NULL));
    sGlobal->playEffect(se_upgrade);
}

void DailyRewardLayer::showNextDay(wyTargetSelector *ts, int dayIndex) {
    int nextDayIndex = dayIndex + 1;
    if (nextDayIndex > 4) {
        nextDayIndex = 4;
    }
    wySpriteEx *sp_bg_mask = (wySpriteEx*)container->getChildByTagLocked(nextDayIndex+500);
    sp_bg_mask->stopAllActions();
    sp_bg_mask->setVisible(true);
    
    sp_bg_mask->runAction(wyRepeatForever::make(wySequence::make(wyFadeOut::make(1.0f), wyFadeIn::make(1.0f), NULL)));
}

static void onEnterAnimEnd(wyAction* action, void* data) {
    DailyRewardLayer *layer = (DailyRewardLayer *)data;
    
    layer->isEnterAnimEnd = true;
    
    if (!layer->isPlaced) {
        layer->runAction(wySequence::make(wyDelayTime::make(1.0f), wyCallFunc::make(wyTargetSelector::make(layer, SEL(DailyRewardLayer::runUnlockRewardEffect))), NULL));
    }
}

void DailyRewardLayer::onEnter() {
    
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
    
    wyTargetSelector* ts = wyTargetSelector::make(this, SEL(DailyRewardLayer::updateCountDown));
    wyTimer* t = wyTimer::make(ts, 1.0f);
    scheduleLocked(t);
}


bool DailyRewardLayer::touchesBegan(wyMotionEvent& event) {
    //    if (isEnterAnimEnd) {
    //        this->getParent()->removeChildLocked(this, true);
    //    }
    
    return true;
}

bool DailyRewardLayer::touchesMoved(wyMotionEvent& event) {
    //    this->getParent()->removeChildLocked(this, true);
    return true;
}

bool DailyRewardLayer::touchesEnded(wyMotionEvent& event) {
    //    this->getParent()->removeChildLocked(this, true);
    return true;
}

void DailyRewardLayer::onCloseButtonClick(wyTargetSelector* ts) {
    //    sGlobal->playEffect("se_button.mp3");
    
    if (isEnterAnimEnd) {
        if (!isPlaced) {
            VoteUsLayer::showVoteUsLayer(this->getParent());
        }
       
        this->getParent()->removeChildLocked(this, true);
    }
}

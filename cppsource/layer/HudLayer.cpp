//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "HudLayer.h"
#include "Global.h"
#include "Intro.h"
#include "OptionLayer.h"
#include "GameCompleteLayer.h"
#include "MainScene.h"

static const int UP_ANIM = 2;
static const int DOWN_ANIM = 1;
static const int LEFT_ANIM = 0;
static const int RIGHT_ANIM = 0;
static const int IDLE_ANIM = 0;

HudLayer::HudLayer(int mapIndex) {
    sGlobal->hudLayer = this;
    
    wySpriteEx* normal2 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "firebtn.png");
	atkBtn = wyButton::make(normal2, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(HudLayer::onRightButtonClick)), wyTargetSelector::make(this, SEL(HudLayer::onRightButtonUp)));
	atkBtn->setPosition(wyDevice::winWidth - DP(65)*sGlobal->scaleX, DP(54)*sGlobal->scaleY);
    atkBtn->setMultiTouchClickable(true);
    atkBtn->setInterceptTouch(true);
    atkBtn->setScaleX(sGlobal->scaleX*0.7f);
    atkBtn->setScaleY(sGlobal->scaleY*0.7f);
    atkBtn->setAlpha(200);
    atkBtn->setTouchPriority(101);
    atkBtn->setMoveOutSelector(wyTargetSelector::make(this, SEL(HudLayer::onRightButtonMoveOut)));
	addChildLocked(atkBtn);

    char temp[50];
    dart = sGlobal->getArmsSpriteByWeaponIndex(sGlobal->activeWeapon); //wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", temp);
    dart->setPosition(atkBtn->getPositionX(), atkBtn->getPositionY());
    dart->setScaleX(sGlobal->scaleX*0.5f/0.8f);
    dart->setScaleY(sGlobal->scaleY*0.5f/0.8f);
    dart->setAlpha(200);
    addChildLocked(dart);

    wySpriteEx* normal3 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "firebtn.png");
	skillBtn = wyButton::make(normal3, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(HudLayer::onSkillButtonClick)), wyTargetSelector::make(this, SEL(HudLayer::onSkillButtonClick1)));
	skillBtn->setPosition(wyDevice::winWidth - DP(200)*sGlobal->scaleX, DP(54)*sGlobal->scaleY);
    skillBtn->setMultiTouchClickable(true);
//    skillBtn->setInterceptTouch(true);
    skillBtn->setScaleX(sGlobal->scaleX*0.7f);
    skillBtn->setScaleY(sGlobal->scaleY*0.7f);
    skillBtn->setAlpha(200);
    skillBtn->setTouchPriority(101);
	addChildLocked(skillBtn);
    
    switch (sGlobal->activeSkill) {
        case SKILL_MISSLE:
            sprintf(temp, "skill_0.png");
            break;
        case SKILL_ROBOT:
            sprintf(temp, "skill_1.png");
            break;
        case SKILL_SHIELD:
            sprintf(temp, "skill_2.png");
            break;
        default:
            sprintf(temp, "skill_0.png");
            break;
    }
    wySpriteEx* normal4 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", temp);
//    wySpriteEx* normal4 = wySpriteEx::make(MyTextureManager::makePNG(temp));
    normal4->setPosition(skillBtn->getPositionX(), skillBtn->getPositionY());
    normal4->setScaleX(sGlobal->scaleX*0.6f);
    normal4->setScaleY(sGlobal->scaleY*0.6f);
    addChildLocked(normal4);
    
    wySprite *normal41 = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "btn_skill_mask.png");
    normal41->setAlpha(150);
    skillCD = wyProgressTimer::make(normal41);
    skillCD->setStyle(RADIAL_CCW);
    skillCD->setPosition(skillBtn->getPositionX(), skillBtn->getPositionY());
    skillCD->setPercentage(0);
    skillCD->setVisible(false);
    skillCD->setScaleX(sGlobal->scaleX*0.75f);
    skillCD->setScaleY(sGlobal->scaleY*0.75f);
    addChildLocked(skillCD);
        
    sGlobal->isOnGround = false;
    
    isJumping = false;
    elapsedTime = 0;
    coolDown = 0.0f;
    isClickedInCoolDown = false;
    isShooting = false;
    isLongPress = false;
    longPressTime = 0;
    isReleased = false;
    isToolInCoolDown = false;
    toolcoolDown = 0;
    
    wySpriteEx *hpBarLeft = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "xuecao.png");
//    wySpriteEx *hpBarLeft = wySpriteEx::make(MyTextureManager::makePNG("xuecao.png"));
    addChildLocked(hpBarLeft);
    hpBarLeft->setAnchor(0, 1.0f);
    hpBarLeft->setPosition(DP(0)*sGlobal->scaleX, wyDevice::winHeight - DP(0)*sGlobal->scaleY);
    hpBarLeft->setScaleY(0.7f*sGlobal->scaleY);
    hpBarLeft->setScaleX(0.7f*sGlobal->scaleX);
    
    wySpriteEx *hpBarRight = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "xuecao_bottom.png");
//    wySpriteEx *hpBarRight = wySpriteEx::make(MyTextureManager::makePNG("xuecao_bottom.png"));
    addChildLocked(hpBarRight);
    hpBarRight->setAnchor(0, 1.0f);
    hpBarRight->setPosition(hpBarLeft->getPositionX() + DP(92)*sGlobal->scaleX, hpBarLeft->getPositionY() - DP(40)*sGlobal->scaleY);
    hpBarRight->setScaleY(0.7f*sGlobal->scaleY);
    hpBarRight->setScaleX(0.7f*sGlobal->scaleX);

    float percentPerGrade = 0.06f;
    float scale = 0.6f + (sGlobal->skillArray[SKILL_STAMINA].getValue()-1)*0.025f;
    
    wySprite *aaa = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "xuecao_top.png");
    aaa->setColor(wyc3b(244, 209, 127));
    hpBar = wyProgressTimer::make(aaa);
    hpBar->setStyle(HORIZONTAL_BAR_LR);
    hpBar->setAnchor(0, 1.0f);
    hpBar->setPosition(hpBarLeft->getPositionX() + DP(92)*sGlobal->scaleX, hpBarLeft->getPositionY() - DP(40.0f)*sGlobal->scaleY);
    hpBar->setScaleY(0.7f*sGlobal->scaleY);
    hpBar->setScaleX(0.7f*sGlobal->scaleX*scale);
    addChildLocked(hpBar);
    
    hpBarRight->setScaleX(hpBar->getScaleX());
    
    aaa = wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "xuecao_top.png");
//    aaa->setColor(wyc4b(255, 255, 255, 200));
    aaa->setAlpha(240);
    aaa->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    
    hpBarLoss = wyProgressTimer::make(aaa);
    hpBarLoss->setStyle(HORIZONTAL_BAR_LR);
    hpBarLoss->setAnchor(0, 1.0f);
    hpBarLoss->setPosition(hpBarLeft->getPositionX() + DP(92)*sGlobal->scaleX, hpBarLeft->getPositionY() - DP(40.0f)*sGlobal->scaleY);
    hpBarLoss->setScaleY(0.7f*sGlobal->scaleY);
    hpBarLoss->setScaleX(0.7f*sGlobal->scaleX*scale);
    
    addChildLocked(hpBarLoss);
    hpBarLoss->setVisible(false);
    
    wySpriteEx *pause = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "zanting.png");
    pauseBtn = wyButton::make(pause, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(HudLayer::onPauseBtnClick)));
    pauseBtn->setScaleX(sGlobal->scaleX*0.6f);
    pauseBtn->setScaleY(sGlobal->scaleY*0.6f);
	pauseBtn->setPosition(wyDevice::winWidth, wyDevice::winHeight);
    pauseBtn->setMultiTouchClickable(false);
    pauseBtn->setAnchor(1.0f, 1.0f);
//    pauseBtn->setInterceptTouch(true);
    pauseBtn->setTouchPriority(101);
	addChildLocked(pauseBtn);
    
    switchBtn = wyButton::make(wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "switch_gun_border.png"), NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL_i(HudLayer::onSwitchBtnClick), 0));
    switchBtn->setScaleX(sGlobal->scaleX*0.65f);
    switchBtn->setScaleY(sGlobal->scaleY*0.65f);
	switchBtn->setPosition(wyDevice::winWidth-DP(150)*sGlobal->scaleX, wyDevice::winHeight-DP(42)*sGlobal->scaleY);
    switchBtn->setMultiTouchClickable(false);
    switchBtn->setTouchPriority(101);
    switchBtn->setContentSize(switchBtn->getContentSize().width*1.2, switchBtn->getContentSize().height*1.2);
	addChildLocked(switchBtn);
    
    switchGunIcon = sGlobal->getArmsSpriteByWeaponIndex(sGlobal->activeWeapon);
    switchGunIcon->setScaleX(sGlobal->scaleX*0.5f/0.8f);
    switchGunIcon->setScaleY(sGlobal->scaleY*0.5f/0.8f);
	switchGunIcon->setPosition(switchBtn->getPositionX(), switchBtn->getPositionY()-DP(0)*sGlobal->scaleY);
	addChildLocked(switchGunIcon);
    
    if (sGlobal->isRealEndless && (sGlobal->combatPeriod > 1 || sGlobal->combatRound > 1)) {
        
    } else {
        sGlobal->curHP = sGlobal->cloth_hp;
    }
    setHPBar(0);
    
    firecoolDown = 0.0f;
    isFireInCoolDown = false;
    
    //setGestureEnabled(true);
    movingStatus = 0;
    movingTimer = 0;
    
    isDialogShowing = false;
    
    isPaused = false;
    
    initNotice();
    
    isTouching = false;
    
    if (mapIndex == 0) {
        setHudStatus(GUIDE_0_1);
//		pauseBtn->setEnabled(false);
    } else {
        setHudStatus(GUIDE_NONE);
    }
    
    handPointer = NULL;
    
    char temp1[30];
    gold_title = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
    gold_title->setScaleX(sGlobal->scaleX*0.7f);
    gold_title->setScaleY(sGlobal->scaleY*0.7f);
    gold_title->setAnchor(1.0f, 0.5f);
	gold_title->setPosition(hpBarRight->getPositionX() + DP(20)*sGlobal->scaleX, wyDevice::winHeight - DP(75)*sGlobal->scaleY);
    addChildLocked(gold_title);
    
	addComma(sGlobal->getGold(), temp1);
    gold_number = wyBitmapFontLabel::make(sGlobal->font_desc, temp1);
    gold_number->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    gold_number->setAlignment(wyBitmapFontLabel::LEFT);
//    gold_number->setColor(goldColor);
    gold_number->setAnchor(0, 0.5f);
    gold_number->setPosition(gold_title->getPositionX() + DP(4)*sGlobal->scaleX, gold_title->getPositionY() + DP(0)*sGlobal->scaleX);
    addChildLocked(gold_number);
    
    cash_title = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "cash_icon.png");
    cash_title->setAnchor(1.0f, 0.5f);
    cash_title->setScaleX(sGlobal->scaleX*0.7f);
    cash_title->setScaleY(sGlobal->scaleY*0.7f);
	cash_title->setPosition(gold_title->getPositionX() + DP(100)*sGlobal->scaleX, gold_title->getPositionY() - DP(0)*sGlobal->scaleY);
    addChildLocked(cash_title);
    
	addComma(sGlobal->zuanshi.getValue(), temp1);
    cash_number = wyBitmapFontLabel::make(sGlobal->font_desc, temp1);
    cash_number->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    cash_number->setAlignment(wyBitmapFontLabel::LEFT);
//    magic_number->setColor(zuanshiColor);
    cash_number->setAnchor(0, 0.5f);
    cash_number->setPosition(cash_title->getPositionX() + DP(4)*sGlobal->scaleX, cash_title->getPositionY() + DP(0)*sGlobal->scaleY);
    addChildLocked(cash_number);
    
    ammo_title = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "bullet_icon.png");
    ammo_title->setAnchor(1.0f, 0.5f);
    ammo_title->setScaleX(sGlobal->scaleX*0.5f);
    ammo_title->setScaleY(sGlobal->scaleY*0.5f);
	ammo_title->setPosition(cash_title->getPositionX() + DP(80)*sGlobal->scaleX, cash_title->getPositionY() - DP(0)*sGlobal->scaleY);
    addChildLocked(ammo_title);
    
	addComma(sGlobal->weaponAmmoArray[sGlobal->activeWeapon], temp1);
    ammo_number = wyBitmapFontLabel::make(sGlobal->font_desc, temp1);
    ammo_number->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    ammo_number->setAlignment(wyBitmapFontLabel::LEFT);
//    ammo_number->setColor(zuanshiColor);
    ammo_number->setAnchor(0, 0.5f);
    ammo_number->setPosition(ammo_title->getPositionX() + DP(4)*sGlobal->scaleX, ammo_title->getPositionY() + DP(0)*sGlobal->scaleY);
    addChildLocked(ammo_number);
    
    updatePropPosition();
    
    if (sGlobal->isRealEndless) {
        addComma(123, temp1);
        char temp2[50];
        sprintf(temp2, "%sm", temp1);
        meter_number = wyBitmapFontLabel::make(sGlobal->font_desc, temp2);
        meter_number->setScale(SP(20)*sGlobal->scaleX/sGlobal->descFontSize);
        meter_number->setAlignment(wyBitmapFontLabel::LEFT);
//        meter_number->setColor(wyc3b(255, 255, 0));
        meter_number->setAnchor(1.0f, 1.0);
        meter_number->setPosition(wyDevice::winWidth - DP(10)*sGlobal->scaleX, pauseBtn->getPositionY() - DP(72)*sGlobal->scaleY);
        addChildLocked(meter_number);
    }
    
//    bool isBoss = false;
//    if (sGlobal->index == 110) {
//        initBossHpBar(0);
//    }
//    if (sGlobal->index == 210) {
//        initBossHpBar(1);
//    }
//    if (sGlobal->index == 310) {
//        initBossHpBar(2);
//    }
    
    shootCDNum = sGlobal->weapon_firecd[sGlobal->activeWeapon];
    if (sGlobal->hasSkillMulti[SKILL_FRENZY]) {
        shootCDNum *= (1.0f - sGlobal->skillMulti[SKILL_FRENZY]);
    }
//    if (sGlobal->activeWeapon == WEAPON_2) {
//        shootCDNum += 0.5f;
//    }
//    shootCDNum = 0.05f;
    shootCDNum *= sGlobal->SHOOT_CD_MULTI;
    
    skillCDNum = 15.0f;
    switch (sGlobal->activeSkill) {
        case SKILL_MISSLE:
            skillCDNum = sGlobal->SKILL_MISSLE_CD;
            break;
        case SKILL_ROBOT:
            skillCDNum = sGlobal->SKILL_ROBOT_CD;
            break;
        case SKILL_SHIELD:
            skillCDNum = sGlobal->skillMulti[SKILL_STUN+1];
            break;
        default:
            break;
    }
//    skillCDNum = 0.0f;
//    LOGE("shootCDNum:%f, skillCDNum:%f", shootCDNum, skillCDNum);
    
    normal2 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "firebtn.png");
    jumpBtn = wyButton::make(normal2, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(HudLayer::onJumpButtonDown)), wyTargetSelector::make(this, SEL(HudLayer::onJumpButtonUp)));
	jumpBtn->setPosition(DP(50)/0.75f*sGlobal->scaleX, DP(40)/0.75f*sGlobal->scaleY);
    jumpBtn->setMultiTouchClickable(true);
    jumpBtn->setInterceptTouch(true);
    jumpBtn->setScaleX(sGlobal->scaleX*0.7f);
    jumpBtn->setScaleY(sGlobal->scaleY*0.7f);
    jumpBtn->setAlpha(200);
    jumpBtn->setTouchPriority(101);
    jumpBtn->setMoveOutSelector(wyTargetSelector::make(this, SEL(HudLayer::onJumpButtonUp)));
    jumpBtn->setContentSize(jumpBtn->getContentSize().width*3, jumpBtn->getContentSize().height*3);
	addChildLocked(jumpBtn);

    jumpSprite = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "jump.png");
    jumpSprite->setPosition(jumpBtn->getPositionX(), jumpBtn->getPositionY());
    jumpSprite->setScaleX(sGlobal->scaleX*0.6f);
    jumpSprite->setScaleY(sGlobal->scaleY*0.6f);
    jumpSprite->setAlpha(200);
    addChildLocked(jumpSprite);
    
    needJump = false;
    needAutoFire = false;
    autoFireTimer = 0;
    
    isFired = false;
    hasCollectCoin = false;

    if (sGlobal->index == 0) {
        this->setTouchPriority(999);
        this->setInterceptTouch(false);
        setTouchEnabled(true);
        
        if (!sGlobal->isTutorialFromMenu) {
            pauseBtn->setEnabled(false);
        }
    } else {
        setTouchEnabled(false);
    }

    isTrainingToLv1 = false;
    
//    wyTargetSelector* ts = wyTargetSelector::make(this, SEL(HudLayer::update));
//    wyTimer* t = wyTimer::make(ts);
//    scheduleLocked(t);
    
    normal2 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "firebtn.png");
    downBtn = wyButton::make(normal2, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(HudLayer::onDownButtonDown)), NULL);
	downBtn->setPosition(DP(200)*sGlobal->scaleX, DP(40)*sGlobal->scaleY);
    downBtn->setMultiTouchClickable(true);
    downBtn->setInterceptTouch(true);
    downBtn->setScaleX(sGlobal->scaleX*0.5f);
    downBtn->setScaleY(sGlobal->scaleY*0.5f);
    downBtn->setAlpha(200);
    downBtn->setTouchPriority(101);
	addChildLocked(downBtn);
    
    wySpriteEx *downSprite = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "jump.png");
    downSprite->setPosition(downBtn->getPositionX(), downBtn->getPositionY());
    downSprite->setScaleX(sGlobal->scaleX*0.4f);
    downSprite->setScaleY(sGlobal->scaleY*0.4f);
    downSprite->setAlpha(200);
    downSprite->setFlipY(true);
    addChildLocked(downSprite);
}

HudLayer::~HudLayer() {

}

void HudLayer::onEnter() {
    wyLayer::onEnter();
}

void HudLayer::onDownButtonDown(wyTargetSelector* ts) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    Hero *hero = sGlobal->mainGameLayer->hero;
    if (sGlobal->isOnGround) {
        GroundSensor::make();
    }
}

void HudLayer::showBossHP() {
    bossContainer->runAction(wyMoveBy::make(0.3f, 0, DP(120)*sGlobal->scaleY));
}

static void onHideHPEnd(wyAction* action, void* data) {
    HudLayer* layer = (HudLayer*)data;
    layer->removeChildLocked(layer->bossContainer, true);
}

void HudLayer::hideBossHP() {
    wyIntervalAction *action = wyMoveBy::make(0.3f, 0, -DP(120)*sGlobal->scaleY);
    wyActionCallback callback = {
        NULL,
        onHideHPEnd,
        NULL
    };
    action->setCallback(&callback, this);
    bossContainer->runAction(action);
}

void HudLayer::setBossHPBar(float percent) {
//    bossBlood->setPercentage(percent*100);
}

void HudLayer::update(float dt) {
//    float dt = ts->getDelta();
    if (isClickedInCoolDown) {
        coolDown += dt;
        if (coolDown > shootCDNum) {
            isClickedInCoolDown = false;
        }
    }
    
    if (isFireInCoolDown) {
        firecoolDown += dt;
        skillCD->setPercentage(100 - firecoolDown / skillCDNum * 100);
        if (firecoolDown > skillCDNum) {
            isFireInCoolDown = false;
            skillCD->setVisible(false);
        }
    }
    
    if (isToolInCoolDown) {
        toolcoolDown += dt;
        toolCD->setPercentage(100 - toolcoolDown / 10.0f * 100);
        if (toolcoolDown > 10.0f) {
            isToolInCoolDown = false;
            toolCD->setVisible(false);
        }
    }
    
    b2Vec2 vel = sGlobal->mainGameLayer->hero->body->GetLinearVelocity();
    if (isJumping) {
        //elapsedTime += ts->getDelta();
        if (vel.y <= 0) {
            isJumping = false;
            sGlobal->mainGameLayer->m_box2d->getWorld()->ClearForces();
        }
    }
    
//    if (!sGlobal->isOnGround && vel.y < 0) {
//        sGlobal->mainGameLayer->hero->playHeroAnimation(HERO_JUMP_FALL);
//    }
    
    if (sGlobal->mainGameLayer->hero->isFlying && sGlobal->mainGameLayer->hero->container->getPositionY() > DP(HERO_YING_MAX_HEIGHT)*sGlobal->scaleY) {
        if (isTouching) {
            sGlobal->mainGameLayer->hero->body->SetLinearVelocity(b2Vec2(sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x, 0));
        } else {
            sGlobal->mainGameLayer->hero->body->SetLinearVelocity(b2Vec2(sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x, HERO_FLY_Y_DOWN));
        }
    }
    
//    if (handPointer != NULL) {
//        handPointer->tick(dt);
//    }
    
    if (needAutoFire) {
        autoFireTimer += dt;
        if (autoFireTimer >= shootCDNum) {
            sGlobal->mainGameLayer->hero->shoot();
            autoFireTimer = 0;
        }
    }
}

void HudLayer::onOKButton(wyTargetSelector* ts, void* data) {
    wyDialog* dialog = (wyDialog*)data;
    dialog->dismiss();
    sGlobal->mainGameLayer->hero->body->SetLinearVelocity(b2Vec2(10.0f, 0));
//    ((wySPXSprite*)sGlobal->mainGameLayer->hero->sprite_mao_tou)->setPaused(false);
//    ((wySPXSprite*)sGlobal->mainGameLayer->hero->sprite_mao_foot)->setPaused(false);
    sGlobal->mainGameLayer->hero->body->SetAwake(true);
    disableMe(true);
    isDialogShowing = false;
}

void HudLayer::onLeftButtonClick(wyTargetSelector* ts) {

}

void HudLayer::onRightButtonMoveOut(wyTargetSelector* ts) {
    if (canAutoFire(sGlobal->activeWeapon)) {
        needAutoFire = false;
        autoFireTimer = 0;
    }
}

void HudLayer::onRightButtonClick(wyTargetSelector* ts) {
    if (!canAtk) {
        return;
    }
    
    isFired = true;
    
    if (canAutoFire(sGlobal->activeWeapon)) {
        needAutoFire = true;
        autoFireTimer = 0;
        
        if (!isClickedInCoolDown || coolDown >= shootCDNum) {
            isClickedInCoolDown = true;
            sGlobal->mainGameLayer->hero->shoot();
            coolDown = 0;
        }
    } else {
        if (!isClickedInCoolDown || coolDown >= shootCDNum) {
            isClickedInCoolDown = true;
            sGlobal->mainGameLayer->hero->shoot();
            coolDown = 0;
        }
    }
}

void HudLayer::onRightButtonUp(wyTargetSelector* ts) {
    if (!canAtk) {
        return;
    }
    
    if (canAutoFire(sGlobal->activeWeapon)) {
        needAutoFire = false;
        autoFireTimer = 0;
    }
}

void HudLayer::onSkillButtonClick(wyTargetSelector* ts) {
    if (!canSkill) {
        return;
    }
    
    if (!isFireInCoolDown || firecoolDown >= skillCDNum) {
        isFireInCoolDown = true;
        sGlobal->mainGameLayer->hero->doSkill();
        firecoolDown = 0;
        skillCD->setPercentage(100);
        skillCD->setVisible(true);
    }
}

void HudLayer::onSkillButtonClick1(wyTargetSelector* ts) {
//    LOGE("skill clicked up..");
}

void HudLayer::onToolBtnClick(wyTargetSelector* ts) {
    if (!canTool) {
        return;
    }
    
//    if (status == GUIDE_11_1) {
//        sGlobal->mainGameLayer->hero->run();
//        hidePointer();
//        setHudStatus(GUIDE_11_2);
//    }
//    
//    if (!isToolInCoolDown || toolcoolDown >= 10.0f) {
//        isToolInCoolDown = true;
//        sGlobal->mainGameLayer->hero->doTool(sGlobal->activeTool);
//        toolcoolDown = 0;
//        toolCD->setPercentage(100);
//        toolCD->setVisible(true);
//        
//        int number = -1;
//        switch (sGlobal->activeTool) {
//            case TOOL_DABIAO_ID:
//                if (sGlobal->tool_dabiao_number > 0) {
//                    sGlobal->tool_dabiao_number -= 1;
//                    MyPrefs::setInt(DB_DABIAO, sGlobal->tool_dabiao_number);
//                    number = sGlobal->tool_dabiao_number;
//                }
//                break;
//            case TOOL_LAOYING_ID:
//                if (sGlobal->tool_laoying_number > 0) {
//                    sGlobal->tool_laoying_number -= 1;
//                    MyPrefs::setInt(DB_LAOYING, sGlobal->tool_laoying_number);
//                    number = sGlobal->tool_laoying_number;
//                }
//                break;
//            case TOOL_XUEPING_ID:
//                if (sGlobal->tool_xueping_number > 0) {
//                    sGlobal->tool_xueping_number -= 1;
//                    MyPrefs::setInt(DB_XUEPING, sGlobal->tool_xueping_number);
//                    number = sGlobal->tool_xueping_number;
//                }
//                break;
//            case TOOL_XIEZI_ID:
//                if (sGlobal->tool_xiezi_number > 0) {
//                    sGlobal->tool_xiezi_number -= 1;
//                    MyPrefs::setInt(DB_XIEZI, sGlobal->tool_xiezi_number);
//                    number = sGlobal->tool_xiezi_number;
//                }
//                break;
//            default:
//                break;
//        }
//        if (number != -1) {
//            char temp[10];
//            sprintf(temp, "x%d", number);
//            tool_number->setText(temp);
//        }
//        
//        if (number == 0) {
//            canTool = false;
//        }
//    }
}

void HudLayer::onPauseBtnClick(wyTargetSelector* ts) {
    if (!pauseBtn->isEnabled()) {
        return;
    }
    
    if (ts!=NULL) {
        sGlobal->playEffect(se_menu);
    }
    
    sGlobal->mainGameLayer->onExit();
    sGlobal->effectLayer->onExit();
    sGlobal->hudLayer->onExit();
    sGlobal->mainGameLayer->getParent()->addChild((wyLayer*) (new OptionLayer())->autoRelease(), 4, 4);
    ((MainScene *)sGlobal->mainGameLayer->getParent())->sceneState = SC_OPTION_OPENED;
}

void HudLayer::onSwitchBtnClick(wyTargetSelector* ts, int needAmmo) {
    if (!canSwitch) {
        return;
    }
    
    int initialSeq = sGlobal->WEAPON_ID2SEQ[sGlobal->activeWeapon];
    int nextSeq = sGlobal->WEAPON_ID2SEQ[sGlobal->activeWeapon];
    while (true) {
        nextSeq += 1;
        if (nextSeq > 15) {
            nextSeq = 0;
        }
        if (sGlobal->weaponUnlockArray[sGlobal->WEAPON_SEQ2ID[nextSeq]].getValue() > 0) {
            if (nextSeq == 0) {
                break;
            }
            if (needAmmo == 1) {
                if (sGlobal->weaponAmmoArray[sGlobal->WEAPON_SEQ2ID[nextSeq]] > 0) {
                    break;
                }
            } else {
                break;
            }
        }
    }

    if (nextSeq != initialSeq) {
//        initialSeq == sGlobal->WEAPON_ID2SEQ[WEAPON_M249] || initialSeq == sGlobal->WEAPON_ID2SEQ[WEAPON_F1] || initialSeq == sGlobal->WEAPON_ID2SEQ[WEAPON_E3]
        if (canAutoFire(sGlobal->WEAPON_SEQ2ID[initialSeq])) {
            needAutoFire = false;
            autoFireTimer = 0;
        }
    
        int nextId = sGlobal->WEAPON_SEQ2ID[nextSeq];
        
        switchGunIcon->getParent()->removeChildLocked(switchGunIcon, true);
        switchGunIcon = sGlobal->getArmsSpriteByWeaponIndex(nextId);
        switchGunIcon->setScaleX(sGlobal->scaleX*0.5f/0.8f);
        switchGunIcon->setScaleY(sGlobal->scaleY*0.5f/0.8f);
        switchGunIcon->setPosition(switchBtn->getPositionX(), switchBtn->getPositionY()-DP(0)*sGlobal->scaleY);
        addChildLocked(switchGunIcon);
        
        dart->getParent()->removeChildLocked(dart, true);
        dart = sGlobal->getArmsSpriteByWeaponIndex(nextId);
        dart->setPosition(atkBtn->getPositionX(), atkBtn->getPositionY());
        dart->setScaleX(sGlobal->scaleX*0.5f/0.8f);
        dart->setScaleY(sGlobal->scaleY*0.5f/0.8f);
        dart->setAlpha(200);
        addChildLocked(dart);
        
        shootCDNum = sGlobal->weapon_firecd[nextId];
        if (sGlobal->hasSkillMulti[SKILL_FRENZY]) {
            shootCDNum *= (1.0f - sGlobal->skillMulti[SKILL_FRENZY]);
        }

        char temp[50];
        addComma(sGlobal->weaponAmmoArray[nextId], temp);
        ammo_number->setText(temp);
        
        sGlobal->mainGameLayer->hero->switchWeapon(nextId);
    }
    
    if (sGlobal->index == 0) {
        switch (status) {
            case GUIDE_4_1:
                hidePointer();
                showPointer(atkBtn->getPositionX(), atkBtn->getPositionY(), true);
                showNotice(sGlobal->getIntroStr(STR_INTRO_RIFLE), 5, NULL, true);
                setHudStatus(GUIDE_4_2);
                break;
            default:
                return;
        }
    }
}

void HudLayer::onJumpButtonDown(wyTargetSelector* ts) {    
    if (!canJump)
        return;
    
    if (sGlobal->mainGameLayer->hero->isDead)
        return;
    
    if (isDialogShowing) {
        return;
    }
    
    if (sGlobal->mainGameLayer->hero->isFlying) {
        sGlobal->mainGameLayer->hero->body->SetLinearVelocity(b2Vec2(sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x, HERO_FLY_Y_UP));
        isTouching = true;
        sGlobal->mainGameLayer->hero->isStartFlyingEnd = true;
        sGlobal->mainGameLayer->hero->playHeroAnimation(SPX_HERO_R_LEG_JUMP2);
        
        sGlobal->playEffect(se_jetpack_jump);
        return;
    }
    
    if (sGlobal->mainGameLayer->hero->container->getPositionX() < DP(20)*sGlobal->scaleX) {
        return;
    }
    
    if (sGlobal->jumpTime == 2) {
        return;
    } else {
        if (sGlobal->jumpTime == 0) {
            sGlobal->mainGameLayer->hero->playHeroAnimation(SPX_HERO_R_LEG_JUMP1);
        } else {
            if (!canDbJump) {
                return;
            }
            sGlobal->mainGameLayer->hero->playHeroAnimation(SPX_HERO_R_LEG_DB_JUMP);
        }
        sGlobal->jumpTime += 1;
    }
    
    if (sGlobal->mainGameLayer->hero->needSetGravity) {
//        LOGE("needSetGravityasdf...");
        sGlobal->mainGameLayer->hero->body->SetGravityScale(1.0f);
        sGlobal->mainGameLayer->hero->needSetGravity = false;
    }
    b2Vec2 f = sGlobal->mainGameLayer->hero->body->GetWorldVector(b2Vec2(0.0f, JUMP_F_Y));
    b2Vec2 p = sGlobal->mainGameLayer->hero->body->GetWorldPoint(b2Vec2(0.0f, 0.0f));
    b2Vec2 vel = sGlobal->mainGameLayer->hero->body->GetLinearVelocity();
    sGlobal->mainGameLayer->hero->body->SetLinearVelocity(b2Vec2(vel.x, 0));
    sGlobal->mainGameLayer->hero->body->ApplyLinearImpulse(f, p);
    sGlobal->mainGameLayer->hero->body->ApplyForceToCenter(b2Vec2(0, JUMP_HOLD_F_Y));
    
    isJumping = true;
    elapsedTime = 0;
    isLongPress = false;
    isReleased = false;
    
//    if (sGlobal->mainGameLayer->hero->isFlyingSlowDown) {
//        sGlobal->mainGameLayer->hero->cancelFlyFromSlowDown();
//    }
    
    sGlobal->playEffect(se_jump);
    
    switch (status) {
        case GUIDE_1_1:
            sGlobal->mainGameLayer->hero->run();
            hidePointer();
            setHudStatus(GUIDE_1_2);
            break;
        case GUIDE_2_1:
            sGlobal->mainGameLayer->hero->run();
            hidePointer();
//            setHudStatus(GUIDE_2_3);
            break;
        case GUIDE_2_3:
//            sGlobal->mainGameLayer->hero->run();
            sGlobal->mainGameLayer->hero->playHeroAnimation(SPX_HERO_R_LEG_DB_JUMP);
            sGlobal->mainGameLayer->hero->isRunning = true;
            sGlobal->mainGameLayer->hero->body->SetGravityScale(1.0f);
            hidePointer();
            setHudStatus(GUIDE_2_3);
            break;
        default:
            return;
    }
}

void HudLayer::onJumpButtonUp(wyTargetSelector* ts) {
    if (sGlobal->mainGameLayer->hero->isDead)
        return;
    
    if (sGlobal->mainGameLayer->hero->isFlying) {
        sGlobal->mainGameLayer->hero->body->SetLinearVelocity(b2Vec2(sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x, HERO_FLY_Y_DOWN));
        isTouching = false;
        sGlobal->mainGameLayer->hero->isStartFlyingEnd = true;
        return;
    }
    
    isJumping = false;
    elapsedTime = 0;
    isReleased = true;
    sGlobal->mainGameLayer->m_box2d->getWorld()->ClearForces();
}

void HudLayer::onCharacterBtnClick(wyTargetSelector* ts) {

}

bool HudLayer::touchesBegan(wyMotionEvent& e) {
    if (isTrainingToLv1) {
        isTrainingToLv1 = false;
        
        sGlobal->mainScene->clearTutorialStatus();
        
        sGlobal->index = 101;
        sGlobal->currentSModeDifficulty = DIFFICULTY_NORMAL;
        
        sGlobal->mainGameLayer->onExit();
        sGlobal->effectLayer->onExit();
        sGlobal->hudLayer->onExit();
        
        sGlobal->gotoMainScene(sGlobal->index);
        
        return false;
    }
    
    switch (status) {
        case GUIDE_0_2:
            removeChildLocked(guidBg, true);
            setHudStatus(GUIDE_0_3);
            sGlobal->mainGameLayer->hero->run();
            sGlobal->mainGameLayer->needAutoScroll = true;
            return false;
//        case GUIDE_1_1:
//            sGlobal->mainGameLayer->hero->run();
//            hidePointer();
//            setHudStatus(GUIDE_1_2);
//            break;
//        case GUIDE_2_1:
//            sGlobal->mainGameLayer->hero->run();
//            hidePointer();
//            setHudStatus(GUIDE_2_2);
//            break;
        case GUIDE_6_1:
            sGlobal->mainGameLayer->hero->run();
            sGlobal->mainGameLayer->hero->isFlyingPaused = false;
            setHudStatus(GUIDE_6_2);
            break;
        case GUIDE_10_1:
            sGlobal->mainGameLayer->hero->run();
            setHudStatus(GUIDE_NONE);
            return false;
        case GUIDE_11_1:
            sGlobal->mainGameLayer->hero->run();
            setHudStatus(GUIDE_NONE);
            return false;
        case GUIDE_12_1:
            sGlobal->mainGameLayer->hero->run();
            setHudStatus(GUIDE_NONE);
            return false;
        default:
            return false;
    }
    
    return false;
}

bool HudLayer::touchesMoved(wyMotionEvent& e) {
	return false;
}

bool HudLayer::touchesEnded(wyMotionEvent& e) {
    return false;
}

bool HudLayer::touchesCancelled(wyMotionEvent& e) {
    return false;
}

bool HudLayer::touchesPointerCancelled(wyMotionEvent& e) {
    return false;
}

bool HudLayer::touchesPointerBegan(wyMotionEvent& e) {
    if (isTrainingToLv1) {
        isTrainingToLv1 = false;
        
        sGlobal->mainScene->clearTutorialStatus();
        
        sGlobal->index = 101;
        sGlobal->currentSModeDifficulty = DIFFICULTY_NORMAL;
        
        sGlobal->mainGameLayer->onExit();
        sGlobal->effectLayer->onExit();
        sGlobal->hudLayer->onExit();
        
        sGlobal->gotoMainScene(sGlobal->index);
        
        return false;
    }
    
    switch (status) {
        case GUIDE_0_2:
            removeChildLocked(guidBg, true);
            setHudStatus(GUIDE_0_3);
            sGlobal->mainGameLayer->hero->run();
            sGlobal->mainGameLayer->needAutoScroll = true;
            return false;
            //        case GUIDE_1_1:
            //            sGlobal->mainGameLayer->hero->run();
            //            hidePointer();
            //            setHudStatus(GUIDE_1_2);
            //            break;
            //        case GUIDE_2_1:
            //            sGlobal->mainGameLayer->hero->run();
            //            hidePointer();
            //            setHudStatus(GUIDE_2_2);
            //            break;
        case GUIDE_6_1:
            sGlobal->mainGameLayer->hero->run();
            sGlobal->mainGameLayer->hero->isFlyingPaused = false;
            setHudStatus(GUIDE_6_2);
            break;
        case GUIDE_10_1:
            sGlobal->mainGameLayer->hero->run();
            setHudStatus(GUIDE_NONE);
            return false;
        case GUIDE_11_1:
            sGlobal->mainGameLayer->hero->run();
            setHudStatus(GUIDE_NONE);
            return false;
        case GUIDE_12_1:
            sGlobal->mainGameLayer->hero->run();
            setHudStatus(GUIDE_NONE);
            return false;
        default:
            return false;
    }
    
    return false;
}

bool HudLayer::touchesPointerEnded(wyMotionEvent& e) {
    return false;
}

void HudLayer::setHPBar(int lostHp) {
    float prev = hpBar->getPercentage();

    sGlobal->curHP -= lostHp;
    sGlobal->curHP = MIN(sGlobal->curHP, sGlobal->cloth_hp);
    hpBar->setPercentage(MAX(0, sGlobal->curHP) / (float)sGlobal->cloth_hp * 100);

    float next = hpBar->getPercentage();
    if (lostHp > 0) {
        hpBarLoss->setVisible(true);
        hpBarLoss->setPosition(hpBar->getPositionX()+hpBar->getWidth()*hpBar->getScaleX()*(next/100.0f), hpBarLoss->getPositionY());
        hpBarLoss->runAction(wyProgressTo::make(0.8f, prev-next, 0));
    }
//    LOGE("total hp:%d, curhp:%d", sGlobal->cloth_hp, sGlobal->curHP);
}

void HudLayer::showNotice(const char *noticeStr, int duration, const char *noticeStr1, bool isSwitchGun) {
    notice_bg->setVisible(true);
    notice_txt->setVisible(true);
    notice_txt->setText(noticeStr);
    
    if (introTimer != NULL) {
        unscheduleLocked(introTimer);
        introTimer = NULL;
    }
    if (duration != -1) {
        introTimer = wyTimer::make(wyTargetSelector::make(this, SEL(HudLayer::hideNotice)), duration+1.0f);
        introTimer->setOneShot(true);
        scheduleLocked(introTimer);
    }
    
    if (isSwitchGun) {
        notice_bg->setPosition(0, wyDevice::winHeight/1.2f-DP(75)*sGlobal->scaleY);
        notice_txt->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/1.2f-DP(41)*sGlobal->scaleY);
    } else {
        notice_bg->setPosition(0, wyDevice::winHeight/1.2f-DP(24)*sGlobal->scaleY);
        notice_txt->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/1.2f+DP(10)*sGlobal->scaleY);
    }
    
    if (noticeStr1 != NULL) {
        notice_txt1->setText(noticeStr1);
        notice_txt1->setVisible(true);
        float w1 = notice_txt->getWidth()*notice_txt->getScale();
        float w2 = notice_txt1->getWidth()*notice_txt1->getScale();
        notice_txt->setPosition(wyDevice::winWidth/2-w2/2-DP(3)*sGlobal->scaleX, notice_txt->getPositionY());
        notice_txt1->setPosition(wyDevice::winWidth/2+w1/2+DP(3)*sGlobal->scaleX, notice_txt->getPositionY());
    } else {
        notice_txt1->setVisible(false);
        notice_txt->setPosition(wyDevice::winWidth/2, notice_txt->getPositionY());
    }
}

void HudLayer::showIntro(Intro *intro) {
    const char *noticeStr = intro->name;
    if (strcmp(noticeStr, "intro_0") == 0) {
        sGlobal->mainGameLayer->hero->stop();
        sGlobal->mainGameLayer->needAutoScroll = false;
        show1stGuide();
    }
    else if (strcmp(noticeStr, "intro_1_1") == 0) {
        sGlobal->mainGameLayer->hero->pause();
        showPointer(jumpBtn->getPositionX(), jumpBtn->getPositionY(), true);
        showNotice(sGlobal->getIntroStr(STR_INTRO_1_1_1), 10, sGlobal->getIntroStr(STR_INTRO_1_1_2));
        setHudStatus(GUIDE_1_1);
        sGlobal->checkPointX = intro->container->getPositionX() - DP(100)*sGlobal->scaleX;
        sGlobal->checkPointY = intro->container->getPositionY() + DP(0)*sGlobal->scaleX;
    }
    else if (strcmp(noticeStr, "intro_1_2") == 0) {
        showNotice(sGlobal->getIntroStr(STR_INTRO_1_2));
    }
    else if (strcmp(noticeStr, "intro_2_1") == 0) {
        sGlobal->mainGameLayer->hero->pause();
        showPointer(jumpBtn->getPositionX(), jumpBtn->getPositionY(), true);
        showNotice(sGlobal->getIntroStr(STR_INTRO_2_1));
        setHudStatus(GUIDE_2_1);
        sGlobal->checkPointX = intro->container->getPositionX() - DP(100)*sGlobal->scaleX;
        sGlobal->checkPointY = intro->container->getPositionY() + DP(0)*sGlobal->scaleX;
    }
    else if (strcmp(noticeStr, "intro_2_2") == 0) {
        showNotice(sGlobal->getIntroStr(STR_INTRO_2_3));
    }
    else if (strcmp(noticeStr, "intro_2_3") == 0) {
        sGlobal->mainGameLayer->hero->pause();
        sGlobal->mainGameLayer->hero->body->SetGravityScale(0);
        showPointer(jumpBtn->getPositionX(), jumpBtn->getPositionY(), true, true);
        showNotice(sGlobal->getIntroStr(STR_INTRO_2_2));
        setHudOp(false, false, false, false, false);
        sGlobal->checkPointX = intro->container->getPositionX() - DP(200)*sGlobal->scaleX;
        sGlobal->checkPointY = intro->container->getPositionY() + DP(0)*sGlobal->scaleX;
    }
    else if (strcmp(noticeStr, "intro_3_1") == 0) {
        sGlobal->mainGameLayer->hero->pause();
        showPointer(atkBtn->getPositionX(), atkBtn->getPositionY(), false);
        showNotice(sGlobal->getIntroStr(STR_INTRO_3_1));
        setHudStatus(GUIDE_3_1);
        sGlobal->checkPointX = intro->container->getPositionX() - DP(100)*sGlobal->scaleX;
        sGlobal->checkPointY = intro->container->getPositionY() + DP(0)*sGlobal->scaleX;
    }
    else if (strcmp(noticeStr, "intro_3_2") == 0) {
        sGlobal->mainGameLayer->hero->pause();
        showPointer(skillBtn->getPositionX(), skillBtn->getPositionY(), false);
        showNotice(sGlobal->getIntroStr(STR_INTRO_4_1));
        setHudStatus(GUIDE_3_3);
        sGlobal->checkPointX = intro->container->getPositionX() - DP(100)*sGlobal->scaleX;
        sGlobal->checkPointY = intro->container->getPositionY() + DP(0)*sGlobal->scaleX;
    }
    else if (strcmp(noticeStr, "intro_switch_gun") == 0) {
        sGlobal->mainGameLayer->hero->pause();
        showPointer(switchGunIcon->getPositionX(), switchGunIcon->getPositionY(), false, false, true);
        showNotice(sGlobal->getIntroStr(STR_INTRO_SWITCH_GUN), 5, NULL, true);
        setHudStatus(GUIDE_4_1);
        sGlobal->checkPointX = intro->container->getPositionX() - DP(100)*sGlobal->scaleX;
        sGlobal->checkPointY = intro->container->getPositionY() + DP(0)*sGlobal->scaleX;
    }
    else if (strcmp(noticeStr, "intro_xie") == 0) {
        if (sGlobal->isXieZiTrained == 1) {
            return;
        }
        sGlobal->mainGameLayer->hero->pause();
        showNotice(sGlobal->getIntroStr(STR_INTRO_MOTO), 5);
        setHudStatus(GUIDE_10_1);
        sGlobal->isXieZiTrained = 1;
        MyPrefs::setInt(DB_IS_XIEZI_TRAINED, 1);
    }
    else if (strcmp(noticeStr, "intro_biao") == 0) {
        if (sGlobal->isDaBiaoTrained == 1) {
            return;
        }
        sGlobal->mainGameLayer->hero->pause();
        showNotice(sGlobal->getIntroStr(STR_INTRO_MISSLE), 5);
        setHudStatus(GUIDE_11_1);
        
        sGlobal->isDaBiaoTrained = 1;
        MyPrefs::setInt(DB_IS_DABIAO_TRAINED, 1);
    }
    else if (strcmp(noticeStr, "intro_ying") == 0) {
        if (sGlobal->isLaoYingTrained == 1) {
            return;
        }
        sGlobal->mainGameLayer->hero->pause();
        showNotice(sGlobal->getIntroStr(STR_INTRO_JETPACK), 5);
        setHudStatus(GUIDE_12_1);
        sGlobal->isLaoYingTrained = 1;
        MyPrefs::setInt(DB_IS_LAOYING_TRAINED, 1);
    }
}


void HudLayer::initNotice() {
    notice_bg = wyColorLayer::make();
    notice_bg->setColor(wyc4b(0, 0, 0, 200));
    notice_bg->setContentSize(wyDevice::winWidth, DP(50)/0.75f*sGlobal->scaleY);
    notice_bg->setPosition(0, wyDevice::winHeight/1.2f-DP(24)*sGlobal->scaleY);
    addChildLocked(notice_bg, 200);
    notice_bg->setVisible(false);
    
    notice_txt = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    notice_txt->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    notice_txt->setAlignment(wyBitmapFontLabel::CENTER);
    notice_txt->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/1.2f+DP(10)*sGlobal->scaleY);
    addChildLocked(notice_txt, 200);
    notice_txt->setVisible(false);
    
    notice_txt1 = wyBitmapFontLabel::make(sGlobal->font_desc, "a");
    notice_txt1->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    notice_txt1->setAlignment(wyBitmapFontLabel::CENTER);
    notice_txt1->setPosition(wyDevice::winWidth/2, wyDevice::winHeight/1.2f+DP(10)*sGlobal->scaleY);
    addChildLocked(notice_txt1, 200);
    notice_txt1->setVisible(false);
    notice_txt1->setColor(wyc3b(253, 86, 30));
    
    introTimer = NULL;
}

void HudLayer::hideNotice(wyTargetSelector *ts) {
    notice_txt->setVisible(false);
    notice_bg->setVisible(false);
    notice_txt1->setVisible(false);
}

void HudLayer::disableMe(bool flag) {
    atkBtn->setEnabled(flag);
    skillBtn->setEnabled(flag);
    toolBtn->setEnabled(flag);
}

void HudLayer::reset() {
    
}

void HudLayer::showCountinueBtn() {
//    wySprite *countinue = wySprite::make(MyTextureManager::makePNG("btn_go.png"));
//    continueBtn = wyButton::make(countinue, NULL, NULL, NULL, NULL, wyTargetSelector::make(this, SEL(HudLayer::onCountinueBtnClick)));
//    continueBtn->setScaleX(sGlobal->scaleX);
//    continueBtn->setScaleY(sGlobal->scaleY);
//	continueBtn->setPosition(wyDevice::winWidth - DP(40)*sGlobal->scaleX, wyDevice::winHeight/2);
//    continueBtn->setMultiTouchClickable(false);
//    continueBtn->setInterceptTouch(true);
//	addChildLocked(continueBtn);
}

void HudLayer::onCountinueBtnClick(wyTargetSelector* ts) {
    sGlobal->mainGameLayer->hero->run();
    removeChildLocked(continueBtn, true);
}

void HudLayer::showPointer(int x, int y, bool isFlipX, bool isDelay, bool isSwitchGun) {
    handPointer = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "handpointer.png");
    handPointer->setScaleX(sGlobal->scaleX*0.8f);
    handPointer->setScaleY(sGlobal->scaleY*0.8f);
    handPointer->setAnchor(0.5f, 0.0f);
    addChildLocked(handPointer, 1000);
    
    if (isSwitchGun) {
        handPointer->setRotation(-90);
        handPointer->setPosition(switchGunIcon->getPositionX()+DP(0)*sGlobal->scaleX, switchGunIcon->getPositionY()-DP(0)*sGlobal->scaleY);
        
        handPointer->runAction(wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.4f, DP(-40)*sGlobal->scaleY, 0), wyMoveBy::make(0.4f, DP(40)*sGlobal->scaleY, 0), NULL)));
    } else {
        handPointer->setPosition(x+DP(0)*sGlobal->scaleX, y-DP(0)*sGlobal->scaleY);
        handPointer->runAction(wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.4f, 0, DP(30)*sGlobal->scaleY), wyMoveBy::make(0.4f, 0, -DP(30)*sGlobal->scaleY), NULL)));
        
        if (isDelay) {
            handPointer->setVisible(false);
            handPointer->runAction(wySequence::make(wyDelayTime::make(0.5f), wyCallFunc::make(wyTargetSelector::make(this, SEL(HudLayer::onHandPointerDelayEnd))), NULL));
        }
    }
}

void HudLayer::onHandPointerDelayEnd(wyTargetSelector* ts){
    handPointer->setVisible(true);
    setHudStatus(GUIDE_2_3);
}

void HudLayer::hidePointer() {
    if (handPointer != NULL) {
        removeChildLocked(handPointer, true);
        handPointer = NULL;
    }
}

void HudLayer::show1stGuide() {
//    guidBg = wyColorLayer::make();
//    guidBg->setColor(wyc4b(0, 0, 0, 100));
//    addChildLocked(guidBg, -1);
//    
//    wySprite *dangqianxueliang = wyZwoptexManager::getInstance()->makeSprite(sGlobal->uiName, "guide_dangqianxueliang.png");
//    dangqianxueliang->setPosition(DP(240)*sGlobal->scaleX, wyDevice::winHeight - DP(85)*sGlobal->scaleY);
//    dangqianxueliang->setScaleX(sGlobal->scaleX);
//    dangqianxueliang->setScaleY(sGlobal->scaleY);
//    guidBg->addChildLocked(dangqianxueliang);
//    
//    wySprite *zantinganniu = wyZwoptexManager::getInstance()->makeSprite(sGlobal->uiName, "guide_zantinganniu.png");
//    zantinganniu->setPosition(wyDevice::winWidth - DP(100)*sGlobal->scaleX, wyDevice::winHeight - DP(96)*sGlobal->scaleY);
//    zantinganniu->setScaleX(sGlobal->scaleX);
//    zantinganniu->setScaleY(sGlobal->scaleY);
//    guidBg->addChildLocked(zantinganniu);
//    
//    wySprite *zuocetiaoyue = wyZwoptexManager::getInstance()->makeSprite(sGlobal->uiName, "guide_zuocetiaoyue.png");
//    zuocetiaoyue->setPosition(DP(150)*sGlobal->scaleX, wyDevice::winHeight/2);
//    zuocetiaoyue->setScaleX(sGlobal->scaleX);
//    zuocetiaoyue->setScaleY(sGlobal->scaleY);
//    guidBg->addChildLocked(zuocetiaoyue);
//    
//    wySprite *shiyongdaoju = wyZwoptexManager::getInstance()->makeSprite(sGlobal->uiName, "guide_shiyongdaoju.png");
//    shiyongdaoju->setPosition(wyDevice::winWidth - DP(288)*sGlobal->scaleX, DP(125)*sGlobal->scaleY);
//    shiyongdaoju->setScaleX(sGlobal->scaleX);
//    shiyongdaoju->setScaleY(sGlobal->scaleY);
//    guidBg->addChildLocked(shiyongdaoju);
//    
//    wySprite *shifangjineng = wyZwoptexManager::getInstance()->makeSprite(sGlobal->uiName, "guide_shifangjineng.png");
//    shifangjineng->setPosition(wyDevice::winWidth - DP(174)*sGlobal->scaleX, DP(125)*sGlobal->scaleY);
//    shifangjineng->setScaleX(sGlobal->scaleX);
//    shifangjineng->setScaleY(sGlobal->scaleY);
//    guidBg->addChildLocked(shifangjineng);
//    
//    wySprite *putonggongji = wyZwoptexManager::getInstance()->makeSprite(sGlobal->uiName, "guide_putonggongji.png");
//    putonggongji->setPosition(wyDevice::winWidth - DP(60)*sGlobal->scaleX, DP(125)*sGlobal->scaleY);
//    putonggongji->setScaleX(sGlobal->scaleX);
//    putonggongji->setScaleY(sGlobal->scaleY);
//    guidBg->addChildLocked(putonggongji);
//    
//    setHudStatus(GUIDE_0_2);
}

void HudLayer::setHudStatus(int status) {
    this->status = status;
    switch (status) {
        case GUIDE_0_1:
        case GUIDE_0_2:
        case GUIDE_0_3:
            setHudOp(false, false, false, false, false, false);
            break;
        case GUIDE_1_1:
            setHudOp(true, false, false, false, false, false);
            break;
        case GUIDE_1_2:
            setHudOp(true, false, false, false, false, false);
            break;
        case GUIDE_2_1:
            setHudOp(true, false, false, false, false, false);
            break;
        case GUIDE_2_3:
            setHudOp(true, true, false, false, false, false);
            break;
        case GUIDE_3_1:
            setHudOp(true, true, false, false, true, false);
            break;
        case GUIDE_3_2:
            setHudOp(true, true, false, false, false, false);
            break;
        case GUIDE_3_3:
            setHudOp(true, true, false, true, false, false);
            break;
        case GUIDE_3_4:
            setHudOp(true, true, false, true, false, false);
            break;
        case GUIDE_4_1:
            setHudOp(true, true, true, false, false, true);
            break;
        case GUIDE_4_2:
            setHudOp(true, true, true, false, true, false);
            break;
        case GUIDE_5_1:
            setHudOp(false, false, false, false, false, false);
            break;
        case GUIDE_NONE:
            setHudOp(true, true, true, true, true, true);
            setTouchEnabled(false);
            break;
        case GUIDE_6_1:
//            showNotice(sGlobal->getStr(STR_ANZHUZUOCEPINGMUFLY), 5);
            break;
        case GUIDE_10_1:
            setHudOp(false, false, false, false, false);
            setTouchPriority(200);
            setTouchEnabled(true);
            break;
        case GUIDE_11_1:
            setHudOp(false, false, false, false, false);
            setTouchPriority(200);
            setTouchEnabled(true);
//            setTouchPriority(200);
            break;
        case GUIDE_12_1:
            setHudOp(false, false, false, false, false);
            setTouchPriority(200);
            setTouchEnabled(true);
//            setTouchPriority(200);
            break;
        default:
            break;
    }
}

void HudLayer::setHudOp(bool canJump, bool canDbJump, bool canTool, bool canSkill, bool canAtk, bool canSwitch) {
    this->canJump = canJump;
    this->canDbJump = canDbJump;
    
//    int number = 0;
//    switch (sGlobal->activeTool) {
//        case TOOL_DABIAO_ID:
//            number = sGlobal->tool_dabiao_number;
//            break;
//        case TOOL_LAOYING_ID:
//            number = sGlobal->tool_laoying_number;
//            break;
//        case TOOL_XUEPING_ID:
//            number = sGlobal->tool_xueping_number;
//            break;
//        case TOOL_XIEZI_ID:
//            number = sGlobal->tool_xiezi_number;
//            break;
//        default:
//            break;
//    }
//    if (number == 0 && sGlobal->index != 0) {
//        this->canTool = false;
//    } else {
//        this->canTool = canTool;
//    }
    
    this->canSkill = canSkill;
    this->canAtk = canAtk;
    
    if (!canAtk) {
        needAutoFire = false;
        autoFireTimer = 0;
    }
    
    this->canSwitch = canSwitch;
}

void HudLayer::updateGuide() {
    switch (status) {
        case GUIDE_3_1:
            setHudStatus(GUIDE_3_2);
            hidePointer();
            sGlobal->mainGameLayer->hero->run();
            showNotice(sGlobal->getIntroStr(STR_INTRO_3_2));
            break;
        case GUIDE_3_3:
            setHudStatus(GUIDE_3_4);
            hidePointer();
            sGlobal->mainGameLayer->hero->run();
            showNotice(sGlobal->getIntroStr(STR_INTRO_4_2));
            break;
        case GUIDE_4_2:
            setHudStatus(GUIDE_4_3);
            hidePointer();
            sGlobal->mainGameLayer->hero->run();
            showNotice(sGlobal->getIntroStr(STR_INTRO_3_2));
            break;
        default:
            break;
    }
}

void HudLayer::addGold(int gold) {
    sGlobal->offsetGold(gold);

    char temp[30];
	addComma(sGlobal->getGold(), temp);
    gold_number->setText(temp);
    
    updatePropPosition();
}

void HudLayer::addZuanshi(int zuanshi) {
    sGlobal->zuanshi.offset(zuanshi);

    char temp[30];
	addComma(sGlobal->zuanshi.getValue(), temp);
    cash_number->setText(temp);
    
    updatePropPosition();
}

void HudLayer::addZuanshiFake(int zuanshi) {
    
    char temp[30];
	addComma(sGlobal->zuanshi.getValue()+zuanshi, temp);
    cash_number->setText(temp);
    
    updatePropPosition();
}

void HudLayer::addAmmo(int ammoNum) {
    sGlobal->weaponAmmoArray[sGlobal->activeWeapon] -= ammoNum;
    
    char temp[30];
	addComma(sGlobal->weaponAmmoArray[sGlobal->activeWeapon], temp);
    ammo_number->setText(temp);
}

void HudLayer::updatePropPosition() {
    cash_title->setPosition(gold_number->getPositionX()+gold_number->getWidth()*gold_number->getScaleX()+DP(40)*sGlobal->scaleX, gold_number->getPositionY());
    cash_number->setPosition(cash_title->getPositionX() + DP(4)*sGlobal->scaleX, cash_title->getPositionY() + DP(0)*sGlobal->scaleY);
    
    ammo_title->setPosition(cash_number->getPositionX()+cash_number->getWidth()*cash_number->getScaleX()+DP(40)*sGlobal->scaleX, cash_number->getPositionY());
    ammo_number->setPosition(ammo_title->getPositionX() + DP(4)*sGlobal->scaleX, ammo_title->getPositionY() + DP(0)*sGlobal->scaleY);
}

static void onAnimEnd(wyAction* action, void* data) {
    wyNode *node = (wyNode *)data;
    node->getParent()->removeChildLocked(node, true);
}

void HudLayer::runGoldDecAnim(int goldNum) {
    wyNode *animContainer = wyNode::make();
    addChildLocked(animContainer);
    
    wySpriteEx *title = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
    title->setScaleX(sGlobal->scaleX*0.7f);
    title->setScaleY(sGlobal->scaleY*0.7f);
    title->setAnchor(1.0f, 0.5f);
	title->setPosition(gold_title->getPositionX(), gold_title->getPositionY());
    animContainer->addChildLocked(title);
    
    char temp[20];
    sprintf(temp, "-%d", goldNum);
    wyBitmapFontLabel *number = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
    number->setScale(SP(16)*sGlobal->scaleX/sGlobal->descFontSize);
    number->setAlignment(wyBitmapFontLabel::LEFT);
    //    gold_number->setColor(goldColor);
    number->setAnchor(0, 0.5f);
    number->setPosition(gold_number->getPositionX(), gold_number->getPositionY());
    animContainer->addChildLocked(number);
    
    wyIntervalAction *a = wyFadeOut::make(1.0f);
    wyActionCallback callback = {
        NULL,
        onAnimEnd,
        NULL
    };
    a->setCallback(&callback, animContainer);
    
    animContainer->runAction(wyMoveBy::make(1.0f, 0, DP(-30)*sGlobal->scaleY));
    animContainer->runAction(a);
}

void HudLayer::runAmmoIncAnim(int ammoNum) {
    ammo_title->runAction(wySequence::make(wyScaleTo::make(0.1f, sGlobal->scaleX*0.5f, sGlobal->scaleX*0.5f, sGlobal->scaleX*0.7f, sGlobal->scaleX*0.7f), wyScaleTo::make(0.1f, sGlobal->scaleX*0.7f, sGlobal->scaleX*0.7f, sGlobal->scaleX*0.5f, sGlobal->scaleX*0.5f), NULL));
    float scale = SP(16)*sGlobal->scaleX/sGlobal->descFontSize;
    ammo_number->runAction(wySequence::make(wyScaleTo::make(0.1f, scale, scale, scale+sGlobal->scaleX*0.2f, scale+sGlobal->scaleX*0.2f), wyScaleTo::make(0.1f, scale+sGlobal->scaleX*0.2f, scale+sGlobal->scaleX*0.2f, scale, scale), NULL));
}

void HudLayer::runOutofResAnim() {
    wyIntervalAction *tint = wyTintTo::make(0.1f, 255, 255, 255, 255, 0, 0);
    gold_number->runAction(wySequence::make(tint, (wyIntervalAction*)tint->reverse(), tint->copy(), (wyIntervalAction*)tint->reverse(), tint->copy(), (wyIntervalAction*)tint->reverse(), NULL));
    
    ammo_number->runAction(wySequence::make((wyIntervalAction*)tint->copy(), (wyIntervalAction*)tint->reverse(), tint->copy(), (wyIntervalAction*)tint->reverse(), tint->copy(), (wyIntervalAction*)tint->reverse(), NULL));
}

void HudLayer::updateTool() {
//    char temp2[30];
//    int number = 0;
//    switch (sGlobal->activeTool) {
//        case TOOL_DABIAO_ID:
//            number = sGlobal->tool_dabiao_number;
//            break;
//        case TOOL_LAOYING_ID:
//            number = sGlobal->tool_laoying_number;
//            break;
//        case TOOL_XUEPING_ID:
//            number = sGlobal->tool_xueping_number;
//            break;
//        case TOOL_XIEZI_ID:
//            number = sGlobal->tool_xiezi_number;
//            break;
//        default:
//            break;
//    }
//    sprintf(temp2, "x%d", number);
//    tool_number->setText(temp2);
////    LOGE("item number:%d", number);
//    //道具数量为0,使用不了。
//    if (number == 0) {
//        canTool = false;
//    }
}

void HudLayer::updateMeter() {
    if (sGlobal->isRealEndless) {
        int x = MAX(sGlobal->mainGameLayer->hero->body->GetPosition().x, 0);
        sGlobal->currentDistance = x + sGlobal->accDistance;
        
        char temp1[30];
        addComma(sGlobal->currentDistance, temp1);
        char temp2[50];
        sprintf(temp2, "%sm", temp1);
        meter_number->setText(temp2);
        
        if (sGlobal->isRealEndless) {
            for (int i = 0; i < 5; i++) {
                int achvNo = ACHV_RUN_400_ONE+i;
                if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
//                    if (achvNo == ACHV_RUN_3200_ONE && sGlobal->currentEModeDifficulty == DIFFICULTY_NORMAL) {
//                        continue;
//                    }
//                    if (achvNo == ACHV_RUN_4800_ONE && sGlobal->currentEModeDifficulty != DIFFICULTY_HARD) {
//                        continue;
//                    }
                    
                    int targetDistance = sGlobal->currentDistance;
                    sGlobal->achv_progress_temp[achvNo] = targetDistance;
                    if (targetDistance >= sGlobal->ACHV_NUM[achvNo*3]) {
                        sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                        sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                        
                        sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
                    }
                }
            }
            
            for (int i = 0; i < 5; i++) {
                int achvNo = ACHV_RUN_1000+i;
                if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
//                    if (achvNo == ACHV_RUN_50000 && sGlobal->currentEModeDifficulty == DIFFICULTY_NORMAL) {
//                        continue;
//                    }
//                    if (achvNo == ACHV_RUN_100000 && sGlobal->currentEModeDifficulty != DIFFICULTY_HARD) {
//                        continue;
//                    }
                    
                    int targetDistance = sGlobal->runDistance + sGlobal->currentDistance;
//                    LOGE("targetDistance:%d, runDistance:%d, currentDistance:%d, achv:%d", targetDistance, sGlobal->runDistance, sGlobal->currentDistance, sGlobal->ACHV_NUM[achvNo*3]);
                    sGlobal->achv_progress[achvNo] = sGlobal->runDistance + sGlobal->currentDistance;
                    if (targetDistance >= sGlobal->ACHV_NUM[achvNo*3]) {
                        sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                        sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                        
                        sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
                        
                        sGlobal->runDistance = targetDistance;
                        MyPrefs::setInt(DB_RUN_DISTANCE, sGlobal->runDistance);
                    }
                }
            }
            
            int achvNo = ACHV_RUN_300_NOSHOOT;
            //    LOGE("achvNo:%d, status:%d, isFired:%s", achvNo, sGlobal->achv_rec[achvNo], isFired ? "true" : "false");
            if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE && !isFired) {
                int targetDistance = sGlobal->currentDistance;
                sGlobal->achv_progress_temp[achvNo] = targetDistance;
                if (targetDistance >= sGlobal->ACHV_NUM[achvNo*3]) {
                    sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                    sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                    
                    sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
                }
            }
            
            achvNo = ACHV_RUN_100_NOCOIN;
            if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE && !hasCollectCoin) {
                int targetDistance = sGlobal->currentDistance;
                sGlobal->achv_progress_temp[achvNo] = targetDistance;
                if (targetDistance >= sGlobal->ACHV_NUM[achvNo*3]) {
                    sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                    sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                    
                    sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
                }
            }
        }
    }
}

void HudLayer::initBossHpBar(int index) {
    bool isBoss = true;
//    switch (index) {
//        case 0:
//            bossHead = wyZwoptexManager::getInstance()->makeSpriteEx("ui_common", "boss1_head.png");
//            isBoss = true;
//            break;
//        case 1:
//            bossHead = wyZwoptexManager::getInstance()->makeSpriteEx("ui_common", "boss2_head.png");
//            isBoss = true;
//            break;
//        case 2:
//            bossHead = wyZwoptexManager::getInstance()->makeSpriteEx("ui_common", "boss3_head.png");
//            isBoss = true;
//            break;
//        default:
//            break;
//    }
//    if (isBoss) {
//        bossContainer = wyNode::make();
//        addChildLocked(bossContainer);
//        
////        bossHead->setScaleX(sGlobal->scaleX);
////        bossHead->setScaleY(sGlobal->scaleY);
////        bossHead->setAnchor(0, 0);
////        bossHead->setPosition(DP(6)*sGlobal->scaleX, DP(10)*sGlobal->scaleY);
//        
//        bossBloodBorder = wySpriteEx::make(MyTextureManager::makePNG("progressbar_bottom.png"));
//        bossBloodBorder->setScaleX(sGlobal->scaleX*0.65f);
//        bossBloodBorder->setScaleY(sGlobal->scaleY*0.8f);
//        bossBloodBorder->setAnchor(0, 0);
//        bossBloodBorder->setPosition(DP(60)*sGlobal->scaleY, DP(20)*sGlobal->scaleY);
//        
//        bossBlood = wyProgressTimer::make(wySprite::make(MyTextureManager::makePNG("progressbar_upper.png")));
//        bossBlood->setStyle(HORIZONTAL_BAR_LR);
//        bossBlood->setAnchor(0, 0);
//        bossBlood->setPosition(bossBloodBorder->getPositionX() + DP(2)*sGlobal->scaleX, bossBloodBorder->getPositionY() + DP(2)*sGlobal->scaleY);
//        bossBlood->setScaleX(sGlobal->scaleX*0.65f);
//        bossBlood->setScaleY(sGlobal->scaleY*0.8f);
//        bossBlood->setPercentage(100);
//        
//        bossContainer->addChildLocked(bossBloodBorder);
//        bossContainer->addChildLocked(bossBlood);
////        bossContainer->addChildLocked(bossHead);
//        bossContainer->setPosition(0, -DP(120)*sGlobal->scaleY);
//    }
}

void HudLayer::showHealEffect() {
    wyParticleSystem *heal_emitter = wyParticleLoader::load("particle_heal.plist");
    heal_emitter->setScaleX(sGlobal->scaleX*1.0f);
    heal_emitter->setScaleY(sGlobal->scaleY*1.0f);
    heal_emitter->setEmissionRate(10);
    heal_emitter->setPosition(hpBarMiddle->getPositionX()+hpBarMiddle->getContentSize().width/2*hpBarMiddle->getScaleX(), hpBarMiddle->getPositionY()-hpBarMiddle->getContentSize().height*hpBarMiddle->getScaleY());
    heal_emitter->setAutoRemoveOnFinish(true);
    addChildLocked(heal_emitter, 101);
    heal_emitter->setPositionType(GROUPED);
}

void HudLayer::onExit() {
    wyLayer::onExit();
    
    atkBtn->setSelected(false);
    skillBtn->setSelected(false);
    jumpBtn->setSelected(false);
#if ANDROID
    jumpBtn->clearTouch();
    skillBtn->clearTouch();
    atkBtn->clearTouch();
#endif
}

void HudLayer::trainToLv1() {
    showNotice("now, let's start real fight. press anywhere to continue.");
    isTrainingToLv1 = true;
}

bool HudLayer::canAutoFire(int weaponId) {
    LOGE("weaponId:%d", weaponId);
    if (weaponId != WEAPON_UMP && weaponId != WEAPON_M870P && weaponId != WEAPON_HYDRA) {
        return true;
    }
    return false;
}




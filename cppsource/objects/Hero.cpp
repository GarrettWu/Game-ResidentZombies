//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "Hero.h"
#include "Global.h"
#include "MainScene.h"
#include "Intro.h"
#include "DeadNoticeLayer.h"
#include "GameCompleteLayer.h"
#include "Bonus.h"
#include "Enemy.h"
#include "EndPoint.h"
#include "NPC.h"
#include "HeroBullet.h"
#include "Boss1.h"

static void onHideYingEnded(wyAction* action, void* data) {
    Hero *hero = (Hero *)data;
    hero->ying->setVisible(false);
    hero->isYingReleasing = false;
}

static void onHeroFootChanged(wyAFCSprite* sprite, void* data) {
    Hero *hero = (Hero *) data;
    wySPXSprite *tou = hero->spx_hero_body;
    wySPXSprite *lfoot = hero->spx_hero_l_foot;
    wySPXSprite *rfoot = hero->spx_hero_r_foot;
    int posx = 0, posy = 0, posy1 = 0;
    float angel;
    float scale = tou->getScaleX();
    switch(sprite->getCurrentAnimationIndex()) {
        case SPX_HERO_R_LEG_RUN: {
            //1,0,1,2,3,2,1,0,1,2,3,2
            switch (sprite->getCurrentFrame()) {
                case 0:
                    posy = DP(4)*scale;
                    angel = 0.5f;
                    break;
                case 1:
                    posy = 0;
                    angel = 1.0f;
                    break;
                case 2:
                    posy = DP(4)*scale;
                    angel = 0.5f;
                    break;
                case 3:
                    posy = DP(8)*scale;
                    angel = 0.0f;
                    break;
                case 4:
                    posy = DP(12)*scale;
                    angel = -0.5f;
                    break;
                case 5:
                    posy = DP(8)*scale;
                    angel = 0.0f;
                    break;
                case 6:
                    posy = DP(4)*scale;
                    angel = 0.5f;
                    break;
                case 7:
                    posy = 0;
                    angel = 1.0f;
                    break;
                case 8:
                    posy = DP(4)*scale;
                    angel = 0.5f;
                    break;
                case 9:
                    posy = DP(8)*scale;
                    angel = 0.0f;
                    break;
                case 10:
                    posy = DP(12)*scale;
                    angel = -0.5f;
                    break;
                case 11:
                    posy = DP(8)*scale;
                    angel = 0.0f;
                    break;
                default:
                    break;
            }
            tou->setPosition(posx, posy);
            lfoot->setPosition(posx, posy);
            rfoot->setPosition(posx, posy);
            hero->setHeadRotation(angel);
            break;
        }
        case SPX_HERO_R_LEG_LAND: {
            switch (sprite->getCurrentFrame()) {
                case 0:
                    posy = DP(-24)*scale;
                    angel = 5;
                    break;
                case 1:
                case 2:
                    posx = DP(8)*scale;
                    posy = DP(-46)*scale;
                    angel = 10;
                    break;
                case 3:
                    posx = DP(5)*scale;
                    posy = DP(-30)*scale;
                    angel = 5;
                    break;
                default:
                    break;
            }
            tou->setPosition(posx, posy);
            hero->setHeadRotation(angel);
            break;
        }
        case SPX_HERO_R_LEG_DB_JUMP: {
            switch (sprite->getCurrentFrame()) {
                case 0:
                    posx = DP(8)*scale;
                    posy = DP(-2)*scale;
                    posy1 = DP(10)*scale;
                    angel = 4;
                    break;
//                case 1:
//                    posy = DP(5)*scale;
//                    posy1 = DP(5)*scale;
//                    angel = 3;
//                    break;
//                case 2:
//                    posy = DP(10)*scale;
//                    posy1 = DP(10)*scale;
//                    angel = 2;
//                    break;
                case 1:
                case 2:
                case 3:
                    posy = DP(20)*scale;
                    posy1 = DP(20)*scale;
                    angel = -2;
                    break;
                case 4:
                    posy = DP(10)*scale;
                    posy1 = DP(10)*scale;
                    angel = -2;
                    break;
                case 5:
                    posy = DP(5)*scale;
                    posy1 = DP(5)*scale;
                    angel = -2;
                    break;
                case 6:
                    posy = 0;
                    posy1 = DP(0)*scale;
                    angel = -2;
                    break;
                default:
                    break;
            }
            tou->setPosition(posx, posy);
            hero->setHeadRotation(angel);
            
            rfoot->setPosition(0, posy1);
            lfoot->setPosition(0, posy1);
            break;
        }
        case SPX_HERO_R_LEG_FALL: {
//            switch (sprite->getCurrentFrame()) {
//                case 0:
//                    angel = -2;
//                    break;
//                case 1:
//                    angel = -1;
//                    break;
//                case 2:
//                    angel = 0;
//                    break;
//                default:
//                    angel = 0;
//                    break;
//            }
//            tou->setRotation(angel);
//            break;
//            LOGE("cur f:%d, rot:%f", sprite->getCurrentFrame(), tou->getRotation());
            break;
        }
        case SPX_HERO_R_LEG_SI: {
            switch (sprite->getCurrentFrame()) {
                case 1:
                    hero->shadow->setPosition(DP(6)*sGlobal->scaleY, -DP(48)*sGlobal->scaleY);
                    break;
                case 2:
                    hero->shadow->setPosition(DP(12)*sGlobal->scaleY, -DP(48)*sGlobal->scaleY);
                    break;
                default:
                    break;
            }
            break;
        }
        case SPX_HERO_R_LEG_FH: {
            switch (sprite->getCurrentFrame()) {
                case 1:
                    hero->shadow->setPosition(DP(6)*sGlobal->scaleY, -DP(48)*sGlobal->scaleY);
                    break;
                case 2:
                    hero->shadow->setPosition(DP(0)*sGlobal->scaleY, -DP(48)*sGlobal->scaleY);
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}

static void onHeroHeadChanged(wyAFCSprite* sprite, void* data) {
    Hero *hero = (Hero *) data;
    
    float angel = 0;
    int texId = hero->getWeaponTexIndex(sGlobal->activeWeapon);
    
    switch(sprite->getCurrentAnimationIndex()+texId*1000) {
        case SPX_MP5A5_FIRE + 1000:
        case SPX_NORMAL_GUN_FIRE: {
            switch (sprite->getCurrentFrame()) {
                case 1:
                    angel = 0;
                    break;
                case 2:
                case 3:
                    angel = -2;
                    break;
                case 4:
                case 5:
                    angel = -1;
                    break;
                case 8:
                case 9:
                    angel = 1;
                    break;
                default:
                    angel = 0;
                    break;
            }
            break;
        }
        case SPX_HYDRA_FIRE + 1000:{
            switch (sprite->getCurrentFrame()) {
                case 2:
                    angel = -2;
                    HeroBullet::make(sGlobal->activeWeapon);
                    break;
                case 3:
                case 4:
                    angel = -1;
                    break;
                case 7:
                case 8:
                    angel = 2;
                    break;
                default:
                    angel = 0;
                    break;
            }
            break;
        }
            
        case SPX_AK47_FIRE + 1000:
        case SPX_RIFLE_GUN_FIRE: {
            switch (sprite->getCurrentFrame()) {
                case 2:
                    angel = -6;
                    HeroBullet::make(sGlobal->activeWeapon);
                    break;
                case 3:
                case 4:
                    angel = -3;
                    break;
                case 7:
                case 8:
                    angel = 3;
                    break;
                default:
                    angel = 0;
                    break;
            }
            break;
        }
        case SPX_GLB_FIRE + 1000:
        case SPX_GLR_FIRE + 1000:
        case SPX_LASER_GUN_FIRE: {
            switch (sprite->getCurrentFrame()) {
                case 1:
                    angel = -1;
                    hero->laserY = DP(4)*sGlobal->scaleX;
                    hero->laserX = DP(-2)*sGlobal->scaleX;
                    break;
                case 2:
                    angel = -2;
                    hero->laserY = DP(6)*sGlobal->scaleX;
                    hero->laserX = DP(-3)*sGlobal->scaleX;
                    break;
                case 3:
                case 4:
                    angel = -3;
                    hero->laserY = DP(8)*sGlobal->scaleX;
                    hero->laserX = DP(-4)*sGlobal->scaleX;
                    break;
                case 5:
                    angel = -1;
                    hero->laserY = DP(4)*sGlobal->scaleX;
                    hero->laserX = DP(-2)*sGlobal->scaleX;
                    break;
                case 7:
                    angel = 1;
                    hero->laserY = -DP(1)*sGlobal->scaleX;
                    hero->laserY = DP(1)*sGlobal->scaleX;
                    break;
                default:
                    angel = 0;
                    hero->laserY = 0;
                    hero->laserX = 0;
                    break;
            }
            break;
        }
        case SPX_F1_FIRE+1000:
        case SPX_E3_FIRE+1000:
        case SPX_MACHINE_GUN_FIRE: {
            switch (sprite->getCurrentFrame()) {
                case 1:
                    angel = -1;
                    HeroBullet::make(sGlobal->activeWeapon);
                    break;
                case 2:
                case 3:
                    angel = -2;
                    break;
                case 5:
                case 6:
                    angel = 2;
                    break;
                default:
                    angel = 0;
                    break;
            }
            break;
        }
        case SPX_TORPEDO_FIRE+1000:
        case SPX_DS2_FIRE + 1000:{
            switch (sprite->getCurrentFrame()) {
                case 1:
                    angel = -3;
                    break;
                case 2:
                    angel = -2;
                    break;
                case 3:
                    angel = -1;
                    break;
                case 4:
                    angel = 0;
                    break;
                case 5:
                    angel = 1;
                    break;
                case 6:
                    angel = 2;
                    break;
                case 7:
                    angel = 1;
                    break;
                case 8:
                    angel = 0;
                    break;
                default:
                    angel = 0;
                    break;
            }
            break;
        }
        case SPX_SHOT_GUN_FIRE: {
            switch (sprite->getCurrentFrame()) {
                case 1:
                    angel = -6;
                    break;
                case 2:
                    angel = -3;
                    break;
                case 3:
                    angel = -1;
                    break;
                case 4:
                    angel = 0;
                    break;
                case 5:
                    angel = 2;
                    break;
                case 6:
                    angel = 4;
                    break;
                case 7:
                    angel = 2;
                    break;
                case 8:
                    angel = 0;
                    break;
                default:
                    angel = 0;
                    break;
            }
            break;
        }
        case SPX_BLASTING_FIRE+1000:{
            switch (sprite->getCurrentFrame()) {
                case 1:
                    angel = 0;
                    HeroBullet::make(sGlobal->activeWeapon);
                    break;
                case 2:
                    angel = -4;
                    break;
                case 3:
                    angel = -2;
                    break;
                case 4:
                    angel = 0;
                    break;
                case 5:
                    angel = 2;
                    break;
                default:
                    angel = 0;
                    break;
            }
            break;
        }
        case SPX_RPG_FIRE: {
            switch (sprite->getCurrentFrame()) {
                case 1:
                    angel = 0;
                    HeroBullet::make(sGlobal->activeWeapon);
                    break;
                case 2:
                    angel = -12;
                    break;
                case 3:
                    angel = -6;
                    break;
                case 4:
                    angel = 0;
                    break;
                case 5:
                    angel = 6;
                    break;
                default:
                    angel = 0;
                    break;
            }
            break;
        }
        default:
            break;
    }
    
//    LOGE("anim index:%d, si index:%d, isFlying:%s", sprite->getCurrentAnimationIndex(), hero->spxSiIndex, hero->isFlying?"true":"false");
    if (sprite->getCurrentAnimationIndex() == hero->spxSiIndex && hero->isFlying) {
        switch (sprite->getCurrentFrame()) {
            case 1:
                hero->yingSprite->setRotation(-6);
                hero->yingSprite->setPosition(DP(18)*sGlobal->scaleX, -DP(10)*sGlobal->scaleX);
                break;
            case 2:
                hero->yingSprite->setRotation(20);
                hero->yingSprite->setPosition(DP(27)*sGlobal->scaleX, -DP(22)*sGlobal->scaleX);
                break;
            default:
                break;
        }
    }
    
    if (sprite->getCurrentAnimationIndex() == hero->spxFHIndex && hero->isFlying) {
        switch (sprite->getCurrentFrame()) {
            case 1:
                hero->yingSprite->setRotation(-6);
                hero->yingSprite->setPosition(DP(18)*sGlobal->scaleX, -DP(10)*sGlobal->scaleX);
                break;
            case 2:
                hero->yingSprite->setRotation(0);
                hero->yingSprite->setPosition(0, 0);
                break;
            default:
                break;
        }
    }
    
    if (!hero->isMotoing) {
        hero->setHeadRotation(angel, true);
    }
}

static void onHeroHeadEnded(wyAFCSprite* sprite, void* data) {
    Hero *hero = (Hero *) data;
    if (sprite->getCurrentAnimationIndex() == hero->spxBodyFireIndex) {
        sprite->setLoopCount(0);
        sprite->playAnimation(hero->spxBodyIdleIndex);
        sprite->setUnitInterval(0.05f);
        hero->isShooting = false;
    }
    else if (sprite->getCurrentAnimationIndex() == hero->spxFHIndex && hero->isFlying) {
        hero->yingSprite->setRotation(0);
        hero->yingSprite->setPosition(0, 0);
    }
}

static void onHeroFootEnded(wyAFCSprite* sprite, void* data) {
    Hero *hero = (Hero *) data;
    wySPXSprite *tou = hero->spx_hero_body;
    wySPXSprite *lfoot = hero->spx_hero_l_foot;
    wySPXSprite *rfoot = hero->spx_hero_r_foot;
    
    switch(sprite->getCurrentAnimationIndex()) {
        case SPX_HERO_R_LEG_LAND: {
            rfoot->setLoopCount(-1);
            rfoot->playAnimation(SPX_HERO_R_LEG_RUN);
            
            lfoot->setLoopCount(-1);
            lfoot->playAnimation(SPX_HERO_L_LEG_RUN);
            
            tou->setPosition(0, DP(2)*sGlobal->scaleY);
            hero->setHeadRotation(0);
            lfoot->setPosition(0, DP(2)*sGlobal->scaleY);
            rfoot->setPosition(0, DP(2)*sGlobal->scaleY);
            break;
        }
        case SPX_HERO_R_LEG_DB_JUMP: {
//            LOGE("db jump end...");
            rfoot->setUnitInterval(0.05f);
            lfoot->setUnitInterval(0.05f);
            hero->isDBJumpAnimating = false;
            break;
        }
        case SPX_HERO_R_LEG_FALL: {
//            LOGE("fall end...");
            rfoot->setUnitInterval(0.05f);
            lfoot->setUnitInterval(0.05f);
            break;
        }
//        case SPX_HERO_R_LEG_HURT: {
//            tou->setPosition(0, 0);
//            rfoot->setUnitInterval(0.05f);
//            lfoot->setUnitInterval(0.05f);
//            hero->playHeroAnimation(SPX_HERO_R_LEG_RUN);
//            break;
//        }
        case SPX_HERO_R_LEG_FH: {
            hero->isInDead = false;
            
            hero->spx_hero_body->setLoopCount(-1);
            hero->spx_hero_body->setUnitInterval(0.05f);
            
            hero->spx_hero_r_foot->setLoopCount(-1);
            hero->spx_hero_r_foot->setUnitInterval(0.05f);
            
            hero->spx_hero_l_foot->setLoopCount(-1);
            hero->spx_hero_l_foot->setUnitInterval(0.05f);
            
            if (sGlobal->isOnGround) {
                hero->playHeroAnimation(SPX_HERO_R_LEG_RUN);
            } else {
                hero->playHeroAnimation(SPX_HERO_R_LEG_FALL);
            }
            
            sGlobal->mainGameLayer->addToEnable(hero->body);
            sGlobal->jumpTime = 0;
            sGlobal->jumpTime1 = 0;
            sGlobal->hudLayer->isJumping = false;
            sGlobal->hudLayer->isReleased = true;
            hero->isDead = false;
            hero->isRunning = true;
            
            if (!hero->isFHing) {
                hero->isSavingMe = true;
//                hero->isWudi = true;
            } else {
                sGlobal->mainGameLayer->needAutoScroll = true;
                hero->isFHing = false;
            }
            
            sGlobal->hudLayer->setHudOp(true, true, true, true, true);
            
            hero->shadow->setPosition(0, -DP(48)*sGlobal->scaleY);

            hero->body->SetLinearVelocity(b2Vec2(hero->body->GetLinearVelocity().x, HERO_FLY_Y_DOWN));
            
            break;
        }
        default:
            break;
    }
}

static void onJumpBombEnded(wyAFCSprite* sprite, void* data) {
    Hero* hero = (Hero*)data;
    wySPXSprite *jumpBomb = ((wySPXSprite *) hero->jumpBomb);
    jumpBomb->setVisible(false);
}

static void onBloodEnded1(wyAFCSprite* sprite, void* data) {
    wyNode* node = (wyNode*)data;
    node->setVisible(false);
}

static void onIllusionGone(wyAFCSprite* sprite, void* data) {
    Hero* hero = (Hero*)data;
    wySPXSprite *illusion = ((wySPXSprite *) hero->illusion);
    illusion->setVisible(false);
    illusion->setAFCSpriteCallback(NULL, NULL);
    illusion->setLoopCount(-1);
    hero->isIllusionActive = false;
    hero->illusionTimer = 0;
    hero->isIllusionGoning = false;
}

static void onSandShieldGone(wyAFCSprite* sprite, void* data) {
    Hero* hero = (Hero*)data;
    hero->isSandShieldActive = false;
    hero->sandShieldTimer = 0;
    hero->isSandShieldGoning = false;
}

void Hero::switchWeapon(int index) {
    if (index == sGlobal->activeWeapon) {
        return;
    }
    
    sGlobal->playEffect(se_changegun);
    int fromWeapon = sGlobal->activeWeapon;
    sGlobal->activeWeapon = index;
    
    setCurrentWeaponAnimIds(index);
    
    changeBody(fromWeapon, index);
    
    spx_hero_body->playAnimation(spxBodyIdleIndex);
}

int Hero::getWeaponTexIndex(int weaponId) {
    if (weaponId == WEAPON_UMP || weaponId == WEAPON_M870P || weaponId == WEAPON_AER14 || weaponId == WEAPON_M249 || weaponId == WEAPON_M4A1 || weaponId == WEAPON_RPG) {
        return 0;
    } else {
        return 1;
    }
}

void Hero::changeBody(int fromWeapon, int toWeapon) {
    if (fromWeapon == toWeapon) {
        return;
    }
    
    int fromWeaponTexIndex = getWeaponTexIndex(fromWeapon);
    int toWeaponTexIndex = getWeaponTexIndex(toWeapon);
    if (fromWeaponTexIndex == 0) {
        if (toWeaponTexIndex == 1) {
            wySPXSprite *newBody = wySPXSprite::make("hero1.sprite", false, MyTextureManager::makePNG("hero1.png"), 0);
            newBody->setForceTickMode(true);
            newBody->setUnitInterval(0.05f);
            newBody->setScaleX(0.62f*sGlobal->scaleX);
            newBody->setScaleY(0.62f*sGlobal->scaleY);
            
            newBody->setPosition(spx_hero_body->getPositionX(), spx_hero_body->getPositionY());
            newBody->setRotation(spx_hero_body->getRotation());
            
            spx_hero_body->getParent()->removeChildLocked(spx_hero_body, true);
            
            spxSprite = newBody;
            spx_hero_body = newBody;
            container->addChildLocked(spx_hero_body, 100);
            
            wyAFCSpriteCallback callback0 = {
                onHeroHeadChanged,
                onHeroHeadEnded
            };
            spx_hero_body->setAFCSpriteCallback(&callback0, this);
        }
    } else {
        if (toWeaponTexIndex == 0) {
            wySPXSprite *newBody = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), 0);
            newBody->setForceTickMode(true);
            newBody->setUnitInterval(0.05f);
            newBody->setScaleX(0.62f*sGlobal->scaleX);
            newBody->setScaleY(0.62f*sGlobal->scaleY);
            
            newBody->setPosition(spx_hero_body->getPositionX(), spx_hero_body->getPositionY());
            newBody->setRotation(spx_hero_body->getRotation());
            
            spx_hero_body->getParent()->removeChildLocked(spx_hero_body, true);
            
            spxSprite = newBody;
            spx_hero_body = newBody;
            container->addChildLocked(spx_hero_body, 100);
            
            wyAFCSpriteCallback callback0 = {
                onHeroHeadChanged,
                onHeroHeadEnded
            };
            spx_hero_body->setAFCSpriteCallback(&callback0, this);
        }
    }
}

void Hero::setCurrentWeaponAnimIds(int index) {
    switch (index) {
        case WEAPON_UMP:
            spxBodyIdleIndex = SPX_NORMAL_GUN_IDLE;
            spxBodyFireIndex = SPX_NORMAL_GUN_FIRE;
            spxSiIndex = SPX_HERO_NGUN_SI;
            spxFHIndex = SPX_HERO_NGUN_FH;
            break;
        case WEAPON_M870P:
            spxBodyIdleIndex = SPX_RIFLE_GUN_IDLE;
            spxBodyFireIndex = SPX_RIFLE_GUN_FIRE;
            spxSiIndex = SPX_HERO_RIFLE_SI;
            spxFHIndex = SPX_HERO_RIFLE_FH;
            break;
        case WEAPON_AER14:
            spxBodyIdleIndex = SPX_LASER_GUN_IDLE;
            spxBodyFireIndex = SPX_LASER_GUN_FIRE;
            spxSiIndex = SPX_HERO_LASER_SI;
            spxFHIndex = SPX_HERO_LASER_FH;
            break;
        case WEAPON_M249:
            spxBodyIdleIndex = SPX_MACHINE_GUN_IDLE;
            spxBodyFireIndex = SPX_MACHINE_GUN_FIRE;
            spxSiIndex = SPX_HERO_MGUN_SI;
            spxFHIndex = SPX_HERO_MGUN_FH;
            break;
        case WEAPON_M4A1:
            spxBodyIdleIndex = SPX_SHOT_GUN_IDLE;
            spxBodyFireIndex = SPX_SHOT_GUN_FIRE;
            spxSiIndex = SPX_HERO_SHOTGUN_SI;
            spxFHIndex = SPX_HERO_SHOTGUN_FH;
            break;
        case WEAPON_RPG:
            spxBodyIdleIndex = SPX_RPG_IDLE;
            spxBodyFireIndex = SPX_RPG_FIRE;
            spxSiIndex = SPX_HERO_RPG_SI;
            spxFHIndex = SPX_HERO_RPG_FH;
            break;
        case WEAPON_MP5A5:
            spxBodyIdleIndex = SPX_MP5A5_IDLE;
            spxBodyFireIndex = SPX_MP5A5_FIRE;
            spxSiIndex = SPX_MP5A5_SI;
            spxFHIndex = SPX_MP5A5_FH;
            break;
        case WEAPON_AK47:
            spxBodyIdleIndex = SPX_AK47_IDLE;
            spxBodyFireIndex = SPX_AK47_FIRE;
            spxSiIndex = SPX_AK47_SI;
            spxFHIndex = SPX_AK47_FH;
            break;
        case WEAPON_HYDRA:
            spxBodyIdleIndex = SPX_HYDRA_IDLE;
            spxBodyFireIndex = SPX_HYDRA_FIRE;
            spxSiIndex = SPX_HYDRA_SI;
            spxFHIndex = SPX_HYDRA_FH;
            break;
        case WEAPON_GLB:
            spxBodyIdleIndex = SPX_GLB_IDLE;
            spxBodyFireIndex = SPX_GLB_FIRE;
            spxSiIndex = SPX_GLB_SI;
            spxFHIndex = SPX_GLB_FH;
            break;
        case WEAPON_GLR:
            spxBodyIdleIndex = SPX_GLR_IDLE;
            spxBodyFireIndex = SPX_GLR_FIRE;
            spxSiIndex = SPX_GLR_SI;
            spxFHIndex = SPX_GLR_FH;
            break;
        case WEAPON_F1:
            spxBodyIdleIndex = SPX_F1_IDLE;
            spxBodyFireIndex = SPX_F1_FIRE;
            spxSiIndex = SPX_F1_SI;
            spxFHIndex = SPX_F1_FH;
            break;
        case WEAPON_E3:
            spxBodyIdleIndex = SPX_E3_IDLE;
            spxBodyFireIndex = SPX_E3_FIRE;
            spxSiIndex = SPX_E3_SI;
            spxFHIndex = SPX_E3_FH;
            break;
        case WEAPON_DS2:
            spxBodyIdleIndex = SPX_DS2_IDLE;
            spxBodyFireIndex = SPX_DS2_FIRE;
            spxSiIndex = SPX_DS2_SI;
            spxFHIndex = SPX_DS2_FH;
            break;
        case WEAPON_TORPEDO:
            spxBodyIdleIndex = SPX_TORPEDO_IDLE;
            spxBodyFireIndex = SPX_TORPEDO_FIRE;
            spxSiIndex = SPX_TORPEDO_SI;
            spxFHIndex = SPX_TORPEDO_FH;
            break;
        case WEAPON_BLASTING:
            spxBodyIdleIndex = SPX_BLASTING_IDLE;
            spxBodyFireIndex = SPX_BLASTING_FIRE;
            spxSiIndex = SPX_BLASTING_SI;
            spxFHIndex = SPX_BLASTING_FH;
            break;
            
        default:
            spxBodyIdleIndex = SPX_NORMAL_GUN_IDLE;
            spxBodyFireIndex = SPX_NORMAL_GUN_FIRE;
            spxSiIndex = SPX_HERO_NGUN_SI;
            spxFHIndex = SPX_HERO_NGUN_FH;
            break;
    }
}

Hero::Hero(wyHashSet* HeroDef, bool needAutoScroll){
    
    wyDirector* director = wyDirector::getInstance();
//    director->setTickFactor(0.1f);
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    spx_hero_body = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), SPX_HERO_BOUNDING_BOX);
    spx_hero_l_foot = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), SPX_HERO_L_LEG_RUN);
    spx_hero_r_foot = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), SPX_HERO_R_LEG_RUN);
    
    jumpBomb = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), 10);
    illusion = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), 10);
    
    spx_hero_body->setForceTickMode(true);
	spx_hero_body->setUnitInterval(0.05f);
    spx_hero_body->setScaleX(0.62f*sGlobal->scaleX);
    spx_hero_body->setScaleY(0.62f*sGlobal->scaleY);
//    wyAFCClipMapping* mapping = wyAFCClipMapping::make(1);
//    mapping->mapClip(21, 29);
//    sprite_mao->addClipMapping(mapping);
    spxSprite = spx_hero_body;
    
    spx_hero_l_foot->setForceTickMode(true);
	spx_hero_l_foot->setUnitInterval(0.05f);
    spx_hero_l_foot->setScaleX(0.62f*sGlobal->scaleX);
    spx_hero_l_foot->setScaleY(0.62f*sGlobal->scaleY);
    
    spx_hero_r_foot->setForceTickMode(true);
	spx_hero_r_foot->setUnitInterval(0.05f);
    spx_hero_r_foot->setScaleX(0.62f*sGlobal->scaleX);
    spx_hero_r_foot->setScaleY(0.62f*sGlobal->scaleY);
    
    spx_hero_r_foot->setLoopCount(-1);
    spx_hero_l_foot->setLoopCount(-1);
    
    wyAFCSpriteCallback callback11 = {
        onHeroFootChanged,
        onHeroFootEnded,
    };
    spx_hero_r_foot->setAFCSpriteCallback(&callback11, this);
    
    wyAFCSpriteCallback callback0 = {
        onHeroHeadChanged,
        onHeroHeadEnded
    };
    spx_hero_body->setAFCSpriteCallback(&callback0, this);
    
    wyNode *heroContainer = wyNode::make();
    sGlobal->mainGameLayer->addChildLocked(heroContainer, 100);
    heroContainer->addChildLocked(spx_hero_l_foot, 0);
    heroContainer->addChildLocked(spx_hero_body, 2);
    heroContainer->addChildLocked(spx_hero_r_foot, 3);
    
    this->obj_type = TYPE_MAO;
    this->container = heroContainer;
    
    createMotoParts();
    
    sGlobal->mainGameLayer->addChildLocked(jumpBomb, 101);
    jumpBomb->setForceTickMode(true);
	jumpBomb->setUnitInterval(0.1f);
    jumpBomb->setLoopCount(0);
//    jumpBomb->setFlipX(true);
    jumpBomb->setScaleX(0.8f*sGlobal->scaleX);
    jumpBomb->setScaleY(0.8f*sGlobal->scaleY);
    jumpBomb->setVisible(false);
    jumpBomb->setAlpha(0);
    wyAFCSpriteCallback callback = {
        NULL,
        onJumpBombEnded
    };
    jumpBomb->setAFCSpriteCallback(&callback, this);
    
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.userData = this;
    body = world->CreateBody(&bd);
    
    bodyRect = spx_hero_body->getCollisionRect(0);
    bodyRect.x *= spxSprite->getScaleX();
    bodyRect.y *= spxSprite->getScaleY();
    bodyRect.width *= spxSprite->getScaleX();
    bodyRect.height *= spxSprite->getScaleY();
    
    spxScaleX = SPX_SCALE_X*2;
    createFixtureByCurrentFrame(false);
//    LOGE("%f,%f,%f,%f", bodyRect.x, bodyRect.y, bodyRect.width, bodyRect.height);
    
    b2Fixture *fixture = body->GetFixtureList();
    b2Fixture *next;
    while (fixture != NULL) {
        next = fixture->GetNext();
        b2Filter filter = fixture->GetFilterData();
        filter.maskBits = MASK_BITS[TYPE_MAO];
        filter.categoryBits = CAT_BITS[TYPE_MAO];
        filter.groupIndex = GROUND_INDEX[TYPE_MAO];
        fixture->SetFilterData(filter);
        fixture = next;
    }
//    wyRect rect = sprite_mao->getCollisionRect(0);
//    halfBodyHeightInMeter = m_box2d->pixel2Meter(rect.height/2 * sprite_mao->getScaleY());
//    halfBodyWidthInMeter = m_box2d->pixel2Meter(rect.width/2 * sprite_mao->getScaleX());
    isHeightInited = false;
//    LOGE("heroWidth: %f, offX:%f", bodyRect.width, offsetX);
    
//    if (needAutoScroll) {
//        sprite_mao_tou->setLoopCount(-1);
//        sprite_mao_tou->playAnimation(2);
//        
//        sprite_mao_foot->setLoopCount(-1);
//        sprite_mao_foot->playAnimation(3);
//    } else {
//        sprite_mao_tou->setLoopCount(-1);
//        sprite_mao_tou->playAnimation(0);
//        
//        sprite_mao_foot->setLoopCount(-1);
//        sprite_mao_foot->playAnimation(1);
//    }
//    spx_hero_body->playAnimation(spxBodyIdleIndex);
    
    //要更新下body图片
//    sGlobal->activeWeapon = WEAPON_6;
    setCurrentWeaponAnimIds(sGlobal->activeWeapon);
    changeBody(WEAPON_UMP, sGlobal->activeWeapon);
    spx_hero_body->playAnimation(spxBodyIdleIndex);

    
//    LOGE("hero mass:%f", body->GetMass());

    if (needAutoScroll) {
        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    }
    
    isHurt = false;
    hurtTimer = 0;

    isDead = false;
    
    shadow = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shadow.png");
    shadow->setScaleX(sGlobal->scaleX*3.2f);
    shadow->setScaleY(sGlobal->scaleY*1.6f);
    container->addChildLocked(shadow, -1);
    shadow->setVisible(false);
    shadow->setAlpha(100);
    shadow->setPosition(-DP(0)*sGlobal->scaleY, -DP(48)*sGlobal->scaleY);

    sGlobal->mainGameLayer->addChildLocked(illusion, container->getZOrder() - 1);
    illusion->setForceTickMode(true);
	illusion->setUnitInterval(0.2f);
    illusion->setLoopCount(-1);
    illusion->setFlipX(true);
    illusion->setScaleX(0.8f*sGlobal->scaleX);
    illusion->setScaleY(0.8f*sGlobal->scaleY);
    illusion->setVisible(false);
    isIllusionActive = false;
    isIllusionGoning = false;
    illusionTimer = 0;
    
    isSandShieldActive = false;
    isSandShieldGoning = false;
    sandShieldTimer = 0;
    
    isShaking = false;
    shakingTimer = 0;
    
    isStrongShaking = false;
    strongShakingTimer = 0;
    
    isRunning = true;
    combo = 0;
    hasShoot = false;
    
    sGlobal->heroEyeZ = 1;
    sGlobal->bossEyeZ = 1;
    
    needSaveMe = false;
    isSavingMe = false;
    savingMeTimer = 0;
    isWudi = false;
    wudiTimer = 0;
    
    hasYing = false;
    isYingReleased = false;
    
    ying = wyNode::make();
    sGlobal->mainGameLayer->addChildLocked(ying, container->getZOrder()-1);
    ying->setVisible(false);
//    LOGE("ying z:%d", ying->getZOrder());
    
    yingSprite = wySPXSprite::make("enemy_2.sprite", false, MyTextureManager::makePNG("enemy_2.png"), 23);
    ying->addChildLocked(yingSprite, 1);
    yingSprite->setForceTickMode(true);
	yingSprite->setUnitInterval(0.1f);
    yingSprite->setLoopCount(-1);
    yingSprite->setScaleX(1.1f*sGlobal->scaleX);
    yingSprite->setScaleY(1.1f*sGlobal->scaleY);
    
    isSaveMeInited = false;
    
    initShieldEffect();

    initDodgeEffect();

    initLaser();
    
    bloodSprite = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), SPX_PENXUE_1);
    sGlobal->mainGameLayer->addChildLocked(bloodSprite, 103);
    bloodSprite->setForceTickMode(true);
    bloodSprite->setUnitInterval(0.1f);
    bloodSprite->setLoopCount(0);
    bloodSprite->setFlipX(false);
    bloodSprite->setScaleX(1.8f*sGlobal->scaleX);
    bloodSprite->setScaleY(1.8f*sGlobal->scaleY);
    bloodSprite->setVisible(false);
    wyAFCSpriteCallback callback1 = {
        NULL,
        onBloodEnded1
    };
    bloodSprite->setAFCSpriteCallback(&callback1, bloodSprite);
    
    isFlying = false;
    isDeFlying = false;
    flyTimer = 0;
	fly_emitter = NULL;
	boost_emitter = NULL;
    
    isBoosting = false;
    isDeBoosting = false;
    boostTimer = 0;
    deBoostTimer = 0;
    isBackingToNormal = false;
    
    curVelX = sGlobal->heroVelocityX;
    heroZ = 1;
    
    motoFixture = NULL;
    shieldFixture = NULL;
    
    isHurtByChongZhuang = false;
    hurtByCZTimer = 0;

	isZhanShenShowing = false;
	zhanshen = NULL;
    
    baseY = -999;
    
    isFaZhenEnded = false;
    isShowingFaZhen = false;
    
    heal_emitter = NULL;
    isHealing = false;
    healTimer = 0;
    
    isFlyingPaused = false;
    needSetGravity = false;
    
    shandian = NULL;
    
    needSetSpeed = false;
    
    isYingReleasing = false;
    
    dist266 = DP(266)*sGlobal->scaleX;
    normalDeathY = -DP(70)*sGlobal->scaleY;
    flyDeathY = -DP(150)*sGlobal->scaleY;
    
    buff_emitter = NULL;
    
    recoverHPTimer = 0;
    
    isFiringMissle = false;
    firingMissleTimer = 0;
    fireTimes = 0;
    
    robertStatus = R_GONE;
    robertTimer = 0;
    robert = NULL;
    
    sGlobal->jumpTime = 0;
    sGlobal->jumpTime1 = 0;
    
    previousY = 0;
    
    isDBJumping = false;
    
    bodySensor = NULL;
    isBodySensorCreated = false;
    
    isMotoing = false;

    questRecordTimer = 0;
    
    needRepeat = false;
    
    needDestroy = true;
    laserCount = 0;
    laserY = 0;
    
    contactNum = 0;
    isHurtRot = false;
    hurtRotTimer = 0;
    
    isInDead = false;
    laserBullet = NULL;
    
    backToNormalSpeed = 5;
    isDisablingContact = false;
    
    isFiringLiudan = false;
    fireLiudanTime = 1;
    fireLiudanTimer = 0;
    fireLiudanInterval = 0.4f;
    
    isDBJumpAnimating = false;
    
    createHeroHurtSensor();
    
    wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("ui_en", "bullet3.png");
    bulletNormalRect = frame->getRect();
    
    frame = wyZwoptexManager::getInstance()->getSpriteFrame("ui_en", "bullet2.png");
    bulletRPGRect = frame->getRect();
    
    frame = wyZwoptexManager::getInstance()->getSpriteFrame("ui_en", "bullet1.png");
    bulletRobotRect = frame->getRect();
    
    frame = wyZwoptexManager::getInstance()->getSpriteFrame("ui_en", "bullet_MP5A5.png");
    bulletMP5A5Rect = frame->getRect();
    
    frame = wyZwoptexManager::getInstance()->getSpriteFrame("ui_en", "bullet_AK47.png");
    bulletAK47Rect = frame->getRect();
    
    frame = wyZwoptexManager::getInstance()->getSpriteFrame("ui_en", "bullet_HYDRA.png");
    bulletHYDRARect = frame->getRect();
    
    frame = wyZwoptexManager::getInstance()->getSpriteFrame("ui_en", "bullet_F1.png");
    bulletF1Rect = frame->getRect();
    
    frame = wyZwoptexManager::getInstance()->getSpriteFrame("ui_en", "bullet_E3.png");
    bulletE3Rect = frame->getRect();

    frame = wyZwoptexManager::getInstance()->getSpriteFrame("ui_en", "bullet_DS2.png");
    bulletDS2Rect = frame->getRect();
    
    frame = wyZwoptexManager::getInstance()->getSpriteFrame("ui_en", "bullet_TORPEDO.png");
    bulletTORPEDORect = frame->getRect();
    
    frame = wyZwoptexManager::getInstance()->getSpriteFrame("ui_en", "bullet_BLASTING.png");
    bulletBLASTINGRect = frame->getRect();

//    LOGE("%f,%f,%f,%f", bulletBLASTINGRect.x, bulletBLASTINGRect.y, bulletBLASTINGRect.width, bulletBLASTINGRect.height);
    
    speedline_emitter = NULL;
    
    gunLightContainer = NULL;
    gunLight = NULL;
    
    baseVert = 0;
}

Hero::~Hero() {

}

void Hero::initShieldEffect() {
    shieldContainer = wyNode::make();
    sGlobal->mainGameLayer->addChildLocked(shieldContainer, 102);
    shieldContainer->setVisible(false);
    
    shieldSprite_0 = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), SPX_HERO_SHIELD_0);
    shieldContainer->addChildLocked(shieldSprite_0);
    shieldSprite_0->setForceTickMode(true);
	shieldSprite_0->setUnitInterval(0.25f);
    shieldSprite_0->setLoopCount(0);
    //    shieldSprite_0->setFlipX(true);
    shieldSprite_0->setScaleX(1.1f*sGlobal->scaleX);
    shieldSprite_0->setScaleY(1.1f*sGlobal->scaleY);
    shieldSprite_0->setAlpha(100);
    shieldSprite_0->setBlendFunc((wyBlendFunc) { GL_SRC_ALPHA, GL_ONE });
    //    shieldSprite->setBlendFunc((wyBlendFunc) { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
    //    shieldSprite->setColor(wyc4b(220, 50, 10, 255));
    
    shieldSprite_2 = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), SPX_HERO_SHIELD_2);
    shieldContainer->addChildLocked(shieldSprite_2);
    shieldSprite_2->setForceTickMode(true);
	shieldSprite_2->setUnitInterval(0.1f);
    shieldSprite_2->setLoopCount(0);
    shieldSprite_2->setScaleX(1.1f*sGlobal->scaleX);
    shieldSprite_2->setScaleY(1.1f*sGlobal->scaleY);
    shieldSprite_2->setBlendFunc((wyBlendFunc) { GL_SRC_ALPHA, GL_ONE });
    shieldSprite_2->setAlpha(100);
    //    shieldSprite_2->setBlendFunc((wyBlendFunc) { GL_ONE_MINUS_SRC_ALPHA, GL_ONE });
    //    shieldSprite_2->setColor(wyc4b(220, 50, 10, 255));
    
    shieldSprite_1 = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), SPX_HERO_SHIELD_1);
    shieldContainer->addChildLocked(shieldSprite_1);
    shieldSprite_1->setForceTickMode(true);
	shieldSprite_1->setUnitInterval(0.15f);
    shieldSprite_1->setLoopCount(-1);
    shieldSprite_1->setScaleX(1.1f*sGlobal->scaleX);
    shieldSprite_1->setScaleY(1.1f*sGlobal->scaleY);
    shieldSprite_1->setBlendFunc((wyBlendFunc) { GL_SRC_ALPHA, GL_ONE });
}

void Hero::initDodgeEffect() {
    if (sGlobal->hasSkillMulti[SKILL_EVASION]) {
        dodgeContainer = wyNode::make();
        dodgeContainer->setVisible(false);
        sGlobal->mainGameLayer->addChildLocked(dodgeContainer, 102);
        
        dodgeTitle = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "dodge_title.png");//wySpriteEx::make(MyTextureManager::makePNG("dodge_title.png"));
        dodgeTitle->setScaleX(sGlobal->scaleX*0.8f);
        dodgeTitle->setScaleY(sGlobal->scaleY*0.8f);
        dodgeContainer->addChildLocked(dodgeTitle, 1);
        
        illusion1 = HeroIllusion::make();
        dodgeContainer->addChildLocked(illusion1);
        illusion1->setVisible(false);
        
        illusion2 = HeroIllusion::make();
        dodgeContainer->addChildLocked(illusion2);
        illusion2->setVisible(false);
        
        illusion3 = HeroIllusion::make();
        dodgeContainer->addChildLocked(illusion3);
        illusion3->setVisible(false);
    } else {
        dodgeContainer = NULL;
    }
}

void Hero::initLaser() {
    if (sGlobal->weaponUnlockArray[WEAPON_AER14].getValue() != 0 || sGlobal->weaponUnlockArray[WEAPON_GLR].getValue() != 0) {
        laserContainer = wyNode::make();
        laserContainer->setVisible(false);
        sGlobal->mainGameLayer->addChildLocked(laserContainer, container->getZOrder()+1);
        
        laser = wySPXSprite::make("hero1.sprite", false, MyTextureManager::makePNG("hero1.png"), 0);
        laser->setForceTickMode(true);
        laser->setUnitInterval(0.1f);
        laser->setScaleX((wyDevice::winWidth-sGlobal->hero_offset_left)/DP(16));
        laser->setScaleY(1.0f*sGlobal->scaleY);
        laserContainer->addChildLocked(laser);
        laser->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        laser->setAlpha(210);
        
        laserLight = wySPXSprite::make("hero1.sprite", false, MyTextureManager::makePNG("hero1.png"), 0);
        laserLight->setForceTickMode(true);
        laserLight->setUnitInterval(0.12f);
        laserLight->setScaleX(1.0f*sGlobal->scaleX);
        laserLight->setScaleY(1.0f*sGlobal->scaleY);
        laserLight->setLoopCount(-1);
        laserContainer->addChildLocked(laserLight);
        laserLight->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
        laserLight->setAlpha(210);
    } else {
        laserContainer = NULL;
    }
}

void Hero::showGunLight(int weaponIndex) {
    isShowingGunLight = true;

    float s = 0.7*sGlobal->scaleX;
    float s_delta = 0.9*sGlobal->scaleX;
    
    if (gunLightContainer == NULL) {
        gunLightContainer = wyNode::make();
        gunLightContainer->setVisible(false);
        sGlobal->mainGameLayer->addChildLocked(gunLightContainer, container->getZOrder()+1);
        
        gunLight = wySPXSprite::make("hero1.sprite", false, MyTextureManager::makePNG("hero1.png"), SPX_F1_LIGHT1);
        gunLight->setForceTickMode(true);
        gunLight->setUnitInterval(0.1f);
        gunLight->setScaleX(s);
        gunLight->setScaleY(s);
        gunLightContainer->addChildLocked(gunLight);
        gunLight->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    }
    
    gunLight->setRotation(rand()%360);
    gunLight->setPosition(0, 0);
    gunLight->stopAllActions();
    gunLightContainer->stopAllActions();
    
    gunLightContainer->runAction(wySequence::make(wyShow::make(), wyDelayTime::make(0.4f), wyHide::make(), NULL));
    gunLight->runAction(wySequence::make(wyScaleTo::make(0.2f, s, s, s+s_delta, s+s_delta), wyScaleTo::make(0.2f, s+s_delta, s+s_delta, s, s), NULL));
    gunLight->runAction(wyRotateBy::make(0.4f, 40));
    gunLight->runAction(wySequence::make(wyMoveBy::make(0.2f, 0, DP(6)*sGlobal->scaleY), wyMoveBy::make(0.2f, 0, -DP(6)*sGlobal->scaleY), NULL));
    
    if (weaponIndex == WEAPON_TORPEDO) {
        gunLightofx = DP(98)*sGlobal->scaleX;
        gunLightofy = DP(40)*sGlobal->scaleY;
        
        gunLight->playAnimation(SPX_F1_LIGHT1);
        gunLight->setAlpha(240);
    }
    if (weaponIndex == WEAPON_F1) {
        gunLightofx = DP(95)*sGlobal->scaleX;
        gunLightofy = DP(36)*sGlobal->scaleY;
        
        gunLight->playAnimation(SPX_F1_LIGHT1);
        gunLight->setAlpha(240);
    }
    if (weaponIndex == WEAPON_BLASTING) {
        gunLightofx = DP(95)*sGlobal->scaleX;
        gunLightofy = DP(36)*sGlobal->scaleY;
        
        gunLight->playAnimation(SPX_BLASTING_LIGHT);
        gunLight->setAlpha(210);
    }
    
    if (isMotoing) {
        gunLightofx += -DP(40)*sGlobal->scaleX;
        gunLightofy += -DP(8)*sGlobal->scaleY;
    }
    gunLightContainer->setPosition(container->getPositionX()+gunLightofx, container->getPositionY()+gunLightofy);
}

void Hero::showLaser() {
    isShootingLaser = true;
    laserTimer = 0;
    laserContainer->setVisible(true);
    laserCount++;
    
    float sx = (wyDevice::winWidth-sGlobal->hero_offset_left)/DP(16);
    float sy = sGlobal->scaleY*0.7f;
    float sy_delta = 0.3*sGlobal->scaleX;
    
    if (isMotoing) {
        laserofx = (wyDevice::winWidth-sGlobal->hero_offset_left) + DP(0)*sGlobal->scaleX;
        laserofy = DP(35)*sGlobal->scaleY;
        sx *= 2;
    } else {
        laserofx = (wyDevice::winWidth-sGlobal->hero_offset_left)/2 + DP(0)*sGlobal->scaleX;
        laserofy = DP(35)*sGlobal->scaleY;
    }
    
    laserLight->setPosition(-laserofx+DP(0)*sGlobal->scaleX, DP(0)*sGlobal->scaleY);
    
    if (isMotoing) {
        laserofx += -DP(40)*sGlobal->scaleX;
        laserofy += -DP(8)*sGlobal->scaleY;
    }
    
    laser->stopAllActions();
    laser->runAction(wySequence::make(wyScaleTo::make(0.2f, sx, sy, sx, sy+sy_delta), wyScaleTo::make(0.2f, sx, sy+sy_delta, sx, sy), NULL));
    
    float s = 0.5*sGlobal->scaleX;
    float s_delta = 0.5*sGlobal->scaleX;
    laserLight->stopAllActions();
    laserLight->runAction(wySequence::make(wyScaleTo::make(0.2f, s, s, s+s_delta, s+s_delta), wyScaleTo::make(0.2f, s+s_delta, s+s_delta, s, s), NULL));
    
    if (laserBullet != NULL) {
        sGlobal->mainGameLayer->addToDestroy(laserBullet->body);
    }
    laserBullet = HeroBullet::make(sGlobal->activeWeapon);
    
    switch (sGlobal->activeWeapon) {
        case WEAPON_AER14:
            laser->playAnimation(SPX_AER14_LIGHT1);
            laserLight->playAnimation(SPX_AER14_LIGHT2);
            
            laserofx += DP(80)*sGlobal->scaleX;
            laserofy += DP(0)*sGlobal->scaleY;
            break;
        case WEAPON_GLR:
            laser->playAnimation(SPX_GLR_LIGHT1);
            laserLight->playAnimation(SPX_GLR_LIGHT2);
            
            laserofx += DP(90)*sGlobal->scaleX;
            laserofy -= DP(6)*sGlobal->scaleY;
            break;
        default:
            break;
    }
    laserContainer->setPosition(container->getPositionX() + laserofx, container->getPositionY()+laserofy);
}

void Hero::createMotoParts() {
    spx_moto_body = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), SPX_MOTO_BODY);
    spx_moto_b_wheel = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), SPX_MOTO_B_WHEEL);
    spx_moto_f_wheel = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), SPX_MOTO_F_WHEEL);
    
    spx_moto_body->setForceTickMode(true);
	spx_moto_body->setUnitInterval(0.05f);
    spx_moto_body->setScaleX(0.62f*sGlobal->scaleX);
    spx_moto_body->setScaleY(0.62f*sGlobal->scaleY);
    
    spx_moto_b_wheel->setForceTickMode(true);
	spx_moto_b_wheel->setUnitInterval(0.05f);
    spx_moto_b_wheel->setScaleX(0.62f*sGlobal->scaleX);
    spx_moto_b_wheel->setScaleY(0.62f*sGlobal->scaleY);
    
    spx_moto_f_wheel->setForceTickMode(true);
	spx_moto_f_wheel->setUnitInterval(0.05f);
    spx_moto_f_wheel->setScaleX(0.62f*sGlobal->scaleX);
    spx_moto_f_wheel->setScaleY(0.62f*sGlobal->scaleY);
    
    spx_moto_body->setLoopCount(0);
    spx_moto_b_wheel->setLoopCount(0);
    spx_moto_f_wheel->setLoopCount(0);
    
    spx_moto_body->setVisible(false);
    spx_moto_b_wheel->setVisible(false);
    spx_moto_f_wheel->setVisible(false);
    
    moto_b_wheel_container = wyNode::make();//wyColorLayer::make();//
//    moto_b_wheel_container->setColor(wyc4b(255, 0, 0, 100));
    moto_b_wheel_container->addChildLocked(spx_moto_b_wheel);
    moto_b_wheel_container->setContentSize(0, spx_moto_b_wheel->getFrameRect().height*spx_moto_b_wheel->getScaleY());
    moto_b_wheel_container->setAnchor(0.5f, 0.0);
    moto_b_wheel_container->setPosition(0, DP(-40)*spx_moto_b_wheel->getScaleY()-DP(10)*sGlobal->scaleY);
    spx_moto_b_wheel->setPosition(0, DP(40)*spx_moto_b_wheel->getScaleY());
    
    moto_f_wheel_container = wyNode::make();//wyColorLayer::make();//wyNode::make();
//    moto_f_wheel_container->setColor(wyc4b(255, 0, 0, 100));
    moto_f_wheel_container->addChildLocked(spx_moto_f_wheel);
    moto_f_wheel_container->setContentSize(0, spx_moto_f_wheel->getFrameRect().height*spx_moto_f_wheel->getScaleY());
    moto_f_wheel_container->setAnchor(0.5f, 0.0);
    moto_f_wheel_container->setPosition(0, DP(-36)*spx_moto_f_wheel->getScaleY()-DP(10)*sGlobal->scaleY);
    spx_moto_f_wheel->setPosition(0, DP(-36)*spx_moto_f_wheel->getScaleY());
    
//    spx_moto_speed_line = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), 31);
//    spx_moto_speed_line->setForceTickMode(true);
//	spx_moto_speed_line->setUnitInterval(0.1f);
//    spx_moto_speed_line->setScaleX(1.0f*sGlobal->scaleX);
//    spx_moto_speed_line->setScaleY(1.0f*sGlobal->scaleY);
    
    container->addChildLocked(spx_moto_body, 1);
    container->addChildLocked(moto_b_wheel_container, 0);
    container->addChildLocked(moto_f_wheel_container, 0);
    
    isMotoRunning = false;
    boost_emitter = NULL;
    motoBody = NULL;
    
    isShootingLaser = false;
}

static void onWheelRotateUpdated(wyAction* action, float t, void* data) {
    Hero* hero = (Hero*)data;
    float ofy = 0;
    
    if (sGlobal->jumpTime > 0) {
        return;
    } else {
        wyAction *a = hero->spx_moto_body->getAction(999);
        if (a != NULL && !a->isDone()) {
            return;
        }
    }
    
//    if (t < 0.25f) {
//        ofy = 0;
//    }
//    else if (t >= 0.25f && t < 0.5f) {
//        ofy = DP(1)*sGlobal->scaleY;
//    }
//    else if (t >= 0.5f && t < 0.75f) {
//        ofy = 0;
//    }
//    else if (t >= 0.75f && t <= 1.0f) {
//        ofy = DP(1)*sGlobal->scaleY;
//    }
    
    if (t < 0.5f) {
        ofy = 0;
    }
    else {
        ofy = DP(2)*sGlobal->scaleY;
    }
    
    hero->spx_hero_body->setPosition(hero->spx_hero_body->getPositionX(), hero->oy1 + ofy);
    hero->spx_hero_r_foot->setPosition(hero->spx_hero_r_foot->getPositionX(), hero->oy2 + ofy);
    hero->spx_moto_body->setPosition(hero->spx_moto_body->getPositionX(), hero->oy3 + ofy);
    hero->moto_b_wheel_container->setPosition(hero->moto_b_wheel_container->getPositionX(), hero->oy4 + ofy);
    hero->moto_f_wheel_container->setPosition(hero->moto_f_wheel_container->getPositionX(), hero->oy5 + ofy);
}

void Hero::showMoto() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    int ofx = -DP(40)*sGlobal->scaleX;
    int ofy = -DP(8)*sGlobal->scaleY;
    
    spx_hero_l_foot->setVisible(false);
    
//    spx_moto_b_wheel->setPosition(-DP(26)*sGlobal->scaleX+ofx, -DP(20)*sGlobal->scaleY+ofy);
//    spx_moto_f_wheel->setPosition(DP(114)*sGlobal->scaleX+ofx, -DP(18)*sGlobal->scaleY+ofy);
    moto_b_wheel_container->setPosition(-DP(26)*sGlobal->scaleX+ofx, -DP(36)*spx_moto_b_wheel->getScaleY()+ofy-DP(10)*sGlobal->scaleY);
    moto_f_wheel_container->setPosition(DP(114)*sGlobal->scaleX+ofx, -DP(40)*spx_moto_f_wheel->getScaleY()+ofy-DP(10)*sGlobal->scaleY);
    
    spx_moto_b_wheel->setPosition(0, DP(36)*spx_moto_b_wheel->getScaleY()+ofy);
    spx_moto_f_wheel->setPosition(0, DP(40)*spx_moto_f_wheel->getScaleY()+ofy);
    
    spx_moto_body->setPosition(ofx, ofy);
    
    spx_moto_body->setVisible(true);
    spx_moto_b_wheel->setVisible(true);
    spx_moto_f_wheel->setVisible(true);
    
    spx_hero_r_foot->playAnimation(SPX_MOTO_R_LEG);
    wyIntervalAction *a = wyRotateBy::make(0.3f, 360);
    wyActionCallback callback = {
        NULL,
        NULL,
        onWheelRotateUpdated,
    };
    a->setCallback(&callback, this);
    spx_moto_b_wheel->runAction(wyRepeatForever::make(a));
    spx_moto_f_wheel->runAction(wyRepeatForever::make(wyRotateBy::make(0.3f, 360)));
    
    spx_hero_body->setPosition(ofx, ofy);
    spx_hero_r_foot->setPosition(ofx, ofy);
    
    shadow->setScaleX(sGlobal->scaleX*6.8f);
    
    b2PolygonShape box;
    box.SetAsBox(m_box2d->pixel2Meter(DP(100)*sGlobal->scaleX), m_box2d->pixel2Meter(DP(30)*sGlobal->scaleY), b2Vec2(m_box2d->pixel2Meter(-DP(0)*sGlobal->scaleX), m_box2d->pixel2Meter(-DP(0)*sGlobal->scaleX)), 0);
    b2FixtureDef fd;
    fd.shape = &box;
    fd.restitution = 0.0f;
    fd.friction = 0.0f;
    fd.density = 0.0f;
    fd.isSensor = false;
    fd.filter.maskBits = MASK_BITS[TYPE_MAO];
    fd.filter.categoryBits = CAT_BITS[TYPE_MAO];
    fd.filter.groupIndex = GROUND_INDEX[TYPE_MAO];
    
    motoFixture = body->CreateFixture(&fd);
    
    oy1 = spx_hero_body->getPositionY();
    oy2 = spx_hero_r_foot->getPositionY();
    oy3 = spx_moto_body->getPositionY();
    oy4 = moto_b_wheel_container->getPositionY();
    oy5 = moto_f_wheel_container->getPositionY();
    
    isMotoRunning = false;
    
//    laserContainer->setVisible(false);
    if (laserContainer != NULL && laserContainer->isVisible()) {
        laserofx += -DP(40)*sGlobal->scaleX;
        laserofy += -DP(8)*sGlobal->scaleY;
    }
    
    if (gunLightContainer != NULL && gunLightContainer->isVisible()) {
        gunLightofx += -DP(40)*sGlobal->scaleX;
        gunLightofy += -DP(8)*sGlobal->scaleY;
    }
    
    speedline_emitter = wyParticleLoader::load("particle_speedline.plist");
    speedline_emitter->setScaleX(sGlobal->scaleX*1.0f);
    speedline_emitter->setScaleY(sGlobal->scaleY*1.0f);
    sGlobal->mainGameLayer->addChildLocked(speedline_emitter, 100);
    speedline_emitter->setPositionType(GROUPED);
//    speedline_emitter->setDuration(10);
//    speedline_emitter->setVisible(false);
}

static void onHideMotoEnded(wyAction* action, void* data) {
//    Hero *hero = (Hero *)data;
//    
//    hero->spx_moto_body->setVisible(false);
//    hero->spx_moto_b_wheel->setVisible(false);
//    hero->spx_moto_f_wheel->setVisible(false);
//    hero->isMotoRunning = false;
//    
//    sGlobal->mainGameLayer->removeChildLocked(hero->boost_emitter, true);
//    hero->boost_emitter = NULL;
}

void Hero::hideMoto() {
//    sGlobal->mainGameLayer->needDestroyMoto = true;
    if (motoFixture != NULL) {
        sGlobal->mainGameLayer->addToDestroyFixture(motoFixture);
        motoFixture = NULL;
    }
    
    isMotoing = false;
    container->stopAllActions();
    spx_hero_body->stopAllActions();
    spx_moto_b_wheel->stopAllActions();
    spx_moto_f_wheel->stopAllActions();
    spx_hero_r_foot->stopAllActions();
    spx_hero_l_foot->stopAllActions();
    
    container->setAlpha(255);
    spx_hero_body->setAlpha(255);
    spx_moto_b_wheel->setAlpha(255);
    spx_moto_f_wheel->setAlpha(255);
    spx_hero_r_foot->setAlpha(255);
    spx_hero_l_foot->setAlpha(255);
    
    container->setRotation(0);
    spx_hero_body->setRotation(0);
    
    spx_hero_r_foot->setPosition(0, 0);
    spx_hero_l_foot->setVisible(true);
    
//    spx_moto_body->setVisible(false);
//    spx_moto_b_wheel->setVisible(false);
//    spx_moto_f_wheel->setVisible(false);
    
    spx_hero_body->setPosition(0, 0);
    spx_hero_r_foot->setPosition(0, 0);
    spx_hero_l_foot->setPosition(0, 0);
    
    spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_JUMP4);
    spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_JUMP4);
    
    if (!isFlying) {
        b2Vec2 f = sGlobal->mainGameLayer->hero->body->GetWorldVector(b2Vec2(0.0f, 30));
        b2Vec2 p = sGlobal->mainGameLayer->hero->body->GetWorldPoint(b2Vec2(0.0f, 0.0f));
        b2Vec2 vel = sGlobal->mainGameLayer->hero->body->GetLinearVelocity();
        sGlobal->mainGameLayer->hero->body->SetLinearVelocity(b2Vec2(vel.x, 0));
        sGlobal->mainGameLayer->hero->body->ApplyLinearImpulse(f, p);
    }
    
//    if (sGlobal->isOnGround) {
//        wyIntervalAction *a = wyMoveBy::make(3.0f, wyDevice::winWidth*2, 0);
//        wyActionCallback callback = {
//            NULL,
//            onHideMotoEnded
//        };
//        a->setCallback(&callback, this);
//        spx_moto_body->runAction(a);
//        
//        wyIntervalAction *a1 = wyMoveBy::make(3.0f, wyDevice::winWidth*2, 0);
//        spx_moto_b_wheel->runAction(a1);
//        spx_moto_f_wheel->runAction(a1->copy());
//    } else {
//        wyIntervalAction *a = wyMoveBy::make(3.0f, wyDevice::winWidth*2, DP(-100)*sGlobal->scaleY);
//        wyActionCallback callback = {
//            NULL,
//            onHideMotoEnded
//        };
//        a->setCallback(&callback, this);
//        spx_moto_body->runAction(a);
//        
//        wyIntervalAction *a1 = wyMoveBy::make(3.0f, wyDevice::winWidth*2, DP(-100)*sGlobal->scaleY);
//        spx_moto_b_wheel->runAction(a1);
//        spx_moto_f_wheel->runAction(a1->copy());
//    }
    isMotoRunning = true;
    
    shadow->setScaleX(sGlobal->scaleX*3.2f);
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    motoBody = new LiuDan(container->getPositionX(), baseY, true);
    motoBody->container->setVisible(false);
    motoBody->body->SetTransform(b2Vec2(m_box2d->pixel2Meter(container->getPositionX()-DP(0)*sGlobal->scaleX), m_box2d->pixel2Meter(container->getPositionY() + DP(0)*sGlobal->scaleY)), 0);
    motoBody->body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x*4.5, 0));
//    motoBody->body->ApplyLinearImpulse(b2Vec2(0, 0), b2Vec2(0,0));
    motoBody->body->SetGravityScale(0.1f);
//    sGlobal->mainGameLayer->removeChildLocked(boost_emitter, true);
//    boost_emitter = NULL;
    
    
//    laserContainer->setVisible(false);
    if (laserContainer != NULL && laserContainer->isVisible()) {
        laserofx -= -DP(40)*sGlobal->scaleX;
        laserofy -= -DP(8)*sGlobal->scaleY;
    }
    
    if (gunLightContainer != NULL && gunLightContainer->isVisible()) {
        gunLightofx -= -DP(40)*sGlobal->scaleX;
        gunLightofy -= -DP(8)*sGlobal->scaleY;
    }
    
    sGlobal->mainGameLayer->removeChildLocked(speedline_emitter, true);
    speedline_emitter = NULL;
}

void Hero::setHeadRotation(float angel, bool isShoot, bool isHurt) {
    float m_angle = -99;
    if (isShoot) {
        spx_hero_body->setRotation(angel);
        isHurtRot = false;
        hurtRotTimer = 0;
        m_angle = angel;
    }
    else if (isHurt) {
        spx_hero_body->setRotation(angel);
        isHurtRot = true;
        hurtRotTimer = 0;
        m_angle = angel;
    }
    else if (!isShooting && !isHurtRot) {
        spx_hero_body->setRotation(angel);
        m_angle = angel;
    }
    
    if (m_angle != -99 && isFlying) {
        ying->setRotation(m_angle);
    }
}

void Hero::showShield() {
    if (!shieldContainer->isVisible()) {
        sGlobal->playEffect(se_absorb);
        
        shieldContainer->setPosition(container->getPositionX()+DP(2)*sGlobal->scaleY, container->getPositionY()+DP(10)*sGlobal->scaleY);
        shieldContainer->setVisible(true);
        
        shieldSprite_0->stopAllActions();
        shieldSprite_0->setScaleX(1.1f*sGlobal->scaleX);
        shieldSprite_0->setScaleY(1.1f*sGlobal->scaleY);
        wyIntervalAction *a = wyScaleTo::make(0.5f, shieldSprite_0->getScaleX()*0.95f, shieldSprite_0->getScaleY()*0.95f, shieldSprite_0->getScaleX()*1.05f, shieldSprite_0->getScaleY()*1.05f);
        shieldSprite_0->runAction(wyRepeatForever::make(wySequence::make((wyIntervalAction *)a->reverse(), a, NULL)));
    }
}

void Hero::onDodgeEnd(wyTargetSelector* ts) {
    spx_hero_body->setColor(wyc3b(255, 255, 255));
    spx_hero_l_foot->setColor(wyc3b(255, 255, 255));
    spx_hero_r_foot->setColor(wyc3b(255, 255, 255));
    
//    spx_hero_body->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
//    spx_hero_l_foot->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
//    spx_hero_r_foot->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
}

void Hero::showDodge() {
    if (dodgeContainer != NULL && !dodgeContainer->isVisible()) {
        sGlobal->playEffect(se_absorb);
        
        dodgeContainer->setPosition(container->getPositionX(), container->getPositionY());
        dodgeContainer->setVisible(true);
        dodgeContainer->runAction(wySequence::make(wyDelayTime::make(1.0f), wyHide::make(), NULL));
        dodgeContainer->runAction(wySequence::make(wyDelayTime::make(0.5f), wyCallFunc::make(wyTargetSelector::make(this, SEL(Hero::onDodgeEnd))), NULL));
        
        dodgeTitle->setPosition(0,0);
        dodgeTitle->setAlpha(255);
        dodgeTitle->runAction(wyMoveBy::make(0.3f, 0, DP(20)*sGlobal->scaleY));
        dodgeTitle->runAction(wySequence::make(wyDelayTime::make(0.9f), wyFadeOut::make(0.1f), NULL));
        
        float fx = spx_hero_body->getScaleX();
        float tx = spx_hero_body->getScaleX()+0.3*sGlobal->scaleX;
        
        float dur = 0.5f;
        float delay = 0.2f;
        int texId = getWeaponTexIndex(sGlobal->activeWeapon);
        
        illusion1->syncAnimation(spx_hero_body->getCurrentAnimationIndex(), spx_hero_l_foot->getCurrentAnimationIndex(), spx_hero_r_foot->getCurrentAnimationIndex(), texId);
        illusion1->runAction(wyScaleTo::make(dur, fx, fx, tx, tx));
        illusion1->runAction(wySequence::make(wyShow::make(), wyFadeTo::make(dur, 255, 0, true), wyHide::make(), NULL));
        illusion1->setScale(fx);
        
        illusion2->syncAnimation(spx_hero_body->getCurrentAnimationIndex(), spx_hero_l_foot->getCurrentAnimationIndex(), spx_hero_r_foot->getCurrentAnimationIndex(), texId);
        illusion2->runAction(wySequence::make(wyDelayTime::make(delay), wyShow::make(), wyScaleTo::make(dur, fx, fx, tx, tx), NULL));
        illusion2->runAction(wySequence::make(wyDelayTime::make(delay), wyFadeTo::make(dur, 255, 0, true), wyHide::make(), NULL));
        illusion2->setScale(fx);
        
        illusion3->syncAnimation(spx_hero_body->getCurrentAnimationIndex(), spx_hero_l_foot->getCurrentAnimationIndex(), spx_hero_r_foot->getCurrentAnimationIndex(), texId);
        illusion3->runAction(wySequence::make(wyDelayTime::make(delay*2), wyShow::make(), wyScaleTo::make(dur, fx, fx, tx, tx), NULL));
        illusion3->runAction(wySequence::make(wyDelayTime::make(delay*2), wyFadeTo::make(dur, 255, 0, true), wyHide::make(), NULL));
        illusion3->setScale(fx);
        
        spx_hero_body->setColor(wyc3b(100, 150, 255));
        spx_hero_l_foot->setColor(wyc3b(100, 150, 255));
        spx_hero_r_foot->setColor(wyc3b(100, 150, 255));
        
//        spx_hero_body->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
//        spx_hero_l_foot->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
//        spx_hero_r_foot->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    }
}

void Hero::recoverHP(float dt) {
//    if (sGlobal->hasSkillMulti[SKILL_4]) {
//        recoverHPTimer+=dt;
//        if (recoverHPTimer > ELEMENT_SKILL_4_PARAM1) {
//            recoverHPTimer = 0;
//            int recoverHP = MIN(sGlobal->cloth_hp-sGlobal->curHP, ceil(sGlobal->cloth_hp*sGlobal->skillMulti[SKILL_4]));
//            sGlobal->hudLayer->setHPBar(-recoverHP);
////            LOGE("recover HP: %d", recoverHP);
//            
//            showRecoverHPEffect(recoverHP);
//        }
//    }
}

void Hero::showRecoverHPEffect(int recoverHP, bool hasHud) {
    sGlobal->playEffect(se_heal);
    if (hasHud) {
        sGlobal->hudLayer->showHealEffect();
    }
    
    wyNode *hpContainer = wyNode::make();
    container->addChildLocked(hpContainer, 103);
    
    wySpriteEx *hpIcon = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "heart.png");
    hpIcon->setScaleX(0.7f*sGlobal->scaleX);
    hpIcon->setScaleY(0.7f*sGlobal->scaleY);
    hpIcon->setPosition(0, DP(0));
    hpContainer->addChildLocked(hpIcon);
    
    char temp[10];
    sprintf(temp, "+%d", recoverHP);

    wyBitmapFontLabel *hpTitle = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
    hpTitle->setScale(SP(24)*sGlobal->scaleX/sGlobal->descFontSize);
    hpTitle->setAlignment(wyBitmapFontLabel::LEFT);
    hpTitle->setColor(hpColor);
    hpTitle->setPosition(DP(40)*sGlobal->scaleX, 0);
    hpContainer->addChildLocked(hpTitle);
    
    hpContainer->setPosition(-DP(10)*sGlobal->scaleX, DP(60)*sGlobal->scaleY);

    wyIntervalAction *action = wySequence::make(wyDelayTime::make(0.2f), wyMoveBy::make(1.0f, 0, DP(30)*sGlobal->scaleY), NULL);
    wyActionCallback callback = {
        NULL,
        removeMyselfonAnimEnded
    };
    action->setCallback(&callback, hpContainer);
    hpContainer->runAction(action);
    hpIcon->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(1.0f), NULL));
    hpTitle->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(1.0f), NULL));
}

void Hero::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
//    LOGE("vely:%f", body->GetLinearVelocity().y);
    if (!sGlobal->isOnGround && fabs(body->GetLinearVelocity().y)<1.0f) {
        previousY = body->GetPosition().y;
//        LOGE("prevY:%f", previousY);
    }
    if (needSaveMe) {
        if (saveType == 1) {
            if (!isSaveMeInited) {
                isSaveMeInited = true;
                isYingReleased = false;

//                ying->setPosition(sGlobal->mainGameLayer->hero->container->getPositionX() + DP(26)*sGlobal->scaleX, sGlobal->mainGameLayer->hero->container->getPositionY() + DP(80)*sGlobal->scaleY + spx_hero_body->getPositionY());
                ying->setPosition(container->getPositionX() + DP(-6)*sGlobal->scaleX, container->getPositionY() + DP(22)*sGlobal->scaleY + spx_hero_body->getPositionY());
                
                ying->stopAllActions();
                ying->setAlpha(255);
                ying->setVisible(true);
                yingSprite->stopAllActions();
                yingSprite->setAlpha(255);
                yingSprite->setVisible(true);
                
//                fly_emitter = wyParticleLoader::load("particle_ying.plist");
//                fly_emitter->setScaleX(sGlobal->scaleX*1.0f);
//                fly_emitter->setScaleY(sGlobal->scaleY*1.0f);
//                ying->addChildLocked(fly_emitter, 0);
//                fly_emitter->setPositionType(FREE);
                return;
            }
            needSaveMe = false;
            
            sGlobal->mainGameLayer->addToEnable(body);
            sGlobal->jumpTime = 0;
            sGlobal->jumpTime1 = 0;
            sGlobal->hudLayer->isJumping = false;
            sGlobal->hudLayer->isReleased = true;
            isDead = false;
            isRunning = true;
            
            isSavingMe = true;
//            isWudi = true;
            
            wyIntervalAction* a = wyFadeTo::make(0.1f, 255, 100);
            wyIntervalAction* r = wySequence::make(a,
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   a->copy(),
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   a->copy(),
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   a->copy(),
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   a->copy(),
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   a->copy(),
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   NULL);
            r->setTag(100);
            spx_hero_body->runAction(r);
            spx_hero_l_foot->runAction(r->copy());
            spx_hero_r_foot->runAction(r->copy());
            
            sGlobal->curHP = sGlobal->cloth_hp;
            sGlobal->hudLayer->setHPBar(0);
            
            doFly(false);
            isStartFlyingEnd = false;
            flyDuration = HERO_SAVE_ME_FLY_TIME;
            body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, HERO_FLY_Y_UP*2));
            
            doSandShield();
            sGlobal->sand_shield_cd = HERO_SAVE_ME_FLY_TIME;
        } else {
            needSaveMe = false;
            
            sGlobal->curHP = sGlobal->cloth_hp;
            sGlobal->hudLayer->setHPBar(0);
            
//            sGlobal->mainGameLayer->addToEnable(body);
//            sGlobal->jumpTime = 0;
//            sGlobal->jumpTime1 = 0;
//            sGlobal->hudLayer->isJumping = false;
//            sGlobal->hudLayer->isReleased = true;
//            isDead = false;
//            isRunning = true;
//            
//            isSavingMe = true;
//            isWudi = true;
            
            wyIntervalAction* a = wyFadeTo::make(0.1f, 255, 100);
            wyIntervalAction* r = wySequence::make(a,
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   a->copy(),
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   a->copy(),
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   a->copy(),
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   a->copy(),
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   a->copy(),
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   NULL);
            r->setTag(100);
            spx_hero_body->runAction(r);
            spx_hero_l_foot->runAction(r->copy());
            spx_hero_r_foot->runAction(r->copy());
            
            doSandShield();
            sGlobal->sand_shield_cd = HERO_SAVE_ME_FLY_TIME;
            
            if (!isFlying) {
                body->SetGravityScale(0);
                doFly(false);
                flyDuration = HERO_SAVE_ME_FLY_TIME;
                
                yingSprite->setRotation(20);
                yingSprite->setPosition(DP(27)*sGlobal->scaleX, -DP(22)*sGlobal->scaleX);
            } else {
                flyTimer = MAX(0, flyTimer-2.0f);
            }
            body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, 0));

//            if (buff_emitter != NULL) {
//                sGlobal->mainGameLayer->removeChildLocked(buff_emitter, true);
//            }
//            buff_emitter = wyParticleLoader::load("particle_active_buff.plist");
//            buff_emitter->setScale(2.2f*sGlobal->scaleX);
//            buff_emitter->setPosition(container->getPositionX()+DP(10)*sGlobal->scaleY, container->getPositionY()+DP(25)*sGlobal->scaleY);
//            buff_emitter->setPositionType(GROUPED);
//            setEmitterColor(sGlobal->activeElement, buff_emitter);
//            sGlobal->mainGameLayer->addChildLocked(buff_emitter, container->getZOrder()-1);
            
//            spx_hero_body->setLoopCount(-1);
//            spx_hero_body->setUnitInterval(0.05f);
//
//            spx_hero_r_foot->setLoopCount(-1);
//            spx_hero_r_foot->setUnitInterval(0.05f);
//
//            spx_hero_l_foot->setLoopCount(-1);
//            spx_hero_l_foot->setUnitInterval(0.05f);
            
            spx_hero_body->setPosition(0, DP(0)*sGlobal->scaleY);
            spx_hero_r_foot->setPosition(0, DP(0)*sGlobal->scaleY);
            spx_hero_l_foot->setPosition(0, DP(0)*sGlobal->scaleY);

//            if (isFlying) {
//                yingSprite->setRotation(20);
//                yingSprite->setPosition(DP(12)*sGlobal->scaleX, -DP(15)*sGlobal->scaleX);
//            }
            sGlobal->hudLayer->setHudOp(false, false, false, false, false, false);
            spx_hero_body->playAnimation(spxFHIndex);
            spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_FH);
            spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_FH);
        }
    }
    
    if (!body->IsActive() && !isBackingToNormal) {
        return;
    }
    
    float prevX = container->getPositionX();
//    float prevY = container->getPositionY();
    float oldX = sGlobal->mainGameLayer->getPositionX();
    container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
    
    if (isRunning && !isBoosting && !isDeBoosting && body->GetLinearVelocity().x < sGlobal->heroVelocityX) {
        body->SetLinearVelocity(b2Vec2(sGlobal->heroVelocityX, body->GetLinearVelocity().y));
        //LOGE("hero stucked...");
    }
    
    float diffY = MIN(wyDevice::winHeight / 2.0f - container->getPositionY(), 0);
//    if ((!isDead && ((!isBoosting && !isDeBoosting && !isBackingToNormal) || (isFlying && isDeBoosting))) || isInDead) {// || (isFlying && isDeBoosting)
//        if (diffY < 0) {
////            LOGE("diffY:%f", diffY);
//            sGlobal->heroEyeZ = -diffY/(HERO_YING_MAX_HEIGHT*sGlobal->scaleY/99.0f);
//        } else {
//            sGlobal->heroEyeZ = 1;
//        }
//        if (sGlobal->bossEyeZ < sGlobal->heroEyeZ && !isDeBoosting) {
////            LOGE("eye:%f", sGlobal->heroEyeZ);
//            sGlobal->mainGameLayer->getCamera()->setEye(0, sGlobal->heroEyeZ/2.0f, sGlobal->heroEyeZ);
//            sGlobal->mainGameLayer->getCamera()->setCenter(0, sGlobal->heroEyeZ/2.0f, 0);
//        }
//    }

    if ((container->getPositionY() < normalDeathY || (isFlying && container->getPositionY() < flyDeathY)) && !isDead && !isSavingMe) {//&& !isSandShieldGoning
        
        float noticeTime;
        if (isBoosting || isDeBoosting) {
            isBoosting = false;
            isDeBoosting = false;
            boostTimer = 0;
            deBoostTimer = 0;
            
            isBackingToNormal = true;
            backToNormalSpeed = 5;
//            noticeTime = 0.8f;
            float z = MAX(MAX(heroZ, sGlobal->heroEyeZ), sGlobal->bossEyeZ);
            heroZ = z;
            sGlobal->heroEyeZ = z;
            
            if (boost_emitter!=NULL) {
                sGlobal->mainGameLayer->removeChildLocked(boost_emitter, true);
            }
			boost_emitter = NULL;
            noticeTime = 0.4f;
        } else {
            noticeTime = 0.2f;
        }
//        LOGE("I'm dead, fall");
//        sGlobal->deadType = HERO_DEAD_TYPE_FALL;
        
//        sGlobal->dead_position_x = container->getPositionX()/sGlobal->tileWidth/sGlobal->scaleX;
//        
//        sGlobal->reportGameEvent(EVT_DEATH);
//        sGlobal->deadType = -1;
//        sGlobal->dead_position_x = -1;
        
        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        body->SetGravityScale(0.0f);
        sGlobal->mainGameLayer->m_box2d->getWorld()->ClearForces();
        
//        timer = wyTimer::make(wyTargetSelector::make(this, SEL_i(Hero::doDeadLogic), 1), noticeTime);
//        timer->setOneShot(true);
//        sGlobal->mainGameLayer->scheduleLocked(timer);
//        doDeadLogic(NULL, 1);
        sGlobal->mainGameLayer->runAction(wySequence::make(wyDelayTime::make(noticeTime), wyCallFunc::make(wyTargetSelector::make(this, SEL_i(Hero::doDeadLogic), 1)), NULL));
        
        isDead = true;
        isRunning = false;
        sGlobal->mainGameLayer->addToDisable(body);

		sGlobal->playEffect(se_player_dead);

        sGlobal->hudLayer->setHudOp(false, false, false, false, false, false);
        if (isFlying) {
            hideYing();
        }
        
//        isFlying = false;
//        ying->setVisible(false);
//        isDeFlying = false;
//        flyTimer = 0;

        isSandShieldActive = false;
        isSandShieldGoning = false;
        sandShieldTimer = 0;
//        sprite_mao_tou->setScaleX(sGlobal->scaleX*0.6f);
//        sprite_mao_tou->setScaleY(sGlobal->scaleY*0.6f);
//        
//        sprite_mao_foot->setScaleX(sGlobal->scaleX*0.6f);
//        sprite_mao_foot->setScaleY(sGlobal->scaleY*0.6f);
//        playHeroAnimation(SPX_HERO);
//        spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_JUMP1);
//        spx_hero_l_foot->playAnimation(SPX_HERO_R_LEG_JUMP1);
        playHeroAnimation(SPX_HERO_R_LEG_JUMP1);
        spx_hero_body->setRotation(0);
//        sGlobal->mainGameLayer->needDestroyMoto = true;
//        sGlobal->mainGameLayer->needDestroyShield = true;
        
        if (motoFixture != NULL) {
            sGlobal->mainGameLayer->addToDestroyFixture(motoFixture);
            motoFixture = NULL;
        }
        if (shieldFixture != NULL) {
            sGlobal->mainGameLayer->addToDestroyFixture(shieldFixture);
            shieldFixture = NULL;
        }
    }
    
    float nextX;
    if (sGlobal->mainGameLayer->isEndless) {
        nextX = MIN(-container->getPositionX() + sGlobal->hero_offset_left, 0);
        //sharyu: 将map移动到map1的右侧
        if (sGlobal->mainGameLayer->m_TileMap1->getPositionX() > sGlobal->mainGameLayer->m_TileMap->getPositionX() && nextX + dist266 < -sGlobal->mainGameLayer->m_TileMap1->getPositionX()) {
            float newX = sGlobal->mainGameLayer->m_TileMap1->getPositionX() + sGlobal->tileMapWidthInPixels1;
            float diffX = newX - sGlobal->mainGameLayer->m_TileMap->getPositionX();
            sGlobal->mainGameLayer->m_TileMap->setPosition(newX, sGlobal->tileMapOffsetY);
            std::vector<b2Body *>::iterator pos2;
            for(pos2 = sGlobal->mainGameLayer->groundBody.begin(); pos2 != sGlobal->mainGameLayer->groundBody.end(); ++pos2) { 
                b2Body *body = *pos2;
                b2Vec2 pos = body->GetPosition();
                body->SetTransform(b2Vec2(m_box2d->pixel2Meter(diffX) + pos.x, pos.y), 0);
                
                wyNode *node = (wyNode *) body->GetUserData();
                if (node != NULL) {
                    node->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
                }
            }
        }
        //sharyu: 将map1移动到map右侧
        if (sGlobal->mainGameLayer->m_TileMap->getPositionX() > sGlobal->mainGameLayer->m_TileMap1->getPositionX() && nextX + dist266 < -sGlobal->mainGameLayer->m_TileMap->getPositionX()) {
            float newX = sGlobal->mainGameLayer->m_TileMap->getPositionX() + sGlobal->tileMapWidthInPixels;
            float diffX = newX - sGlobal->mainGameLayer->m_TileMap1->getPositionX();
            sGlobal->mainGameLayer->m_TileMap1->setPosition(newX, sGlobal->tileMapOffsetY);
            std::vector<b2Body *>::iterator pos2;
            for(pos2 = sGlobal->mainGameLayer->groundBody1.begin(); pos2 != sGlobal->mainGameLayer->groundBody1.end(); ++pos2) { 
                b2Body *body = *pos2;
                b2Vec2 pos = body->GetPosition();
                body->SetTransform(b2Vec2(m_box2d->pixel2Meter(diffX) + pos.x, pos.y), 0);
                
                wyNode *node = (wyNode *) body->GetUserData();
                if (node != NULL) {
                    node->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
                }
            }
        }
    }
    else if (sGlobal->mainGameLayer->isRealEndless) {
        if (sGlobal->elapsedNormalCombat == sGlobal->MAX_COMBAT_TIME+1) {
            nextX = MIN(-container->getPositionX() + sGlobal->hero_offset_left, 0);
            float maxX = -sGlobal->realEndlessMapWidth + wyDevice::winWidth+DP(150)*sGlobal->scaleX;
            if (nextX < maxX) {
                nextX = maxX;
                needDestroy = false;
            }
            //LOGE("x1:%f, x2:%f, x3:%f", nextX, -sGlobal->realEndlessMapWidth+wyDevice::winWidth, sprite->getPositionX());
            if (container->getPositionX() >= sGlobal->realEndlessMapWidth-DP(60)*sGlobal->scaleX) {
                if (sGlobal->elapsedNormalCombat == sGlobal->MAX_COMBAT_TIME+1) {
                    //1轮结束
                    if (sGlobal->combatPeriod >= 3) {
                        //增加轮次
                        sGlobal->combatRound += 1;
                        //重置场景计数
                        sGlobal->combatPeriod = 1;
                        //重新随机下一轮场景出现次序
                        sGlobal->seasonSeq = POSSIBLE_COMBO[rand()%6];
                        //重新计算combatvalue
                        sGlobal->calcEnemyCombatValue();
                    } else {
                        //场景数+1
                        sGlobal->combatPeriod += 1;
                    }
                    //计算当前战斗值
                    sGlobal->calcCurrentCombatValue();
                    //重置小地图计数器
                    sGlobal->elapsedNormalCombat = 1;
                    
                    switch (sGlobal->combatPeriod) {
                        case 1:
                            sGlobal->currentSeasonId = sGlobal->seasonSeq / 100;
                            sGlobal->currentBossId = sGlobal->bossSeq / 100;
                            break;
                        case 2:
                            sGlobal->currentSeasonId = sGlobal->seasonSeq / 10 - sGlobal->seasonSeq / 100 * 10;
                            sGlobal->currentBossId = sGlobal->bossSeq / 10 - sGlobal->bossSeq / 100 * 10;
                            break;
                        case 3:
                            sGlobal->currentSeasonId = sGlobal->seasonSeq % 10;
                            sGlobal->currentBossId = sGlobal->bossSeq % 10;
                            break;
                        default:
                            break;
                    }
//                    LOGE("switch season...%d", sGlobal->currentSeasonId);
                    sGlobal->mainGameLayer->needSwitchSeason = true;
                    sGlobal->mainGameLayer->hasSwitchedSeason = false;
                    sGlobal->mainGameLayer->nextSeasonId = sGlobal->currentSeasonId;
                    return;
                }
            }
        } else {
            nextX = MIN(-container->getPositionX() + sGlobal->hero_offset_left, 0);
            //sharyu: 将map移动到map1的右侧
            if (sGlobal->mainGameLayer->m_TileMap1->getPositionX() > sGlobal->mainGameLayer->m_TileMap->getPositionX() && nextX + dist266 < -sGlobal->mainGameLayer->m_TileMap1->getPositionX()) {
                float newX = sGlobal->mainGameLayer->m_TileMap1->getPositionX() + sGlobal->tileMapWidthInPixels1;
//                float diffX = newX - sGlobal->mainGameLayer->m_TileMap->getPositionX();
                sGlobal->mainGameLayer->generateNextMap(sGlobal->mainGameLayer->m_TileMap, 0);
                sGlobal->mainGameLayer->m_TileMap->setPosition(newX, sGlobal->tileMapOffsetY);
                if (sGlobal->mainGameLayer->front0 != NULL) {
                    sGlobal->mainGameLayer->front0->setPosition(newX, sGlobal->tileMapOffsetY);
                }
            }
            //sharyu: 将map1移动到map右侧
            if (sGlobal->mainGameLayer->m_TileMap->getPositionX() > sGlobal->mainGameLayer->m_TileMap1->getPositionX() && nextX + dist266 < -sGlobal->mainGameLayer->m_TileMap->getPositionX()) {
                float newX = sGlobal->mainGameLayer->m_TileMap->getPositionX() + sGlobal->tileMapWidthInPixels;
//                float diffX = newX - sGlobal->mainGameLayer->m_TileMap1->getPositionX();
                sGlobal->mainGameLayer->generateNextMap(sGlobal->mainGameLayer->m_TileMap1, 1);
                sGlobal->mainGameLayer->m_TileMap1->setPosition(newX, sGlobal->tileMapOffsetY);
                if (sGlobal->mainGameLayer->front1 != NULL) {
                    sGlobal->mainGameLayer->front1->setPosition(newX, sGlobal->tileMapOffsetY);
                }
            }
        }
    }
    else {
        if (sGlobal->index == 104) {
            nextX = MIN(-container->getPositionX() + sGlobal->hero_offset_left, 0);
        } else {
            nextX = MIN(-container->getPositionX() + sGlobal->hero_offset_left, 0);
            float maxX = -sGlobal->tileMapWidthInPixels + wyDevice::winWidth;
            if (nextX < maxX) {
                nextX = maxX;
                needDestroy = false;
            }
        }
        if (!needRepeat) {
            int idx = (int) (fabs(nextX) / sGlobal->singleMapWidth) + 1;
            //        LOGE("idx: %d", idx);
            wyTMXTileMap **mapArray = sGlobal->mainGameLayer->m_TileMapArray;
            if (!mapArray[idx]->isVisible()) {
                mapArray[idx]->setVisible(true);
            }
            //LOGE("num: %f, %f, %f", mapArray[idx]->getPositionX(), sGlobal->singleMapWidth, nextX);
            if (mapArray[idx]->getPositionX() + sGlobal->singleMapWidth + nextX < wyDevice::winWidth + dist266) {
                if (idx + 1 <= sGlobal->mapPieceNumber) {
                    mapArray[idx+1]->setVisible(true);
                    if (sGlobal->mainGameLayer->m_FrontArray[idx+1]!=NULL) {
                        sGlobal->mainGameLayer->m_FrontArray[idx+1]->setVisible(true);
                    }
                }
                //            LOGE("show map: %d", idx + 1);
            }
            if (nextX + mapArray[idx]->getPositionX() < -dist266) {
                if (idx - 1 != 0 && mapArray[idx-1] != 0) {
                    mapArray[idx-1]->setVisible(false);
                    if (sGlobal->mainGameLayer->m_FrontArray[idx-1]!=NULL) {
                        sGlobal->mainGameLayer->m_FrontArray[idx-1]->setVisible(true);
                    }
                    //                LOGE("hide map: %d", idx - 1);
                }
            }
            
            if (sGlobal->index == 104 && idx == (sGlobal->mapPieceNumber - 2)) {
                needRepeat = true;
                sGlobal->mainGameLayer->m_TileMap->setVisible(true);
                sGlobal->mainGameLayer->m_TileMap1->setVisible(true);
            }
        } else {
            if (sGlobal->index == 104) {
                //sharyu: 将map移动到map1的右侧
                //            LOGE("x1:%f, x2:%f, base:%f, heroX:%f", sGlobal->mainGameLayer->m_TileMap->getPositionX(), sGlobal->mainGameLayer->m_TileMap1->getPositionX(), sGlobal->baseTileMapWidthInPixels, nextX);
                if (sGlobal->mainGameLayer->m_TileMap1->getPositionX() > sGlobal->mainGameLayer->m_TileMap->getPositionX() && nextX + dist266 < -sGlobal->mainGameLayer->m_TileMap1->getPositionX()) {
                    float newX = sGlobal->mainGameLayer->m_TileMap1->getPositionX() + sGlobal->tileMapWidthInPixels1;
                    //                LOGE("newX for 0:%f", newX);
                    float diffX = newX - sGlobal->mainGameLayer->m_TileMap->getPositionX();
                    sGlobal->mainGameLayer->m_TileMap->setPosition(newX, sGlobal->tileMapOffsetY);
                    std::vector<b2Body *>::iterator pos2;
                    for(pos2 = sGlobal->mainGameLayer->groundBody.begin(); pos2 != sGlobal->mainGameLayer->groundBody.end(); ++pos2) {
                        b2Body *body = *pos2;
                        b2Vec2 pos = body->GetPosition();
                        body->SetTransform(b2Vec2(m_box2d->pixel2Meter(diffX) + pos.x, pos.y), 0);
                    }
                }
                //sharyu: 将map1移动到map右侧
                if (sGlobal->mainGameLayer->m_TileMap->getPositionX() > sGlobal->mainGameLayer->m_TileMap1->getPositionX() && nextX + dist266 < -sGlobal->mainGameLayer->m_TileMap->getPositionX()) {
                    float newX = sGlobal->mainGameLayer->m_TileMap->getPositionX() + sGlobal->tileMapWidthInPixels;
                    //                LOGE("newX for 1:%f", newX);
                    float diffX = newX - sGlobal->mainGameLayer->m_TileMap1->getPositionX();
                    sGlobal->mainGameLayer->m_TileMap1->setPosition(newX, sGlobal->tileMapOffsetY);
                    std::vector<b2Body *>::iterator pos2;
                    for(pos2 = sGlobal->mainGameLayer->groundBody1.begin(); pos2 != sGlobal->mainGameLayer->groundBody1.end(); ++pos2) {
                        b2Body *body = *pos2;
                        b2Vec2 pos = body->GetPosition();
                        body->SetTransform(b2Vec2(m_box2d->pixel2Meter(diffX) + pos.x, pos.y), 0);
                    }
                }
            }
        }
    }
    if (isShaking) {
        nextX += randRange(-4, 0)/0.75f*sGlobal->scaleX;
        diffY += randRange(-5, 5)/0.75f*sGlobal->scaleY;
        shakingTimer += dt;
        if (shakingTimer > 0.3f) {
            isShaking = false;
            shakingTimer = 0;
        }
    }
    if (isStrongShaking) {
        nextX += randRange(-4, 0)/0.75f*sGlobal->scaleX*2;
        diffY += randRange(-5, 5)/0.75f*sGlobal->scaleY*2;
        strongShakingTimer += dt;
        if (strongShakingTimer > 0.6f) {
            isStrongShaking = false;
            strongShakingTimer = 0;
        }
    }

    sGlobal->mainGameLayer->setPosition(nextX, diffY*0.9f);
    
//    LOGE("effectLayer:%d", sGlobal->effectLayer->getTag());
    if (nextX != oldX) {
        sGlobal->gameBackgroundLayer->offsetBy(prevX - container->getPositionX(), diffY);
        sGlobal->effectLayer->offsetBy(prevX - container->getPositionX(), diffY);
    } else {
        sGlobal->gameBackgroundLayer->offsetBy(0, diffY);
        sGlobal->effectLayer->offsetBy(0, diffY);
    }
    sGlobal->gameBackgroundLayer->update(dt);
    sGlobal->effectLayer->updateTimer(dt);

    if (isHurt) {
        hurtTimer += dt;
        if (hurtTimer > sGlobal->HURT_CD) {
            isHurt = false;
        }
    }

    if (sGlobal->jumpTime > 0 && !isFlying) {
        float vely = body->GetLinearVelocity().y;
        if (isMotoing) {

        } else {
            if (!isDBJumping) {
//                LOGE("vely:%f", vely);
                if (vely >= 20.0f && vely < 24.0f) {
                    spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_JUMP2);
                    spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_JUMP2);
                    setHeadRotation(2);
//                    LOGE("rot 2");
                }
                if (vely >= 18.0f && vely < 20.0f) {
                    spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_JUMP3);
                    spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_JUMP3);
                    setHeadRotation(1);
//                    LOGE("rot 1");
                }
                if (vely >= 16.0f && vely < 18.0f) {
                    spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_JUMP4);
                    spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_JUMP4);
                    setHeadRotation(0);
//                    LOGE("rot 0");
                }
            }
            if (vely < 0.0f && !isBodySensorCreated) {
                setHeadRotation(-3);
                createBodySensor();
            }
        }
    }
    
    if (bodySensor != NULL) {
        bodySensor->body->SetTransform(b2Vec2(body->GetPosition().x-m_box2d->pixel2Meter(DP(0)*sGlobal->scaleX), body->GetPosition().y-m_box2d->pixel2Meter(DP(45)*sGlobal->scaleX)), 0);
    }
    
    if (hurtSensor != NULL) {
        hurtSensor->body->SetTransform(b2Vec2(body->GetPosition().x+m_box2d->pixel2Meter(DP(0)*sGlobal->scaleX), body->GetPosition().y-m_box2d->pixel2Meter(DP(0)*sGlobal->scaleX)), 0);
    }
    
    spx_hero_body->tick(dt);
    spx_hero_l_foot->tick(dt);
    spx_hero_r_foot->tick(dt);
    
    if (jumpBomb->isVisible()) {
        jumpBomb->tick(dt);
    }
    if (bloodSprite->isVisible()) {
        bloodSprite->tick(dt);
        bloodSprite->setPosition(container->getPositionX()+DP(30)*sGlobal->scaleX, container->getPositionY()+DP(26)*sGlobal->scaleY);
    }
    
//    if (shadow->isVisible()) {
//        shadow->setPosition(container->getPositionX(), container->getPositionY()-DP(50)*sGlobal->scaleY);
//    }
    
//    if (isIllusionActive) {
//        illusionTimer += dt;
//        if (illusionTimer > 8.0f && !isIllusionGoning) {
//            //LOGE("%d", sGlobal->effectLayer->animIndex);
//            illusion->setLoopCount(0);
//            illusion->setReverse(true);
//            illusion->playAnimation(sGlobal->effectLayer->animIndex);
//            wyAFCSpriteCallback callback = {
//                NULL,
//                onIllusionGone
//            };
//            illusion->setAFCSpriteCallback(&callback, this);
//            isIllusionGoning = true;
//        }
//        illusion->tick(dt);
//        illusion->setPosition(container->getPositionX(), container->getPositionY() + DP(0));
//    }
    
    if (isSandShieldActive) {
        sandShieldTimer += dt;
        if (sandShieldTimer > sGlobal->sand_shield_cd - 1.2f && !isSSFaded) {
            isSSFaded = true;
            doFadeEffect();
        }
        if (sandShieldTimer > sGlobal->sand_shield_cd && !isSandShieldGoning) {
            sandShieldGone();
        }
        
//        if (shandian != NULL) {
//            shandian->tick(dt);
//            shandian->setPosition(container->getPositionX(), container->getPositionY()+DP(40)*sGlobal->scaleY);
//        }
        if (shieldContainer->isVisible()) {
//            shandian->tick(dt);
            shieldSprite_1->tick(dt);
            shieldContainer->setPosition(container->getPositionX()+DP(2)*sGlobal->scaleY, container->getPositionY()+DP(10)*sGlobal->scaleY+spx_hero_body->getPositionY());
        }
    }
    
    if (isSavingMe) {
        savingMeTimer += dt;
        if (savingMeTimer > 0.2f) {
            isSavingMe = false;
            savingMeTimer = 0;
        }
    }
    
//    if (isWudi) {
//        wudiTimer += dt;
//        if (wudiTimer > wudiDuration) {
//            isWudi = false;
//            wudiTimer = 0;
//        }
//    }
    
    if (isFlying) {
        if (!isFlyingPaused) {
            flyTimer += dt;
            if (!isStartFlyingEnd && container->getPositionY()>DP(320)*sGlobal->scaleY) {
                isStartFlyingEnd = true;
                body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, HERO_FLY_Y_DOWN));
            }
            if (flyTimer > flyDuration) {
                flyTimer = 0;
                isFlying = false;
//                body->SetGravityScale(1.0f);

				isYingReleased = false;
                hideYing();
                
                
            }
            else if (flyTimer > flyDuration - 2.0f && !isDeFlying){
                isDeFlying = true;
//                doFadeEffect();
//                wyIntervalAction *a = wyTintTo::make(0.2, 255, 255, 255, 255, 0, 0);
//                yingSprite->runAction(wySequence::make(a, (wyIntervalAction*)a->reverse(), a->copy(), (wyIntervalAction*)a->reverse(), a->copy(), (wyIntervalAction*)a->reverse(), a->copy(), (wyIntervalAction*)a->reverse(), a->copy(), (wyIntervalAction*)a->reverse(), NULL));
                
                wyIntervalAction* a = wyFadeTo::make(0.2f, 255, 100);
                wyIntervalAction* r = wySequence::make(a,
                                                       (wyFiniteTimeAction*)a->reverse(),
                                                       a->copy(),
                                                       (wyFiniteTimeAction*)a->reverse(),
                                                       a->copy(),
                                                       (wyFiniteTimeAction*)a->reverse(),
                                                       a->copy(),
                                                       (wyFiniteTimeAction*)a->reverse(),
                                                       a->copy(),
                                                       (wyFiniteTimeAction*)a->reverse(),
                                                       a->copy(),
                                                       (wyFiniteTimeAction*)a->reverse(),
                                                       NULL);
                
                    yingSprite->runAction(r->copy());
                    spx_hero_body->runAction(r);
                    spx_hero_r_foot->runAction(r->copy());
                    spx_hero_l_foot->runAction(r->copy());
            }
            ying->setPosition(container->getPositionX() + DP(-6)*sGlobal->scaleX, container->getPositionY() + DP(22)*sGlobal->scaleY + spx_hero_body->getPositionY());
        }
        yingSprite->tick(dt);
    }
    
//    if (isYingReleasing) {
//        yingSprite->tick(dt);
//        ying->setPosition(container->getPositionX() + DP(-6)*sGlobal->scaleX, container->getPositionY() + DP(22)*sGlobal->scaleY + spx_hero_body->getPositionY());
//    }
    
    if (!isDead && isInDead) {// || (isFlying && isDeBoosting)
        if (diffY < 0) {
            //            LOGE("diffY:%f", diffY);
            sGlobal->heroEyeZ = -diffY/(HERO_YING_MAX_HEIGHT*sGlobal->scaleY/99.0f);
        } else {
            sGlobal->heroEyeZ = 1;
        }
        if (sGlobal->bossEyeZ < sGlobal->heroEyeZ) {
            //            LOGE("eye:%f", sGlobal->heroEyeZ);
            sGlobal->mainGameLayer->getCamera()->setEye(0, sGlobal->heroEyeZ/HERO_BOOTST_EYE_BASEY_DIV, sGlobal->heroEyeZ);
            sGlobal->mainGameLayer->getCamera()->setCenter(0, sGlobal->heroEyeZ/HERO_BOOTST_EYE_BASEY_DIV, 0);
        }
    }
    
    if (isBoosting) {
        boostTimer += dt;
		if (boost_emitter != NULL) {
            if (boost_emitter != NULL) {
                if (sGlobal->jumpTime > 0) {
                    boost_emitter->setPosition(container->getPositionX()-DP(85)*sGlobal->scaleY, container->getPositionY()-DP(50)*sGlobal->scaleY);
                } else {
                    boost_emitter->setPosition(container->getPositionX()-DP(85)*sGlobal->scaleY, container->getPositionY()-DP(30)*sGlobal->scaleY);
                }
            }
		}
        if (heroZ < 90) {
            heroZ += 1;
            sGlobal->mainGameLayer->getCamera()->setEye(0, heroZ/HERO_BOOTST_EYE_BASEY_DIV, heroZ);
            sGlobal->mainGameLayer->getCamera()->setCenter(0, heroZ/HERO_BOOTST_EYE_BASEY_DIV, 0);
//            LOGE("eye:%f", heroZ);
        }
        if (boostTimer > HERO_BOOST_TIME) {
            boostTimer = 0;
            isBoosting = false;
            isDeBoosting = true;
            deBoostTimer = 0;
            
            doFadeEffect();
        }
    }
    
    if (isDeBoosting && !isDead) {
        deBoostTimer += dt;
        //LOGE("eyeZ: %f, velX: %f", sGlobal->heroEyeZ, body->GetLinearVelocity().x);
		if (boost_emitter != NULL) {
            if (sGlobal->jumpTime > 0) {
                boost_emitter->setPosition(container->getPositionX()-DP(85)*sGlobal->scaleY, container->getPositionY()-DP(50)*sGlobal->scaleY);
            } else {
                boost_emitter->setPosition(container->getPositionX()-DP(85)*sGlobal->scaleY, container->getPositionY()-DP(30)*sGlobal->scaleY);
            }
		}
        if (isFlying) {
            bool flag = true;
            float maxEyeZ = MAX(sGlobal->heroEyeZ, sGlobal->bossEyeZ);
            if (heroZ > maxEyeZ) {
                heroZ -= 1;
                sGlobal->mainGameLayer->getCamera()->setEye(0, heroZ/HERO_BOOTST_EYE_BASEY_DIV, heroZ);
                sGlobal->mainGameLayer->getCamera()->setCenter(0, heroZ/HERO_BOOTST_EYE_BASEY_DIV, 0);
                flag = false;
            }
            if (body->GetLinearVelocity().x - 0.2f < sGlobal->heroVelocityX) {
                body->SetLinearVelocity(b2Vec2(sGlobal->heroVelocityX, body->GetLinearVelocity().y));
                curVelX = sGlobal->heroVelocityX;
            } else {
                curVelX = body->GetLinearVelocity().x - 0.2f;
                body->SetLinearVelocity(b2Vec2(curVelX, body->GetLinearVelocity().y));
                flag = false;
            }
            if (flag) {
//                heroZ = 1;
//                sGlobal->heroEyeZ = 1;
                sGlobal->mainGameLayer->getCamera()->setEye(0, sGlobal->heroEyeZ/HERO_BOOTST_EYE_BASEY_DIV, sGlobal->heroEyeZ);
                sGlobal->mainGameLayer->getCamera()->setCenter(0, sGlobal->heroEyeZ/HERO_BOOTST_EYE_BASEY_DIV, 0);
                isDeBoosting = false;
                deBoostTimer = 0;
                body->SetLinearVelocity(b2Vec2(sGlobal->heroVelocityX, body->GetLinearVelocity().y));
                curVelX = sGlobal->heroVelocityX;
                
                if (!isFlying) {
                    hideMoto();
                }
            }
        } else {
            float maxEyeZ = MAX(sGlobal->heroEyeZ, sGlobal->bossEyeZ);
            if (heroZ > maxEyeZ || fabs(body->GetLinearVelocity().x - sGlobal->heroVelocityX) > 0.00000001f) {
                heroZ -= 1;
                sGlobal->mainGameLayer->getCamera()->setEye(0, heroZ/HERO_BOOTST_EYE_BASEY_DIV, heroZ);
                sGlobal->mainGameLayer->getCamera()->setCenter(0, heroZ/HERO_BOOTST_EYE_BASEY_DIV, 0);
                if (body->GetLinearVelocity().x - 0.2f < sGlobal->heroVelocityX) {
                    body->SetLinearVelocity(b2Vec2(sGlobal->heroVelocityX, body->GetLinearVelocity().y));
                    curVelX = sGlobal->heroVelocityX;
                } else {
                    curVelX = body->GetLinearVelocity().x - 0.2f;
                    body->SetLinearVelocity(b2Vec2(curVelX, body->GetLinearVelocity().y));
                }
            } else {
                heroZ = maxEyeZ;
                sGlobal->mainGameLayer->getCamera()->setEye(0, heroZ/HERO_BOOTST_EYE_BASEY_DIV, heroZ);
                sGlobal->mainGameLayer->getCamera()->setCenter(0, heroZ/HERO_BOOTST_EYE_BASEY_DIV, 0);
                isDeBoosting = false;
                deBoostTimer = 0;
                body->SetLinearVelocity(b2Vec2(sGlobal->heroVelocityX, body->GetLinearVelocity().y));
                curVelX = sGlobal->heroVelocityX;

                if (!isFlying) {
                    hideMoto();
                }
            }
        }
    }
    
    if (isRunning && (isBoosting || isDeBoosting)) {
        if (body->GetLinearVelocity().x < curVelX) {
            //LOGE("hero stucked in boosting...");
            body->SetLinearVelocity(b2Vec2(curVelX, body->GetLinearVelocity().y));
        }
    }
    
    if (isMotoRunning && boost_emitter != NULL) {
        spx_moto_body->setPosition(m_box2d->meter2Pixel(motoBody->body->GetPosition().x)-container->getPositionX(), m_box2d->meter2Pixel(motoBody->body->GetPosition().y)-container->getPositionY());
        
        moto_b_wheel_container->setPosition(-DP(26)*sGlobal->scaleX+spx_moto_body->getPositionX(), -DP(40)*spx_moto_b_wheel->getScaleY()+spx_moto_body->getPositionY()-DP(10)*sGlobal->scaleY);
        moto_f_wheel_container->setPosition(DP(114)*sGlobal->scaleX+spx_moto_body->getPositionX(), -DP(36)*spx_moto_f_wheel->getScaleY()+spx_moto_body->getPositionY()-DP(10)*sGlobal->scaleY);

//        int ofx = -DP(40)*sGlobal->scaleX;
        int ofy = -DP(8)*sGlobal->scaleY;
        spx_moto_b_wheel->setPosition(0, DP(40)*spx_moto_b_wheel->getScaleY()+ofy);
        spx_moto_f_wheel->setPosition(0, DP(36)*spx_moto_f_wheel->getScaleY()+ofy);
        
        boost_emitter->setPosition(container->getPositionX()+spx_moto_body->getPositionX()-DP(45)*sGlobal->scaleY, container->getPositionY()+spx_moto_body->getPositionY()-DP(25)*sGlobal->scaleY);
    }
    
    if (isBackingToNormal) {
//        LOGE("heroZ:%f", heroZ);
        if (heroZ - backToNormalSpeed > 1) {
            heroZ -= backToNormalSpeed;
        } else {
            sGlobal->heroEyeZ = 1;
            heroZ = 1;
            isBackingToNormal = false;
        }
        sGlobal->mainGameLayer->getCamera()->setEye(0, heroZ/HERO_BOOTST_EYE_BASEY_DIV, heroZ);
        sGlobal->mainGameLayer->getCamera()->setCenter(0, heroZ/HERO_BOOTST_EYE_BASEY_DIV, 0);
    }
    
//    if (isStreaking) {
//        streakTimer+=dt;
//        if (streakTimer > 1.0f) {
//            isStreaking = false;
//            streakTimer = 0;
//        }
//        m_streak1->addPoint(sprite->getPositionX(), sprite->getPositionY());
//    }
    
    if (isHurtByChongZhuang) {
        hurtByCZTimer += dt;
        if (hurtByCZTimer > 2.0f) {
            hurtByCZTimer = 0;
            isHurtByChongZhuang = false;
        }
    }
    
//    if (isZhanShenShowing) {
//        zhanshen->tick(dt);
//        if (zhanshenDist < 0) {
//            zhanshenDist += DP(8)*sGlobal->scaleX;
//        }
//        zhanshen->setPosition(container->getPositionX() + zhanshenDist, zhanshen->getPositionY());
//    }
    if (isFiringMissle) {
        if (fireTimes < 20) {
            firingMissleTimer += dt;
            if (firingMissleTimer > 0.3f) {
                firingMissleTimer = 0;
                fireTimes++;
                for (int i = 0; i < rand()%2+1; i++) {
                    new Missle(container->getPositionX(), baseY, i);
                }
            }
        } else {
            isFiringMissle = false;
        }
    }
    
//    if (isShowingFaZhen) {
//        if (isFaZhenEnded) {
//            isShowingFaZhen = false;
//            isFaZhenEnded = false;
//            sGlobal->mainGameLayer->removeChildLocked(fazhen, true);
//            sGlobal->mainGameLayer->needAutoScroll = true;
//            container->setVisible(true);
//            shadow->setVisible(true);
//            run();
//        } else {
//            fazhen->tick(dt);
//        }
//    }
    
    if (isHealing) {
        healTimer += dt;
        if (healTimer > 3.0f) {
            isHealing = false;
            healTimer = 0;
            sGlobal->mainGameLayer->removeChildLocked(heal_emitter, true);
            heal_emitter = NULL;
            if (sGlobal->index == 0) {
                sGlobal->hudLayer->updateGuide();
            }
        } else {
            heal_emitter->setPosition(container->getPositionX() + DP(20)*sGlobal->scaleX, container->getPositionY()+DP(0)*sGlobal->scaleY);
        }
    }
    
    if (sGlobal->isRealEndless) {
        sGlobal->hudLayer->updateMeter();
    }
    
//    if (shieldSprite->isVisible()) {
//        shieldSprite->tick(dt);
//        shieldSprite->setPosition(container->getPositionX() + DP(8), container->getPositionY()+DP(24)*sGlobal->scaleY);
//    }
    
    if (dodgeContainer != NULL && dodgeContainer->isVisible()) {
        dodgeContainer->setPosition(container->getPositionX(), container->getPositionY());
    }
    
    if (isShootingLaser && laserContainer != NULL) {
        laserContainer->setPosition(container->getPositionX() + laserofx, container->getPositionY()+laserofy+laserY);//+laserY
        laserLight->tick(dt);
        
        laserTimer += dt;
        if (laserTimer > 0.4f) {
            isShootingLaser = false;
            sGlobal->mainGameLayer->addToDestroy(laserBullet->body);
            laserBullet = NULL;
            laserContainer->setVisible(false);
        } else {
            laserBullet->body->SetTransform(b2Vec2(m_box2d->pixel2Meter(container->getPositionX() + laserBullet->x), m_box2d->pixel2Meter(container->getPositionY() + laserBullet->y)), 0);
        }
    }
    
    recoverHP(dt);
    
//    if (hpContainer != NULL && hpContainer->isVisible()) {
//        hpDist += DP(1)*sGlobal->scaleX;
//        hpContainer->setPosition(sprite->getPositionX()-DP(10)*sGlobal->scaleX, sprite->getPositionY()+hpDist+DP(30)*sGlobal->scaleY);
//    }
    
    if (robertStatus == R_APPEARING && robert != NULL) {
        robert->tick(dt);
    }
    
//    if (firefire->isVisible()) {
//        firefire->tick(dt);
//    }
    
    if (sGlobal->isRealEndless && !sGlobal->isQuestRecorded) {
        questRecordTimer += dt;
        if (questRecordTimer > 2.0f) {
            sGlobal->isQuestRecorded = true;
            checkQuest();
        }
    }
    
//    sGlobal->mainGameLayer->getCamera()->setEye(0, 0, 1);
//    sGlobal->mainGameLayer->getCamera()->setCenter(0, 0, 0);
    if (isHurtRot) {
        hurtRotTimer += dt;
        if (hurtRotTimer > 0.2f) {
            isHurtRot = false;
            spx_hero_body->setRotation(0);
        }
    }
    
    if (isFiringLiudan) {
        fireLiudanTimer += dt;
        if (fireLiudanTimer > fireLiudanInterval) {
            fireLiudanTime += 1;
            fireLiudanTimer = 0;
            if (fireLiudanTime == 2) {
                new LiuDan(container->getPositionX(), baseY, false, liudanSeq%100/10);
            }
            else if (fireLiudanTime == 3) {
                new LiuDan(container->getPositionX(), baseY, false, liudanSeq%10);
            }
            
            if (fireLiudanTime >= 3) {
                isFiringLiudan = false;
                fireLiudanTime = 0;
                fireLiudanTimer = 0;
            }
        }
    }
    
    if (speedline_emitter != NULL) {
        speedline_emitter->setPosition(container->getPositionX()+DP(120)*sGlobal->scaleX, container->getPositionY());
    }
    
    if (gunLightContainer != NULL && gunLightContainer->isVisible()) {
        gunLightContainer->setPosition(container->getPositionX()+gunLightofx, container->getPositionY()+gunLightofy);
    }
}

void Hero::checkQuest() {
    if (!sGlobal->isRealEndless) {
        return;
    }
    
    for (int i = 0; i < 13; i++) {
        int achvNo = ACHV_GUN_3+i;
//        LOGE("achvNo:%d, status:%d", achvNo, sGlobal->achv_rec[achvNo]);
        if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
            switch (achvNo) {
                case ACHV_GUN_3:
                case ACHV_GUN_5:
                case ACHV_GUN_8:
                case ACHV_GUN_10: {
                    int max = 0;
                    for (int i = 0; i < 16; i++) {
                        if (max < sGlobal->weaponUnlockArray[i].getValue()) {
                            max = sGlobal->weaponUnlockArray[i].getValue();
                        }
                    }
                    sGlobal->achv_progress[achvNo] = max;
                    break;
                }
                case ACHV_ASKILL_5:
                case ACHV_ASKILL_12:
                case ACHV_ASKILL_20:
                case ACHV_ASKILL_30: {
                    int max = 0;
                    for (int i = 0; i < 3; i++) {
                        if (max < sGlobal->skillArray[SKILL_MISSLE+i].getValue()) {
                            max = sGlobal->skillArray[SKILL_MISSLE+i].getValue();
                        }
                    }
                    sGlobal->achv_progress[achvNo] = max;
                    break;
                }
                case ACHV_PSKILL_5:
                case ACHV_PSKILL_12:
                case ACHV_PSKILL_20:
                case ACHV_PSKILL_30: {
                    int max = 0;
                    for (int i = 0; i < 7; i++) {
                        if (max < sGlobal->skillArray[SKILL_GREED+i].getValue()) {
                            max = sGlobal->skillArray[SKILL_GREED+i].getValue();
                        }
                    }
                    sGlobal->achv_progress[achvNo] = max;
                    break;
                }
                case ACHV_ALL_SKILL_30: {
                    int min = 100;
                    for (int i = 0; i < 10; i++) {
                        if (min > sGlobal->skillArray[SKILL_MISSLE+i].getValue()) {
                            min = sGlobal->skillArray[SKILL_MISSLE+i].getValue();
                        }
                    }
                    sGlobal->achv_progress[achvNo] = min;
                }
                default:
                    break;
            }
            if (sGlobal->achv_progress[achvNo] >= sGlobal->ACHV_NUM[achvNo*3]) {
                sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
            }
        }
    }
}

void Hero::handleCollision(b2Body *actorB) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
    int tagB = userDataB->obj_type;
    switch (tagB) {
//        case TYPE_SHOCK_WAVE:
//        case TYPE_ENEMY_BULLET: {
//            EnemyBulletBase *enemyBullet = (EnemyBulletBase *)userDataB;
//            enemyBullet->onHitHero();
//            
////            sGlobal->mainGameLayer->addToDestroy(actorB);
//            if (isBoosting || isDeBoosting || isSandShieldActive) {
//                
//            } else {
//                hurt(userDataB->atk, enemyBullet->b_id != 0);
//            }
////            userDataB->onHitHero();
//            break;
//            LOGE("aaa");
//        }
//        case TYPE_CHONGZHUANG: {
//            if (isBoosting || isDeBoosting || isSandShieldActive) {
//                
//            } else {
//                hurt(userDataB->atk);
//            }
//            break;
//        }
//        case TYPE_WATER_BALL:
//        case TYPE_LIGHTING: {
//            if (isBoosting || isDeBoosting || isSandShieldActive) {
//                
//            } else {
//                hurt(userDataB->atk, true);
//            }
//            break;
//        }
        case TYPE_ENEMY: {
            Enemy *enemy = (Enemy *)userDataB;
            if (enemy->isDead) {
                return;
            }
            if (isBoosting || isDeBoosting || isSandShieldActive) {
                if (enemy->e_id != ENEMY_XUANKONGBIAO && enemy->e_id != ENEMY_BOSS1 && enemy->e_id != ENEMY_BOSS2 && enemy->e_id != ENEMY_BOSS3) {
                    if (isSandShieldActive) {
                        enemy->deadType = DEAD_TYPE_DIAN;
                        
                        if (sGlobal->isRealEndless && !enemy->isDead) {
                            int achvNo = ACHV_SHIELD_10;
                            if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
                                sGlobal->achv_progress[achvNo]+=1;
                                if (sGlobal->achv_progress[achvNo] >= sGlobal->ACHV_NUM[achvNo*3]) {
                                    sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                                    sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                                    sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
                                }
                            }
                        }
                    } else {
                        enemy->deadType = DEAD_TYPE_MOTO;
                    }
                    enemy->dead();
                }
                userDataB->onHitHero();
            }
//            else {
//                if (!enemy->isStunning) {
//                    if (enemy->e_id == ENEMY_XUANKONGBIAO || enemy->e_id == ENEMY_BOSS1 || enemy->e_id == ENEMY_BOSS2 || enemy->e_id == ENEMY_BOSS3 || enemy->e_id == ENEMY_NEED_BLOOD) {
//                        hurt(userDataB->atk, true);
//                    } else {
//                        hurt(userDataB->atk, rand()%3==0);
//                    }
//                }
//            }
            
            break;
        }
        case TYPE_WORLD_BOUND: {

        }
        case TYPE_GROUND: {
//            if (!sGlobal->isOnGround) {
//                sGlobal->isOnGround = true;
//                if (!isShowingFaZhen)
//                    shadow->setVisible(true);
//            }
            break;
        }
//        case TYPE_TRAP: {
//            if (isBoosting || isDeBoosting || isSandShieldActive) {
//                
//            } else {
//                hurt(userDataB->atk);
//            }
//            userDataB->onHitHero();
//            break;
//        }
//        case TYPE_BOMB: {
//            if (isBoosting || isDeBoosting || isSandShieldActive) {
//                
//            } else {
//                hurt(userDataB->atk);
//            }
//            userDataB->onHitHero();
//            break;
//        }
        case TYPE_BONUS: {
            ((Bonus *)userDataB)->beEaten();
            break;
        }
        case TYPE_INTRO: {
            sGlobal->mainGameLayer->addToDestroy(actorB);
            
            Intro *intro = (Intro *)actorB->GetUserData();
            sGlobal->hudLayer->showIntro(intro);
            break;
        }
        case TYPE_ENDPOINT: {
            sGlobal->mainGameLayer->addToDisable(actorB);
            
            isRunning =false;
            body->SetLinearVelocity(b2Vec2(3.0f, 0.0f));

            if (isMotoing) {
                curVelX = 5.0f;
                body->SetLinearVelocity(b2Vec2(5.0f, 0.0f));

                isDeFlying = false;
                isBoosting = false;
                isDeBoosting = false;
                isBackingToNormal = true;
                backToNormalSpeed = 1.5f;
                
                if (boost_emitter!=NULL) {
                    sGlobal->mainGameLayer->removeChildLocked(boost_emitter, true);
                }
                boost_emitter = NULL;
                
                spx_moto_b_wheel->stopAllActions();
                spx_moto_f_wheel->stopAllActions();
                
                spx_moto_b_wheel->runAction(wyEaseIn::make(0.8f, wyRotateBy::make(0.8f, 360)));
                spx_moto_f_wheel->runAction(wyEaseIn::make(0.8f, wyRotateBy::make(0.8f, 360)));
            }
            
//            timer = wyTimer::make(wyTargetSelector::make(this, SEL_p(Hero::doEndLogic), userDataB->container), 1.0f);
//            timer->setOneShot(true);
//            sGlobal->mainGameLayer->scheduleLocked(timer);
//            LOGE("aaa");
            sGlobal->mainGameLayer->runAction(wySequence::make(wyDelayTime::make(1.0f), wyCallFunc::make(wyTargetSelector::make(this, SEL_p(Hero::doEndLogic), userDataB->container)), NULL));
            break;
        }
        case TYPE_CHECKPOINT: {
            reachCheckPoint();
            CheckPoint *checkpoint = (CheckPoint *)actorB->GetUserData();
            checkpoint->fire();
            
//            stop();
            break;
        }
        case TYPE_CANNON: {
//            Cannon *cannon = (Cannon *)actorB->GetUserData();
//            cannon->fire();
            break;
        }
        default:
            break;
    }
}

void Hero::doContactLogic() {
//    LOGE("do contact logic...");
    
    sGlobal->isOnGround = true;
    isBodySensorCreated = false;

//    contactNum++;
    
    if (isMotoing) {
        container->setRotation(0);
        spx_hero_body->setRotation(0);
        if (sGlobal->jumpTime > 0) {
            playHeroAnimation(SPX_HERO_R_LEG_LAND);
        }
    } else {
        playHeroAnimation(SPX_HERO_R_LEG_RUN);
    }
    
    shadow->setVisible(true);
    
    baseY = container->getPositionY();
    
    if (needSetGravity) {
//        LOGE("needSetGravity123...");
        body->SetGravityScale(1.0f);
        needSetGravity = false;
    }
    
    if (needSetSpeed) {
        body->SetLinearVelocity(b2Vec2(sGlobal->heroVelocityX, 0));
        needSetSpeed = false;
    }
    
    sGlobal->jumpTime1 = sGlobal->jumpTime;
    sGlobal->jumpTime = 0;
    isDBJumping = false;
    isSlowed = false;
}

void Hero::onFHDelayEnd(wyTargetSelector* ts){
    spx_hero_body->setPaused(false);
    spx_hero_r_foot->setPaused(false);
    spx_hero_l_foot->setPaused(false);
}

void Hero::beginContact(b2Body *actorB) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
    int tagB = userDataB->obj_type;
    switch (tagB) {
        case TYPE_GROUND: {
//            LOGE("velY:%f", body->GetLinearVelocity().y);
//            if (body->GetLinearVelocity().y <= 0) {

//            }
            float vely = body->GetLinearVelocity().y;
//            LOGE("begin contact..., %d, vel:%f", sGlobal->isOnGround ? 1:0, vely);
            if (vely < 0 || fabs(vely)<0.0000001f) {//!sGlobal->isOnGround &&
                sGlobal->isOnGround = true;
                isBodySensorCreated = false;
                contactNum++;
//                LOGE("begin contact..., %d", contactNum);
                
                if (isMotoing) {
                    container->setRotation(0);
                    spx_hero_body->setRotation(0);
                    if (sGlobal->jumpTime > 0) {
                        playHeroAnimation(SPX_HERO_R_LEG_LAND);
                    }
                } else {
//                    if (body->GetLinearVelocity().y <= 0) {
                    if (sGlobal->mainGameLayer->needAutoScroll) {
                        float groundY = userDataB->body->GetPosition().y;
                        //                    LOGE("diffY:%f-%f=%f", previousY, groundY, previousY - groundY);
                        if (previousY - groundY > m_box2d->pixel2Meter(DP(400)*sGlobal->scaleY)) {
                            playHeroAnimation(SPX_HERO_R_LEG_LAND);
                        } else {
                            playHeroAnimation(SPX_HERO_R_LEG_RUN);
//                            LOGE("play run...");
                        }
                        previousY = groundY;
                    } else {
                        if (isFHing) {
                            spx_hero_r_foot->setLoopCount(0);
                            spx_hero_l_foot->setLoopCount(0);
                            spx_hero_body->setLoopCount(0);
                            spx_hero_r_foot->setUnitInterval(0.3f);
                            spx_hero_l_foot->setUnitInterval(0.3f);
                            spx_hero_body->setUnitInterval(0.3f);
                            
                            spx_hero_body->playAnimation(spxFHIndex);
                            spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_FH);
                            spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_FH);
                            
                            spx_hero_body->setPaused(true);
                            spx_hero_r_foot->setPaused(true);
                            spx_hero_l_foot->setPaused(true);
                            spx_hero_body->runAction(wySequence::make(wyDelayTime::make(0.5f), wyCallFunc::make(wyTargetSelector::make(this, SEL(Hero::onFHDelayEnd))), NULL));
                        } else {
                            playHeroAnimation(SPX_HERO_R_LEG_IDLE);
                        }
                    }
//                    }
                }

//                LOGE("begin contact...%d", contactNum);
//                if (!shadow->isVisible() && !isShowingFaZhen) {
                    shadow->setVisible(true);
//                }
                
                if (!isHeightInited) {
//                    halfBodyHeightInMeter = body->GetPosition().y - actorB->GetPosition().y;
                    isHeightInited = true;
//                    LOGE("hero height: %f", halfBodyHeightInMeter);
                }
//                isShaking = true;
                baseY = container->getPositionY();
                
                baseVert = userDataB->body->GetPosition().y;
                
                if (needSetGravity) {
                    LOGE("needSetGravity...");
                    body->SetGravityScale(1.0f);
                    needSetGravity = false;
                }
                
                if (needSetSpeed) {
                    body->SetLinearVelocity(b2Vec2(sGlobal->heroVelocityX, 0));
                    needSetSpeed = false;
                }
                
                if (isFlyingSlowDown) {
                    cancelFlyFromSlowDown();
                }
                
                sGlobal->jumpTime1 = sGlobal->jumpTime;
                sGlobal->jumpTime = 0;
                isDBJumping = false;
                isSlowed = false;
            }
            break;
        }
        default:
            break;
    }
}

void Hero::endContact(b2Body *actorB) {
    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
    int tagB = userDataB->obj_type;
    switch (tagB) {
        case TYPE_GROUND: {
//            if (body->GetLinearVelocity().y != 0) {
                
            contactNum--;
            contactNum = MAX(contactNum, 0);
//            LOGE("end contact...%d", contactNum);
            if (contactNum == 0) {
                sGlobal->isOnGround = false;
                shadow->setVisible(false);
            }
//            }
            if (isFlying && !sGlobal->hudLayer->isTouching && isStartFlyingEnd) {
                body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, HERO_FLY_Y_DOWN));
            }
            break;
        }
        default:
            break;
    }
}

void Hero::cancelFlyFromSlowDown() {
//    isFlyingSlowDown = false;
//    body->SetGravityScale(1.0f);
//    
//    wyIntervalAction* a = wyFadeTo::make(0.15f, 255, 100);
//    wyIntervalAction* r = wySequence::make(a,
//                                           (wyFiniteTimeAction*)a->reverse(),
//                                           a->copy(),
//                                           (wyFiniteTimeAction*)a->reverse(),
//                                           a->copy(),
//                                           (wyFiniteTimeAction*)a->reverse(),
//                                           wyHide::make(),
//                                           NULL);
//    wyActionCallback callback = {
//        NULL,
//        onHideYingEnded
//    };
//    r->setCallback(&callback, this);
//    yingSprite->runAction(r);
}

void Hero::doDeadLogic(wyTargetSelector* ts, int data) {
    if (sGlobal->index == 0) {
        dealDeadInGuide();
    }
    else {
        isBackingToNormal = false;
        if (data == 1) {
            heroZ = 1;
            sGlobal->heroEyeZ = 1;
            sGlobal->mainGameLayer->getCamera()->setEye(0, sGlobal->heroEyeZ/HERO_BOOTST_EYE_BASEY_DIV, sGlobal->heroEyeZ);
            sGlobal->mainGameLayer->getCamera()->setCenter(0, sGlobal->heroEyeZ/HERO_BOOTST_EYE_BASEY_DIV, 0);
        }
        
        sGlobal->mainGameLayer->onExit();
        sGlobal->effectLayer->onExit();
        sGlobal->hudLayer->onExit();
        DeadNoticeLayer *cLayer = (DeadNoticeLayer*) (new DeadNoticeLayer(data))->autoRelease();
        sGlobal->mainGameLayer->getParent()->addChildLocked(cLayer, 4, 4);
        ((MainScene *)sGlobal->mainGameLayer->getParent())->sceneState = SC_DEAD_OPENED;
    }
}

void Hero::shoot() {
    
    int index = sGlobal->activeWeapon;
    if (index != WEAPON_UMP && index != WEAPON_HYDRA) {
        if (sGlobal->weaponAmmoArray[index] <= 0) {
            if (sGlobal->getGold() >= sGlobal->weapon_ammo_gold[index]) {
                sGlobal->offsetGold(-sGlobal->weapon_ammo_gold[index]);
                sGlobal->weaponAmmoArray[index] += sGlobal->WEAPON_PARAM[index*8+WP_AMMO_AMOUNT];
                sGlobal->arrayToStr(DB_WEAPON_AMMO_ARRAY, sGlobal->weaponAmmoArray, ARR_LEN_WEAPON_NUM);
                
                sGlobal->hudLayer->addGold(0);
                sGlobal->hudLayer->runGoldDecAnim(sGlobal->weapon_ammo_gold[index]);
                sGlobal->hudLayer->runAmmoIncAnim(sGlobal->WEAPON_PARAM[index*8+WP_AMMO_AMOUNT]);
            } else {
//                sGlobal->hudLayer->runOutofResAnim();
                sGlobal->hudLayer->onSwitchBtnClick(NULL, 1);
                return;
            }
        }
        sGlobal->hudLayer->addAmmo(1);
    }
    
    hasShoot = true;
    
    switch (sGlobal->activeWeapon) {
        case WEAPON_MP5A5:
            sGlobal->playEffect(se_gun_mp5a5);
            HeroBullet::make(sGlobal->activeWeapon);
            break;
        case WEAPON_UMP:
            sGlobal->playEffect(se_gun_normal);
            HeroBullet::make(sGlobal->activeWeapon);
            break;
        case WEAPON_M870P:
            sGlobal->playEffect(se_gun_rifle);
            break;
        case WEAPON_AK47:
            sGlobal->playEffect(se_gun_ak47);
            break;
        case WEAPON_HYDRA:
            sGlobal->playEffect(se_gun_hydra);
            break;
        case WEAPON_AER14:
            sGlobal->playEffect(se_gun_laser);
            showLaser();
            break;
        case WEAPON_GLB:
            sGlobal->playEffect(se_gun_glb);
            HeroBullet::make(sGlobal->activeWeapon, 0, 0);
            HeroBullet::make(sGlobal->activeWeapon, 0, -1);
            HeroBullet::make(sGlobal->activeWeapon, 0, 1);
            break;
        case WEAPON_GLR:
            sGlobal->playEffect(se_gun_glr);
            showLaser();
            break;
        case WEAPON_F1:
            showGunLight(WEAPON_F1);
            sGlobal->playEffect(se_gun_f1);
            break;
        case WEAPON_M249:
            sGlobal->playEffect(se_gun_gatling);
            break;
        case WEAPON_E3:
            sGlobal->playEffect(se_gun_e3);
            break;
        case WEAPON_M4A1:
            sGlobal->playEffect(se_gun_shot);
            HeroBullet::make(sGlobal->activeWeapon);
            break;
        case WEAPON_DS2:
            sGlobal->playEffect(se_gun_ds2);
            HeroBullet::make(sGlobal->activeWeapon, 0, 0);
            HeroBullet::make(sGlobal->activeWeapon, 0, -1);
            HeroBullet::make(sGlobal->activeWeapon, 0, 1);
            break;
        case WEAPON_TORPEDO:
            showGunLight(WEAPON_TORPEDO);
            sGlobal->playEffect(se_gun_torpedo);
            HeroBullet::make(sGlobal->activeWeapon);
            break;
        case WEAPON_BLASTING:
            showGunLight(WEAPON_BLASTING);
            sGlobal->playEffect(se_gun_blasting);
            break;
        case WEAPON_RPG:
            sGlobal->playEffect(se_gun_rpg);
            break;
        default:
            break;
    }
    
    spx_hero_body->setLoopCount(0);
    spx_hero_body->playAnimation(spxBodyFireIndex);
    if (!isMotoing) {
        setHeadRotation(0);
    }
    isShooting = true;
    
    switch (sGlobal->activeWeapon) {
        case WEAPON_RPG:
            spx_hero_body->setUnitInterval(0.08f);
            break;
        default:
            spx_hero_body->setUnitInterval(0.05f);
            break;
    }
}

void Hero::fireDart(float x, float y, float scale, float velX, float velY, float atkPercent) {
    if (velX == -1) {
        velX = BULLET_VEL_X;
    }

    HeroBullet::make(sGlobal->activeWeapon, velX, velY, x, y);
}

static void onHeroHurtDelayEnded(wyAction* action, void* data) {
    Hero* hero = (Hero*)data;
    hero->spx_hero_body->setColor(wyc3b(255, 255, 255));
    hero->spx_hero_r_foot->setColor(wyc3b(255, 255, 255));
    hero->spx_hero_l_foot->setColor(wyc3b(255, 255, 255));
}

void Hero::hurt(int atk, bool needBlood) {
    if (!isDead && !isWudi && (!isHurt || hurtTimer > sGlobal->HURT_CD) && !isShowingFaZhen) {
//        if (isHurtByCZ) {
//            isHurtByChongZhuang = true;
//            hurtByCZTimer = 0;
//        }
        
        isHurt = true;
        hurtTimer = 0;
        
        //闪避几率n％
        if (sGlobal->hasSkillMulti[SKILL_EVASION]) {
            int seed = rand()%100;
//            LOGE("dodge:%f", sGlobal->skillMulti[SKILL_9]*100);
            if (seed < sGlobal->skillMulti[SKILL_EVASION]*100) {
                showDodge();
                return;
            }
        }

//        //15％几率抵挡n伤害
//        int absorbDamage = 0;
//        if (sGlobal->hasSkillMulti[SKILL_3]) {
//            int seed = rand()%100;
//            if (seed < ELEMENT_SKILL_3_PARAM1) {
//                absorbDamage = (int)sGlobal->skillMulti[SKILL_3];
//                showShield();
//            } else {
//                sGlobal->playEffect("se_hurt"));
//            }
//        } else {
//            sGlobal->playEffect("se_hurt"));
//        }
        
        //增加n％防御
        int heroDef;
        if (sGlobal->hasSkillMulti[SKILL_ARMOR]) {
            heroDef = sGlobal->skillMulti[SKILL_ARMOR] + sGlobal->HERO_BASE_DEF;//ceil(sGlobal->head_def * (1.0f + sGlobal->skillMulti[SKILL_ARMOR]));
        } else {
            heroDef = sGlobal->HERO_BASE_DEF;
        }
//        LOGE("atk:%d, hero def:%d, curHP:%d", atk, heroDef, sGlobal->curHP);
        heroDef = 1000;
        sGlobal->hudLayer->setHPBar(MAX(atk - heroDef, 0));
        
        sGlobal->effectLayer->showHurtAnim();
//        playHeroAnimation(SPX_HERO_R_LEG_HURT);
        setHeadRotation(-5, false, true);
        
        if (needBlood) {
            bloodSprite->setVisible(true);
            int seed = rand()%2;
            bloodSprite->playAnimation(SPX_PENXUE_1+seed);
            bloodSprite->setPosition(container->getPositionX(), container->getPositionY()+DP(24)*sGlobal->scaleY);
        }
        
        spx_hero_body->setColor(wyc3b(255, 0, 0));
        spx_hero_r_foot->setColor(wyc3b(255, 0, 0));
        spx_hero_l_foot->setColor(wyc3b(255, 0, 0));
        wyIntervalAction *delay = wyDelayTime::make(0.2f);
        wyActionCallback callback = {
            NULL,
            onHeroHurtDelayEnded,
            NULL
        };
        delay->setCallback(&callback, this);
        container->runAction(delay);

//        sGlobal->curHP = -1;
        if (sGlobal->curHP <= 0) {
//            sGlobal->deadType = HERO_DEAD_TYPE_HURT;
//            sGlobal->dead_position_x = container->getPositionX()/sGlobal->tileWidth/sGlobal->scaleX;
//            sGlobal->reportGameEvent(EVT_DEATH);
//            sGlobal->deadType = -1;
//            sGlobal->dead_position_x = -1;

//            if (isFlying) {
//                hideYing();
//            }
            
            body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
            body->SetGravityScale(0.0f);
            sGlobal->mainGameLayer->m_box2d->getWorld()->ClearForces();
            
            setHeadRotation(0, true);
            spx_hero_r_foot->setLoopCount(0);
            spx_hero_l_foot->setLoopCount(0);
            spx_hero_body->setLoopCount(0);
            spx_hero_r_foot->setUnitInterval(0.3f);
            spx_hero_l_foot->setUnitInterval(0.3f);
            spx_hero_body->setUnitInterval(0.3f);
            
            spx_hero_body->playAnimation(spxSiIndex);
            spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_SI);
            spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_SI);
            isHurtRot = false;
            hurtRotTimer = 0;
            sGlobal->hudLayer->setHudOp(false, false, false, false, false, false);
            
//            timer = wyTimer::make(wyTargetSelector::make(this, SEL_i(Hero::doDeadLogic), 0), 1.2f);
//            timer->setOneShot(true);
//            sGlobal->mainGameLayer->scheduleLocked(timer);
//            doDeadLogic(NULL, 0);
            sGlobal->mainGameLayer->runAction(wySequence::make(wyDelayTime::make(1.2f), wyCallFunc::make(wyTargetSelector::make(this, SEL_i(Hero::doDeadLogic), 0)), NULL));
            
            isDead = true;
            isRunning = false;
            isInDead = true;
            
//            isFlying = false;
////            ying->setVisible(false);
//            ying->runAction(wySequence::make(wyBlink::make(0.2f, 2), wyHide::make(), NULL));
//            isDeFlying = false;
//            flyTimer = 0;
            
            isSandShieldActive = false;
            isSandShieldGoning = false;
            sandShieldTimer = 0;

			sGlobal->playEffect(se_player_dead);
        } else {
            sGlobal->playEffect(se_player_hurt);
        }
    }
}

void Hero::doSkill() {
    switch (sGlobal->activeSkill) {
        case SKILL_MISSLE:

            sGlobal->playEffect(se_call_liudan);
            
            isFiringLiudan = true;
            fireLiudanTime = 1;
            fireLiudanTimer = 0;
            fireLiudanInterval = randRange(1, 3)*0.1f;
            liudanSeq = POSSIBLE_COMBO[rand()%6];
            
            new LiuDan(container->getPositionX(), baseY, false, liudanSeq/100);

            break;
        case SKILL_ROBOT: {
            robertStatus = R_APPEARING;
            robert = new Robert();
            
            if (sGlobal->isRealEndless) {
                int achvNo = ACHV_ROBOT_20;
                if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
                    sGlobal->achv_progress[achvNo]+=1;
                    if (sGlobal->achv_progress[achvNo] >= sGlobal->ACHV_NUM[achvNo*3]) {
                        sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                        sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                        sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
                    }
                }
            }
            sGlobal->playEffect(se_robert_speak);
            break;
        }
        case SKILL_SHIELD:
            doSandShield();
            break;
    }
}

void Hero::doRealSkill() {
//    hasShoot = true;
    switch (sGlobal->activeSkill) {
        case SKILL_MISSLE:
            doFireFury();
            break;
        case SKILL_ROBOT:
            doIllusion();
            break;
        case SKILL_SHIELD:
            doSandShield();
            break;
        default:
            break;
    }
}

void Hero::doFireFury() {

}

void Hero::doIllusion() {
    isIllusionActive = true;
    isIllusionGoning = false;
    illusionTimer = 0;
    illusion->setVisible(true);
    illusion->setLoopCount(-1);
    illusion->setReverse(false);
    illusion->playAnimation(sGlobal->effectLayer->animIndex1);
    illusion->setPosition(container->getPositionX(), container->getPositionY() + DP(0));
}

void Hero::doSandShield() {
    if (isSandShieldActive) {
        sandShieldTimer = 0;
        isSandShieldGoning = false;
        isSSFaded = false;
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    sGlobal->sand_shield_cd = sGlobal->skillMulti[SKILL_SHIELD];
//    LOGE("shield last:%f", sGlobal->sand_shield_cd);
    isSSFaded = false;
    isSandShieldActive = true;
    isSandShieldGoning = false;
    sandShieldTimer = 0;
//    wySPXSprite *spxSprite = ((wySPXSprite *)sprite_mao);
//    spxSprite->setLoopCount(-1);
//    spxSprite->setAFCSpriteCallback(NULL, NULL);
//    spxSprite->playAnimation(HERO_TUOLUO);
//    spxSprite->setScaleX(sGlobal->scaleX*1.2f);
//    spxSprite->setScaleY(sGlobal->scaleY*1.2f);
//    
    b2PolygonShape box;
    box.SetAsBox(m_box2d->pixel2Meter(DP(65)*sGlobal->scaleX), halfBodyHeightInMeter/3, b2Vec2(m_box2d->pixel2Meter(-DP(0)*sGlobal->scaleX), 0), 0);
    b2FixtureDef fd;
    fd.shape = &box;
    fd.restitution = 0.0f;
    fd.friction = 0.0f;
    fd.density = 0.0f;
    fd.isSensor = false;
    shieldFixture = body->CreateFixture(&fd);
    
//
//    shandian = wySPXSprite::make("shandian"), MyTextureManager::makePNG("shandian")), 0);
//    shandian->setForceTickMode(true);
//	shandian->setUnitInterval(0.1f);
//    shandian->setLoopCount(-1);
//    sGlobal->mainGameLayer->addChildLocked(shandian, container->getZOrder());
//    shandian->setScaleX(sGlobal->scaleX*1.0f);
//    shandian->setScaleY(sGlobal->scaleY*1.0f);
//
    
    sGlobal->playEffect(se_shield_active);
    
    showShield();
}

void Hero::sandShieldGone() {
    isSandShieldActive = false;
    sandShieldTimer = 0;
    isSandShieldGoning = true;
//    wySPXSprite *spxSprite = ((wySPXSprite *)sprite_mao);
//    spxSprite->setLoopCount(-1);
//    spxSprite->setAFCSpriteCallback(NULL, NULL);
//    spxSprite->playAnimation(HERO_RUN);
//    spxSprite->setScaleX(sGlobal->scaleX*0.8f);
//    spxSprite->setScaleY(sGlobal->scaleY*0.8f);
//    
//    sGlobal->mainGameLayer->removeChildLocked(shandian, true);
//    shandian = NULL;
//
    
//    sGlobal->mainGameLayer->needDestroyShield = true;
    if (shieldFixture != NULL) {
        sGlobal->mainGameLayer->addToDestroyFixture(shieldFixture);
        shieldFixture = NULL;
    }
    
    shieldContainer->setVisible(false);
}

void Hero::doEndLogic(wyTargetSelector* ts, void *data) {
//    LOGE("bbb");
    wyNode *endPoint = (wyNode *)data;
    
    if (isMotoing) {
        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        
        spx_hero_r_foot->setLoopCount(-1);
        spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_IDLE);
        
        spx_hero_body->setPosition(spx_hero_body->getPositionX()+DP(4)*sGlobal->scaleY, spx_hero_body->getPositionY()+DP(4)*sGlobal->scaleY);
        
        if (speedline_emitter != NULL) {
            speedline_emitter->stopSystem();
        }
    } else {
        stop();
    }
    
//    timer = wyTimer::make(wyTargetSelector::make(this, SEL_p(Hero::doEndAnimation), endPoint), 0.5f);
//    timer->setOneShot(true);
//    sGlobal->mainGameLayer->scheduleLocked(timer);
    
    sGlobal->mainGameLayer->runAction(wySequence::make(wyDelayTime::make(0.5f), wyCallFunc::make(wyTargetSelector::make(this, SEL_p(Hero::doEndAnimation), endPoint)), NULL));
}

void Hero::doEndAnimation(wyTargetSelector* ts, void *data) {
//    spx_hero_body->playAnimation(42);
//    wySPXSprite *qiang = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), 44+sGlobal->activeWeapon);
//    qiang->setForceTickMode(true);
//	qiang->setUnitInterval(0.05f);
//    qiang->setScaleX(0.62f*sGlobal->scaleX);
//    qiang->setScaleY(0.62f*sGlobal->scaleY);
//    sGlobal->mainGameLayer->addChildLocked(qiang);
//    qiang->setPosition(container->getPositionX(), container->getPositionY());
//    qiang->runAction(wyRotateBy::make(5.0f, 360*10));
//    qiang->runAction(wyJumpBy::make(1.0f, -DP(200)*sGlobal->scaleX, -DP(400)*sGlobal->scaleY, DP(500)*sGlobal->scaleY, 1));
    wyNode *endPoint = (wyNode *)data;
    wySPXSprite *spx = (wySPXSprite*)endPoint->getChildByTag(1);
    spx->setUnitInterval(0.1f);
    spx->playAnimation(SPX_NVREN_FEIWEN);
    spx->setLoopCount(0);
    
    wySpriteEx *heart = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "heart.png");//wySprite::make(MyTextureManager::makePNG("heart.png"));
    heart->setScaleX(0.5f*sGlobal->scaleX);
    heart->setScaleY(0.5f*sGlobal->scaleY);
    heart->setPosition(endPoint->getPositionX()+DP(-10)*sGlobal->scaleX, endPoint->getPositionY()+DP(96)*sGlobal->scaleY);
    sGlobal->mainGameLayer->addChildLocked(heart, 999);
    heart->runAction(wySequence::make(wyDelayTime::make(0.2f), wyMoveBy::make(1.0f, DP(-80)*sGlobal->scaleX, DP(0)*sGlobal->scaleY), NULL));
//    heart->setAlpha(0);
    heart->runAction(wySequence::make(wyDelayTime::make(1.1f), wyFadeOut::make(0.1f), NULL));
    
//    timer = wyTimer::make(wyTargetSelector::make(this, SEL_p(Hero::showWinDialog), data), 1.5f);
//    timer->setOneShot(true);
//    sGlobal->mainGameLayer->scheduleLocked(timer);
    
    sGlobal->mainGameLayer->runAction(wySequence::make(wyDelayTime::make(1.5f), wyCallFunc::make(wyTargetSelector::make(this, SEL_p(Hero::showWinDialog), data)), NULL));
}

void Hero::showWinDialog(int needForceNext, void *data) {
    if (sGlobal->index == 0) {
        if (sGlobal->isTrainingEnd == 0) {
            sGlobal->isTrainingEnd = 1;
            MyPrefs::setInt(DB_IS_TRAINING_END, 1);
        }
        
        if (sGlobal->isTutorialFromMenu) {
            sGlobal->mainGameLayer->onExit();
            sGlobal->effectLayer->onExit();
            sGlobal->hudLayer->onExit();
            
            sGlobal->gotoCGScene();
        } else {
            wyNode *endPoint = (wyNode *)data;
            wySPXSprite *spx = (wySPXSprite*)endPoint->getChildByTag(1);
            spx->setUnitInterval(0.2f);
            spx->playAnimation(SPX_NVREN_DAIJI);
            spx->setLoopCount(-1);
            
            sGlobal->hudLayer->trainToLv1();
        }
    } else {
        needForceNext = tempFlag;
        GameCompleteLayer *cLayer;
        switch (needForceNext) {
            case 0:
                cLayer = (GameCompleteLayer*) (new GameCompleteLayer(false))->autoRelease();
                break;
            case 1:
                //打开技能面板，教学技能
                cLayer = (GameCompleteLayer*) (new GameCompleteLayer(true, 1))->autoRelease();
                break;
            case 2:
                //打开道具面板，教学闪电标
                cLayer = (GameCompleteLayer*) (new GameCompleteLayer(true, 2))->autoRelease();
                break;
            case 3:
                //打开铁匠面板，教学武器升级
                cLayer = (GameCompleteLayer*) (new GameCompleteLayer(true, 0))->autoRelease();
                break;
            default:
                cLayer = (GameCompleteLayer*) (new GameCompleteLayer(false))->autoRelease();
                break;
        }
        
        sGlobal->mainGameLayer->getParent()->addChild(cLayer, 4, 4);
        sGlobal->mainGameLayer->onExit();
        sGlobal->effectLayer->onExit();
        sGlobal->hudLayer->onExit();
        ((MainScene *)sGlobal->mainGameLayer->getParent())->sceneState = SC_COMPLETE_OPEND;
    }
}

void Hero::playHeroAnimation(int index) {
    if (isInDead) {
        return;
    }
//    LOGE("play anim: %d...", index);
    prevAnimIndex = spx_hero_r_foot->getCurrentAnimationIndex();

    switch (index) {
        case SPX_HERO_R_LEG_JUMP1: {
            if (prevAnimIndex == index) {
                return;
            }

            if (isMotoing) {
                spx_moto_body->stopAllActions();
                spx_hero_body->stopAllActions();
                spx_hero_r_foot->stopAllActions();
                
                spx_moto_body->setAlpha(255);
                spx_hero_body->setAlpha(255);
                spx_hero_r_foot->setAlpha(255);

                moto_b_wheel_container->stopAllActions();
                moto_f_wheel_container->stopAllActions();
                
                moto_b_wheel_container->setScaleX(1.0f);
                moto_b_wheel_container->setScaleY(1.0f);
                moto_f_wheel_container->setScaleX(1.0f);
                moto_f_wheel_container->setScaleY(1.0f);

                container->runAction(wyRotateTo::make(0.05f, 0, -20));
                spx_hero_body->runAction(wyRotateTo::make(0.05f, 0, 16));
                
                float ofy = DP(6)*sGlobal->scaleY;
                spx_hero_body->setPosition(spx_hero_body->getPositionX(), oy1+ofy);
                spx_hero_r_foot->setPosition(spx_hero_r_foot->getPositionX(), oy2+ofy);
                spx_moto_body->setPosition(spx_moto_body->getPositionX(), oy3+ofy);
                
                moto_b_wheel_container->setPosition(moto_b_wheel_container->getPositionX(), oy4+ofy);
                moto_f_wheel_container->setPosition(moto_f_wheel_container->getPositionX(), oy5+ofy);
            } else {
                spx_hero_r_foot->setLoopCount(0);
                spx_hero_l_foot->setLoopCount(0);
                
                spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_JUMP1);
                spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_JUMP1);
                setHeadRotation(3);
//                LOGE("rot 3");
                
                spx_hero_body->setPosition(0, 0);
                spx_hero_r_foot->setPosition(0, 0);
                spx_hero_l_foot->setPosition(0, 0);
            }

            break;
        }
        case SPX_HERO_R_LEG_JUMP2: {
            if (prevAnimIndex == index) {
                return;
            }
//            
//            if (isMotoing) {
//                container->runAction(wyRotateTo::make(0.02f, 0, -20));
//                spx_hero_body->runAction(wyRotateTo::make(0.02f, 0, 16));
//                
//                float ofy = DP(6)*sGlobal->scaleY;
//                spx_hero_body->setPosition(spx_hero_body->getPositionX(), oy1+ofy);
//                spx_hero_r_foot->setPosition(spx_hero_r_foot->getPositionX(), oy2+ofy);
//                spx_moto_body->setPosition(spx_moto_body->getPositionX(), oy3+ofy);
//                spx_moto_b_wheel->setPosition(spx_moto_b_wheel->getPositionX(), oy4+ofy);
//                spx_moto_f_wheel->setPosition(spx_moto_f_wheel->getPositionX(), oy5+ofy);
//            } else {
                spx_hero_r_foot->setLoopCount(0);
                spx_hero_l_foot->setLoopCount(0);
                
                spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_JUMP2);
                spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_JUMP2);

                spx_hero_body->setPosition(0, 0);
                spx_hero_r_foot->setPosition(0, 0);
                spx_hero_l_foot->setPosition(0, 0);
//            }
            
            break;
        }
        case SPX_HERO_R_LEG_FALL: {
            if (prevAnimIndex == index) {
                return;
            }
            
//            LOGE("fall...");
            if (isMotoing) {

            } else {
                spx_hero_r_foot->setLoopCount(0);
                spx_hero_l_foot->setLoopCount(0);
                
                spx_hero_r_foot->setUnitInterval(0.05f);
                spx_hero_l_foot->setUnitInterval(0.05f);
                
                spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_FALL);
                spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_FALL);
                
                spx_hero_body->setPosition(0, 0);
                setHeadRotation(-3);
                spx_hero_r_foot->setPosition(0, 0);
                spx_hero_l_foot->setPosition(0, 0);
                
//                LOGE("cur rot:%f", spx_hero_body->getRotation());
            }
            
            break;
        }
        case SPX_HERO_R_LEG_DB_JUMP: {
            if (prevAnimIndex == index) {
                return;
            }
            
            isDBJumping = true;
            isDBJumpAnimating = true;
            isBodySensorCreated = false;
            
            if (isMotoing) {
                container->runAction(wySequence::make(wyRotateTo::make(0.02f, -20, -18), wyRotateTo::make(0.02f, -18, -24), NULL));
                spx_hero_body->runAction(wySequence::make(wyRotateTo::make(0.02f, 16, 14), wyRotateTo::make(0.02f, 14, 18), NULL));
            } else {
                spx_hero_r_foot->setLoopCount(0);
                spx_hero_l_foot->setLoopCount(0);
                
                spx_hero_r_foot->setUnitInterval(0.02f);
                spx_hero_l_foot->setUnitInterval(0.02f);
                
                spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_DB_JUMP);
                spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_DB_JUMP);
                
                float posy = DP(-2)*spx_hero_body->getScaleY();
                float posy1 = DP(10)*spx_hero_body->getScaleY();
                float angel = 4;
                
                spx_hero_body->setPosition(DP(8)*spx_hero_body->getScaleY(), posy);
                setHeadRotation(angel);
                
                spx_hero_r_foot->setPosition(0, posy1);
                spx_hero_l_foot->setPosition(0, posy1);
            }

            break;
        }
        case SPX_HERO_R_LEG_IDLE:{
            if (prevAnimIndex == index) {
                return;
            }
            spx_hero_body->playAnimation(spxBodyIdleIndex);
            spx_hero_body->setLoopCount(0);
            spx_hero_body->setAFCSpriteCallback(NULL, NULL);
            
            spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_IDLE);
            spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_IDLE);
            break;
        }
        case SPX_HERO_R_LEG_RUN:{
            if (prevAnimIndex == index) {
                return;
            }

            if (isMotoing) {

            } else {
                spx_hero_r_foot->setLoopCount(-1);
                spx_hero_l_foot->setLoopCount(-1);
                
                spx_hero_r_foot->setUnitInterval(0.05f);
                spx_hero_l_foot->setUnitInterval(0.05f);
                
                spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_RUN);
                spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_RUN);
            }

            break;
        }
        case SPX_HERO_R_LEG_LAND:{
            if (prevAnimIndex == index || isDead) {
                return;
            }
            
            if (isMotoing) {
                float ofyy = DP(8)*sGlobal->scaleY;
                wyIntervalAction *a = wyEaseBounceIn::make(wyMoveBy::make(0.25f, 0, -ofyy));
                wyIntervalAction *a1 = wySequence::make(a, (wyIntervalAction*)a->reverse(), NULL);
                a1->setTag(999);
                spx_moto_body->runAction(a1);
                spx_hero_body->runAction(a1->copy());
                spx_hero_r_foot->runAction(a1->copy());
                
                float scaleX = 1.0f;
                float scaleY = 1.0f;
                wyIntervalAction *a2 = wyEaseBounceIn::make(wyScaleTo::make(0.25f, scaleX, scaleY, scaleX, scaleY-0.18f));
                wyIntervalAction *a21 = wySequence::make(a2, (wyIntervalAction*)a2->reverse(), NULL);
                moto_b_wheel_container->runAction(a21);
                moto_f_wheel_container->runAction(a21->copy());
                a = wyEaseBounceIn::make(wyMoveBy::make(0.25f, 0, -ofyy/4));
                a1 = wySequence::make(a, (wyIntervalAction*)a->reverse(), NULL);
                moto_b_wheel_container->runAction(a1);
                moto_f_wheel_container->runAction(a1->copy());
                
                float ofy = DP(0)*sGlobal->scaleY;
                spx_hero_body->setPosition(spx_hero_body->getPositionX(), oy1+ofy);
                spx_hero_r_foot->setPosition(spx_hero_r_foot->getPositionX(), oy2+ofy);
                spx_moto_body->setPosition(spx_moto_body->getPositionX(), oy3+ofy);
                
                moto_b_wheel_container->setPosition(moto_b_wheel_container->getPositionX(), oy4+ofy);
                moto_f_wheel_container->setPosition(moto_f_wheel_container->getPositionX(), oy5+ofy);
                
            } else {
                spx_hero_r_foot->setLoopCount(0);
                spx_hero_l_foot->setLoopCount(0);
                
                spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_LAND);
                spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_LAND);
                
                spx_hero_body->setPosition(0, DP(-24)*spx_hero_body->getScaleX());
            }
            
            break;
        }
//        case HERO_JUMP_SECOND_0: {
//            prevAnimIndex = HERO_JUMP_UP;
//            prevLoopCount = 0;
//            wyAFCSpriteCallback callback = {
//                NULL,
//                onAFCAnimationEnded
//            };
//            spxSprite->playAnimation(index);
//            spxSprite->setLoopCount(0);
//            spxSprite->setAFCSpriteCallback(&callback, this);
//            
//            jumpBomb->setVisible(true);
//            jumpBomb->playAnimation(HERO_JUMP_SECOND_1);
//            jumpBomb->setPosition(container->getPositionX(), container->getPositionY());
//            
////            m_streak1->reset();
////            isStreaking = true;
////            m_streak1->setVisible(true);
//            break;
//        }
//        case SPX_HERO_R_LEG_HURT: {
//            if (prevAnimIndex == index) {
//                return;
//            }
//            
//            spx_hero_r_foot->setLoopCount(0);
//            spx_hero_l_foot->setLoopCount(0);
//            
//            spx_hero_r_foot->setUnitInterval(0.2f);
//            spx_hero_l_foot->setUnitInterval(0.2f);
//            
//            spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_HURT);
//            spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_HURT);
//            
//            spx_hero_body->setPosition(0, DP(-12)*spx_hero_body->getScaleX());
//            
//            break;
//        }
        default:
            break;
    }
}

void Hero::hideYing() {
//    if (!isYingReleased) {
////        isYingReleasing = true;
//////        wyIntervalAction *a = wyMoveBy::make(2.0f, wyDevice::winWidth*3, 0);
////        wyIntervalAction *a = wyDelayTime::make(0.1f);
////        wyActionCallback callback = {
////            NULL,
////            onHideYingEnded
////        };
////        a->setCallback(&callback, this);
////        ying->runAction(a);
////
////        isYingReleased = true;
////        body->SetGravityScale(1.0f);
//        flyTimer = 0;
//        isFlying = false;
////        isStartFlyingEnd = false;
////        isDeFlying = false;
//        
//        isYingReleased = true;
//        body->SetGravityScale(1.0f);
////        ying->setVisible(false);
//        wyIntervalAction* a = wyFadeTo::make(0.15f, 255, 100);
//        wyIntervalAction* r = wySequence::make(a,
//                                               (wyFiniteTimeAction*)a->reverse(),
//                                               a->copy(),
//                                               (wyFiniteTimeAction*)a->reverse(),
//                                               wyHide::make(),
//                                               NULL);
////        wyIntervalAction *a = wySequence::make(wyBlink::make(0.5f, 2), wyHide::make(), NULL);
//        wyActionCallback callback = {
//            NULL,
//            onHideYingEnded
//        };
//        r->setCallback(&callback, this);
//        yingSprite->runAction(r);
//        isYingReleasing = false;
//        isYingReleased = true;
//    }

    flyTimer = 0;
    isFlying = false;
    if (!isDead) {
        body->SetGravityScale(1.0f);
    }
    
//    wyIntervalAction* a = wyFadeTo::make(0.15f, 255, 100);
//    wyIntervalAction* r = wySequence::make(a,
//                                           (wyFiniteTimeAction*)a->reverse(),
//                                           a->copy(),
//                                           (wyFiniteTimeAction*)a->reverse(),
//                                           a->copy(),
//                                           (wyFiniteTimeAction*)a->reverse(),
//                                           wyHide::make(),
//                                           NULL);
//    wyActionCallback callback = {
//        NULL,
//        onHideYingEnded
//    };
//    r->setCallback(&callback, this);
//    yingSprite->runAction(r);

    onHideYingEnded(NULL, this);
//    yingSprite->setPaused(true);
    
//    isFlyingSlowDown = true;
//    body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -6));
    
//    isYingReleasing = true;
//    isYingReleased = true;

}

void Hero::doFly(bool needSE) {
    if (needSE) {
        sGlobal->playEffect(se_jetpack);
    }
    
    isFlying = true;
    isStartFlyingEnd = true;
    isDeFlying = false;
    flyTimer = 0;
    
    yingSprite->stopAllActions();

    ying->setAlpha(255);
    ying->setVisible(true);
    yingSprite->setAlpha(255);
    yingSprite->setVisible(true);
    
    body->SetGravityScale(0);
    body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, HERO_FLY_Y_DOWN));
//    body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, 0));
    
//    if (fly_emitter != NULL) {
//        ying->removeChildLocked(fly_emitter, true);
//    }
//    fly_emitter = wyParticleLoader::load("particle_ying.plist");
//    fly_emitter->setScaleX(sGlobal->scaleX*1.0f);
//    fly_emitter->setScaleY(sGlobal->scaleY*1.0f);
//    ying->addChildLocked(fly_emitter, 0);
//    fly_emitter->setPositionType(FREE);
//    fly_emitter->setVisible(false);

	flyDuration = HERO_FLY_TIME;
    
//    playHeroAnimation(SPX_HERO_R_LEG_JUMP2);
    
    if (isMotoing) {
        boostTimer = 0;
        isBoosting = false;
        isDeBoosting = true;
        deBoostTimer = 0;
        hideMoto();
    }
    
    if (sGlobal->isOnGround) {
        prevAnimIndex = -1;
        playHeroAnimation(SPX_HERO_R_LEG_RUN);
    } else {
        prevAnimIndex = -1;
        playHeroAnimation(SPX_HERO_R_LEG_JUMP2);
    }
}

void Hero::doBoost() {
    if (isFlying) {
        ying->setVisible(false);
        hideYing();
    }
    
    if (isBoosting || (isDeBoosting && !isMotoRunning)) {
        isMotoing = true;
        isBoosting = true;
        isDeBoosting = false;
        boostTimer = 0;
        deBoostTimer = 0;
        body->SetLinearVelocity(b2Vec2(sGlobal->heroVelocityX*2.0f, body->GetLinearVelocity().y));
        curVelX = sGlobal->heroVelocityX*2.0f;
        
        return;
    }
    if (isDeBoosting )
    
    if (!isBoosting && !isDeBoosting) {
        heroZ = sGlobal->heroEyeZ;
    }
    sGlobal->playEffect(se_boost);
    sGlobal->playEffect(se_motocycle);
    
    isMotoing = true;
    isBoosting = true;
    isDeBoosting = false;
    boostTimer = 0;
    deBoostTimer = 0;
    body->SetLinearVelocity(b2Vec2(sGlobal->heroVelocityX*2.0f, body->GetLinearVelocity().y));
    curVelX = sGlobal->heroVelocityX*2.0f;
    
    showMoto();
    
    boost_emitter = wyParticleLoader::load("particle_moto.plist");
    boost_emitter->setScaleX(sGlobal->scaleX*0.5f);
    boost_emitter->setScaleY(sGlobal->scaleY*0.5f);
    sGlobal->mainGameLayer->addChildLocked(boost_emitter, 100);
//    boost_emitter->setPosition(container->getPositionX(), container->getPositionY()+DP(20)*sGlobal->scaleY);
    boost_emitter->setPositionType(FREE);
//    boost_emitter->setAlpha(200);
}

static void onZhanShenEnded(wyAction* action, void* data) {
    Hero *hero = (Hero *)data;
    sGlobal->mainGameLayer->removeChildLocked(hero->zhanshen, true);
    hero->zhanshen = NULL;
    hero->isZhanShenShowing = false;
}

void Hero::doBiao() {
    if (isZhanShenShowing && zhanshen != NULL) {
        sGlobal->mainGameLayer->removeChildLocked(zhanshen, true);
    }
    sGlobal->playEffect(se_call_liudan);
//    zhanshenDist = -DP(266)*sGlobal->scaleX;
//    isZhanShenShowing = true;
//    zhanshen = wySPXSprite::make("test_items"), MyTextureManager::makePNG("test_items")), 9);
//    zhanshen->setForceTickMode(true);
//	zhanshen->setUnitInterval(0.1f);
//    zhanshen->setLoopCount(-1);
//    sGlobal->mainGameLayer->addChildLocked(zhanshen);
//    zhanshen->setScaleX(sGlobal->scaleX*1.0f);
//    zhanshen->setScaleY(sGlobal->scaleY*1.0f);
//    zhanshen->setPosition(DP(-200)*sGlobal->scaleY, baseY + DP(100)*sGlobal->scaleY);
//    wyIntervalAction *a = wyMoveBy::make(0.5f, DP(200)*sGlobal->scaleX, 0);
//    wyIntervalAction *a1 = wySequence::make(wyDelayTime::make(5.0f), wyFadeOut::make(0.5f), NULL);
//    wyActionCallback callback = {
//        NULL,
//        onZhanShenEnded
//    };
//    a1->setCallback(&callback, this);
//    zhanshen->runAction(a1);
    isFiringMissle = true;
    firingMissleTimer = 0;
    fireTimes = 1;
    for (int i = 0; i < rand()%2+1; i++) {
        new Missle(container->getPositionX(), baseY, i);
    }
}

void Hero::doFadeEffect() {
    wyIntervalAction* a = wyFadeTo::make(0.1f, 255, 100);
    wyIntervalAction* r = wySequence::make(a,
                                           (wyFiniteTimeAction*)a->reverse(),
                                           a->copy(),
                                           (wyFiniteTimeAction*)a->reverse(),
                                           a->copy(),
                                           (wyFiniteTimeAction*)a->reverse(),
                                           a->copy(),
                                           (wyFiniteTimeAction*)a->reverse(),
                                           a->copy(),
                                           (wyFiniteTimeAction*)a->reverse(),
                                           a->copy(),
                                           (wyFiniteTimeAction*)a->reverse(),
                                           NULL);
    
    if (ying->isVisible()) {
        yingSprite->runAction(r->copy());
    } else {
        spx_hero_body->runAction(r);
        spx_hero_r_foot->runAction(r->copy());
        spx_hero_l_foot->runAction(r->copy());
    }
}

void Hero::destroyMotoFixture() {
    if (motoFixture != NULL) {
        body->DestroyFixture(motoFixture);
        motoFixture = NULL;
    }
}

void Hero::destroyShieldFixture() {
    if (shieldFixture != NULL) {
        body->DestroyFixture(shieldFixture);
        shieldFixture = NULL;
    }
}

void Hero::doXuePing() {
    int recoverHP = sGlobal->cloth_hp - sGlobal->curHP;
//    sGlobal->curHP = MIN(sGlobal->cloth_hp, sGlobal->curHP + sGlobal->cloth_hp);
    sGlobal->curHP = sGlobal->cloth_hp;
	sGlobal->hudLayer->setHPBar(0);
    showHealEffect();
//    LOGE("xueping hp:%d", recoverHP);
//    sGlobal->playEffect("se_heal"));
    showRecoverHPEffect(recoverHP, false);
}

void Hero::doTool(int toolIndex) {

}

void Hero::doBonus(int bonusIndex) {
    switch (bonusIndex) {
        case BONUS_BIAO:
            sGlobal->mainGameLayer->hero->doBiao();
            recoverHPWhenUseItem();
            break;
        case BONUS_YING:
            sGlobal->mainGameLayer->hero->doFly();
            recoverHPWhenUseItem();
            break;
        case BONUS_XIEZI:
            sGlobal->mainGameLayer->hero->doBoost();
            recoverHPWhenUseItem();
            break;
        case BONUS_XUE:
            sGlobal->mainGameLayer->hero->doXuePing();
            break;
        default:
            break;
    }
}

void Hero::recoverHPWhenUseItem() {

}

void Hero::reachCheckPoint() {
    sGlobal->hasReachCheckPoint = true;
    
    sGlobal->prev_gold_normal = sGlobal->level_gold_normal;
    sGlobal->prev_gold = sGlobal->level_gold;
    sGlobal->prev_kills = sGlobal->level_kills;
    sGlobal->prev_enemygold = sGlobal->level_enemy_gold;
    
//    LOGE("log enemy gold:%d", sGlobal->prev_enemygold);
}

void Hero::stop() {
    body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    spx_hero_body->setLoopCount(-1);
    spx_hero_body->playAnimation(spxBodyIdleIndex);
    
    spx_hero_r_foot->setLoopCount(-1);
    spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_IDLE);
    
    spx_hero_l_foot->setLoopCount(-1);
    spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_IDLE);
    
    isFlying = false;
    ying->setVisible(false);
//    ying->runAction(wySequence::make(wyBlink::make(0.2f, 2), wyHide::make(), NULL));
    body->SetGravityScale(1.0f);
    
    isDeFlying = false;
    isBoosting = false;
    isDeBoosting = false;
    isBackingToNormal = false;
    isRunning = false;
    
    spx_hero_body->setPosition(0, DP(-2)*sGlobal->scaleY);
    spx_hero_r_foot->setPosition(0, DP(-2)*sGlobal->scaleY);
    spx_hero_l_foot->setPosition(0, DP(-2)*sGlobal->scaleY);
}

void Hero::pause() {
    body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

//    isFlying = false;
//    isDeFlying = false;
//    isBoosting = false;
//    isDeBoosting = false;
//    isBackingToNormal = false;
    isRunning = false;
}

void Hero::run() {
    isRunning = true;
    body->SetLinearVelocity(b2Vec2(sGlobal->heroVelocityX, body->GetLinearVelocity().y));
    
    spx_hero_body->setLoopCount(-1);
    spx_hero_body->setUnitInterval(0.05f);
    spx_hero_body->playAnimation(spxBodyIdleIndex);
    
    spx_hero_r_foot->setLoopCount(-1);
    spx_hero_r_foot->setUnitInterval(0.05f);
    spx_hero_r_foot->playAnimation(SPX_HERO_R_LEG_RUN);
    
    spx_hero_l_foot->setLoopCount(-1);
    spx_hero_l_foot->setUnitInterval(0.05f);
    spx_hero_l_foot->playAnimation(SPX_HERO_L_LEG_RUN);
    
    spx_hero_body->setPosition(0, DP(0)*sGlobal->scaleY);
    spx_hero_r_foot->setPosition(0, DP(0)*sGlobal->scaleY);
    spx_hero_l_foot->setPosition(0, DP(0)*sGlobal->scaleY);
}

void Hero::dealDeadInGuide() {
    sGlobal->mainGameLayer->onExit();
    sGlobal->effectLayer->onExit();
    sGlobal->hudLayer->onExit();
    
    ((MainScene *)sGlobal->mainGameLayer->getParent())->reset(true);
}

void Hero::dealEndInGuide() {
    sGlobal->hudLayer->setHudStatus(GUIDE_5_1);
    stop();

    NPC *npc = NPC::make(container->getPositionX() + wyDevice::winWidth, baseY + DP(0)*sGlobal->scaleY, NULL, NULL);
    npc->move(-10.0f, 1.2f);
}

static void onFaZhenEnded(wyAFCSprite* sprite, void* data) {
    Hero *hero = (Hero *)data;
    hero->isFaZhenEnded = true;
}

static void onFaZhenChangeded(wyAFCSprite* sprite, void* data) {
    Hero *hero = (Hero *)data;
    if (sprite->getCurrentFrame() == 8) {
//        sGlobal->mainGameLayer->addToEnable(hero->body);
        hero->container->setVisible(true);
        hero->spx_hero_body->runAction(wyFadeIn::make(1.5f));
        hero->spx_hero_r_foot->runAction(wyFadeIn::make(1.5f));
        hero->spx_hero_l_foot->runAction(wyFadeIn::make(1.5f));
    }
}

void Hero::showFaZhen() {
    isRunning = false;
//    sGlobal->mainGameLayer->addToDisable(body);
    sGlobal->mainGameLayer->needAutoScroll = false;
    stop();
//    container->setVisible(false);
//    shadow->setVisible(false);
//    isShowingFaZhen = true;
//    isFaZhenEnded = false;
//    
//    fazhen = wySPXSprite::make("fazhen.sprite", false, MyTextureManager::makePNG("fazhen.png"), 0);
//    fazhen->setForceTickMode(true);
//	fazhen->setUnitInterval(0.25f);
//    fazhen->setLoopCount(0);
//    sGlobal->mainGameLayer->addChildLocked(fazhen);
//    fazhen->setScaleX(sGlobal->scaleX*1.2f);
//    fazhen->setScaleY(sGlobal->scaleY*1.2f);
//    fazhen->setPosition(container->getPositionX(), container->getPositionY()-DP(12)*sGlobal->scaleY);
//    wyAFCSpriteCallback callback = {
//        onFaZhenChangeded,
//        onFaZhenEnded
//    };
//    fazhen->setAFCSpriteCallback(&callback, this);
    
    sGlobal->hudLayer->setHudOp(false, false, false, false, false, false);
    
    isFHing = true;
}

void Hero::showHealEffect() {
    isHealing = true;
    healTimer = 0;
    
    if (heal_emitter != NULL) {
        sGlobal->mainGameLayer->removeChildLocked(heal_emitter, true);
    }
    heal_emitter = wyParticleLoader::load("particle_heal.plist");
    heal_emitter->setScaleX(sGlobal->scaleX*1.0f);
    heal_emitter->setScaleY(sGlobal->scaleY*1.0f);
    sGlobal->mainGameLayer->addChildLocked(heal_emitter, 101);
    heal_emitter->setPositionType(GROUPED);
}





static void onMissleEnded(wyAFCSprite* sprite, void* data) {
    Missle* missle = (Missle*)data;
    sGlobal->mainGameLayer->addToDestroy(missle->body);
}

Missle::Missle(float baseX, float baseY, int i) {
    //LOGE("baseX:%f, baseY:%f", baseX, baseY);
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
    sGlobal->mainGameLayer->addChildLocked(container);
    
//    float scale = 0.6f;//(float)(rand()%20+30)/100.0f;
    wySPXSprite *bullet = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);//wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), SPX_HUOJIAN);
    bullet->setForceTickMode(true);
    bullet->setUnitInterval(0.06f);
    bullet->setLoopCount(-1);
    bullet->setScaleX(sGlobal->scaleX*0.65f);
    bullet->setScaleY(sGlobal->scaleY*0.6f);
    bullet->playAnimation(SPX_MISSLE_1);
    container->addChildLocked(bullet);
    spxSprite = bullet;
    
    int xx;
    int yy;
    
    if (rand()%2 == 0) {
        xx = -rand()%80;
    } else {
        xx = rand()%(wyDevice::winWidth/2);
    }
    
    yy = rand()%200 + 300;
    
    xx = DP(xx)*sGlobal->scaleX;
    yy = DP(yy)*sGlobal->scaleY;
    
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(m_box2d->pixel2Meter(baseX + xx), m_box2d->pixel2Meter(baseY + yy));
    bd.gravityScale = 0.0f;
    b2Body* bulletBody = m_box2d->getWorld()->CreateBody(&bd);
    bulletBody->SetTransform(bd.position, 0);
    b2PolygonShape box;
    box.SetAsBox(m_box2d->pixel2Meter(DP(60)*sGlobal->scaleX), m_box2d->pixel2Meter(DP(50)*sGlobal->scaleY));
    b2FixtureDef fd;
    fd.shape = &box;
    fd.restitution = 0.0f;
    fd.friction = 0.0f;
    fd.density = 0.0f;
    fd.isSensor = true;
    fd.filter.maskBits = MASK_BITS[TYPE_MISSLE];
    if (rand()%2 == 0) {
        fd.filter.maskBits = fd.filter.maskBits ^ CAT_BITS[TYPE_GROUND];
    }
    fd.filter.categoryBits = CAT_BITS[TYPE_MISSLE];
    fd.filter.groupIndex = GROUND_INDEX[TYPE_MISSLE];
    
    bulletBody->CreateFixture(&fd);
    
    this->obj_type = TYPE_MISSLE;
    this->body = bulletBody;
    bulletBody->SetUserData(this);
//    if (sGlobal->hudLayer->status == GUIDE_11_2) {
//        bulletBody->SetLinearVelocity(b2Vec2(sGlobal->heroVelocityX * 1.5f, 0.0f));
//        bullet->runAction(wySequence::make(wyDelayTime::make(0.4f), wyFadeIn::make(0.4f), NULL));
//        sGlobal->hudLayer->setHudStatus(GUIDE_NONE);
//    } else {
    float velX, velY;
    velX = MAX(sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x * 2.0f, HERO_VEL_X* 2.0f);
    velY = -rand()%10 - 15;
        bulletBody->SetLinearVelocity(b2Vec2(velX, velY));
//        bullet->runAction(wySequence::make(wyDelayTime::make(0.6f), wyFadeIn::make(0.5f), NULL));
//    }
    
    float angle = atan(velY/velX);
    bullet->setRotation(-angle*57.3f);
//    LOGE("missle angle:%f", bullet->getRotation());
    
    deadSprite = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);//wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), SPX_JIQUN_HUOJIAN_BAOZHA);
    sGlobal->mainGameLayer->addChildLocked(deadSprite, 102);
    deadSprite->setForceTickMode(true);
    deadSprite->setUnitInterval(0.1f);
    deadSprite->setLoopCount(0);
    deadSprite->setScaleX(1.5f*sGlobal->scaleX);
    deadSprite->setScaleY(1.5f*sGlobal->scaleY);
    deadSprite->playAnimation(SPX_JIQUN_HUOJIAN_BAOZHA);
    wyAFCSpriteCallback callback = {
        NULL,
        onMissleEnded
    };
    deadSprite->setVisible(false);
    deadSprite->setAFCSpriteCallback(&callback, this);
    deadSprite->setPosition(container->getPositionX(), container->getPositionY()-DP(40)*sGlobal->scaleY);
    
    bullet_emitter = (wyParticleSystem*) sGlobal->popFromCache(sGlobal->fire_emitterArray);//wyParticleLoader::load("particle_rpg_rocket.plist");
    bullet_emitter->setScaleX(sGlobal->scaleX*0.3f);
    bullet_emitter->setScaleY(sGlobal->scaleY*0.3f);
    container->addChildLocked(bullet_emitter, -1);
    bullet_emitter->setPosition(-DP(25)*sGlobal->scaleX*cos(angle), -DP(25)*sGlobal->scaleX*sin(angle));
//    bullet_emitter->setDirectionAngleVariance(90+bullet->getRotation(), 0);
    bullet_emitter->setPositionType(GROUPED);
//    bullet_emitter->setStartSizeVariance(60, 0);
//    bullet_emitter->setLifeVariance(0.5f, 0);
//    bullet_emitter->setSpeedVariance(600, 0);
//    bullet_emitter->setEmissionRate(20);
    bullet_emitter->setDirectionAngleVariance(0, 0);
    bullet_emitter->setStartSizeVariance(120, 0);
    bullet_emitter->setLifeVariance(0.2f, 0);
    bullet_emitter->setSpeedVariance(800, 0);
    bullet_emitter->setRotation(bullet->getRotation()-180);
    
    elapsedTime = 0;
    isFading = false;
}
Missle::~Missle() {
    
}

void Missle::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    elapsedTime += dt;
    if (spxSprite->isVisible()) {
        spxSprite->tick(dt);
    }
    container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
    
    if (deadSprite->isVisible()) {
        deadSprite->tick(dt);
    }
}

void Missle::handleCollision(b2Body *actorB) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
    int tagB = userDataB->obj_type;
    switch (tagB) {
        case TYPE_GROUND: {
            deadSprite->setVisible(true);
            deadSprite->setPosition(container->getPositionX(), m_box2d->meter2Pixel(actorB->GetPosition().y)+DP(0)*sGlobal->scaleY);
//            spxSprite->setVisible(false);
            spxSprite->runAction(wyFadeOut::make(0.2f));
            body->SetLinearVelocity(b2Vec2(0,0));
            sGlobal->mainGameLayer->addToDisable(body);
            
            bullet_emitter->stopSystem();
            
            sGlobal->mainGameLayer->hero->isShaking = true;
            
            sGlobal->playEffect(se_bomb);
            break;
        }
        default:
            break;
    }
}

void Missle::destroyMyself() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    world->DestroyBody(body);
    
    spxSprite->stopAllActions();
    spxSprite->setAFCSpriteCallback(NULL, NULL);
    sGlobal->pushToCache(sGlobal->effects_cache, spxSprite);
    
    deadSprite->stopAllActions();
    deadSprite->setAFCSpriteCallback(NULL, NULL);
    sGlobal->pushToCache(sGlobal->effects_cache, deadSprite);
    
    if (bullet_emitter != NULL) {
        bullet_emitter->stopAllActions();
//        bullet_emitter->setVisible(false);
        bullet_emitter->stopSystem();
        bullet_emitter->resetSystem();
        sGlobal->pushToCache(sGlobal->fire_emitterArray, bullet_emitter);
    }
    
    container->stopAllActions();
    container->removeAllChildren(true);
    sGlobal->pushToCache(sGlobal->containerCache, container);
    
    delete this;
}



LiuDan::LiuDan(float baseX, float baseY, bool isMoto, int seq) {
    this->m_isMoto = isMoto;
    
    if (baseY < -900.0f) {
        baseY = sGlobal->mainGameLayer->hero->container->getPositionY();
    }
//    LOGE("baseX:%f, baseY:%f", baseX, baseY);
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
    sGlobal->mainGameLayer->addChildLocked(container);
    
//    float scale = (float)(rand()%20+60)/100.0f;
    wySPXSprite *bullet = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
    bullet->setForceTickMode(true);
    bullet->setUnitInterval(0.15f);
    bullet->setLoopCount(-1);
    bullet->setScaleX(sGlobal->scaleX*0.7f);
    bullet->setScaleY(sGlobal->scaleY*0.7f);
    bullet->playAnimation(SPX_HUOJIAN);
    container->addChildLocked(bullet);
    spxSprite = bullet;
    
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(0,0);
    bd.gravityScale = 0.2f;
    b2Body* bulletBody = m_box2d->getWorld()->CreateBody(&bd);
    bulletBody->SetTransform(bd.position, 0);
    b2PolygonShape box;
    box.SetAsBox(m_box2d->pixel2Meter(DP(150)*sGlobal->scaleX), m_box2d->pixel2Meter(DP(150)*sGlobal->scaleY));
    b2FixtureDef fd;
    fd.shape = &box;
    fd.restitution = 0.0f;
    fd.friction = 0.0f;
    fd.density = 0.0f;
    fd.isSensor = true;
    fd.filter.maskBits = MASK_BITS[TYPE_LIUDAN];
    fd.filter.categoryBits = CAT_BITS[TYPE_LIUDAN];
    fd.filter.groupIndex = GROUND_INDEX[TYPE_LIUDAN];
    
    bulletBody->CreateFixture(&fd);
    
    this->obj_type = TYPE_LIUDAN;
    this->body = bulletBody;
    bulletBody->SetUserData(this);
    float velx, vely;

    deadSprite = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
    sGlobal->mainGameLayer->addChildLocked(deadSprite, 102);
    deadSprite->setForceTickMode(true);
    deadSprite->setUnitInterval(0.05f);
    deadSprite->setLoopCount(0);
    deadSprite->setScaleX(3.0f*sGlobal->scaleX);
    deadSprite->setScaleY(3.0f*sGlobal->scaleY);
    deadSprite->playAnimation(SPX_HUOJIAN_BAOZHA);
    wyAFCSpriteCallback callback = {
        NULL,
        onMissleEnded
    };
    deadSprite->setVisible(false);
    deadSprite->setAFCSpriteCallback(&callback, this);
    deadSprite->setPosition(container->getPositionX(), container->getPositionY());
    
//    smokeSprite = wySPXSprite::make("testtest"), MyTextureManager::makePNG("testtest")), 2);
//    container->addChildLocked(smokeSprite, 102);
//    smokeSprite->setForceTickMode(true);
//    smokeSprite->setUnitInterval(0.12f);
//    smokeSprite->setLoopCount(-1);
//    smokeSprite->setScaleX(0.5f*sGlobal->scaleX);
//    smokeSprite->setScaleY(0.5f*sGlobal->scaleY);
//    smokeSprite->setPosition(-DP(100)*sGlobal->scaleX, 0);
//    smokeSprite->setFlipX(true);
    
    bullet_emitter = (wyParticleSystem*) sGlobal->popFromCache(sGlobal->smoke_emitterArray);
    bullet_emitter->setScaleX(sGlobal->scaleX*0.4f);
    bullet_emitter->setScaleY(sGlobal->scaleY*0.4f);
    container->addChildLocked(bullet_emitter, -1);
    bullet_emitter->setPositionType(GROUPED);
    bullet_emitter->setPosition(-DP(20)*sGlobal->scaleX, DP(4)*sGlobal->scaleX);
    bullet_emitter->setDirectionAngleVariance(180, 0);
    bullet_emitter->setStartSizeVariance(70, 0);
    bullet_emitter->setLifeVariance(0.8f, 0);
    bullet_emitter->setSpeedVariance(1000, 0);
    bullet_emitter->resetSystem();
    
    if (m_isMoto) {
        velx = HERO_VEL_X*4;
        vely = -7.0f;
        bulletBody->SetLinearVelocity(b2Vec2(velx, vely));
        bulletBody->SetGravityScale(0);
    } else {
        velx = 56.0f;
        vely = -20.0f;
        bulletBody->SetLinearVelocity(b2Vec2(velx, vely));
        bulletBody->SetGravityScale(0);
        
//        float degree = randRange(20, 40);
//        float speed = randRange(HERO_VEL_X*4, HERO_VEL_X*5);
//        bulletBody->SetLinearVelocity(b2Vec2(speed*cos(wyMath::d2r(degree)), -speed*sin(wyMath::d2r(degree))));
        
        bullet->setRotation(25);
        bullet_emitter->setRotation(25);
        int yy;
        switch (seq) {
            case 1:
                yy = DP(200)*sGlobal->scaleY;
                break;
            case 2:
                yy = DP(250)*sGlobal->scaleY;
                break;
            case 3:
                yy = DP(300)*sGlobal->scaleY;
                break;
            default:
                yy = DP(200)*sGlobal->scaleY;
                break;
        }
        bulletBody->SetTransform(b2Vec2(m_box2d->pixel2Meter(baseX-DP(200)*sGlobal->scaleX), m_box2d->pixel2Meter(baseY + yy)), 0);
        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
//        LOGE("start: container pos:%f,%f", container->getPositionX(), container->getPositionY());
        bullet_emitter->setPosition(-DP(20)*sGlobal->scaleX, DP(10)*sGlobal->scaleY);
    }
    
    elapsedTime = 0;
    isFading = false;
    
    isBoosted = false;
}
LiuDan::~LiuDan() {
    
}

void LiuDan::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    float distance = m_box2d->meter2Pixel(body->GetPosition().x - sGlobal->mainGameLayer->hero->body->GetPosition().x);
    
    if (m_isMoto) {
        if (distance > wyDevice::winWidth) {
            body->SetLinearVelocity(b2Vec2(0,0));
            sGlobal->mainGameLayer->addToDestroy(body);
        }
    } else {
        if (distance > DP(450)*sGlobal->scaleX && !deadSprite->isVisible()) {
            deadSprite->setVisible(true);
            deadSprite->setPosition(container->getPositionX(), container->getPositionY()+DP(20)*sGlobal->scaleY);
            spxSprite->runAction(wyFadeOut::make(0.4f));
            body->SetLinearVelocity(b2Vec2(0,0));
            
            bullet_emitter->stopSystem();
            
            sGlobal->mainGameLayer->hero->isStrongShaking = true;
            sGlobal->playEffect(se_bomb);
        }
        if (spxSprite->isVisible()) {
            spxSprite->tick(dt);
        }
        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
//        LOGE("container pos:%f,%f", container->getPositionX(), container->getPositionY());
        
        if (deadSprite->isVisible()) {
            deadSprite->tick(dt);
        }
    }
}

void LiuDan::handleCollision(b2Body *actorB) {
//    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    
//    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
//    int tagB = userDataB->obj_type;
//    switch (tagB) {
//        case TYPE_GROUND: {
//            deadSprite->setVisible(true);
//            deadSprite->setPosition(container->getPositionX(), m_box2d->meter2Pixel(actorB->GetPosition().y)-DP(60)*sGlobal->scaleY);
//            spxSprite->setVisible(false);
//            body->SetLinearVelocity(b2Vec2(0,0));
//            sGlobal->mainGameLayer->addToDisable(body);
//            break;
//        }
//        default:
//            break;
//    }
}

void LiuDan::destroyMe() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    if (m_isMoto) {
        sGlobal->mainGameLayer->hero->isMotoRunning = false;
        
        if (!sGlobal->mainGameLayer->hero->isMotoing) {
            sGlobal->mainGameLayer->hero->spx_moto_body->setVisible(false);
            sGlobal->mainGameLayer->hero->spx_moto_b_wheel->setVisible(false);
            sGlobal->mainGameLayer->hero->spx_moto_f_wheel->setVisible(false);
            
            if (sGlobal->mainGameLayer->hero->boost_emitter != NULL) {
                sGlobal->mainGameLayer->removeChildLocked(sGlobal->mainGameLayer->hero->boost_emitter, true);
            }
            sGlobal->mainGameLayer->hero->boost_emitter = NULL;
        }
    }

    world->DestroyBody(body);
    
    spxSprite->stopAllActions();
    spxSprite->setAFCSpriteCallback(NULL, NULL);
    sGlobal->pushToCache(sGlobal->effects_cache, spxSprite);
    
    deadSprite->stopAllActions();
    deadSprite->setAFCSpriteCallback(NULL, NULL);
    sGlobal->pushToCache(sGlobal->effects_cache, deadSprite);
    
    if (bullet_emitter != NULL) {
        bullet_emitter->setVisible(false);
        bullet_emitter->stopAllActions();
        bullet_emitter->stopSystem();
//        bullet_emitter->resetSystem();
        sGlobal->pushToCache(sGlobal->smoke_emitterArray, bullet_emitter);
    }
    
    container->stopAllActions();
    container->removeAllChildren(true);
    sGlobal->pushToCache(sGlobal->containerCache, container);
    
    delete this;
}

Robert::Robert() {
    robertStatus = R_APPEARING;
    robertTimer = 0;
    robertFireTimer = 0.5f;
    sGlobal->nearestDistance = 10000;
    sGlobal->nearestEnemy = NULL;

    outerContainer = wyNode::make();
    sGlobal->mainGameLayer->addChildLocked(outerContainer, 104);
    
    robertContainer = wyNode::make();
    robertContainer->setScale(0.5f);
//    robertContainer->setPosition(DP(-20)*sGlobal->scaleX, DP(30)*sGlobal->scaleY);
    outerContainer->addChildLocked(robertContainer);
    
    robertGun = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "robert_gun.png");
    robertGun->setPosition(DP(-30)*sGlobal->scaleX, DP(70)*sGlobal->scaleY);
    robertGun->setScaleX(0.5f*sGlobal->scaleX);
    robertGun->setScaleY(0.5f*sGlobal->scaleY);
    robertGun->setAnchor(0, 0.5f);
//    robertGun->setAlpha(200);
    robertContainer->addChildLocked(robertGun, 0);
    
    robert_0 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "robert_0.png");
    robert_0->setPosition(DP(-30)*sGlobal->scaleX, DP(70)*sGlobal->scaleY);
    robert_0->setScaleX(1.0f*sGlobal->scaleX);
    robert_0->setScaleY(1.0f*sGlobal->scaleY);
    robertContainer->addChildLocked(robert_0, 0);

    robert_1 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "robert_1.png");
    robert_1->setPosition(robert_0->getPositionX()+DP(0)*sGlobal->scaleX, robert_0->getPositionY()+DP(0)*sGlobal->scaleY);
    robert_1->setScaleX(1.0f*sGlobal->scaleX);
    robert_1->setScaleY(1.0f*sGlobal->scaleY);
    robert_1->setAnchor(0.5f, 0.5f);
    robertContainer->addChildLocked(robert_1, 0);
    
    robert_2 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "robert_2.png");
    robert_2->setPosition(robert_0->getPositionX()+DP(0)*sGlobal->scaleX, robert_0->getPositionY()+DP(0)*sGlobal->scaleY);
    robert_2->setScaleX(0.5f*sGlobal->scaleX);
    robert_2->setScaleY(0.5f*sGlobal->scaleY);
    robert_2->setAnchor(0.5f, 0.5f);
    robertContainer->addChildLocked(robert_2, 0);
    
    robert_3 = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "robert_3.png");
    robert_3->setPosition(robert_0->getPositionX()+DP(0)*sGlobal->scaleX, robert_0->getPositionY()-DP(1)*sGlobal->scaleY);
    robert_3->setScaleX(0.51f*sGlobal->scaleX);
    robert_3->setScaleY(0.51f*sGlobal->scaleY);
    robert_3->setAnchor(0.5f, 0.5f);
    robertContainer->addChildLocked(robert_3, 0);
    
//    robert_0->runAction(wyRepeatForever::make(wyRotateBy::make(4.0f, 360)));
//    robert_1->runAction(wyRepeatForever::make(wySequence::make(wyTintTo::make(0.01f, 255, 255, 255, 0, 192, 255), wyDelayTime::make(1.0f), wyTintTo::make(0.01f, 0, 192, 255, 0, 0, 0), wyDelayTime::make(1.0f),NULL)));
//    robert_1->runAction(wyRepeatForever::make(wyRotateBy::make(4.0f, 360)));
    
    robertContainer->runAction(wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.8f, 0, DP(4)), wyMoveBy::make(0.8f, 0, -DP(4)), NULL)));
    
    isWaitingFire = false;
    
    sGlobal->nearestEnemy = NULL;
    sGlobal->nearestDistance = 1000;
    
//    blink();
    robertContainer->runAction(wyBlink::make(0.5f, 3));
}

Robert::~Robert() {
    robertContainer->getParent()->removeChildLocked(robertContainer, true);
}

static void onRotateEnd(wyAction* action, void* data) {
    Robert* r = (Robert*)data;
//    sGlobal->mainGameLayer->hero->fireDart(-DP(24), DP(52), 1.0f, r->tempX, r->tempY, 0.9f);
//    HeroBullet::make(-DP(30)*sGlobal->scaleX, DP(45)*sGlobal->scaleY, WEAPON_7, r->tempX, r->tempY, 0.9f, 1.0f);
    
    float sx = r->robertGun->getScaleX();
    float sy = r->robertGun->getScaleY();
    r->robertGun->runAction(wySequence::make(wyScaleTo::make(0.05f, sx, sy, sx-0.1f, sy), wyScaleTo::make(0.05f, sx-0.1f, sy, sx, sy), NULL));
    
    float diffx = (r->robertGun->getPositionX()+r->robertGun->getWidth()*r->robertGun->getScaleX()*cosf(-wyMath::d2r(r->robertGun->getRotation())))*0.5f+r->robertContainer->getPositionX()+r->outerContainer->getPositionX()-sGlobal->mainGameLayer->hero->container->getPositionX()-sGlobal->mainGameLayer->hero->spx_hero_body->getPositionX();
    float diffy = (r->robertGun->getPositionY()+r->robertGun->getWidth()*r->robertGun->getScaleX()*sinf(-wyMath::d2r(r->robertGun->getRotation())))*0.5f+r->robertContainer->getPositionY()+r->outerContainer->getPositionY()-sGlobal->mainGameLayer->hero->container->getPositionY()-sGlobal->mainGameLayer->hero->spx_hero_body->getPositionY();
    
//    float diffx = r->robertGun->getPositionX()*0.5f+r->robertContainer->getPositionX()+r->outerContainer->getPositionX()-sGlobal->mainGameLayer->hero->container->getPositionX()-sGlobal->mainGameLayer->hero->spx_hero_body->getPositionX();
//    float diffy = r->robertGun->getPositionY()*0.5f+r->robertContainer->getPositionY()+r->outerContainer->getPositionY()-sGlobal->mainGameLayer->hero->container->getPositionY()-sGlobal->mainGameLayer->hero->spx_hero_body->getPositionY();
    
//    if (sGlobal->mainGameLayer->hero->isMotoing) {
//        diffx += DP(40)*sGlobal->scaleX;
//        diffy += DP(4)*sGlobal->scaleY;
//    }
    HeroBullet::make(WEAPON_ROBOT_BULLET, r->tempX, r->tempY, diffx, diffy);//
    
    r->isWaitingFire = false;
//    float diffX =  sGlobal->nearestEnemy->container->getPositionX()-r->robert_0->getPositionX()-sGlobal->mainGameLayer->hero->container->getPositionX();
//    float diffY =  sGlobal->nearestEnemy->container->getPositionY()-r->robert_0->getPositionY()-sGlobal->mainGameLayer->hero->container->getPositionY()-DP(20)*sGlobal->scaleY;
//    float angle = atan(diffY / diffX);
//    r->tempX = cosf(angle)*(HERO_VEL_X+2.0f);
//    r->tempY = sinf(angle)*(HERO_VEL_X+2.0f);
//    
//    HeroBullet::make(-DP(24)*sGlobal->scaleX, DP(52)*sGlobal->scaleY, WEAPON_7, r->tempX, r->tempY, 0.9f, 1.0f);
}

static void onRobotDisappearEnd(wyAction* action, void* data) {
    sGlobal->mainGameLayer->hero->robert = NULL;
    Robert *robert = (Robert *) data;
    delete robert;
    return;
}

void Robert::blink() {
    isBlinking = true;
    blinkTimer = 0;
    blinkTime = 0;
    isBlink = false;
    
    wyIntervalAction *a = wyBlink::make(1.0f, 5);
    wyActionCallback callback = {
        NULL,
        onRobotDisappearEnd,
        NULL
    };
    a->setCallback(&callback, this);
    robertContainer->runAction(a);
}

void Robert::tick(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    Hero *hero = sGlobal->mainGameLayer->hero;
    
    if (hero->isMotoing) {
        outerContainer->setPosition(hero->container->getPositionX()+DP(-60)*sGlobal->scaleX, hero->container->getPositionY()+DP(26)*sGlobal->scaleY);
    } else {
        outerContainer->setPosition(hero->container->getPositionX()+DP(-20)*sGlobal->scaleX, hero->container->getPositionY()+DP(30)*sGlobal->scaleY);
    }
    
    
    robertTimer += dt;
    if (robertTimer > sGlobal->SKILL_ROBOT_LAST && robertStatus != R_DISAPPEARING) {
        robertStatus = R_DISAPPEARING;
        blink();
    } else {
        if (robertStatus == R_DISAPPEARING) {
            return;
        }
        
        robertFireTimer += dt;
        if (robertFireTimer > 0.36f && sGlobal->nearestEnemy != NULL && !isWaitingFire) {
            robertFireTimer = 0;
            
//            float diffX = sGlobal->nearestEnemy->body->GetPosition().x-hero->body->GetPosition().x+m_box2d->pixel2Meter(DP(50)*sGlobal->scaleX);
//            float diffY =  sGlobal->nearestEnemy->body->GetPosition().y-hero->body->GetPosition().y-m_box2d->pixel2Meter(DP(100)*sGlobal->scaleY);
            float diffX = sGlobal->nearestEnemy->body->GetPosition().x-hero->body->GetPosition().x+m_box2d->pixel2Meter(DP(40)*sGlobal->scaleX);
            float diffY =  sGlobal->nearestEnemy->body->GetPosition().y-hero->body->GetPosition().y-m_box2d->pixel2Meter(DP(60)*sGlobal->scaleY);
            
//            float x0 = diffX * 15.0f / (12.0f + 15.0f);
            float angle = atanf(diffY / diffX);
            tempX = wyMath::cos(angle)*70.0f;
            tempY = wyMath::sin(angle)*70.0f;
//            LOGE("tempX:%f, tempY:%f, diffX:%f, diffY:%f", tempX, tempY, diffX, diffY);
            
//            HeroBullet::make(-DP(24)*sGlobal->scaleX, DP(52)*sGlobal->scaleY, WEAPON_7, tempX, tempY, 0.9f, 1.0f);
            
            robertGun->stopAllActions(); 
            float degree = -angle*57.3-robertGun->getRotation();
            wyIntervalAction *rotate = wyRotateBy::make(0.025f*fabs(degree/10), degree);
            rotate->setTag(100);
            wyActionCallback callback = {
                NULL,
                onRotateEnd,
                NULL
            };
            rotate->setCallback(&callback, this);
            robertGun->runAction(rotate);
//            robertGun->setRotation(-angle*57.3);
            
            isWaitingFire = false;
            
            wyIntervalAction *a1 = wyTintTo::make(0.01f, 255, 255, 255, 255, 0, 0);
            wyIntervalAction *a2 = wyTintTo::make(0.01f, 255, 0, 0, 255, 255, 255);
            robert_3->runAction(wySequence::make(a1, wyDelayTime::make(0.1f), a2, wyDelayTime::make(0.1f), a1->copy(), wyDelayTime::make(0.1f), a2->copy(), wyDelayTime::make(0.1f), a1->copy(), wyDelayTime::make(0.1f), a2->copy(), NULL));
        } else {
            if (robertGun->getAction(100) == NULL) {
                if (sGlobal->nearestEnemy == NULL) {
                    float angle = 0-robertGun->getRotation();
                    wyIntervalAction *rotate = wyRotateBy::make(0.05f*fabs(angle/10), angle);
                    rotate->setTag(100);
                    robertGun->runAction(rotate);
                } else {
//                    float diffX =  sGlobal->nearestEnemy->container->getPositionX()-robert_0->getPositionX()-hero->container->getPositionX();
//                    float diffY =  sGlobal->nearestEnemy->container->getPositionY()-robert_0->getPositionY()-hero->container->getPositionY()+DP(40)*sGlobal->scaleY;
//                    float degree = -atan(diffY / diffX) * 57.3f - robertGun->getRotation();
//                    
//                    wyIntervalAction *rotate = wyRotateBy::make(0.025f*fabs(degree/10), degree);
//                    rotate->setTag(100);
//                    robertGun->runAction(rotate);
                }
            }
        }
    }
}

void Hero::createBodySensor() {
//    if (bodySensor != NULL) {
//        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//        b2World* world = m_box2d->getWorld();
//        
//        world->DestroyBody(bodySensor->body);
//        delete bodySensor;
//        bodySensor = NULL;
//    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    if (bodySensor == NULL) {
        bodySensor = BodySensor::make(body->GetPosition().x-m_box2d->pixel2Meter(DP(0)*sGlobal->scaleX), body->GetPosition().y-m_box2d->pixel2Meter(DP(45)*sGlobal->scaleX));
    }
    isBodySensorCreated = true;
}

void Hero::createHeroHurtSensor() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    hurtSensor = HeroHurtSensor::make(body->GetPosition().x, body->GetPosition().y);
    isHeroHurtSensorCreated = true;
}

BodySensor::BodySensor(float x, float y) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    obj_type = TYPE_BODY_SENSOR;
    
    float boxWidth = DP(10)*sGlobal->scaleX;
    float boxHeight = DP(110)*sGlobal->scaleY;
    b2BodyDef bd;
    bd.position.Set(x, y);
    bd.userData = this;
    bd.type = b2_dynamicBody;
    
    b2PolygonShape staticBox;
    staticBox.SetAsBox(m_box2d->pixel2Meter(boxWidth), m_box2d->pixel2Meter(boxHeight), b2Vec2(0, -m_box2d->pixel2Meter(boxHeight)), 0);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = MASK_BITS[TYPE_BODY_SENSOR];
    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_BODY_SENSOR];
    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_BODY_SENSOR];
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0);
    
    halfBodyHeightInMeter = m_box2d->pixel2Meter(boxHeight);
}

BodySensor::~BodySensor() {
    
}

void BodySensor::handleCollision(b2Body *actorB) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
    int tagB = userDataB->obj_type;
    switch (tagB) {
        case TYPE_GROUND: {
//            LOGE("vely: %f", body->GetLinearVelocity().y);
//            if (sGlobal->mainGameLayer->hero->body->GetLinearVelocity().y < 0) {
//                LOGE("rot 2:%f", sGlobal->mainGameLayer->hero->spx_hero_body->getRotation());
                sGlobal->mainGameLayer->addToDestroy(body);
//            }
            break;
        }
        default:
            break;
    }
}


HeroHurtSensor::HeroHurtSensor(float x, float y) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    obj_type = TYPE_HERO_HURT;
    
    float boxWidth = DP(10)*sGlobal->scaleX;
    float boxHeight = DP(50)*sGlobal->scaleY;
    b2BodyDef bd;
    bd.position.Set(x, y);
    bd.userData = this;
    bd.type = b2_dynamicBody;
    
    b2PolygonShape staticBox;
    staticBox.SetAsBox(m_box2d->pixel2Meter(boxWidth), m_box2d->pixel2Meter(boxHeight));
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = MASK_BITS[TYPE_HERO_HURT];
    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_HERO_HURT];
    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_HERO_HURT];
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0);
    
    halfBodyHeightInMeter = m_box2d->pixel2Meter(boxHeight);
}

HeroHurtSensor::~HeroHurtSensor() {
    
}

void HeroHurtSensor::handleCollision(b2Body *actorB) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    Hero *hero = sGlobal->mainGameLayer->hero;
    
    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
    int tagB = userDataB->obj_type;
    switch (tagB) {
        case TYPE_SHOCK_WAVE:
        case TYPE_ENEMY_BULLET: {
            EnemyBulletBase *enemyBullet = (EnemyBulletBase *)userDataB;
            enemyBullet->onHitHero();
            
            //            sGlobal->mainGameLayer->addToDestroy(actorB);
            if (hero->isBoosting || hero->isDeBoosting || hero->isSandShieldActive) {
                
            } else {
                hero->hurt(userDataB->atk, enemyBullet->b_id != 0);
            }
            //            userDataB->onHitHero();
            break;
        }
        case TYPE_CHONGZHUANG: {
            if (hero->isBoosting || hero->isDeBoosting || hero->isSandShieldActive) {
                
            } else {
                hero->hurt(userDataB->atk);
            }
            break;
        }
        case TYPE_WATER_BALL:
        case TYPE_LIGHTING: {
            if (hero->isBoosting || hero->isDeBoosting || hero->isSandShieldActive) {
                
            } else {
                hero->hurt(userDataB->atk, true);
            }
            break;
        }
        case TYPE_ENEMY: {
            Enemy *enemy = (Enemy *)userDataB;
            if (enemy->isDead) {
                return;
            }
            if (hero->isBoosting || hero->isDeBoosting || hero->isSandShieldActive) {

            } else {
                if (!enemy->isStunning) {
                    if (enemy->e_id == ENEMY_XUANKONGBIAO || enemy->e_id == ENEMY_BOSS1 || enemy->e_id == ENEMY_BOSS2 || enemy->e_id == ENEMY_BOSS3 || enemy->e_id == ENEMY_NEED_BLOOD) {
                        hero->hurt(userDataB->atk, true);
                    } else {
                        hero->hurt(userDataB->atk, rand()%3==0);
                    }
                }
            }
            userDataB->onHitHero();
            break;
        }
        case TYPE_TRAP: {
            if (hero->isBoosting || hero->isDeBoosting || hero->isSandShieldActive) {
                
            } else {
                hero->hurt(userDataB->atk);
            }
            userDataB->onHitHero();
            break;
        }
        case TYPE_BOMB: {
            if (hero->isBoosting || hero->isDeBoosting || hero->isSandShieldActive) {
                
            } else {
                hero->hurt(userDataB->atk);
            }
            userDataB->onHitHero();
            break;
        }
        default:
            break;
    }
}


HeroIllusion::HeroIllusion() {
    body = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), 0);
    body1 = wySPXSprite::make("hero1.sprite", false, MyTextureManager::makePNG("hero1.png"), 0);
    l_leg = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), 0);
    r_leg = wySPXSprite::make("hero.sprite", false, MyTextureManager::makePNG("hero.png"), 0);
    addChildLocked(body);
    addChildLocked(body1);
    addChildLocked(l_leg);
    addChildLocked(r_leg);
    
    body->setColor(wyc3b(0, 100, 255));
    body1->setColor(wyc3b(0, 100, 255));
    l_leg->setColor(wyc3b(0, 100, 255));
    r_leg->setColor(wyc3b(0, 100, 255));
    
    body->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    body1->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    l_leg->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    r_leg->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
}

HeroIllusion::~HeroIllusion() {
    
}

void HeroIllusion::syncAnimation(int animIdx1, int animIdx2, int animIdx3, int texId) {
    if (texId == 0) {
        body->setVisible(true);
        body1->setVisible(false);
        
        body->playAnimation(animIdx1);
    } else {
        body->setVisible(false);
        body1->setVisible(true);
        
        body1->playAnimation(animIdx1);
    }
    
    l_leg->playAnimation(animIdx2);
    r_leg->playAnimation(animIdx3);
}




GroundSensor::GroundSensor() {
    Hero *hero = sGlobal->mainGameLayer->hero;
    float x = hero->body->GetPosition().x;
    float y = hero->body->GetPosition().y;
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    obj_type = TYPE_GROUND_SENSOR;
    
    float boxWidth = DP(10)*sGlobal->scaleX;
    float boxHeight = DP(1000)*sGlobal->scaleY;
    b2BodyDef bd;
    bd.position.Set(x, y);
    bd.userData = this;
    bd.type = b2_dynamicBody;
    
    b2PolygonShape staticBox;
    staticBox.SetAsBox(m_box2d->pixel2Meter(boxWidth), m_box2d->pixel2Meter(boxHeight));
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = MASK_BITS[TYPE_GROUND_SENSOR];
    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_GROUND_SENSOR];
    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_GROUND_SENSOR];
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0);
    
    halfBodyHeightInMeter = m_box2d->pixel2Meter(boxHeight);
    
    groundArray = wyArrayNew(100);
    isContacted = false;
    
    isDestroied = false;
    deltaTime = 0;
}

GroundSensor::~GroundSensor() {
    
}

void GroundSensor::update(float dt) {
    if (isContacted && !isDestroied) {
        Hero *hero = sGlobal->mainGameLayer->hero;
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        for (int i = 0; i < groundArray->num; i++) {
            BaseObject *ground = (BaseObject*)wyArrayGet(groundArray, i);
            if (ground->body->GetPosition().y < hero->body->GetPosition().y - hero->halfBodyHeightInMeter*1.5f) {
                hero->body->SetTransform(b2Vec2(hero->body->GetPosition().x, hero->body->GetPosition().y-m_box2d->pixel2Meter(DP(4)*sGlobal->scaleY)), 0);
                sGlobal->isOnGround = false;
                break;
            }
        }
        
        isDestroied = true;
        sGlobal->mainGameLayer->addToDestroy(body);
    }
    
    deltaTime += dt;
    if (!isDestroied && deltaTime > 1.0f) {
        isDestroied = true;
        sGlobal->mainGameLayer->addToDestroy(body);
    }
}

void GroundSensor::handleCollision(b2Body *actorB) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
    int tagB = userDataB->obj_type;
    switch (tagB) {
        case TYPE_GROUND: {
            wyArrayPush(groundArray, userDataB);
            
//            LOGE("ground:%f", userDataB->body->GetPosition().y);
            isContacted = true;
            break;
        }
        default:
            break;
    }
}


//
//  MyContactListener.h
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//
#pragma once

#include "WiEngine.h"
#include "Enemy.h"
#include "HeroBullet.h"
#include "FlyZombie1.h"

const static int BOSS1_IDLE = 0;
const static int BOSS1_WALK = 100;
const static int BOSS1_ATK_RENG_DAO = 2;//扔菜刀
const static int BOSS1_ATK_KAN_0 = 30;//停住
const static int BOSS1_ATK_KAN_1 = 31;//砍
const static int BOSS1_ATK_KAN_2 = 32;//回去
const static int BOSS1_ATK_XULI_LIANDAO_0 = 40;//蓄力
const static int BOSS1_ATK_XULI_LIANDAO_1 = 41;//蓄力
const static int BOSS1_ATK_HUIXUAN_CAIDAO_0 = 50;//回旋菜刀
const static int BOSS1_ATK_HUIXUAN_CAIDAO_1 = 51;



const static int BOSS_FAKE_DEAD = -1;
const static int BOSS_HURT = -2;

const static int BOSS1_ANIM_DAIJI = 0;
const static int BOSS1_ANIM_WALK = 1;
const static int BOSS1_ANIM_TIAOQI = 1;
const static int BOSS1_ANIM_KAN = 2;
const static int BOSS1_ANIM_RENGLIANDAO = 3;
const static int BOSS1_ANIM_RENGCAIDAO = 4;
const static int BOSS1_ANIM_CAIDAO = 5;
const static int BOSS1_ANIM_ZHAOHUANXIAOBING = 6;
const static int BOSS1_ANIM_SHOUSHANG = 7;
const static int BOSS1_ANIM_SIWANG = 8;

const static int BOSS1_ANIM_XULI_LIANDAO = 9;
const static int BOSS1_ANIM_RENG_SUCCESS = 10;
const static int BOSS1_ANIM_RENG_FAILED = 11;
const static int BOSS1_ANIM_RENG_KANDAO = 12;
const static int BOSS1_ANIM_RENG_HUIXUAN_DAO = 13;
const static int BOSS1_ANIM_KANDAO = 14;

//effects.spx
const static int BOSS1_ANIM_LIANDAO = 16;
const static int BOSS1_ANIM_SUOLIAN = 17;

class Boss : public Enemy {
public:
    float baseY;
    bool isHurtByLiuDan;
    float hurtByLiuDanTimer;
    bool isHurtByMissle;
    float hurtByMissleTimer;
    float multiY;
    float stayX, stayY;
    float shadowX, shadowY, shadowScaleX;
    int status_idle;
//    float bloodX, bloodY;
    
    Boss(float x, float y) : Enemy(x, y) {
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(false);
        sGlobal->mainGameLayer->addChildLocked(container, 3);
        
        bossBlood = wyProgressTimer::make(wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "progressbar_upper.png"));
        bossBlood->setStyle(HORIZONTAL_BAR_LR);
        bossBlood->setAnchor(0.5f, 0);
//        bossBlood->setPosition(DP(20)*sGlobal->scaleX, DP(100)*sGlobal->scaleY);
        bossBlood->setScaleX(sGlobal->scaleX*0.7f);
        bossBlood->setScaleY(sGlobal->scaleY*0.5f);
        bossBlood->setPercentage(100);
        container->addChildLocked(bossBlood, 200);
        
        isInitToShow = false;
        
        if (sGlobal->isRealEndless) {
            baseY = container->getPositionY();
            if (sGlobal->isBoostOn) {
                multiY = sGlobal->boostMulti;
            } else {
                multiY = 1.0f;
            }
        } else {
            multiY = 1.0f;
        }
        
        isHurtByLiuDan = false;
        hurtByLiuDanTimer = 0;
        isHurtByMissle = false;
        hurtByMissleTimer = 0;
        sGlobal->bossEyeZ = 1;
        
        needDeadBlink = false;
        needStay = true;
        
        bloodArray = wyArrayNew(10);
        
//        isStunning = false;
//        faintSprite = NULL;
        isHeightInited = true;
        stuckTimer = 0;
    }
    
    ~Boss() {
        wyArrayDestroy(bloodArray);
    }

    void beginContact(b2Body *actorB) {
        if (sGlobal->isRealEndless) {
            return;
        }
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
        int tagB = userDataB->obj_type;
        if (tagB == TYPE_GROUND) {
//            isOnGround = true;
            
//            shadow->setVisible(true);
            sGlobal->mainGameLayer->hero->isShaking = true;
            
            if (!isHeightInited) {
//                halfBodyHeightInMeter = body->GetPosition().y - actorB->GetPosition().y;
                isHeightInited = true;
            }
            
            baseY = container->getPositionY();
            
            if (!sGlobal->isRealEndless) {
                body->SetGravityScale(1.0f);
            }
        }
    }
    
    void endContact(b2Body *actorB) {
//        if (sGlobal->isRealEndless) {
//            return;
//        }
//
//        BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
//        int tagB = userDataB->obj_type;
//        if (tagB == TYPE_GROUND) {
////            isOnGround = false;
////            shadow->setVisible(false);
//        }
    }
    
    static void onBulletFadeEnd(wyAction* action, void* data) {
        HeroBullet* bullet = (HeroBullet*)data;
        sGlobal->mainGameLayer->addToDestroy(bullet->body);
    }

    void dealHitByMissle(float y, b2Body *bulletBody) {
        if (isHurtByMissle) {
            return;
        }
        
        isHurtByMissle = true;
        hurtByMissleTimer = 0;

        hp -= hp/10;
        if (hp <= 0) {
            hp = 1;
        }
        wyAction *action = spxSprite->getAction(100);
        if (action == NULL || action->isDone()) {
            wyIntervalAction* a = wyFadeTo::make(0.2f, 255, 100);
            wyIntervalAction* r = wySequence::make(a,
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   a->copy(),
                                                   (wyFiniteTimeAction*)a->reverse(),
                                                   NULL);
            r->setTag(100);
            spxSprite->runAction(r);
        }
        
        setBossHPBar((float)hp/(float)maxHp);
    }

    void dealHitByDart(float y, b2Body *bulletBody) {
        HeroBullet *bullet = (HeroBullet *)bulletBody->GetUserData();
        if (bullet->isDisabled) {
            return;
        }

        //n％几率眩晕0.5s,必须处于idle状态中
        if (status == status_idle && sGlobal->hasSkillMulti[SKILL_STUN] && bullet->bulletType != WEAPON_ROBOT_BULLET) {
//            LOGE("stun chance:%f", sGlobal->skillMulti[SKILL_STUN]*100);
            int seed = rand()%100;
            if (seed < sGlobal->skillMulti[SKILL_STUN]*100) {
                applyStun();
            }
        }
//        if (status == status_idle) {
//            applyStun();
//        }
        
        if (bullet->bulletType != WEAPON_GLB && bullet->bulletType != WEAPON_GLR && bullet->bulletType != WEAPON_AER14
            && bullet->bulletType != WEAPON_RPG && bullet->bulletType != WEAPON_BLASTING && bullet->bulletType != WEAPON_RPG_BOMB && bullet->bulletType != WEAPON_BLASTING_BOMB) {
            
            wyAction *a = wyFadeOut::make(0.1f);
            wyActionCallback callback = {
                NULL,
                onBulletFadeEnd,
                NULL
            };
            a->setCallback(&callback, bullet);
            bullet->container->runAction(a);
            bullet->container->runAction(wyScaleBy::make(0.1f, -bullet->container->getScaleX()*0.8f, 0));
            sGlobal->mainGameLayer->addToDisable(bullet->body);
        }
        int damage = sGlobal->getWeaponAtk();
        
        //击退
        if (bullet->bulletType == WEAPON_M870P || bullet->bulletType == WEAPON_HYDRA) {
            if (e_id != ENEMY_BOSS1 && e_id != ENEMY_BOSS2 && e_id != ENEMY_BOSS3) {
                knockBack();
            }
        }
        //穿透
        if (bullet->bulletType == WEAPON_AER14 || bullet->bulletType == WEAPON_GLB || bullet->bulletType == WEAPON_GLR) {
            addBulletMask(bullet->catBit);
        }
        //火箭
        if (bullet->bulletType == WEAPON_RPG || bullet->bulletType == WEAPON_BLASTING) {
            bullet->makeBomb(bullet->bulletType);
            return;
        }
        //机器人子弹
        if (bullet->bulletType == WEAPON_ROBOT_BULLET) {
            damage = sGlobal->skillMulti[SKILL_ROBOT];
        }
        //火箭爆炸
        if (bullet->bulletType == WEAPON_RPG_BOMB || bullet->bulletType == WEAPON_BLASTING_BOMB) {
            addBulletMask(bullet->catBit);
        }
        hurt(y);
        
        damage *= bullet->atkPercent;
        
        //n％几率2倍暴击
        if (sGlobal->hasSkillMulti[SKILL_CRITICAL] && bullet->bulletType != WEAPON_ROBOT_BULLET) {
            int seed = rand()%100;
            if (seed < sGlobal->skillMulti[SKILL_CRITICAL]*100) {
                //1.5~2
                float beishu = (int)randRange(18, 23)/10.0f;
                damage *= beishu;
                showCriticalEffect(damage);
            }
        }
        
        //    //5％几率吸血，血量为攻击的n％
        //    if (sGlobal->hasSkillMulti[SKILL_6]) {
        //        int seed = rand()%100;
        //        if (seed < ELEMENT_SKILL_6_PARAM1) {
        //            int recoverHP = MIN(sGlobal->cloth_hp-sGlobal->curHP, ceil(damage*sGlobal->skillMulti[SKILL_6]));;
        //            sGlobal->hudLayer->setHPBar(-recoverHP);
        ////            LOGE("leech HP: %d", recoverHP);
        //
        //            sGlobal->mainGameLayer->hero->showRecoverHPEffect(recoverHP, false);
        //        }
        //    }
        
        if (hp - damage <= 0) {
            //如果是walk状态才会死
            if (status == 100 || status == BOSS_HURT) {
                hp -= damage;
                fakeDead();
            } else {
                hp = 1;
            }
        } else {
            hp -= damage;
        }

        float percent = (float)hp/(float)maxHp;
        setBossHPBar(percent);
        
        if (bullet->bulletType == WEAPON_RPG_BOMB || bullet->bulletType == WEAPON_BLASTING_BOMB) {
            sGlobal->playEffect(se_bomb);
        } else {
            sGlobal->playEffect(se_hit);
        }
    }
    
    virtual void fakeDead() {
        
    }
    
    void dealHitByLiudan(b2Body *bulletBody) {
        if (isHurtByLiuDan) {
            return;
        }

        isHurtByLiuDan = true;
        hurtByLiuDanTimer = 0;

        int damage = sGlobal->skillMulti[SKILL_MISSLE]*2;
        if (hp - damage <= 0) {
            hp = 1;
        } else {
            hp -= damage;
        }
//        wyAction *action = spxSprite->getAction(100);
//        if (action == NULL || action->isDone()) {
//            wyIntervalAction* a = wyFadeTo::make(0.2f, 255, 100);
//            wyIntervalAction* r = wySequence::make(a,
//                                                   (wyFiniteTimeAction*)a->reverse(),
//                                                   a->copy(),
//                                                   (wyFiniteTimeAction*)a->reverse(),
//                                                   NULL);
//            r->setTag(100);
//            spxSprite->runAction(r);
//        }

//        wyAction *action = spxSprite->getAction(101);
//        if (action == NULL || action->isDone()) {
            spxSprite->setColor(wyc3b(255, 0, 0));
            wyIntervalAction *delay = wyDelayTime::make(0.2f);
            wyActionCallback callback = {
                NULL,
                onHurtDelayEnded,
                NULL
            };
            delay->setCallback(&callback, spxSprite);
            delay->setTag(101);
            spxSprite->runAction(delay);
//        }
        
        setBossHPBar((float)hp/(float)maxHp);
    }
    
    void dead() {
        if (isDead)
            return;
        
        sGlobal->level_kills += 1;
        sGlobal->totalKilledEnemies += 1;
        sGlobal->level_bosses += 1;
        sGlobal->killedBosses += 1;

        int extraGold = ceil(this->gold * sGlobal->skillMulti[SKILL_GREED]);
        sGlobal->extraGold += extraGold;
        LOGE("extraGold:%d", extraGold);
        int finalPlusGold = this->gold + extraGold;
        switch (e_id) {
            case ENEMY_BOSS1:
                if (sGlobal->isBoss1Strong) {
                    finalPlusGold = MAX(finalPlusGold-40, 0);
                } else {
                    finalPlusGold = MAX(finalPlusGold-180, 0);
                }
                break;
            case ENEMY_BOSS2:
                finalPlusGold = MAX(finalPlusGold-280, 0);
                break;
            case ENEMY_BOSS3:
                finalPlusGold = MAX(finalPlusGold-300, 0);
                break;
            default:
                break;
        }
        sGlobal->hudLayer->addGold(finalPlusGold);
        sGlobal->level_enemy_gold += finalPlusGold;
        sGlobal->level_total_gold += finalPlusGold;
        LOGE("finalPlusGold:%d", finalPlusGold);
        
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
        isDead = true;
        
        if (!isKnockingBack) {
            body->SetLinearVelocity(b2Vec2(0,0));
        }
        body->SetGravityScale(0);

        spxSprite->setUnitInterval(0.2f);
        spxSprite->setLoopCount(0);
        spxSprite->playAnimation(animDead);

        sGlobal->mainGameLayer->addToDisable(body);
        
        if (sGlobal->isRealEndless) {
            for (int i = 0; i < 5; i++) {
                int achvNo = ACHV_BOSS_3+i;
                if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
//                    if (achvNo == ACHV_BOSS_100 && sGlobal->currentEModeDifficulty == DIFFICULTY_NORMAL) {
//                        continue;
//                    }
//                    if (achvNo == ACHV_BOSS_300 && sGlobal->currentEModeDifficulty != DIFFICULTY_HARD) {
//                        continue;
//                    }
                    sGlobal->achv_progress[achvNo]+=1;
                    if (sGlobal->achv_progress[achvNo] >= sGlobal->ACHV_NUM[achvNo*3]) {
                        sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                        sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                        sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
                    }
                }
            }
            
            for (int i = 0; i < 5; i++) {
                int achvNo = ACHV_BOSS_2_ONE+i;
                if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
//                    if (achvNo == ACHV_BOSS_16_ONE && sGlobal->currentEModeDifficulty == DIFFICULTY_NORMAL) {
//                        continue;
//                    }
//                    if (achvNo == ACHV_BOSS_24_ONE && sGlobal->currentEModeDifficulty != DIFFICULTY_HARD) {
//                        continue;
//                    }
                    sGlobal->achv_progress_temp[achvNo]+=1;
                    if (sGlobal->achv_progress_temp[achvNo] >= sGlobal->ACHV_NUM[achvNo*3]) {
                        sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                        sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                        sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
                    }
                }
            }
        }
        
        sGlobal->isBossFighted = true;
        sGlobal->isInBossBattle = false;
    }
    
    void destroyMyself() {
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        spxSprite->stopAllActions();
        spxSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(spxCacheArray, spxSprite);

        if (shadow != NULL) {
            sGlobal->pushToCache(sGlobal->shadowSpriteCache, shadow);
        }
        
        if (faintSprite != NULL) {
            faintSprite->stopAllActions();
            faintSprite->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, faintSprite);
        }
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
        
//        wyNode *parent = container->getParent();
//        parent->removeChildLocked(container, true);
        
//        if (bloodSprite != NULL) {
//            bloodSprite->setFlipX(false);
//            bloodSprite->stopAllActions();
//            bloodSprite->setAFCSpriteCallback(NULL, NULL);
//            sGlobal->pushToCache(sGlobal->effects_cache, bloodSprite);
//        }
        
        delete this;
    }
    
    void setBossHPBar(float percent) {
        bossBlood->setPercentage(percent*100);
    }
    
    void hurt(float y) {
//        LOGE("boss hp:%d", hp);
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

//        bloodSprite->setVisible(true);
//        int seed = rand()%2;
//        bloodSprite->playAnimation(SPX_PENXUE_1+seed);
//        bloodSprite->setPosition(container->getPositionX(), m_box2d->meter2Pixel(y));
        penxue(y);
    }
    
    void update(float dt) {
        if (!isInitToShow) {
            isInitToShow = true;
            initToShow();
            return;
        }
        
        if (sGlobal->mainGameLayer->hero->robertStatus == R_APPEARING && !isDead && isAppeared) {
            float dist = body->GetPosition().x - sGlobal->mainGameLayer->hero->body->GetPosition().x;
            if (dist <= -DP(50)*sGlobal->scaleX) {
                sGlobal->nearestEnemy = NULL;
                sGlobal->nearestDistance = 1000;
            } else {
                if (dist < sGlobal->nearestDistance && dist > 0) {
                    sGlobal->nearestEnemy = this;
                    sGlobal->nearestDistance = dist;
                }
            }
        }
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        for (int i = 0; i < bloodArray->num; i++) {
            wySPXSprite *blood = (wySPXSprite*)wyArrayGet(bloodArray, i);
            blood->tick(dt);
        }
        
        if (isDead) {
            spxSprite->tick(dt);
            
            if (sGlobal->isRealEndless) {
                body->SetLinearVelocity(b2Vec2(sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x/3*2, 0));
                container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
            } else {
                body->SetLinearVelocity(b2Vec2(0, 0));
            }
            return;
        }
        
        distance = m_box2d->meter2Pixel(body->GetPosition().x)+offsetX-m_box2d->meter2Pixel(sGlobal->mainGameLayer->hero->body->GetPosition().x);
        
        float prevY = container->getPositionY();
        if (distance < appearDistance) {
            if (!container->isVisible() && status != BOSS_FAKE_DEAD) {
                container->setVisible(true);
                sGlobal->mainGameLayer->addToEnable(body);
                isAppeared = true;
            }
            spxSprite->tick(dt);
            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
        }
        
        if (isHurtByLiuDan) {
            hurtByLiuDanTimer+=dt;
            if (hurtByLiuDanTimer > 5.0f) {
                isHurtByLiuDan = false;
                hurtByLiuDanTimer = 0;
            }
        }
        
        if (isHurtByMissle) {
            hurtByMissleTimer+=dt;
            if (hurtByMissleTimer > 5.0f) {
                isHurtByMissle = false;
                hurtByMissleTimer = 0;
            }
        }
        
        if (isScheduled && !isStunning) {
            elapsedAtkTime+=dt;
            if (elapsedAtkTime>=nextTime) {
                doAttack();
            }
        }
        
        if (isStunning) {
            stunTimer += dt;
            if (stunTimer > sGlobal->SKILL_STUN_DUR*sGlobal->BOSS_STUN_TIME_MULTI) {
                isStunning = false;
                stunTimer = 0;
            } else {
                faintSprite->tick(dt);
            }
        }
        
        stuckTimer += dt;
        if (!isDead && stuckTimer > 10.0f) {
            LOGE("stucked, scheduleNextAttack..., unitinterval:%f, loopcount:%d", spxSprite->getUnitInterval(), spxSprite->getLoopCount());
            scheduleNextAttack();
        }
        
        for (int i = 0; i < removeMaskBitArray->num; i++) {
            RemoveMaskBitTask *task = (RemoveMaskBitTask*)wyArrayGet(removeMaskBitArray, i);
            bool isFinished = task->updateTask(dt);
            if (isFinished) {
//                LOGE("removeMaskBitArray:%d", removeMaskBitArray->num);
                clearBulletMask(task->m_maskBit);
                wyArrayDeleteObj(removeMaskBitArray, task, NULL, NULL);
                task->release();
            }
        }
    }

    void scheduleNextAttack() {
        isScheduled = true;
        elapsedAtkTime = 0;
        nextTime = randRange(0, 2) + 0.3f;
        if (sGlobal->isRealEndless) {
            nextTime *= sGlobal->BOSS_ATK_INTERVAL_MULTI;
        }
        
        stuckTimer = 0;
//        LOGE("next atk time:%f", nextTime);
    }
    
    void setCurVel(float velX, float velY) {
        relativeVelX = velX - sGlobal->heroVelocityX;
        body->SetLinearVelocity(b2Vec2(sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x + relativeVelX, velY));
    }
    
    void initToShow() {        
        shadow = (wySpriteEx*) sGlobal->popFromCache(sGlobal->shadowSpriteCache);
        shadow->setTextureRect(wyZwoptexManager::getInstance()->getFrameRect("dialog_deco", "shadow.png"));
        shadow->setScaleX(sGlobal->scaleX*shadowScaleX);
        shadow->setScaleY(sGlobal->scaleY*1.8f);
        container->addChildLocked(shadow, -1);
        shadow->setVisible(true);
        shadow->setAlpha(100);
        shadow->setPosition(shadowX, shadowY);
        shadow->runAction(wyRepeatForever::make(wySequence::make(wyScaleTo::make(1.0f, sGlobal->scaleX*shadowScaleX, sGlobal->scaleY*1.8f, sGlobal->scaleX*shadowScaleX+0.4f, sGlobal->scaleY*1.8f + 0.1f), wyScaleTo::make(1.0f, sGlobal->scaleX*shadowScaleX+0.4f, sGlobal->scaleY*1.8f+0.1f, sGlobal->scaleX*shadowScaleX, sGlobal->scaleY*1.8f), NULL)));
        
        if (sGlobal->isRealEndless) {
            shadow->setVisible(false);
        }
    }
    
    virtual void doAttack() {
        
    }
    
    wyProgressTimer *bossBlood;
    float stayDist;
    float elapsedAtkTime, nextTime;
    bool isScheduled;
    float relativeVelX;
    int status;
    
    float stuckTimer;
    
    virtual void onTargetShootEnd(int index) {
        
    }
    
    static void onBossPenxueEnded(wyAFCSprite* sprite, void* data) {
        Boss *boss = ((Boss *) data);
        if (sprite != NULL) {
            sprite->setFlipX(false);
            sprite->stopAllActions();
            sprite->setAFCSpriteCallback(NULL, NULL);
            wyArrayDeleteObj(boss->bloodArray, sprite, NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, sprite);
        }
    }
    
    wyArray *bloodArray;
    void penxue(float y) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//        LOGE("y:%f", y);
        
        wySPXSprite *blood = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        container->addChildLocked(blood, 103);
        blood->setForceTickMode(true);
        blood->setUnitInterval(0.1f);
        blood->setLoopCount(0);
        blood->setFlipX(true);
        blood->setScaleX(2.0f*sGlobal->scaleX);
        blood->setScaleY(2.5f*sGlobal->scaleX);
        blood->setVisible(true);
        wyAFCSpriteCallback callback1 = {
            NULL,
            onBossPenxueEnded
        };
        blood->setAFCSpriteCallback(&callback1, this);
        blood->playAnimation(rand()%2+SPX_PENXUE_1);
        int fuhao = rand()%2 == 0 ? 1 : -1;
        blood->setPosition(fuhao*DP(rand()%20)*sGlobal->scaleX, m_box2d->meter2Pixel(y)-container->getPositionY()-offsetY-DP(40)*sGlobal->scaleY);
        
        int seed = rand()%2;
        int rotation = randRange(0,20);
        if (rand()%4 == 0) {
            rotation*=-1;
        }
        blood->setRotation(rotation);
        
        wyArrayPush(bloodArray, blood);
    }
};

class ShootTarget;
class GouziObject;

class Boss1 : public Boss {
public:
    Boss1(float x, float y);
    ~Boss1();
    
    void update(float dt);
    
    static Boss1 *make(float x, float y) {
        return WYNEW Boss1(x, y);
    }
    
    void doAttack();
    
    void doAttackById(int idx);
    void doAttack_Rengdao();
    void doAttack_RengHuiXuanDao();
    void doAttack_XuliSuolian();
    void doAttack_Kan();
    
    void fakeDead();
    
    void throwCoin();
    
    static Boss1 *instance;
    
    bool isDialogShowing;
    void hideDialogBox();
    float dialogTimer;
    void showDialogBox(int idx);
    wyNode *dialogBox;
    wyBitmapFontLabel *content;
    
    wySPXSprite *deadSprite1, *deadSprite2;
    void shootCaiDao(int no);
    
    int atk1;
    void initShootTargets();
    void generateNextTarget();
    void attackSuccess();
    void attackFailed();
    float time2, time3, totalTime, stepTime;
    float stepTimer, totalTimer;
    ShootTarget *target1, *target2, *target3;
    bool *shootFlags;
    int curTime, curIndex, curSeq;
    int attackFlag;
    
    void onTargetShootEnd(int index);
    void initGouzi();
    void throwGouzi();
    bool isGouziHide;
    void hideGouzi();
    wyNode *gouziContainer1, *gouziContainer2, *gouziContainer3;
    wyTiledSprite *suolian1, *suolian2, *suolian3;
    wySPXSprite *gouzi1, *gouzi2, *gouzi3;
    wyNode *xuanzhuansuolian;
    int gouziLength;
    GouziObject *gouziObj1, *gouziObj2, *gouziObj3;
    void deleteGouziObj();
    void throwHuixuanCaidao();
    
    int huixuan_times;
    void hurt(float y);
    float bossScale;
    
    int caidaoAtk, kanAtk, xuliliandaoHP, xuliliandaoAtk, huixuancaidaoHP, huixuancaidaoAtk;
};

class ShootTarget : public Enemy {
public:
    int idx;
    float ofx, ofy;
    wyNode *aimContainer;
    Boss *boss;
    
    ShootTarget(float x, float y, float p_ofx, float p_ofy, int index, Boss *m_boss, float boxWidth = 24, float boxHeight = 24) : Enemy(x, y, APPEAR_NORMAL, NULL, NULL, NULL, false){
        idx = index;
        ofx = p_ofx;
        ofy = p_ofy;
        boss = m_boss;
        
        stunnable = false;
        canKnockBack = false;
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        int e_id = ENEMY_XIANJING3;
        animIdle = BOSS1_ANIM_CAIDAO;
        animDead = BOSS1_ANIM_CAIDAO;
        needShowDeadEffect = false;
        needDeadBlink = false;
        needStay = false;
        
        unitInterval = 0.05f;
        myScaleX = sGlobal->scaleX*0.4f;
        myScaleY = sGlobal->scaleY*0.4f;
        spxCacheArray = sGlobal->boss1Cache;
        
        bloodSprite = NULL;
        smokeSprite = NULL;
        shadow = NULL;
        isInitToShow = true;
        
        isAppeared = true;
        
        hp = 10;
        atk = 0;
        gold = 0;
        
        //        LOGE("cache len:%d", sGlobal->containerCache->num);
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(container, 102);
        
//        spxSprite = (wySPXSprite *)sGlobal->popFromCache(spxCacheArray);
//        spxSprite->stopAllActions();
//        spxSprite->setAFCSpriteCallback(NULL, NULL);
//        spxSprite->playAnimation(animIdle);
//        spxSprite->setUnitInterval(unitInterval);
//        spxSprite->setScaleX(myScaleX);
//        spxSprite->setScaleY(myScaleY);
//        spxSprite->setVisible(true);
//        spxSprite->setAlpha(255);
//        spxSprite->setColor(wyc4b(255, 0, 0, 255));
//        container->addChildLocked(spxSprite);
        aimContainer = wyNode::make();
        aimContainer->setPosition(DP(5)*sGlobal->scaleX, 0);
        container->addChildLocked(aimContainer);
        
        wySpriteEx *circle, *arrow_left, *arrow_right, *arrow_top, *arrow_bottom;
        circle = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "circle.png");//wySpriteEx::make(MyTextureManager::makePNG("circle.png"));
        circle->setScaleX(myScaleX);
        circle->setScaleY(myScaleY);
        aimContainer->addChildLocked(circle);
        
        wyAction *action1 = wyRepeatForever::make(wySequence::make(wyTintTo::make(0.1, 255, 255, 255, 255, 0, 0), wyTintTo::make(0.02, 255, 0, 0, 255, 255, 255), NULL));
        wyAction *action2 = wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.5f, DP(10)*sGlobal->scaleX, 0), wyMoveBy::make(0.5f, -DP(10)*sGlobal->scaleX, 0), NULL));
        
        arrow_left = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "arrow.png");//wySpriteEx::make(MyTextureManager::makePNG("arrow.png"));
        arrow_left->setScaleX(myScaleX);
        arrow_left->setScaleY(myScaleY);
        arrow_left->setRotation(90);
        arrow_left->setPosition(-DP(25)*sGlobal->scaleX, 0);
        aimContainer->addChildLocked(arrow_left);
        arrow_left->runAction(action1);
        arrow_left->runAction(action2);
        
        arrow_right = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "arrow.png");//wySpriteEx::make(MyTextureManager::makePNG("arrow.png"));
        arrow_right->setScaleX(myScaleX);
        arrow_right->setScaleY(myScaleY);
        arrow_right->setRotation(-90);
        arrow_right->setPosition(DP(25)*sGlobal->scaleX, 0);
        aimContainer->addChildLocked(arrow_right);
        action2 = wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.5f, -DP(10)*sGlobal->scaleX, 0), wyMoveBy::make(0.5f, DP(10)*sGlobal->scaleX, 0), NULL));
        arrow_right->runAction(action1->copy());
        arrow_right->runAction(action2);
        
        arrow_top = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "arrow.png");//wySpriteEx::make(MyTextureManager::makePNG("arrow.png"));
        arrow_top->setScaleX(myScaleX);
        arrow_top->setScaleY(myScaleY);
        arrow_top->setRotation(180);
        arrow_top->setPosition(0, DP(25)*sGlobal->scaleX);
        aimContainer->addChildLocked(arrow_top);
        action2 = wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.5f, 0, -DP(10)*sGlobal->scaleX), wyMoveBy::make(0.5f, 0, DP(10)*sGlobal->scaleX), NULL));
        arrow_top->runAction(action1->copy());
        arrow_top->runAction(action2);

        arrow_bottom = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "arrow.png");//wySpriteEx::make(MyTextureManager::makePNG("arrow.png"));
        arrow_bottom->setScaleX(myScaleX);
        arrow_bottom->setScaleY(myScaleY);
        arrow_bottom->setRotation(0);
        arrow_bottom->setPosition(0, -DP(25)*sGlobal->scaleX);
        aimContainer->addChildLocked(arrow_bottom);
        action2 = wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.5f, 0, DP(10)*sGlobal->scaleX), wyMoveBy::make(0.5f, 0, -DP(10)*sGlobal->scaleX), NULL));
        arrow_bottom->runAction(action1->copy());
        arrow_bottom->runAction(action2);
        
        bloodSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        container->addChildLocked(bloodSprite, 103);
        bloodSprite->setForceTickMode(true);
        bloodSprite->setUnitInterval(0.1f);
        bloodSprite->setLoopCount(0);
        bloodSprite->setFlipX(false);
        bloodSprite->setScaleX(1.0f*sGlobal->scaleX);
        bloodSprite->setScaleY(1.0f*sGlobal->scaleY);
        bloodSprite->setVisible(false);
        wyAFCSpriteCallback callback1 = {
            NULL,
            onBloodEnded
        };
        bloodSprite->setAFCSpriteCallback(&callback1, bloodSprite);
        bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
        
        deadSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        container->addChildLocked(deadSprite);
        deadSprite->setForceTickMode(true);
        deadSprite->setUnitInterval(0.03f);
        deadSprite->setLoopCount(0);
        deadSprite->setScaleX(2.0f*sGlobal->scaleX);
        deadSprite->setScaleY(2.0f*sGlobal->scaleY);
        wyAFCSpriteCallback callback = {
            NULL,
            onBDeadEnded
        };
        deadSprite->setVisible(false);
        deadSprite->setAFCSpriteCallback(&callback, this);
        deadSprite->playAnimation(SPX_WUTI_BAOZHA);
        
        obj_type = TYPE_ENEMY;
        
        float w = DP(boxWidth)*sGlobal->scaleX;
        float h = DP(boxWidth)*sGlobal->scaleX;
        b2BodyDef bd;
        bd.position.Set(x+ofx, y+ofy);
        bd.userData = this;
        bd.type = b2_dynamicBody;
        
        b2PolygonShape staticBox;
        staticBox.SetAsBox(m_box2d->pixel2Meter(w), m_box2d->pixel2Meter(h));
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &staticBox;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.isSensor = false;
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_ENEMY]^CAT_BITS[TYPE_GROUND];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);
        body->SetGravityScale(0.0f);
        
        offsetX = 0;
        offsetY = 0;
        
        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
        
        spxSprite = NULL;
    }
    
    ~ShootTarget() {
        
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

        if (sGlobal->mainGameLayer->hero->robertStatus == R_APPEARING && !isDead && isAppeared) {//
            float dist = body->GetPosition().x - sGlobal->mainGameLayer->hero->body->GetPosition().x - DP(200)*sGlobal->scaleX;
//            LOGE("ShootTarget: dist:%f, nearest dist: %f", dist, sGlobal->nearestDistance);
                if (dist < sGlobal->nearestDistance) {
//                    LOGE("im nearest...");
                    sGlobal->nearestEnemy = this;
                    sGlobal->nearestDistance = dist;
                }
        }
        
        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));

        if (deadSprite->isVisible()) {
            deadSprite->tick(dt);
        }
        if (bloodSprite->isVisible()) {
            bloodSprite->tick(dt);
        }
    }
    
    void onHitHero() {
        
    }
    
    void dead() {
        if (isDead)
            return;
        
        sGlobal->playEffect(se_bomb);
        
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
        if (!deadSprite->isVisible()) {
            deadSprite->setVisible(true);
        }
        
        aimContainer->setVisible(false);
        
        boss->onTargetShootEnd(idx);
        
        sGlobal->mainGameLayer->addToDisable(body);
        
        isDead = true;
    }
    
    void handleCollision(b2Body *actorB) {
        
    }
    
    void hurt(float y) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        bloodSprite->setVisible(true);
        bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
        bloodSprite->setPosition(-DP(20)*sGlobal->scaleX, m_box2d->meter2Pixel(y)-container->getPositionY()+DP(5)*sGlobal->scaleY);
    }
    
    void beginContact(b2Body *actorB) {
        
    }
    
    void endContact(b2Body *actorB) {
        
    }
    
    void destroyMyself() {
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }

        if (deadSprite != NULL) {
            deadSprite->stopAllActions();
            deadSprite->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, deadSprite);
        }
        
        if (bloodSprite != NULL) {
            bloodSprite->stopAllActions();
            bloodSprite->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, bloodSprite);
        }
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        this->release();
        
        return;
    }
    
    bool canBeHurt() {
        return true;
    }
};


class CaiDao : public EnemyBulletBase {
public:
//    float lifetime;
    bool isGenZong;
    bool isBaozha;
    bool hasAction;
    wySPXSprite *baozha;
    wyParticleSystem *emitter;
    
    CaiDao(wyArray *p_spxCacheArray, int p_animIdle, float p_unitInterval, float p_myScaleX, float p_myScaleY, int zorder = 103, bool isBao = false) : EnemyBulletBase() {
        
        b_id = 1;
        
        isGenZong = false;
        isBaozha = isBao;
        hasAction = false;
        
        emitter = NULL;
        
        spxCacheArray = p_spxCacheArray;
        animIdle = p_animIdle;
        unitInterval = p_unitInterval;
        myScaleX = p_myScaleX;
        myScaleY = p_myScaleY;
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        int e_id = ENEMY_XIANJING3;

        hp = 1;
        atk = 0;
        gold = 0;

        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(container, zorder);
        
        spxSprite = (wySPXSprite *)sGlobal->popFromCache(spxCacheArray);
        spxSprite->stopAllActions();
        spxSprite->setAFCSpriteCallback(NULL, NULL);
        spxSprite->playAnimation(animIdle);
        spxSprite->setUnitInterval(unitInterval);
        spxSprite->setScaleX(myScaleX);
        spxSprite->setScaleY(myScaleY);
        spxSprite->setVisible(true);
        spxSprite->setAlpha(255);
        container->addChildLocked(spxSprite);
        
        obj_type = TYPE_ENEMY_BULLET;
        
        float boxWidth = DP(20)*sGlobal->scaleX;
        float boxHeight = DP(10)*sGlobal->scaleX;
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(DP(60)*sGlobal->scaleY), m_box2d->pixel2Meter(DP(50)*sGlobal->scaleY));
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
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_ENEMY_BULLET];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY_BULLET];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY_BULLET];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);
        body->SetGravityScale(0.0f);
        
        offsetX = 0;
        offsetY = 0;
        
        if (isBaozha) {
            baozha = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
            container->addChildLocked(baozha, 102);
            baozha->setForceTickMode(true);
            baozha->setUnitInterval(0.03f);
            baozha->setLoopCount(0);
            baozha->setScaleX(2.0f*sGlobal->scaleX);
            baozha->setScaleY(2.0f*sGlobal->scaleY);
            baozha->setVisible(false);
            baozha->playAnimation(SPX_WUTI_BAOZHA);
        } else {
            baozha = NULL;
        }
    }
    
    ~CaiDao() {
        
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

        if (hasAction) {
            if (!isGenZong) {
                body->SetTransform(b2Vec2(m_box2d->pixel2Meter(container->getPositionX()+spxSprite->getPositionX()), m_box2d->pixel2Meter(container->getPositionY()+spxSprite->getPositionY())), -wyMath::d2r(spxSprite->getRotation()));
//                LOGE("posy:%f", sGlobal->mainGameLayer->getPositionY());
//                emitter->setPosition(emitter->getPositionX(), emitter->getPositionY()-sGlobal->mainGameLayer->getPositionY());
//                LOGE("angle:%f", spxSprite->getRotation());
                emitter->setDirectionAngleVariance(360-spxSprite->getRotation(), 0);
            } else {
                float dist = m_box2d->meter2Pixel(body->GetPosition().x - sGlobal->mainGameLayer->hero->body->GetPosition().x);
                if (dist < DP(100)*sGlobal->scaleX) {
                    spxSprite->stopAllActions();
                    container->stopAllActions();
                    spxSprite->setPosition(0, 0);
                    float maxSpeed = 5.0f;
                    b2Vec2 desiredVelocity = sGlobal->mainGameLayer->hero->body->GetPosition() - body->GetPosition()-b2Vec2(0,m_box2d->pixel2Meter(DP(20)*sGlobal->scaleY));
                    if (maxSpeed > 0)
                    {
                        desiredVelocity.Normalize();
                        desiredVelocity *= maxSpeed;
                    }
                    b2Vec2 finalVelocity = desiredVelocity - body->GetLinearVelocity();
                    b2Vec2 steerVector = b2Vec2(finalVelocity.x, finalVelocity.y);
                    body->ApplyLinearImpulse(steerVector, body->GetPosition());
                    
                    container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
                    b2Vec2 vel = body->GetLinearVelocity();
                    float angle = atan(vel.y/vel.x);
                    spxSprite->setRotation(-wyMath::r2d(angle));
                } else {
                    body->SetTransform(b2Vec2(m_box2d->pixel2Meter(container->getPositionX()+spxSprite->getPositionX()), m_box2d->pixel2Meter(container->getPositionY()+spxSprite->getPositionY())), -wyMath::d2r(spxSprite->getRotation()));
                }
            }
        }
        else {
            if (sGlobal->mainGameLayer->hero->isDead) {
                body->SetLinearVelocity(b2Vec2(-HERO_VEL_X-body->GetLinearVelocity().x, body->GetLinearVelocity().y));
            }
            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
        }
        
        if (isBaozha && baozha != NULL && baozha->isVisible()) {
            baozha->tick(dt);
        }
    }
    
    static void onDisappearEnd1(wyAction* action, void* data) {
        EnemyBulletBase *enemyBullet = (EnemyBulletBase *) data;

        enemyBullet->spxSprite->stopAllActions();
        enemyBullet->spxSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(enemyBullet->spxCacheArray, enemyBullet->spxSprite);
        
        enemyBullet->container->stopAllActions();
        enemyBullet->container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, enemyBullet->container);

        delete enemyBullet;
    }
    
    void destroyMyself() {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        if (!isBaozha) {
            wyFadeOut* fade = wyFadeOut::make(0.2f, true);
            wyActionCallback callback = {
                NULL,
                onDisappearEnd1,
                NULL
            };
            fade->setCallback(&callback, this);
            spxSprite->runAction(fade);
            container->runAction(wyMoveBy::make(0.2f, m_box2d->meter2Pixel(sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x*0.2f), -DP(6)*sGlobal->scaleY));
//            container->stopAllActions();
        } else {
            spxSprite->stopAllActions();
            spxSprite->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(spxCacheArray, spxSprite);
            
            if (baozha != NULL) {
                baozha->stopAllActions();
                baozha->setAFCSpriteCallback(NULL, NULL);
                sGlobal->pushToCache(sGlobal->effects_cache, baozha);
            }
            
            if (emitter != NULL) {
                emitter->stopAllActions();
//                emitter->setVisible(false);
                emitter->stopSystem();
                emitter->resetSystem();
                sGlobal->pushToCache(sGlobal->fire_emitterArray, emitter);
            }
            
            container->stopAllActions();
            container->removeAllChildren(true);
            sGlobal->pushToCache(sGlobal->containerCache, container);
        
            delete this;
        }
        
        return;
    }
    
    void onHitHero() {
        if (!isBaozha) {
            if (sGlobal->mainGameLayer->hero->isBoosting || sGlobal->mainGameLayer->hero->isDeBoosting) {
                sGlobal->mainGameLayer->addToDestroy(body);
                return;
            }
        } else {
            if (!baozha->isVisible()) {
                baozha->setVisible(true);
                baozha->setPosition(spxSprite->getPositionX(), spxSprite->getPositionY());
                sGlobal->playEffect(se_bomb);
            }
            emitter->setVisible(false);
            spxSprite->setVisible(false);
        }
        
        b2Fixture *fixture = body->GetFixtureList();
        b2Fixture *next;
        while (fixture != NULL) {
            next = fixture->GetNext();
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = filter.maskBits ^ CAT_BITS[TYPE_MAO];
            fixture->SetFilterData(filter);
            fixture = next;
        }
    }
};

class GouziObject : public EnemyBulletBase {
public:
    wyNode *followNode;
    int index;
    int degree;
    
    GouziObject(int x, int y, int attack, wyNode *node, int m_ofx, int m_ofy, int idx, int d) : EnemyBulletBase() {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        b_id = 1;
        
        followNode = node;
        offsetX = m_ofx;
        offsetY = m_ofy;
        index = idx;
        degree = d;
        
        int e_id = ENEMY_XIANJING3;
        
        hp = 1;
        atk = attack;
        gold = 0;
        
        obj_type = TYPE_ENEMY_BULLET;
        
        float boxWidth = DP(30)*sGlobal->scaleX;
        float boxHeight = DP(20)*sGlobal->scaleY;
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(x+offsetX), m_box2d->pixel2Meter(y+offsetY));
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
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_ENEMY_BULLET];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY_BULLET];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY_BULLET];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);
        body->SetGravityScale(0.0f);
    }
    
    ~GouziObject() {
        
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        wyNode *c = Boss1::instance->container;
        body->SetTransform(b2Vec2(m_box2d->pixel2Meter(c->getPositionX()+followNode->getPositionX()*wyMath::cos(-wyMath::d2r(degree))+followNode->getParent()->getPositionX()+offsetX), m_box2d->pixel2Meter(c->getPositionY()+followNode->getPositionY()+followNode->getParent()->getPositionY()+offsetY+followNode->getPositionX()*wyMath::sin(-wyMath::d2r(degree)))), 0);
    }
    
    void destroyMyself() {
        if (Boss1::instance != NULL) {
            switch (index) {
                case 0:
                    Boss1::instance->gouziObj1 = NULL;
                    break;
                case 1:
                    Boss1::instance->gouziObj2 = NULL;
                    break;
                case 2:
                    Boss1::instance->gouziObj3 = NULL;
                    break;
                default:
                    break;
            }
        }
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        delete this;
        
        return;
    }
};

class HuixuanCaidao : public ShootableObject {
public:
    wyNode *aimContainer;
    wyParticleSystem *emitter;
    wyArray *emitterCacheArray;
    bool isPhysics;
    bool needBaozha;
    bool isQuantou;
    float initialDegree;
    float speed;
    
    HuixuanCaidao(wyArray *p_spxCacheArray, int p_animIdle, float p_unitInterval, float p_myScaleX, float p_myScaleY, float p_boxWidth, float p_boxHeight, int attack) : ShootableObject(p_spxCacheArray, p_animIdle, p_unitInterval, p_myScaleX, p_myScaleY, p_boxWidth, p_boxHeight, attack) {
        
        stunnable = false;
        canKnockBack = false;
        isAppeared = true;
        
//        atk = attack;
        stunnable = false;
        isQuantou = false;
        needBaozha = true;
        isPhysics = false;
        aimContainer = wyNode::make();
        container->addChildLocked(aimContainer);
        
        float arrowScaleX = sGlobal->scaleX*0.4f;
        float arrowScaleY = sGlobal->scaleY*0.4f;
        
        wySpriteEx *circle, *arrow_left, *arrow_right, *arrow_top, *arrow_bottom;
        circle = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "circle.png");
        circle->setScaleX(arrowScaleX);
        circle->setScaleY(arrowScaleY);
        aimContainer->addChildLocked(circle);
        
        wyAction *action1 = wyRepeatForever::make(wySequence::make(wyTintTo::make(0.1, 255, 255, 255, 255, 0, 0), wyTintTo::make(0.02, 255, 0, 0, 255, 255, 255), NULL));
        wyAction *action2 = wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.5f, DP(10)*sGlobal->scaleX, 0), wyMoveBy::make(0.5f, -DP(10)*sGlobal->scaleX, 0), NULL));
        
        arrow_left = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "arrow.png");
        arrow_left->setScaleX(arrowScaleX);
        arrow_left->setScaleY(arrowScaleY);
        arrow_left->setRotation(90);
        arrow_left->setPosition(-DP(25)*sGlobal->scaleX, 0);
        aimContainer->addChildLocked(arrow_left);
        arrow_left->runAction(action1);
        arrow_left->runAction(action2);
        
        arrow_right = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "arrow.png");
        arrow_right->setScaleX(arrowScaleX);
        arrow_right->setScaleY(arrowScaleY);
        arrow_right->setRotation(-90);
        arrow_right->setPosition(DP(25)*sGlobal->scaleX, 0);
        aimContainer->addChildLocked(arrow_right);
        action2 = wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.5f, -DP(10)*sGlobal->scaleX, 0), wyMoveBy::make(0.5f, DP(10)*sGlobal->scaleX, 0), NULL));
        arrow_right->runAction(action1->copy());
        arrow_right->runAction(action2);
        
        arrow_top = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "arrow.png");
        arrow_top->setScaleX(arrowScaleX);
        arrow_top->setScaleY(arrowScaleY);
        arrow_top->setRotation(180);
        arrow_top->setPosition(0, DP(25)*sGlobal->scaleX);
        aimContainer->addChildLocked(arrow_top);
        action2 = wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.5f, 0, -DP(10)*sGlobal->scaleX), wyMoveBy::make(0.5f, 0, DP(10)*sGlobal->scaleX), NULL));
        arrow_top->runAction(action1->copy());
        arrow_top->runAction(action2);
        
        arrow_bottom = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "arrow.png");
        arrow_bottom->setScaleX(arrowScaleX);
        arrow_bottom->setScaleY(arrowScaleY);
        arrow_bottom->setRotation(0);
        arrow_bottom->setPosition(0, -DP(25)*sGlobal->scaleX);
        aimContainer->addChildLocked(arrow_bottom);
        action2 = wyRepeatForever::make(wySequence::make(wyMoveBy::make(0.5f, 0, DP(10)*sGlobal->scaleX), wyMoveBy::make(0.5f, 0, -DP(10)*sGlobal->scaleX), NULL));
        arrow_bottom->runAction(action1->copy());
        arrow_bottom->runAction(action2);
        
        bloodSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        container->addChildLocked(bloodSprite, 103);
        bloodSprite->setForceTickMode(true);
        bloodSprite->setUnitInterval(0.1f);
        bloodSprite->setLoopCount(0);
        bloodSprite->setFlipX(false);
        bloodSprite->setScaleX(1.0f*sGlobal->scaleX);
        bloodSprite->setScaleY(1.0f*sGlobal->scaleY);
        bloodSprite->setVisible(false);
        wyAFCSpriteCallback callback1 = {
            NULL,
            onBloodEnded
        };
        bloodSprite->setAFCSpriteCallback(&callback1, bloodSprite);
        bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
        
        deadSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        container->addChildLocked(deadSprite, 103);
        deadSprite->setForceTickMode(true);
        deadSprite->setUnitInterval(0.03f);
        deadSprite->setLoopCount(0);
        deadSprite->setScaleX(2.0f*sGlobal->scaleX);
        deadSprite->setScaleY(2.0f*sGlobal->scaleY);
        wyAFCSpriteCallback callback = {
            NULL,
            onBDeadEnded
        };
        deadSprite->setVisible(false);
        deadSprite->setAFCSpriteCallback(&callback, this);
        deadSprite->playAnimation(SPX_WUTI_BAOZHA);
        
        emitter = NULL;
    }
    
    ~HuixuanCaidao() {
        
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        if (isDead) {
            if (deadSprite->isVisible()) {
                deadSprite->tick(dt);
            }
            if (bloodSprite->isVisible()) {
                bloodSprite->tick(dt);
            }
            return;
        }
        
//        if (sGlobal->mainGameLayer->hero->robertStatus == R_APPEARING && !isDead && isAppeared) {//
//            float dist = body->GetPosition().x - sGlobal->mainGameLayer->hero->body->GetPosition().x;
////            LOGE("HuixuanCaidao: dist:%f, nearest dist: %f", dist, sGlobal->nearestDistance);
//            if (dist <= -DP(50)*sGlobal->scaleX) {
//                sGlobal->nearestEnemy = NULL;
//                sGlobal->nearestDistance = 1000;
//            } else {
//                if (dist < sGlobal->nearestDistance && dist > 0) {
//                    sGlobal->nearestEnemy = this;
//                    sGlobal->nearestDistance = dist;
//                }
//            }
//        }
        
//        float dist = m_box2d->meter2Pixel(body->GetPosition().x - sGlobal->mainGameLayer->hero->body->GetPosition().x);
//        if (dist < DP(20)) {
//            container->stopAllActions();
//            float maxSpeed = 10.0f;
//            b2Vec2 desiredVelocity = sGlobal->mainGameLayer->hero->body->GetPosition() - body->GetPosition()-b2Vec2(0,m_box2d->pixel2Meter(DP(20)*sGlobal->scaleY));
//            if (maxSpeed > 0)
//            {
//                desiredVelocity.Normalize();
//                desiredVelocity *= maxSpeed;
//            }
//            b2Vec2 finalVelocity = desiredVelocity - body->GetLinearVelocity();
//            b2Vec2 steerVector = b2Vec2(finalVelocity.x, finalVelocity.y);
//            body->ApplyLinearImpulse(steerVector, body->GetPosition());
//
//            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
//            b2Vec2 vel = body->GetLinearVelocity();
//            float angle = atan(vel.y/vel.x);
//            if (vel.x > 0) {
//                angle += M_PI;
//            }
//            container->setRotation(-wyMath::r2d(angle));
//        } else {
        if (isPhysics) {
            float heroVelx = sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x;
            body->SetLinearVelocity(b2Vec2(-speed*cos(wyMath::d2r(initialDegree))+heroVelx, body->GetLinearVelocity().y));
            
            b2Vec2 vel = body->GetLinearVelocity();
            float angle = atan(vel.y/(vel.x-heroVelx));
            spxSprite->setRotation(-wyMath::r2d(angle));
            
            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
        }
        else if (isQuantou) {
            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
        }
        else {
            body->SetTransform(b2Vec2(m_box2d->pixel2Meter(container->getPositionX()+spxSprite->getPositionX()), m_box2d->pixel2Meter(container->getPositionY()+spxSprite->getPositionY())), 0);
        }
        
        aimContainer->setPosition(spxSprite->getPositionX(), spxSprite->getPositionY());
        
        if (bloodSprite->isVisible()) {
            bloodSprite->tick(dt);
        }
//        }
    }
    
    void onHitHero() {
        if (needBaozha) {
            dead();
        } else {
            b2Fixture *fixture = body->GetFixtureList();
            b2Fixture *next;
            while (fixture != NULL) {
                next = fixture->GetNext();
                b2Filter filter = fixture->GetFilterData();
                filter.maskBits = filter.maskBits ^ CAT_BITS[TYPE_MAO];
                fixture->SetFilterData(filter);
                fixture = next;
            }
            
            if (sGlobal->nearestEnemy == this) {
                sGlobal->nearestEnemy = NULL;
                sGlobal->nearestDistance = 1000;
            }
        }
    }
    
    void dead() {
        if (isDead)
            return;
        
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
        isBaoing = true;
        isDead = true;
        sGlobal->playEffect(se_bomb);

        container->stopAllActions();
        spxSprite->stopAllActions();
        spxSprite->setVisible(false);
        if (!deadSprite->isVisible()) {
            deadSprite->setVisible(true);
            deadSprite->setPosition(spxSprite->getPositionX(), spxSprite->getPositionY());
        }
        aimContainer->setVisible(false);
        if (emitter != NULL) {
            emitter->stopSystem();
        }
        
        sGlobal->mainGameLayer->addToDisable(body);
    }
    
    void destroyMyself() {
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
//        LOGE("aaabbb:%d", sGlobal->effects_cache->num);
        spxSprite->stopAllActions();
        spxSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(spxCacheArray, spxSprite);
        
        deadSprite->stopAllActions();
        deadSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(sGlobal->effects_cache, deadSprite);
        
        if (emitter != NULL) {
            emitter->stopAllActions();
            emitter->setVisible(false);
            emitter->stopSystem();
//            emitter->resetSystem();
            sGlobal->pushToCache(emitterCacheArray, emitter);
        }
        
        if (bloodSprite != NULL) {
            bloodSprite->stopAllActions();
            bloodSprite->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, bloodSprite);
        }
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        delete this;
        
        return;
    }
    
    void hurt(float y) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        bloodSprite->setVisible(true);
        bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
        bloodSprite->setPosition(-DP(10)*sGlobal->scaleX+spxSprite->getPositionX(), m_box2d->meter2Pixel(y)-container->getPositionY()-spxSprite->getPositionY()+DP(5)*sGlobal->scaleY);
    }
};



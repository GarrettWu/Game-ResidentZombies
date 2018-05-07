//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
// 

#include "Boss2.h"
#include "Global.h"
#include "EnemyFactory.h"
#include "SpittleZombie.h"
#include "MaleZombie.h"
#include "Devourer.h"
#include "FatZombie.h"
#include "Bonus.h"
#include "EndPoint.h"
#include "Barricade.h"
#include "GhostBaby.h"
#include "FatZombie.h"
#include "MuscleZombie1.h"
#include "FlyZombie2.h"
#include "FemaleZombie.h"
#include "HeroBullet.h"

Boss2 *Boss2::instance = NULL;

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    Boss2* enemy = (Boss2*)data;
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    if (enemy->status == BOSS2_ATK_RENG_MAO) {
        if (sprite->getCurrentFrame() == 3) {
            enemy->shootArrow(rand()%3+1);
        }
    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    Boss2* enemy = (Boss2*)data;
    wySPXSprite *enemySprite = ((wySPXSprite *) enemy->spxSprite);
    if (enemy->status == BOSS2_ATK_RENG_MAO) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS2_ANIM_ZOULU);
        enemy->status = BOSS2_WALK;
    }
    else if (enemy->status == BOSS2_ATK_CHONGJI_0) {
        enemy->doChongji();
    }
    else if (enemy->status == BOSS2_ATK_XULI_JIGUANG_1) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS2_ANIM_ZOULU);
        enemy->status = BOSS2_WALK;
//        sGlobal->mainGameLayer->addToRecreateFixture(enemy);
//        
//        if (!sGlobal->isRealEndless) {
//            enemy->body->SetGravityScale(1.0f);
//        }
    }
    else if (enemy->status == BOSS2_ATK_HUIXUAN_MAO_0) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS2_ANIM_ZOULU);
        enemy->status = BOSS2_WALK;
        
        enemy->isFiringMissle = true;
        enemy->firingMissleTimer = 0;
        enemy->fireTimes = 1;
        enemy->daodanCount = rand()%3+1;
        enemy->firingMissleInterval = randRange(3, 6)*0.1f;
        enemy->create3mao();
    }
    else if (enemy->status == BOSS_HURT) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS2_ANIM_ZOULU);
        enemy->status = BOSS2_WALK;
    }
}

Boss2::Boss2(float x, float y) : Boss(x, y){
    instance = this;

    deadType = DEAD_TYPE_BOSS;
    status_idle = BOSS2_WALK;
    
    e_id = ENEMY_BOSS2;

    if (sGlobal->isRealEndless) {
        int colIndex;
        float power = 1.0f;
        
        switch (sGlobal->combatRound) {
            case 1:
                hp = sGlobal->ENEMY_HP_EASY[e_id];
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                colIndex = COL_NORMAL;
                break;
            case 2:
                hp = sGlobal->ENEMY_HP_HARD[e_id];
                atk = sGlobal->ENEMY_ATK_HARD[e_id];
                gold = sGlobal->ENEMY_GOLD_HARD[e_id];
                colIndex = COL_HARD;
                break;
            case 3:
                hp = sGlobal->ENEMY_HP_CRAZY[e_id];
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id];
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id];
                colIndex = COL_CRAZY;
                break;
            default:
                colIndex = COL_CRAZY;
                power = (float)pow(sGlobal->ENDLESS_ATK_POWER, sGlobal->combatRound - 3);
                hp = sGlobal->ENEMY_HP_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_HP_POWER, sGlobal->combatRound - 3);
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_ATK_POWER, sGlobal->combatRound - 3);
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_GOLD_POWER, sGlobal->combatRound - 3);
                break;
        }
        
        lanceAtk = sGlobal->BOSS2_LANCE_ATK[colIndex];
        chongfengAtk = sGlobal->BOSS2_CHONGFENG_ATK[colIndex]* power;
        xulilaserHP = sGlobal->BOSS2_XULI_LASER_HP[colIndex]* power;
        xulilaserAtk = sGlobal->BOSS2_XULI_LASER_ATK[colIndex]* power;
        smalllanceHP = sGlobal->BOSS2_SMALL_LANCE_HP[colIndex]* power;
        smalllanceAtk = sGlobal->BOSS2_SMALL_LANCE_ATK[colIndex]* power;
        
//        LOGE("combat round:%d, hp:%d, atk:%d, gold:%d, lanceAtk:%d, chongfengAtk:%d, xulilaserHP:%d, xulilaserAtk:%d, smalllanceHP:%d, smalllanceAtk:%d", sGlobal->combatRound, hp, atk, gold, lanceAtk, chongfengAtk, xulilaserHP, xulilaserAtk, smalllanceHP, smalllanceAtk);
    } else {
        int colIndex;
        switch (sGlobal->currentSModeDifficulty) {
            case DIFFICULTY_NORMAL:
                hp = sGlobal->ENEMY_HP_EASY[e_id];
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                colIndex = COL_NORMAL;
                break;
            case DIFFICULTY_HEROIC:
                hp = sGlobal->ENEMY_HP_HARD[e_id];
                atk = sGlobal->ENEMY_ATK_HARD[e_id];
                gold = sGlobal->ENEMY_GOLD_HARD[e_id];
                colIndex = COL_HARD;
                break;
            case DIFFICULTY_HARD:
                hp = sGlobal->ENEMY_HP_CRAZY[e_id];
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id];
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id];
                colIndex = COL_CRAZY;
                break;
            default:
                hp = sGlobal->ENEMY_HP_EASY[e_id];
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                colIndex = COL_NORMAL;
                break;
        }
        
        lanceAtk = sGlobal->BOSS2_LANCE_ATK[colIndex];
        chongfengAtk = sGlobal->BOSS2_CHONGFENG_ATK[colIndex];
        xulilaserHP = sGlobal->BOSS2_XULI_LASER_HP[colIndex];
        xulilaserAtk = sGlobal->BOSS2_XULI_LASER_ATK[colIndex];
        smalllanceHP = sGlobal->BOSS2_SMALL_LANCE_HP[colIndex];
        smalllanceAtk = sGlobal->BOSS2_SMALL_LANCE_ATK[colIndex];
    }
    atk *= sGlobal->ENEMY_ATK_MULTI;
    hp *= sGlobal->ENEMY_HP_MULTI;
    gold *= sGlobal->ENEMY_GOLD_MULTI;
    
    lanceAtk *= sGlobal->ENEMY_ATK_MULTI;
    chongfengAtk *= sGlobal->ENEMY_ATK_MULTI;
    xulilaserHP *= sGlobal->ENEMY_ATK_MULTI;
    xulilaserAtk *= sGlobal->ENEMY_ATK_MULTI;
    smalllanceHP *= sGlobal->ENEMY_ATK_MULTI;
    smalllanceAtk *= sGlobal->ENEMY_ATK_MULTI;
    
    atk = chongfengAtk;
//    LOGE("diff:%d, e_id:%d, atk:%d, hp:%d, gold:%d, lanceAtk:%d, chongfengAtk:%d, xulilaserHP:%d, xulilaserAtk:%d, smalllanceHP:%d, smalllanceAtk:%d", sGlobal->currentSModeDifficulty, e_id, atk, hp, gold, lanceAtk, chongfengAtk, xulilaserHP, xulilaserAtk, smalllanceHP, smalllanceAtk);
    
    maxHp = hp;
    sub_type = 100;
    
    obj_type = TYPE_ENEMY;
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
	spxSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->boss2Cache);//wySPXSprite::make("boss2.sprite", false, MyTextureManager::makePNG("boss2.png"), 0);
    spxSprite->setForceTickMode(true);
	spxSprite->setUnitInterval(0.2f);
    spxSprite->setLoopCount(-1);
    spxSprite->setScaleX(sGlobal->scaleX * 0.75f);
    spxSprite->setScaleY(sGlobal->scaleY * 0.75f);
    spxSprite->playAnimation(BOSS2_ANIM_DAIJI);
    container->addChildLocked(spxSprite, 1);
    
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y));
    bd.userData = this;
    bd.type = b2_dynamicBody;
    body = world->CreateBody(&bd);
    body->SetTransform(bd.position, 0);
    
    bodyRect = spxSprite->getFrameRect();
    bodyRect.x *= spxSprite->getScaleX();
    bodyRect.y *= spxSprite->getScaleY();
    bodyRect.width *= spxSprite->getScaleX();
    bodyRect.height *= spxSprite->getScaleY();
    
    createFixtureByCurrentFrame(sGlobal->isRealEndless);
    body->SetActive(false);
    body->SetAwake(false);

    if (sGlobal->isRealEndless) {
        body->SetGravityScale(0.0f);
    }
//    spxSprite->playAnimation(BOSS2_ANIM_ZOULU);
    
    isScheduled = false;
    status = BOSS2_IDLE;
    
    sGlobal->bossEyeZ = 1;
    
    deadSprite1 = NULL;
    deadSprite2 = NULL;
    
    isHeightInited = false;
    
    animDead = BOSS2_ANIM_SI;
    animWalk = BOSS2_ANIM_ZOULU;
    
    needDeadBlink = false;
    needStay = true;

    if (sGlobal->isRealEndless) {
        jiguangTime = randRange(12, 16)*0.1f;
    } else {
        jiguangTime = randRange(15, 20)*0.1f;
    }
    
    if (sGlobal->isRealEndless) {
        body->SetGravityScale(0.0f);
        baseY = m_box2d->pixel2Meter(y+DP(35)*sGlobal->scaleY);
        body->SetTransform(b2Vec2(m_box2d->pixel2Meter(x), baseY), 0);
    }
//    LOGE("boss2:%f,%f,%f", offsetX, offsetY, m_box2d->meter2Pixel(halfBodyHeightInMeter));
    
    isInitToShow = false;
    
    stayDist = wyDevice::winWidth - sGlobal->hero_offset_left-DP(125)*sGlobal->scaleX;
    
    elapsedAtkTime = 0;
    
    target1 = NULL;
    target2 = NULL;
    target3 = NULL;
    shootFlags = new bool[3];
    
    isLighting = false;
    lightingTimer = 0;
    lightingBody = NULL;
    lightingBody1 = NULL;
    moveLightingTimer = 0;
    
    shadowScaleX = 6.0f;
    shadowX = DP(10)*sGlobal->scaleX;
    shadowY = -DP(115)*sGlobal->scaleY;
    
    bossBlood->setPosition(-DP(10)*sGlobal->scaleX, DP(110)*sGlobal->scaleY);
    
    s1 = NULL;
    s2 = NULL;
    s3 = NULL;
    
    spxCacheArray = sGlobal->boss2Cache;
    
    isFiringMissle = false;
    fireTimes = 0;
    firingMissleTimer = 0;
    daodanCount = 0;
    
    faintSpriteScale = 1.5*sGlobal->scaleX;
    faintSpriteX = DP(-10)*sGlobal->scaleY;
    faintSpriteY = DP(100)*sGlobal->scaleY;
    
//    wySprite *a = NULL;
//    a->setColor(wyc3b(255, 0, 0));
//    LOGE("aaa:%s", a->getClassName());
}

Boss2::~Boss2() {
    delete [] shootFlags;
    instance = NULL;
}

void Boss2::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    Boss::update(dt);
    
    if (isDead) {
        deadSprite2->tick(dt);
    }
    
    if (status == BOSS2_IDLE && distance < stayDist) {
        spxSprite->playAnimation(BOSS2_ANIM_ZOULU);
        setCurVel(sGlobal->heroVelocityX, 0.0f);
        isMoving = true;
        status = BOSS2_WALK;
        
        sGlobal->playEffect(se_boss2_start);
    }
    
    if (status == BOSS2_ATK_CHONGJI_1) {
        if (distance < -sGlobal->hero_offset_left*2) {
            spxSprite->setLoopCount(-1);
            spxSprite->setAFCSpriteCallback(NULL, NULL);
            spxSprite->playAnimation(BOSS2_ANIM_ZOULU);
            sGlobal->mainGameLayer->addToRecreateFixture(this);
            hideHuanying();
            setCurVel(0, 0.0f);
            
            body->SetTransform(b2Vec2(sGlobal->mainGameLayer->hero->body->GetPosition().x + m_box2d->pixel2Meter(wyDevice::winWidth+DP(80)*sGlobal->scaleX), body->GetPosition().y), 0);

            status = BOSS2_ATK_CHONGJI_2;
        }
        else {
            if (isCongjiInit) {
                isCongjiInit = false;
            } else {
                s1->tick(dt);
                s2->tick(dt);
                s3->tick(dt);
            }
        }
    }
    
    if (status == BOSS2_ATK_CHONGJI_2) {
        if (distance > 0 && distance < stayDist) {
            setCurVel(sGlobal->heroVelocityX, 0.0f);
            spxSprite->setLoopCount(-1);
            spxSprite->setAFCSpriteCallback(NULL, NULL);
            spxSprite->playAnimation(BOSS2_ANIM_ZOULU);
            status = BOSS2_WALK;
            
//            sGlobal->mainGameLayer->addToRecreateFixture(this);
//            hideHuanying();
        }
//        else {
//            s1->tick(dt);
//            s2->tick(dt);
//            s3->tick(dt);
//        }
    }
    
    if (isLighting) {
        lightingTimer += dt;
        moveLightingTimer += dt;
        if (lightingBody != NULL) {
            BaseObject *obj = (BaseObject*) lightingBody->GetUserData();
            wyNode *sprite = (wyNode *) obj->container;
            
            BaseObject *obj1 = (BaseObject*) lightingBody1->GetUserData();
            wyNode *sprite1 = (wyNode *) obj1->container;
            if (lightingTimer > jiguangTime) {
                hideJiguang();
                
                spxSprite->setLoopCount(-1);
                spxSprite->setAFCSpriteCallback(NULL, NULL);
                spxSprite->playAnimation(BOSS2_ANIM_ZOULU);
                status = BOSS2_WALK;
                sGlobal->mainGameLayer->addToRecreateFixture(this);
            } else {
//                if (moveLightingTimer > 0.05f) {
//                    moveLightingTimer = 0;
//                    angle-=0.01f;
//                }
//                lightingBody->SetTransform(b2Vec2(body->GetPosition().x - m_box2d->pixel2Meter(DP(54)*sGlobal->scaleX), body->GetPosition().y + m_box2d->pixel2Meter(DP(22)*sGlobal->scaleY)), angle);
                
                sprite->setPosition(m_box2d->meter2Pixel(lightingBody->GetPosition().x + lightingWidth), m_box2d->meter2Pixel(lightingBody->GetPosition().y) + lightingHeight);
                sprite->setRotation(-lightingBody->GetAngle() *57.3);
                
//                lightingBody1->SetTransform(b2Vec2(body->GetPosition().x + m_box2d->pixel2Meter(DP(10)*sGlobal->scaleX), body->GetPosition().y + m_box2d->pixel2Meter(DP(22)*sGlobal->scaleY)), angle);
                sprite1->setPosition(m_box2d->meter2Pixel(lightingBody1->GetPosition().x + lightingWidth), m_box2d->meter2Pixel(lightingBody1->GetPosition().y) + lightingHeight);
                sprite1->setRotation(-lightingBody1->GetAngle() *57.3);
            }
        }
    }
    
    if (status == BOSS2_WALK && !isScheduled && !isDead) {
        scheduleNextAttack();
    }
    
    float heroVelX = sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x;
    if (status != BOSS2_IDLE && fabs(body->GetLinearVelocity().x - relativeVelX - heroVelX ) > 0.0000001f) {
        body->SetLinearVelocity(b2Vec2(heroVelX + relativeVelX, body->GetLinearVelocity().y));
        if (lightingBody != NULL) {
            lightingBody->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, body->GetLinearVelocity().y));
        }
        if (lightingBody1 != NULL) {
            lightingBody1->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, body->GetLinearVelocity().y));
        }
    }
    
    if (status == BOSS2_ATK_XULI_JIGUANG_0) {
        if (curTime < 2) {
            stepTimer += dt;
            if (stepTimer > stepTime) {
                curTime++;
                generateNextTarget();
            }
        }
        
        if (attackFlag == 0) {
            totalTimer += dt;
            if (totalTimer > totalTime) {
                attackSuccess();
            }
        }
        
        if (target1 != NULL) {
            target1->body->SetTransform(body->GetPosition()+b2Vec2(target1->ofx, target1->ofy), 0);
        }
        
        if (target2 != NULL) {
            target2->body->SetTransform(body->GetPosition()+b2Vec2(target2->ofx, target2->ofy), 0);
        }
        
        if (target3 != NULL) {
            target3->body->SetTransform(body->GetPosition()+b2Vec2(target3->ofx, target3->ofy), 0);
        }
    }
    
    if (isFiringMissle) {
        if (fireTimes < daodanCount) {
            firingMissleTimer += dt;
            if (firingMissleTimer > firingMissleInterval) {
                firingMissleTimer = 0;
                fireTimes++;
                create3mao();
            }
        } else {
            isFiringMissle = false;
        }
    }
}

void Boss2::doAttack() {
    if (status == BOSS_FAKE_DEAD || isDead || sGlobal->mainGameLayer->hero->isDead) {
        return;
    }
    
    isScheduled = false;
    
    int prob = rand()%100;
    
    if (prob < 25) {
        doAttack_chongfeng();
        sGlobal->playEffect(se_boss2_attack2);
    }
    else if (prob >= 25 && prob < 50) {
        doAttack_rengmao();
        sGlobal->playEffect(se_boss2_attack2);
    }
    else if (prob >= 50 && prob < 75) {
        doAttack_jiguang();
    }
    else {
        doAttack_reng3mao();
        sGlobal->playEffect(se_boss2_attack2);
    }
    
    spxSprite->setPaused(false);
}

void Boss2::doAttack_rengmao() {
    status = BOSS2_ATK_RENG_MAO;
    
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    enemySprite->setLoopCount(0);
    enemySprite->playAnimation(BOSS2_ANIM_RENG_MAO);
    enemySprite->setAFCSpriteCallback(&callback, this);
}

void Boss2::shootArrow(int no) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    float ox, oy;
    
    float heroVelx = sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x;
    float stime;// = 2.0f;//(480 - 250*0.75f - 120*0.75f - 112*0.75f)/16.0f/4.5f;
    float tv;// = heroVelx - m_box2d->pixel2Meter(stayDist-DP(25)*sGlobal->scaleX)/stime;
    //LOGE("tv:%f, dist:%f, stime:%f", tv, m_box2d->pixel2Meter(stayDist-DP(25)*sGlobal->scaleX), stime);
    
    CaiDao *enemyBullet = new CaiDao(sGlobal->boss2Cache, BOSS2_ANIM_MAO, 0.1f, sGlobal->scaleX*0.75f, sGlobal->scaleY*0.75f, 103);
    enemyBullet->atk = lanceAtk;
    b2Body *bulletBody = enemyBullet->body;

//    no = 3;
    switch (no) {
        case 1:
            stime = randRange(6, 8)/10.0f;//(480 - 250*0.75f - 120*0.75f - 112*0.75f)/16.0f/4.5f;
            tv = heroVelx - m_box2d->pixel2Meter(stayDist-DP(120)*sGlobal->scaleX-DP(25)*sGlobal->scaleX)/stime;
            
            bulletBody->SetGravityScale(0.13f);
            bulletBody->SetLinearVelocity(b2Vec2(tv,4));
            enemyBullet->container->runAction(wyRotateTo::make(2.0f, 20, -50));
            
            ox = m_box2d->pixel2Meter(DP(30)*sGlobal->scaleX);
            oy = m_box2d->pixel2Meter(DP(60)*sGlobal->scaleY);
            break;
        case 2:
            stime = randRange(12, 14)/10.0f;//(480 - 250*0.75f - 120*0.75f - 112*0.75f)/16.0f/4.5f;
            tv = heroVelx - m_box2d->pixel2Meter(stayDist-DP(120)*sGlobal->scaleX-DP(25)*sGlobal->scaleX)/stime;
            
            bulletBody->SetGravityScale(0.2f);
            bulletBody->SetLinearVelocity(b2Vec2(tv,20));
            enemyBullet->container->runAction(wyRotateTo::make(2.0f, 80, -80));
            
            ox = m_box2d->pixel2Meter(DP(30)*sGlobal->scaleX);
            oy = m_box2d->pixel2Meter(DP(60)*sGlobal->scaleY);
            break;
        case 3:
            stime = randRange(5, 8)/10.0f;//(480 - 250*0.75f - 120*0.75f - 112*0.75f)/16.0f/4.5f;
            tv = heroVelx - m_box2d->pixel2Meter(stayDist-DP(120)*sGlobal->scaleX-DP(25)*sGlobal->scaleX)/stime;
            
            bulletBody->SetGravityScale(0);
            bulletBody->SetLinearVelocity(b2Vec2(tv,-5.0f));
            enemyBullet->container->setRotation(-6);
            
            ox = -m_box2d->pixel2Meter(DP(20)*sGlobal->scaleX);
            oy = m_box2d->pixel2Meter(DP(20)*sGlobal->scaleY);
            break;
        default:
            break;
    }
    
    bulletBody->SetTransform(b2Vec2(body->GetPosition().x+ox, body->GetPosition().y + oy), 0);
    enemyBullet->container->setPosition(m_box2d->meter2Pixel(bulletBody->GetPosition().x), m_box2d->meter2Pixel(bulletBody->GetPosition().y));
}

void Boss2::doAttack_chongfeng() {
    status = BOSS2_ATK_CHONGJI_0;
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    enemySprite->setLoopCount(0);
    enemySprite->setAFCSpriteCallback(&callback, this);
    enemySprite->playAnimation(BOSS2_ANIM_CHONGFENGXULI);
}

void Boss2::doChongji() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    enemySprite->setLoopCount(-1);
    enemySprite->setAFCSpriteCallback(NULL, NULL);
    enemySprite->playAnimation(BOSS2_ANIM_CHONGFENG);
    
    sGlobal->mainGameLayer->addToRecreateFixture(this);
    setCurVel(-randRange(8, 15), 0.0f);
    
    status = BOSS2_ATK_CHONGJI_1;
    
    isCongjiInit = true;
    
    s1 = (wySPXSprite *)sGlobal->popFromCache(sGlobal->boss2Cache);//wySPXSprite::make("boss2.sprite", false, MyTextureManager::makePNG("boss2.png"), BOSS2_ANIM_CHONGFENG);
    s1->setForceTickMode(true);
	s1->setUnitInterval(0.2f);
    s1->setLoopCount(-1);
    s1->setScaleX(sGlobal->scaleX * 0.75f);
    s1->setScaleY(sGlobal->scaleY * 0.75f);
    container->addChildLocked(s1);
    s1->setPosition(DP(60)*sGlobal->scaleX, 0);
    s1->setAlpha(0);
    s1->runAction(wySequence::make(wyDelayTime::make(0.25f), wyFadeTo::make(0.05f, 0, 100), NULL));
    s1->playAnimation(BOSS2_ANIM_CHONGFENG);
    
    s2 = (wySPXSprite *)sGlobal->popFromCache(sGlobal->boss2Cache);//wySPXSprite::make("boss2.sprite", false, MyTextureManager::makePNG("boss2.png"), BOSS2_ANIM_CHONGFENG);
    s2->setForceTickMode(true);
	s2->setUnitInterval(0.2f);
    s2->setLoopCount(-1);
    s2->setScaleX(sGlobal->scaleX * 0.75f);
    s2->setScaleY(sGlobal->scaleY * 0.75f);
    container->addChildLocked(s2);
    s2->setPosition(DP(40)*sGlobal->scaleX, 0);
    s2->setAlpha(0);
    s2->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeTo::make(0.05f, 0, 150), NULL));
    s2->playAnimation(BOSS2_ANIM_CHONGFENG);
    
    s3 = (wySPXSprite *)sGlobal->popFromCache(sGlobal->boss2Cache);//wySPXSprite::make("boss2.sprite", false, MyTextureManager::makePNG("boss2.png"), BOSS2_ANIM_CHONGFENG);
    s3->setForceTickMode(true);
	s3->setUnitInterval(0.2f);
    s3->setLoopCount(-1);
    s3->setScaleX(sGlobal->scaleX * 0.75f);
    s3->setScaleY(sGlobal->scaleY * 0.75f);
    container->addChildLocked(s3);
    s3->setPosition(DP(20)*sGlobal->scaleX, 0);
    s3->setAlpha(0);
    s3->runAction(wySequence::make(wyDelayTime::make(0.15f), wyFadeTo::make(0.05f, 0, 200), NULL));
    s3->playAnimation(BOSS2_ANIM_CHONGFENG);
}

void Boss2::doAttack_jiguang() {
    status = BOSS2_ATK_XULI_JIGUANG_0;
    
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    enemySprite->setLoopCount(-1);
    enemySprite->setAFCSpriteCallback(&callback, this);
    enemySprite->playAnimation(BOSS2_ANIM_FASHE_JIGUANG);
    calcJiGuangPosition();
    
    initShootTargets();
    sGlobal->mainGameLayer->addToRecreateFixture(this);
}

static void onFakeDeadEnded(wyAFCSprite* sprite, void* data) {
    Boss2 *boss2 = (Boss2 *)data;
    boss2->throwCoin();
}

void Boss2::fakeDead() {
    status = BOSS_FAKE_DEAD;
    
    instance = NULL;
    
    deadSprite2 = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), SPX_BOSS_DEAD_BOMB);
    sGlobal->mainGameLayer->addChildLocked(deadSprite2, container->getZOrder());
    deadSprite2->setForceTickMode(true);
    deadSprite2->setUnitInterval(0.2f);
    deadSprite2->setLoopCount(0);
    deadSprite2->setFlipX(false);
    deadSprite2->setScaleX(1.3f*sGlobal->scaleX);
    deadSprite2->setScaleY(1.3f*sGlobal->scaleY);
    deadSprite2->setPosition(container->getPositionX() - DP(0), container->getPositionY()+DP(0));
    deadSprite2->setTag(1);
    wyAFCSpriteCallback callback = {
        NULL,
        onFakeDeadEnded
    };
    deadSprite2->setAFCSpriteCallback(&callback, this);
    
    sGlobal->bossEyeZ = 1;
//    bloodSprite->setVisible(false);

    hideHuanying();
    hideJiguang();
    hideShootTarget();
    
    dead();
    
    sGlobal->playEffect(se_boss2_dead);
}

void Boss2::throwCoin() {
    if (sGlobal->isRealEndless) {
        dead();
        return;
    }
    //320
    //10 coins 40
    int x, y;
    for (int i = 0; i < 10; i++) {
        x = sGlobal->mainGameLayer->hero->container->getPositionX() + wyDevice::winWidth + DP(300)*sGlobal->scaleX + DP(80)*i*sGlobal->scaleX;
        y = container->getPositionY() + DP(60)*sGlobal->scaleY;
        BonusCoin *bonus = BonusCoin::make(x, y, COIN, false, true);
    }
    //2 big coins 40
    int x1, y1;
    for (int i = 1; i < 3; i++) {
        x1 = x + DP(200)*i*sGlobal->scaleX;
        y1 = container->getPositionY() + DP(100)*sGlobal->scaleY;
        BonusCoin *bonus = BonusCoin::make(x1, y1, BIGCOIN, false, true);
    }
    //2 bigbig coin 200
    for (int i = 1; i < 3; i++) {
        x1 = x1 + DP(200)*sGlobal->scaleX*i;
        y1 = container->getPositionY() + DP(300)*sGlobal->scaleY;
        BonusCoin *bonus = BonusCoin::make(x1, y1, BIGBIGCOIN, false, true);
    }
    
    EndPoint *endPoint = EndPoint::make(x1 + DP(500)*sGlobal->scaleX, container->getPositionY()-DP(115)*sGlobal->scaleY, true);
    
    dead();
}

void Boss2::calcJiGuangPosition() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    Hero *hero = sGlobal->mainGameLayer->hero;
    dx = body->GetPosition().x - sGlobal->mainGameLayer->hero->body->GetPosition().x + m_box2d->pixel2Meter(DP(200)*sGlobal->scaleX);
    dy = body->GetPosition().y + m_box2d->pixel2Meter(bodyRect.height/3) - sGlobal->mainGameLayer->hero->body->GetPosition().y + m_box2d->pixel2Meter(sGlobal->mainGameLayer->hero->bodyRect.height/2 + DP(200)*sGlobal->scaleY);
    dl = sqrtf(dx*dx + dy*dy);
    angle = atan(dy / dx) - randRange(10, 15)*0.01f;
}

void Boss2::shootJiGuang() {
    sGlobal->playEffect(se_boss2_attack1);
    
    isLighting = true;
    lightingTimer = 0;
    moveLightingTimer = 0;
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    Laser *laser1 = new Laser(sGlobal->boss2Cache);
    laser1->atk = xulilaserAtk;
    
    wySpriteEx* bullet = wySpriteEx::make(MyTextureManager::makePNG("boss2.png"), wyr(DP(578), DP(545), DP(32), DP(32)));
    bullet->setScaleX(m_box2d->meter2Pixel(dl) / DP(32));
    bullet->setScaleY(sGlobal->scaleY);
    bullet->setAnchor(1.0f, 1.0f);
    laser1->container->addChildLocked(bullet);
//    bullet->setAlpha(220);
    
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(body->GetPosition().x - m_box2d->pixel2Meter(DP(50)*sGlobal->scaleX), body->GetPosition().y + m_box2d->pixel2Meter(DP(25)*sGlobal->scaleY));
    lightingBody = m_box2d->getWorld()->CreateBody(&bd);
    lightingBody->SetTransform(bd.position, angle);
    b2PolygonShape box;
    box.SetAsBox(dl/2, m_box2d->pixel2Meter(DP(20/2)*sGlobal->scaleY), b2Vec2(-dl/2, m_box2d->pixel2Meter(-DP(20/2)*sGlobal->scaleY)), 0);
    b2FixtureDef fd;
    fd.shape = &box;
    fd.restitution = 0.0f;
    fd.friction = 0.0f;
    fd.density = 0.0f;
    fd.isSensor = true;
    fd.filter.maskBits = MASK_BITS[TYPE_ENEMY_BULLET];
    fd.filter.categoryBits = CAT_BITS[TYPE_ENEMY_BULLET];
    fd.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY_BULLET];
    lightingBody->CreateFixture(&fd);
    lightingBody->SetUserData(laser1);
    lightingBody->SetGravityScale(0);
    laser1->body = lightingBody;
    lightingBody->SetAngularVelocity(-0.1f);
    if (lightingBody != NULL) {
        lightingBody->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, body->GetLinearVelocity().y));
    }
    

    laser1 = new Laser(sGlobal->boss2Cache);
    laser1->atk = xulilaserAtk;
    
    bullet = wySpriteEx::make(MyTextureManager::makePNG("boss2.png"), wyr(DP(578), DP(545), DP(32), DP(32)));
    bullet->setScaleX(m_box2d->meter2Pixel(dl) / DP(32));
    bullet->setScaleY(sGlobal->scaleY);
    bullet->setAnchor(1.0f, 1.0f);
    laser1->container->addChildLocked(bullet);
//    bullet->setAlpha(220);
    
    bd.type = b2_dynamicBody;
    bd.position.Set(body->GetPosition().x + m_box2d->pixel2Meter(DP(6)*sGlobal->scaleX), body->GetPosition().y + m_box2d->pixel2Meter(DP(25)*sGlobal->scaleY));
    
    lightingBody1 = m_box2d->getWorld()->CreateBody(&bd);
    lightingBody1->SetTransform(bd.position, angle);
    box.SetAsBox(dl/2, m_box2d->pixel2Meter(DP(20/2)*sGlobal->scaleY), b2Vec2(-dl/2, m_box2d->pixel2Meter(-DP(20/2)*sGlobal->scaleY)), 0);
    fd.shape = &box;
    fd.restitution = 0.0f;
    fd.friction = 0.0f;
    fd.density = 0.0f;
    fd.isSensor = true;
    fd.filter.maskBits = MASK_BITS[TYPE_ENEMY_BULLET];
    fd.filter.categoryBits = CAT_BITS[TYPE_ENEMY_BULLET];
    fd.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY_BULLET];
    lightingBody1->CreateFixture(&fd);
    lightingBody1->SetUserData(laser1);
    lightingBody1->SetGravityScale(0);
    laser1->body = lightingBody1;
    
    lightingBody1->SetAngularVelocity(-0.1f);
    if (lightingBody1 != NULL) {
        lightingBody1->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, body->GetLinearVelocity().y));
    }
    
    lightingWidth = 0;
    lightingHeight = 0;
}

void Boss2::initShootTargets() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    isShootTargetHide = false;
    shootFlags[0] = false;
    shootFlags[1] = false;
    shootFlags[2] = false;
    
    time2 = rand()%10/10.0f;
    time3 = rand()%10/10.0f;
    totalTime = time2 + time3 + 2.0f;
    stepTime = time2;
    
    curSeq = POSSIBLE_COMBO[rand()%6];
    curTime = 0;
    stepTimer = 0;
    totalTimer = 0;
    attackFlag = 0;//0:正在进行；1:成功; 2:失败
    //LOGE("init: seq:%d", curSeq);
    generateNextTarget();
}

void Boss2::generateNextTarget() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    switch (curTime) {
        case 0:
            curIndex = curSeq / 100;
            stepTime = time2;
            break;
        case 1:
            curIndex = curSeq / 10 - curSeq / 100 * 10;
            stepTime = time3;
            break;
        case 2:
            curIndex = curSeq % 10;
            break;
        default:
            break;
    }
    stepTimer = 0;
    
    switch (curIndex) {
        case 1:
            target1 = new ShootTarget(body->GetPosition().x, body->GetPosition().y, m_box2d->pixel2Meter(DP(-randRange(0, 30))*sGlobal->scaleX), m_box2d->pixel2Meter(DP(randRange(80, 120))*sGlobal->scaleY), 0, this);
            target1->hp = xulilaserHP;
            break;
        case 2:
            target2 = new ShootTarget(body->GetPosition().x, body->GetPosition().y, m_box2d->pixel2Meter(DP(-randRange(50, 80))*sGlobal->scaleX), m_box2d->pixel2Meter(DP(randRange(30, 60))*sGlobal->scaleY), 1, this);
            target2->hp = xulilaserHP;
            break;
        case 3:
            target3 = new ShootTarget(body->GetPosition().x, body->GetPosition().y, m_box2d->pixel2Meter(-DP(randRange(10, 40))*sGlobal->scaleX), m_box2d->pixel2Meter(-DP(randRange(20, 50))*sGlobal->scaleY), 2, this);
            target3->hp = xulilaserHP;
            break;
        default:
            break;
    }
    //LOGE("generateNextTarget, curTime:%d, curIndex:%d, stepTime:%f", curTime, curIndex, stepTime);
}

void Boss2::attackSuccess() {
    //LOGE("attack success...");
    attackFlag = 1;
    
    if (!shootFlags[0] || !shootFlags[1] || !shootFlags[2]) {
        if (!shootFlags[0] && !shootFlags[1] && !shootFlags[2]) {
//            LOGE("shoot 3");
            //扔3个钩子
            attackFlag = 3;
        }
        else {
//            LOGE("shoot 1");
            //扔1个钩子
            attackFlag = 1;
        }
//        spxSprite->playAnimation(BOSS2_ANIM_ZOULU);
//        spxSprite->setLoopCount(0);
//        wyAFCSpriteCallback callback = {
//            onAFCAnimationFrameChanged,
//            onAFCAnimationEnded
//        };
//        spxSprite->setAFCSpriteCallback(&callback, this);
//        status = BOSS2_ATK_XULI_JIGUANG_1;
        shootJiGuang();
    }
    
    hideShootTarget();
}

void Boss2::attackFailed() {
    attackFlag = -1;

    sGlobal->playEffect(se_boss2_dead);
//    delete[] shootFlags;
//    shootFlags = NULL;
}

void Boss2::onTargetShootEnd(int index) {
    if (isShootTargetHide) {
//        LOGE("shoot target has hidden...");
        return;
    }
    if (attackFlag == 1) {
//        LOGE("boss2 atk success, return...");
        return;
    }
    
    //LOGE("%d has shooted...", index);
    switch (index) {
        case 0:
            target1 = NULL;
            shootFlags[0] = true;
            break;
        case 1:
            target2 = NULL;
            shootFlags[1] = true;
            break;
        case 2:
            target3 = NULL;
            shootFlags[2] = true;
            break;
        default:
            break;
    }
    
    if (shootFlags[0] && shootFlags[1] && shootFlags[2]) {
        attackFailed();
        
        spxSprite->setLoopCount(0);
        wyAFCSpriteCallback callback = {
            NULL,
            onAFCAnimationEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(BOSS2_ANIM_HURT);

//        sGlobal->mainGameLayer->addToRecreateFixture(this);
        status = BOSS2_ATK_XULI_JIGUANG_1;
    }
}

void Boss2::hideShootTarget() {
    isShootTargetHide = true;
    
    if (target1 != NULL) {
//        target1->body->SetAwake(false);
//        target1->body->SetActive(false);
        sGlobal->mainGameLayer->addToDestroy(target1->body);
        
        target1 = NULL;
    }
    if (target2 != NULL) {
//        target2->body->SetAwake(false);
//        target2->body->SetActive(false);
        sGlobal->mainGameLayer->addToDestroy(target2->body);
        
        target2 = NULL;
    }
    if (target3 != NULL) {
//        target3->body->SetAwake(false);
//        target3->body->SetActive(false);
        sGlobal->mainGameLayer->addToDestroy(target3->body);
        
        target3 = NULL;
    }
    
//    if (shootFlags != NULL) {
//        delete[] shootFlags;
//        shootFlags = NULL;
//    }
}

void Boss2::hideHuanying() {
    if (s1 != NULL) {
        s1->stopAllActions();
        s1->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(sGlobal->boss2Cache, s1);
    }
    if (s2 != NULL) {
        s2->stopAllActions();
        s2->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(sGlobal->boss2Cache, s2);
    }
    if (s3 != NULL) {
        s3->stopAllActions();
        s3->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(sGlobal->boss2Cache, s3);
    }
    
    s1 = NULL;
    s2 = NULL;
    s3 = NULL;
}

void Boss2::hideJiguang() {
    if (lightingBody != NULL) {
        BaseObject *obj = (BaseObject*) lightingBody->GetUserData();
        wyNode *sprite = (wyNode *) obj->container;
        
        BaseObject *obj1 = (BaseObject*) lightingBody1->GetUserData();
        wyNode *sprite1 = (wyNode *) obj1->container;
        
        sprite->setVisible(false);
        sprite1->setVisible(false);
//        lightingBody->SetActive(false);
//        lightingBody->SetAwake(false);
//        lightingBody1->SetActive(false);
//        lightingBody1->SetAwake(false);
        
        sGlobal->mainGameLayer->addToDestroy(lightingBody);
        lightingBody = NULL;
        
        sGlobal->mainGameLayer->addToDestroy(lightingBody1);
        lightingBody1 = NULL;
        
        lightingTimer = 0;
        isLighting = false;
    }
}

void Boss2::doAttack_reng3mao() {
    status = BOSS2_ATK_HUIXUAN_MAO_0;
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    enemySprite->setLoopCount(0);
    enemySprite->setAFCSpriteCallback(&callback, this);
    enemySprite->playAnimation(BOSS2_ANIM_ZHAOHUAN);
}

void Boss2::create3mao() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    int degree;// = 20+rand()%30;
    float ox, oy;
    ox = 200+rand()%100;
    oy = rand()%100;
    degree = 10+rand()%20;

    ox = m_box2d->pixel2Meter(DP(ox)*sGlobal->scaleX);
    oy = m_box2d->pixel2Meter(DP(oy)*sGlobal->scaleY);
    
    float scale, aimscale;
    int _hp;
    wyColor4B color;
    //    if (size == 1) {
    //        scale = 1.2f;
    //        aimscale = 1.5f;
    //        _hp = 200;
    //        color = wyc4b(255, 200, 200, 255);
    //    } else {
    scale = 0.8f;
    aimscale = 0.8f;
    _hp = 10;
    color = wyc4b(255, 255, 255, 255);
    //    }
    HuixuanCaidao *enemyBullet = new HuixuanCaidao(sGlobal->boss2Cache, BOSS2_ANIM_XIAO_MAO, 0.1f, scale*sGlobal->scaleX, scale*sGlobal->scaleY, DP(20), DP(20), smalllanceAtk);
    enemyBullet->hp = smalllanceHP;
    enemyBullet->spxSprite->setColor(color);
    enemyBullet->aimContainer->setScale(aimscale);
    enemyBullet->isPhysics = true;
    enemyBullet->needBaozha = false;
    enemyBullet->e_id = ENEMY_NEED_BLOOD;
    
    b2Body *bulletBody = enemyBullet->body;
    bulletBody->SetTransform(b2Vec2(body->GetPosition().x+ox, body->GetPosition().y + oy), 0);
    bulletBody->SetGravityScale(0.08f);
    enemyBullet->container->setPosition(m_box2d->meter2Pixel(bulletBody->GetPosition().x), m_box2d->meter2Pixel(bulletBody->GetPosition().y));

    enemyBullet->spxSprite->setRotation(-degree);
    
    enemyBullet->initialDegree = degree;
//    float speed = 10;
//    float heroVelx = sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x;
//    if (sGlobal->mainGameLayer->hero->isBoosting) {
//        enemyBullet->isCreateInBoost = true;
//    }
//    bulletBody->SetLinearVelocity(b2Vec2(-speed*cos(wyMath::d2r(degree))+heroVelx, 20*sin(wyMath::d2r(degree))));
    enemyBullet->speed = randRange(10, 20);
    bulletBody->SetLinearVelocity(b2Vec2(0, 20*sin(wyMath::d2r(degree))));
}

void Boss2::hurt(float y) {
    Boss::hurt(y);
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    if (status == BOSS2_WALK && !isRecreatingFixture && rand()%5 == 0) {
        wyAFCSpriteCallback callback = {
            NULL,
            onAFCAnimationEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(BOSS2_ANIM_HURT);
        status = BOSS_HURT;
        
//        sGlobal->playEffect(se_boss2_dead);
    }
}




//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "Boss3.h"
#include "Global.h"
#include "EnemyFactory.h"
#include "Doctor1.h"
#include "Doctor2.h"
#include "MuscleZombie.h"
#include "FlyZombie3.h"
#include "WolfZombie.h"
#include "FemaleZombie.h"
#include "Bonus.h"
#include "EndPoint.h"
#include "Barricade.h"
#include "Trap.h"
#include "XuanKongBiao.h"
#include "HeroBullet.h"

Boss3 *Boss3::instance = NULL;

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    Boss3* enemy = (Boss3*)data;
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    if (enemy->status == BOSS3_ATK_TIEQUAN_0) {
        if (sprite->getCurrentFrame() == 2) {
            enemy->shootQuantou();
        }
    }
    else if (enemy->status == BOSS3_ATK_ZA_1 && sprite->getCurrentAnimationIndex() == BOSS3_ANIM_ZA) {
        if (sprite->getCurrentFrame() == 3) {
            sGlobal->mainGameLayer->addToRecreateFixture(enemy);
//            LOGE("BOSS3_ATK_ZA_1 sharyu");
            if (rand()%2 == 0) {
                sGlobal->playEffect(se_boss3_attack1);
            } else {
                sGlobal->playEffect(se_boss3_attack2);
            }
//            enemy->body->SetTransform(b2Vec2(enemy->body->GetPosition().x, enemy->body->GetPosition().y + m_box2d->pixel2Meter(DP(6)*sGlobal->scaleY)), 0);
//            if (!sGlobal->isRealEndless) {
//                enemy->body->SetGravityScale(1.0f);
//            }
        }
    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    Boss3* enemy = (Boss3*)data;
    wySPXSprite *enemySprite = ((wySPXSprite *) enemy->spxSprite);
    if (enemy->status == BOSS3_ATK_TIEQUAN_0) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS3_ANIM_ZOULU);
        enemy->status = BOSS3_WALK;
    }
    else if (enemy->status == BOSS3_ATK_ZA_1) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS3_ANIM_ZOULU);
        enemy->status = BOSS3_ATK_ZA_2;
        enemy->setCurVel(30.0f, 0.0f);
        sGlobal->mainGameLayer->addToRecreateFixture(enemy);
//        LOGE("BOSS3_ATK_ZA_1");
//        enemy->body->SetTransform(b2Vec2(enemy->body->GetPosition().x, enemy->body->GetPosition().y + m_box2d->pixel2Meter(DP(6)*sGlobal->scaleY)), 0);
//        if (!sGlobal->isRealEndless) {
//            enemy->body->SetGravityScale(1.0f);
//        }
    }
    else if (enemy->status == BOSS3_ATK_XULI_TIEQUAN_1) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS3_ANIM_ZOULU);
        enemy->status = BOSS3_WALK;
        sGlobal->mainGameLayer->addToRecreateFixture(enemy);
//        LOGE("BOSS3_ATK_XULI_TIEQUAN_1");
//        enemy->body->SetTransform(b2Vec2(enemy->body->GetPosition().x, enemy->body->GetPosition().y + m_box2d->pixel2Meter(DP(6)*sGlobal->scaleY)), 0);
//        if (!sGlobal->isRealEndless) {
//            enemy->body->SetGravityScale(1.0f);
//        }
    }
    else if (enemy->status == BOSS3_ATK_DAODAN_0) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS3_ANIM_ZOULU);
        enemy->status = BOSS3_WALK;
        
        enemy->isFiringMissle = true;
        enemy->firingMissleTimer = 0;
        enemy->fireTimes = 1;
        enemy->daodanCount = rand()%3+3;
        enemy->createDaoDan();
    }
    else if (enemy->status == BOSS_HURT) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS3_ANIM_ZOULU);
        enemy->status = BOSS3_WALK;
    }
}

Boss3::Boss3(float x, float y) : Boss(x, y){
    instance = this;
    deadType = DEAD_TYPE_BOSS;
    status_idle = BOSS3_WALK;
    
    e_id = ENEMY_BOSS3;

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
        
        ironfistAtk = sGlobal->BOSS3_IRONFIST_ATK[colIndex]* power;
        knockAtk = sGlobal->BOSS3_KNOCK_ATK[colIndex]* power;
        xulifistHP = sGlobal->BOSS3_XULI_FIST_HP[colIndex]* power;
        xulitargetHP = sGlobal->BOSS3_XULI_TARGET_HP[colIndex]* power;
        xuliFistAtk = sGlobal->BOSS3_XULI_FIST_ATK[colIndex]* power;
        missleHP = sGlobal->BOSS3_MISSLE_HP[colIndex]* power;
        missleAtk = sGlobal->BOSS3_MISSLE_ATK[colIndex]* power;
        
//        LOGE("combat round:%d, hp:%d, atk:%d, gold:%d, ironfistAtk:%d, knockAtk:%d, xulifistHP:%d, xulitargetHP:%d, xuliFistAtk:%d, missleHP:%d, missleAtk:%d", sGlobal->combatRound, hp, atk, gold, ironfistAtk, knockAtk, xulifistHP, xulitargetHP, xuliFistAtk, missleHP, missleAtk);
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
        
        ironfistAtk = sGlobal->BOSS3_IRONFIST_ATK[colIndex];
        knockAtk = sGlobal->BOSS3_KNOCK_ATK[colIndex];
        xulifistHP = sGlobal->BOSS3_XULI_FIST_HP[colIndex];
        xulitargetHP = sGlobal->BOSS3_XULI_TARGET_HP[colIndex];
        xuliFistAtk = sGlobal->BOSS3_XULI_FIST_ATK[colIndex];
        missleHP = sGlobal->BOSS3_MISSLE_HP[colIndex];
        missleAtk = sGlobal->BOSS3_MISSLE_ATK[colIndex];
    }
    atk *= sGlobal->ENEMY_ATK_MULTI;
    hp *= sGlobal->ENEMY_HP_MULTI;
    gold *= sGlobal->ENEMY_GOLD_MULTI;
    
    ironfistAtk *= sGlobal->ENEMY_ATK_MULTI;
    knockAtk *= sGlobal->ENEMY_ATK_MULTI;
    xulifistHP *= sGlobal->ENEMY_ATK_MULTI;
    xulitargetHP *= sGlobal->ENEMY_ATK_MULTI;
    xuliFistAtk *= sGlobal->ENEMY_ATK_MULTI;
    missleHP *= sGlobal->ENEMY_ATK_MULTI;
    missleAtk *= sGlobal->ENEMY_ATK_MULTI;
    
    atk = knockAtk;
//    LOGE("diff:%d, e_id:%d, atk:%d, hp:%d, gold:%d, ironfistAtk:%d, knockAtk:%d, xulifistHP:%d, xulitargetHP:%d, xuliFistAtk:%d, missleHP:%d, missleAtk:%d", sGlobal->currentSModeDifficulty, e_id, atk, hp, gold, ironfistAtk, knockAtk, xulifistHP, xulitargetHP, xuliFistAtk, missleHP, missleAtk);

    maxHp = hp;
    sub_type = 100;
    
    obj_type = TYPE_ENEMY;
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();

	spxSprite = (wySPXSprite*)sGlobal->popFromCache(sGlobal->boss3Cache);//wySPXSprite::make("boss3.sprite", false, MyTextureManager::makePNG("boss3.png"), BOSS3_ANIM_DAIJI);
    spxSprite->setForceTickMode(true);
	spxSprite->setUnitInterval(0.2f);
    spxSprite->setLoopCount(-1);
    spxSprite->setScaleX(sGlobal->scaleX * 0.75f);
    spxSprite->setScaleY(sGlobal->scaleY * 0.75f);
    spxSprite->playAnimation(BOSS3_ANIM_DAIJI);
    container->addChildLocked(spxSprite);

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
        baseY = m_box2d->pixel2Meter(y+DP(0)*sGlobal->scaleY);
        body->SetTransform(b2Vec2(m_box2d->pixel2Meter(x), baseY), 0);
    }
//    LOGE("boss3:%f,%f,%f", offsetX, offsetY, m_box2d->meter2Pixel(halfBodyHeightInMeter));
    
    isScheduled = false;

    status = BOSS3_IDLE;

    sGlobal->bossEyeZ = 1;
    
    deadSprite1 = NULL;
    deadSprite2 = NULL;
    
    isHeightInited = false;

    animDead = BOSS3_ANIM_SI;
    animWalk = BOSS3_ANIM_ZOULU;
    
    needDeadBlink = false;
    needStay = true;
    
    if (sGlobal->isRealEndless && sGlobal->isBoostOn) {
        multiY = sGlobal->boostMulti;
    } else {
        multiY = 1.0f;
    }
    
    if (sGlobal->isRealEndless) {
        baseY = container->getPositionY();
    }
    
    isInitToShow = false;
    
    stayDist = wyDevice::winWidth - sGlobal->hero_offset_left-DP(125)*sGlobal->scaleX;
    
    elapsedAtkTime = 0;
    
    target1 = NULL;
    target2 = NULL;
    target3 = NULL;
    shootFlags = new bool[3];
    
    shadowScaleX = 6.5f;
    shadowX = DP(10)*sGlobal->scaleX;
    shadowY = -DP(90)*sGlobal->scaleY;
    
    bossBlood->setPosition(DP(20)*sGlobal->scaleX, DP(125)*sGlobal->scaleY);
    
    spxCacheArray = sGlobal->boss3Cache;
    
    isFiringMissle = false;
    fireTimes = 0;
    firingMissleTimer = 0;
    daodanCount = 0;
    
    faintSpriteScale = 1.5*sGlobal->scaleX;
    faintSpriteX = DP(-10)*sGlobal->scaleY;
    faintSpriteY = DP(100)*sGlobal->scaleY;
}

Boss3::~Boss3() {
    delete [] shootFlags;
    instance = NULL;
}

void Boss3::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    Boss::update(dt);
    
    if (isDead) {
        deadSprite2->tick(dt);
    }
    
    if (status == BOSS3_IDLE && distance < stayDist) {
        spxSprite->playAnimation(BOSS3_ANIM_ZOULU);
        setCurVel(sGlobal->heroVelocityX, 0.0f);
        isMoving = true;
        status = BOSS3_WALK;
        
        sGlobal->playEffect(se_boss3_start);
    }
    
    if (status == BOSS3_ATK_ZA_0 && distance < DP(130) * sGlobal->scaleX) {
        wyAFCSpriteCallback callback = {
            onAFCAnimationFrameChanged,
            onAFCAnimationEnded
        };
        spxSprite->setLoopCount(0);
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(BOSS3_ANIM_ZA);
        setCurVel(sGlobal->heroVelocityX, 0.0f);
        status = BOSS3_ATK_ZA_1;
    }
    
    if (status == BOSS3_ATK_ZA_2) {
        if (distance > stayDist) {
            setCurVel(sGlobal->heroVelocityX, 0.0f);
            status = BOSS3_WALK;
        }
    }
    
    if (status == BOSS3_WALK && !isScheduled && !isDead && !isFiringMissle) {
        scheduleNextAttack();
    }
    
    float heroVelX = sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x;
    if (status != BOSS3_IDLE && fabs(body->GetLinearVelocity().x - relativeVelX - heroVelX ) > 0.0000001f) {
        body->SetLinearVelocity(b2Vec2(heroVelX + relativeVelX, body->GetLinearVelocity().y));
    }
    
    if (status == BOSS3_ATK_XULI_TIEQUAN_0) {
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
            if (firingMissleTimer > 0.8f) {
                firingMissleTimer = 0;
                fireTimes++;
                createDaoDan();
            }
        } else {
            isFiringMissle = false;
        }
    }
}

void Boss3::doAttack() {
    if (status == BOSS3_FAKE_DEAD) {
        return;
    }
    
    isScheduled = false;
    int prob = rand()%100;

    if (prob < 25) {
        doAttack_daodan();
        if (rand()%2 == 0) {
            sGlobal->playEffect(se_boss3_attack1);
        } else {
            sGlobal->playEffect(se_boss3_attack2);
        }
    }
    else if (prob >= 25 && prob < 50) {
        doAttack_xulitiequan();
    }
    else if (prob >= 50 && prob < 75) {
        doAttack_tiequan();
        if (rand()%2 == 0) {
            sGlobal->playEffect(se_boss3_attack1);
        } else {
            sGlobal->playEffect(se_boss3_attack2);
        }
    }
    else {
        doAttack_za();
    }
    
    spxSprite->setPaused(false);
}

void Boss3::doAttack_tiequan() {
    status = BOSS3_ATK_TIEQUAN_0;
    
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    enemySprite->setLoopCount(0);
    enemySprite->setAFCSpriteCallback(&callback, this);
    enemySprite->playAnimation(BOSS3_ANIM_FASHE_TIEQUAN);
}

void Boss3::shootQuantou() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    float ox, oy;
    ox = -m_box2d->pixel2Meter(DP(84)*sGlobal->scaleX);
    oy = m_box2d->pixel2Meter(DP(24)*sGlobal->scaleY);

    CaiDao *enemyBullet = new CaiDao(sGlobal->boss3Cache, BOSS3_ANIM_QUANTOU, 0.1f, sGlobal->scaleX*0.75f, sGlobal->scaleY*0.75f, 103, true);
    enemyBullet->atk = ironfistAtk;
    enemyBullet->isGenZong = false;
    enemyBullet->hasAction = true;
    
    b2Body *bulletBody = enemyBullet->body;
    bulletBody->SetTransform(b2Vec2(body->GetPosition().x+ox, body->GetPosition().y + oy), 0);
    bulletBody->SetGravityScale(0.0f);
    enemyBullet->container->setPosition(m_box2d->meter2Pixel(bulletBody->GetPosition().x), m_box2d->meter2Pixel(bulletBody->GetPosition().y));

    enemyBullet->emitter = (wyParticleSystem*) sGlobal->popFromCache(sGlobal->fire_emitterArray);
    enemyBullet->emitter->setScaleX(sGlobal->scaleX*0.4f);
    enemyBullet->emitter->setScaleY(sGlobal->scaleY*0.4f);
    enemyBullet->container->addChildLocked(enemyBullet->emitter, -1);
    enemyBullet->emitter->setPositionType(GROUPED);
    enemyBullet->emitter->setDirectionAngleVariance(0, 0);
    enemyBullet->emitter->setStartSizeVariance(120, 0);
    enemyBullet->emitter->setLifeVariance(0.2f, 0);
    enemyBullet->emitter->setSpeedVariance(800, 0);
    enemyBullet->emitter->setPosition(0, -DP(3)*sGlobal->scaleY);
    enemyBullet->emitter->setAnchor(0, 0.5f);
    enemyBullet->emitter->resetSystem();

    if (rand()%2 == 0) {
        float r = DP(rand()%120+50)*sGlobal->scaleX;
        float diff_degree = rand()%40+10;
        float moveY = -DP(600)*sGlobal->scaleX*tan(wyMath::d2r(diff_degree));
        wyHypotrochoidConfig m_circle;
        m_circle = wyhcCircle(r, 270, -90+diff_degree, -r/2, r);
        float hypoTime = randRange(1, 2)+0.5f;
        float totalTime = 0.3+hypoTime;
        wyHypotrochoid* a = wyHypotrochoid::make(hypoTime, m_circle);
        a->setAutoRotate(true, 180);
        wyIntervalAction *a1 = wySequence::make(wyMoveBy::make(0.3f, -r/2, 0), a, wyMoveBy::make(4.0f, 0, moveY), NULL);
        enemyBullet->spxSprite->runAction(a1);
        
        a = wyHypotrochoid::make(hypoTime, m_circle);
        a->setAutoRotate(false, 0);
        a1 = wySequence::make(wyMoveBy::make(0.3f, -r/2, 0), a, wyMoveBy::make(4.0f, 0, moveY), NULL);
        enemyBullet->emitter->runAction(a1);
        
        float heroVelx = sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x;
        enemyBullet->container->runAction(wySequence::make(wyMoveBy::make(totalTime, m_box2d->meter2Pixel(totalTime*heroVelx), 0), wyMoveBy::make(4.0f, -m_box2d->meter2Pixel(4.0f*randRange(5, 10)), 0), NULL));
    } else {
        float heroVelx = sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x;
        enemyBullet->spxSprite->setRotation(-6);
        float dx = -m_box2d->meter2Pixel(10.0f*randRange(5, 10));
        float dy = dx/3;
        enemyBullet->container->runAction(wyMoveBy::make(10.0f, dx, dy));
    }
}

void Boss3::doAttack_xulitiequan() {
    status = BOSS3_ATK_XULI_TIEQUAN_0;
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    enemySprite->setLoopCount(-1);
    enemySprite->setAFCSpriteCallback(&callback, this);
    enemySprite->playAnimation(BOSS3_ANIM_XULI_TIEQUAN);

    initShootTargets();
    sGlobal->mainGameLayer->addToRecreateFixture(this);
//    LOGE("doAttack_xulitiequan");
//    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    body->SetTransform(b2Vec2(body->GetPosition().x, body->GetPosition().y + m_box2d->pixel2Meter(DP(6)*sGlobal->scaleY)), 0);
//    if (!sGlobal->isRealEndless) {
//        body->SetGravityScale(1.0f);
//    }
}

void Boss3::shootDaQuantou(int size) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    float ox, oy;
    ox = -m_box2d->pixel2Meter(DP(84)*sGlobal->scaleX);
    oy = -m_box2d->pixel2Meter(DP(10)*sGlobal->scaleY);
    
    float scale, aimscale;
    int _hp;
    wyColor4B color;
    if (size == 1) {
        scale = 1.2f;
        aimscale = 1.5f;
        _hp = xulifistHP;
        color = wyc4b(255, 200, 200, 255);
    } else {
        scale = 1.0f;
        aimscale = 1.2f;
        _hp = xulifistHP/2;
        color = wyc4b(255, 255, 255, 255);
    }
    HuixuanCaidao *enemyBullet = new HuixuanCaidao(sGlobal->boss3Cache, BOSS3_ANIM_QUANTOU, 0.1f, scale*sGlobal->scaleX, scale*sGlobal->scaleY, DP(40), DP(40), atk);
    enemyBullet->hp = _hp;
    enemyBullet->spxSprite->setColor(color);
    enemyBullet->aimContainer->setScale(aimscale);
    enemyBullet->isQuantou = true;
    
    b2Body *bulletBody = enemyBullet->body;
    bulletBody->SetTransform(b2Vec2(body->GetPosition().x+ox, body->GetPosition().y + oy), 0);
    bulletBody->SetGravityScale(0.0f);
    enemyBullet->container->setPosition(m_box2d->meter2Pixel(bulletBody->GetPosition().x), m_box2d->meter2Pixel(bulletBody->GetPosition().y));
    float velx = randRange(0, HERO_VEL_X*2/3);
    if (rand()%2 == 0) {
        velx = velx*-1;
    }
    bulletBody->SetLinearVelocity(b2Vec2(velx, 0));
    
//    float r = DP(rand()%40+130)*sGlobal->scaleX;
//    float diff_degree = rand()%40;
//    float moveY = -DP(200)*sGlobal->scaleX*tan(wyMath::d2r(diff_degree));
//    wyHypotrochoidConfig m_circle;
//    m_circle = wyhcCircle(r, 270, -90+diff_degree, -r/2, r);
//    wyHypotrochoid* a = wyHypotrochoid::make(2.5f, m_circle);
//    a->setAutoRotate(true, 180);
//    wyIntervalAction *a1 = wySequence::make(wyMoveBy::make(0.3f, -r/2, 0), a, wyMoveBy::make(2.0f, 0, moveY), NULL);
//    enemyBullet->spxSprite->runAction(a1);
    
//    float heroVelx = sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x;
//    enemyBullet->container->runAction(wyMoveBy::make(5.0f, m_box2d->meter2Pixel(5.0f*heroVelx/3*2), 0));
    
    enemyBullet->emitterCacheArray = sGlobal->fire_emitterArray;
    enemyBullet->emitter = (wyParticleSystem*) sGlobal->popFromCache(enemyBullet->emitterCacheArray);
    enemyBullet->emitter->setScaleX(sGlobal->scaleX*0.6f);
    enemyBullet->emitter->setScaleY(sGlobal->scaleY*0.6f);
    enemyBullet->container->addChildLocked(enemyBullet->emitter, -1);
    enemyBullet->emitter->setPositionType(GROUPED);
    enemyBullet->emitter->setDirectionAngleVariance(0, 0);
    enemyBullet->emitter->setStartSizeVariance(120, 0);
    enemyBullet->emitter->setLifeVariance(0.2f, 0);
    enemyBullet->emitter->setSpeedVariance(800, 0);
    enemyBullet->emitter->setPosition(DP(0)*sGlobal->scaleX, -DP(3)*sGlobal->scaleY);
    enemyBullet->emitter->setAnchor(0, 0.5f);
    
//    a = wyHypotrochoid::make(2.5f, m_circle);
//    a->setAutoRotate(false, 0);
//    a1 = wySequence::make(wyMoveBy::make(0.3f, -r/2, 0), a, wyMoveBy::make(2.0f, 0, moveY), NULL);
//    enemyBullet->emitter->runAction(a1);
}

void Boss3::doAttack_za() {
    status = BOSS3_ATK_ZA_0;
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    enemySprite->setLoopCount(0);
    enemySprite->setAFCSpriteCallback(&callback, this);
    enemySprite->playAnimation(BOSS3_ANIM_DAIJI);
    
    setCurVel(-12, 0.0f);
}

void Boss3::doAttack_daodan() {
    status = BOSS3_ATK_DAODAN_0;
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    enemySprite->setLoopCount(0);
    enemySprite->setAFCSpriteCallback(&callback, this);
    enemySprite->playAnimation(BOSS3_ANIM_ZHAOHUAN);
}

void Boss3::createDaoDan() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    int degree;// = 20+rand()%30;
    float ox, oy;
    ox = 20+rand()%300;
    if (ox >= 100) {
        oy = 150+rand()%100;
        degree = 10+rand()%20;
    } else {
        oy = 350+rand()%100;
        degree = 30+rand()%20;
    }
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
        scale = 0.5f;
        aimscale = 0.8f;
        _hp = missleHP;
        color = wyc4b(255, 255, 255, 255);
//    }
    HuixuanCaidao *enemyBullet = new HuixuanCaidao(sGlobal->effects_cache, SPX_HUOJIAN, 0.1f, scale*sGlobal->scaleX, scale*sGlobal->scaleY, DP(20), DP(20), missleAtk);
    enemyBullet->hp = _hp;
    enemyBullet->spxSprite->setFlipX(true);
    enemyBullet->spxSprite->setColor(color);
    enemyBullet->aimContainer->setScale(aimscale);
    enemyBullet->isPhysics = true;
    
    b2Body *bulletBody = enemyBullet->body;
    bulletBody->SetTransform(b2Vec2(body->GetPosition().x+ox, body->GetPosition().y + oy), 0);
    bulletBody->SetGravityScale(0.0f);
    enemyBullet->container->setPosition(m_box2d->meter2Pixel(bulletBody->GetPosition().x), m_box2d->meter2Pixel(bulletBody->GetPosition().y));
    
    enemyBullet->emitterCacheArray = sGlobal->smoke_emitterArray;
    enemyBullet->emitter = (wyParticleSystem*) sGlobal->popFromCache(enemyBullet->emitterCacheArray);
    enemyBullet->emitter->setScaleX(sGlobal->scaleX*0.6f);
    enemyBullet->emitter->setScaleY(sGlobal->scaleY*0.6f);
    enemyBullet->container->addChildLocked(enemyBullet->emitter, -1);
    enemyBullet->emitter->setPositionType(GROUPED);
    enemyBullet->emitter->setDirectionAngleVariance(0, 0);
    enemyBullet->emitter->setStartSizeVariance(40, 0);
    enemyBullet->emitter->setLifeVariance(0.5f, 0);
    enemyBullet->emitter->setSpeedVariance(600, 0);
    enemyBullet->emitter->setEmissionRate(20);
    enemyBullet->emitter->resetSystem();
//    enemyBullet->emitter->setPosition(DP(20)*sGlobal->scaleX, DP(15)*sGlobal->scaleY);

//    float r = DP(rand()%40+130)*sGlobal->scaleX;
//    float diff_degree = rand()%40;
//    float moveY = -DP(200)*sGlobal->scaleX*tan(wyMath::d2r(diff_degree));
//    wyHypotrochoidConfig m_circle;
//    m_circle = wyhcCircle(r, 270, -90+diff_degree, -r/2, -r/2);
//    wyHypotrochoid* a = wyHypotrochoid::make(2.5f, m_circle);
//    a->setAutoRotate(true, 180);
//    wyIntervalAction *a1 = wySequence::make(wyMoveBy::make(0.3f, -r/2, -r), a, wyMoveBy::make(2.0f, 0, moveY), NULL);
//    enemyBullet->spxSprite->runAction(a1);
//    
//    a = wyHypotrochoid::make(2.5f, m_circle);
//    a->setAutoRotate(false, 0);
//    a1 = wySequence::make(wyMoveBy::make(0.3f, -r/2, -r), a, wyMoveBy::make(2.0f, 0, moveY), NULL);
//    enemyBullet->emitter->runAction(a1);
//    enemyBullet->spxSprite->runAction(wyMoveBy::make(20.0f, m_box2d->meter2Pixel(20.0f*HERO_VEL_X/3), 0));
    
    enemyBullet->spxSprite->setRotation(-degree);
    enemyBullet->emitter->setRotation(-degree);
    
//    float speed = m_box2d->meter2Pixel(10.0f*HERO_VEL_X/5);
//    enemyBullet->container->runAction(wyMoveBy::make(10.0f, 0, -5*tan(wyMath::d2r(degree))));
    enemyBullet->speed = randRange(15, 20);;
    bulletBody->SetLinearVelocity(b2Vec2(-enemyBullet->speed*cos(wyMath::d2r(degree))+HERO_VEL_X, -enemyBullet->speed*sin(wyMath::d2r(degree))));
    enemyBullet->initialDegree = degree;
}

static void onFakeDeadEnded3(wyAFCSprite* sprite, void* data) {
    Boss3 *boss3 = (Boss3 *)data;
    boss3->throwCoin();
}

void Boss3::fakeDead() {
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
        onFakeDeadEnded3
    };
    deadSprite2->setAFCSpriteCallback(&callback, this);
    
    sGlobal->bossEyeZ = 1;
//    bloodSprite->setVisible(false);
    
    hideShootTarget();
    
    dead();
    
    sGlobal->playEffect(se_boss2_dead);
}

void Boss3::throwCoin() {
    if (sGlobal->isRealEndless) {
        dead();
        return;
    }
    //15 coins
    int x, y;
    for (int i = 0; i < 15; i++) {
        x = sGlobal->mainGameLayer->hero->container->getPositionX() + wyDevice::winWidth + DP(300)*sGlobal->scaleX + DP(60)*i*sGlobal->scaleX;
        y = container->getPositionY() + DP(60)*sGlobal->scaleY;
        BonusCoin::make(x, y, COIN, false, true);
    }
    //2 big coins
    int x1, y1;
    for (int i = 1; i < 3; i++) {
        x1 = x + DP(200)*i*sGlobal->scaleX;
        y1 = container->getPositionY() + DP(100)*sGlobal->scaleY;
        BonusCoin::make(x1, y1, BIGCOIN, false, true);
    }
    //2 bigbig coin
    for (int i = 1; i < 3; i++) {
        x1 = x1 + DP(200)*sGlobal->scaleX*i;
        y1 = container->getPositionY() + DP(300)*sGlobal->scaleY;
        BonusCoin::make(x1, y1, BIGBIGCOIN, false, true);
    }
    
    EndPoint *endPoint = EndPoint::make(x1 + DP(500)*sGlobal->scaleX, container->getPositionY()-DP(90)*sGlobal->scaleY, true);
    
    dead();
}

void Boss3::initShootTargets() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    isShootTargetHide = false;
    shootFlags[0] = false;
    shootFlags[1] = true;
    shootFlags[2] = false;
    
    time2 = 0;//rand()%10/10.0f;
    time3 = 0;//rand()%10/10.0f;
    totalTime = time2 + time3 + 4.0f;
    stepTime = time2;
    
    curSeq = POSSIBLE_COMBO[rand()%6];
    curTime = 0;
    stepTimer = 0;
    totalTimer = 0;
    attackFlag = 0;//0:正在进行；1:成功; 2:失败
    //LOGE("init: seq:%d", curSeq);
    generateNextTarget();
}

void Boss3::generateNextTarget() {
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
            target1 = new ShootTarget(body->GetPosition().x, body->GetPosition().y, m_box2d->pixel2Meter(DP(35)*sGlobal->scaleX), m_box2d->pixel2Meter(DP(80)*sGlobal->scaleY), 0, this);
            target1->hp = xulitargetHP;
            break;
        case 2:
//            target2 = new ShootTarget(body->GetPosition().x, body->GetPosition().y, m_box2d->pixel2Meter(DP(-10)*sGlobal->scaleX), m_box2d->pixel2Meter(DP(50)*sGlobal->scaleY), 1, this);
            target2 = NULL;
            shootFlags[1] = true;
            break;
        case 3:
            target3 = new ShootTarget(body->GetPosition().x, body->GetPosition().y, m_box2d->pixel2Meter(-DP(75)*sGlobal->scaleX), m_box2d->pixel2Meter(DP(10)*sGlobal->scaleY), 2, this, 40, 40);
            target3->aimContainer->setScale(1.5f);
            target3->hp = xulifistHP;
            break;
        default:
            break;
    }
    //LOGE("generateNextTarget, curTime:%d, curIndex:%d, stepTime:%f", curTime, curIndex, stepTime);
}

void Boss3::attackSuccess() {
    //LOGE("attack success...");
    attackFlag = 1;
    
    if (!shootFlags[0] || !shootFlags[1] || !shootFlags[2]) {
        if (!shootFlags[0] && !shootFlags[2]) {
//            LOGE("shoot 3");
            //扔3个钩子
            attackFlag = 3;
            xuliquan_size = 1;
        }
        else {
//            LOGE("shoot 1");
            //扔1个钩子
            attackFlag = 1;
            xuliquan_size = 0;
        }

        spxSprite->setLoopCount(0);
        wyAFCSpriteCallback callback = {
            onAFCAnimationFrameChanged,
            onAFCAnimationEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(BOSS3_ANIM_FASHE_TIEQUAN_1);
        status = BOSS3_ATK_XULI_TIEQUAN_1;
        
        shootDaQuantou(xuliquan_size);
        
        if (rand()%2 == 0) {
            sGlobal->playEffect(se_boss3_attack1);
        } else {
            sGlobal->playEffect(se_boss3_attack2);
        }
    }
    
    hideShootTarget();
}

void Boss3::attackFailed() {
    attackFlag = -1;
    
    sGlobal->playEffect(se_boss3_dead);
//    delete[] shootFlags;
//    shootFlags = NULL;
}

void Boss3::onTargetShootEnd(int index) {
    if (isShootTargetHide) {
//        LOGE("boss3 shoot target has hidden...");
        return;
    }
    if (attackFlag == 1) {
//        LOGE("boss3 atk success, return...");
        return;
    }
    
//    LOGE("%d has shooted...", index);
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
//    LOGE("flags:%d,%d,%d...",shootFlags[0],shootFlags[1],shootFlags[2]);
    if (shootFlags[0] && shootFlags[1] && shootFlags[2]) {
//        LOGE("atk failed...");
        attackFailed();

        spxSprite->setLoopCount(0);
        wyAFCSpriteCallback callback = {
            NULL,
            onAFCAnimationEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(BOSS3_ANIM_FASHE_FAILED);
        status = BOSS3_ATK_XULI_TIEQUAN_1;
    } else {
        if (index == 2) {
            spxSprite->playAnimation(BOSS3_ANIM_FASHE_JIPO_TIEQUAN);
        }
    }
}

void Boss3::hideShootTarget() {
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

void Boss3::hurt(float y) {
    Boss::hurt(y);
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    if (status == BOSS3_WALK && !isRecreatingFixture && rand()%5 == 0) {
//        LOGE("hurt...");
        wyAFCSpriteCallback callback = {
            NULL,
            onAFCAnimationEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(BOSS3_ANIM_HURT);
        status = BOSS_HURT;
        
//        sGlobal->playEffect(se_boss3_dead);
    }
}

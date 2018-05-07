
//  MyContactListener.h
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//
#pragma once

#include "WiEngine.h"
#include "Enemy.h"
#include "Boss1.h"


//b 1. 普通扔标枪，标枪可被击毁
//b 4. 蓄力发射激光，直线闪0.5s，眼部，头部，可打断。
//b 5. 冲锋
//b 7. 召唤2－3个小标枪，屏幕右侧飞入，标枪可被击毁。

const static int BOSS2_IDLE = 0;
const static int BOSS2_ATK_RENG_MAO = 2;//扔长矛
const static int BOSS2_ATK_CHONGJI_0 = 30;//停住
const static int BOSS2_ATK_CHONGJI_1 = 31;//冲
const static int BOSS2_ATK_CHONGJI_2 = 32;//回去
const static int BOSS2_ATK_XULI_JIGUANG_0 = 40;//蓄力激光
const static int BOSS2_ATK_XULI_JIGUANG_1 = 41;//蓄力激光
const static int BOSS2_ATK_HUIXUAN_MAO_0 = 50;//小矛
const static int BOSS2_WALK = 100;
const static int BOSS2_FAKE_DEAD = 101;

const static int BOSS2_ANIM_DAIJI = 0;
const static int BOSS2_ANIM_ZOULU = 1;
const static int BOSS2_ANIM_FASHE_JIGUANG = 2;
const static int BOSS2_ANIM_RENG_MAO = 3;

const static int BOSS2_ANIM_MAO = 4;
const static int BOSS2_ANIM_ZHAOHUAN = 5;
const static int BOSS2_ANIM_CHONGFENG = 6;

const static int BOSS2_ANIM_HURT = 7;
const static int BOSS2_ANIM_SI = 8;

const static int BOSS2_ANIM_CHONGFENGXULI = 9;

const static int BOSS2_ANIM_XIAO_MAO = 10;

const static int BOSS2_ANIM_JIGUANG = 11;
//class Boss2 : public Boss {
//    
//public:
//    Boss2(float x, float y);
//    ~Boss2();
//    
//    void update(float dt);
//    void handleCollision(b2Body *actorB);
//    void endContact(b2Body *actorB);
//    void beginContact(b2Body *actorB);
//    
//    static Boss2 *make(float x, float y) {
//        return WYNEW Boss2(x, y);
//    }
//    
//    int currentIndex;
////    int idleAnimIndex, fightAnim1Index, fightAnim2Index, walkAnimIndex;
//    
//    float toRightDistance;
//    
//    float minCD, maxCD;
//    void doAttack(wyTargetSelector* ts);
//    void scheduleNextAttack();
//    wyTimer *attackTimer, *atk3Timer;
//    
//    void doAttack1();
//    void doAttack2();
//    void doAttack3();
//    void doAttack4();
//    void doAttack5();
//    void doAttack6();
//    void shootArrow(float x, float y);
//    void shootShitou();
//    void summonEnemy();
//    void doChongji();
//    void doChongjiBack(wyTargetSelector* ts);
//
//    int status;
//    bool isScheduled;
//    int atk3_times;
//    
//    void atk3ShootArrow(wyTargetSelector* ts);
//    float hurtByFireTimer;
//    
//    void dead();
//    void fakeDead();
//    bool isFlying;
//    float baseY;
//    
//    int summon_times;
//    void doSummon(wyTargetSelector* ts);
//    void waitForAWhile(wyTargetSelector* ts);
//    
//    wySprite *hpBar;
//    float maxHpScale;
//    
//    void hurt(float y);
//    void dealHitByDart(float y, b2Body *bulletBody);
//    void dealHitByLiudan(b2Body *bulletBody);
//    bool canBeHurt();
//    void destroyMyself();
//    
//    wySPXSprite *shieldSprite, *deadSprite1, *deadSprite2, *shanDianSprite;
//    void showShield();
//    void throwCoin();
//    void deadStep2();
//    void deadStep3();
//    
//    static Boss2 *instance;
//    bool isVSShowed;
//    
//    bool canBeMissed();
//    void calcJiGuangPosition();
//    float dx, dy, dl, angle;
//    b2Body *lightingBody;
//    void shootJiGuang();
//    
//    bool isLighting;
//    float lightingTimer;
//    float lightingWidth, lightingHeight;
//    
//    float moveLightingTimer;
//    float deltaAngle;
//
//    void summonShiTou();
//    
//    float relativeVelX;
//    void setCurVel(float velX, float velY);
//    
//    int preAtkId;
//    
//    void summonShisun();
//    
//    bool isHurtByDabiao;
//    float hurtByDabiaoTimer;
//    void dealHitByDaBiao(float y, b2Body *bulletBody);
//    
//    float shanDianTimer;
//    
//    float stayDist;
//    int atk1, atk2;
//    
//    float elapsedAtkTime, nextTime;
//    float jiguangTime;
//    
//    void initToShow();
//    
//    wyProgressTimer *bossBlood;
//    void setBossHPBar(float percent);
//};

class Boss2 : public Boss {
public:
    Boss2(float x, float y);
    ~Boss2();
    
    void update(float dt);
    
    static Boss2 *make(float x, float y) {
        return WYNEW Boss2(x, y);
    }
    
    void doAttack();
    
//    void doAttackById(int idx);
    
    void fakeDead();
    
    void throwCoin();
    
    static Boss2 *instance;
    
    wySPXSprite *deadSprite1, *deadSprite2;
    void shootChangMao(int no);
    
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
    void hideShootTarget();
    bool isShootTargetHide;
    
    void calcJiGuangPosition();
    float dx, dy, dl, angle;
    b2Body *lightingBody, *lightingBody1;
    void shootJiGuang();
    
    bool isLighting;
    float lightingTimer;
    float lightingWidth, lightingHeight;
    
    float moveLightingTimer;
    float deltaAngle;
    float jiguangTime;
    
    void doChongji();

    void shootArrow(int no);
    
    void doAttack_jiguang();
    void doAttack_chongfeng();
    void doAttack_rengmao();
    void doAttack_reng3mao();
    
    void onTargetShootEnd(int index);
    
    bool isCongjiInit;
    wySPXSprite *s1, *s2, *s3;
    
//    void destroyMyself();
    void hideHuanying();
    void hideJiguang();
    
    void create3mao();
    bool isFiringMissle;
    float firingMissleTimer, firingMissleInterval;
    int fireTimes;
    int daodanCount;
    
    void hurt(float y);
    
    int lanceAtk, chongfengAtk, xulilaserHP, xulilaserAtk, smalllanceHP, smalllanceAtk;
};

class Laser : public EnemyBulletBase {
public:
    
    Laser(wyArray *p_spxCacheArray) : EnemyBulletBase() {
        
        b_id = 1;
        
        spxCacheArray = p_spxCacheArray;
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        int e_id = ENEMY_XIANJING3;
        
        hp = 1;
        atk = 0;
        gold = 0;
        
        obj_type = TYPE_ENEMY_BULLET;
        
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(container, 103);
        
        needDestoryMe = false;
        
        spxSprite = NULL;
    }
    
    ~Laser() {
        
    }
    
    void update(float dt) {
//        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//        
//        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
        if (needDestoryMe) {
            sGlobal->mainGameLayer->addToDestroy(body);
            needDestoryMe = false;
        }
    }

    void destroyMyself() {
//        LOGE("destory toun...");
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);

        if (spxSprite != NULL) {
            spxSprite->stopAllActions();
            spxSprite->setAFCSpriteCallback(NULL, NULL);
            spxSprite->setPaused(false);
            sGlobal->pushToCache(spxCacheArray, spxSprite);
        }
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
            
        delete this;
        
        return;
    }
    
    void onHitHero() {
        b2Fixture *fixture = body->GetFixtureList();
        b2Fixture *next;
        while (fixture != NULL) {
            next = fixture->GetNext();
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = filter.maskBits ^ CAT_BITS[TYPE_HERO_HURT];
            fixture->SetFilterData(filter);
            fixture = next;
        }
    }
    
    bool needDestoryMe;
    void destoryMe() {
        sGlobal->mainGameLayer->addToDisable(body);
        container->setVisible(false);
        needDestoryMe = true;
    }
};

//
//  MyContactListener.h
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//
#pragma once

#include "WiEngine.h"
#include "BaseObject.h"

class HeroIllusion : public wyNode {
public:
    HeroIllusion();
    
    ~HeroIllusion();
    
    static HeroIllusion *make() {
        HeroIllusion *h = WYNEW HeroIllusion();
        h->autoRelease();
        return h;
    }
    
    wySPXSprite *body, *l_leg, *r_leg, *body1;
    void syncAnimation(int animIdx1, int animIdx2, int animIdx3, int texId);
};

class BodySensor : public BaseObject {
    
public:
    BodySensor(float x, float y);
    
    ~BodySensor();
    
    static BodySensor *make(float x, float y) {
        return WYNEW BodySensor(x, y);
    }
    
    void handleCollision(b2Body *actorB);
};

class HeroHurtSensor : public BaseObject {
    
public:
    HeroHurtSensor(float x, float y);
    
    ~HeroHurtSensor();
    
    static HeroHurtSensor *make(float x, float y) {
        return WYNEW HeroHurtSensor(x, y);
    }
    
    void handleCollision(b2Body *actorB);
};

class GroundSensor : public BaseObject {
    
public:
    GroundSensor();
    
    ~GroundSensor();
    
    static GroundSensor *make() {
        return WYNEW GroundSensor();
    }
    
    void handleCollision(b2Body *actorB);
    
    wyArray *groundArray;
    
    void update(float dt);
    
    bool isContacted;
    
    float deltaTime;
};

class Robert {
public:
    wySpriteEx *robert_0, *robert_1, *robert_2, *robert_3, *robertGun;
    int robertStatus;
    float robertTimer, robertFireTimer;
    wyNode *robertContainer, *outerContainer;

    Robert();
    ~Robert();
    
    void tick(float dt);
    
    float tempX, tempY;
    bool isWaitingFire;
    
    bool isBlinking, isBlink;
    float blinkTimer;
    int blinkTime;
    void blink();
};

class LiuDan : public BaseObject {
public:
    bool m_isMoto;
    float elapsedTime;
    bool isFading;
    LiuDan(float baseX, float baseY, bool isMoto, int seq = 1);
    ~LiuDan();
    wySPXSprite *deadSprite, *smokeSprite;
    
    void update(float dt);
    void handleCollision(b2Body *actorB);
    void destroyMe();
    
    float initialY;
    bool isBoosted;
    wyParticleSystem *bullet_emitter;
};

class HeroBullet;

class Hero : public BaseObject {
public:
    Hero(wyHashSet* HeroDef, bool needAutoScroll);
    ~Hero();
    
    void update(float dt);
    
    void shoot();
    void hurt(int atk) {
        hurt(atk, false);
    }
    void hurt(int atk, bool needBlood);
    bool isHurt, isHurtRot;
    float hurtTimer, hurtRotTimer;

//    wySPXSprite *bigFire;
//    b2Body* bigFireBody;
//    bool isFire;
//    float fireTimer;
    void doSkill();
    void doRealSkill();
    void doFireFury();
    void doIllusion();
    void doSandShield();
    
    bool isDead;
    void doDeadLogic(wyTargetSelector* t, int data);
    
    void handleCollision(b2Body *actorB);
    void endContact(b2Body *actorB);
    void beginContact(b2Body *actorB);
    
    bool isSlowed;
    
    wyTimer *timer;
    
    void doEndLogic(wyTargetSelector* ts, void *data);
    void showWinDialog(int needForceNext = 0, void *data = NULL);
    void doEndAnimation(wyTargetSelector* ts, void *data);
    
    void playHeroAnimation(int index);
    int prevAnimIndex;
    int prevLoopCount;
    wySPXSprite *jumpBomb;
    
    wySpriteEx *shadow;
    bool isAlreadyOnGround;
    
    wySPXSprite *bloodSprite;
    
    bool isShaking, isStrongShaking;
    float shakingTimer, strongShakingTimer;
//    void shake();
    
    bool isRunning;
    bool isFHing;
    
    bool isIllusionActive, isSandShieldActive;
    bool isIllusionGoning, isSandShieldGoning, isSSFaded;
    float illusionTimer, sandShieldTimer;
    wySPXSprite *illusion, *sandShield;
    
    void fireDart(float x, float y, float scale, float velX = -1, float velY=0.0f, float atkPercent = 1.0f);
    void sandShieldGone();
    
    int bigFireKills;
    int combo;
    bool hasShoot;
    
    int saveType;
    bool isSavingMe, isWudi, needSaveMe, isSaveMeInited;
    float savingMeTimer, wudiTimer;
    
    bool isYingReleased, hasYing, isYingReleasing;
    float yingReleaseTimer;
    void hideYing();
    wyNode *ying;
    wySPXSprite *yingSprite;
    wyParticleSystem *particle_ying;
    
    wyMotionStreak *m_streak1;
    float streakTimer;
    bool isStreaking;
    
    bool isFlying, isDeFlying, isStartFlyingEnd, isFlyingPaused;
    bool isFlyingSlowDown;
    void cancelFlyFromSlowDown();
    void doFly(bool needSE = true);
    float flyTimer;
    
    bool isBoosting, isDeBoosting;
    void doBoost();
    float boostTimer, deBoostTimer;
    
    bool isBackingToNormal;
    float curVelX;
    float backToNormalSpeed;
    
    void doBiao();
    
    void doFadeEffect();
    float heroZ;
    
    b2Fixture* motoFixture;
    void destroyMotoFixture();
    
    b2Fixture* shieldFixture;
    void destroyShieldFixture();
    
    float hurtByCZTimer;
    bool isHurtByChongZhuang;
    
    void doXuePing();
    void doTool(int toolIndex);
    void doBonus(int bonusIndex);
    void recoverHPWhenUseItem();
    
    wyParticleSystem *boost_emitter, *fly_emitter, *heal_emitter, *speedline_emitter;
    
    wySPXSprite *zhanshen;
    bool isZhanShenShowing, isFiringMissle;
    float zhanshenDist;
    float firingMissleTimer;
    int fireTimes;
    
    float baseY;
    
    void reachCheckPoint();
    
    void stop();
    void pause();
    void run();
    void dealDeadInGuide();
    void dealEndInGuide();
    
    void showFaZhen();
    bool isShowingFaZhen, isFaZhenEnded;
    wySPXSprite *fazhen;
    
    bool isHealing;
    float healTimer;
    void showHealEffect();
    
    bool needSetGravity;
    
    wySPXSprite *shandian;
    
    bool needSetSpeed;
    
	float flyDuration, wudiDuration;
    
    int tempFlag;
    
    float dist266, normalDeathY, flyDeathY;
    
    wyParticleSystem *buff_emitter;
//    wySPXSprite *shieldSprite;
    
    wyNode *dodgeContainer;
    wySpriteEx *dodgeTitle;
    wySPXSprite *dodgeSprite0_body, *dodgeSprite1_body, *dodgeSprite2_body;
    wySPXSprite *dodgeSprite0_l_leg, *dodgeSprite1_l_leg, *dodgeSprite2_l_leg;
    wySPXSprite *dodgeSprite0_r_leg, *dodgeSprite1_r_leg, *dodgeSprite2_r_leg;
    void showDodge();
    
    float recoverHPTimer;
    void recoverHP(float dt);
    void showRecoverHPEffect(int recoverHP, bool hasHud = true);
    
//    wySPXSprite *sprite_mao_tou, *sprite_mao_foot;
    wySPXSprite *spx_hero_body, *spx_hero_r_foot, *spx_hero_l_foot;
    int breakAnimIndex;
    
    wySpriteEx *robertHead, *robertGun;
    int robertStatus;
    float robertTimer, robertFireTimer;
    
    wySPXSprite *firefire;
    
    Robert *robert;
    
    BodySensor *bodySensor;
    void createBodySensor();
    bool isBodySensorCreated;
    
    bool isDBJumping, isDBJumpAnimating;
    float previousY;
    
    void createMotoParts();
    void showMoto();
    void hideMoto();
    wySPXSprite *spx_moto_body, *spx_moto_b_wheel, *spx_moto_f_wheel, *spx_moto_speed_line;
    wyNode *moto_b_wheel_container, *moto_f_wheel_container;
    bool isMotoing;
    
    bool isShooting;
    void setHeadRotation(float angel, bool isShoot = false, bool isHurt = false);
    float oy1, oy2, oy3, oy4, oy5, oy6;
    
    int spxBodyIdleIndex, spxBodyFireIndex, spxSiIndex, spxFHIndex;

    void showLaser();
    wyNode *laserContainer;
    wySPXSprite *laser, *laser1, *laserLight, *laserLight1;
    float deltaX, deltaY;
    int laserCount;
    float laserX, laserY, laserofx, laserofy;
    HeroBullet *laserBullet;
    bool isShootingLaser;
    float laserTimer;
    
    wyNode *shieldContainer;
    wySPXSprite *shieldSprite_0, *shieldSprite_1, *shieldSprite_2;
    void showShield();

    float questRecordTimer;
    void checkQuest();
    
    bool isMotoRunning;
    LiuDan *motoBody;
    bool needRepeat;
    
    bool needDestroy;
    int contactNum;
    
    bool isInDead;
    bool isDisablingContact;
    
    void doContactLogic();
    void onFHDelayEnd(wyTargetSelector* ts);
    
    void switchWeapon(int index);
    void setCurrentWeaponAnimIds(int index);
    
    HeroHurtSensor *hurtSensor;
    void createHeroHurtSensor();
    bool isHeroHurtSensorCreated;
    
    void initShieldEffect();
    void initDodgeEffect();
    void initLaser();
    
    HeroIllusion *illusion1, *illusion2, *illusion3;
    void onDodgeEnd(wyTargetSelector* ts);

    int liudanSeq;
    bool isFiringLiudan;
    int fireLiudanTime;
    float fireLiudanTimer, fireLiudanInterval;
    
    wyRect bulletNormalRect, bulletRPGRect, bulletRobotRect, bulletMP5A5Rect, bulletAK47Rect, bulletHYDRARect, bulletF1Rect, bulletE3Rect, bulletDS2Rect, bulletTORPEDORect, bulletBLASTINGRect;
    
    void changeBody(int fromWeapon, int toWeapon);
    int getWeaponTexIndex(int weaponId);
    
    void showGunLight(int weaponIndex);
    wyNode *gunLightContainer;
    wySPXSprite *gunLight;
    bool isShowingGunLight;
    float gunLightofx, gunLightofy;
    
    bool isBaseVertInited;
    float baseVert;
};

static const int R_GONE = 0;
static const int R_APPEARING = 1;
static const int R_FIRING = 2;
static const int R_DISAPPEARING = 3;

static void removeMyselfonAnimEnded(wyAction* action, void* data) {
    wyNode* node = (wyNode *) data;
    node->getParent()->removeChildLocked(node, true);
}

static void hideMyselfonAnimEndedAFC(wyAFCSprite* sprite, void* data) {
    sprite->setVisible(false);
}

class Missle : public BaseObject {
public:
    float elapsedTime;
    bool isFading;
    Missle(float baseX, float baseY, int i);
    ~Missle();
    wySPXSprite *deadSprite;
    
    void update(float dt);
    void handleCollision(b2Body *actorB);
    wyParticleSystem *bullet_emitter;
    
    void destroyMyself();
};





//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "FlyZombie2.h"
#include "Global.h"
#include "EnemyFactory.h"

FlyZombie2::FlyZombie2(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_FLY_NORMAL, objectsGroup, obj, myCmd){
    e_id = ENEMY_KULOUSHIBINGFLY;
    this->appearType = APPEAR_FLY_NORMAL;
    
	sub_type = 100;
    deadSFX = se_devourer_dead;
    
    animIdle = 15;
    animWalk = 16;
    animWalkBack = 17;
    animHurt = 22;
    animAtk = 19;
    
    animJump = -2;
    animLanding = -1;
    animDead = -3;
    
    isFighting = false;
    fightingTimer = 0;

    needDeadBlink = false;
    
    unitInterval = 0.15f;
    myScaleX = sGlobal->scaleX*0.7f;
    myScaleY = sGlobal->scaleY*0.7f;
    spxCacheArray = sGlobal->enemy_2_cache;
    
    afterInit(x, y);
    
    body->SetGravityScale(0);
    
    xuecaoSx = 0.48f*sGlobal->scaleX;
    xuecaoOfx = -DP(0)*sGlobal->scaleX;
    xuecaoOfy = DP(56)*sGlobal->scaleY;
}

FlyZombie2::~FlyZombie2() {
    
}

void FlyZombie2::update(float dt) {
    Enemy::update(dt);
    
    if (isDead) {
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    if (needAttack && !isFighting && !isStunning) {
        if (distance < DP(300)*sGlobal->scaleX && distance > 0) {
            isAttacking = true;
            doShootArrowAnim();
        }
    }
}

void FlyZombie2::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

void FlyZombie2::beginContact(b2Body *actorB) {
    Enemy::beginContact(actorB);
}

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    FlyZombie2* enemy = (FlyZombie2*)data;
    //enemy->createFixtureByCurrentFrame();
    if (enemy->isDead) {
        return;
    }
    
    if (sprite->getCurrentFrame() == 2) {
        enemy->shootArrow();
    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    //FlyZombie2* enemy = (FlyZombie2*)data;
    FlyZombie2* enemy = (FlyZombie2*)data;
    if (enemy->isDead) {
        return;
    }
    wySPXSprite *enemySprite = ((wySPXSprite *) enemy->spxSprite);
    enemySprite->setAFCSpriteCallback(NULL, NULL);
    enemySprite->setLoopCount(-1);
    enemySprite->playAnimation(enemy->animIdle);
    
    enemy->isAttacking = false;
}

void FlyZombie2::doShootArrowAnim() {
    isFighting = true;

    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    enemySprite->setUnitInterval(unitInterval);
    enemySprite->setLoopCount(0);
    enemySprite->setAFCSpriteCallback(&callback, this);
    enemySprite->playAnimation(animAtk);
    
    enemySprite->setFlipX(false);
}

void FlyZombie2::shootArrow() {
    Bomb *bomb = Bomb::make(container->getPositionX()-DP(20)*sGlobal->scaleX, container->getPositionY()-DP(40)*sGlobal->scaleX);
    bomb->atk = this->atk;
}


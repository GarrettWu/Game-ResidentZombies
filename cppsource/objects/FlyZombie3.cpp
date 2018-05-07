//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "FlyZombie3.h"
#include "Global.h"
#include "EnemyFactory.h"
#include "FlyZombie1.h"

FlyZombie3::FlyZombie3(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_FLY_NORMAL, objectsGroup, obj, myCmd){
    e_id = ENEMY_HONGYIWUSHIFLY;
    this->appearType = APPEAR_FLY_NORMAL;
    
	sub_type = 100;
    deadSFX = se_devourer_dead;
    
    animIdle = 15;
    animWalk = 16;
    animWalkBack = 17;
    animHurt = 22;
    animAtk = 18;
    
    animJump = -2;
    animLanding = -1;
    animDead = -3;
    
    isFighting = false;
    fightingTimer = 0;
    
    needDeadBlink = false;
    
    shootTime = 0;
    maxTime = rand()%3 + 1;
    
    if (sGlobal->isRealEndless && sGlobal->isBoostOn) {
        multiY = sGlobal->boostMulti;
    } else {
        multiY = 1.0f;
    }
    
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

FlyZombie3::~FlyZombie3() {
    
}

void FlyZombie3::update(float dt) {
    Enemy::update(dt);
    
    if (isDead) {
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (needAttack && !isFighting && !isStunning) {
        if (distance < wyDevice::winWidth*0.8f && distance > 0) {
            isAttacking = true;
            shootTime = 0;
            doShootArrowAnim();
        }
    }
}

void FlyZombie3::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

void FlyZombie3::beginContact(b2Body *actorB) {
    Enemy::beginContact(actorB);
}

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    FlyZombie3* enemy = (FlyZombie3*)data;
    if (enemy->isDead) {
        return;
    }
    //enemy->createFixtureByCurrentFrame();
    
    if (sprite->getCurrentFrame() == 2) {
        enemy->shootArrow();
        enemy->shootTime++;
    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    //FlyZombie3* enemy = (FlyZombie3*)data;
    FlyZombie3* enemy = (FlyZombie3*)data;
    if (enemy->isDead) {
        return;
    }
    
    if (enemy->shootTime < enemy->maxTime) {
        enemy->doShootArrowAnim();
    } else {
        wySPXSprite *enemySprite = ((wySPXSprite *) enemy->spxSprite);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->setLoopCount(-1);
        enemySprite->playAnimation(enemy->animIdle);
        
        enemy->isAttacking = false;
    }
}

void FlyZombie3::doShootArrowAnim() {
    isFighting = true;
    isAttacking = true;

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

void FlyZombie3::shootArrow() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    EnemyBullet *enemyBullet = EnemyBullet::make(sGlobal->enemy_2_cache, 20, 0.1f, sGlobal->scaleX*0.7f, sGlobal->scaleY*0.7f);
    enemyBullet->atk = atk;
    enemyBullet->sub_type = 0;
    b2Body *bulletBody = enemyBullet->body;
    bulletBody->SetTransform(b2Vec2(body->GetPosition().x - m_box2d->pixel2Meter(DP(30)*sGlobal->scaleX), body->GetPosition().y - m_box2d->pixel2Meter(DP(15)*sGlobal->scaleX)), 0);
    bulletBody->SetGravityScale(0);
    bulletBody->SetLinearVelocity(b2Vec2(0,0));
//    enemyBullet->spxSprite->setColor(wyc4b(200, 0, 0, 255));
    
    float velx, vely;
    switch (shootTime) {
        case 0:
            velx = -34.0f;
            vely = -20.0f*multiY;
            break;
        case 1:
            velx = -22.0f;
            vely = -20.0f*multiY;
            break;
        case 2:
            velx = -12.0f;
            vely = -20.0f*multiY;
            break;
        default:
            break;
    }
    
    b2Vec2 f = bulletBody->GetWorldVector(b2Vec2(velx, vely));
    b2Vec2 p = bulletBody->GetWorldPoint(b2Vec2(0.0f, 0.0f));
    bulletBody->ApplyLinearImpulse(f, p);
    
    enemyBullet->spxSprite->setPosition(m_box2d->meter2Pixel(bulletBody->GetPosition().x), m_box2d->meter2Pixel(bulletBody->GetPosition().y));
}


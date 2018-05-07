//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "FlyZombie1.h"
#include "Global.h"
#include "EnemyFactory.h"

FlyZombie1::FlyZombie1(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_FLY_NORMAL, objectsGroup, obj, myCmd){
    e_id = ENEMY_TUZIGONGJIANSHOUFLY;
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
    
    isFighted = false;
    needDeadBlink = false;
    
    unitInterval = 0.15f;
    myScaleX = sGlobal->scaleX*0.7f;
    myScaleY = sGlobal->scaleY*0.7f;
    spxCacheArray = sGlobal->enemy_2_cache;
    
    shadowSX = 0.0f;
    shadowSY = 0.0f;
    shadowPosX = 0;
    shadowPosY = 0;
    
    fightDistance = sGlobal->virtualWinWidth * randRange(4, 5)/10.0f;
    
    afterInit(x, y);
    
    body->SetGravityScale(0);
    
    xuecaoSx = 0.48f*sGlobal->scaleX;
    xuecaoOfx = -DP(0)*sGlobal->scaleX;
    xuecaoOfy = DP(56)*sGlobal->scaleY;
}

FlyZombie1::~FlyZombie1() {
    
}

void FlyZombie1::update(float dt) {
    Enemy::update(dt);
    
    if (isDead) {
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    if (needAttack && !isFighted && !isStunning) {
        if (distance < fightDistance && distance > 0) {
            isAttacking = true;
            doShootArrowAnim();
        }
    }
}

void FlyZombie1::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

void FlyZombie1::beginContact(b2Body *actorB) {
    Enemy::beginContact(actorB);
}

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    FlyZombie1* enemy = (FlyZombie1*)data;
    if (enemy->isDead) {
        return;
    }

    if (sprite->getCurrentFrame() == 2) {
        enemy->shootArrow();
    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    FlyZombie1* enemy = (FlyZombie1*)data;
    if (enemy->isDead) {
        return;
    }
    
    wySPXSprite *enemySprite = ((wySPXSprite *) enemy->spxSprite);
    enemySprite->setAFCSpriteCallback(NULL, NULL);
    enemySprite->setLoopCount(-1);
    enemySprite->playAnimation(enemy->animIdle);
    
    enemy->isAttacking = false;
}

void FlyZombie1::doShootArrowAnim() {
    isFighted = true;

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

void FlyZombie1::shootArrow() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    EnemyBullet *enemyBullet = EnemyBullet::make(sGlobal->enemy_2_cache, 20, 0.1f, sGlobal->scaleX*0.7f, sGlobal->scaleY*0.7f);
    enemyBullet->atk = atk;
    b2Body *bulletBody = enemyBullet->body;
    bulletBody->SetTransform(b2Vec2(body->GetPosition().x - m_box2d->pixel2Meter(DP(30)*sGlobal->scaleX), body->GetPosition().y - m_box2d->pixel2Meter(DP(15)*sGlobal->scaleX)), 0);
    bulletBody->SetGravityScale(0.2f);
    bulletBody->SetLinearVelocity(b2Vec2(0,0));

    b2Vec2 f = bulletBody->GetWorldVector(b2Vec2(-10.0f, -6.0f));
    b2Vec2 p = bulletBody->GetWorldPoint(b2Vec2(0.0f, 0.0f));
    bulletBody->ApplyLinearImpulse(f, p);
    
    enemyBullet->spxSprite->setPosition(m_box2d->meter2Pixel(bulletBody->GetPosition().x), m_box2d->meter2Pixel(bulletBody->GetPosition().y));

}


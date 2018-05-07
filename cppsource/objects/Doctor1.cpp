//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "Doctor1.h"
#include "Global.h"
#include "EnemyFactory.h"

Doctor1::Doctor1(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_NORMAL, objectsGroup, obj, myCmd){
    obj_type = TYPE_ENEMY;
    
    animIdle = 7;
    animWalk = 8;
    animWalkBack = 9;
    animHurt = 12;
    animAtk = 10;
    
    animJump = 13;
    animLanding = -1;
    animDead = -3;

    e_id = ENEMY_GOUTOUJUNSHI;
    
    deadSFX = se_doctor_dead;
    
    isFighted = false;

    isOnGround = false;

    unitInterval = 0.15f;
    myScaleX = sGlobal->scaleX*0.7f;
    myScaleY = sGlobal->scaleY*0.7f;
    spxCacheArray = sGlobal->enemy_1_cache;
    
    shadowSX = 2.8f;
    shadowSY = 1.6f;
    shadowPosX = DP(4)*sGlobal->scaleY;
    shadowPosY = -DP(50)*sGlobal->scaleY;
    
    fightDistance = sGlobal->virtualWinWidth * randRange(6, 7)/10.0f;
//    LOGE("fightDistance:%f, factor:%f", fightDistance, fightDistance/sGlobal->virtualWinWidth);
    
    afterInit(x, y);
    
    xuecaoSx = 0.32f*sGlobal->scaleX;
    xuecaoOfx = -DP(0)*sGlobal->scaleX;
    xuecaoOfy = DP(60)*sGlobal->scaleY;
}

Doctor1::~Doctor1() {
    
}

void Doctor1::update(float dt) {
    Enemy::update(dt);
    
    if (isDead) {
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (needAttack && !isFighted && !isStunning && isCmdFinished) {
        if (distance < fightDistance && distance > 0) {
            isAttacking = true;
            doShootArrowAnim();
        }
    }
}

void Doctor1::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

void Doctor1::beginContact(b2Body *actorB) {
    Enemy::beginContact(actorB);
}

void Doctor1::endContact(b2Body *actorB) {
    Enemy::endContact(actorB);
}

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    Doctor1* enemy = (Doctor1*)data;
    if (enemy->isDead) {
        return;
    }
    //enemy->createFixtureByCurrentFrame();
    //LOGE("curFrame: %d", sprite->getCurrentFrame());
    if (sprite->getCurrentFrame() == 2) {
        enemy->shootArrow();
    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    Doctor1* enemy = (Doctor1*)data;
    if (enemy->isDead) {
        return;
    }

    sprite->setLoopCount(-1);
    sprite->setAFCSpriteCallback(NULL, NULL);
    if (!enemy->isOnGround) {
        sprite->playAnimation(enemy->animJump);
    } else {
        if (enemy->body->GetLinearVelocity().x > 1.0f) {
            sprite->playAnimation(enemy->animWalkBack);
        }
        else if (enemy->body->GetLinearVelocity().x < -1.0f){
            sprite->playAnimation(enemy->animWalk);
        }
        else {
            sprite->playAnimation(enemy->animIdle);
        }
    }
    
    enemy->isAttacking = false;
}

void Doctor1::doShootArrowAnim() {
    isFighted = true;
    
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    enemySprite->setLoopCount(0);
    enemySprite->setUnitInterval(unitInterval);
    enemySprite->playAnimation(animAtk);
    enemySprite->setAFCSpriteCallback(&callback, this);
    //createFixtureByCurrentFrame();
    
    enemySprite->setFlipX(false);
    
    if (isWandering) {
        body->SetLinearVelocity(b2Vec2(0,0));
    }
}

void Doctor1::shootArrow() {
    FlyingBottle *bomb = FlyingBottle::make(container->getPositionX()-DP(35)*sGlobal->scaleX, container->getPositionY()-DP(20)*sGlobal->scaleX);
    bomb->atk = this->atk;
}

void Doctor1::destroyMyself() {
    Enemy::destroyMyself();
}

void Doctor1::dead() {
    Enemy::dead();
    
//    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
}

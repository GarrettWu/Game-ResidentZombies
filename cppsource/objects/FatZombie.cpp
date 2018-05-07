//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "FatZombie.h"
#include "Global.h"
#include "EnemyFactory.h"

FatZombie::FatZombie(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd, int type) : Enemy(x, y, APPEAR_NORMAL, objectsGroup, obj, myCmd){
    
    animIdle = 12;
    animWalk = 13;
    animWalkBack = 14;
    animHurt = 18;
    animAtk = 15;
    
    animJump = 19;
    animLanding = -1;
    animDead = -3;
    
    if (type == 0) {
        e_id = ENEMY_TUZIJIANSHENG;
    } else {
        e_id = ENEMY_KULOUWUSHI;
    }
    
    deadSFX = se_fatzombie_dead;
    
    isFighted = false;
    isOnGround = false;

    unitInterval = 0.15f;
    myScaleX = sGlobal->scaleX*0.7f;
    myScaleY = sGlobal->scaleY*0.7f;
    spxCacheArray = sGlobal->enemy_0_cache;
    
    shadowSX = 3.8f;
    shadowSY = 1.8f;
    shadowPosX = DP(10)*sGlobal->scaleY;
    shadowPosY = -DP(50)*sGlobal->scaleY;
    
    fightDistance = sGlobal->virtualWinWidth * randRange(6, 7)/10.0f;
    
    afterInit(x, y);
    
    xuecaoSx = 0.48f*sGlobal->scaleX;
    xuecaoOfx = -DP(0)*sGlobal->scaleX;
    xuecaoOfy = DP(70)*sGlobal->scaleY;
}

FatZombie::~FatZombie() {
    
}

void FatZombie::update(float dt) {
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

void FatZombie::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

void FatZombie::beginContact(b2Body *actorB) {
    Enemy::beginContact(actorB);
}

void FatZombie::endContact(b2Body *actorB) {
    Enemy::endContact(actorB);
}

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    FatZombie* enemy = (FatZombie*)data;
    if (enemy->isDead) {
        return;
    }
    if (sprite->getCurrentFrame() == 2) {
        enemy->shootArrow();
    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    FatZombie* enemy = (FatZombie*)data;
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

void FatZombie::doShootArrowAnim() {
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
    
    enemySprite->setFlipX(false);
    
    if (isWandering) {
        body->SetLinearVelocity(b2Vec2(0,0));
    }
}

void FatZombie::shootArrow() {
    FlyingStone *bomb = FlyingStone::make(container->getPositionX(), container->getPositionY());
    bomb->atk = this->atk;
}

void FatZombie::onDead() {
    if (sGlobal->index == 0) {
        sGlobal->hudLayer->updateGuide();
    }
}
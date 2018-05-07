//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "MuscleZombie.h"
#include "Global.h"

MuscleZombie::MuscleZombie(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_NORMAL, objectsGroup, obj, myCmd){
    obj_type = TYPE_ENEMY;

    animIdle = 8;
    animWalk = 9;
    animWalkBack = 10;
    animHurt = 13;
    animAtk = 12;
    
    animJump = 14;
    animLanding = -1;
    animDead = -3;
    
    e_id = ENEMY_HONGYIWUSHI;
    
    deadSFX = se_musclezombie_dead;
    
    isFighted = false;
    isOnGround = false;
    
    unitInterval = 0.15f;
    myScaleX = sGlobal->scaleX*0.7f;
    myScaleY = sGlobal->scaleY*0.7f;
    spxCacheArray = sGlobal->enemy_2_cache;
    
    shadowSX = 4.0f;
    shadowSY = 1.8f;
    shadowPosX = DP(12)*sGlobal->scaleY;
    shadowPosY = -DP(60)*sGlobal->scaleY;
    
    fightDistance = sGlobal->virtualWinWidth * randRange(2, 4)/10.0f;
    
    afterInit(x, y);
    
    xuecaoSx = 0.48f*sGlobal->scaleX;
    xuecaoOfx = -DP(0)*sGlobal->scaleX;
    xuecaoOfy = DP(65)*sGlobal->scaleY;
}

MuscleZombie::~MuscleZombie() {
    
}

void MuscleZombie::update(float dt) {
    Enemy::update(dt);
    
    if (isDead) {
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (needAttack && !isFighted && !isStunning) {
        if (distance > 0 && distance < fightDistance) {
            isAttacking = true;
            doShootArrowAnim();
        }
    }
}

void MuscleZombie::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

void MuscleZombie::beginContact(b2Body *actorB) {
    Enemy::beginContact(actorB);
}

void MuscleZombie::endContact(b2Body *actorB) {
    Enemy::endContact(actorB);
}

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    MuscleZombie* enemy = (MuscleZombie*)data;
    if (enemy->isDead) {
        return;
    }
//    sGlobal->mainGameLayer->addToRecreateFixture(enemy);
    //LOGE("curFrame: %d", sprite->getCurrentFrame());
//    if (sprite->getCurrentFrame() == 2) {
//        sGlobal->mainGameLayer->addToRecreateFixture(enemy);
//    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    MuscleZombie* enemy = (MuscleZombie*)data;
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

void MuscleZombie::doShootArrowAnim() {
    isFighted = true;
    
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    enemySprite->setUnitInterval(unitInterval);
    enemySprite->setLoopCount(0);
    enemySprite->playAnimation(animAtk);
    enemySprite->setAFCSpriteCallback(&callback, this);
    
    enemySprite->setFlipX(false);
    
    if (isWandering) {
        body->SetLinearVelocity(b2Vec2(0,0));
    }
}

void MuscleZombie::destroyMyself() {
    Enemy::destroyMyself();
}

void MuscleZombie::dead() {
    Enemy::dead();
}

//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "MaleZombie.h"
#include "Global.h"

MaleZombie::MaleZombie(float x, float y, int type, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_NORMAL, objectsGroup, obj, myCmd){
    
    obj_type = TYPE_ENEMY;
    switch (type) {
        case SPX_RABBIT_SWORD_MAN_0:
            e_id = ENEMY_TUZIJIANKE;

            animJump = 5;
            animLanding = -1;
            animDead = -2;
            animWalk = 1;
            animHurt = 4;
            animIdle = 0;
            animWalkBack = 2;
            
            animAtk = 3;
            break;
        case SPX_RABBIT_SWORD_MAN_1:
            e_id = ENEMY_TUZILANGKE;
            
            animJump = 5;
            animLanding = -1;
            animDead = -2;
            animWalk = 1;
            animHurt = 4;
            animIdle = 0;
            animWalkBack = 2;
            
            animAtk = 3;
            break;
        default:
            break;
    }

    deadSFX = se_malezombie_dead;

    isFighted = false;
    
    unitInterval = 0.1f;
    myScaleX = sGlobal->scaleX*0.7f;
    myScaleY = sGlobal->scaleY*0.7f;
    spxCacheArray = sGlobal->enemy_0_cache;
    
    shadowSX = 3.0f;
    shadowSY = 1.6f;
    shadowPosX = DP(4)*sGlobal->scaleY;
    shadowPosY = -DP(55)*sGlobal->scaleY;
    
    fightDistance = sGlobal->virtualWinWidth * randRange(2, 3)/10.0f;
    
    afterInit(x, y);
    
    xuecaoSx = 0.32f*sGlobal->scaleX;
    xuecaoOfx = -DP(6)*sGlobal->scaleX;
    xuecaoOfy = DP(55)*sGlobal->scaleY;
}

MaleZombie::~MaleZombie() {
}

void MaleZombie::update(float dt) {
    Enemy::update(dt);
    
    if (isDead) {
        return;
    }
    
    if (needAttack && !isFighted && !isStunning) {
        if (distance < fightDistance && distance > 0) {
            isAttacking = true;
            fight();
        }
    }
    
//    LOGE("shadow posy:%f", shadow->getPositionY());
}

void MaleZombie::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

void MaleZombie::beginContact(b2Body *actorB) {
    Enemy::beginContact(actorB);
    
//    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
//    
//    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
//    int tagB = userDataB->obj_type;
//    if (tagB == TYPE_GROUND) {
//        isOnGround = true;
//        //        shadow->setVisible(true);
//    }
}

void MaleZombie::endContact(b2Body *actorB) {
    Enemy::endContact(actorB);
    
//    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
//    int tagB = userDataB->obj_type;
//    if (tagB == TYPE_GROUND) {
//        isOnGround = false;
//        //        shadow->setVisible(false);
//    }
}

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    MaleZombie* enemy = (MaleZombie*)data;
    if (enemy->isDead) {
        return;
    }
    
//    if (sprite->getCurrentAnimationIndex() == enemy->fightAnimIndex) {
//        sGlobal->mainGameLayer->addToRecreateFixture(enemy);
//    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    MaleZombie* enemy = (MaleZombie*)data;
    if (enemy->isDead) {
        return;
    }
    
    sprite->setLoopCount(-1);
    sprite->setAFCSpriteCallback(NULL, NULL);
    if (!enemy->isOnGround) {
//        LOGE("on air, play jump...");
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

void MaleZombie::fight() {
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    
    enemySprite->setUnitInterval(unitInterval);
    enemySprite->setLoopCount(0);
    enemySprite->setAFCSpriteCallback(&callback, this);
    enemySprite->playAnimation(animAtk);
    
    enemySprite->setFlipX(false);
    
    if (isWandering) {
        body->SetLinearVelocity(b2Vec2(0,0));
    }

    isFighted = true;
}

void MaleZombie::onDead() {
    if (sGlobal->index == 0) {
        sGlobal->hudLayer->updateGuide();
    }
}



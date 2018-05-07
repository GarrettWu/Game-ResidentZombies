//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "FemaleZombie.h"
#include "Global.h"
#include "EnemyFactory.h"

FemaleZombie::FemaleZombie(float x, float y, int type, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_NORMAL, objectsGroup, obj, myCmd){
    obj_type = TYPE_ENEMY;

    switch (type) {
        case SPX_DELUYI:
            e_id = ENEMY_ZHAOHUANSHI;
            
            animJump = 11;
            animLanding = -1;
            animDead = -3;
            animWalk = 7;
            animHurt = 10;
            animIdle = 6;
            animWalkBack = 8;
            animAtk = 9;
            
            break;
        case SPX_SKELETON_SPECULATOR:
            e_id = ENEMY_KULOUTOUJIZHE;
            
            animJump = 11;
            animLanding = -1;
            animDead = -3;
            animWalk = 7;
            animHurt = 10;
            animIdle = 6;
            animWalkBack = 8;
            animAtk = 9;
            
            break;
        default:
            break;
    }

    deadSFX = se_femalezombie_dead;
    
    isFighted = false;
    isOnGround = false;

    unitInterval = 0.15f;
    myScaleX = sGlobal->scaleX*0.7f;
    myScaleY = sGlobal->scaleY*0.7f;
    spxCacheArray = sGlobal->enemy_0_cache;
    
    shadowSX = 3.0f;
    shadowSY = 1.6f;
    shadowPosX = DP(-4)*sGlobal->scaleY;
    shadowPosY = -DP(55)*sGlobal->scaleY;
    
    fightDistance = sGlobal->virtualWinWidth * randRange(2, 3)/10.0f;
    
    afterInit(x, y);
    
    xuecaoSx = 0.36f*sGlobal->scaleX;
    xuecaoOfx = -DP(4)*sGlobal->scaleX;
    xuecaoOfy = DP(58)*sGlobal->scaleY;
}

FemaleZombie::~FemaleZombie() {
    
}

void FemaleZombie::update(float dt) {
    Enemy::update(dt);
    
    if (isDead) {
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    if (needAttack && !isFighted && !isStunning && isCmdFinished) {
        if (distance < fightDistance && distance > 0) {
            isAttacking = true;
            doFightAnim();
        }
    }
}

void FemaleZombie::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

void FemaleZombie::beginContact(b2Body *actorB) {
    Enemy::beginContact(actorB);
    
//    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
//    
//    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
//    int tagB = userDataB->obj_type;
//    if (tagB == TYPE_GROUND) {
//        isOnGround = true;
////        shadow->setVisible(true);
//    }
}

void FemaleZombie::endContact(b2Body *actorB) {
    Enemy::endContact(actorB);
    
//    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
//    int tagB = userDataB->obj_type;
//    if (tagB == TYPE_GROUND) {
//        isOnGround = false;
////        shadow->setVisible(false);
//    }
}

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    FemaleZombie* enemy = (FemaleZombie*)data;
    if (enemy->isDead) {
        return;
    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    FemaleZombie* enemy = (FemaleZombie*)data;
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

void FemaleZombie::doFightAnim() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
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
    
//    body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    
    isFighted = true;
    
    if (isWandering) {
        body->SetLinearVelocity(b2Vec2(0,0));
    }
}

void FemaleZombie::dead() {
    Enemy::dead();
}

void FemaleZombie::destroyMyself() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    Enemy::destroyMyself();
}

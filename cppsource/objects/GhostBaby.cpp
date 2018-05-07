//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "GhostBaby.h"
#include "Global.h"

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    GhostBaby* enemy = (GhostBaby*)data;
    if (enemy->isDead) {
        return;
    }
    //enemy->createFixtureByCurrentFrame();
//    if (enemy->e_id == ENEMY_KULOUSHIBING && sprite->getCurrentFrame() == 2) {
//        sGlobal->mainGameLayer->addToRecreateFixture(enemy);
//    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    GhostBaby* enemy = (GhostBaby*)data;
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

GhostBaby::GhostBaby(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_NORMAL, objectsGroup, obj, myCmd){
    obj_type = TYPE_ENEMY;

    e_id = ENEMY_KULOUSHIBING;
    
    animIdle = 14;
    animWalk = 15;
    animWalkBack = 16;
    animHurt = 18;
    animAtk = 17;
    
    animJump = 19;
    animLanding = -1;
    animDead = -3;

    deadSFX = se_ghostbaby_dead;
    
    isFighted = false;
    
    unitInterval = 0.15f;
    myScaleX = sGlobal->scaleX*0.75f;
    myScaleY = sGlobal->scaleY*0.75f;
    spxCacheArray = sGlobal->enemy_1_cache;
    
    shadowSX = 3.0f;
    shadowSY = 1.6f;
    shadowPosX = DP(-2)*sGlobal->scaleY;
    shadowPosY = -DP(40)*sGlobal->scaleY;
    
    fightDistance = sGlobal->virtualWinWidth * randRange(2, 3)/10.0f;
    
    afterInit(x, y);
    
    xuecaoSx = 0.36f*sGlobal->scaleX;
    xuecaoOfx = -DP(0)*sGlobal->scaleX;
    xuecaoOfy = DP(54)*sGlobal->scaleY;
}

GhostBaby::~GhostBaby() {
}

void GhostBaby::update(float dt) {
    Enemy::update(dt);
    
    if (isDead) {
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    if (needAttack && !isFighted && !isStunning) {
        if (distance < fightDistance && distance > 0) {
            fight();
            isAttacking = true;
        }
    }
}

void GhostBaby::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

void GhostBaby::fight() {
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    
    enemySprite->setUnitInterval(unitInterval);
    enemySprite->setLoopCount(0);
    enemySprite->playAnimation(animAtk);
    enemySprite->setAFCSpriteCallback(&callback, this);
    
    enemySprite->setFlipX(false);
    
    isFighted = true;
    
    if (isWandering) {
        body->SetLinearVelocity(b2Vec2(0,0));
    }
}


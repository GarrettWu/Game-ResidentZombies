//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "WolfZombie.h"
#include "Global.h"

WolfZombie::WolfZombie(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_NORMAL, objectsGroup, obj, myCmd){
    obj_type = TYPE_ENEMY;
    
    animIdle = 20;
    animWalk = 21;
    animWalkBack = 22;
    animHurt = 24;
    animAtk = 23;
    
    animJump = 25;
    animLanding = -1;
    animDead = -3;

    e_id = ENEMY_HEIYIWUSHI;
    
	sub_type = 100;
    deadSFX = se_wolf_dead;

    isFighted = false;
    
    unitInterval = 0.15f;
    myScaleX = sGlobal->scaleX*0.8f;
    myScaleY = sGlobal->scaleY*0.8f;
    spxCacheArray = sGlobal->enemy_1_cache;
    
    shadowSX = 4.0f;
    shadowSY = 1.8f;
    shadowPosX = DP(-4)*sGlobal->scaleY;
    shadowPosY = -DP(30)*sGlobal->scaleY;
    
    fightDistance = sGlobal->virtualWinWidth * randRange(3, 4)/10.0f;
    
    afterInit(x, y);
    
    xuecaoSx = 0.5f*sGlobal->scaleX;
    xuecaoOfx = -DP(0)*sGlobal->scaleX;
    xuecaoOfy = DP(40)*sGlobal->scaleY;
}

WolfZombie::~WolfZombie() {
}

void WolfZombie::update(float dt) {
    Enemy::update(dt);
    
    if (isDead) {
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (needAttack && !isFighted && !isStunning) {
        if (distance < fightDistance && distance > 0) {
            isAttacking = true;
            fight();
        }
    }
}

void WolfZombie::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    WolfZombie* enemy = (WolfZombie*)data;
    if (enemy->isDead) {
        return;
    }
    //enemy->createFixtureByCurrentFrame();
//    if (sprite->getCurrentFrame() == 2) {
////        enemy->createFixtureByCurrentFrame();
//        sGlobal->mainGameLayer->addToRecreateFixture(enemy);
//    }
//    if (sprite->getCurrentFrame() == 9) {
//        enemy->body->SetLinearVelocity(b2Vec2(0,0));
//    }
    
//    if (sprite->getCurrentAnimationIndex() == enemy->animIdle) {
//        sprite->setAFCSpriteCallback(NULL, NULL);
//    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    WolfZombie* enemy = (WolfZombie*)data;
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

void WolfZombie::fight() {
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    
    spxSprite->setUnitInterval(unitInterval);
    spxSprite->setLoopCount(0);
    spxSprite->playAnimation(animAtk);
    spxSprite->setAFCSpriteCallback(&callback, this);
//    body->SetLinearVelocity(b2Vec2(-10.0f,0));
    //createFixtureByCurrentFrame();
    
    spxSprite->setFlipX(false);
    
    isFighted = true;
    
    if (isWandering) {
        body->SetLinearVelocity(b2Vec2(0,0));
    }
}


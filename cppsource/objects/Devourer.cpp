//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "Devourer.h"
#include "Global.h"
#include "EnemyFactory.h"


Devourer::Devourer(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_NORMAL, objectsGroup, obj, myCmd){
    e_id = ENEMY_TUZIFASHI;
    
    deadSFX = se_devourer_dead;
    
    animIdle = 0;
    animWalk = 1;
    animWalkBack = 2;
    animHurt = 6;
    animAtk = 3;
    
    animJump = 7;
    animLanding = -1;
    animDead = -3;
    
    isFighted = false;
    
    isLighting = false;
    lightingBody = NULL;
    
    unitInterval = 0.15f;
    myScaleX = sGlobal->scaleX*0.8f;
    myScaleY = sGlobal->scaleY*0.8f;;
    spxCacheArray = sGlobal->enemy_2_cache;
    
    shadowSX = 4.2f;
    shadowSY = 1.8f;
    shadowPosX = 0;
    shadowPosY = -DP(24)*sGlobal->scaleY;
    
    fightDistance = sGlobal->virtualWinWidth * randRange(3, 4)/10.0f;
//    fightDistance = sGlobal->virtualWinWidth * 0.38f;
    
    afterInit(x, y);
    
    xuecaoSx = 0.4f*sGlobal->scaleX;
    xuecaoOfx = -DP(0)*sGlobal->scaleX;
    xuecaoOfy = DP(40)*sGlobal->scaleY;
}

Devourer::~Devourer() {
    
}

void Devourer::update(float dt) {
    Enemy::update(dt);
    
    if (isDead) {
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    if (needAttack && !isFighted && !isStunning) {
        if (distance < fightDistance && distance > 0) {
            doShootArrowAnim();
            isFighted = true;
            isAttacking = true;
        }
    }
    
    if (isLighting) {
        lightingTimer += dt;
        if (lightingTimer > 0.6f) {
            laser1->destoryMe();
            lightingBody = NULL;
            lightingTimer = 0;
            isLighting = false;
        } else {
            lightingBody->SetTransform(b2Vec2(body->GetPosition().x+m_box2d->pixel2Meter(-DP(120)*sGlobal->scaleX), body->GetPosition().y+m_box2d->pixel2Meter(DP(20)*sGlobal->scaleY)), 0);
            laser1->container->setPosition(m_box2d->meter2Pixel(lightingBody->GetPosition().x), m_box2d->meter2Pixel(lightingBody->GetPosition().y));
        }
    }
}

void Devourer::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    Devourer* enemy = (Devourer*)data;
    if (enemy->isDead) {
        return;
    }
    if (sprite->getCurrentFrame() == 2) {
        enemy->shootArrow();
    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    Devourer* enemy = (Devourer*)data;
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

void Devourer::doShootArrowAnim() {
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    enemySprite->setAFCSpriteCallback(&callback, this);
    enemySprite->setLoopCount(0);
    enemySprite->setUnitInterval(unitInterval);
    enemySprite->playAnimation(animAtk);

    enemySprite->setFlipX(false);
    
    if (isWandering) {
        body->SetLinearVelocity(b2Vec2(0,0));
    }
}

void Devourer::shootArrow() {
    isLighting = true;
    lightingTimer = 0;
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    laser1 = new Laser(sGlobal->enemy_2_cache);
    laser1->atk = atk;
    laser1->spxSprite = (wySPXSprite*)sGlobal->popFromCache(sGlobal->enemy_2_cache);
    laser1->spxSprite->setScaleX(sGlobal->scaleX*1.0f);
    laser1->spxSprite->setScaleY(sGlobal->scaleY*0.8f);
    laser1->spxSprite->stopAllActions();
    laser1->spxSprite->setAFCSpriteCallback(NULL, NULL);
    laser1->spxSprite->setUnitInterval(0.1f);
    laser1->spxSprite->setVisible(true);
    laser1->spxSprite->setAlpha(255);
    laser1->spxSprite->playAnimation(5);
    laser1->container->addChildLocked(laser1->spxSprite);
    laser1->sub_type = -999;
    
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(body->GetPosition().x - m_box2d->pixel2Meter(DP(120)*sGlobal->scaleX), body->GetPosition().y + m_box2d->pixel2Meter(DP(30)*sGlobal->scaleY));
    lightingBody = m_box2d->getWorld()->CreateBody(&bd);
    lightingBody->SetTransform(bd.position, 0);
    b2PolygonShape box;
    box.SetAsBox(m_box2d->pixel2Meter(DP(70)*sGlobal->scaleX), m_box2d->pixel2Meter(DP(4)*sGlobal->scaleY));
    b2FixtureDef fd;
    fd.shape = &box;
    fd.restitution = 0.0f;
    fd.friction = 0.0f;
    fd.density = 0.0f;
    fd.isSensor = true;
    fd.filter.maskBits = MASK_BITS[TYPE_ENEMY_BULLET];
    fd.filter.categoryBits = CAT_BITS[TYPE_ENEMY_BULLET];
    fd.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY_BULLET];
    lightingBody->CreateFixture(&fd);
    lightingBody->SetUserData(laser1);
    lightingBody->SetGravityScale(0);
    laser1->body = lightingBody;
    laser1->container->setPosition(m_box2d->meter2Pixel(lightingBody->GetPosition().x), m_box2d->meter2Pixel(lightingBody->GetPosition().y));
}

void Devourer::destroyMyself() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
//    LOGE("destory devourer...");
    if (lightingBody != NULL) {
//        LOGE("destory devourer's toun...");
//        sGlobal->mainGameLayer->addToDestroy(lightingBody);
        laser1->destoryMe();
        lightingBody = NULL;
        lightingTimer = 0;
        isLighting = false;
    }
    
    Enemy::destroyMyself();
}

void Devourer::dead() {
//    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
//    enemySprite->setUnitInterval(0.1f);
    Enemy::dead();
    
//    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
//    LOGE("destory devourer dead...");
    if (lightingBody != NULL) {
//        sGlobal->mainGameLayer->addToDestroy(lightingBody);
        laser1->destoryMe();
        lightingBody = NULL;
        lightingTimer = 0;
        isLighting = false;
    }
}


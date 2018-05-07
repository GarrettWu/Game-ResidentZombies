//
//  MyContactListener.h
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//
#pragma once

#include "WiEngine.h"
#include "Enemy.h"

class FatZombie : public Enemy {
    
public:
    FatZombie(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd, int type);//type:0 兔子剑圣 1 骷髅武士
    ~FatZombie();

    void update(float dt);
    void handleCollision(b2Body *actorB);
    void endContact(b2Body *actorB);
    void beginContact(b2Body *actorB);
    
    void doShootArrowAnim();
    void shootArrow();
    
    static FatZombie *make(int type, float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW FatZombie(x, y, objectsGroup, obj, myCmd, type);
    }
    
    void onDead();
};

class FlyingStone : public Enemy {
    
public:
    wySPXSprite *baozha;
    bool isBaoing;
    
    static void onBombEnded(wyAFCSprite* sprite, void* data) {
        FlyingStone *bomb = (FlyingStone *)data;
        bomb->spxSprite->setVisible(false);
        sGlobal->mainGameLayer->addToDestroy(bomb->body);
    }
    
    FlyingStone(float x, float y) : Enemy(x, y) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        int e_id = ENEMY_XIANJING3;

        hp = 1;
        atk = 0;
        gold = 0;
        
        animIdle = 16;
        animDead = 17;
        
        unitInterval = 0.1f;
        myScaleX = sGlobal->scaleX*1.0f;
        myScaleY = sGlobal->scaleY*1.0f;
        spxCacheArray = sGlobal->enemy_0_cache;
        
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(container, 101);
        
        baozha = getSpxSprite(true);
        baozha->setForceTickMode(true);
        baozha->setUnitInterval(0.1f);
        baozha->setLoopCount(0);
        baozha->setScaleX(sGlobal->scaleX*0.7f);
        baozha->setScaleY(sGlobal->scaleY*0.7f);
        baozha->playAnimation(16);
        container->addChildLocked(baozha);
        spxSprite = baozha;
        
        obj_type = TYPE_ENEMY;
        
        float boxWidth = DP(20)*sGlobal->scaleX;
        float boxHeight = DP(20)*sGlobal->scaleX;
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(x-DP(60)*sGlobal->scaleY), m_box2d->pixel2Meter(y+DP(50)*sGlobal->scaleY));
        bd.userData = this;
        bd.type = b2_dynamicBody;
        
        b2PolygonShape staticBox;
        staticBox.SetAsBox(m_box2d->pixel2Meter(boxWidth), m_box2d->pixel2Meter(boxHeight));
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &staticBox;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.isSensor = false;
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_ENEMY]^CAT_BITS[TYPE_GROUND];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);
        body->SetGravityScale(0);
        
        float velx, vely;
        velx = -12.0f;
        vely = -2.0f;
        body->SetLinearVelocity(b2Vec2(velx, vely));
        
        offsetX = 0;
        offsetY = 0;
        
        isBaoing = false;
        
        needShowDeadEffect = false;
        needDeadBlink = false;
        needStay = false;
        
        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
        
        canKnockBack = false;
        stunnable = false;
        isInScreen = true;
    }
    
    ~FlyingStone() {
        
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        if (isDead) {
            spxSprite->tick(dt);
        }
        
        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
    }
    
    static FlyingStone *make(float x, float y) {
        return WYNEW FlyingStone(x, y);
    }
    
    void onHitHero() {
        if (isDead)
            return;
        
        isBaoing = true;
        isDead = true;
        
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
        wyAFCSpriteCallback callback = {
            NULL,
            onBombEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(17);
        spxSprite->setPaused(false);
        spxSprite->stopAllActions();
        sGlobal->mainGameLayer->addToDisable(body);
    }
    
    void handleCollision(b2Body *actorB) {

    }
    
    void dead() {
        if (isDead)
            return;
        
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
        isBaoing = true;
        isDead = true;
        wyAFCSpriteCallback callback = {
            NULL,
            onBombEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(17);
        spxSprite->setPaused(false);
        spxSprite->stopAllActions();
        sGlobal->mainGameLayer->addToDisable(body);
    }
    
    void hurt(float y) {

    }
    
    void destroyMyself() {
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
        spxSprite->stopAllActions();
        spxSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(spxCacheArray, spxSprite);
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        delete this;
        
        return;
    }
    
    void beginContact(b2Body *actorB) {

    }
    
    void endContact(b2Body *actorB) {

    }
};
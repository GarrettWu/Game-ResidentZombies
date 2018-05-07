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

class Doctor1 : public Enemy {
    
public:
    Doctor1(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~Doctor1();

    void update(float dt);
    void handleCollision(b2Body *actorB);
    void endContact(b2Body *actorB);
    void beginContact(b2Body *actorB);
    
    void doShootArrowAnim();
    void shootArrow();
    
    static Doctor1 *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW Doctor1(x, y, objectsGroup, obj, myCmd);
    }
    
    void destroyMyself();
    void dead();
};

class FlyingBottle : public Enemy {
    
public:
    bool isBaoing;
    
    static void onBombEnded(wyAFCSprite* sprite, void* data) {
        FlyingBottle *bomb = (FlyingBottle *)data;
        bomb->spxSprite->setVisible(false);
        sGlobal->mainGameLayer->addToDestroy(bomb->body);
    }
    
    FlyingBottle(float x, float y) : Enemy(x, y, APPEAR_NORMAL, NULL, NULL, NULL, false) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        int e_id = ENEMY_GOUTOUJUNSHI;
        hp = 1;
        atk = 0;
        gold = 0;
        
        offsetX = 0;
        offsetY = 0;
        
        isBaoing = false;
        stunnable = false;
        canKnockBack = false;
        
        animIdle = 11;
        animDead = 11;
        needShowDeadEffect = false;
        needDeadBlink = false;
        needStay = false;
        
        unitInterval = 0.05f;
        myScaleX = sGlobal->scaleX*1.0f;
        myScaleY = sGlobal->scaleY*1.0f;
        spxCacheArray = sGlobal->enemy_1_cache;

        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(container, 101);
        
        wySPXSprite *enemySprite = getSpxSprite(true);
        enemySprite->setPaused(true);
        enemySprite->runAction(wyRepeatForever::make(wyRotateBy::make(0.4f, -360)));
        enemySprite->setUnitInterval(0.1f);
        enemySprite->setLoopCount(0);
        spxSprite = enemySprite;
        container->addChildLocked(spxSprite);
        
        obj_type = TYPE_ENEMY;
        
        float boxWidth = DP(15)*sGlobal->scaleX;
        float boxHeight = DP(15)*sGlobal->scaleX;
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y+DP(40)*sGlobal->scaleY));
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
        velx = -20.0f;
        vely = 0.0f;
        body->SetLinearVelocity(b2Vec2(velx, vely));
        
        bloodSprite = NULL;
        smokeSprite = NULL;
        shadow = NULL;
        isInitToShow = true;
        
        isInScreen = true;
    }
    
    ~FlyingBottle() {
        
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

        if (isDead) {
            spxSprite->tick(dt);
        }
        
        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
    }
    
    static FlyingBottle *make(float x, float y) {
        return WYNEW FlyingBottle(x, y);
    }
    
    void onHitHero() {
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
        spxSprite->playAnimation(11);
        spxSprite->setPaused(false);
        spxSprite->stopAllActions();
        spxSprite->runAction(wyMoveBy::make(0.5f, DP(100)*sGlobal->scaleX, 0));
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
        spxSprite->playAnimation(11);
        spxSprite->setPaused(false);
        spxSprite->stopAllActions();
//        spxSprite->runAction(wyMoveBy::make(0.5f, DP(100)*sGlobal->scaleX, 0));
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
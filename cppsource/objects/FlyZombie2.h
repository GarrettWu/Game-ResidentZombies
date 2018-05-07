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

class FlyZombie2 : public Enemy {
    
public:
    FlyZombie2(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~FlyZombie2();

    bool isFighting;
    float fightingTimer;
    
    void update(float dt);
    void handleCollision(b2Body *actorB);
    void beginContact(b2Body *actorB);
    
    void doShootArrowAnim();
    void shootArrow();
    
    static FlyZombie2 *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW FlyZombie2(x, y, objectsGroup, obj, myCmd);
    }
    
    int currentIndex;
};

class Bomb : public BaseObject {
    
public:
    wySPXSprite *baozha;
    wySpriteEx *bomb;
    bool isBaoing;
    int curColor;
    float colorTimer;
    
    static void onBombEnded(wyAFCSprite* sprite, void* data) {
        Bomb *bomb = (Bomb *)data;
        sGlobal->mainGameLayer->addToDestroy(bomb->body);
    }
    
    Bomb(float x, float y) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        int e_id = ENEMY_XIANJING3;
        hp = 1;
        atk = 0;
        gold = 0;
        
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(container);
        
        baozha = (wySPXSprite *)sGlobal->popFromCache(sGlobal->enemy_2_cache);
        baozha->setForceTickMode(true);
        baozha->setUnitInterval(0.2f);
        baozha->setLoopCount(0);
        baozha->setScaleX(sGlobal->scaleX*0.7f);
        baozha->setScaleY(sGlobal->scaleY*0.7f);
        baozha->setPaused(true);
        baozha->playAnimation(21);
        container->addChildLocked(baozha);
        wyAFCSpriteCallback callback = {
            NULL,
            onBombEnded
        };
        baozha->setAFCSpriteCallback(&callback, this);
        
        obj_type = TYPE_BOMB;
        
        float boxWidth = DP(10)*sGlobal->scaleX;
        float boxHeight = DP(10)*sGlobal->scaleX;
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y));
        bd.userData = this;
        bd.type = b2_dynamicBody;
        
        b2PolygonShape staticBox;
        staticBox.SetAsBox(m_box2d->pixel2Meter(boxWidth), m_box2d->pixel2Meter(boxHeight));	
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &staticBox;	
        fixtureDef.density = 0.0f;   
        fixtureDef.friction = 0.8f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.isSensor = false;
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_BOMB];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_BOMB];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_BOMB];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);
        body->SetGravityScale(0.1f);
        
        float velx, vely;
        velx = -2.0f;
        vely = -10.0f;
        b2Vec2 f = body->GetWorldVector(b2Vec2(velx, vely));
        b2Vec2 p = body->GetWorldPoint(b2Vec2(0.0f, 0.0f));
        body->ApplyLinearImpulse(f, p);

        offsetX = 0;
        offsetY = 0;
        
        isBaoing = false;
    }
    
    ~Bomb() {
        
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        if (isBaoing) {
            baozha->tick(dt);
        } else {
            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
        }
    }
    
    static Bomb *make(float x, float y) {
        return WYNEW Bomb(x, y);
    }
    
    void onHitHero() {
        isBaoing = true;
        baozha->setPaused(false);
        sGlobal->mainGameLayer->addToDisable(body);
    }
    
    void handleCollision(b2Body *actorB) {
        BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
        int tagB = userDataB->obj_type;
        switch (tagB) {
            case TYPE_GROUND: {
                if (!isBaoing) {
                    isBaoing = true;
                    baozha->setPaused(false);
                    body->SetLinearVelocity(b2Vec2(0,0));
                    body->SetGravityScale(0);
                }
                break;
            }
            default:
                break;
        }
    }
    
    void destroyMyself() {
        baozha->stopAllActions();
        baozha->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(sGlobal->enemy_2_cache, baozha);
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        delete this;
        
        return;
    }
};

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

class SpittleZombie : public Enemy {
    
public:
    SpittleZombie(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~SpittleZombie();

    void update(float dt);
    void handleCollision(b2Body *actorB);
    void beginContact(b2Body *actorB);
    
    void doShootArrowAnim();
    void shootArrow();
    
    static SpittleZombie *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW SpittleZombie(x, y, objectsGroup, obj, myCmd);
    }
};

class Spittle : public UnShootableObject {
public:
    Spittle(wyArray *p_spxCacheArray, int p_animIdle, float p_unitInterval, float p_myScaleX, float p_myScaleY, float p_boxWidth, float p_boxHeight, int attack) : UnShootableObject(p_spxCacheArray, p_animIdle, p_unitInterval, p_myScaleX, p_myScaleY, p_boxWidth, p_boxHeight, attack){
        
        postInit();
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        spxSprite->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
        b2Vec2 vel = body->GetLinearVelocity();
        float angle = atan(vel.y/vel.x);
        spxSprite->setRotation(-wyMath::r2d(angle));
    }
    
};
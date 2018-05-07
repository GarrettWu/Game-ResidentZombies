//
//  MyContactListener.h
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//
#pragma once

#include "WiEngine.h"
#include "BaseObject.h"
#include "Global.h"
#include "Enemy.h"

class HeroBullet : public BaseObject {
public:
    float atkPercent;
    int bulletType;
    uint16 catBit;
    
    static HeroBullet *make(int bulletType, float velocityX=0, float velocityY=0, float posx=0, float posy=0) {
        return WYNEW HeroBullet(bulletType, velocityX, velocityY, posx, posy);
    }
    
    HeroBullet(int bulletType, float velocityX, float velocityY, float posx, float posy);
    
    ~HeroBullet();
    
    virtual void update(float dt);
    
    void dealHitEnemy(float y, Enemy *enemy);
    
    void makeBomb(int weaponId);
    
    float targetVelX;
    
    bool isLaserDestroied;
    float laserTimer;
    
    wyParticleSystem *bullet_emitter;
    bool isDisabled;
    
    void destroyMe();
    float x, y;
    
    void launch();
    bool isLaunching;
    float launchTimer;
    bool isLaser;
};


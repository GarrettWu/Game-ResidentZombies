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

class FlyZombie3 : public Enemy {
    
public:
    FlyZombie3(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~FlyZombie3();

    bool isFighting;
    float fightingTimer;
    
    void update(float dt);
    void handleCollision(b2Body *actorB);
    void beginContact(b2Body *actorB);
    
    void doShootArrowAnim();
    void shootArrow();
    
    static FlyZombie3 *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW FlyZombie3(x, y, objectsGroup, obj, myCmd);
    }
    
    int currentIndex;
    
    int shootTime, maxTime;
    float multiY;
};

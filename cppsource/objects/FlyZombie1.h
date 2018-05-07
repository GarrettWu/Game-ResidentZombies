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

class FlyZombie1 : public Enemy {
    
public:
    FlyZombie1(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~FlyZombie1();

    void update(float dt);
    void handleCollision(b2Body *actorB);
    void beginContact(b2Body *actorB);
    
    void doShootArrowAnim();
    void shootArrow();
    
    static FlyZombie1 *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW FlyZombie1(x, y, objectsGroup, obj, myCmd);
    }
    
    int currentIndex;
};
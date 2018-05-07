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
#include "Doctor1.h"

class Doctor2 : public Enemy {
    
public:
    Doctor2(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~Doctor2();
    
    void update(float dt);
    void handleCollision(b2Body *actorB);
    void endContact(b2Body *actorB);
    void beginContact(b2Body *actorB);
    
    void doShootArrowAnim();
    void shootArrow();
    
    static Doctor2 *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW Doctor2(x, y, objectsGroup, obj, myCmd);
    }
    
    void destroyMyself();
    void dead();
};

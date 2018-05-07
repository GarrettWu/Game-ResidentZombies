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
#include "Boss2.h"

class Devourer : public Enemy {
    
public:
    Devourer(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd);
    ~Devourer();

    bool isLighting;
    float lightingTimer;
    
    void update(float dt);
    void handleCollision(b2Body *actorB);
    
    void doShootArrowAnim();
    void shootArrow();
    
    static Devourer *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd = NULL) {
        return WYNEW Devourer(x, y, objectsGroup, obj, myCmd);
    }

    b2Body *lightingBody;

    void destroyMyself();
    void dead();
    
    Laser *laser1;
};


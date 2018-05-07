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

class NPC : public BaseObject {
    
public:
    NPC(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj);
    virtual ~NPC();
    
    virtual void update(float dt);
    
    static NPC *make(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj) {
        return WYNEW NPC(x, y, objectsGroup, obj);
    }
    
    float moveTimer, duration;
    bool isActing, isMoved, isTalked, isEnded;
    void move(float velX, float duration);
    void showDialogBox(int idx);
    
    void doEnd();
};


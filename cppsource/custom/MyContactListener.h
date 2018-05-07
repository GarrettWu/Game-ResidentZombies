//
//  MyContactListener.h
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//
#pragma once

#include "WiEngine-Box2D.h"
#include <vector>
#include <algorithm>

struct CustomContact {
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    bool operator==(const CustomContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};

class CustomContactListener : public b2ContactListener {
    
public:
    std::vector<CustomContact> _contacts;
    
    CustomContactListener();
    ~CustomContactListener();
    
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);    
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};
    
class CustomContactFilter : public b2ContactFilter {
    public:
    CustomContactFilter() {
        
    }
    ~CustomContactFilter() {
        
    }
        
    bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
};

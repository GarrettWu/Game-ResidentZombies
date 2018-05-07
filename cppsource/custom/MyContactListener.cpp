//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "MyContactListener.h"
#include "Global.h"
#include "BaseObject.h"

CustomContactListener::CustomContactListener() : _contacts() {
}

CustomContactListener::~CustomContactListener() {
}

void CustomContactListener::BeginContact(b2Contact* contact) {
    // We need to copy out the data because the b2Contact passed in
    // is reused.
    CustomContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    _contacts.push_back(myContact);
    
    b2Body *actorA = contact->GetFixtureA()->GetBody();
    b2Body *actorB = contact->GetFixtureB()->GetBody();
    BaseObject *userDataA = (BaseObject*)actorA->GetUserData();
    BaseObject *userDataB = ((BaseObject*)actorB->GetUserData());
    int tagA = userDataA->obj_type;
    int tagB = userDataB->obj_type;
    
    if(tagA == TYPE_MAO) {
        userDataA->beginContact(actorB);
    }
    else if (tagB == TYPE_MAO) {
        userDataB->beginContact(actorA); 
    }
    
    if (tagA == TYPE_ENEMY) {
        userDataA->beginContact(actorB);
    }
    else if (tagB == TYPE_ENEMY) {
        userDataB->beginContact(actorA); 
    }
    
    if (tagA == TYPE_BLOOD) {
        userDataA->beginContact(actorB);
    }
    else if (tagB == TYPE_BLOOD) {
        userDataB->beginContact(actorA);
    }
}

void CustomContactListener::EndContact(b2Contact* contact) {
    CustomContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<CustomContact>::iterator pos;
    pos = std::find(_contacts.begin(), _contacts.end(), myContact);
    if (pos != _contacts.end()) {
        _contacts.erase(pos);
    }
    
    b2Body *actorA = contact->GetFixtureA()->GetBody();
    b2Body *actorB = contact->GetFixtureB()->GetBody();
    BaseObject *userDataA = (BaseObject*)actorA->GetUserData();
    BaseObject *userDataB = ((BaseObject*)actorB->GetUserData());
    int tagA = userDataA->obj_type;
    int tagB = userDataB->obj_type;
    
    if(tagA == TYPE_MAO) {
        userDataA->endContact(actorB);
    }
    else if (tagB == TYPE_MAO) {
        userDataB->endContact(actorA); 
    }
    
    if (tagA == TYPE_ENEMY) {
        userDataA->endContact(actorB);
    }
    else if (tagB == TYPE_ENEMY) {
        userDataB->endContact(actorA); 
    }
}

void CustomContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	
	sGlobal->mainGameLayer->PreSolve(contact, oldManifold);
}

void CustomContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
}









bool CustomContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) {
    int typeA = ((BaseObject*)fixtureA->GetBody()->GetUserData())->obj_type;
    int typeB = ((BaseObject*)fixtureB->GetBody()->GetUserData())->obj_type;
//    LOGE("typeA:%d, typeB:%d", typeA, typeB);
    if ((typeA == TYPE_ENEMY && typeB == TYPE_GROUND) || (typeB == TYPE_ENEMY && typeA == TYPE_GROUND)) {
        return false;
    }
    
    return true;
}





//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "Intro.h"
#include "Global.h"

Intro::Intro(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj){
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    //空的sprite
    wySpriteEx *coinSprite = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "heart.png");
    obj_type = TYPE_INTRO;
    coinSprite->setScaleX(sGlobal->scaleX);
    coinSprite->setScaleY(sGlobal->scaleY);
    sGlobal->mainGameLayer->addChildLocked(coinSprite);
    coinSprite->setPosition(x, y);
    coinSprite->setVisible(false);
    container = coinSprite;
    
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y));
    bd.userData = this;
    bd.type = b2_staticBody;
    
    b2PolygonShape staticBox;
    float wd = m_box2d->pixel2Meter(DP(3));
    staticBox.SetAsBox(wd, m_box2d->pixel2Meter(wyDevice::winHeight*10));	
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;	
    fixtureDef.density = 0.0f;   
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = MASK_BITS[TYPE_INTRO];
    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_INTRO];
    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_INTRO];
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetTransform(bd.position, 0);
    
    name = objectsGroup->getObjectProperty(obj, "name");
}

Intro::~Intro() {
    
}

void Intro::update(float dt) {
    //LOGE("Enemy update...");
//    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
//    
//    wyNode *sprite = this->sprite;
//    sprite->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
}
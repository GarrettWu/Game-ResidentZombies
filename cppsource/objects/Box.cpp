//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "Box.h"
#include "Global.h"
#include "Bonus.h"

static void onDeadEnded(wyAFCSprite* sprite, void* data) {
    Box* box = (Box*)data;
    sGlobal->mainGameLayer->addToDestroy(box->body);
}

Box::Box(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj){
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    y += DP(32)*sGlobal->scaleY;
    
    container = wyNode::make();
    container->setPosition(x, y);
    container->setVisible(false);
    sGlobal->mainGameLayer->addChildLocked(container);
    
    spxSprite = wySPXSprite::make("xiangzi.sprite", false, MyTextureManager::makePNG("xiangzi.png"), 0);
    spxSprite->setScaleX(sGlobal->scaleX*1.2f);
    spxSprite->setScaleY(sGlobal->scaleY*1.2f);
    spxSprite->setForceTickMode(true);
	spxSprite->setUnitInterval(0.25f);
    spxSprite->setLoopCount(0);
    spxSprite->setPaused(true);
    spxSprite->setAnchor(0, 0);
    container->addChildLocked(spxSprite);
    
    obj_type = TYPE_BOX;
    
    float boxWidth = DP(40)*sGlobal->scaleX;
    float boxHeight = DP(50)*sGlobal->scaleY;
    
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + boxHeight));
    bd.userData = this;
    bd.type = b2_staticBody;
    
    b2PolygonShape staticBox;
    float wd = m_box2d->pixel2Meter(boxWidth);
    staticBox.SetAsBox(wd, m_box2d->pixel2Meter(boxHeight));	
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;	
    fixtureDef.density = 0.0f;   
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetTransform(bd.position, 0);
    body->SetActive(false);
    body->SetAwake(false);
    
    const char *item = objectsGroup->getObjectProperty(obj, "item");
    if (item != NULL) {
        if (strcmp(item, "coin") == 0) {
            getBonus(0, x, y);
        }
        else if (strcmp(item, "bigcoin") == 0) {
            getBonus(1, x, y);
        }
        else if (strcmp(item, "bigbigcoin") == 0) {
            getBonus(2, x, y);
        }
        else if (strcmp(item, "biao") == 0) {
            getBonus(3, x, y);
        }
        else if (strcmp(item, "xiezi") == 0) {
            getBonus(4, x, y);
        }
        else if (strcmp(item, "xue") == 0) {
            getBonus(5, x, y);
        }
        else if (strcmp(item, "ying") == 0) {
            getBonus(6, x, y);
        }
    } else {
        int seed = rand()%5+2;
        getBonus(seed, x, y);//
    }
    
    child->container->setVisible(false);
    child->body->SetActive(false);
    child->body->SetAwake(false);
}

Box::~Box() {
    
}

void Box::getBonus(int type, int x, int y) {
    switch (type) {
        case 0:
            child = BonusCoin::make(x, y + DP(30)*sGlobal->scaleY, COIN, true);
//            sGlobal->total_gold += 10;
            break;
        case 1:
            child = BonusCoin::make(x, y + DP(30)*sGlobal->scaleY, BIGCOIN, true);
//            sGlobal->total_gold += 50;
            break;
        case 2:
            child = BonusCoin::make(x, y + DP(30)*sGlobal->scaleY, BIGBIGCOIN, true);
//            sGlobal->total_gold += 100;
            break;
        case 3:
            child = BonusExtra::make(x, y + DP(30)*sGlobal->scaleY, BONUS_BIAO, true);
            break;
        case 4:
            child = BonusExtra::make(x, y + DP(30)*sGlobal->scaleY, BONUS_XIEZI, true);
            break;
        case 5:
            child = BonusExtra::make(x, y + DP(30)*sGlobal->scaleY, BONUS_XUE, true);
            break;
        case 6:
            child = BonusExtra::make(x, y + DP(30)*sGlobal->scaleY, BONUS_YING, true);
            break;
        default:
            break;
    }
}

void Box::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (!container->isVisible()) {
        container->setVisible(true);
        sGlobal->mainGameLayer->addToEnable(body);
    }
    spxSprite->tick(dt);
    
//    if (deadSprite->isVisible()) {
//        deadSprite->tick(dt);
//    }
}

void Box::dead() {
    sGlobal->mainGameLayer->addToDisable(body);
    
//    if (!deadSprite->isVisible()) {
//        deadSprite->setVisible(true);
//        deadSprite->playAnimation(0);
//        Bonus *bonus = (Bonus *) child;
//        bonus->show();
//        sGlobal->mainGameLayer->addToEnable(bonus->body);
//    }
    spxSprite->setPaused(false);
    Bonus *bonus = (Bonus *) child;
    bonus->show();
    sGlobal->mainGameLayer->addToEnable(bonus->body);
}


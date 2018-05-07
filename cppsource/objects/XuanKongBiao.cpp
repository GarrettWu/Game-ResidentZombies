//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "XuanKongBiao.h"
#include "Global.h"

XuanKongBiao::XuanKongBiao(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_NORMAL, objectsGroup, obj, myCmd){
    obj_type = TYPE_ENEMY;
    sub_type = 100;
    stunnable = false;
            
    e_id = ENEMY_XUANKONGBIAO;
    hp = 0;

    if (sGlobal->isRealEndless) {
        int idx = ENEMY_XIANJING1;
        switch (sGlobal->combatRound) {
            case 1:
                atk = sGlobal->ENEMY_ATK_EASY[idx];
                break;
            case 2:
                atk = sGlobal->ENEMY_ATK_HARD[idx];
                break;
            case 3:
                atk = sGlobal->ENEMY_ATK_CRAZY[idx];
                break;
            default:
                atk = sGlobal->ENEMY_ATK_CRAZY[idx] * (float)pow(sGlobal->ENDLESS_ATK_POWER, sGlobal->combatRound - 3);
                break;
        }
        
    } else {
        int idx = ENEMY_XIANJING1;
        switch (sGlobal->index/100) {
            case 1:
                idx = ENEMY_XIANJING1;
                break;
            case 2:
                idx = ENEMY_XIANJING2;
                break;
            case 3:
                idx = ENEMY_XIANJING3;
                break;
            default:
                idx = ENEMY_XIANJING1;
                break;
        }
        switch (sGlobal->currentSModeDifficulty) {
            case DIFFICULTY_NORMAL:
                atk = sGlobal->ENEMY_ATK_EASY[idx];
                break;
            case DIFFICULTY_HEROIC:
                atk = sGlobal->ENEMY_ATK_HARD[idx];
                break;
            case DIFFICULTY_HARD:
                atk = sGlobal->ENEMY_ATK_CRAZY[idx];
                break;
            default:
                atk = sGlobal->ENEMY_ATK_EASY[idx];
                break;
        }
    }
    
    gold = 0;
    
    atk *= sGlobal->ENEMY_ATK_MULTI;
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();

    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y));
    bd.userData = this;
    bd.type = b2_dynamicBody;
    body = world->CreateBody(&bd);
    body->SetTransform(bd.position, 0);
    
    b2PolygonShape staticBox;
    staticBox.SetAsBox(m_box2d->pixel2Meter(DP(28)*sGlobal->scaleX), m_box2d->pixel2Meter(DP(28)*sGlobal->scaleY));
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;	
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_TRAP];
    fixtureDef.filter.maskBits = MASK_BITS[TYPE_TRAP];
    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_TRAP];
    
    body->CreateFixture(&fixtureDef);

    body->SetActive(false);
    body->SetAwake(false);
    body->SetGravityScale(0);
    
    isAppeared = true;
    isReadyToCmd = true;
    
    isSFXPlayed = false;
    
    animIdle = SPX_XUANKONGBIAO;
    unitInterval = 0.15f;
    myScaleX = sGlobal->scaleX*0.65f;
    myScaleY = sGlobal->scaleY*0.65f;
    spxCacheArray = sGlobal->effects_cache;
}

XuanKongBiao::~XuanKongBiao() {
}

void XuanKongBiao::update(float dt) {
    Enemy::update(dt);
    
    if (sGlobal->mainGameLayer->hero->robertStatus == R_APPEARING && !isDead && isAppeared) {//
        sGlobal->nearestEnemy = NULL;
        sGlobal->nearestDistance = 1000;
    }
    
    if (!isSFXPlayed && container->isVisible()) {
        isSFXPlayed = true;
        sGlobal->playEffect(se_fly_dart);
        
        spxSprite->runAction(wyRepeatForever::make(wyRotateBy::make(0.7f, -360)));
    }
}

void XuanKongBiao::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

void XuanKongBiao::beginContact(b2Body *actorB) {
    Enemy::beginContact(actorB);
}

bool XuanKongBiao::canBeMissed() {
    return true;
}

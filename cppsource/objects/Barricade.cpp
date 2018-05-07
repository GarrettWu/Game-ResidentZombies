//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "Barricade.h"
#include "Global.h"

Barricade::Barricade(float x, float y) : Enemy(x, y, APPEAR_NORMAL, NULL, NULL, NULL, false){
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();

    e_id = ENEMY_ZHANGAI1;
    if (sGlobal->isRealEndless) {
        switch (sGlobal->combatRound) {
            case 1:
                hp = sGlobal->ENEMY_HP_EASY[e_id];
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                break;
            case 2:
                hp = sGlobal->ENEMY_HP_HARD[e_id];
                atk = sGlobal->ENEMY_ATK_HARD[e_id];
                gold = sGlobal->ENEMY_GOLD_HARD[e_id];
                break;
            case 3:
                hp = sGlobal->ENEMY_HP_CRAZY[e_id];
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id];
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id];
                break;
            default:
                hp = sGlobal->ENEMY_HP_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_HP_POWER, sGlobal->combatRound - 3);
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_ATK_POWER, sGlobal->combatRound - 3);
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_GOLD_POWER, sGlobal->combatRound - 3);
                break;
        }
//        LOGE("combat round:%d, hp:%d, atk:%d, gold:%d", sGlobal->combatRound, hp, atk, gold);
    } else {
        switch (sGlobal->currentSModeDifficulty) {
            case DIFFICULTY_NORMAL:
                hp = sGlobal->ENEMY_HP_EASY[e_id];
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                break;
            case DIFFICULTY_HEROIC:
                hp = sGlobal->ENEMY_HP_HARD[e_id];
                atk = sGlobal->ENEMY_ATK_HARD[e_id];
                gold = sGlobal->ENEMY_GOLD_HARD[e_id];
                break;
            case DIFFICULTY_HARD:
                hp = sGlobal->ENEMY_HP_CRAZY[e_id];
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id];
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id];
                break;
            default:
                hp = sGlobal->ENEMY_HP_EASY[e_id];
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                break;
        }
    }
    atk *= sGlobal->ENEMY_ATK_MULTI;
    hp *= sGlobal->ENEMY_HP_MULTI;
    gold *= sGlobal->ENEMY_GOLD_MULTI;
    
    isInitToShow = false;
    spxCacheArray = sGlobal->effects_cache;
    
    obj_type = TYPE_ENEMY;
    isMoving = false;

    float boxWidth = DP(40)*sGlobal->scaleX*0.8f;
    float boxHeight = DP(50)*sGlobal->scaleY*0.8f;
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + boxHeight + DP(45)*sGlobal->scaleY));
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
    fixtureDef.filter.maskBits = MASK_BITS[TYPE_ENEMY];
    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY];
    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY];
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetTransform(bd.position, 0);
    body->SetActive(false);
    body->SetAwake(false);
    
    isDead = false;
    
    originX = x;
    originY = y;
    
    stunnable = false;
    canKnockBack = false;
}

Barricade::~Barricade() {
    
}

void Barricade::initToShow() {
//    LOGE("cache len:%d", sGlobal->containerCache->num);
    container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
    container->setVisible(false);
    sGlobal->mainGameLayer->addChildLocked(container);
    
    spxSprite = (wySPXSprite*)sGlobal->popFromCache(sGlobal->effects_cache);
    spxSprite->setForceTickMode(true);
	spxSprite->setUnitInterval(0.1f);
    spxSprite->setLoopCount(0);
    spxSprite->setScaleX(sGlobal->scaleX*0.6f);
    spxSprite->setScaleY(sGlobal->scaleY*0.6f);
    spxSprite->playAnimation(SPX_QIYOUTONG);
    spxSprite->setVisible(true);
    spxSprite->setPosition(0, 0);
    container->addChildLocked(spxSprite);
    
    bloodSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
    sGlobal->mainGameLayer->addChildLocked(bloodSprite, 103);
    bloodSprite->setForceTickMode(true);
    bloodSprite->setUnitInterval(0.1f);
    bloodSprite->setLoopCount(0);
    bloodSprite->setFlipX(false);
    bloodSprite->setScaleX(1.8f*sGlobal->scaleX);
    bloodSprite->setScaleY(1.8f*sGlobal->scaleY);
    bloodSprite->setVisible(false);
    wyAFCSpriteCallback callback1 = {
        NULL,
        onBloodEnded
    };
    bloodSprite->setAFCSpriteCallback(&callback1, bloodSprite);
    bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
    
    deadSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
    sGlobal->mainGameLayer->addChildLocked(deadSprite, 102);
    deadSprite->setForceTickMode(true);
	deadSprite->setUnitInterval(0.03f);
    deadSprite->setLoopCount(0);
    deadSprite->setScaleX(2.0f*sGlobal->scaleX);
    deadSprite->setScaleY(2.0f*sGlobal->scaleY);
    wyAFCSpriteCallback callback = {
        NULL,
        onBDeadEnded
    };
    deadSprite->setVisible(false);
    deadSprite->setAFCSpriteCallback(&callback, this);
    deadSprite->playAnimation(SPX_WUTI_BAOZHA);
    
    deadSprite->setPosition(originX+DP(15)*sGlobal->scaleX, originY+DP(60)*sGlobal->scaleY);
    bloodSprite->setPosition(originX+DP(15)*sGlobal->scaleX, originY+DP(10)*sGlobal->scaleY);
    container->setPosition(originX, originY + DP(50)*sGlobal->scaleY*0.8f+DP(36)*sGlobal->scaleY);
    
    isAppeared = true;
//    LOGE("container posx:%f, posy:%f", container->getPositionX(), container->getPositionY());
}

void Barricade::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    if (!isInitToShow) {
        isInitToShow = true;
        initToShow();
    }
    
    if (sGlobal->mainGameLayer->hero->robertStatus == R_APPEARING && !isDead && isAppeared) {//
        float dist = distance;
        if (dist <= -DP(50)*sGlobal->scaleX) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        } else {
            if (dist < sGlobal->nearestDistance && dist > 0) {
                sGlobal->nearestEnemy = this;
                sGlobal->nearestDistance = dist;
            }
        }
    }
    
    if (!isDead && !container->isVisible()) {
        container->setVisible(true);
        sGlobal->mainGameLayer->addToEnable(body);
    }
    
    spxSprite->tick(dt);
    
    if (bloodSprite->isVisible()) {
        bloodSprite->tick(dt);
    }
    
    if (deadSprite->isVisible()) {
        deadSprite->tick(dt);
    }
}

void Barricade::dead() {
    if (isDead)
        return;
    
    if (sGlobal->nearestEnemy == this) {
        sGlobal->nearestEnemy = NULL;
        sGlobal->nearestDistance = 1000;
    }
    
    bloodSprite->setVisible(false);
    if (!deadSprite->isVisible()) {
        deadSprite->setVisible(true);
    }

    spxSprite->setVisible(false);
    showDeadEffect();

    sGlobal->playEffect(se_bomb);
    sGlobal->mainGameLayer->addToDisable(body);
    
    isDead = true;
}

bool Barricade::canBeHurt() {
    return true;
}

void Barricade::dealHitByDart(float y, b2Body *bulletBody) {
    if (hp < 0 || !canBeHurt()) {
        return;
    }
    
    if (canBeMissed()) {
        return;
    } else {
        HeroBullet *bullet = (HeroBullet *)bulletBody->GetUserData();
        bullet->dealHitEnemy(y, this);
    }
}

void Barricade::dealHitByLiudan(b2Body *bulletBody) {
    if (hp < 0 || isHurtByFire || !canBeHurt()) {
        return;
    }
    
    LiuDan *liudan = (LiuDan *)bulletBody->GetUserData();
    
    isHurtByFire = true;
    
    if (liudan->m_isMoto || hp - sGlobal->skillMulti[SKILL_MISSLE] <= 0) {
        hp -= sGlobal->skillMulti[SKILL_MISSLE];
        if (liudan->m_isMoto) {
            deadType = DEAD_TYPE_MOTO;
        } else {
            deadType = DEAD_TYPE_MISSLE;
        }
        dead();
    } else {
        hp -= sGlobal->skillMulti[SKILL_MISSLE];
        
        spxSprite->setColor(wyc3b(255, 0, 0));
        wyIntervalAction *delay = wyDelayTime::make(0.2f);
        wyActionCallback callback = {
            NULL,
            onHurtDelayEnded,
            NULL
        };
        delay->setCallback(&callback, spxSprite);
        delay->setTag(101);
        spxSprite->runAction(delay);
        
//        wyAction *action = spxSprite->getAction(101);
//        if (action == NULL || action->isDone()) {
//            spxSprite->setColor(wyc3b(255, 0, 0));
//            wyIntervalAction *delay = wyDelayTime::make(0.3f);
//            wyActionCallback callback = {
//                NULL,
//                onHurtDelayEnded,
//                NULL
//            };
//            delay->setCallback(&callback, spxSprite);
//            delay->setTag(101);
//            spxSprite->runAction(delay);
//        }
    }
}

void Barricade::hurt(float y) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    bloodSprite->setVisible(true);
    bloodSprite->playAnimation(SPX_JIZHONG_WUTI);
    bloodSprite->setPosition(container->getPositionX()-DP(15)*sGlobal->scaleX, m_box2d->meter2Pixel(y));
}

void Barricade::onHitHero() {
    deadType = DEAD_TYPE_DART;
    dead();
}

void Barricade::destroyMyself() {
    if (sGlobal->nearestEnemy == this) {
        sGlobal->nearestEnemy = NULL;
        sGlobal->nearestDistance = 1000;
    }
    
    spxSprite->stopAllActions();
    spxSprite->setAFCSpriteCallback(NULL, NULL);
    sGlobal->pushToCache(spxCacheArray, spxSprite);
    
    container->stopAllActions();
    container->removeAllChildren(true);
    sGlobal->pushToCache(sGlobal->containerCache, container);
    
    if (bloodSprite != NULL) {
        bloodSprite->stopAllActions();
        bloodSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(sGlobal->effects_cache, bloodSprite);
    }
    
    if (deadSprite != NULL) {
        deadSprite->stopAllActions();
        deadSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(sGlobal->effects_cache, deadSprite);
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    world->DestroyBody(body);

    this->release();
    
    return;
}

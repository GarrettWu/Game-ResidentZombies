//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "SpittleZombie.h"
#include "Global.h"
#include "EnemyFactory.h"
#include "FlyZombie1.h"

SpittleZombie::SpittleZombie(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, const char *myCmd) : Enemy(x, y, APPEAR_NORMAL, objectsGroup, obj, myCmd){
    e_id = ENEMY_TUZIGONGJIANSHOU;
    
    deadSFX = se_spittlezombie_dead;
    
    animIdle = 0;
    animWalk = 1;
    animWalkBack = 2;
    animHurt = 5;
    animAtk = 3;
    
    animJump = 6;
    animLanding = -1;
    animDead = -3;
    
    isFighted = false;
    
    unitInterval = 0.15f;
    myScaleX = sGlobal->scaleX*0.7f;
    myScaleY = sGlobal->scaleY*0.7f;
    spxCacheArray = sGlobal->enemy_1_cache;
    
    shadowSX = 3.0f;
    shadowSY = 1.6f;
    shadowPosX = DP(4)*sGlobal->scaleY;
    shadowPosY = -DP(55)*sGlobal->scaleY;
    
    fightDistance = sGlobal->virtualWinWidth * randRange(6, 8)/10.0f;
    
    afterInit(x, y);
    
    xuecaoSx = 0.32f*sGlobal->scaleX;
    xuecaoOfx = -DP(8)*sGlobal->scaleX;
    xuecaoOfy = DP(58)*sGlobal->scaleY;
}

SpittleZombie::~SpittleZombie() {
    
}

void SpittleZombie::update(float dt) {
    Enemy::update(dt);
    
    if (isDead) {
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (needAttack && !isFighted && !isStunning && isCmdFinished) {
        if (distance < fightDistance && distance > 0) {
            isAttacking = true;
            doShootArrowAnim();
        }
    }
}

void SpittleZombie::handleCollision(b2Body *actorB) {
    Enemy::handleCollision(actorB);
}

void SpittleZombie::beginContact(b2Body *actorB) {
    Enemy::beginContact(actorB);
}

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    SpittleZombie* enemy = (SpittleZombie*)data;
    if (enemy->isDead) {
        return;
    }

    if (sprite->getCurrentFrame() == 2) {
        enemy->shootArrow();
    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    SpittleZombie* enemy = (SpittleZombie*)data;
    if (enemy->isDead) {
        return;
    }
    
    sprite->setLoopCount(-1);
    sprite->setAFCSpriteCallback(NULL, NULL);
    if (!enemy->isOnGround) {
        sprite->playAnimation(enemy->animJump);
    } else {
        if (enemy->body->GetLinearVelocity().x > 1.0f) {
            sprite->playAnimation(enemy->animWalkBack);
        }
        else if (enemy->body->GetLinearVelocity().x < -1.0f){
            sprite->playAnimation(enemy->animWalk);
        }
        else {
            sprite->playAnimation(enemy->animIdle);
        }
    }
    
    enemy->isAttacking = false;
}

void SpittleZombie::doShootArrowAnim() {
    isFighted = true;

    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    enemySprite->setUnitInterval(unitInterval);
    enemySprite->setLoopCount(0);
    enemySprite->setAFCSpriteCallback(&callback, this);
    enemySprite->playAnimation(animAtk);
    
    enemySprite->setFlipX(false);
    
    if (isWandering) {
        body->SetLinearVelocity(b2Vec2(0,0));
    }
}

void SpittleZombie::shootArrow() {
//    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
////    b2World* world = m_box2d->getWorld();
//    
//    wySPXSprite *bullet = wySPXSprite::make("enemy_1"), MyTextureManager::makePNG("enemy_1")), 4);
////    wySpriteEx* bullet = wySpriteEx::make(MyTextureManager::makePNG("enemy_0")), wyr(DP(393),DP(191),DP(89),DP(34)));
//    bullet->setScaleX(sGlobal->scaleX*0.6f);
//    bullet->setScaleY(sGlobal->scaleY*0.6f);
//    bullet->setPaused(true);
//    sGlobal->mainGameLayer->addChildLocked(bullet);
//    
//    b2BodyDef bd;
//    bd.type = b2_dynamicBody;
//    bd.position.Set(body->GetPosition().x - m_box2d->pixel2Meter(DP(16)*sGlobal->scaleX), body->GetPosition().y + m_box2d->pixel2Meter(DP(30)*sGlobal->scaleY));
//    bd.gravityScale = 0.2f;
//    b2Body* bulletBody = m_box2d->getWorld()->CreateBody(&bd);
//    bulletBody->SetTransform(bd.position, 0);
//    b2PolygonShape box;
//    box.SetAsBox(m_box2d->pixel2Meter(DP(48/2)*sGlobal->scaleX), m_box2d->pixel2Meter(DP(13/2)*sGlobal->scaleY));
//    b2FixtureDef fd;
//    fd.shape = &box;
//    fd.restitution = 0.0f;
//    fd.friction = 0.0f;
//    fd.density = 0.0f;
//    fd.isSensor = true;
//    fd.filter.maskBits = MASK_BITS[TYPE_ENEMY_BULLET];
//    fd.filter.categoryBits = CAT_BITS[TYPE_ENEMY_BULLET];
//    fd.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY_BULLET];
//    
//    bulletBody->CreateFixture(&fd);
//    BaseObject *obj = WYNEW BaseObject();
//    obj->obj_type = TYPE_ENEMY_BULLET;
//    obj->container = bullet;
//    obj->body = bulletBody;
//    obj->atk = atk;
//    obj->hp = 10;
//    bulletBody->SetUserData(obj);
//    
//    float velx, vely;
//    velx = -20.0f;
//    vely = 2.0f;
//    
//    b2Vec2 f = bulletBody->GetWorldVector(b2Vec2(velx, vely));
//    b2Vec2 p = bulletBody->GetWorldPoint(b2Vec2(0.0f, 0.0f));
//    bulletBody->ApplyLinearImpulse(f, p);
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    EnemyBullet *enemyBullet = EnemyBullet::make(sGlobal->enemy_1_cache, 4, 0.1f, sGlobal->scaleX*0.5f, sGlobal->scaleY*0.5f);
    enemyBullet->atk = atk;
    b2Body *bulletBody = enemyBullet->body;
    bulletBody->SetTransform(b2Vec2(body->GetPosition().x - m_box2d->pixel2Meter(DP(55)*sGlobal->scaleX), body->GetPosition().y + m_box2d->pixel2Meter(DP(45)*sGlobal->scaleY)), 0);
    bulletBody->SetGravityScale(0.1f);

    float velx, vely;
    int seed = rand()%2;
    switch (seed) {
        case 0:
            velx = -7.0f;
            vely = 2.0f;
            break;
        case 1:
            velx = -16.0f;
            vely = -1.0f;
            break;
        default:
            break;
    }
    
    b2Vec2 f = bulletBody->GetWorldVector(b2Vec2(velx, vely));
    b2Vec2 p = bulletBody->GetWorldPoint(b2Vec2(0.0f, 0.0f));
    bulletBody->ApplyLinearImpulse(f, p);
    
    enemyBullet->spxSprite->setPosition(m_box2d->meter2Pixel(bulletBody->GetPosition().x), m_box2d->meter2Pixel(bulletBody->GetPosition().y));
    
    enemyBullet->needRotate = true;
//
//    wyHypotrochoidConfig m_circle;
//    m_circle = wyhcCircle(DP(50), 270, -90, enemyBullet->spxSprite->getPositionX()-DP(50), enemyBullet->spxSprite->getPositionY()+DP(50));
//    wyHypotrochoid* a = wyHypotrochoid::make(1.0f, m_circle);
//    a->setAutoRotate(true, 180);
//    wyIntervalAction *a1 = wySequence::make(wyMoveBy::make(0.2f, -DP(50), 0), a, wyMoveBy::make(1.0f, -wyDevice::winWidth, 0), NULL);
//    enemyBullet->spxSprite->runAction(a1);
}


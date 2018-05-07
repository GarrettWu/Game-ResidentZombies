//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "HeroBullet.h"
#include "Global.h"
#include "Boss1.h"
#include "Boss2.h"
#include "Boss3.h"

static void onBulletBombEnded(wyAFCSprite* sprite, void* data) {
    HeroBullet* bullet = (HeroBullet*)data;
    sGlobal->mainGameLayer->addToDestroy(bullet->body);
}

HeroBullet::HeroBullet(int bulletType, float velocityX, float velocityY, float posx, float posy) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    float velX, velY = 0;
    
    this->atkPercent = 1.0f;
    isDisabled = false;
    
    float dur = 0.2f;
    float w = DP(15)*sGlobal->scaleX;
    float h = DP(15)*sGlobal->scaleY;
    
    
//    fleshSprite->setScaleX(parent->spxSprite->getScaleX());
//    fleshSprite->setScaleY(parent->spxSprite->getScaleY());
//    fleshSprite->setContentSize(r.width*fleshSprite->getScaleX(), r.height*fleshSprite->getScaleY());
    Hero *hero = sGlobal->mainGameLayer->hero;
    isLaser = false;
    if (bulletType != WEAPON_RPG_BOMB && bulletType != WEAPON_BLASTING_BOMB) {
        wySpriteEx* bullet;
        switch (bulletType) {
            case WEAPON_MP5A5: {
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletMP5A5Rect);
                
                bullet->setScaleX(sGlobal->scaleX*0.7f);
                bullet->setScaleY(sGlobal->scaleY*0.7f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                x = DP(80)*sGlobal->scaleX;
                y = DP(32)*sGlobal->scaleY;
                
                w = DP(60)*sGlobal->scaleX;
                
                velX = BULLET_VEL_X*4.0f;
                
                dur = 0.2f;
                break;
            }
            case WEAPON_UMP: {
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletNormalRect);
                
                bullet->setScaleX(sGlobal->scaleX*1.0f);
                bullet->setScaleY(sGlobal->scaleY*1.0f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                x = DP(80)*sGlobal->scaleX;
                y = DP(36)*sGlobal->scaleY;
                
                w = DP(60)*sGlobal->scaleX;

                velX = BULLET_VEL_X*4.0f;
                
                dur = 0.4f;
                break;
            }
            case WEAPON_AK47:{
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletAK47Rect);
                
                bullet->setScaleX(sGlobal->scaleX*1.2f);
                bullet->setScaleY(sGlobal->scaleY*0.6f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                x = DP(95)*sGlobal->scaleX;
                y = DP(38)*sGlobal->scaleY;
                
                w = DP(60)*sGlobal->scaleX;
                
                velX = BULLET_VEL_X*10.0f;
                
                dur = 0.1f;
                
                bullet->runAction(wyScaleTo::make(1.0f, sGlobal->scaleX*1.2f, sGlobal->scaleY*0.6f, sGlobal->scaleX*2.0f, sGlobal->scaleY*0.6f));
                break;
            }
            case WEAPON_HYDRA:{
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletHYDRARect);
                
                bullet->setScaleX(sGlobal->scaleX*1.0f);
                bullet->setScaleY(sGlobal->scaleY*0.3f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                x = DP(95)*sGlobal->scaleX;
                y = DP(40)*sGlobal->scaleY;
                
                w = DP(60)*sGlobal->scaleX;
                
                velX = BULLET_VEL_X*4.0f;
                
                dur = 0;
                break;
            }
                
            case WEAPON_M870P: {
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletNormalRect);
                
                bullet->setScaleX(sGlobal->scaleX*1.0f);
                bullet->setScaleY(sGlobal->scaleY*1.0f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
//                bullet->setColor(wyc3b(255, 180, 0));
                
                x = DP(95)*sGlobal->scaleX;
                y = DP(40)*sGlobal->scaleY;
                
                w = DP(60)*sGlobal->scaleX;
                
                velX = BULLET_VEL_X*20.0f;
                
                dur = 0;
                break;
            }
            case WEAPON_GLB:{
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletTORPEDORect);
                
                bullet->setScaleX(sGlobal->scaleX*1.0f);
                bullet->setScaleY(sGlobal->scaleY*1.0f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                if (velY > 0) {
                    bullet->setRotation(-6);
                }
                else if (velY < 0) {
                    bullet->setRotation(6);
                }
                
                x = DP(80)*sGlobal->scaleX;
                y = DP(42)*sGlobal->scaleY;
                
                w = DP(60)*sGlobal->scaleX;
                h = DP(20)*sGlobal->scaleY;
                
                velX = BULLET_VEL_X*4.0f;
                velY = velocityY*7;
                break;
            }
            case WEAPON_GLR:
            case WEAPON_AER14: {
                container = NULL;

                w = (wyDevice::winWidth-sGlobal->hero_offset_left)/2;
                h = DP(15)*sGlobal->scaleY;

                isLaserDestroied = false;
                laserTimer = 0;

                x = (wyDevice::winWidth-sGlobal->hero_offset_left)/2 + DP(35)*sGlobal->scaleX;
                y = DP(38)*sGlobal->scaleY;
                
                if (sGlobal->mainGameLayer->hero->isMotoing) {
                    w *= 1.5f;
                    x = (wyDevice::winWidth-sGlobal->hero_offset_left)/2*1.5f + DP(35)*sGlobal->scaleX;
                }
                
                velX = 0;

                isLaser = true;
                break;
            }
            
            case WEAPON_F1:{
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletF1Rect);
                
                bullet->setScaleX(sGlobal->scaleX*0.6f);
                bullet->setScaleY(sGlobal->scaleY*0.6f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                x = DP(80)*sGlobal->scaleX;
                y = DP(38)*sGlobal->scaleY;
                
                w = DP(60)*sGlobal->scaleX;
                
                velX = BULLET_VEL_X*4.0f;
                
                dur = 0;
                break;
            }
            case WEAPON_E3:{
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletE3Rect);
                
                bullet->setScaleX(sGlobal->scaleX*0.6f);
                bullet->setScaleY(sGlobal->scaleY*0.5f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                x = DP(80)*sGlobal->scaleX;
                y = DP(34)*sGlobal->scaleY;
                
                w = DP(60)*sGlobal->scaleX;
                
                velX = BULLET_VEL_X*4.0f;
                
                dur = 0;
                break;
            }
            case WEAPON_M249: {
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletNormalRect);
                
                bullet->setScaleX(sGlobal->scaleX*1.0f);
                bullet->setScaleY(sGlobal->scaleY*1.0f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                x = DP(80)*sGlobal->scaleX;
                y = DP(12)*sGlobal->scaleY;
                
                w = DP(60)*sGlobal->scaleX;
                
                velX = BULLET_VEL_X*4.0f;
                
                dur = 0;
                break;
            }
            case WEAPON_DS2:{
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletDS2Rect);
                
                bullet->setScaleX(sGlobal->scaleX*1.0f);
                bullet->setScaleY(sGlobal->scaleY*0.15f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                if (velY > 0) {
                    bullet->setRotation(-6);
                }
                else if (velY < 0) {
                    bullet->setRotation(6);
                }
                
                x = DP(80)*sGlobal->scaleX;
                y = DP(42)*sGlobal->scaleY;
                
                w = DP(60)*sGlobal->scaleX;
                h = DP(20)*sGlobal->scaleY;
                
                velX = BULLET_VEL_X*4.0f;
                velY = velocityY*7;
                break;
            }
            case WEAPON_TORPEDO:{
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletTORPEDORect);
                
                bullet->setScaleX(sGlobal->scaleX*4.0f);
                bullet->setScaleY(sGlobal->scaleY*8.0f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                x = DP(80)*sGlobal->scaleX;
                y = DP(42)*sGlobal->scaleY;
                
                w = DP(100)*sGlobal->scaleX;
                h = DP(100)*sGlobal->scaleY;
                
                velX = BULLET_VEL_X*4.0f;
                break;
            }
            case WEAPON_M4A1: {
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletNormalRect);
                
                bullet->setScaleX(sGlobal->scaleX*0.8f);
                bullet->setScaleY(sGlobal->scaleY*1.0f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                if (velY > 0) {
                    bullet->setRotation(-4);
                }
                else if (velY < 0) {
                    bullet->setRotation(4);
                }
                
                x = DP(80)*sGlobal->scaleX;
                y = DP(42)*sGlobal->scaleY;
                
                w = DP(60)*sGlobal->scaleX;
                
                velX = BULLET_VEL_X*4.0f;
                velY = velocityY*6;
                break;
            }
            case WEAPON_BLASTING:{
                container = (wyNode*) sGlobal->popFromCache(sGlobal->containerCache);
                container->setVisible(true);
                sGlobal->mainGameLayer->addChildLocked(container, 1);
                
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletBLASTINGRect);
                
                bullet->setScaleX(sGlobal->scaleX*0.45f);
                bullet->setScaleY(sGlobal->scaleY*0.5f);
                bullet->setVisible(true);
                container->addChildLocked(bullet, 1, 1);
                
                x = DP(64)*sGlobal->scaleX;
                y = DP(40)*sGlobal->scaleY;
                
                dur = 0.0f;
                
                w = DP(40)*sGlobal->scaleX;
                h = DP(40)*sGlobal->scaleY;
                
                velX = BULLET_VEL_X*2.5f;
                break;
            }
            case WEAPON_RPG: {
                container = (wyNode*) sGlobal->popFromCache(sGlobal->containerCache);
                container->setVisible(true);
                sGlobal->mainGameLayer->addChildLocked(container, 1);
                
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletRPGRect);
                
                bullet->setScaleX(sGlobal->scaleX*0.32f);
                bullet->setScaleY(sGlobal->scaleY*0.32f);
                bullet->setVisible(true);
                container->addChildLocked(bullet, 1, 1);

                bullet_emitter = (wyParticleSystem*) sGlobal->popFromCache(sGlobal->smoke_emitterArray);
                bullet_emitter->setVisible(true);
                bullet_emitter->setScaleX(sGlobal->scaleX*0.2f);
                bullet_emitter->setScaleY(sGlobal->scaleY*0.2f);
                container->addChildLocked(bullet_emitter, 0, 0);
                bullet_emitter->setPositionType(FREE);
                bullet_emitter->setPosition(-DP(20)*sGlobal->scaleX, DP(2)*sGlobal->scaleX);
                bullet_emitter->setSpeedVariance(2500, 0);
                bullet_emitter->setEmissionRate(40);
                bullet_emitter->setLifeVariance(0.25, 0);
                bullet_emitter->setDirectionAngleVariance(180, 0);
                bullet_emitter->resetSystem();

                x = DP(64)*sGlobal->scaleX;
                y = DP(46)*sGlobal->scaleY;
                
                dur = 0.0f;
                
                w = DP(40)*sGlobal->scaleX;
                h = DP(60)*sGlobal->scaleY;
                
                velX = BULLET_VEL_X*2.5f;
                break;
            }
            case WEAPON_ROBOT_BULLET: {
                bullet = (wySpriteEx*) sGlobal->popFromCache(sGlobal->bulletSpriteCache);
                
                bullet->setTextureRect(hero->bulletRobotRect);
            
                bullet->setScaleX(sGlobal->scaleX*0.7f);
                bullet->setScaleY(sGlobal->scaleY*0.7f);
                sGlobal->mainGameLayer->bulletBatchNode->addChildLocked(bullet);
                container = bullet;
                
                dur = 0;
                
                x = posx;
                y = posy;
                
                velX = velocityX;
                velY = velocityY;
                break;
            }
            default:
                break;
        }
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.gravityScale = 0.0f;
        
        if (sGlobal->mainGameLayer->hero->isMotoing && bulletType != WEAPON_ROBOT_BULLET) {
            x += -DP(40)*sGlobal->scaleX;
            y += -DP(4)*sGlobal->scaleY;
        }
        bd.position.Set(m_box2d->pixel2Meter(sGlobal->mainGameLayer->hero->container->getPositionX()+sGlobal->mainGameLayer->hero->spx_hero_body->getPositionX()+ x), m_box2d->pixel2Meter(sGlobal->mainGameLayer->hero->container->getPositionY()+sGlobal->mainGameLayer->hero->spx_hero_body->getPositionY()+ y));

        b2Body* bulletBody = m_box2d->getWorld()->CreateBody(&bd);
        bulletBody->SetTransform(bd.position, 0);
        b2PolygonShape box;

        box.SetAsBox(m_box2d->pixel2Meter(w), m_box2d->pixel2Meter(h));
        
        b2FixtureDef fd;
        fd.shape = &box;
        fd.restitution = 0.0f;
        fd.friction = 0.0f;
        fd.density = 0.0f;
        fd.isSensor = true;
        
        if (bulletType == WEAPON_AER14 || bulletType == WEAPON_GLR || bulletType == WEAPON_TORPEDO) {
            this->catBit = sGlobal->getSeqNo();
        } else {
            this->catBit = CAT_BITS[TYPE_BULLET];
        }
//        this->catBit = sGlobal->getSeqNo();
        fd.filter.categoryBits = this->catBit;
        fd.filter.maskBits = MASK_BITS[TYPE_BULLET];
        fd.filter.groupIndex = GROUND_INDEX[TYPE_BULLET];
        
        bulletBody->CreateFixture(&fd);
        bulletBody->SetBullet(true);
        this->obj_type = TYPE_BULLET;
        this->body = bulletBody;
        this->body->SetUserData(this);
        
//        if (sGlobal->isRealEndless && sGlobal->isBoostOn) {
//            velX *= sGlobal->boostMulti;
//            velY *= sGlobal->boostMulti;
//        }
        body->SetLinearVelocity(b2Vec2(velX, velY));
        
        this->bulletType = bulletType;
        if (container != NULL) {
            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
//            container->runAction(wyFadeIn::make(dur));
        }
    } else {
        float ofy;
        wySPXSprite *baozha;
        if (bulletType == WEAPON_RPG_BOMB) {
            baozha = (wySPXSprite*) sGlobal->popFromCache(sGlobal->effects_cache);
            baozha->playAnimation(SPX_HUOJIAN_BAOZHA);
            baozha->setUnitInterval(0.04f);
            baozha->setScaleX(sGlobal->scaleX*2.0f);
            baozha->setScaleY(sGlobal->scaleY*2.0f);
            
            ofy = -DP(50)*sGlobal->scaleY;
        } else {
            baozha = (wySPXSprite*) sGlobal->popFromCache(sGlobal->dian_cache);
            baozha->playAnimation(0);
            baozha->setUnitInterval(0.04f);
            baozha->setScaleX(sGlobal->scaleX*1.2f);
            baozha->setScaleY(sGlobal->scaleY*1.2f);
            baozha->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
            
            ofy = 0;
        }
        
        baozha->setVisible(true);
        baozha->setForceTickMode(true);
        baozha->setLoopCount(0);
        wyAFCSpriteCallback callback = {
            NULL,
            onBulletBombEnded
        };
        baozha->setAFCSpriteCallback(&callback, this);
        sGlobal->mainGameLayer->addChildLocked(baozha, 103);
        spxSprite = baozha;
        container = baozha;
        
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(m_box2d->pixel2Meter(posx), m_box2d->pixel2Meter(posy));
        bd.gravityScale = 0.0f;
        b2Body* bulletBody = m_box2d->getWorld()->CreateBody(&bd);
        bulletBody->SetTransform(bd.position, 0);
        b2PolygonShape box;
        box.SetAsBox(m_box2d->pixel2Meter(DP(100)*sGlobal->scaleX), m_box2d->pixel2Meter(DP(100)*sGlobal->scaleY));
        b2FixtureDef fd;
        fd.shape = &box;
        fd.restitution = 0.0f;
        fd.friction = 0.0f;
        fd.density = 0.0f;
        fd.isSensor = true;
        this->catBit = sGlobal->getSeqNo();
        if (this->catBit == CAT_BITS[TYPE_BULLET]) {
            sGlobal->seqNo *= 2;
            this->catBit = sGlobal->seqNo;
        }
		fd.filter.categoryBits = this->catBit;
        fd.filter.maskBits = MASK_BITS[TYPE_BULLET];
        fd.filter.groupIndex = GROUND_INDEX[TYPE_BULLET];
        
        bulletBody->CreateFixture(&fd);
        
        this->obj_type = TYPE_BULLET;
        this->body = bulletBody;
        this->body->SetUserData(this);
        this->body->SetLinearVelocity(b2Vec2(0, 0));
        
        this->bulletType = bulletType;
        
        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y)+ofy);
        
        laserTimer = 0;
        isLaserDestroied = false;
    }

    targetVelX = body->GetLinearVelocity().x;
}

HeroBullet::~HeroBullet() {
    
}

void HeroBullet::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    if (bulletType == WEAPON_AER14 || bulletType == WEAPON_GLR) {
        return;
    }

    int dist = m_box2d->meter2Pixel(body->GetPosition().x - sGlobal->mainGameLayer->hero->body->GetPosition().x - sGlobal->mainGameLayer->hero->halfBodyWidthInMeter*2) + sGlobal->hero_offset_left;
    if (body->GetPosition().y < 0 || dist > wyDevice::winWidth) {
        isDisabled = true;
        if (dist > wyDevice::winWidth + DP(400)*sGlobal->scaleX) {
            sGlobal->mainGameLayer->addToDestroy(body);
            return;
        }
    }
    
    if (bulletType == WEAPON_RPG_BOMB || bulletType == WEAPON_BLASTING_BOMB) {
        laserTimer += dt;
        if (laserTimer > 0.1f && !isLaserDestroied) {
            isLaserDestroied = true;
            sGlobal->mainGameLayer->addToDisable(body);
        }
        spxSprite->tick(dt);
    } else {
        b2Vec2 vel = body->GetLinearVelocity();
        if (vel.x < targetVelX) {
            body->SetLinearVelocity(b2Vec2(targetVelX, body->GetLinearVelocity().y));
        }
        if (bulletType == WEAPON_ROBOT_BULLET) {
            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x)+DP(0)*sGlobal->scaleX, m_box2d->meter2Pixel(body->GetPosition().y));
        } else {
            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x)+DP(40)*sGlobal->scaleX, m_box2d->meter2Pixel(body->GetPosition().y));
        }
    }
}

static void onBulletFadeEnd(wyAction* action, void* data) {
    HeroBullet* bullet = (HeroBullet*)data;
    sGlobal->mainGameLayer->addToDestroy(bullet->body);
}

void HeroBullet::dealHitEnemy(float y, Enemy *enemy) {
    if (isDisabled) {
        return;
    }
    
    //n％几率眩晕0.5s
    if (enemy->stunnable && sGlobal->hasSkillMulti[SKILL_STUN] && bulletType != WEAPON_ROBOT_BULLET) {
//        LOGE("stun chance:%f", sGlobal->skillMulti[SKILL_STUN]*100);
        int seed = rand()%100;
        if (seed < sGlobal->skillMulti[SKILL_STUN]*100) {
            enemy->applyStun();
        }
    }

    if (bulletType != WEAPON_GLR && bulletType != WEAPON_AER14
        && bulletType != WEAPON_RPG && bulletType != WEAPON_BLASTING && bulletType != WEAPON_RPG_BOMB && bulletType != WEAPON_BLASTING_BOMB && bulletType != WEAPON_TORPEDO) {
        
        wyAction *a = wyFadeOut::make(0.1f);
        wyActionCallback callback = {
            NULL,
            onBulletFadeEnd,
            NULL
        };
        a->setCallback(&callback, this);
        container->runAction(a);
        container->runAction(wyScaleBy::make(0.1f, -container->getScaleX()*0.8f, 0));
        sGlobal->mainGameLayer->addToDisable(body);
    }
    int damage = sGlobal->getWeaponAtk();
    
    //击退
    if (bulletType == WEAPON_M870P || bulletType == WEAPON_HYDRA) {
        if (enemy->e_id != ENEMY_BOSS1 && enemy->e_id != ENEMY_BOSS2 && enemy->e_id != ENEMY_BOSS3) {
            enemy->knockBack();
        }
    }
    //穿透
    if (bulletType == WEAPON_AER14 || bulletType == WEAPON_GLR || bulletType == WEAPON_TORPEDO) {
        enemy->addBulletMask(this->catBit);
    }
    //火箭
    if (bulletType == WEAPON_RPG || bulletType == WEAPON_BLASTING) {
        makeBomb(bulletType);
        return;
    }
    //机器人子弹
    if (bulletType == WEAPON_ROBOT_BULLET) {
        damage = sGlobal->skillMulti[SKILL_ROBOT];
    }
    //火箭爆炸
    if (bulletType == WEAPON_RPG_BOMB || bulletType == WEAPON_BLASTING_BOMB) {
        enemy->addBulletMask(this->catBit);
    }

    damage *= atkPercent;
    
    //n％几率2倍暴击
    if (sGlobal->hasSkillMulti[SKILL_CRITICAL] && bulletType != WEAPON_ROBOT_BULLET) {
        int seed = rand()%100;
        if (seed < sGlobal->skillMulti[SKILL_CRITICAL]*100) {
            //1.5~2
            float beishu = (int)randRange(18, 23)/10.0f;
            damage *= beishu;
//            LOGE("critical: %f, %d", beishu, damage);
            
            enemy->showCriticalEffect(damage);
        }
    }
    
//    //5％几率吸血，血量为攻击的n％
//    if (sGlobal->hasSkillMulti[SKILL_6]) {
//        int seed = rand()%100;
//        if (seed < ELEMENT_SKILL_6_PARAM1) {
//            int recoverHP = MIN(sGlobal->cloth_hp-sGlobal->curHP, ceil(damage*sGlobal->skillMulti[SKILL_6]));;
//            sGlobal->hudLayer->setHPBar(-recoverHP);
////            LOGE("leech HP: %d", recoverHP);
//            
//            sGlobal->mainGameLayer->hero->showRecoverHPEffect(recoverHP, false);
//        }
//    }
    
    if (enemy->hp - damage <= 0) {
        hp -= damage;
        enemy->deadType = DEAD_TYPE_DART;
        enemy->dead();
    } else {
        enemy->hp -= damage;
        enemy->hurt(y);
    }
    
//    LOGE("enemy->hp:%d", enemy->hp);
    if (bulletType == WEAPON_RPG_BOMB || bulletType == WEAPON_BLASTING_BOMB) {
        sGlobal->playEffect(se_bomb);
    } else {
        sGlobal->playEffect(se_hit);
    }
}

void HeroBullet::makeBomb(int weaponId) {
    int bombType;
    switch (weaponId) {
        case WEAPON_RPG:
            bombType = WEAPON_RPG_BOMB;
            break;
        case WEAPON_BLASTING:
            bombType = WEAPON_BLASTING_BOMB;
            break;
        default:
            break;
    }
    new HeroBullet(bombType, 0, 0, container->getPositionX(), container->getPositionY());
    sGlobal->mainGameLayer->addToDestroy(body);
}

void HeroBullet::destroyMe() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    switch (bulletType) {
        case WEAPON_AER14:
        case WEAPON_GLR:
            
            break;
        case WEAPON_BLASTING:{
            wySpriteEx *bullet = (wySpriteEx*)container->getChildByTag(1);
            sGlobal->pushToCache(sGlobal->bulletSpriteCache, bullet);
            
            container->stopAllActions();
            container->removeAllChildren(true);
            sGlobal->pushToCache(sGlobal->containerCache, container);
            break;
        }
            
        case WEAPON_RPG: {
            wySpriteEx *bullet = (wySpriteEx*)container->getChildByTag(1);
            wyParticleSystem *emitter = (wyParticleSystem*)container->getChildByTag(0);

            sGlobal->pushToCache(sGlobal->bulletSpriteCache, bullet);
            
            emitter->setVisible(false);
            emitter->stopSystem();
//            emitter->resetSystem();

            sGlobal->pushToCache(sGlobal->smoke_emitterArray, emitter);
            
            container->stopAllActions();
            container->removeAllChildren(true);
            sGlobal->pushToCache(sGlobal->containerCache, container);
            break;
        }
        case WEAPON_BLASTING_BOMB:{
            spxSprite->stopAllActions();
            spxSprite->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->dian_cache, spxSprite);
            break;
        }
        case WEAPON_RPG_BOMB: {
            spxSprite->stopAllActions();
            spxSprite->setAFCSpriteCallback(NULL, NULL);
            sGlobal->pushToCache(sGlobal->effects_cache, spxSprite);
            break;
        }
        default:
//            container->stopAllActions();
            sGlobal->pushToCache(sGlobal->bulletSpriteCache, container);
            break;
    }
    
    
    world->DestroyBody(body);
    delete this;
}

void HeroBullet::launch() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    x = DP(64)*sGlobal->scaleX;
    y = DP(46)*sGlobal->scaleY;
    
    float velX = BULLET_VEL_X*2.5f;
    
    body->SetTransform(b2Vec2(m_box2d->pixel2Meter(sGlobal->mainGameLayer->hero->container->getPositionX() + x), m_box2d->pixel2Meter(sGlobal->mainGameLayer->hero->container->getPositionY() + y)), 0);
    body->SetLinearVelocity(b2Vec2(velX, 0));
}



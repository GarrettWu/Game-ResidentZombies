//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "EndPoint.h"
#include "Global.h"
#include "MainScene.h"

EndPoint::EndPoint(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj, bool needShow){
    this->needShow = needShow;
    offsetX = 0;
    offsetY = 0;
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    float wd = m_box2d->pixel2Meter(DP(160)*sGlobal->scaleX);

    type = END_NORMAL;
    
    container = wyNode::make();
    container->setPosition(x, y - DP(2)*sGlobal->scaleY);
    sGlobal->mainGameLayer->addChildLocked(container);
    
    wySpriteEx *shadow = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shadow.png");
    shadow->setScaleX(sGlobal->scaleX*2.5f);
    shadow->setScaleY(sGlobal->scaleY*1.5f);
    container->addChildLocked(shadow, -1, 0);
    shadow->setAlpha(100);
    shadow->setPosition(DP(10)*sGlobal->scaleY, DP(3)*sGlobal->scaleY);
    
    women = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), SPX_NVREN_DAIJI);
    women->setForceTickMode(true);
	women->setUnitInterval(0.2f);
    women->setScaleX(0.80f*sGlobal->scaleX);
    women->setScaleY(0.78f*sGlobal->scaleY);
    women->setLoopCount(-1);
    container->addChildLocked(women, 1, 1);
    
//    }
    
    obj_type = TYPE_ENDPOINT;
//    container->setScaleX(sGlobal->scaleX);
//    container->setScaleY(sGlobal->scaleY);
//    container->setAnchor(0.5f, 0);
    if (!sGlobal->hasReachCheckPoint)
        container->setVisible(false);
    
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(0));
    bd.userData = this;
    bd.type = b2_staticBody;
    
    b2PolygonShape staticBox;
    staticBox.SetAsBox(wd, m_box2d->pixel2Meter(wyDevice::winHeight*10));	
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;	
    fixtureDef.density = 0.0f;   
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = MASK_BITS[TYPE_ENDPOINT];
    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENDPOINT];
    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENDPOINT];
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetTransform(bd.position, 0);
}

EndPoint::~EndPoint() {
    
}

void EndPoint::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    float distance = m_box2d->meter2Pixel(body->GetPosition().x)+offsetX-m_box2d->meter2Pixel(sGlobal->mainGameLayer->hero->body->GetPosition().x);
    
    if (distance < wyDevice::winWidth) {
        if (!container->isVisible() && needShow) {
            container->setVisible(true);
            if (type == END_ILLUSION || type == END_TUOLUO) {
                wyMoveBy* m = wyMoveBy::make(1.0f, 0, DP(8));
                wyIntervalAction* t = wySequence::make(m, (wyFiniteTimeAction*)m->reverse(), NULL);
                container->runAction(wyRepeatForever::make(t));
            }
        }
        if (type == END_ILLUSION || type == END_TUOLUO) {
            guang->tick(dt);
        }
        if (type == END_BOSS1) {
            spxSprite->tick(dt);
            boss1Timer += dt;
            if (!isBoss1Talking) {
                if (boss1Timer > 2.0f) {
                    showDialogBox(0);
                    isBoss1Talking = true;
                }
            } else {
                if (boss1Timer > 6.0f && !isBoss1Ending) {
                    doEnd();
                    isBoss1Ending = true;
                }
            }
        }

        women->tick(dt);
    }
}

void EndPoint::showDialogBox(int idx) {
    wyColor3B subColor = wyc3b(255, 192, 0);

    wyBitmapFontLabel *content;
    content = wyBitmapFontLabel::make(sGlobal->font_desc, sGlobal->getStr(STR_BOSS1_TALK));
    content->setScale(SP(22)*sGlobal->scaleX/sGlobal->descFontSize);
    content->setAlignment(wyBitmapFontLabel::LEFT);
    content->setLineWidth(DP(220)*sGlobal->scaleX/content->getScale());
    content->setPosition(container->getPositionX() - DP(250)*sGlobal->scaleX, container->getPositionY() + DP(176)*sGlobal->scaleY);
    content->setColor(wyc3b(255, 255, 0));
    content->setAnchor(0, 1.0f);
    sGlobal->mainGameLayer->addChildLocked(content, container->getZOrder()-1);
}

void EndPoint::doEnd() {
//    sGlobal->saveMapResult();
    sGlobal->isBoss1Strong = true;
    
//    ((MainScene*)sGlobal->mainGameLayer->getParent())->goToBoss1Fight();
}

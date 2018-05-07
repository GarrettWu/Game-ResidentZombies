//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "NPC.h"
#include "Global.h"
#include "LoadingScene.h"
#include "MainScene.h"

NPC::NPC(float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj){
    offsetX = 0;
    offsetY = 0;
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    container = wyNode::make();
    container->setVisible(false);
    sGlobal->mainGameLayer->addChildLocked(container);
    
    wySPXSprite *checkPointSprite = wySPXSprite::make("npc.sprite", false, MyTextureManager::makePNG("npc.png"), 4);
    checkPointSprite->setForceTickMode(true);
    checkPointSprite->setUnitInterval(0.1f);
    checkPointSprite->setLoopCount(-1);
    checkPointSprite->setScaleX(sGlobal->scaleX*1.5f);
    checkPointSprite->setScaleY(sGlobal->scaleY*1.5f);
    container->addChildLocked(checkPointSprite);
    spxSprite = checkPointSprite;
    
    obj_type = TYPE_NPC;
    
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y));
    bd.userData = this;
    bd.type = b2_dynamicBody;
    
    b2PolygonShape staticBox;
    float wd = m_box2d->pixel2Meter(DP(1));
    staticBox.SetAsBox(wd, m_box2d->pixel2Meter(DP(1)));	
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;	
    fixtureDef.density = 0.0f;   
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetTransform(bd.position, 0);
    body->SetGravityScale(0);
    
    isActing = false;
    isMoved = false;
    isTalked = false;
    isEnded = false;
    moveTimer = 0;
    duration = 0;
}

NPC::~NPC() {
    
}

void NPC::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (!container->isVisible())
        container->setVisible(true);
    
    spxSprite->tick(dt);
    container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
    
    if (isActing) {
        moveTimer += dt;
        if (moveTimer > duration && !isMoved) {
            body->SetLinearVelocity(b2Vec2(0,0));
            isMoved = true;
        }
        if (moveTimer > duration + 0.5f && !isTalked) {
            showDialogBox(0);
            isTalked = true;
        }
        if (moveTimer > duration + 1.5f && !isEnded) {
            isEnded = true;
            isActing = false;
            doEnd();
        }
    }
}

void NPC::move(float velX, float duration) {
    body->SetLinearVelocity(b2Vec2(velX, 0));
    this->duration = duration;
    isActing = true;
    moveTimer = 0;
    isMoved = false;
    isTalked = false;
    isEnded = false;
}

void NPC::showDialogBox(int idx) {
//    wySprite *dialogBox = wyZwoptexManager::getInstance()->makeSprite("main_game_01", "npc_dialog.png");
//    dialogBox->setAnchor(1.0f, 0);
//    dialogBox->setScaleX(sGlobal->scaleX);
//    dialogBox->setScaleY(sGlobal->scaleY);
//    dialogBox->setPosition(container->getPositionX() + DP(20)*sGlobal->scaleX, container->getPositionY() + DP(70)*sGlobal->scaleY);
//    sGlobal->mainGameLayer->addChildLocked(dialogBox);
    
    wyColor3B subColor = wyc3b(255, 192, 0);

    wyBitmapFontLabel *content;
    content = wyBitmapFontLabel::make(sGlobal->font_desc, "aaa");
    content->setScale(SP(22)*sGlobal->scaleX/sGlobal->descFontSize);
    content->setAlignment(wyBitmapFontLabel::LEFT);
    content->setLineWidth(DP(220)*sGlobal->scaleX/content->getScale());
    content->setPosition(container->getPositionX() - DP(250)*sGlobal->scaleX, container->getPositionY() + DP(176)*sGlobal->scaleY);
    content->setColor(wyc3b(255, 255, 0));
    content->setAnchor(0, 1.0f);
    sGlobal->mainGameLayer->addChildLocked(content);
}

void NPC::doEnd() {
	MyPrefs::setInt(DB_GOLD, sGlobal->getGold());

//    ((MainScene*)sGlobal->mainGameLayer->getParent())->backToMap();
}

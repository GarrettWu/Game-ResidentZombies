//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "BaseObject.h"
#include "Global.h"

BaseObject::BaseObject(){
    sub_type = -1;
    offsetX = 0;
    offsetY = 0;
    isSensor = false;
    
    spxScaleX = 1.0f;
    
    startX = -1;
    endX = 65535;
    
    isDestroied = false;
    isRecreatingFixture = false;
}

BaseObject::~BaseObject() {
}

void BaseObject::update(float dt) {
    //LOGE("BaseObject update...");
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
}

void BaseObject::handleCollision(b2Body *actorB) {
    
}

void BaseObject::beginContact(b2Body *actorB) {
    
}

void BaseObject::endContact(b2Body *actorB) {
    
}

void BaseObject::createFixtureByCurrentFrame() {
    createFixtureByCurrentFrame(false);
}

void BaseObject::createFixtureByCurrentFrame(bool isSensor) {
    this->isSensor = isSensor;
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
	uint16 prevMaskBit = MASK_BITS[TYPE_ENEMY];
	b2Vec2 vel = body->GetLinearVelocity();
    //sharyu: 先删除所有fixture
    b2Fixture *fixture = body->GetFixtureList();
    b2Fixture *next;
    
    body->SetActive(false);
    body->SetAwake(false);
    while (fixture != NULL) {
		b2Filter filter = fixture->GetFilterData();
		prevMaskBit = filter.maskBits;
        next = fixture->GetNext();
        body->DestroyFixture(fixture);
        fixture = next;
    }
    body->SetActive(true);
    body->SetAwake(true);
    
    //sharyu: 根据spx中定义的碰撞矩形或默认图的矩形生成fixture
//    wyRect newRect = spxSprite->getFrameRect();
//    newRect.x *= spxSprite->getScaleX();
//    newRect.y *= spxSprite->getScaleY();
//    newRect.width *= spxSprite->getScaleX();
//    newRect.height *= spxSprite->getScaleY();
//    LOGE("recreate fixture:%d,%d", spxSprite->getCurrentAnimationIndex(), spxSprite->getCurrentFrame());
    wyRect newRect = bodyRect;
//    wyRect newRect = wyr(rect.x * spxSprite->getScaleX(), rect.y * spxSprite->getScaleY(), rect.width * spxSprite->getScaleX(), rect.height * spxSprite->getScaleY());
    int collisionRectCount = spxSprite->getCollisionRectCount();
    if (collisionRectCount > 0) {
        for (int i = 0; i < collisionRectCount; i++) {
            wyRect curRect = spxSprite->getCollisionRect(i);
            curRect.x *= spxSprite->getScaleX();
            curRect.y *= spxSprite->getScaleY();
            curRect.width *= spxSprite->getScaleX();
            curRect.height *= spxSprite->getScaleY();
            //LOGE("collrect: %f,%f, %f, %f", curRect.width, curRect.height, spxSprite->getScaleX(), spxSprite->getScaleY());
            
            b2PolygonShape dynamicBox;
            if (i == -1) {
                dynamicBox.SetAsBox(m_box2d->pixel2Meter(curRect.width/2)*spxScaleX, m_box2d->pixel2Meter(curRect.height/2));
                ofyInMeter = 0;
            } else {
                float centerY = m_box2d->pixel2Meter((curRect.y + curRect.height/2 - newRect.y - newRect.height/2));
                dynamicBox.SetAsBox(m_box2d->pixel2Meter(curRect.width/2)*spxScaleX, m_box2d->pixel2Meter(curRect.height/2), b2Vec2(m_box2d->pixel2Meter((curRect.x + curRect.width/2 - newRect.x - newRect.width/2)), centerY), 0);
                
//                ofyInMeter = m_box2d->pixel2Meter(curRect.height/2-(curRect.y + curRect.height/2 - newRect.y - newRect.height/2));
                ofyInMeter = centerY;
                
//                LOGE("w:%f, h:%f, ofx:%f, ofy:%f, ofyInMeter:%f", m_box2d->pixel2Meter(curRect.width/2)*spxScaleX, m_box2d->pixel2Meter(curRect.height/2), m_box2d->pixel2Meter((curRect.x + curRect.width/2 - newRect.x - newRect.width/2)), m_box2d->pixel2Meter((curRect.y + curRect.height/2 - newRect.y - newRect.height/2)), ofyInMeter);
            }
//            dynamicBox.SetAsBox(m_box2d->pixel2Meter(curRect.width/2), m_box2d->pixel2Meter(curRect.height/2), b2Vec2(m_box2d->pixel2Meter((curRect.x + curRect.width/2 - newRect.x - newRect.width/2)), m_box2d->pixel2Meter((curRect.y + curRect.height/2 - newRect.y - newRect.height/2))), 0);
            
//            if (i == 0) {
//                float width = fabs(dynamicBox.m_vertices[0].x);
//                float height = fabs(dynamicBox.m_vertices[0].y);
//                float32 edgeWidth = 0.05f;  // Add chamfered corners to allow the body to slide over edges
//                float32 edgeHeight = 0.05f;
//                b2Vec2 vertices[6];
//                vertices[0].Set(-width + edgeWidth, -height);	  // bottom
//                vertices[1].Set(width - edgeWidth, -height);       // bottom-right edge start
//                vertices[2].Set(width, -height + edgeHeight);     // bottom-right edge end
//                vertices[3].Set(width, height);                            // top-right
//                vertices[4].Set(-width, height);                          // top-left
//                vertices[5].Set(-width, -height + edgeHeight);  // bottom-left edge
//                dynamicBox.Set(vertices, 6);
//            }
//            LOGE("vertex y: %f/%f;%f/%f;%f/%f;%f/%f/%f", dynamicBox.m_vertices[0].x, dynamicBox.m_vertices[0].y, dynamicBox.m_vertices[1].x, dynamicBox.m_vertices[1].y, dynamicBox.m_vertices[2].x, dynamicBox.m_vertices[2].y, dynamicBox.m_vertices[3].x, dynamicBox.m_vertices[3].y,m_box2d->pixel2Meter((curRect.y + curRect.height/2 - newRect.y - newRect.height/2)));
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 0.0f;
            fixtureDef.friction = 0.0f;
            fixtureDef.restitution = 0.0f;
            fixtureDef.isSensor = isSensor;
			fixtureDef.filter.maskBits = prevMaskBit;
            fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY];
            fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY];
            
            body->CreateFixture(&fixtureDef);
//            LOGE("0: %f,%f", dynamicBox.m_vertices[0].x, dynamicBox.m_vertices[0].y);
//            LOGE("1: %f,%f", dynamicBox.m_vertices[1].x, dynamicBox.m_vertices[1].y);
//            LOGE("2: %f,%f", dynamicBox.m_vertices[2].x, dynamicBox.m_vertices[2].y);
//            LOGE("3: %f,%f", dynamicBox.m_vertices[3].x, dynamicBox.m_vertices[3].y);
            if (i == 0) {
                halfBodyWidthInMeter = m_box2d->pixel2Meter(curRect.width/2);
                halfBodyHeightInMeter = m_box2d->pixel2Meter(curRect.height/2);
                
//                float diffX = (curRect.x + curRect.width/2 - newRect.x - newRect.width/2);
//                float diffY = ((curRect.y + curRect.height/2 - newRect.y - newRect.height/2));
//                body->SetTransform(b2Vec2(body->GetPosition().x - m_box2d->pixel2Meter(diffX), body->GetPosition().y - m_box2d->pixel2Meter(diffY)), 0);
//                offsetX = -(curRect.x + curRect.width/2 - diffX);
//                offsetY = -(curRect.y + curRect.height/2 - diffY);
//                bodyRect = curRect;
                
                float diffX = ((newRect.x + newRect.width/2) - (bodyRect.x + bodyRect.width/2));
                float diffY = ((newRect.y + newRect.height/2) - (bodyRect.y + bodyRect.height/2));
                float yyy = 0;
                if (!isSensor) {
                    yyy = DP(4)*sGlobal->scaleY;
                }
                body->SetTransform(b2Vec2(body->GetPosition().x + m_box2d->pixel2Meter(diffX), body->GetPosition().y + m_box2d->pixel2Meter(diffY+yyy)), 0);
                offsetX = -(newRect.x + newRect.width/2);
                offsetY = -(newRect.y + newRect.height/2);
                bodyRect = newRect;
            }
        }
    } else {
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(m_box2d->pixel2Meter(newRect.width/2)*spxScaleX, m_box2d->pixel2Meter(newRect.height/2));

//        float width = fabs(dynamicBox.m_vertices[0].x);
//        float height = fabs(dynamicBox.m_vertices[0].y);
//        float32 edgeWidth = 0.05f;  // Add chamfered corners to allow the body to slide over edges
//        float32 edgeHeight = 0.05f;
//        b2Vec2 vertices[6];
//        vertices[0].Set(-width + edgeWidth, -height);	  // bottom
//        vertices[1].Set(width - edgeWidth, -height);       // bottom-right edge start
//        vertices[2].Set(width, -height + edgeHeight);     // bottom-right edge end
//        vertices[3].Set(width, height);                            // top-right
//        vertices[4].Set(-width, height);                          // top-left
//        vertices[5].Set(-width, -height + edgeHeight);  // bottom-left edge
//        dynamicBox.Set(vertices, 6);
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.isSensor = isSensor;
		fixtureDef.filter.maskBits = prevMaskBit;
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY];
        
        body->CreateFixture(&fixtureDef);
//        LOGE("0: %f,%f", dynamicBox.m_vertices[0].x, dynamicBox.m_vertices[0].y);
//        LOGE("1: %f,%f", dynamicBox.m_vertices[1].x, dynamicBox.m_vertices[1].y);
//        LOGE("2: %f,%f", dynamicBox.m_vertices[2].x, dynamicBox.m_vertices[2].y);
//        LOGE("3: %f,%f", dynamicBox.m_vertices[3].x, dynamicBox.m_vertices[3].y);
        
        halfBodyWidthInMeter = m_box2d->pixel2Meter(newRect.width/2);
        halfBodyHeightInMeter = m_box2d->pixel2Meter(newRect.height/2);
        
        float diffX = ((newRect.x + newRect.width/2) - (bodyRect.x + bodyRect.width/2));
        float diffY = ((newRect.y + newRect.height/2) - (bodyRect.y + bodyRect.height/2));
        body->SetTransform(b2Vec2(body->GetPosition().x + m_box2d->pixel2Meter(diffX), body->GetPosition().y + m_box2d->pixel2Meter(diffY)), 0);
        offsetX = -(newRect.x + newRect.width/2);
        offsetY = -(newRect.y + newRect.height/2);
        bodyRect = newRect;
        
        ofyInMeter = 0;
    }

//    container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
	body->SetLinearVelocity(vel);
    
//    LOGE("%f, %f, %f, %f, %f, %f", bodyRect.x, bodyRect.y, bodyRect.width, bodyRect.height, offsetX, offsetY);
//    halfBodyHeightInMeter = m_box2d->pixel2Meter(bodyRect.height/2);
//    halfBodyWidthInMeter = m_box2d->pixel2Meter(bodyRect.width/2);
    
//    LOGE("obj height: %f", halfBodyHeightInMeter);
}

void BaseObject::onHitHero() {
    
}



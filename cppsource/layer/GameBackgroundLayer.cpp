//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameBackgroundLayer.h"
#include "Global.h"

GameBackgroundLayer::GameBackgroundLayer(int mapIndex) {
    sGlobal->gameBackgroundLayer = this;
    
    emitter = NULL;
    lightPos = NULL;
    leftX = DP(-100)*sGlobal->scaleX;
    eagle = NULL;
    eagle1 = NULL;
    eagle2 = NULL;
    
    this->bgIndex = mapIndex / 100;
    switch (bgIndex) {
        case 0:
        case 1: {
            initSeason1();
            break;
        }
        case 2: {
            initSeason2();
            
            break;
        }
        case 3: {
            initSeason3();
            
            break;
        }
        case 4:            
//            particleZ = 31;
            
            break;
        default:
            break;
    }
}

GameBackgroundLayer::~GameBackgroundLayer() {
    if (lightPos != NULL) {
        delete []lightPos;
    }
    
}

void GameBackgroundLayer::onEnter() {
    wyLayer::onEnter();
}

void GameBackgroundLayer::initSeason1() {
    tp1x = 0;
    tp1y = DP(-40)*sGlobal->scaleY;
    wyTexture2D *tex = MyTextureManager::makePNG("bamboo_0.png", WY_TEXTURE_PIXEL_FORMAT_RGBA8888);
    tp1 = wyTiledSprite::make(tex);
    tp1->setScaleX(sGlobal->scaleX*0.9f);
    tp1->setScaleY(sGlobal->scaleY*0.9f);
    tp1->setContentSize(wyDevice::winWidth / sGlobal->scaleX/0.9f, (wyDevice::winHeight + DP(266)*sGlobal->scaleY/0.9f) / sGlobal->scaleY / 0.9f);
    tp1->setTileDirection(true, false);
    tp1->setAnchor(0, 0);
    tp1->setPosition(tp1x, tp1y);
    addChildLocked(tp1, 10);
    
    tp2x = 0;
    tp2y = DP(-30)*sGlobal->scaleY;
    tex = MyTextureManager::makePNG("bamboo_2.png", WY_TEXTURE_PIXEL_FORMAT_RGBA8888);
    tp2 = wyTiledSprite::make(tex);
    tp2->setScaleX(sGlobal->scaleX*0.8f);
    tp2->setScaleY(sGlobal->scaleY*0.8f);
    
    maxX = tex->getWidth()*tp2->getScaleX();
    
    tp2->setContentSize(wyDevice::winWidth / sGlobal->scaleX/0.8f, wyDevice::winHeight / sGlobal->scaleY/0.8f);
    tp2->setTileDirection(true, false);
    tp2->setAnchor(0, 0);
    tp2->setPosition(tp2x, tp2y);
    addChildLocked(tp2, 30);
    
    lightPos = new float[8];
    lightPos[0] = DP(265)*sGlobal->scaleX;
    lightPos[1] = DP(220)*sGlobal->scaleY;
    
    lightPos[2] = DP(265)*sGlobal->scaleX;
    lightPos[3] = DP(166)*sGlobal->scaleY;
    
    lightPos[4] = DP(300)*sGlobal->scaleX;
    lightPos[5] = DP(158)*sGlobal->scaleY;
    
    lightPos[6] = DP(160)*sGlobal->scaleX;
    lightPos[7] = DP(234)*sGlobal->scaleY;
    
    deco_light1 = getDecoLightSprite(0.8,0.8,lightPos[0],lightPos[1]);
    deco_light2 = getDecoLightSprite(0.6,0.6,lightPos[2],lightPos[3]);
    deco_light3 = getDecoLightSprite(0.4,0.4,lightPos[4],lightPos[5]);
    deco_light4 = getDecoLightSprite(0.8,0.8,lightPos[6],lightPos[7]);
    
    eagleContainer = wyNode::make();
    addChildLocked(eagleContainer, 11);
    eagleOrgY = wyDevice::winHeight-DP(randRange(40, 160))*sGlobal->scaleY;
    eagleContainer->setPosition(wyDevice::winWidth+DP(50)*sGlobal->scaleX, eagleOrgY);
    
    eagle = wySPXSprite::make("s02.sprite", false, MyTextureManager::makePNG("s02.png"), 3);
    eagle->setForceTickMode(true);
    eagle->setUnitInterval(0.16f);
    eagle->setScaleX(0.5f*sGlobal->scaleX);
    eagle->setScaleY(0.5f*sGlobal->scaleY);
    eagleContainer->addChildLocked(eagle, 11);
    eagle->setLoopCount(-1);
    setPressColor(eagle);
    
    eagle1 = wySPXSprite::make("s02.sprite", false, MyTextureManager::makePNG("s02.png"), 3);
    eagle1->setForceTickMode(true);
    eagle1->setUnitInterval(0.17f);
    eagle1->setScaleX(0.4f*sGlobal->scaleX);
    eagle1->setScaleY(0.4f*sGlobal->scaleY);
    eagleContainer->addChildLocked(eagle1, 11);
    eagle1->setLoopCount(-1);
    eagle1->setPosition(DP(30)*sGlobal->scaleX, -DP(4)*sGlobal->scaleY);
    setPressColor(eagle1);
    
    eagle2 = wySPXSprite::make("s02.sprite", false, MyTextureManager::makePNG("s02.png"), 3);
    eagle2->setForceTickMode(true);
    eagle2->setUnitInterval(0.18f);
    eagle2->setScaleX(0.3f*sGlobal->scaleX);
    eagle2->setScaleY(0.3f*sGlobal->scaleY);
    eagleContainer->addChildLocked(eagle2, 11);
    eagle2->setLoopCount(-1);
    eagle2->setPosition(DP(60)*sGlobal->scaleX, -DP(4)*sGlobal->scaleY);
    eagle2->setVisible(rand()%2);
    setPressColor(eagle2);
    
    eagleSpeed = randRange(3,6)*0.1f;

    emitter = wyParticleLoader::load("particle_snow.plist");
    emitter->setParticlePositionVariance(0, 0, 0, wyDevice::winHeight);
    emitter->setStartSizeVariance(10, 4);
    addChildLocked(emitter, 40);
    emitter->setPositionType(GROUPED);
    emitter->setPosition(wyDevice::winWidth+DP(40)*sGlobal->scaleX, 0);
    emitter->setScaleX(sGlobal->scaleX);
    emitter->setScaleY(sGlobal->scaleY);
    emitter->setEmissionRate(20);
}

void GameBackgroundLayer::updateSeason1(float dt) {
    if (eagle != NULL) {
        eagle->tick(dt);
    }
    if (eagle1 != NULL) {
        eagle1->tick(dt);
    }
    if (eagle2 != NULL) {
        eagle2->tick(dt);
    }
}

void GameBackgroundLayer::offsetSeason1(float dx, float dy) {
    tp1->offsetBy(dx*0.05f, 0);
    
    eagleContainer->translate(MIN(dx*eagleSpeed, -0.5f), 0);
//    LOGE("eagelx:%f", eagleContainer->getPositionX());
    
    float dxx =dx*0.2f;
    tp2->offsetBy(dxx, 0);
//    emitter->translate(dxx, 0);
    
    if(dy != 0) {
        tp1->setPosition(tp1x, tp1y + dy*0.2f);
        eagleContainer->setPosition(eagleContainer->getPositionX(), eagleOrgY+dy*0.22);
        
        
        float dyy = dy*0.25f;
        tp2->setPosition(tp2x, tp2y + dyy);
//        LOGE("eagleY:%f, a:%f, b:%f", eagleContainer->getPositionY(), dy*0.1, dyy);
        
        emitter->setPosition(emitter->getPositionX(), wyDevice::winHeight + dy*0.9f);
        
        deco_light1->setPosition(deco_light1->getPositionX(), lightPos[1] + dyy);
        deco_light2->setPosition(deco_light2->getPositionX(), lightPos[3] + dyy);
        deco_light3->setPosition(deco_light3->getPositionX(), lightPos[5] + dyy);
        deco_light4->setPosition(deco_light4->getPositionX(), lightPos[7] + dyy);
    }
    float dxxx = dxx*tp2->getScaleX();
    deco_light1->setPosition(deco_light1->getPositionX()+dxxx, deco_light1->getPositionY());
    deco_light2->setPosition(deco_light2->getPositionX()+dxxx, deco_light2->getPositionY());
    deco_light3->setPosition(deco_light3->getPositionX()+dxxx, deco_light3->getPositionY());
    deco_light4->setPosition(deco_light4->getPositionX()+dxxx, deco_light4->getPositionY());
    
    offsetLight(deco_light1);
    offsetLight(deco_light2);
    offsetLight(deco_light3);
    offsetLight(deco_light4);
    
    if (eagleContainer->getPositionX() < leftX) {
        eagleContainer->setPosition(eagleContainer->getPositionX()+randRange(2, 3)*wyDevice::winWidth, wyDevice::winHeight-DP(randRange(60, 150))*sGlobal->scaleY);
        eagleSpeed = randRange(3, 6)*0.1f;
        eagleOrgY = wyDevice::winHeight-DP(randRange(60, 150))*sGlobal->scaleY;
        eagle2->setVisible(rand()%2);
    }
}

void GameBackgroundLayer::offsetLight(wyNode *lightNode) {
    if (lightNode->getPositionX() < leftX) {
        lightNode->setPosition(lightNode->getPositionX()+maxX, lightNode->getPositionY());
        
        int fa = (int)randRange(200, 255);
        int ta = (int)randRange(0, 50);
        float dur = randRange(2,4);
        lightNode->stopAllActions();
        lightNode->runAction(wyRepeatForever::make(wySequence::make(wyFadeTo::make(dur, fa, ta), wyFadeTo::make(dur, ta, fa), NULL)));
        lightNode->setVisible(rand()%2);
    }

}

wySPXSprite * GameBackgroundLayer::getDecoLightSprite(float sx, float sy, float posx, float posy) {
    wySPXSprite *deco_light;
    deco_light = wySPXSprite::make("s02.sprite", false, MyTextureManager::makePNG("s02.png"), 4);
    //    deco_light->setForceTickMode(true);
    //    deco_light->setUnitInterval(0.1f);
    deco_light->setScaleX(sx*sGlobal->scaleX);
    deco_light->setScaleY(sy*sGlobal->scaleY);
    addChildLocked(deco_light, 100);
    //    deco_light->setLoopCount(0);
    deco_light->setPosition(posx, posy);
    //    deco_light->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    
    int fa = (int)randRange(180, 240);
    int ta = (int)randRange(0, 50);
    float dur = randRange(2,4);
    deco_light->runAction(wyRepeatForever::make(wySequence::make(wyFadeTo::make(dur, fa, ta), wyFadeTo::make(dur, ta, fa), NULL)));
    deco_light->setVisible(rand()%2);
    return deco_light;
}

void GameBackgroundLayer::initSeason2() {
    wyTexture2D *tex = MyTextureManager::makePNG("cave_0.png", WY_TEXTURE_PIXEL_FORMAT_RGBA8888);
    tp1 = wyTiledSprite::make(tex);
    tp1->setScaleX(sGlobal->scaleX*0.9f);
    tp1->setScaleY(sGlobal->scaleY*0.9f);
    tp1->setContentSize(wyDevice::winWidth / sGlobal->scaleX/0.9f, (wyDevice::winHeight + DP(266)*sGlobal->scaleY/0.9f) / sGlobal->scaleY /0.9f);
    tp1->setTileDirection(true, false);
    tp1->setAnchor(0, 0);
    tp1->setPosition(0, DP(-60)*sGlobal->scaleY);
    tp1->setBlendFunc( (wyBlendFunc){GL_ONE, GL_ZERO});
    addChildLocked(tp1, 10);
    tp1->setClipRect(wyr(0, 0, wyDevice::winWidth, wyDevice::winHeight));
    
    tex = MyTextureManager::makePNG("cave_3.png");
    tp2 = wyTiledSprite::make(tex);
    tp2->setScaleX(sGlobal->scaleX*0.8f);
    tp2->setScaleY(sGlobal->scaleY*0.8f);
    tp2->setContentSize(wyDevice::winWidth / sGlobal->scaleX/0.8f, (wyDevice::winHeight + DP(0)) / sGlobal->scaleY/0.8f);
    tp2->setTileDirection(true, false);
    tp2->setAnchor(0, 0);
    tp2->setPosition(0, DP(-30)*sGlobal->scaleY);
    addChildLocked(tp2, 40);
    tp2->setClipRect(wyr(0, 0, wyDevice::winWidth, wyDevice::winHeight));
    
    
    emitter = wyParticleLoader::load("particle_firefly.plist");
    emitter->setStartSizeVariance(DP(8)*sGlobal->scaleX, 0);
    emitter->setEndSizeVariance(DP(8)*sGlobal->scaleX, 0);
    emitter->setParticlePositionVariance(wyDevice::winWidth/2, wyDevice::winHeight/5, wyDevice::winWidth/2, wyDevice::winHeight/5);
    emitter->setPosition(0, 0);
    addChildLocked(emitter, 200);
    emitter->setPositionType(GROUPED);
    emitter->setScaleX(sGlobal->scaleX);
    emitter->setScaleY(sGlobal->scaleY);
    emitter->setEmissionRate(30);
    emitter->setLifeVariance(4, 2);
    
    emitter1 = wyParticleLoader::load("particle_firefly.plist");
    emitter1->setStartSizeVariance(DP(8)*sGlobal->scaleX, 0);
    emitter1->setEndSizeVariance(DP(8)*sGlobal->scaleX, 0);
    emitter1->setParticlePositionVariance(wyDevice::winWidth/2, wyDevice::winHeight/5, wyDevice::winWidth/2, wyDevice::winHeight/5);
    emitter1->setPosition(wyDevice::winWidth, 0);
    addChildLocked(emitter1, 200);
    emitter1->setPositionType(GROUPED);
    emitter1->setScaleX(sGlobal->scaleX);
    emitter1->setScaleY(sGlobal->scaleY);
    emitter1->setEmissionRate(30);
    emitter1->setLifeVariance(4, 2);
    
//    emitter = wyParticleLoader::load("particle_snow.plist");
//    emitter->setParticlePositionVariance(0, 0, 0, wyDevice::winHeight);
//    emitter->setStartSizeVariance(10, 4);
//    addChildLocked(emitter, 50);
//    emitter->setPositionType(GROUPED);
//    emitter->setPosition(wyDevice::winWidth+DP(40)*sGlobal->scaleX, 0);
//    emitter->setScaleX(sGlobal->scaleX);
//    emitter->setScaleY(sGlobal->scaleY);
//    emitter->setEmissionRate(20);
}

void GameBackgroundLayer::offsetSeason2(float dx, float dy) {
    tp1->offsetBy(dx*0.05f, 0);
    
    float dxx = dx*0.2f;
    tp2->offsetBy(dxx, 0);
    emitter->translate(dxx*tp2->getScaleX(), 0);
    emitter1->translate(dxx*tp2->getScaleX(), 0);
    
    if(dy != 0) {
        tp1->setPosition(0, DP(-60)*sGlobal->scaleY + dy*0.2f);
        
        float dyy = dy*0.25f;
        tp2->setPosition(0, DP(-30)*sGlobal->scaleY + dyy);
        emitter->setPosition(emitter->getPositionX(), dyy);
        emitter1->setPosition(emitter1->getPositionX(), dyy);
        
//        emitter->setPosition(emitter->getPositionX(), wyDevice::winHeight + dy*0.9f);
    }
    
    if (emitter->getPositionX() < -wyDevice::winWidth) {
        emitter->setPosition(wyDevice::winWidth, emitter->getPositionY());
    }
    if (emitter1->getPositionX() < -wyDevice::winWidth) {
        emitter1->setPosition(wyDevice::winWidth, emitter1->getPositionY());
    }
}

void GameBackgroundLayer::updateSeason2(float dt) {
    
}

void GameBackgroundLayer::initSeason3() {
    wyTexture2D *tex = MyTextureManager::makePNG("grass_0.png", WY_TEXTURE_PIXEL_FORMAT_RGBA8888);
    tp1 = wyTiledSprite::make(tex);
    tp1->setScaleX(sGlobal->scaleX*0.9f);
    tp1->setScaleY(sGlobal->scaleY*0.9f);
    tp1->setContentSize(wyDevice::winWidth / sGlobal->scaleX / 0.9f, (wyDevice::winHeight + DP(266)*sGlobal->scaleY/ 0.9f) / sGlobal->scaleY/ 0.9f);
    tp1->setTileDirection(true, false);
    tp1->setAnchor(0, 0);
    tp1->setPosition(0, DP(-40)*sGlobal->scaleY);
    addChildLocked(tp1, 10);
    tp1->setBlendFunc( (wyBlendFunc){GL_ONE, GL_ZERO});
    tp1->setClipRect(wyr(0, 0, wyDevice::winWidth, wyDevice::winHeight));
    
    tex = MyTextureManager::makePNG("grass_4.png");
    tp2 = wyTiledSprite::make(tex);
    tp2->setScaleX(sGlobal->scaleX*0.8f);
    tp2->setScaleY(sGlobal->scaleY*0.8f);
    
    maxX = tex->getWidth()*tp2->getScaleX();
    
    tp2->setContentSize(wyDevice::winWidth / sGlobal->scaleX/ 0.8f, (wyDevice::winHeight + DP(0)) / sGlobal->scaleY/ 0.8f);
    tp2->setTileDirection(true, false);
    tp2->setAnchor(0, 0);
    tp2->setPosition(0, DP(-30)*sGlobal->scaleY);
    addChildLocked(tp2, 40);
    tp2->setClipRect(wyr(0, 0, wyDevice::winWidth, wyDevice::winHeight));
    
    
    waterContainer = wyNode::make();
    waterContainer->setPosition(0, 0);
    addChildLocked(waterContainer, 41);
    
    
   

    cloudContainer = wyNode::make();
    cloudContainer->setPosition(wyDevice::winWidth*1.2f, 0);
    addChildLocked(cloudContainer, 11);
    
    c1 = getCloudSprite(1.8f, 0, 500);
    c2 = getCloudSprite(1.8f, 150, 550);
    c3 = getCloudSprite(1.8f, 300, 520);
    
    onCloudMoveEnd(NULL);
}

wySPXSprite * GameBackgroundLayer::getCloudSprite(float sx, float posx, float posy) {
    wySPXSprite *cloud = wySPXSprite::make("s02.sprite", false, MyTextureManager::makePNG("s02.png"), rand()%2+5);
    cloud->setForceTickMode(true);
    cloud->setUnitInterval(0.16f);
    cloud->setScaleX(sx*sGlobal->scaleX);
    cloud->setScaleY(sx*sGlobal->scaleY);
    cloud->setPosition(DP(posx)*sGlobal->scaleX, DP(posy)*sGlobal->scaleY);
    cloudContainer->addChildLocked(cloud, 11);
    cloud->setLoopCount(-1);
    
    return cloud;
}

void GameBackgroundLayer::onCloudMoveEnd(wyTargetSelector* ts) {
    cloudContainer->setPosition(wyDevice::winWidth*1.2f, 0);
    
    c1->setPosition(0, wyDevice::winHeight-DP(randRange(80, 200))*sGlobal->scaleY);
    c2->setPosition(DP(randRange(50, 100))*sGlobal->scaleX, wyDevice::winHeight-DP(randRange(80, 200))*sGlobal->scaleY);
    c3->setPosition(DP(randRange(150, 200))*sGlobal->scaleX, wyDevice::winHeight-DP(randRange(80, 200))*sGlobal->scaleY);
    
    c1->setRotation(randRange(0, 45));
    c2->setRotation(randRange(0, 45));
    c3->setRotation(randRange(0, 45));
}

void GameBackgroundLayer::offsetSeason3(float dx, float dy) {
    tp1->offsetBy(dx*0.05f, 0);
    
    float dxx = dx*0.2f;
    tp2->offsetBy(dxx, 0);
    waterContainer->translate(dxx*tp2->getScaleX(), 0);
    
    if(dy != 0) {
        tp1->setPosition(0, DP(-40)*sGlobal->scaleY + dy*0.2f);
        
        float dyy = dy*0.25f;
        tp2->setPosition(0, DP(-30)*sGlobal->scaleY + dyy);
        waterContainer->setPosition(waterContainer->getPositionX(), dyy);
    }
    
    cloudContainer->translate(dx*0.4f, 0);
    cloudContainer->setPosition(cloudContainer->getPositionX(), dy*0.22f);
    //        LOGE("cloud:%f", cloudContainer->getPositionX());
    if (cloudContainer->getPositionX() < -wyDevice::winWidth) {
        onCloudMoveEnd(NULL);
    }
    
//    LOGE("water:%f", waterContainer->getPositionX());
    if (waterContainer->getPositionX() < -DP(300)*sGlobal->scaleX) {
        waterContainer->setPosition(waterContainer->getPositionX()+maxX, waterContainer->getPositionY());
    }
}


void GameBackgroundLayer::update(float dt) {
    switch (bgIndex) {
        case 0:
        case 1:
            updateSeason1(dt);
            break;
        case 2:
            updateSeason2(dt);
            break;
        //case 3:
            //updateSeason3(dt);
           // break;
        default:
            break;
    }
}

void GameBackgroundLayer::offsetBy(float dx, float dy) {
    switch (bgIndex) {
        case 4:
            break;
        case 2:
            offsetSeason2(dx, dy);
            break;
        case 3:
            offsetSeason3(dx, dy);
            break;
        case 0:
        case 1: {
            offsetSeason1(dx, dy);
            break;
        }
        default:
            break;
    }
}




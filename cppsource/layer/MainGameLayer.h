//
//  MainGameLayer.h
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "wyLayer.h"
#include "WiEngine-Box2D.h"
#include "MyContactListener.h"
#include "Hero.h"

class MainGameLayer : public wyLayer, public b2ContactListener {
public:
    MainGameLayer(int mapIndex, bool isFromCheckPoint = false);
    virtual ~MainGameLayer();

    void loadMap(int mapIndex);
    void loadEndlessMap(int mapIndex);
    wyTMXTileMap* m_TileMap, *m_TileMap1;
    wyTMXTileMap** m_TileMapArray;
    wyTMXLayer** m_FrontArray;
    
    wyBox2D* m_box2d;
    bool isFromCheckPoint;
    
    void updateWorld(wyTargetSelector* ts);
    
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    
    CustomContactListener *_contactListener;
    CustomContactFilter *_contactFilter;
    
    
    bool needAutoScroll, isEndless;
    
    Hero *hero;
    
    std::vector<b2Body *>toDestroy;
    void addToDestroy(b2Body *body);
    std::vector<b2Body *>toDisable;
    void addToDisable(b2Body *body);
    std::vector<b2Body *>toEnable;
    void addToEnable(b2Body *body);
    std::vector<BaseObject *>toRecreateFixture;
    void addToRecreateFixture(BaseObject *obj);
    std::vector<b2Fixture *>toDestroyFixture;
    void addToDestroyFixture(b2Fixture *fixture);
    
    std::vector<b2Body *>bodyPiecePool;
    
    void handleEnemyBulletCollision(b2Body *enemy, b2Body *bullet);
    void handleEnemyBigFireCollision(b2Body *enemy, b2Body *bullet);
    void handleBoxBulletCollision(b2Body *box, b2Body *bullet);
    void handleEnemyDaBiaoCollision(b2Body *enemyBody, b2Body *bulletBody);
    void handleEnemyLiudanCollision(b2Body *enemyBody, b2Body *bulletBody);
    
    std::vector<b2Body *>groundBody, groundBody1;
    
    void shake();
    
    void dealPhysics(float dt);
    
    float timeAccumulator;
    void renderFullStep(float dt);
    void renderPartStep(const float alpha);
    void consumeContacts_();
    
    bool isDestroied;
    bool isFirstTime;
    
    void reset();
    
    wySpriteBatchNode* m_batchNode;
    
    bool isRealEndless;
    void loadRealEndlessMap(int index);
    void generateNextMap(wyTMXTileMap* nextMapPtr, int idx);
    wyTMXLayer *front0, *front1;
    bool needSwitchSeason, hasSwitchedSeason;
    int nextSeasonId;
//    int curMapIndex;
    int bossFightIndex;
    void summonBoss();
    
    wySpriteBatchNode *bulletBatchNode, *bloodBatchNode, *fleshBatchNode, *bloodSpriteBatchNode, *shadowBatchNode;
    
//    bool isDrawDebug;
//    float x1, y1, x2, y2, xx, yy;
//    virtual void draw();
    wyBox2DPELoader* m_bodyLoader, *m_bodyLoader1;
    int currentBodies;
    
    void appendEndlesssMap();
    
    bool drawDebug;
    wyBezierConfig m_config, m_config1;
    virtual void draw();
    
    int *enemyCount;
    
    int getMapPieceNumber(int mapIndex);
    
    void onEnter();
    
    int mapEnemyCount, altEnemyCount;
    int *mapEnemyCountArray;
    int *mapEnemyPlaceArray;
    float *mapOffsetXArray;
    int *enemyTypeArray;
    int *enemySDFArray;
};

static void onShockWaveDisappearEnd(wyAction* action, void* data) {
    wyNode *sprite = (wyNode *) data;
    sprite->getParent()->removeChildLocked(sprite, true);
}


//
//  MainGameLayer.cpp
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "MainGameLayer.h"
#include "Global.h"
#include "BaseObject.h"
#include "MainScene.h"
#include "EnemyFactory.h"
#include "Enemy.h"
#include "ObjectsFactory.h"
#include "Box.h"
#include "Bonus.h"
#include "Intro.h"
#include <cmath>
#include "Boss1.h"
#include "Boss2.h"
#include "Boss3.h"
#include "HeroBullet.h"
#include "Deco.h"
#include "FlyZombie1.h"
#include "Trap.h"
#include <stdexcept>

MainGameLayer::MainGameLayer(int mapIndex, bool isFromCheckPoint) {
    sGlobal->isBoss1Strong = false;
    
    sGlobal->resetSeqNo();
    
    sGlobal->mainGameLayer = this;
    needSwitchSeason = false;
    hasSwitchedSeason = false;

    sGlobal->tileMapOffsetY = 0;
    
    this->isFromCheckPoint = isFromCheckPoint;
    
    bulletBatchNode = wySpriteBatchNode::make(MyTextureManager::makePNG("ui_en.png"));
    addChildLocked(bulletBatchNode, 100);
    
    bloodBatchNode = wySpriteBatchNode::make(MyTextureManager::makePNG("dialog_deco.png"));
    addChildLocked(bloodBatchNode);
    
    fleshBatchNode = wySpriteBatchNode::make(MyTextureManager::makePNG("zombiebodies_pic.png"));
    addChildLocked(fleshBatchNode, 1);
    
//    bloodSpriteBatchNode = wySpriteBatchNode::make(MyTextureManager::makePNG("blood_pen"));
//    addChildLocked(bloodSpriteBatchNode);
    
//    shadowBatchNode = wySpriteBatchNode::make(MyTextureManager::makePNG("dialog_deco.png"));
//    addChildLocked(shadowBatchNode, 999);
    
    m_bodyLoader = new wyBox2DPELoader("zombiebodies_0.plist", false);
    m_bodyLoader1 = new wyBox2DPELoader("zombiebodies_1.plist", false);
    
    isRealEndless = false;
    sGlobal->isRealEndless = false;
    enemyCount = NULL;
    if (mapIndex != 0 && mapIndex % 100 == 0) {
        isRealEndless = true;
        sGlobal->isRealEndless = true;
        isEndless = false;
        loadRealEndlessMap(100);
    } else {
        isEndless = (mapIndex % 100 == 10);
//        loadMap(mapIndex);
        if (!isEndless) {
            loadMap(mapIndex);
        } else {
            loadEndlessMap(mapIndex);
        }
    }
    
//    this->getCamera()->setCenter(0, 0, 0);
//    this->getCamera()->setEye(0, 0, 1);
//    float eye[3] = {0.0f};
//    this->getCamera()->getEye(eye);
//    LOGE("eye: x:%f, y:%f, z%f", eye[0], eye[1], eye[2]);
    //    
    //    float *center = new float[3];
    //    this->getCamera()->getCenter(center);
    //    LOGE("center: x:%f, y:%f, z%f", center[0], center[1], center[2]);
    //    
    //    LOGE("center: x:%f, y:%f, z%f", center[0], center[1], center[2]);
    b2World* world = m_box2d->getWorld();
    _contactListener = new CustomContactListener();
    world->SetContactListener(_contactListener);
    
//    _contactFilter = new CustomContactFilter();
//    world->SetContactFilter(_contactFilter);
    
    timeAccumulator = 0;
    
    toDestroy.clear();
    toDisable.clear();
    toEnable.clear();
    toRecreateFixture.clear();
//    LOGE("toDestroy length: %d", (int)toDestroy.size());
    bodyPiecePool.clear();
    
    isDestroied = false;
    isFirstTime = true;
//    needDestroyMoto = false;
//    needDestroyShield = false;
    
    currentBodies = 0;
    drawDebug = false;
}

MainGameLayer::~MainGameLayer() {
//    LOGE("mainGameLayer dealloc...");
    isDestroied = true;
    
    delete _contactListener;
	_contactListener = NULL;
    
//    delete _contactFilter;
//	_contactFilter = NULL;
    
    b2World* world = m_box2d->getWorld();
    for(b2Body* body = world->GetBodyList(); body != NULL; body = body->GetNext()) {
        BaseObject *obj = (BaseObject*)body->GetUserData();
        if (obj != NULL) {
//            WYDELETE(obj);
            obj->release();
        }
    }
    
    toDestroy.clear();
    toDisable.clear();
    toEnable.clear();
    toRecreateFixture.clear();
    bodyPiecePool.clear();
    groundBody.clear();
    groundBody1.clear();

    if (!isEndless && !isRealEndless) {
        delete[] m_TileMapArray;
        delete[] m_FrontArray;
    }
    
    wyObjectRelease(m_bodyLoader);
    wyObjectRelease(m_bodyLoader1);
    
    m_box2d->release();
    m_box2d = NULL;
}

void MainGameLayer::onEnter() {
    wyLayer::onEnter();
    
    wyTargetSelector* ts = wyTargetSelector::make(this, SEL(MainGameLayer::updateWorld));
    wyTimer* t = wyTimer::make(ts);
    scheduleLocked(t);
}

int MainGameLayer::getMapPieceNumber(int mapIndex) {
    int count = 0;
    int seasonId = mapIndex / 100;
    int mapId = mapIndex % 100;
    char mapName[50];
    
    while (true) {
        count += 1;
        sprintf(mapName, "map/s%02de%02d_%d.tmx", seasonId, mapId, count);
//        LOGE("aaa:%s", mapName);
        if (!wyUtils::isResExistent(mapName, false)) {
            break;
        }
//        LOGE("bbb1");
    }
    count--;
    if (count == 0) {
        count = 1;
    }
//    LOGE("map piece of %d is:%d", mapIndex, count);
    return count;
}

void MainGameLayer::loadMap(int mapIndex) {
//    enemyCount = new int[100];
//    memset(enemyCount, 0, sizeof(int)*100);
    
    sGlobal->tileMapWidthInPixels = 0;
    sGlobal->mapOffsetX = 0;
    int seasonId = mapIndex / 100;
    int mapId = mapIndex % 100;
    char *mapName = new char[50];
    char *textureName = new char[50];
    sprintf(textureName, "s%02d.png", seasonId);
    
    needAutoScroll = true;
//    mapIndex = 101;
//	switch (mapIndex) {
//		case 101:
//			sGlobal->mapPieceNumber = 6;
//			break;
//		case 102:
//			sGlobal->mapPieceNumber = 8;
//			break;
//		case 103:
//			sGlobal->mapPieceNumber = 12;
//			break;
//		case 104:
//			sGlobal->mapPieceNumber = 8;
////            sGlobal->mapPieceNumber = 1;
//			break;
//        case 207:
////            sGlobal->mapPieceNumber = 13;
////            break;
//		case 105:
//		case 106:
//		case 301:
//		case 303:
//		case 304:
//			sGlobal->mapPieceNumber = 13;
//			break;
//		case 107:
//		case 201:
//		case 202:
//		case 203:
//		case 204:
//            //case 205:
//		case 302:
//			sGlobal->mapPieceNumber = 14;
//			break;
//		case 206:
//		case 305:
//		case 306:
//			sGlobal->mapPieceNumber = 15;
//			break;
//		case 307:
//			sGlobal->mapPieceNumber = 17;
//			break;
//		case 205:
//			sGlobal->mapPieceNumber = 14;
//			break;
//		default:
//			sGlobal->mapPieceNumber = 1;
//			break;
//	}
    
    sGlobal->mapPieceNumber = getMapPieceNumber(mapIndex);
    
    LOGE("map piece: %d", sGlobal->mapPieceNumber);
    m_TileMapArray = new wyTMXTileMap*[50];
    memset(m_TileMapArray, 0, sizeof(wyTMXTileMap*)*50);
    m_FrontArray = new wyTMXLayer*[50];
    memset(m_FrontArray, 0, sizeof(wyTMXLayer*)*50);
    wyTMXTileMap *tempMap;
    float y0;
    int startNo;
    if (isFromCheckPoint && sGlobal->mapPieceNumber > 1) {
        startNo = MAX(sGlobal->checkPointIdx - 1, 1);
    } else {
        startNo = 1;
    }
//    LOGE("startNo: %d", startNo);
    mapEnemyCount = 0;
    altEnemyCount = 0;
    mapEnemyCountArray = new int[50];
    memset(mapEnemyCountArray, 0, sizeof(int)*50);
    mapOffsetXArray = new float[50];
    memset(mapOffsetXArray, 0, sizeof(float)*50);
    enemyTypeArray = new int[50];
    memset(enemyTypeArray, 0, sizeof(int)*50);
    int maxEId = -1;
    
    for (int i = startNo; i <= sGlobal->mapPieceNumber; i++) {
        sGlobal->curMapPieceIndex = i;
        
        if (sGlobal->mapPieceNumber == 1) {
            sprintf(mapName, "map/s%02de%02d.tmx", seasonId, mapId);
        } else {
            sprintf(mapName, "map/s%02de%02d_%d.tmx", seasonId, mapId, i);
        }
        if (mapIndex == 0) {
            tempMap = wyTMXTileMap::make("training.tmx", false, MyTextureManager::makePNG("s01.png"), NULL);
        }
        else {
//            LOGE("mapName:%s", mapName);
            tempMap = wyTMXTileMap::make(mapName, false, MyTextureManager::makePNG(textureName), NULL);
        }
        tempMap->setScaleX(sGlobal->scaleX);
        tempMap->setScaleY(sGlobal->scaleY);
        tempMap->setAnchor(0, 0);
        sGlobal->tileWidth = tempMap->getTileWidth()*sGlobal->scaleX;
        
        if (i == startNo) {
            sGlobal->tileMapHeightInPixels = tempMap->getMapHeight() * tempMap->getTileHeight() * sGlobal->scaleY;
            sGlobal->tileMapOffsetY = -tempMap->getTileHeight() * sGlobal->scaleY * atoi(tempMap->getProperty("offsetY"))+sGlobal->tileMapOffsetYExtra;
            //            LOGE("mapHeight:%d, tileHeight:%f, scaleY:%f", tempMap->getMapHeight(), tempMap->getTileHeight(), sGlobal->scaleY);
        }
        if (isFromCheckPoint && sGlobal->mapPieceNumber > 1 && i == startNo) {
            sGlobal->tileMapWidthInPixels = tempMap->getMapWidth() * tempMap->getTileWidth() * sGlobal->scaleX * (startNo - 1);
        }
        tempMap->setPosition(sGlobal->tileMapWidthInPixels, sGlobal->tileMapOffsetY);
        //        LOGE("mapNo: %d, pos: %f, %f", i, tempMap->getPositionX(), tempMap->getPositionY());
        //        tempMap->setClipRect(wyr(0, 0, wyDevice::winWidth, wyDevice::winHeight));
        sGlobal->mapOffsetX = sGlobal->tileMapWidthInPixels;
        sGlobal->tileMapWidthInPixels += tempMap->getMapWidth() * tempMap->getTileWidth() * sGlobal->scaleX;
        addChildLocked(tempMap, -1);
        m_TileMapArray[i] = tempMap;
//        tempMap->setVisible(false);
        
//        tempMap->setAlpha(0);
        
        
        wyTMXLayer *front0 = tempMap->getLayer("tile_terrain_deco_front");
		//wyTMXLayer *front1 = tempMap->getLayer("tile_terrain_3");
        //		wyTMXLayer *front2 = tempMap->getLayer("tile_terrain_0");
		//wyTMXLayer *front3 = tempMap->getLayer("tile_terrain_deco_back");
        //		wyTMXLayer *front4 = tempMap->getLayer("tile_terrain_2");
		//tempMap->removeChild(front1, true);
        //		tempMap->removeChild(front2, true);
		//tempMap->removeChild(front3, true);
        //		tempMap->removeChild(front4, true);
        if (front0 != NULL) {
            front0->setScaleX(sGlobal->scaleX);
            front0->setScaleY(sGlobal->scaleY);
            front0->setAnchor(0, 0);
            front0->setPosition(tempMap->getPositionX(), tempMap->getPositionY());
            tempMap->removeChildLocked(front0, false);
            this->addChildLocked(front0, 1000);
            m_FrontArray[i] = front0;
//            front0->setVisible(false);
//            LOGE("show front...");
        }
        if (i != startNo) {
            tempMap->setVisible(false);
            if (front0 != NULL) {
                front0->setVisible(false);
                if (startNo > 1 && i == startNo+1) {
                    front0->setVisible(true);
                }
            }
        } else {
            sGlobal->singleMapWidth = tempMap->getMapWidth() * tempMap->getTileWidth() * sGlobal->scaleX;
            m_TileMap = tempMap;
            
            m_box2d = new wyBox2D();
            m_box2d->setDebugDraw(sGlobal->isShowBox2DDebug);
            addChildLocked(m_box2d);
            
            b2World* world = m_box2d->getWorld();
            world->SetGravity(b2Vec2(0, -112));
            world->SetContinuousPhysics(true);
            m_box2d->setPosition(0, 0);
            world->SetAutoClearForces(false);
            
            //            if (wyDevice::winWidth == 1024) {
            //                m_box2d->setMeterPixels(30.72);
            //            }
            LOGE("meter2Pixel: %f", m_box2d->meter2Pixel(1));
        }
        
        b2World* world = m_box2d->getWorld();
        //生成地面
        wyTMXObjectGroup* og = tempMap->getObjectGroup("obj_terrain");
        for (int j = 0; j < og->getObjectCount(); j++) {
            wyTMXObject* obj = og->getObjectAt(j);
            float x = atof(og->getObjectProperty(obj, "x")) * sGlobal->scaleX + sGlobal->mapOffsetX;
            float y = sGlobal->tileMapHeightInPixels - atof(og->getObjectProperty(obj, "y")) * sGlobal->scaleY;
            //            LOGE("ground y:%f", atof(og->getObjectProperty(obj, "y")));
            float width = atof(og->getObjectProperty(obj, "width")) * sGlobal->scaleX;
//            float height = atof(og->getObjectProperty(obj, "height"));
            const char *type = og->getObjectProperty(obj, "type");
            b2Body* ground1 = NULL;
            BaseObject *obj2 = WYNEW BaseObject();
            bool isStart = false;
            
            if (type != NULL && strcmp(type, "bound") == 0) {
                obj2->obj_type = TYPE_WORLD_BOUND;
                {
                    b2BodyDef bd;
                    bd.position.Set(m_box2d->pixel2Meter(x), 0);
                    ground1 = world->CreateBody(&bd);
                    
                    b2EdgeShape top;
                    top.Set(b2Vec2(0, 0), b2Vec2(0, m_box2d->pixel2Meter(wyDevice::winHeight)));
                    b2FixtureDef fixtureDef;
                    //                    fixtureDef.density = 0.0f;
                    //                    fixtureDef.friction = 0.0f;
                    //                    fixtureDef.restitution = 0.0f;
                    fixtureDef.shape = &top;
					fixtureDef.filter.categoryBits = CAT_BITS[TYPE_WORLD_BOUND];
                    fixtureDef.filter.maskBits = MASK_BITS[TYPE_WORLD_BOUND];
                    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_WORLD_BOUND];
                    
                    ground1->CreateFixture(&fixtureDef);
                    ground1->SetUserData(obj2);
                    obj2->body = ground1;
                    obj2->widthInPixels = 0;
                }
                sGlobal->tileMapWidthInPixels = x;
            } else {
                obj2->obj_type = TYPE_GROUND;
                if (i == startNo && x == 0) {
                    x -= DP(1000)*sGlobal->scaleX;
                    width += DP(1000)*sGlobal->scaleX;
                    isStart = true;
                }
                {
                    b2BodyDef bd;
                    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + sGlobal->tileMapOffsetY));
                    ground1 = world->CreateBody(&bd);
                    
                    b2EdgeShape top;
                    top.Set(b2Vec2(0, 0), b2Vec2(m_box2d->pixel2Meter(width), 0));
                    b2FixtureDef fixtureDef;
                    fixtureDef.density = 0.0f;
                    fixtureDef.friction = 1.0f;
                    fixtureDef.restitution = 0.0f;
                    fixtureDef.shape = &top;
                    fixtureDef.isSensor = false;
					fixtureDef.filter.categoryBits = CAT_BITS[TYPE_GROUND];
                    fixtureDef.filter.maskBits = MASK_BITS[TYPE_GROUND];
                    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_GROUND];
                    
                    ground1->CreateFixture(&fixtureDef);
                    ground1->SetUserData(obj2);
                    obj2->body = ground1;
                    obj2->widthInPixels = width;
                    
                    obj2->startX = x;
                    obj2->endX = x + width;
                }
            }
            
            if (isStart && obj2->obj_type == TYPE_GROUND) {
                y0 = y;
            }
        }
        
        if (!(isFromCheckPoint && i == startNo - 1)) {
            sObjectsFactory->createObjects(tempMap->getObjectGroup("obj_objects"));
            
            wyTMXObjectGroup *e = tempMap->getObjectGroup("obj_enemies");
            wyTMXObjectGroup *e1 = tempMap->getObjectGroup("obj_enemies_alt");
            int c, c1;
            if (e != NULL) {
                c = e->getObjectCount();
            } else {
                c = 0;
            }
            if (e1 != NULL) {
                c1 = e1->getObjectCount();
            } else {
                c1 = 0;
            }
            if (i == 0) {
                mapEnemyCountArray[i] = c + c1;
            } else {
                mapEnemyCountArray[i] = mapEnemyCountArray[i-1] + c + c1;
            }
            
            mapEnemyCount = mapEnemyCountArray[i];
            altEnemyCount += c1;
            
            if (e != NULL) {
                for (int i = 0; i < e->getObjectCount(); i++) {
                    wyTMXObject* obj = e->getObjectAt(i);
                    if (obj == NULL)
                        continue;
                    const char *type = e->getObjectProperty(obj, "type");
                    int e_id = sEnemyFactory->getEnemyIdByType(type);
                    enemyTypeArray[e_id]++;
                    
                    if (maxEId < e_id) {
                        maxEId = e_id;
                    }
                }
            }
            //easy难度中，alt里的enemy种类不统计。
            if (e1 != NULL && sGlobal->currentSModeDifficulty != DIFFICULTY_NORMAL) {
                for (int i = 0; i < e1->getObjectCount(); i++) {
                    wyTMXObject* obj = e1->getObjectAt(i);
                    if (obj == NULL)
                        continue;
                    const char *type = e1->getObjectProperty(obj, "type");
                    int e_id = sEnemyFactory->getEnemyIdByType(type);
                    enemyTypeArray[e_id]++;
                    
                    if (maxEId < e_id) {
                        maxEId = e_id;
                    }
                }
            }
        }
        
        mapOffsetXArray[i] = sGlobal->mapOffsetX;
    }
    
    maxEId++;
    enemySDFArray = new int[maxEId];
    memset(enemySDFArray, 0, sizeof(int)*(maxEId));
    enemySDFArray[0] = enemyTypeArray[0];
    for (int i = 1; i < maxEId; i++) {
        enemySDFArray[i] += enemySDFArray[i-1]+enemyTypeArray[i];
    }
    enemySDFArray[maxEId - 1] = mapEnemyCount;
    for (int i = 0; i < maxEId; i++) {
        LOGE("enemySDFArray:%d, %d", i, enemySDFArray[i]);
    }
    
    LOGE("mapEnemyCount:%d", mapEnemyCount);
    int diffCount;
    mapEnemyPlaceArray = new int[mapEnemyCount];
    memset(mapEnemyPlaceArray, 0, sizeof(int)*mapEnemyCount);
    
    if (!sGlobal->isRealEndless) {
        int seasonId = sGlobal->index / 100;
        int mapId = sGlobal->index - seasonId*100;
        int startIdx = 0;
        switch (sGlobal->currentSModeDifficulty) {
            case DIFFICULTY_NORMAL:
                startIdx = 0;
                break;
            case DIFFICULTY_HEROIC:
                startIdx = 30;
                break;
            case DIFFICULTY_HARD:
                startIdx = 60;
                break;
            default:
                break;
        }
        int targetEnemyCount = sGlobal->ENEMY_COUNT[startIdx+(seasonId-1)*10+mapId-1];
        if (sGlobal->currentSModeDifficulty == DIFFICULTY_NORMAL) {
            LOGE("altEnemyCount:%d", altEnemyCount);
            targetEnemyCount += altEnemyCount;
        }
        if (sGlobal->index == 0) {
            targetEnemyCount = 0;
        }
        LOGE("targetEnemyCount:%d", targetEnemyCount);
        
        diffCount = MAX(0, (targetEnemyCount - mapEnemyCount));
        LOGE("diffCount:%d", diffCount);
        
        int initialCount = 0;
        if (diffCount > 0) {
            int div = targetEnemyCount / mapEnemyCount;
            int yushu = diffCount % mapEnemyCount;
            if (div - 1 > 0) {
                for (int i = 0; i < mapEnemyCount; i++) {
                    mapEnemyPlaceArray[i] = div;
                }
                initialCount = div;
            }
            for (int i = 0; i < yushu; i++) {
                int seed = rand()%mapEnemyCount;
                while (mapEnemyPlaceArray[seed] > initialCount) {
                    seed = rand()%mapEnemyCount;
                }
                mapEnemyPlaceArray[seed] += 1;
            }
        }
    }
    
    for (int i = 0; i < mapEnemyCount; i++) {
        LOGE("mapEnemyPlaceArray:%d, %d", i, mapEnemyPlaceArray[i]);
    }
    
    for (int i = startNo; i <= sGlobal->mapPieceNumber; i++) {
        wyTMXTileMap *map = m_TileMapArray[i];
        wyTMXObjectGroup *e = map->getObjectGroup("obj_enemies");
        wyTMXObjectGroup *e1 = map->getObjectGroup("obj_enemies_alt");
        int c, c1;
        if (e != NULL) {
            c = e->getObjectCount();
        } else {
            c = 0;
        }
        if (e1 != NULL) {
            c1 = e1->getObjectCount();
        } else {
            c1 = 0;
        }
        
        sEnemyFactory->createEnemiesWithExtra(map->getObjectGroup("obj_enemies"), mapEnemyCountArray[i]-c-c1, mapOffsetXArray[i], false);
        sEnemyFactory->createEnemiesWithExtra(map->getObjectGroup("obj_enemies_alt"), mapEnemyCountArray[i]-c1, mapOffsetXArray[i], true);
    }
    LOGE("1");
    delete [] mapEnemyPlaceArray;
    LOGE("2");
    delete [] mapEnemyCountArray;
    LOGE("3");
    delete [] mapOffsetXArray;
    LOGE("4");
    delete [] enemyTypeArray;
    LOGE("5");
    delete [] enemySDFArray;

    if (mapIndex == 104) {
        appendEndlesssMap();
    }
    
    hero = WYNEW Hero(NULL, needAutoScroll);
    
    if (isFromCheckPoint) {
        hero->body->SetTransform(b2Vec2(m_box2d->pixel2Meter(sGlobal->hero_offset_left + sGlobal->checkPointX - DP(160)*sGlobal->scaleX), m_box2d->pixel2Meter(sGlobal->checkPointY + DP(30)*sGlobal->scaleY) - hero->ofyInMeter + hero->halfBodyHeightInMeter), 0);// + hero->ofyInMeter
        hero->container->setPosition(m_box2d->meter2Pixel(hero->body->GetPosition().x) + hero->offsetX, m_box2d->meter2Pixel(hero->body->GetPosition().y) + hero->offsetY);
        hero->update(0);
    } else {
        hero->body->SetTransform(b2Vec2(m_box2d->pixel2Meter(sGlobal->hero_offset_left - DP(400)*sGlobal->scaleX), m_box2d->pixel2Meter(y0 + sGlobal->tileMapOffsetY + DP(30)*sGlobal->scaleY) - hero->ofyInMeter + hero->halfBodyHeightInMeter), 0);// + hero->ofyInMeter
//        LOGE("%f, %f, %f", hero->bodyRect.height/2 + y0 + sGlobal->tileMapOffsetY + DP(0), y0, hero->ofyInMeter);
        hero->container->setPosition(m_box2d->meter2Pixel(hero->body->GetPosition().x) + hero->offsetX, m_box2d->meter2Pixel(hero->body->GetPosition().y) + hero->offsetY);
    }
    
    delete [] mapName;
    delete [] textureName;
    
//    for (int i = 0; i < 1000; i++) {
//        BaseObject *obj2 = WYNEW BaseObject();
//        
//        obj2->obj_type = TYPE_GROUND;
//        b2BodyDef bd;
//        bd.position.Set(DP(200)*sGlobal->scaleX, DP(250)*sGlobal->scaleY);
//        bd.userData = obj2;
//        bd.type = b2_dynamicBody;
//        
//        b2PolygonShape staticBox;
//        staticBox.SetAsBox(m_box2d->pixel2Meter(20), m_box2d->pixel2Meter(20));
//        b2FixtureDef fixtureDef;
//        fixtureDef.shape = &staticBox;
//        fixtureDef.density = 0.0f;
//        fixtureDef.friction = 0.0f;
//        fixtureDef.restitution = 0.0f;
//        fixtureDef.isSensor = false;
//        fixtureDef.filter.maskBits = MASK_BITS[TYPE_BODY_PIECE];
//        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_BODY_PIECE];
//        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_BODY_PIECE];
//        
//        b2Body *bp = m_box2d->getWorld()->CreateBody(&bd);
//        bp->CreateFixture(&fixtureDef);
//        bp->SetTransform(bd.position, 0);
//        bp->SetActive(false);
//        bp->SetAwake(false);
//        
//        obj2->body = bp;
//    }
    
    
//    char temp[50];
//    LOGE("=====================enemy count start...=========================");
//    LOGE("map:%d, difficulty:%d", sGlobal->index, sGlobal->currentSModeDifficulty);
//    for (int i = 0; i < 100; i++) {
//        if (enemyCount[i] == 0) {
//            continue;
//        }
//        switch (i) {
//            case ENEMY_TUZIJIANKE:
//                sprintf(temp, "MaleZombie:%d", enemyCount[i]);
//                break;
//            case ENEMY_TUZIGONGJIANSHOU:
//                sprintf(temp, "SpittleZombie:%d", enemyCount[i]);
//                break;
//            case ENEMY_TUZILANGKE:
//                sprintf(temp, "MaleZombie(TUZILANGKE):%d", enemyCount[i]);
//                break;
//            case ENEMY_TUZIFASHI:
//                sprintf(temp, "Devourer:%d", enemyCount[i]);
//                break;
//            case ENEMY_TUZIJIANSHENG:
//                sprintf(temp, "FatZombie(jiansheng):%d", enemyCount[i]);
//                break;
//            case ENEMY_TUZIGONGJIANSHOUFLY:
//                sprintf(temp, "FlyZombie1(tuzigonjianshoufly):%d", enemyCount[i]);
//                break;
//                
//            case ENEMY_KULOUSHIBING:
//                sprintf(temp, "GhostBaby:%d", enemyCount[i]);
//                break;
//            case ENEMY_KULOUWUSHI:
//                sprintf(temp, "FatZombie(KULOUWUSHI):%d", enemyCount[i]);
//                break;
//            case ENEMY_KULOUTOULING:
//                sprintf(temp, "MuscleZombie1(KULOUTOULING):%d", enemyCount[i]);
//                break;
//            case ENEMY_KULOUTOUJIZHE:
//                sprintf(temp, "FemaleZombie(KULOUTOUJIZHE):%d", enemyCount[i]);
//                break;
//            case ENEMY_KULOUSHIBINGFLY:
//                sprintf(temp, "FlyZombie2:%d", enemyCount[i]);
//                break;
//                
//            case ENEMY_HONGYIWUSHI:
//                sprintf(temp, "MuscleZombie(HONGYIWUSHI):%d", enemyCount[i]);
//                break;
//            case ENEMY_HEIYIWUSHI:
//                sprintf(temp, "WolfZombie:%d", enemyCount[i]);
//                break;
//            case ENEMY_ZHAOHUANSHI:
//                sprintf(temp, "FemaleZombie(ZHAOHUANSHI):%d", enemyCount[i]);
//                break;
//            case ENEMY_GOUTOUJUNSHI:
//                sprintf(temp, "Doctor1(GOUTOUJUNSHI):%d", enemyCount[i]);
//                break;
//            case ENEMY_JIAMIANJUNSHI:
//                sprintf(temp, "Doctor1(JIAMIANJUNSHI):%d", enemyCount[i]);
//                break;
//            case ENEMY_HONGYIWUSHIFLY:
//                sprintf(temp, "FlyZombie3:%d", enemyCount[i]);
//                break;
//            default:
//                break;
//        }
//        
//        LOGE("%s", temp);
//    }
//    LOGE("=====================enemy count end...=========================");
//    delete [] enemyCount;

//    LOGE("p2m:%f", m_box2d->getMeterPixels());
}

void MainGameLayer::appendEndlesssMap() {
    m_TileMap = wyTMXTileMap::make("map/s01e10.tmx", false, MyTextureManager::makePNG("s01.png"), NULL);
    m_TileMap->setScaleX(sGlobal->scaleX);
    m_TileMap->setScaleY(sGlobal->scaleY);
    m_TileMap->setAnchor(0, 0);

    m_TileMap->setPosition(sGlobal->tileMapWidthInPixels, sGlobal->tileMapOffsetY);
    sGlobal->baseTileMapWidthInPixels = sGlobal->tileMapWidthInPixels;
    sGlobal->mapOffsetX = sGlobal->tileMapWidthInPixels;
    sGlobal->tileMapWidthInPixels = m_TileMap->getMapWidth() * m_TileMap->getTileWidth() * sGlobal->scaleX;
    addChildLocked(m_TileMap, -1);
    m_TileMapArray[sGlobal->mapPieceNumber+1] = m_TileMap;
    
    m_TileMap->setVisible(false);
    
    b2World* world = m_box2d->getWorld();
    wyTMXObjectGroup* og = m_TileMap->getObjectGroup("obj_terrain");
    for (int j = 0; j < og->getObjectCount(); j++) {
        wyTMXObject* obj = og->getObjectAt(j);
        float x = atof(og->getObjectProperty(obj, "x")) * sGlobal->scaleX + sGlobal->mapOffsetX;
        float y = sGlobal->tileMapHeightInPixels - atof(og->getObjectProperty(obj, "y")) * sGlobal->scaleY;
        float width = atof(og->getObjectProperty(obj, "width")) * sGlobal->scaleX;
        const char *type = og->getObjectProperty(obj, "type");
        b2Body* ground1 = NULL;
        BaseObject *obj2 = WYNEW BaseObject();
        
        if (type != NULL && strcmp(type, "bound") == 0) {

        } else {
            obj2->obj_type = TYPE_GROUND;
            {
                b2BodyDef bd;
                bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + sGlobal->tileMapOffsetY));
                ground1 = world->CreateBody(&bd);
                
                b2EdgeShape top;
                top.Set(b2Vec2(0, 0), b2Vec2(m_box2d->pixel2Meter(width), 0));
                b2FixtureDef fixtureDef;
                fixtureDef.density = 0.0f;
                fixtureDef.friction = 1.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.shape = &top;
                fixtureDef.isSensor = false;
                fixtureDef.filter.categoryBits = CAT_BITS[TYPE_GROUND];
                fixtureDef.filter.maskBits = MASK_BITS[TYPE_GROUND];
                fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_GROUND];
                
                ground1->CreateFixture(&fixtureDef);
                ground1->SetUserData(obj2);
                obj2->body = ground1;
                groundBody.push_back(ground1);
                obj2->widthInPixels = width;
                
                obj2->startX = x;
                obj2->endX = x + width;
                obj2->sub_type = 987;
            }
        }
    }
    
    m_TileMap1 = wyTMXTileMap::make("map/s01e11.tmx", false, MyTextureManager::makePNG("s01.png"), NULL);
    m_TileMap1->setScaleX(sGlobal->scaleX);
    m_TileMap1->setScaleY(sGlobal->scaleY);
    m_TileMap1->setAnchor(0, 0);
    
    m_TileMap1->setPosition(sGlobal->baseTileMapWidthInPixels+sGlobal->tileMapWidthInPixels, sGlobal->tileMapOffsetY);
    sGlobal->tileMapWidthInPixels1 = m_TileMap1->getMapWidth() * m_TileMap1->getTileWidth() * sGlobal->scaleX;
    addChildLocked(m_TileMap1, -1);
    m_TileMapArray[sGlobal->mapPieceNumber+2] = m_TileMap1;
    
    m_TileMap1->setVisible(false);
    
    og = m_TileMap1->getObjectGroup("obj_terrain");
    for (int j = 0; j < og->getObjectCount(); j++) {
        wyTMXObject* obj = og->getObjectAt(j);
        float x = atof(og->getObjectProperty(obj, "x")) * sGlobal->scaleX + sGlobal->baseTileMapWidthInPixels+sGlobal->tileMapWidthInPixels;
        float y = sGlobal->tileMapHeightInPixels - atof(og->getObjectProperty(obj, "y")) * sGlobal->scaleY;
        float width = atof(og->getObjectProperty(obj, "width")) * sGlobal->scaleX;
        const char *type = og->getObjectProperty(obj, "type");
        b2Body* ground1 = NULL;
        BaseObject *obj2 = WYNEW BaseObject();
        
        if (type != NULL && strcmp(type, "bound") == 0) {
            
        } else {
            obj2->obj_type = TYPE_GROUND;
            {
                b2BodyDef bd;
                bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + sGlobal->tileMapOffsetY));
                ground1 = world->CreateBody(&bd);
                
                b2EdgeShape top;
                top.Set(b2Vec2(0, 0), b2Vec2(m_box2d->pixel2Meter(width), 0));
                b2FixtureDef fixtureDef;
                fixtureDef.density = 0.0f;
                fixtureDef.friction = 1.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.shape = &top;
                fixtureDef.isSensor = false;
                fixtureDef.filter.categoryBits = CAT_BITS[TYPE_GROUND];
                fixtureDef.filter.maskBits = MASK_BITS[TYPE_GROUND];
                fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_GROUND];
                
                ground1->CreateFixture(&fixtureDef);
                ground1->SetUserData(obj2);
                obj2->body = ground1;
                groundBody1.push_back(ground1);
                obj2->widthInPixels = width;
                
                obj2->startX = x;
                obj2->endX = x + width;
                obj2->sub_type = 987;
            }
        }
    }
    
    sGlobal->mapPieceNumber += 2;
}

void MainGameLayer::loadEndlessMap(int mapIndex) {
    sGlobal->mapOffsetX = 0;
    int seasonId = mapIndex / 100;
    int mapId = mapIndex % 100;
    char *mapName = new char[20];
    sprintf(mapName, "map/s%02de%02d.tmx", seasonId, mapId);
    char *textureName = new char[20];
    sprintf(textureName, "s%02d.png", seasonId);
    
    m_TileMap = wyTMXTileMap::make(mapName, false, MyTextureManager::makePNG(textureName), NULL);
    delete [] mapName;
    delete [] textureName;
    
    needAutoScroll = true;
    
    sGlobal->tileMapOffsetY = - DP(32) * sGlobal->scaleY * atoi(m_TileMap->getProperty("offsetY"))+sGlobal->tileMapOffsetYExtra;
    
    char *mapName1 = new char[20];
    sprintf(mapName1, "map/s%02de%02d.tmx", seasonId, mapId + 1);
    char *textureName1 = new char[20];
    sprintf(textureName1, "s%02d.png", seasonId);
    wyTexture2D *tex = MyTextureManager::makePNG(textureName1);
    m_TileMap1 = wyTMXTileMap::make(mapName1, false, tex, NULL);
    delete [] mapName1;
    delete [] textureName1;
    
    m_TileMap->setScaleX(sGlobal->scaleX);
    m_TileMap->setScaleY(sGlobal->scaleY);
	m_TileMap->setAnchor(0, 0);
	m_TileMap->setPosition(0, sGlobal->tileMapOffsetY);
	m_TileMap->setClipRect(wyr(0, 0, wyDevice::winWidth, wyDevice::winHeight));
	sGlobal->tileMapWidthInPixels = m_TileMap->getMapWidth() * m_TileMap->getTileWidth() * sGlobal->scaleX;
	sGlobal->tileMapHeightInPixels = m_TileMap->getMapHeight() * m_TileMap->getTileHeight() * sGlobal->scaleY;
    addChildLocked(m_TileMap, -1);
	//m_TileMap->setVisible(false);
    sGlobal->tileWidth = m_TileMap->getTileWidth()*sGlobal->scaleX;
    
	wyTMXLayer *front0 = m_TileMap->getLayer("tile_terrain_deco_front");
//    wyTMXLayer *front1 = m_TileMap->getLayer("tile_terrain_deco_back");
	if (front0 != NULL) { // && front1 != NULL
		front0->setScaleX(sGlobal->scaleX);
		front0->setScaleY(sGlobal->scaleY);
		front0->setAnchor(0, 0);
		front0->setPosition(m_TileMap->getPositionX(), m_TileMap->getPositionY());
		m_TileMap->removeChildLocked(front0, false);
        this->addChildLocked(front0, 1000);
	}
    
    if (isEndless) {
        m_TileMap1->setScaleX(sGlobal->scaleX);
        m_TileMap1->setScaleY(sGlobal->scaleY);
        m_TileMap1->setAnchor(0, 0);
        //m_TileMap1->setVisible(false);
        addChildLocked(m_TileMap1, -1);
        m_TileMap1->setPosition(sGlobal->tileMapWidthInPixels, sGlobal->tileMapOffsetY);
        sGlobal->tileMapWidthInPixels1 = m_TileMap1->getMapWidth() * m_TileMap1->getTileWidth() * sGlobal->scaleX;
    }
    
    m_box2d = new wyBox2D();
    m_box2d->setDebugDraw(sGlobal->isShowBox2DDebug);
    addChildLocked(m_box2d);
    
    b2World* world = m_box2d->getWorld();
    world->SetGravity(b2Vec2(0, -112));
    world->SetContinuousPhysics(true);
    m_box2d->setPosition(0, 0);
    world->SetAutoClearForces(false);
//    float meterWidth = m_box2d->pixel2Meter(wyDevice::winWidth);
//    float meterHeight = m_box2d->pixel2Meter(wyDevice::winHeight);
    
    hero = WYNEW Hero(NULL, needAutoScroll);
    
    b2Body* ground = NULL;
    {
//        b2BodyDef bd;
//        bd.position.Set(0, meterHeight / 2);
//        ground = world->CreateBody(&bd);
//        // left edge
//        b2EdgeShape left;
//        left.Set(b2Vec2(0, -meterHeight / 2), b2Vec2(0, meterHeight / 2));
//        b2FixtureDef fixtureDef;
//        fixtureDef.density = 0.0f;
//        fixtureDef.friction = 0.0f;
//        fixtureDef.restitution = 0.0f;
//        fixtureDef.shape = &left;
//        ground->CreateFixture(&fixtureDef);
//        BaseObject *obj = WYNEW BaseObject();
//        obj->obj_type = TYPE_WORLD_BOUND;
//        obj->body = ground;
//        ground->SetUserData(obj);
    }
    
    //生成地面
    wyTMXObjectGroup* og = ((wyTMXTileMap*)m_TileMap)->getObjectGroup("obj_terrain");
    for (int i = 0; i < og->getObjectCount(); i++) {
        wyTMXObject* obj = og->getObjectAt(i);
        float x = atof(og->getObjectProperty(obj, "x")) * sGlobal->scaleX;
        //LOGE("x:%f", atof(og->getObjectProperty(obj, "x")));
        float y = sGlobal->tileMapHeightInPixels - atof(og->getObjectProperty(obj, "y")) * sGlobal->scaleY;
        float width = atof(og->getObjectProperty(obj, "width")) * sGlobal->scaleX;
        //LOGE("width:%f", atof(og->getObjectProperty(obj, "width")));
//        float height = atof(og->getObjectProperty(obj, "height"));
        const char *type = og->getObjectProperty(obj, "type");
        b2Body* ground1 = NULL;
        BaseObject *obj2 = WYNEW BaseObject();
        if (type != NULL && strcmp(type, "bound") == 0) {
            obj2->obj_type = TYPE_WORLD_BOUND;
            {
                b2BodyDef bd;
                bd.position.Set(m_box2d->pixel2Meter(x), 0);
                ground1 = world->CreateBody(&bd);
                
                b2EdgeShape top;
                top.Set(b2Vec2(0, 0), b2Vec2(0, m_box2d->pixel2Meter(wyDevice::winHeight)));
                b2FixtureDef fixtureDef;
                fixtureDef.density = 0.0f;
                fixtureDef.friction = 0.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.shape = &top;
                fixtureDef.filter.categoryBits = CAT_BITS[TYPE_WORLD_BOUND];
                fixtureDef.filter.maskBits = MASK_BITS[TYPE_WORLD_BOUND];
                fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_WORLD_BOUND];
                
                ground1->CreateFixture(&fixtureDef);
                ground1->SetUserData(obj2);
                obj2->body = ground1;
            }
            sGlobal->tileMapWidthInPixels = x;
        } else {
            obj2->obj_type = TYPE_GROUND;
            if (x == 0) {
                x -= DP(1000)*sGlobal->scaleX;
                width += DP(1000)*sGlobal->scaleX;
            }
            {
                b2BodyDef bd;
                bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + sGlobal->tileMapOffsetY));
                ground1 = world->CreateBody(&bd);
                
                b2EdgeShape top;
                top.Set(b2Vec2(0, 0), b2Vec2(m_box2d->pixel2Meter(width), 0));
                b2FixtureDef fixtureDef;
                fixtureDef.density = 0.0f;
                fixtureDef.friction = 1.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.shape = &top;
                fixtureDef.filter.categoryBits = CAT_BITS[TYPE_GROUND];
                fixtureDef.filter.maskBits = MASK_BITS[TYPE_GROUND];
                fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_GROUND];
                
                ground1->CreateFixture(&fixtureDef);
                ground1->SetUserData(obj2);
                obj2->body = ground1;
                groundBody.push_back(ground1);
                obj2->widthInPixels = width;
                
                obj2->startX = x;
                obj2->endX = x + width;
                
                obj2->sub_type = 987;
            }
        }
        
        if (obj2->obj_type == TYPE_GROUND) {
//            hero->body->SetTransform(b2Vec2(m_box2d->pixel2Meter(sGlobal->hero_offset_left), m_box2d->pixel2Meter(hero->bodyRect.height/2 + y + sGlobal->tileMapOffsetY + DP(10))), 0);
            hero->body->SetTransform(b2Vec2(m_box2d->pixel2Meter(sGlobal->hero_offset_left - DP(400)*sGlobal->scaleX), m_box2d->pixel2Meter(hero->bodyRect.height/2 + y + sGlobal->tileMapOffsetY + DP(30)*sGlobal->scaleY) - hero->ofyInMeter + hero->halfBodyHeightInMeter), 0);// + hero->ofyInMeter
            hero->container->setPosition(m_box2d->meter2Pixel(hero->body->GetPosition().x) + hero->offsetX, m_box2d->meter2Pixel(hero->body->GetPosition().y) + hero->offsetY);
//            LOGE("groundY:%f, heroY:%f", y + sGlobal->tileMapOffsetY, hero->body->GetPosition().y);
        }
    }
    
    if (isEndless) {
        {
            wyTMXObjectGroup* og = ((wyTMXTileMap*)m_TileMap1)->getObjectGroup("obj_terrain");
            for (int i = 0; i < og->getObjectCount(); i++) {
                wyTMXObject* obj = og->getObjectAt(i);
                float x = atof(og->getObjectProperty(obj, "x")) * sGlobal->scaleX + sGlobal->tileMapWidthInPixels;
                //LOGE("x:%f", atof(og->getObjectProperty(obj, "x")));
                float y = sGlobal->tileMapHeightInPixels - atof(og->getObjectProperty(obj, "y")) * sGlobal->scaleY;
                float width = atof(og->getObjectProperty(obj, "width")) * sGlobal->scaleX;
                //LOGE("width:%f", atof(og->getObjectProperty(obj, "width")));
//                float height = atof(og->getObjectProperty(obj, "height"));
//                const char *type = og->getObjectProperty(obj, "type");
                b2Body* ground1 = NULL;
                BaseObject *obj2 = WYNEW BaseObject();
                obj2->obj_type = TYPE_GROUND;
                {
                    b2BodyDef bd;
                    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + sGlobal->tileMapOffsetY));
                    ground1 = world->CreateBody(&bd);
                    
                    b2EdgeShape top;
                    top.Set(b2Vec2(0, 0), b2Vec2(m_box2d->pixel2Meter(width), 0));
                    b2FixtureDef fixtureDef;
                    fixtureDef.density = 0.0f;
                    fixtureDef.friction = 1.0f;
                    fixtureDef.restitution = 0.0f;
                    fixtureDef.shape = &top;
                    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_GROUND];
                    fixtureDef.filter.maskBits = MASK_BITS[TYPE_GROUND];
                    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_GROUND];
                    
                    ground1->CreateFixture(&fixtureDef);
                    ground1->SetUserData(obj2);
                    obj2->body = ground1;
                    groundBody1.push_back(ground1);
                    obj2->widthInPixels = width;
                    
                    obj2->startX = x;
                    obj2->endX = x + width;
                    
                    obj2->sub_type = 987;
                }
            }
        }
    }
    
    sEnemyFactory->createEnemies(((wyTMXTileMap*)m_TileMap)->getObjectGroup("obj_enemies"));
    
    sObjectsFactory->createObjects(((wyTMXTileMap*)m_TileMap)->getObjectGroup("obj_objects"));
}

void MainGameLayer::loadRealEndlessMap(int mapIndex) {
    sGlobal->realEndlessMapWidth = 0;

    m_TileMap = sGlobal->getSEMap(sGlobal->currentSeasonId);
    needAutoScroll = true;
    
    sGlobal->tileMapOffsetY = - DP(32) * sGlobal->scaleY * atoi(m_TileMap->getProperty("offsetY"))+sGlobal->tileMapOffsetYExtra;
    
    m_TileMap->setScaleX(sGlobal->scaleX);
    m_TileMap->setScaleY(sGlobal->scaleY);
	m_TileMap->setAnchor(0, 0);
	m_TileMap->setPosition(0, sGlobal->tileMapOffsetY);
	m_TileMap->setClipRect(wyr(0, 0, wyDevice::winWidth, wyDevice::winHeight));
	sGlobal->tileMapWidthInPixels = m_TileMap->getMapWidth() * m_TileMap->getTileWidth() * sGlobal->scaleX;
	sGlobal->tileMapHeightInPixels = m_TileMap->getMapHeight() * m_TileMap->getTileHeight() * sGlobal->scaleY;
    addChildLocked(m_TileMap, -1);
    sGlobal->tileWidth = m_TileMap->getTileWidth()*sGlobal->scaleX;
//    int size = sizeof(*m_TileMap);
    
	front0 = sGlobal->getSEMapFront(sGlobal->currentSeasonId);
	if (front0 != NULL) {
		front0->setScaleX(sGlobal->scaleX);
		front0->setScaleY(sGlobal->scaleY);
		front0->setAnchor(0, 0);
		front0->setPosition(m_TileMap->getPositionX(), m_TileMap->getPositionY());
        this->addChildLocked(front0, 1000);
	}
    
    int currentMapId;
    m_TileMap1 = sGlobal->getMap(sGlobal->currentSeasonId, currentMapId);
    if (isRealEndless) {
        m_TileMap1->setScaleX(sGlobal->scaleX);
        m_TileMap1->setScaleY(sGlobal->scaleY);
        m_TileMap1->setAnchor(0, 0);
        addChildLocked(m_TileMap1, -1);
        m_TileMap1->setPosition(sGlobal->tileMapWidthInPixels, sGlobal->tileMapOffsetY);
        sGlobal->tileMapWidthInPixels1 = m_TileMap1->getMapWidth() * m_TileMap1->getTileWidth() * sGlobal->scaleX;
    }
    front1 = sGlobal->getMapFront(sGlobal->currentSeasonId, currentMapId);
	if (front1 != NULL) {
		front1->setScaleX(sGlobal->scaleX);
		front1->setScaleY(sGlobal->scaleY);
		front1->setAnchor(0, 0);
		front1->setPosition(m_TileMap1->getPositionX(), m_TileMap1->getPositionY());
        this->addChildLocked(front1, 200);
	}
    
    m_box2d = new wyBox2D();
    m_box2d->setDebugDraw(sGlobal->isShowBox2DDebug);
    addChildLocked(m_box2d);
    
    b2World* world = m_box2d->getWorld();
    world->SetGravity(b2Vec2(0, -112));
    world->SetContinuousPhysics(true);
    m_box2d->setPosition(0, 0);
    world->SetAutoClearForces(false);
//    float meterWidth = m_box2d->pixel2Meter(wyDevice::winWidth);
//    float meterHeight = m_box2d->pixel2Meter(wyDevice::winHeight);
    
    hero = WYNEW Hero(NULL, needAutoScroll);
    
    //生成地面
    wyTMXObjectGroup* og = ((wyTMXTileMap*)m_TileMap)->getObjectGroup("obj_terrain");
    for (int i = 0; i < og->getObjectCount(); i++) {
        wyTMXObject* obj = og->getObjectAt(i);
        float x = atof(og->getObjectProperty(obj, "x")) * sGlobal->scaleX;
        //LOGE("x:%f", atof(og->getObjectProperty(obj, "x")));
        float y = sGlobal->tileMapHeightInPixels - atof(og->getObjectProperty(obj, "y")) * sGlobal->scaleY;
        float width = atof(og->getObjectProperty(obj, "width")) * sGlobal->scaleX;
        //LOGE("width:%f", atof(og->getObjectProperty(obj, "width")));
//        float height = atof(og->getObjectProperty(obj, "height"));
        const char *type = og->getObjectProperty(obj, "type");
        b2Body* ground1 = NULL;
        BaseObject *obj2 = WYNEW BaseObject();
        bool isStart = false;
        
        if (type != NULL && strcmp(type, "bound") == 0) {
            obj2->obj_type = TYPE_WORLD_BOUND;
            {
                b2BodyDef bd;
                bd.position.Set(m_box2d->pixel2Meter(x), 0);
                ground1 = world->CreateBody(&bd);
                
                b2EdgeShape top;
                top.Set(b2Vec2(0, 0), b2Vec2(0, m_box2d->pixel2Meter(wyDevice::winHeight)));
                b2FixtureDef fixtureDef;
                fixtureDef.density = 0.0f;
                fixtureDef.friction = 0.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.shape = &top;
                fixtureDef.filter.categoryBits = CAT_BITS[TYPE_WORLD_BOUND];
                fixtureDef.filter.maskBits = MASK_BITS[TYPE_WORLD_BOUND];
                fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_WORLD_BOUND];
                
                ground1->CreateFixture(&fixtureDef);
                ground1->SetUserData(obj2);
                obj2->body = ground1;
            }
            sGlobal->tileMapWidthInPixels = x;
        } else {
            obj2->obj_type = TYPE_GROUND;
            if (x == 0) {
                x -= DP(1000)*sGlobal->scaleX;
                width += DP(1000)*sGlobal->scaleX;
                isStart = true;
            }
            {
                b2BodyDef bd;
                bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + sGlobal->tileMapOffsetY));
                ground1 = world->CreateBody(&bd);
                
                b2EdgeShape top;
                top.Set(b2Vec2(0, 0), b2Vec2(m_box2d->pixel2Meter(width), 0));
                b2FixtureDef fixtureDef;
                fixtureDef.density = 0.0f;
                fixtureDef.friction = 1.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.shape = &top;
                fixtureDef.filter.categoryBits = CAT_BITS[TYPE_GROUND];
                fixtureDef.filter.maskBits = MASK_BITS[TYPE_GROUND];
                fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_GROUND];
                
                ground1->CreateFixture(&fixtureDef);
                ground1->SetUserData(obj2);
                obj2->body = ground1;
                obj2->widthInPixels = width;
                
                obj2->startX = x;
                obj2->endX = x + width;
            }
        }
        
        if (isStart && obj2->obj_type == TYPE_GROUND) {
//            hero->body->SetTransform(b2Vec2(m_box2d->pixel2Meter(sGlobal->hero_offset_left - DP(500)*sGlobal->scaleX), m_box2d->pixel2Meter(hero->bodyRect.height/2 + y + sGlobal->tileMapOffsetY + DP(10))), 0);
            hero->body->SetTransform(b2Vec2(m_box2d->pixel2Meter(sGlobal->hero_offset_left - DP(400)*sGlobal->scaleX), m_box2d->pixel2Meter(hero->bodyRect.height/2 + y + sGlobal->tileMapOffsetY + DP(10)*sGlobal->scaleY) - hero->ofyInMeter + hero->halfBodyHeightInMeter), 0);// + hero->ofyInMeter
            hero->container->setPosition(m_box2d->meter2Pixel(hero->body->GetPosition().x) + hero->offsetX, m_box2d->meter2Pixel(hero->body->GetPosition().y) + hero->offsetY);
        }
    }
    
    if (isRealEndless) {
        {
            wyTMXObjectGroup* og = ((wyTMXTileMap*)m_TileMap1)->getObjectGroup("obj_terrain");
            for (int i = 0; i < og->getObjectCount(); i++) {
                wyTMXObject* obj = og->getObjectAt(i);
                float x = atof(og->getObjectProperty(obj, "x")) * sGlobal->scaleX + sGlobal->tileMapWidthInPixels;
                //LOGE("x:%f", atof(og->getObjectProperty(obj, "x")));
                float y = sGlobal->tileMapHeightInPixels - atof(og->getObjectProperty(obj, "y")) * sGlobal->scaleY;
                float width = atof(og->getObjectProperty(obj, "width")) * sGlobal->scaleX;
                //LOGE("width:%f", atof(og->getObjectProperty(obj, "width")));
//                float height = atof(og->getObjectProperty(obj, "height"));
//                const char *type = og->getObjectProperty(obj, "type");
                b2Body* ground1 = NULL;
                BaseObject *obj2 = WYNEW BaseObject();
                obj2->obj_type = TYPE_GROUND;
                {
                    b2BodyDef bd;
                    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + sGlobal->tileMapOffsetY));
                    ground1 = world->CreateBody(&bd);
                    
                    b2EdgeShape top;
                    top.Set(b2Vec2(0, 0), b2Vec2(m_box2d->pixel2Meter(width), 0));
                    b2FixtureDef fixtureDef;
                    fixtureDef.density = 0.0f;
                    fixtureDef.friction = 1.0f;
                    fixtureDef.restitution = 0.0f;
                    fixtureDef.shape = &top;
                    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_GROUND];
                    fixtureDef.filter.maskBits = MASK_BITS[TYPE_GROUND];
                    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_GROUND];
                    
                    ground1->CreateFixture(&fixtureDef);
                    ground1->SetUserData(obj2);
                    obj2->body = ground1;
                    obj2->widthInPixels = width;
                    
                    obj2->startX = x;
                    obj2->endX = x + width;
                }
            }
        }
    }
    
    sGlobal->realEndlessMapWidth = 0;
    sGlobal->mapOffsetX = sGlobal->realEndlessMapWidth;
    sEnemyFactory->createEnemies(((wyTMXTileMap*)m_TileMap)->getObjectGroup("obj_enemies"));
    sObjectsFactory->createObjects(((wyTMXTileMap*)m_TileMap)->getObjectGroup("obj_objects"));
    
    sGlobal->realEndlessMapWidth += sGlobal->tileMapWidthInPixels;
    sGlobal->mapOffsetX = sGlobal->realEndlessMapWidth;
    sEnemyFactory->createEnemies(((wyTMXTileMap*)m_TileMap1)->getObjectGroup("obj_enemies"));
    sObjectsFactory->createObjects(((wyTMXTileMap*)m_TileMap1)->getObjectGroup("obj_objects"));
    
    sGlobal->realEndlessMapWidth += sGlobal->tileMapWidthInPixels1;
    sGlobal->mapOffsetX = sGlobal->realEndlessMapWidth;
    
    bossFightIndex = rand()%5+3;
    int seed = rand()%4;
    if (seed == 0) {
        sGlobal->isBossFighted = true;
    } else {
        sGlobal->isBossFighted = false;
    }
//    sGlobal->isBossFighted = false;
    sGlobal->isInBossBattle = false;
}

//8个小地图切换为1个场景，3个场景切换为1轮
void MainGameLayer::generateNextMap(wyTMXTileMap* nextMapPtr, int idx) {
//    LOGE("isBossFighted:%d, isInBossBattle:%d, bossFightIndex:%d, elapsedNormalCombat:%d", sGlobal->isBossFighted ? 1:0, sGlobal->isInBossBattle ? 1:0, bossFightIndex, sGlobal->elapsedNormalCombat);
    if (!sGlobal->isBossFighted && !sGlobal->isInBossBattle && (sGlobal->elapsedNormalCombat + 1 == bossFightIndex)) {
        summonBoss();
        sGlobal->isInBossBattle = true;
    } else {
        if (!sGlobal->isInBossBattle) {
            sGlobal->elapsedNormalCombat += 1;
        }
    }

//    LOGE("elapsed normal combat:%d", sGlobal->elapsedNormalCombat);
    if (nextMapPtr != NULL) {
        removeChildLocked(nextMapPtr, false);
    }
    if (idx == 0) {
        if (front0 != NULL) {
            this->removeChildLocked(front0, false);
        }
    }
    else if (idx == 1) {
        if (front1 != NULL) {
            this->removeChildLocked(front1, false);
        }
    }
    
    b2World* world = m_box2d->getWorld();
    
//    int seasonId = 1;
//    char *mapName1 = new char[20];
//    sprintf(mapName1, "R.raw.s%02d_%02d", seasonId, 1);
//    char *textureName1 = new char[20];
//    sprintf(textureName1, "R.drawable.s%02d", seasonId);
//    LOGE("generate mapname:%s, texname:%s", mapName1, textureName1);
//    wyTexture2D *tex = MyTextureManager::makePNG(RES(textureName1));
//    nextMapPtr = wyTMXTileMap::make(RES(mapName1), tex, NULL);
    
//    int nextMapIndex = rand()%26;
//    while (nextMapIndex == curMapIndex) {
//        nextMapIndex = rand()%26;
//    }
//    curMapIndex = nextMapIndex;
    int mapIndex;
    wyTMXLayer *front;
    if (sGlobal->elapsedNormalCombat == sGlobal->MAX_COMBAT_TIME+1) {
        nextMapPtr = sGlobal->getSEMap(sGlobal->currentSeasonId);
        front = sGlobal->getSEMapFront(sGlobal->currentSeasonId);
    } else {
        nextMapPtr = sGlobal->getMap(sGlobal->currentSeasonId, mapIndex);
        front = sGlobal->getMapFront(sGlobal->currentSeasonId, mapIndex);
    }
    
    nextMapPtr->setScaleX(sGlobal->scaleX);
    nextMapPtr->setScaleY(sGlobal->scaleY);
    nextMapPtr->setAnchor(0, 0);
    addChildLocked(nextMapPtr, -1);
//    LOGE("endless mapIndex:%d", mapIndex);
    
	if (front != NULL) {
		front->setScaleX(sGlobal->scaleX);
		front->setScaleY(sGlobal->scaleY);
		front->setAnchor(0, 0);
        this->addChildLocked(front, 200);
	}
//    wyTMXLayer *front = nextMapPtr->getLayer("tile_terrain_deco_front");
//	if (front != NULL) {
//		front->setScaleX(sGlobal->scaleX);
//		front->setScaleY(sGlobal->scaleY);
//		front->setAnchor(0, 0);
//        front->retain();
//		nextMapPtr->removeChildLocked(front, false);
//        this->addChildLocked(front, 200);
//        front->release();
//	}
    
    wyTMXObjectGroup* og = ((wyTMXTileMap*)nextMapPtr)->getObjectGroup("obj_terrain");
    for (int i = 0; i < og->getObjectCount(); i++) {
        wyTMXObject* obj = og->getObjectAt(i);
        float x = atof(og->getObjectProperty(obj, "x")) * sGlobal->scaleX + sGlobal->realEndlessMapWidth;
        //LOGE("x:%f", atof(og->getObjectProperty(obj, "x")));
        float y = sGlobal->tileMapHeightInPixels - atof(og->getObjectProperty(obj, "y")) * sGlobal->scaleY;
        float width = atof(og->getObjectProperty(obj, "width")) * sGlobal->scaleX;
        //LOGE("width:%f", atof(og->getObjectProperty(obj, "width")));
//        float height = atof(og->getObjectProperty(obj, "height"));
//        const char *type = og->getObjectProperty(obj, "type");
        b2Body* ground1 = NULL;
        BaseObject *obj2 = WYNEW BaseObject();
        obj2->obj_type = TYPE_GROUND;
        {
            b2BodyDef bd;
            bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y + sGlobal->tileMapOffsetY));
            ground1 = world->CreateBody(&bd);
            
            b2EdgeShape top;
            top.Set(b2Vec2(0, 0), b2Vec2(m_box2d->pixel2Meter(width), 0));
            b2FixtureDef fixtureDef;
            fixtureDef.density = 0.0f;
            fixtureDef.friction = 1.0f;
            fixtureDef.restitution = 0.0f;
            fixtureDef.shape = &top;
            fixtureDef.filter.categoryBits = CAT_BITS[TYPE_GROUND];
            fixtureDef.filter.maskBits = MASK_BITS[TYPE_GROUND];
            fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_GROUND];
            
            ground1->CreateFixture(&fixtureDef);
            ground1->SetUserData(obj2);
            obj2->body = ground1;
            obj2->widthInPixels = width;
            
            obj2->startX = x;
            obj2->endX = x + width;
        }
    }
    
    sGlobal->mapOffsetX = sGlobal->realEndlessMapWidth;
    sEnemyFactory->createEnemies(((wyTMXTileMap*)nextMapPtr)->getObjectGroup("obj_enemies"));
    sObjectsFactory->createObjects(((wyTMXTileMap*)nextMapPtr)->getObjectGroup("obj_objects"));
    
    sGlobal->realEndlessMapWidth += nextMapPtr->getMapWidth() * nextMapPtr->getTileWidth() * sGlobal->scaleX;
//    LOGE("next map offsetX: %f", sGlobal->realEndlessMapWidth);
    if (idx == 0) {
        m_TileMap = nextMapPtr;
        front0 = front;
        sGlobal->tileMapWidthInPixels = nextMapPtr->getMapWidth() * nextMapPtr->getTileWidth() * sGlobal->scaleX;
    } else {
        m_TileMap1 = nextMapPtr;
        front1 = front;
        sGlobal->tileMapWidthInPixels1 = nextMapPtr->getMapWidth() * nextMapPtr->getTileWidth() * sGlobal->scaleX;
    }
}

const float32 FIXED_TIMESTEP = 1.0f / 30.0f;

// Minimum remaining time to avoid box2d unstability caused by very small delta times
// if remaining time to simulate is smaller than this, the rest of time will be added to the last step,
// instead of performing one more single step with only the small delta time.
const float32 MINIMUM_TIMESTEP = 1.0f / 300.0f;

const int32 VELOCITY_ITERATIONS = 4;
const int32 POSITION_ITERATIONS = 3;

// maximum number of steps per tick to avoid spiral of death
const int32 MAXIMUM_NUMBER_OF_STEPS = 2;

void MainGameLayer::updateWorld(wyTargetSelector* ts) {
    float dt = ts->getDelta();
    b2World* world = m_box2d->getWorld();

    if (isFirstTime) {
        toDestroy.clear();
        toDisable.clear();
        toEnable.clear();
        toRecreateFixture.clear();
        bodyPiecePool.clear();
        isFirstTime = false;
    }
    
    if (hasSwitchedSeason) {
//        LOGE("hasSwitchedSeason");
        return;
    }
    
    if (needSwitchSeason) {
//        LOGE("switching Season");
        needSwitchSeason = false;
        hasSwitchedSeason = true;
        ((MainScene *)this->getParent())->switchSeaon(nextSeasonId);
        return;
    }
    
    try {
        float32 frameTime = dt;
        int stepsPerformed = 0;
        while ( (frameTime > 0.0) && (stepsPerformed < MAXIMUM_NUMBER_OF_STEPS) ){
            float32 deltaTime = std::min( frameTime, FIXED_TIMESTEP );
            frameTime -= deltaTime;
            if (frameTime < MINIMUM_TIMESTEP) {
                deltaTime += frameTime;
                frameTime = 0.0f;
            }
            world->Step(deltaTime,VELOCITY_ITERATIONS,POSITION_ITERATIONS);
            stepsPerformed++;
            consumeContacts_();
        }
        
        dealPhysics(dt);
        
    }
    catch(std::logic_error exp) {
        LOGE("exception found 0...");
        LOGE("%s", exp.what());
    }
    catch(...) {
        LOGE("exception found 1...");
    }
    
    sGlobal->hudLayer->update(dt);
}

void MainGameLayer::renderFullStep(float dt) {
//    b2World* world = m_box2d->getWorld();
    
    dealPhysics(dt);
}

void MainGameLayer::renderPartStep(const float alpha) {
    b2World* world = m_box2d->getWorld();
    
    const float reminder = 1.0f - alpha;
    for (b2Body *body = world->GetBodyList(); body; body = body->GetNext()) {
        if (body->GetType() != b2_staticBody) {
            BaseObject *obj = (BaseObject*)body->GetUserData();
            if (obj != NULL) {
                float xx = m_box2d->meter2Pixel(obj->body->GetPosition().x * alpha) + obj->container->getPositionX() * reminder;
                float yy = m_box2d->meter2Pixel(obj->body->GetPosition().y * alpha) + obj->container->getPositionY() * reminder;
                obj->container->setPosition(xx, yy);
            }
        }
    }
} 

void MainGameLayer::consumeContacts_() {
    if (needSwitchSeason || hasSwitchedSeason) {
        return;
    }
    
    b2World* world = m_box2d->getWorld();

    std::vector<CustomContact>::iterator pos;
    for(pos = _contactListener->_contacts.begin(); pos != _contactListener->_contacts.end(); ++pos) 
    {
        CustomContact contact = *pos;
        
        if (contact.fixtureA == NULL) {
            LOGE("contact a is null...");
        }
        
        if (contact.fixtureB == NULL) {
            LOGE("contact b is null...");
        }
        
        b2Body *actorA = contact.fixtureA->GetBody();
        b2Body *actorB = contact.fixtureB->GetBody();
        
        if (actorA == NULL || actorB == NULL) {
            return;
        }
        
        BaseObject *userDataA = (BaseObject*)actorA->GetUserData();
        BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
        
        int tagA = userDataA->obj_type;
        int tagB = userDataB->obj_type;
//        LOGE("tagA:%d, tagB:%d", tagA, tagB);
        if(tagA == TYPE_MAO) {
            userDataA->handleCollision(actorB);
        }
        else if (tagB == TYPE_MAO) {
            userDataB->handleCollision(actorA);
        }
        
        if (tagA == TYPE_BULLET && tagB == TYPE_ENEMY) {
            handleEnemyBulletCollision(actorB, actorA);
        }
        if (tagB == TYPE_BULLET && tagA == TYPE_ENEMY) {
            handleEnemyBulletCollision(actorA, actorB);
        }
        
        if (tagA == TYPE_MISSLE && tagB == TYPE_ENEMY) {
            handleEnemyDaBiaoCollision(actorB, actorA);
        }
        if (tagB == TYPE_MISSLE && tagA == TYPE_ENEMY) {
            handleEnemyDaBiaoCollision(actorA, actorB);
        }
        if (tagA == TYPE_MISSLE && tagB == TYPE_GROUND) {
            userDataA->handleCollision(actorB);
        }
        if (tagB == TYPE_MISSLE && tagA == TYPE_GROUND) {
            userDataB->handleCollision(actorA);
        }
        
        if (tagA == TYPE_LIUDAN && tagB == TYPE_ENEMY) {
            handleEnemyLiudanCollision(actorB, actorA);
        }
        if (tagB == TYPE_LIUDAN && tagA == TYPE_ENEMY) {
            handleEnemyLiudanCollision(actorA, actorB);
        }
//        if (tagA == TYPE_LIUDAN && tagB == TYPE_GROUND) {
//            userDataA->handleCollision(actorB);
//        }
//        if (tagB == TYPE_LIUDAN && tagA == TYPE_GROUND) {
//            userDataB->handleCollision(actorA);
//        }
        
        if (tagA == TYPE_ENEMY) {
            userDataA->handleCollision(actorB);
        } 
        else if (tagB == TYPE_ENEMY){
            userDataB->handleCollision(actorA);
        }
        
        if (tagA == TYPE_BOMB) {
            userDataA->handleCollision(actorB);
        } 
        else if (tagB == TYPE_BOMB){
            userDataB->handleCollision(actorA);
        }
        
        if (tagA == TYPE_BLOOD) {
            userDataA->handleCollision(actorB);
        }
        else if (tagB == TYPE_BLOOD){
            userDataB->handleCollision(actorA);
        }
        
        if (tagA == TYPE_BODY_PIECE) {
            userDataA->handleCollision(actorB);
        }
        else if (tagB == TYPE_BODY_PIECE){
            userDataB->handleCollision(actorA);
        }
        
        if (tagA == TYPE_BODY_SENSOR) {
            userDataA->handleCollision(actorB);
        }
        else if (tagB == TYPE_BODY_SENSOR){
            userDataB->handleCollision(actorA);
        }
        
        if (tagA == TYPE_GROUND_SENSOR) {
            userDataA->handleCollision(actorB);
        }
        else if (tagB == TYPE_GROUND_SENSOR){
            userDataB->handleCollision(actorA);
        }
        
        if (tagA == TYPE_HERO_HURT) {
            userDataA->handleCollision(actorB);
        }
        else if (tagB == TYPE_HERO_HURT){
            userDataB->handleCollision(actorA);
        }
    }
    _contactListener->_contacts.clear();
    
    std::vector<BaseObject *>::iterator pos5;
    for(pos5 = toRecreateFixture.begin(); pos5 != toRecreateFixture.end(); ++pos5) { 
        BaseObject *obj = *pos5;
        obj->createFixtureByCurrentFrame(obj->isSensor);
        obj->isRecreatingFixture = false;
    }
    toRecreateFixture.clear(); 
    
    std::vector<b2Body *>::iterator pos3;
    for(pos3 = toDisable.begin(); pos3 != toDisable.end(); ++pos3) { 
        b2Body *body = *pos3;
        body->SetActive(false);
        body->SetAwake(false);
    }
    toDisable.clear();
    
    std::vector<b2Body *>::iterator pos4;
    for(pos4 = toEnable.begin(); pos4 != toEnable.end(); ++pos4) { 
        b2Body *body = *pos4;
        body->SetActive(true);
        body->SetAwake(true);
    }
    toEnable.clear();
    
    std::vector<b2Fixture *>::iterator pos6;
    for(pos6 = toDestroyFixture.begin(); pos6 != toDestroyFixture.end(); ++pos6) {
        b2Fixture *fixture = *pos6;
        if (fixture != NULL) {
            b2Body *b = fixture->GetBody();
            if (b != NULL) {
                b->DestroyFixture(fixture);
            }
        }
    }
    toDestroyFixture.clear();
    
    std::vector<b2Body *>::iterator pos2;
    for(pos2 = toDestroy.begin(); pos2 != toDestroy.end(); ++pos2) { 
        b2Body *body = *pos2; 
        if (body == NULL || body->GetUserData() == NULL)
            continue;
        
        BaseObject *obj = (BaseObject*) body->GetUserData();
//        BaseObject *obj = dynamic_cast<BaseObject*>(body->GetUserData());
        if (obj != NULL) {
            switch (obj->obj_type) {
                case TYPE_MISSLE: {
                    Missle *missle = (Missle *)obj;
                    
//                    wyNode *sprite = (wyNode *) obj->container;
//                    sprite->getParent()->removeChildLocked(sprite, true);
//                    
//                    missle->deadSprite->getParent()->removeChildLocked(missle->deadSprite, true);
//                    
//                    delete obj;
//                    world->DestroyBody(body);
                    missle->destroyMyself();
                    
                    break;
                }
                case TYPE_LIUDAN: {
                    LiuDan *liudan = (LiuDan *)obj;
                    
                    liudan->destroyMe();
                    
                    break;
                }
                case TYPE_DABIAO:{
                    wyNode *sprite = (wyNode *) obj->container;
                    sprite->getParent()->removeChildLocked(sprite, true);
                    delete obj;
                    world->DestroyBody(body);
                    break;
                }
                case TYPE_BULLET: {
                    HeroBullet *bullet = (HeroBullet *)obj;
                    bullet->destroyMe();
                    break;
                }
                case TYPE_SHOCK_WAVE: {
                    world->DestroyBody(body);
                    wyNode *sprite = (wyNode *) obj->container;
                    wyFadeOut* fade = wyFadeOut::make(0.2f, true);
                    wyActionCallback callback = {
    					NULL,
    					onShockWaveDisappearEnd,
    					NULL
                    };
                    fade->setCallback(&callback, sprite);
                    sprite->runAction(fade);
                    delete obj;
                    break;
                }
                case TYPE_ENEMY_BULLET: {
                    EnemyBulletBase *enemyBullet = (EnemyBulletBase *)obj;
                    enemyBullet->destroyMyself();
                    break;
                }
                case TYPE_INTRO:
                case TYPE_BOX:{
                    world->DestroyBody(body);
                    wyNode *sprite = (wyNode *) obj->container;
                    
                    wyNode *parent = sprite->getParent();
                    parent->removeChildLocked(sprite, true);
                    delete obj;
                    break;
                }
                case TYPE_BONUS: {
                    Bonus *bonus = (Bonus *)obj;
                    bonus->destroyBonus();
                    break;
                }
                case TYPE_ENEMY: {
                    Enemy *enemy = (Enemy *)obj;
                    enemy->destroyMyself();
                    break;
                }
                case TYPE_ENDPOINT: {
                    world->DestroyBody(body);
                    break;
                }
                case TYPE_BIG_FIRE: {
                    world->DestroyBody(body);
                    delete obj;
                    break;
                }
                case TYPE_BLOOD: {
                    Blood *blood = (Blood *)obj;
                    blood->destroyMe();
                    break;
                }
                case TYPE_BODY_PIECE: {
                    Flesh *flesh = (Flesh *)obj;
                    flesh->destroyMe();
                    break;
                }
                case TYPE_BOMB:
                    obj->destroyMyself();
                    break;
                case TYPE_WATER_BALL:
                case TYPE_LIGHTING: {
                    wyNode *parent = obj->container->getParent();
                    parent->removeChildLocked(obj->container, true);
                    delete obj;
                    world->DestroyBody(body);
                    break;
                }
                case TYPE_GROUND: {
                    delete obj;
                    world->DestroyBody(body);
                    break;
                }
                case TYPE_BODY_SENSOR: {
                    delete obj;
                    world->DestroyBody(body);
                    
                    if (!sGlobal->isOnGround) {
                        if (!sGlobal->mainGameLayer->hero->isDBJumpAnimating) {
                            sGlobal->mainGameLayer->hero->playHeroAnimation(SPX_HERO_R_LEG_FALL);
                        } else {
//                            LOGE("is dbjumping, no need to play fall animation...");
                        }
                    } else {
//                        LOGE("is on ground, no need to play fall animation...");
                    }
                    sGlobal->mainGameLayer->hero->bodySensor = NULL;
                    break;
                }
                case TYPE_GROUND_SENSOR: {
                    delete obj;
                    world->DestroyBody(body);
                    
                    break;
                }
                case TYPE_HERO_HURT: {
                    delete obj;
                    world->DestroyBody(body);
                    
                    break;
                }
                case TYPE_MAP_DECO: {
//                    LOGE("bbb...");
                    Deco *deco = (Deco *)obj;
                    deco->destroyMe();
                    break;
                }
                case TYPE_TRAP: {
                    TrapBase *trap = (TrapBase *)obj;
                    trap->destroyMyself();
                    break;
                }
                default:
                    break;
            }
            
        }
    }
    toDestroy.clear();
    
//    if (needDestroyMoto) {
//        destroyMotoFixture();
//        needDestroyMoto = false;
//    }
//    
//    if (needDestroyShield) {
//        destroyShieldFixture();
//        needDestroyShield = false;
//    }
}

void MainGameLayer::dealPhysics(float dt) {
    if (needSwitchSeason || hasSwitchedSeason) {
        return;
    }
    
    b2World* world = m_box2d->getWorld();
    
    for(b2Body* body = world->GetBodyList(); body != NULL; body = body->GetNext()) {
        BaseObject *obj = (BaseObject*)body->GetUserData();
//        LOGE("objtype:%d", obj->obj_type);
//        if (body->IsActive() && !body->IsAwake()) {
//            continue;
//        }
        
        switch (obj->obj_type) {
            case TYPE_MAO: {
                obj->update(dt);
                break;
            }
            case TYPE_BULLET: {
                obj->update(dt);
                break;
            }
            case TYPE_DABIAO: {
//                wyNode *sprite = obj->container;
                if ((body->GetPosition().y < 0) || (m_box2d->meter2Pixel(body->GetPosition().x - hero->body->GetPosition().x - hero->halfBodyWidthInMeter*2) + sGlobal->hero_offset_left > wyDevice::winWidth)) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
                        toDestroy.push_back(body);
                    }
                    continue;
                }
                obj->update(dt);
                break;
            }
            case TYPE_MISSLE: {
//                wyNode *sprite = obj->container;
                if ((body->GetPosition().y < 0) || (m_box2d->meter2Pixel(body->GetPosition().x - hero->body->GetPosition().x - hero->halfBodyWidthInMeter*2) + sGlobal->hero_offset_left > wyDevice::winWidth)) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
                        toDestroy.push_back(body);
                    }
                    continue;
                }
                obj->update(dt);
                break;
            }
            case TYPE_LIUDAN: {
//                wyNode *sprite = obj->container;
                if ((body->GetPosition().y < 0) || (m_box2d->meter2Pixel(body->GetPosition().x - hero->body->GetPosition().x - hero->halfBodyWidthInMeter*2) + sGlobal->hero_offset_left > wyDevice::winWidth)) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
                        toDestroy.push_back(body);
                    }
                    continue;
                }
                obj->update(dt);
                break;
            }
            case TYPE_ENEMY: {
                float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x) - obj->offsetX;
                if (hero->needDestroy && ((distance > sGlobal->hero_offset_left + DP(200)*sGlobal->scaleX) || (body->GetPosition().y < m_box2d->pixel2Meter(-DP(300)*sGlobal->scaleY)))) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
                        toDestroy.push_back(body);
                    }
                }
                
                if (obj->isDestroied) {
                    continue;
                }
                
                obj->distance = -distance;
                if (-distance < wyDevice::winWidth + DP(200)*sGlobal->scaleX) {//sGlobal->virtualWinWidth
                    obj->update(dt);
                }
                break;
            }
            case TYPE_ENEMY_BULLET: {
                float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x);
//                LOGE("%d, %s, %f/%f, %f, %f",obj->sub_type, hero->needDestroy ? "true" : "false", body->GetPosition().y, -m_box2d->pixel2Meter(wyDevice::winHeight), distance, m_box2d->pixel2Meter(wyDevice::winHeight*2));
                if (obj->sub_type!=-999 && hero->needDestroy && ((body->GetPosition().y < -m_box2d->pixel2Meter(wyDevice::winHeight)) || (distance > wyDevice::winWidth) || (body->GetPosition().y > m_box2d->pixel2Meter(wyDevice::winHeight*2)))) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
//                        LOGE("destroy obj123:%s", ((wyNode*)body->GetUserData())->getClassName());
                        toDestroy.push_back(body);
                    }
                    continue;
                }
                
                obj->update(dt);
                break;
            }
            case TYPE_BOMB: {
//                wyNode *sprite = obj->container;
                float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x);
                if (hero->needDestroy && ((body->GetPosition().y < 0) || (distance > wyDevice::winWidth))) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
                        toDestroy.push_back(body);
                    }
                    continue;
                }
                
//                sprite->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
                obj->update(dt);
                break;
            }
            case TYPE_BODY_PIECE: {
                if (obj->isDestroied) {
                    continue;
                }
                
                float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x)-DP(100)*sGlobal->scaleX;
                if ((body->GetPosition().y < 0) && (obj->container->isVisible())) {
                    obj->container->setVisible(false);
                    if(std::find(toDisable.begin(), toDisable.end(),body) == toDisable.end()) {
//                        LOGE("destroy obj123:%s", ((wyNode*)body->GetUserData())->getClassName());
                        toDisable.push_back(body);
                    }
                    continue;
                }
                if (hero->needDestroy && (distance > sGlobal->hero_offset_left*3)) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
                        toDestroy.push_back(body);
                    }
                    continue;
                }
                obj->update(dt);
                break;
            }
            case TYPE_BLOOD: {
                float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x)-DP(100)*sGlobal->scaleX;
                if ((body->GetPosition().y < 0) || (distance > sGlobal->hero_offset_left)) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
//                        LOGE("destroy obj123:%s", ((wyNode*)body->GetUserData())->getClassName());
                        toDestroy.push_back(body);
                    }
                    continue;
                }
                obj->update(dt);
                break;
            }
            case TYPE_SHOCK_WAVE: {
                float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x);
                if (distance > wyDevice::winWidth) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
                        toDestroy.push_back(body);
                    }
                    continue;
                } else {
                    obj->spxSprite->tick(dt);
                    obj->update(dt);
                }
                break;
            }
            case TYPE_CHONGZHUANG: {
                float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x);
                if (distance > sGlobal->hero_offset_left*2) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
                        toDestroy.push_back(body);
                    }
                    continue;
                } else {
                    obj->spxSprite->tick(dt);
                    obj->update(dt);
                }
                break;
            }
            case TYPE_LIGHTING: {
                float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x);
                if (distance > wyDevice::winWidth) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
                        toDestroy.push_back(body);
                    }
                    continue;
                } else {
                    if (obj->sub_type != 0) {
                        obj->spxSprite->tick(dt);
                    }
                }
                break;
            }
            case TYPE_WATER_BALL: {
                float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x);
                if (distance > wyDevice::winWidth) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
                        toDestroy.push_back(body);
                    }
                    continue;
                } else {
                    obj->spxSprite->tick(dt);
                    obj->update(dt);
                }
                break;
            }
            case TYPE_BARRICADE:
            case TYPE_BOX:
            case TYPE_ENDPOINT:
            case TYPE_TRAP:
            case TYPE_CHECKPOINT:
            case TYPE_NPC:
            case TYPE_CANNON:
            case TYPE_BONUS: {
                if (obj->isDestroied) {
                    break;
                }
                
                float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x);
                if (-distance < wyDevice::winWidth + DP(200)*sGlobal->scaleX) {
                    obj->update(dt);
                }
                if (hero->needDestroy && (distance > sGlobal->hero_offset_left*2)) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
                        toDestroy.push_back(body);
                    }
                }
                break;
            }
            case TYPE_GROUND: {
//                if (!isEndless && sGlobal->index != 104) {
//                    float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x) - obj->widthInPixels;
//                    if (distance > sGlobal->hero_offset_left*2) {
//                        if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
//                            toDestroy.push_back(body);
//                        }
//                    }
//                }
                break;
            }
            case TYPE_MAP_DECO: {
                float distance = m_box2d->meter2Pixel(hero->body->GetPosition().x - obj->body->GetPosition().x);
                obj->distance = -distance;
                if (-distance < wyDevice::winWidth + DP(200)*sGlobal->scaleX) {
                    obj->update(dt);
                }
//                LOGE("isEndless:%s, hero->needDestroy:%s, distance:%f, dist:%f", isEndless ? "true" : "false", hero->needDestroy?"true" : "false", distance, sGlobal->hero_offset_left*3);
                if (!isEndless && hero->needDestroy && (distance > sGlobal->hero_offset_left*3)) {
                    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
//                        LOGE("aaa");
                        toDestroy.push_back(body);
                    }
                }
                break;
            }
            case TYPE_GROUND_SENSOR: {
                obj->update(dt);
                break;
            }
            default:
                break;
        }
    }
}

void MainGameLayer::addToDestroy(b2Body *body) {
    if(std::find(toDestroy.begin(), toDestroy.end(),body) == toDestroy.end()) {
//        LOGE("destroy obj:%s", ((wyNode*)body->GetUserData())->getClassName());
        toDestroy.push_back(body);
    }
}

void MainGameLayer::addToDisable(b2Body *body) {
    if(std::find(toDisable.begin(), toDisable.end(),body) == toDisable.end()) {
//        if(std::find(toDestroy.begin(), toDestroy.end(), body) != toDestroy.end()) {
//            return;
//        }
        toDisable.push_back(body);
    }
}

void MainGameLayer::addToEnable(b2Body *body) {
    if(std::find(toEnable.begin(), toEnable.end(),body) == toEnable.end()) {
//        if(std::find(toDestroy.begin(), toDestroy.end(), body) != toDestroy.end()) {
//            return;
//        }
        toEnable.push_back(body);
    }
}

void MainGameLayer::addToRecreateFixture(BaseObject *obj) {
    if(std::find(toRecreateFixture.begin(), toRecreateFixture.end(),obj) == toRecreateFixture.end()) {
//        if(std::find(toDestroy.begin(), toDestroy.end(), obj->body) != toDestroy.end()) {
//            return;
//        }
        obj->isRecreatingFixture = true;
        toRecreateFixture.push_back(obj);
    }
}

void MainGameLayer::addToDestroyFixture(b2Fixture *fixture) {
    if(std::find(toDestroyFixture.begin(), toDestroyFixture.end(),fixture) == toDestroyFixture.end()) {
        toDestroyFixture.push_back(fixture);
    }
}

void MainGameLayer::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    int tagA = ((BaseObject*)contact->GetFixtureA()->GetBody()->GetUserData())->obj_type;
    int tagB = ((BaseObject*)contact->GetFixtureB()->GetBody()->GetUserData())->obj_type;
    
//    LOGE("tagA:%d, tagB:%d", tagA, tagB);
    
    if ((tagA == TYPE_MAO && tagB == TYPE_ENEMY) || (tagB == TYPE_MAO && tagA == TYPE_ENEMY)) {
        contact->SetEnabled(false);
        return;
    }
    
//    if ((tagA == TYPE_MAO && tagB == TYPE_ENEMY_BULLET) || (tagB == TYPE_MAO && tagA == TYPE_ENEMY_BULLET)) {
//        contact->SetEnabled(false);
//        return;
//    }
    
//    if ((tagA == TYPE_MAO && tagB == TYPE_BOMB) || (tagB == TYPE_MAO && tagA == TYPE_BOMB)) {
//        contact->SetEnabled(false);
//        return;
//    }
//    if ((tagA == TYPE_MAO && tagB == TYPE_BODY_PIECE) || (tagB == TYPE_MAO && tagA == TYPE_BODY_PIECE)) {
//        contact->SetEnabled(false);
//        return;
//    }
//    if ((tagA == TYPE_MAO && tagB == TYPE_BLOOD) || (tagB == TYPE_MAO && tagA == TYPE_BLOOD)) {
//        contact->SetEnabled(false);
//        return;
//    }
//    if ((tagA == TYPE_ENEMY && tagB == TYPE_ENEMY) || (tagB == TYPE_ENEMY && tagA == TYPE_ENEMY)) {
//        contact->SetEnabled(false);
//        return;
//    }
//    if ((tagA == TYPE_ENEMY_BULLET && tagB == TYPE_ENEMY) || (tagB == TYPE_ENEMY_BULLET && tagA == TYPE_ENEMY)) {
//        contact->SetEnabled(false);
//        return;
//    }
//    if ((tagA == TYPE_ENEMY && tagB == TYPE_BOMB) || (tagB == TYPE_ENEMY && tagA == TYPE_BOMB)) {
//        contact->SetEnabled(false);
//        return;
//    }
//    if ((tagA == TYPE_ENEMY && tagB == TYPE_BODY_PIECE) || (tagB == TYPE_ENEMY && tagA == TYPE_BODY_PIECE)) {
//        contact->SetEnabled(false);
//        return;
//    }
//    if ((tagA == TYPE_ENEMY && tagB == TYPE_BLOOD) || (tagB == TYPE_ENEMY && tagA == TYPE_BLOOD)) {
//        contact->SetEnabled(false);
//        return;
//    }
    if ((tagA == TYPE_MAO && tagB == TYPE_BONUS) || (tagB == TYPE_MAO && tagA == TYPE_BONUS)) {
        contact->SetEnabled(false);
        return;
    }
    if ((tagA == TYPE_MAO && tagB == TYPE_BOMB) || (tagB == TYPE_MAO && tagA == TYPE_BOMB)) {
        contact->SetEnabled(false);
        return;
    }
//    if ((tagA == TYPE_ENEMY && tagB == TYPE_BONUS) || (tagB == TYPE_ENEMY && tagA == TYPE_BONUS)) {
//        contact->SetEnabled(false);
//        return;
//    }
//    if (tagA == TYPE_BONUS && tagB == TYPE_BONUS) {
//        contact->SetEnabled(false);
//        return;
//    }
    
    b2EdgeShape* edge;
    
    b2Vec2 position = hero->body->GetPosition();
    float top = 0;
    if (tagA == TYPE_MAO && tagB == TYPE_GROUND) {
//        if (hero->body->GetLinearVelocity().y < 0) {
//            return;
//        }
        top = fixtureB->GetBody()->GetPosition().y;
        edge = (b2EdgeShape*)fixtureB->GetShape();
//        LOGE("%f,%f,%f,%f", position.y, top, hero->halfBodyHeightInMeter,hero->ofyInMeter);
        if (edge->m_vertex2.x - edge->m_vertex1.x > 0) {
            if(position.y + hero->ofyInMeter < top + hero->halfBodyHeightInMeter - DP(3.0f * b2_linearSlop)) { //  - DP(3.0f * b2_linearSlop)
                contact->SetEnabled(false);
                hero->isDisablingContact = true;
                hero->shadow->setVisible(false);
//                hero->contactNum++;
//                _contactListener->EndContact(contact);
//                sGlobal->jumpTime = sGlobal->jumpTime1;
//                LOGE("bbb:%f", hero->body->GetLinearVelocity().y);
//                LOGE("%f,%f,%f,%f", position.y, top, hero->halfBodyHeightInMeter,hero->ofyInMeter);
            } else {
                if (hero->isDisablingContact && hero->body->GetLinearVelocity().y <= 0) {
                    hero->isDisablingContact = false;
//                    LOGE("ccc:%f", hero->body->GetLinearVelocity().y);
//                    if (hero->body->GetLinearVelocity().y <= 0) {
                        hero->doContactLogic();
//                    }
                }
            }
        }
    } else {
        if (tagB == TYPE_MAO && tagA == TYPE_GROUND) {
//            if (hero->body->GetLinearVelocity().y < 0) {
//                return;
//            }
            top = fixtureA->GetBody()->GetPosition().y;
            edge = (b2EdgeShape*)fixtureA->GetShape();
//            LOGE("%f,%f,%f,%f", position.y, top, hero->halfBodyHeightInMeter,hero->ofyInMeter);
            if (edge->m_vertex2.x - edge->m_vertex1.x > 0) {
                if(position.y + hero->ofyInMeter < top + hero->halfBodyHeightInMeter - DP(3.0f * b2_linearSlop)) { //  - DP(3.0f * b2_linearSlop)
//                    LOGE("bodyHeight:%f, top:%f, posy:%f, slop:%f", hero->halfBodyHeightInMeter, top, position.y, - DP(3.0f * b2_linearSlop));
                    contact->SetEnabled(false);
                    hero->isDisablingContact = true;
                    hero->shadow->setVisible(false);
//                    hero->contactNum++;
//                    _contactListener->EndContact(contact);
//                    sGlobal->jumpTime = sGlobal->jumpTime1;
//                    LOGE("bbb:%f", hero->body->GetLinearVelocity().y);
//                    LOGE("%f,%f,%f,%f", position.y, top, hero->halfBodyHeightInMeter,hero->ofyInMeter);
                } else {
                    if (hero->isDisablingContact && hero->body->GetLinearVelocity().y <= 0) {
                        hero->isDisablingContact = false;
//                        LOGE("ccc:%f", hero->body->GetLinearVelocity().y);
//                        if (hero->body->GetLinearVelocity().y <= 0) {
                            hero->doContactLogic();
//                        }
                    }
                }
            }
        }
    }
    
    if (tagA == TYPE_ENEMY && tagB == TYPE_GROUND) {
        float top = fixtureB->GetBody()->GetPosition().y;
        Enemy *enemy = (Enemy *)fixtureA->GetBody()->GetUserData();
        if (enemy->e_id == ENEMY_BOSS1 || enemy->e_id == ENEMY_BOSS2 || enemy->e_id == ENEMY_BOSS3) {
            return;
        }
        
        b2Vec2 position = enemy->body->GetPosition();
        edge = (b2EdgeShape*)fixtureB->GetShape();
//        LOGE("%f,%f,%f,%f", position.y, top, enemy->halfBodyHeightInMeter,enemy->ofyInMeter);
        if (edge->m_vertex2.x - edge->m_vertex1.x > 0) {
            if(position.y+enemy->ofyInMeter < top + enemy->halfBodyHeightInMeter - DP(3.0f * b2_linearSlop)) { //  - DP(3.0f * b2_linearSlop)
                contact->SetEnabled(false);
//                enemy->contactNum++;
                _contactListener->EndContact(contact);
//				LOGE("aaaa");
            }
        }
    } else {
        if (tagB == TYPE_ENEMY && tagA == TYPE_GROUND) {
            float top = fixtureA->GetBody()->GetPosition().y;
            Enemy *enemy = (Enemy *)fixtureB->GetBody()->GetUserData();
            if (enemy->e_id == ENEMY_BOSS1 || enemy->e_id == ENEMY_BOSS2 || enemy->e_id == ENEMY_BOSS3) {
                return;
            }
            
            b2Vec2 position = enemy->body->GetPosition();
            edge = (b2EdgeShape*)fixtureA->GetShape();
//            LOGE("%f,%f,%f,%f", position.y, top, enemy->halfBodyHeightInMeter,enemy->ofyInMeter);
            if (edge->m_vertex2.x - edge->m_vertex1.x > 0) {
                if(position.y+enemy->ofyInMeter < top + enemy->halfBodyHeightInMeter - DP(3.0f * b2_linearSlop)) { //  - DP(3.0f * b2_linearSlop)
                    contact->SetEnabled(false);
//                    enemy->contactNum++;
                    _contactListener->EndContact(contact);
//					LOGE("aaaa");
                }
            }
        }
    }
    
    
    
    
//    if (tagA == TYPE_BODY_PIECE && tagB == TYPE_GROUND) {
//        Flesh *flesh = (Flesh *)fixtureA->GetBody()->GetUserData();
//
//        if (flesh->body->GetLinearVelocity().y > 0) {
//            contact->SetEnabled(false);
//        }
//    } else {
//        if (tagB == TYPE_BODY_PIECE && tagA == TYPE_GROUND) {
//            Flesh *flesh = (Flesh *)fixtureB->GetBody()->GetUserData();
//            
//            if (flesh->body->GetLinearVelocity().y > 0) {
//                contact->SetEnabled(false);
//            }
//        }
//    }
}

void MainGameLayer::handleEnemyDaBiaoCollision(b2Body *enemyBody, b2Body *bulletBody) {    
    Enemy *enemy = (Enemy *)enemyBody->GetUserData();
    if (enemy->isDead || enemy->e_id == ENEMY_XUANKONGBIAO)
        return;

    enemy->dealHitByDaBiao(bulletBody->GetPosition().y, bulletBody);
}

void MainGameLayer::handleEnemyLiudanCollision(b2Body *enemyBody, b2Body *bulletBody) {
    Enemy *enemy = (Enemy *)enemyBody->GetUserData();
    if (enemy->isDead || enemy->e_id == ENEMY_XUANKONGBIAO)
        return;
    
    enemy->dealHitByLiudan(bulletBody);
    
//    Missle *missle = (Missle *) bulletBody;
//    missle->handleCollision(enemyBody);
}

void MainGameLayer::handleEnemyBulletCollision(b2Body *enemyBody, b2Body *bulletBody) {
//    if(std::find(toDestroy.begin(), toDestroy.end(),bulletBody) == toDestroy.end()) {
//        toDestroy.push_back(bulletBody);
//    }
    
    Enemy *enemy = (Enemy *)enemyBody->GetUserData();

    if (enemy->isDead) {
//        LOGE("y");
        return;
    }
//    if (enemy->hp < 0) {
//        return;
//    }
//
//    if (enemy->hp - sGlobal->weapon_atk <= 0) {
//        enemy->hp -= sGlobal->weapon_atk;
//        enemy->dead();
//    } else {
//        enemy->hp -= sGlobal->weapon_atk;
//        enemy->hurt(bulletBody->GetPosition().y);
//    }
    //LOGE("hit by dart");
    enemy->dealHitByDart(bulletBody->GetPosition().y, bulletBody);
}

void MainGameLayer::handleEnemyBigFireCollision(b2Body *enemyBody, b2Body *bulletBody) {
    Enemy *enemy = (Enemy *)enemyBody->GetUserData();
    if (enemy->isDead)
        return;
    enemy->dealHitByLiudan(bulletBody);
}

void MainGameLayer::handleBoxBulletCollision(b2Body *boxBody, b2Body *bulletBody) {
    if(std::find(toDestroy.begin(), toDestroy.end(),bulletBody) == toDestroy.end()) {
        toDestroy.push_back(bulletBody);
    }
    Box *box = (Box *)boxBody->GetUserData();
    box->dead();
}

void MainGameLayer::shake() {
    wyIntervalAction* a = wyShake::make(1.0f, 20);
    sGlobal->hudLayer->runAction(a);
}

void MainGameLayer::reset() {
    delete _contactListener;
	_contactListener = NULL;
    
//    toDestroy.clear();
    
    b2World* world = m_box2d->getWorld();
    for(b2Body* body = world->GetBodyList(); body != NULL; body = body->GetNext()) {
        BaseObject *obj = (BaseObject*)body->GetUserData();
        if (obj != NULL && obj->obj_type != TYPE_GROUND && obj->obj_type != TYPE_WORLD_BOUND && obj->obj_type != TYPE_MAO && obj->obj_type != TYPE_ENDPOINT && obj->obj_type != TYPE_CHECKPOINT) {
            WYDELETE(obj);
            addToDestroy(body);
        }
    }
    
    std::vector<b2Body *>::iterator pos2;
    for(pos2 = toDestroy.begin(); pos2 != toDestroy.end(); ++pos2) { 
        b2Body *body = *pos2; 
        world->DestroyBody(body);
    }
    
    toDestroy.clear();
    toDisable.clear();
    toEnable.clear();
    toRecreateFixture.clear();
    
    if (!isEndless && !isRealEndless) {
        delete[] m_TileMapArray;
        delete[] m_FrontArray;
    }
    
    _contactListener = new CustomContactListener();
    world->SetContactListener(_contactListener);
}

void MainGameLayer::summonBoss() {
//    LOGE("summon boss:%d", sGlobal->currentBossId);
    int ofy = 0;
    switch (sGlobal->currentSeasonId) {
        case 1:
            ofy = DP(190)*sGlobal->scaleY;
            break;
        case 2:
            ofy = DP(215)*sGlobal->scaleY;
            break;
        case 3:
            ofy = DP(215)*sGlobal->scaleY;
            break;
        default:
            break;
    }
//    sGlobal->currentBossId = 2;
    switch (sGlobal->currentBossId) {
        case 1:
            Boss1::make(hero->container->getPositionX() + wyDevice::winWidth*1.5f, ofy);
            sGlobal->hudLayer->initBossHpBar(0);
            break;
        case 2:
            Boss2::make(hero->container->getPositionX() + wyDevice::winWidth*1.5f, ofy);
            sGlobal->hudLayer->initBossHpBar(1);
            break;
        case 3:
            Boss3::make(hero->container->getPositionX() + wyDevice::winWidth*1.5f, ofy);
            sGlobal->hudLayer->initBossHpBar(2);
            break;
        default:
            break;
    }
}

void MainGameLayer::draw() {
//    if (!drawDebug)
//        return;
//    
//    // draw bezier curve so we can see the node is follow the track
//    glColor4f(0, 1, 0, 1);
////    wyDrawBezier(m_config, 30);
//    wyDrawBezier(m_config1, 30);
//    
//    // draw bezier start, end, and control points
//    glColor4f(1, 0, 0, 1);
//    glPointSize(5);
////    wyDrawPoint(m_config.startX, m_config.startY);
////    wyDrawPoint(m_config.cp1X, m_config.cp1Y);
////    wyDrawPoint(m_config.cp2X, m_config.cp2Y);
////    wyDrawPoint(m_config.endX, m_config.endY);
//    
//    wyDrawPoint(m_config1.startX, m_config1.startY);
//    wyDrawPoint(m_config1.cp1X, m_config1.cp1Y);
//    wyDrawPoint(m_config1.cp2X, m_config1.cp2Y);
//    wyDrawPoint(m_config1.endX, m_config1.endY);
//    
//    glColor4f(1, 1, 1, 1);
}

//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "EnemyFactory.h"
#include "Enemy.h"
#include "Global.h"
#include "SpittleZombie.h"
#include "MaleZombie.h"
#include "Devourer.h"
#include "FatZombie.h"
#include "Boss1.h"
#include "GhostBaby.h"
#include "MuscleZombie1.h"
#include "Boss2.h"
#include "FemaleZombie.h"
#include "Doctor1.h"
#include "Doctor2.h"
#include "MuscleZombie.h"
#include "WolfZombie.h"
#include "Boss3.h"
#include "FlyZombie1.h"
#include "XuanKongBiao.h"
#include "FlyZombie2.h"
#include "FlyZombie3.h"

DECLARE_SINGLETON_MEMBER(EnemyFactory);

EnemyFactory::EnemyFactory(){
    
}

EnemyFactory::~EnemyFactory() {
    
}

void EnemyFactory::createEnemies(wyTMXObjectGroup* enemyGroup) {
    if (enemyGroup == NULL) {
        return;
    }
    
    for (int i = 0; i < enemyGroup->getObjectCount(); i++) {
        wyTMXObject* obj = enemyGroup->getObjectAt(i);
		if (obj == NULL)
			continue;
        float x = atof(enemyGroup->getObjectProperty(obj, "x")) * sGlobal->scaleX + sGlobal->mapOffsetX;
        float y = sGlobal->tileMapHeightInPixels - atof(enemyGroup->getObjectProperty(obj, "y")) * sGlobal->scaleY;
        const char *type = enemyGroup->getObjectProperty(obj, "type");
        
        if (strcmp(type, "summonPoint") == 0) {
            int prob = rand()%100;
            if (prob < sGlobal->calcAppearChance()) {
                summonEnemyInPoint(type, x, y + sGlobal->tileMapOffsetY, enemyGroup, obj);
            }
        } else {
            if (sGlobal->index == -1)
                continue;
            
            Enemy *enemy = createEnemyByType(type, x, y + sGlobal->tileMapOffsetY, enemyGroup, obj);
            if (enemy != NULL) {
                sGlobal->total_enemies += 1;
                
                if (sGlobal->index == 0) {
                    enemy->canWander = false;
                    enemy->gold = 0;
                }
            }
        }
    }
}

void EnemyFactory::createEnemiesWithExtra(wyTMXObjectGroup* enemyGroup, int startNo, float mapOffsetX, bool isAlt) {
//    LOGE("enemyStartNo:%d", startNo);
    if (enemyGroup == NULL) {
        return;
    }
    
    for (int i = 0; i < enemyGroup->getObjectCount(); i++) {
        wyTMXObject* obj = enemyGroup->getObjectAt(i);
		if (obj == NULL)
			continue;
        float x = atof(enemyGroup->getObjectProperty(obj, "x")) * sGlobal->scaleX + mapOffsetX;
        float y = sGlobal->tileMapHeightInPixels - atof(enemyGroup->getObjectProperty(obj, "y")) * sGlobal->scaleY;
        const char *type = enemyGroup->getObjectProperty(obj, "type");
        
        if (strcmp(type, "summonPoint") == 0) {
            int prob = rand()%100;
            if (prob < sGlobal->calcAppearChance()) {
                summonEnemyInPoint(type, x, y + sGlobal->tileMapOffsetY, enemyGroup, obj);
            }
        } else {
            if (sGlobal->index == -1)
                continue;
            
            Enemy *enemy = createEnemyByType(type, x, y + sGlobal->tileMapOffsetY, enemyGroup, obj);
            if (enemy != NULL) {
                sGlobal->total_enemies += 1;
                
                if (sGlobal->index == 0) {
                    enemy->canWander = false;
                    enemy->gold = 0;
                }
            } else {
                continue;
            }
            
//            LOGE("count:%d", sGlobal->mainGameLayer->mapEnemyPlaceArray[startNo+i]);
            for (int j = 0; j < sGlobal->mainGameLayer->mapEnemyPlaceArray[startNo+i]; j++) {
                float ofx = randRange(40, 100)*sGlobal->scaleX;
                if (rand()%2) {
                    ofx *= -1;
                }
                int seed = rand()%sGlobal->mainGameLayer->mapEnemyCount;
                int targetEId = 0;
                for (int k = 0; k < 100; k++) {
                    if (seed < sGlobal->mainGameLayer->enemySDFArray[k]) {
                        targetEId = k;
                        break;
                    }
                }

//                LOGE("targetEId:%d", targetEId);
                summonEnemyByEnemyPoint(enemy, x+ofx, y + sGlobal->tileMapOffsetY, enemyGroup, obj, targetEId);
            }
            
            if (isAlt && sGlobal->currentSModeDifficulty == DIFFICULTY_NORMAL) {
                enemy->destroyMyselfWhenNotInit();
            }
        }
    }
}

Enemy* EnemyFactory::createEnemyByType(const char *type, float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj) {
    LOGE("create %s at: %f, %f", type, x, y);
    
    if (type != NULL && strcmp(type, "mz") == 0) {
        return MaleZombie::make(x, y, SPX_RABBIT_SWORD_MAN_0, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "mz") == 0) {
        return MaleZombie::make(x, y, SPX_RABBIT_SWORD_MAN_1, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "sz") == 0) {
        return SpittleZombie::make(x, y, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "fz1") == 0) {
        return FlyZombie1::make(x, y, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "dv") == 0) {
        return Devourer::make(x, y, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "fatz") == 0) {
        return FatZombie::make(0, x, y, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "boss1") == 0) {
        if (sGlobal->index == 104) {
            sGlobal->isBoss1Strong = true;
        }
        return Boss1::make(x, y);
    }
    else if (type != NULL && strcmp(type, "gb") == 0) {
        return GhostBaby::make(x, y, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "fmz") == 0) {
        return FemaleZombie::make(x, y, SPX_SKELETON_SPECULATOR, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "fatz") == 0) {
        return FatZombie::make(1, x, y, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "msz") == 0) {
        return MuscleZombie1::make(x, y, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "boss2") == 0) {
        return Boss2::make(x, y);
    }
    else if (type != NULL && strcmp(type, "dr") == 0) {
        return Doctor1::make(x, y, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "msz") == 0) {
        return MuscleZombie::make(x, y, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "wz") == 0) {
        return WolfZombie::make(x, y, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "boss3") == 0) {
        return Boss3::make(x, y);
    }
    else if (type != NULL && strcmp(type, "fz2") == 0) {
        return FlyZombie2::make(x, y, objectsGroup, obj);
    }
    else if (type != NULL && strcmp(type, "fz3") == 0) {
        return FlyZombie3::make(x, y, objectsGroup, obj);
    }
    else {
        return NULL;
    }
    
//    return MuscleZombie::make(x, y, objectsGroup, obj);
}

void EnemyFactory::summonEnemyInPoint(const char *type, float x, float y, wyTMXObjectGroup* enemyGroup, wyTMXObject* obj) {
    int maxX = 2;
    const char *scope = enemyGroup->getObjectProperty(obj, "scope");
    if (scope != NULL) {
        maxX = atoi(scope);
    }
    
    bool inAir = false;
    const char *fly = enemyGroup->getObjectProperty(obj, "inAir");
    if (fly != NULL) {
        inAir = true;
    }
    
    
    int range = randRange(sGlobal->COMBAT_VAR_MIN, sGlobal->COMBAT_VAR_MAX);
    int maxCombatValue;
    if (sGlobal->isInBossBattle) {
        maxCombatValue = sGlobal->currentCombatValue/2 * range / 100;
    } else {
        maxCombatValue = sGlobal->currentCombatValue * range / 100;
    }
    int seed;
    int minIndex = -1;
    
    int *prob_arry;
    int prob_arry_size;
    int size = sGlobal->prob_combat_value->size();
    prob_arry = new int[size];
    prob_arry_size = size;
    for (int i = 0; i < size; i++) {
        prob_arry[i] = sGlobal->prob_combat_value->at(i);
//        if (prob_arry[i] > maxCombatValue) {
//            minIndex = i - 1;
//        }
//        if (minIndex == -1) {
//            minIndex = 0;
//        }
//        LOGE("probarray:%d", prob_arry[i]);
    }

//    std::map< int, vector<int>* >::iterator it;
//    for(it = sGlobal->combatValueToEnemyIdMap->begin(); it != sGlobal->combatValueToEnemyIdMap->end(); ++it) {
//        LOGE("map key:%d", it->first);
//        vector<int> *probEnemyArry;
//        probEnemyArry = it->second;
//        for (int i = 0; i < probEnemyArry->size(); i++) {
//            LOGE("enemy id:%d", probEnemyArry->at(i));
//        }
//    }
    
    prevX = -1.0f;
//    LOGE("cur cb:%d, total combat value: %d", sGlobal->currentCombatValue, maxCombatValue);
    while (maxCombatValue > 0) {
        minIndex = -1;
        if (maxCombatValue >= prob_arry[prob_arry_size-1]) {
            minIndex = prob_arry_size;
        } else {
            for (int i = 0; i < prob_arry_size; i++) {
                if (prob_arry[i] > maxCombatValue) {
                    minIndex = i - 1;
                    break;
                }
            }
            if (minIndex == -1) {
                minIndex = 0;
            }
        }
        
//        LOGE("minIndex:%d", minIndex);
        if (minIndex == 0) {
            seed = prob_arry[0];
            summonEnemyInPointByCombatValue(type, x, y, enemyGroup, obj, seed, maxX, inAir);
            break;
        } else {
            int idx = rand()%(minIndex+1);
            seed = prob_arry[idx];
//            LOGE("seed:%d, idx:%d", seed, idx);
            summonEnemyInPointByCombatValue(type, x, y, enemyGroup, obj, seed, maxX, inAir);
            maxCombatValue -= seed;
        }
    }
    
    delete [] prob_arry;
}

void EnemyFactory::summonEnemyInPointByCombatValue(const char *type, float x, float y, wyTMXObjectGroup *objectsGroup, wyTMXObject *obj, int combatValue, int maxX, bool inAir) {
//    LOGE("part combat value: %d", combatValue);
    
    vector<int> *probEnemyArry;
    std::map< int, vector<int>* >::iterator itr;
    itr = sGlobal->combatValueToEnemyIdMap->find(combatValue);
    if( itr != sGlobal->combatValueToEnemyIdMap->end() ) {
        probEnemyArry = itr->second;
    } else {
        return;
    }
    int num = probEnemyArry->size();
    int enemyId = probEnemyArry->at(rand()%num);
//    LOGE("select enemy: %d", enemyId);
    
    summonEnemyInPointById(type, getNextX(x), y, objectsGroup, obj, enemyId, maxX, inAir);
}

void EnemyFactory::summonEnemyInPointById(const char *type, float x, float y, wyTMXObjectGroup *objectsGroup, wyTMXObject *obj, int enemyId, int maxX, bool inAir) {
    int xx = x;
    char cmd[50];
    float ofY;
    int yy;

    LOGE("create %d at: %f, %f", enemyId, x, y);
//    if (inAir) {
//        yy = y + DP(randRange(0, 40))*sGlobal->scaleY;
//    } else {
//        yy = y + DP(randRange(120, 160))*sGlobal->scaleY;
//    }
//    getRandomCMD(cmd, false, true, true);
////    sprintf(cmd, "n,1.0;w,0.5,0,0;");
//    FlyZombie1::make(xx, yy, NULL, NULL, cmd);
//    return;
    
    switch (enemyId) {
        case ENEMY_TUZIJIANKE: {
            ofY = getRandomCMD(cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            MaleZombie::make(xx, yy, SPX_RABBIT_SWORD_MAN_0, NULL, NULL, cmd);
            break;
        }
        case ENEMY_TUZIGONGJIANSHOUGAODI:
        case ENEMY_TUZIGONGJIANSHOU: {
            ofY = getRandomCMD(cmd, false, false);
            yy = y + DP(ofY)*sGlobal->scaleY;
            SpittleZombie::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_TUZIGONGJIANSHOUFLY: {
            if (inAir) {
                yy = y + DP(randRange(0, 40))*sGlobal->scaleY;
            } else {
                yy = y + DP(randRange(120, 160))*sGlobal->scaleY;
            }
            getRandomCMD(cmd, false, true, true);
            FlyZombie1::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_TUZILANGKE: {
            ofY = getRandomCMD(cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            MaleZombie::make(xx, yy, SPX_RABBIT_SWORD_MAN_1, NULL, NULL, cmd);
            break;
        }
        case ENEMY_TUZIFASHI: {
            ofY = getRandomCMD(cmd, false, false);
            yy = y + DP(ofY)*sGlobal->scaleY;
            Devourer::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_TUZIJIANSHENG: {
            ofY = getRandomCMD(cmd, false);
            yy = y + DP(ofY)*sGlobal->scaleY;
            FatZombie::make(0, xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_JIANSHEN_JIANKE3: {
            ofY = getRandomCMD(cmd, false);
            yy = y + DP(ofY)*sGlobal->scaleY;
            FatZombie::make(1, xx, yy, NULL, NULL, cmd);
            
            for (int i = 0; i < 3; i++) {
                char cmd1[50];
                int ofY1 = getRandomCMD(cmd1);
                int yy1 = y + DP(ofY1)*sGlobal->scaleY;
                int xx1 = getNextX(x);
                MaleZombie::make(xx1, yy1, SPX_RABBIT_SWORD_MAN_0, NULL, NULL, cmd1);
            }
            break;
        }
        case ENEMY_KULOUSHIBING: {
            ofY = getRandomCMD(cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            GhostBaby::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_KULOUSHIBINGFLY: {
            if (inAir) {
                yy = y + DP(randRange(0, 40))*sGlobal->scaleY;
            } else {
                yy = y + DP(randRange(120, 160))*sGlobal->scaleY;
            }
            getRandomCMD(cmd, false, true, true);
            FlyZombie2::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_KULOUWUSHI: {
            ofY = getRandomCMD(cmd, false);
            yy = y + DP(ofY)*sGlobal->scaleY;
            FatZombie::make(1, xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_KULOUTOULING: {
            ofY = getRandomCMD(cmd, false);
            yy = y + DP(ofY)*sGlobal->scaleY;
            MuscleZombie1::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_KULOUTOUJIZHE: {
            ofY = getRandomCMD(cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            FemaleZombie::make(xx, yy, SPX_SKELETON_SPECULATOR, NULL, NULL, cmd);
            break;
        }
        case ENEMY_KULOU3: {
            if (inAir) {
                yy = y + DP(randRange(0, 40))*sGlobal->scaleY;
            } else {
                yy = y + DP(randRange(120, 160))*sGlobal->scaleY;
            }
            getRandomCMD(cmd, false, true, true);
            FlyZombie2::make(xx, yy, NULL, NULL, cmd);
            
            for (int i = 0; i < 2; i++) {
                char cmd1[50];
                getRandomCMD(cmd1, false, true, true);
                int yy1;
                if (inAir) {
                    yy1 = y + DP(randRange(0, 40))*sGlobal->scaleY;
                } else {
                    yy1 = y + DP(randRange(120, 160))*sGlobal->scaleY;
                }
                int xx1 = getNextX(x);
                FlyZombie2::make(xx1, yy1, NULL, NULL, cmd1);
            }
            break;
        }
        case ENEMY_HONGYIWUSHI: {
            ofY = getRandomCMD(cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            MuscleZombie::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_HONGYIWUSHIFLY: {
            if (inAir) {
                yy = y + DP(randRange(0, 40))*sGlobal->scaleY;
            } else {
                yy = y + DP(randRange(120, 160))*sGlobal->scaleY;
            }
            getRandomCMD(cmd, false, true, true);
            FlyZombie3::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_HEIYIWUSHI: {
            ofY = getRandomCMD(cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            WolfZombie::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_ZHAOHUANSHI: {
            ofY = getRandomCMD(cmd, false, true);
            yy = y + DP(ofY)*sGlobal->scaleY;
            FemaleZombie::make(xx, yy, SPX_DELUYI, NULL, NULL, cmd);
            break;
        }
        case ENEMY_GOUTOUJUNSHI: {
            ofY = getRandomCMD(cmd, false, true);
            yy = y + DP(ofY)*sGlobal->scaleY;
            Doctor1::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_JIAMIANJUNSHI: {
            ofY = getRandomCMD(cmd, false, true);
            yy = y + DP(ofY)*sGlobal->scaleY;
            Doctor1::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        default:
            break;
    }
}

int EnemyFactory::getNextX(int x) {
    int xx;
//    if (prevX < 0) {
//        prevX = xx;
//    } else {
//        if (fabs(prevX - xx) < sGlobal->tileWidth) {
//            if (prevX - x > 0) {
//                xx = prevX - sGlobal->tileWidth*2;
//            } else {
//                xx = prevX + sGlobal->tileWidth*2;
//            }
//        }
//        prevX = xx;
//    }
    if (prevX < 0) {
        prevX = x - randRange(2, 3)*sGlobal->tileWidth;
        xx = prevX;
    } else {
        xx = prevX + randRange(1, 3)*sGlobal->tileWidth;
        prevX = xx;
    }
    
    return xx;
}

int EnemyFactory::getEnemyIdByType(const char *type) {
    if (type != NULL && strcmp(type, "tuzijianke") == 0) {
        return ENEMY_TUZIJIANKE;
    }
    else if (type != NULL && strcmp(type, "tuzilangke") == 0) {
        return ENEMY_TUZILANGKE;
    }
    else if (type != NULL && strcmp(type, "tuzigongjianshou") == 0) {
        return ENEMY_TUZIGONGJIANSHOU;
    }
    else if (type != NULL && strcmp(type, "tuzigongjianshoufly") == 0) {
        return ENEMY_TUZIGONGJIANSHOUFLY;
    }
    else if (type != NULL && strcmp(type, "tuzifashi") == 0) {
        return ENEMY_TUZIFASHI;
    }
    else if (type != NULL && strcmp(type, "tuzijiansheng") == 0) {
        return ENEMY_TUZIJIANSHENG;
    }
    else if (type != NULL && strcmp(type, "boss1") == 0) {
        return ENEMY_BOSS1;
    }
    else if (type != NULL && strcmp(type, "kuloushibing") == 0) {
        return ENEMY_KULOUSHIBING;
    }
    else if (type != NULL && strcmp(type, "kuloutoujizhe") == 0) {
        return ENEMY_KULOUTOUJIZHE;
    }
    else if (type != NULL && strcmp(type, "kulouwushi") == 0) {
        return ENEMY_KULOUWUSHI;
    }
    else if (type != NULL && strcmp(type, "kuloutouling") == 0) {
        return ENEMY_KULOUTOULING;
    }
    else if (type != NULL && strcmp(type, "boss2") == 0) {
        return ENEMY_BOSS2;
    }
    else if (type != NULL && strcmp(type, "zhaohuanshi") == 0) {
        return ENEMY_ZHAOHUANSHI;
    }
    else if (type != NULL && strcmp(type, "goutoujunshi") == 0) {
        return ENEMY_GOUTOUJUNSHI;
    }
    else if (type != NULL && strcmp(type, "jiamianjunshi") == 0) {
        return ENEMY_JIAMIANJUNSHI;
    }
    else if (type != NULL && strcmp(type, "hongyiwushi") == 0) {
        return ENEMY_HONGYIWUSHI;
    }
    else if (type != NULL && strcmp(type, "heiyiwushi") == 0) {
        return ENEMY_HEIYIWUSHI;
    }
    else if (type != NULL && strcmp(type, "boss3") == 0) {
        return ENEMY_BOSS3;
    }
    else if (type != NULL && strcmp(type, "kuloushibingfly") == 0) {
        return ENEMY_KULOUSHIBINGFLY;
    }
    else if (type != NULL && strcmp(type, "hongyiwushifly") == 0) {
        return ENEMY_HONGYIWUSHIFLY;
    }
    else {
        return ENEMY_TUZIJIANKE;
    }
}

//在一个enemy对象附近生成另外一个enemy
void EnemyFactory::summonEnemyByEnemyPoint(Enemy *baseEnemy, float x, float y, wyTMXObjectGroup *objectsGroup, wyTMXObject *obj, int enemyId) {
    int xx = x;
    char cmd[50];
    float ofY;
    int yy;
    
    switch (enemyId) {
        case ENEMY_TUZIJIANKE: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            MaleZombie::make(xx, yy, SPX_RABBIT_SWORD_MAN_0, NULL, NULL, cmd);
            break;
        }
        case ENEMY_TUZIGONGJIANSHOUGAODI:
        case ENEMY_TUZIGONGJIANSHOU: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            SpittleZombie::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_TUZIGONGJIANSHOUFLY: {
            ofY = DP(randRange(0, 40))*sGlobal->scaleY;
            if (rand()%2 == 0) {
                ofY *= -1;
            }
            yy = y + ofY;
            getRandomCMDInStoryMode(baseEnemy, cmd, true);
            FlyZombie1::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_TUZILANGKE: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            MaleZombie::make(xx, yy, SPX_RABBIT_SWORD_MAN_1, NULL, NULL, cmd);
            break;
        }
        case ENEMY_TUZIFASHI: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            Devourer::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_TUZIJIANSHENG: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            FatZombie::make(0, xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_KULOUSHIBING: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            GhostBaby::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_KULOUSHIBINGFLY: {
            ofY = DP(randRange(0, 40))*sGlobal->scaleY;
            if (rand()%2 == 0) {
                ofY *= -1;
            }
            yy = y + ofY;
            getRandomCMDInStoryMode(baseEnemy, cmd, true);
            FlyZombie2::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_KULOUWUSHI: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            FatZombie::make(1, xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_KULOUTOULING: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            MuscleZombie1::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_KULOUTOUJIZHE: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            FemaleZombie::make(xx, yy, SPX_SKELETON_SPECULATOR, NULL, NULL, cmd);
            break;
        }
        case ENEMY_HONGYIWUSHI: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            MuscleZombie::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_HONGYIWUSHIFLY: {
            ofY = DP(randRange(0, 40))*sGlobal->scaleY;
            if (rand()%2 == 0) {
                ofY *= -1;
            }
            yy = y + ofY;
            getRandomCMDInStoryMode(baseEnemy, cmd, true);
            FlyZombie3::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_HEIYIWUSHI: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            WolfZombie::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_ZHAOHUANSHI: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            FemaleZombie::make(xx, yy, SPX_DELUYI, NULL, NULL, cmd);
            break;
        }
        case ENEMY_GOUTOUJUNSHI: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            Doctor1::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        case ENEMY_JIAMIANJUNSHI: {
            ofY = getRandomCMDInStoryMode(baseEnemy, cmd);
            yy = y + DP(ofY)*sGlobal->scaleY;
            Doctor1::make(xx, yy, NULL, NULL, cmd);
            break;
        }
        default:
            break;
    }
}

#pragma once

#include "WiEngine.h"
#include "Singleton.h"
#include "Enemy.h"

class EnemyFactory : public Singleton<EnemyFactory>
{
    DECLARE_SINGLETON_CLASS(EnemyFactory);
    
public:
    EnemyFactory(void);
    ~EnemyFactory(void);

    void createEnemies(wyTMXObjectGroup* enemyGroup);
    
    void createEnemiesWithExtra(wyTMXObjectGroup* enemyGroup, int mapNo, float mapOffsetX, bool isAlt);
    
    Enemy* createEnemyByType(const char *type, float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj);
    
    wySpriteBatchNode *m_arrowBatchNode;
    
    void summonEnemyInPoint(const char *type, float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj);
    void summonEnemyInPointByCombatValue(const char *type, float x, float y, wyTMXObjectGroup *objectsGroup, wyTMXObject *obj, int combatValue, int maxX, bool inAir);
    float prevX;
    void summonEnemyInPointById(const char *type, float x, float y, wyTMXObjectGroup *objectsGroup, wyTMXObject *obj, int enemyId, int maxX, bool inAir);
    int getNextX(int x);
    
    int getEnemyIdByType(const char *type);
    
    void summonEnemyByEnemyPoint(Enemy *baseEnemy, float x, float y, wyTMXObjectGroup *objectsGroup, wyTMXObject *obj, int enemyId);
};

#define sEnemyFactory EnemyFactory::instance()

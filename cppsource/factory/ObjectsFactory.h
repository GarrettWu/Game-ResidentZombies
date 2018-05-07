//
//  MyContactListener.h
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//
#pragma once

#include "WiEngine.h"
#include "Singleton.h"

class ObjectsFactory : public Singleton<ObjectsFactory>
{
    DECLARE_SINGLETON_CLASS(ObjectsFactory);
    
public:
    ObjectsFactory(void);
    ~ObjectsFactory(void);

    void createObjects(wyTMXObjectGroup* objectsGroup);
    
    void createObjectByType(const char *type, float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj);
    
    void generateRandomCoins(const char *type, float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj);
    void generateRandomItem(const char *type, float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj);
    void generateRandomTraps(const char *type, float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj);
    
    void generateBox(float x, float y, wyTMXObjectGroup *objectsGroup, wyTMXObject *obj);
    void getBonus(int type, int x, int y);
};

#define sObjectsFactory ObjectsFactory::instance()

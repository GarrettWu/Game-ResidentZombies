//
//  MainGameLayer.h
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "WiEngine.h"

class GotoSceneRunnable : public wyRunnable {
private:
    wyScene* m_scene;
    
protected:
    GotoSceneRunnable(wyScene* s) {
        m_scene = s;
        s->retain();
    }
    
public:
    virtual ~GotoSceneRunnable() {
        m_scene->release();
    }
    
    static GotoSceneRunnable* make(wyScene* s) {
        GotoSceneRunnable* r = new GotoSceneRunnable(s);
        return (GotoSceneRunnable*)r->autoRelease();
    }
    
    virtual void run() {
        wyDirector::getInstance()->replaceScene(m_scene);
    }
};

static const int ENEMY_0_TIP[7] = {
    0,1,
    6,7,9,
    12,13,
};

static const int ENEMY_1_TIP[8] = {
    0,1,
    7,8,
    14,15,
    20, 21,
};

static const int ENEMY_2_TIP[7] = {
    0,1,
    8,9,11,12,
    15,
};

class LoadingScene : public wyScene {
public:
    LoadingScene(int sceneId, int mapIndex);
    virtual ~LoadingScene();

    bool m_busying;
    
    virtual void onEnterTransitionDidFinish();
    void loadingThreadEntry(wyTargetSelector* sel);
    wyLabel* m_hintLabel;
    
    void updateLoadHintLabel(int index);
    
    int sceneId, mapIndex;
    
    wySPXSprite *avatar;
    void updateLogic(wyTargetSelector* ts);
    
    void gotoNextScene(wyTargetSelector* ts, void *data);
    
    char *loadingResId;
    
    wySPXSprite *loading;
    
    void loadEffects(int sceneId);
};


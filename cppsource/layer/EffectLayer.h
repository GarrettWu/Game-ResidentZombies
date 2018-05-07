//
//  MainGameLayer.h
//  NinjaCat_ios
//
//  Created by  on 12-2-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "WiEngine.h"

class EffectLayer : public wyLayer {
public:
    EffectLayer(int mapIndex);
    virtual ~EffectLayer();
    
    wySprite *hurtSprite;

    void update(wyTargetSelector* ts);
    
    bool isShowingHurtAnim;
    void showHurtAnim();
    
    void showEatCoinEffect(float x, float y, int gold, int type);
    
    int animIndex, animIndex1;

    void reset();
    
    bool touchesBegan(wyMotionEvent& e) {
        return true;
    }
    bool touchesEnded(wyMotionEvent& e) {
//        LOGE("effect layer onEnd...");
        return true;
    }
    bool touchesPointerBegan(wyMotionEvent& e) {
        return true;
    }
    bool touchesPointerEnded(wyMotionEvent& e) {
        return true;
    }
    bool touchesCancelled(wyMotionEvent& e) {
        return true;
    }
    bool touchesMoved(wyMotionEvent& e) {
        return true;
    }
    bool touchesPointerCancelled(wyMotionEvent& e) {
        return true;
    }
    
    void showDeadCoinEffect(float x, float y, int gold);
    void showDeadZuanShiEffect(float x, float y, int zuanshi);

	wySpriteBatchNode *hurtNode;
//    wySpriteEx *hurtNode;
    
    bool isEffecting;
    
    wyParticleSystem *emitter;
    void offsetBy(float dx, float dy);
    
    float emitterTimer, emitterInterval;
    void updateTimer(float dt);
    
    int bgIndex;
    wyNode *batContainer;
    wySPXSprite *bat1, *bat2, *bat3, *bat4, *bat5, *bat6;
    wySPXSprite *getBatSprite(float sx, float posx, float posy);
    void onBatFlyEnd(wyTargetSelector* ts);
    
    wyNode *cloudContainer;
    wySPXSprite *c1, *c2, *c3;
    wySPXSprite *getCloudSprite(float sx, float posx, float posy);
    void onCloudMoveEnd(wyTargetSelector* ts);
};



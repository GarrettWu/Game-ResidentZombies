#pragma once

#include "WiEngine.h"

class GameBackgroundLayer : public wyLayer {
public:
    GameBackgroundLayer(int mapIndex);
    virtual ~GameBackgroundLayer();
    
    void update(float dt);
    
	wyTiledSprite *tp1, *tp2;

    int bgIndex;
    
    void offsetBy(float dx, float dy);
    
    wyParticleSystem *emitter, *emitter1;
    
    void onEnter();
    
    float tp1x, tp1y, tp2x, tp2y;
    
    
    wySPXSprite *getDecoLightSprite(float sx, float sy, float posx, float posy);
    wySPXSprite *deco_light1, *deco_light2, *deco_light3, *deco_light4;
    float leftX;
    float *lightPos;
    float maxX;
    void offsetLight(wyNode *lightNode);
    
    wyNode *eagleContainer;
    wySPXSprite *eagle, *eagle1, *eagle2, *eagle3;
    float eagleSpeed;
    float eagleOrgY;

    void initSeason1();
    void initSeason2();
    void initSeason3();
    
    void updateSeason1(float dt);
    void updateSeason2(float dt);
   
    
    void offsetSeason1(float dx, float dy);
    void offsetSeason2(float dx, float dy);
    void offsetSeason3(float dx, float dy);
    
    wyNode *waterContainer;
    
    
    wyNode *cloudContainer;
    wySPXSprite *c1, *c2, *c3;
    wySPXSprite *getCloudSprite(float sx, float posx, float posy);
    void onCloudMoveEnd(wyTargetSelector* ts);
};







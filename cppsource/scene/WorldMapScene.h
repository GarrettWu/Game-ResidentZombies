//
//  WorldMapScene.h
//  NinjaCat_ios
//
//  Created by xu songlin on 12-2-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#ifndef _WorldMapScene_h
#define _WorldMapScene_h

#include "WiEngine.h"
#include "LevelSelectLayer.h"
#include "Global.h"
#include "ShPageControl.h"

class LevelNode : public wyNode {
public:
    LevelNode(int mapIndex);
    virtual ~LevelNode();
    
    bool isLocked;
    wySpriteEx *bg;
    wySpriteEx *photo;
    wyBitmapFontLabel *levelNo;
    wySpriteEx *grade;
    int mapIndex, orgIndex;
    
    void onClick(wyTargetSelector* ts);
    
    void getGradeSprite(int mapIndex);
    
    bool isEnabled;
    float posx, posy;
    void updateNodeBySeasonId(int seasonId);
    int curSeasonId;
    wySpriteEx *lock;
    
    void setGrade(int gradeNo);
};

class WorldMapScene: public wyScene {
public:
    WorldMapScene(bool isExpand);
    virtual ~WorldMapScene();
    
    void onEnter();
    void onEnterTransitionDidFinish();

    bool keyUp(wyKeyEvent& event);

    int mapIndex;
    
    void onBackButtonClick(wyTargetSelector* ts);
    void onStartButtonClick(wyTargetSelector* ts);
    
    bool touchesBegan(wyMotionEvent& event);
    
    void onUpdateBg(wyTargetSelector* ts);
    GameBackgroundLayer *bgLayer0, *bgLayer1, *bgLayer2;
    
    wySpriteEx *highlight, *photo_mask;
    void hideHighlight();
    void showHighlight();
    int currentMapId;
    void showInfo(int mapId, float x, float y, float px, float py, LevelNode *node, bool needAnim = true);
    
    wyBitmapFontLabel *highscore, *playtime, *levelDesc;
    wyButton *startBtn, *backBtn;

    bool isTraining;
    wySPXSprite *handPointer;
    void showTraining();
    void showTraining1();
    void update1(wyTargetSelector* ts);
    
    wyNode *levelNodeContainer;
    void switchSeason(int seasonId);
    int currentSeasonId;
    bool isExpandingSeason;
    void expandSeason(int seasonId, int levelId = -1, bool needAnim = true);
    ShPageControl* pageControl;
    void onSeasonPhotoClicked(int index);
    bool *lockFlags;
    
    wyColorLayer *cLayer;
    wyNode *coverLayer;
    TouchLayer *touchLayer;
    
    wyArray *levelNodeArr;
    LevelNode *toSelectNode;
    bool needSelDelay;
    
    wyBitmapFontLabel *maptitle, *mapInfo;
    wySpriteEx *mapInfoGrade;
    void updateSeasonInfo(int seasonId);
    
    LevelNode *currentNode;
    
    void initLocks();
    int lockSelIndex;
    
    bool needUnlockS2, needUnlockS3;
    wyNode *lockContainerS2, *lockContainerS3;
    void onUnlockEnd(wyTargetSelector* ts, int data);
    void onUnlockNeedBlink(wyTargetSelector* ts, int data);
    
    void updateWorld(wyTargetSelector *ts);
};
#endif

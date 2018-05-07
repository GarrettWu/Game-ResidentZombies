#include "MainScene.h"
#include "WiEngine.h"
#include "Global.h"
#include "LoadingScene.h"
#include "QuestLayer.h"

#if ANDROID
#define FONT_NAME "DroidSans"
#elif IOS
#define FONT_NAME "Verdana"
#endif

#include "VungleVedioController.h"

#define TARGETSELECTOR(id) (wyTargetSelector*)(wyTargetSelector::make(this, SEL_i(MainScene::onTargetSelector), id))

static const int IDLE = 0;
static const int UP = 1;
static const int DOWN = 2;
static const int LEFT = 3;
static const int RIGHT = 4;
static const int SPEED = 360;

static const int UP_ANIM = 2;
static const int DOWN_ANIM = 1;
static const int LEFT_ANIM = 0;
static const int RIGHT_ANIM = 0;
static const int IDLE_ANIM = 0;

wyPoint getLocation(wyMotionEvent event) {
	wyPoint loc = wyp(event.x[0], event.y[0]);
	return loc;
}

MainScene::MainScene(int mapIndex, bool isFromCheckPoint) {
    sGlobal->mainScene = this;
    
//    this->mapIndex = mapIndex;
    
    if (mapIndex == 0) {
        prevWeapon = sGlobal->activeWeapon;
        prevRifleGrade = sGlobal->weaponUnlockArray[WEAPON_M870P].getValue();
        prevRifleAmmoNumber = sGlobal->weaponAmmoArray[WEAPON_M870P];
        prevSkill = sGlobal->activeSkill;
        
        sGlobal->activeWeapon = WEAPON_UMP;
        sGlobal->weaponUnlockArray[WEAPON_M870P].setValue(1);
        sGlobal->weaponAmmoArray[WEAPON_M870P] = 1000;
        sGlobal->activeSkill = SKILL_MISSLE;
    }
    
    if (isFromCheckPoint) {
        LOGE("isFromCheckPoint...");
    }
    this->isFromCheckPoint = isFromCheckPoint;
	sGlobal->index = mapIndex;
    this->mapIndex = mapIndex;
    
    sGlobal->hasBadge = false;
    if (!isFromCheckPoint) {
        sGlobal->prev_gold = 0;
        sGlobal->prev_gold_normal = 0;
        sGlobal->prev_kills = 0;
        sGlobal->prev_enemygold = 0;
        sGlobal->level_gold = 0;
        sGlobal->level_gold_normal = 0;
        sGlobal->level_kills = 0;
        sGlobal->total_gold = 0;
        sGlobal->total_enemies = 0;
        sGlobal->curHP = sGlobal->cloth_hp;
        sGlobal->hasReachCheckPoint = false;
        sGlobal->hasReviveFromCheckPoint = false;
        
        sGlobal->extraGold = 0;
        sGlobal->accDistance = 0;
    } else {
        sGlobal->level_gold = 0;
        sGlobal->level_gold_normal = 0;
        sGlobal->level_kills = 0;
        sGlobal->curHP = sGlobal->cloth_hp/2;
        sGlobal->hasReviveFromCheckPoint = true;
        
        sGlobal->extraGold = 0;
    }
    
    if (sGlobal->isRealEndless) {
        sGlobal->level_enemy_kill = 0;
        sGlobal->level_bosses = 0;
        sGlobal->level_zuanshi = 0;
        sGlobal->extraGold = 0;
        sGlobal->isQuestRecorded = false;
    }
    sGlobal->level_enemy_gold = 0;
    sGlobal->level_total_gold = 0;
    
    sGlobal->resetCaches();
    
    addChildLocked((wyLayer*) (new GameBackgroundLayer(mapIndex))->autoRelease(), 0, 0);
    addChildLocked((wyLayer*) (new EffectLayer(mapIndex))->autoRelease(), 3, 2);
    addChildLocked((wyLayer*) (new HudLayer(mapIndex))->autoRelease(), 4, 3);
    addChildLocked((wyLayer*) (new MainGameLayer(mapIndex, isFromCheckPoint))->autoRelease(), 2, 1);
    
    
//    if (mapIndex == 0) {
//        sGlobal->playBGM(1);
//    } else {
//        sGlobal->playBGM(mapIndex / 100);
//    }
    sGlobal->playBGM(1);
    
    setKeyEnabled(true);
    setKeyPriority(1);
    
    sceneState = SC_NORMAL;
    
    isQuiting = false;
    
    sGlobal->curAchvShowCount = 0;
    
    VungleVedioController::updatePlayTimes();
}

MainScene::~MainScene() {
    LOGE("~MainScene");
//    sGlobal->playBGM(0);
//    sGlobal->mainScene = NULL;
    if (mapIndex == 0) {
        sGlobal->activeWeapon = prevWeapon;
        sGlobal->weaponUnlockArray[WEAPON_M870P].setValue(prevRifleGrade);
        sGlobal->weaponAmmoArray[WEAPON_M870P] = prevRifleAmmoNumber;
        sGlobal->activeSkill = prevSkill;
    }
    
    sGlobal->needPauseGame = false;
}

void MainScene::quit() {
    isQuiting = true;
}

void MainScene::clearTutorialStatus() {
//    if (mapIndex == 0) {
        sGlobal->activeWeapon = prevWeapon;
        sGlobal->weaponUnlockArray[WEAPON_M870P].setValue(prevRifleGrade);
        sGlobal->weaponAmmoArray[WEAPON_M870P] = prevRifleAmmoNumber;
        sGlobal->activeSkill = prevSkill;
//    }
}

bool MainScene::keyUp(wyKeyEvent& event) {
//    if (sGlobal->effectLayer->isEffecting)
//        return true;
//    
    switch(event.keyCode) {
        case KEYCODE_BACK:
            if (sceneState == SC_NORMAL) {
                sGlobal->playEffect(se_menu);
                sGlobal->hudLayer->onPauseBtnClick(NULL);
            }
            else if (sceneState == SC_OPTION_OPENED) {
                sGlobal->optionLayer->onResumeClicked(NULL);
            }
            return true;
        default:
            break;
    }
    
    return wyScene::keyUp(event);
}

void MainScene::onEnter() {
    wyScene::onEnter();
    
    sGlobal->needPauseGame = true;
    
    if (!sGlobal->isRealEndless) {
        sGlobal->reportGameEvent(EVT_GAME_START, mapIndex, 0.0f, DIFFICULTY_STR[sGlobal->currentSModeDifficulty]);
    } else {
        sGlobal->reportGameEvent(EVT_GAME_ENDLESS_START, 0, 0.0f, DIFFICULTY_STR[sGlobal->currentEModeDifficulty]);
    }
}

void MainScene::onEnterTransitionDidFinish() {
    wyScene::onEnterTransitionDidFinish();

}

static void onTransEnded(wyAction* action, void* data) {
    wyNode *node = (wyNode *) data;
    node->getParent()->removeChildLocked(node, true);
}

void MainScene::reset(bool isFromCheckPoint) {
    isQuiting = true;
    
    sGlobal->needPauseGame = true;
    
    if (mapIndex == 0) {
        sGlobal->activeWeapon = WEAPON_UMP;
        sGlobal->weaponUnlockArray[WEAPON_M870P].setValue(1);
        sGlobal->weaponAmmoArray[WEAPON_M870P] = 1000;
        sGlobal->activeSkill = SKILL_MISSLE;
    }
    
    wyColorLayer *bg = wyColorLayer::make();
    bg->setColor(wyc3b(0, 0, 0));
    addChildLocked(bg, 100);
    
    int tempTotalGold, tempTotalEnemies;
    if (!isFromCheckPoint) {
        sGlobal->prev_gold = 0;
        sGlobal->prev_gold_normal = 0;
        sGlobal->prev_kills = 0;
        sGlobal->prev_enemygold = 0;
        sGlobal->level_gold = 0;
        sGlobal->level_gold_normal = 0;
        sGlobal->level_kills = 0;
        sGlobal->total_gold = 0;
        sGlobal->total_enemies = 0;
        sGlobal->curHP = sGlobal->cloth_hp;
        sGlobal->hasReachCheckPoint = false;
        sGlobal->hasReviveFromCheckPoint = false;
        
        sGlobal->extraGold = 0;
        
        sGlobal->level_enemy_gold = 0;
        sGlobal->level_total_gold = 0;
        sGlobal->accDistance = 0;
    } else {
        sGlobal->level_gold_normal = sGlobal->prev_gold_normal;
        sGlobal->level_gold = sGlobal->prev_gold;
        sGlobal->level_kills = sGlobal->prev_kills;
        sGlobal->level_enemy_gold = sGlobal->prev_enemygold;
        sGlobal->hasReviveFromCheckPoint = true;
//        LOGE("prev gold:%d, prev kills:%d, prev_enemy_gold:%d", sGlobal->prev_gold, sGlobal->prev_kills, sGlobal->level_enemy_gold);
        
        tempTotalGold = sGlobal->total_gold;
        tempTotalEnemies = sGlobal->total_enemies;
    }
    
    if (sGlobal->isRealEndless) {
        sGlobal->initEndlessMap(sGlobal->currentEModeDifficulty);
        sGlobal->level_enemy_kill = 0;
        sGlobal->level_bosses = 0;
        sGlobal->level_enemy_gold = 0;
        sGlobal->level_zuanshi = 0;
        sGlobal->extraGold = 0;
        sGlobal->isQuestRecorded = false;
    }

    sGlobal->mainGameLayer->onExit();
    sGlobal->effectLayer->onExit();
    sGlobal->hudLayer->onExit();
    sGlobal->gameBackgroundLayer->onExit();

    removeChildLocked(sGlobal->mainGameLayer, true);
    removeChildLocked(sGlobal->hudLayer, true);
    removeChildLocked(sGlobal->effectLayer, true);
    removeChildLocked(sGlobal->gameBackgroundLayer, true);
    wyNode *node = this->getChildByTag(4);
    if (node != NULL) {
        removeChildLocked(node, true);
    }
    sGlobal->resetCaches();
    
    addChildLocked((wyLayer*) (new GameBackgroundLayer(sGlobal->index))->autoRelease(), 0, 0);
    addChildLocked((wyLayer*) (new EffectLayer(sGlobal->index))->autoRelease(), 4, 2);
    addChildLocked((wyLayer*) (new HudLayer(sGlobal->index))->autoRelease(), 3, 3);
    addChildLocked((wyLayer*) (new MainGameLayer(sGlobal->index, isFromCheckPoint))->autoRelease(), 2, 1);
    
    wyIntervalAction *a = wyFadeOut::make(2.0f);
    wyActionCallback callback = {
        NULL,
        onTransEnded
    };
    a->setCallback(&callback, bg);
    
    bg->runAction(a);
    
    if (isFromCheckPoint) {
        if (mapIndex == 0) {
            sGlobal->hudLayer->showNotice(sGlobal->getIntroStr(STR_INTRO_FAILED));
        } else {
            sGlobal->hudLayer->setHPBar(sGlobal->cloth_hp/2);
        }
        sGlobal->mainGameLayer->hero->showFaZhen();
        
        sGlobal->total_gold = tempTotalGold;
        sGlobal->total_enemies = tempTotalEnemies;
    }
    
    //    if (!isFromCheckPoint && sGlobal->isRealEndless) {
    //        sGlobal->mainGameLayer->onExit();
    //        sGlobal->effectLayer->onExit();
    //        sGlobal->hudLayer->onExit();
    //        sGlobal->mainGameLayer->getParent()->addChild((wyLayer*) (new QuestLayer())->autoRelease(), 4, 4);
    //        ((MainScene *)sGlobal->mainGameLayer->getParent())->sceneState = SC_OPTION_OPENED;
    //    }
    
    sceneState = SC_NORMAL;
    
    isQuiting = false;
    sGlobal->curAchvShowCount = 0;
    //    sGlobal->reportGameEvent(EVT_LEVEL);
#if ANDROID
    //callGC();
#endif
    
    if (!isFromCheckPoint) {
        sGlobal->stopBGM();
        wyAction * action = wySequence::make(wyDelayTime::make(0.5f), wyCallFunc::make(wyTargetSelector::make(this, SEL(MainScene::playbgmWithDelay))), NULL);
        this->runAction(action);
    }
    
#if ANDROID
    if (!sGlobal->isRealEndless) {
        if (isFromCheckPoint) {
            //EVT_GAME_REVIVE
            sGlobal->reportGameEvent(EVT_GAME_REVIVE, mapIndex, 0.0f, DIFFICULTY_STR[sGlobal->currentSModeDifficulty]);
        } else {
            //EVT_GAME_RESTART
            sGlobal->reportGameEvent(EVT_GAME_RESTART, mapIndex, 0.0f, DIFFICULTY_STR[sGlobal->currentSModeDifficulty]);
        }
    } else {
        sGlobal->reportGameEvent(EVT_GAME_ENDLESS_START, 0, 0.0f, DIFFICULTY_STR[sGlobal->currentEModeDifficulty]);
    }
#endif
    
    VungleVedioController::updatePlayTimes();
}

void MainScene::playbgmWithDelay(wyTargetSelector* ts){
    sGlobal->playBGM(1, true);
}

void MainScene::switchSeaon(int seasonId) {
    int accDist = sGlobal->accDistance + sGlobal->mainGameLayer->hero->body->GetPosition().x;
    
    sGlobal->mainGameLayer->onExit();
    sGlobal->effectLayer->onExit();
    sGlobal->hudLayer->onExit();
    sGlobal->gameBackgroundLayer->onExit();
    
    sGlobal->needPauseGame = true;

    wyColorLayer *bg = wyColorLayer::make();
    bg->setColor(wyc3b(0, 0, 0));
    addChildLocked(bg, 100);
    
    removeChildLocked(sGlobal->mainGameLayer, true);
    removeChildLocked(sGlobal->hudLayer, true);
    removeChildLocked(sGlobal->effectLayer, true);
    removeChildLocked(sGlobal->gameBackgroundLayer, true);
    wyNode *node = this->getChildByTag(4);
    if (node != NULL) {
        removeChildLocked(node, true);
    }

    sGlobal->resetCaches();
    
    int realSeason = seasonId * 100;
    addChildLocked((wyLayer*) (new GameBackgroundLayer(realSeason))->autoRelease(), 0, 0);
    addChildLocked((wyLayer*) (new HudLayer(realSeason))->autoRelease(), 4, 3);
    addChildLocked((wyLayer*) (new EffectLayer(realSeason))->autoRelease(), 3, 2);
    addChildLocked((wyLayer*) (new MainGameLayer(realSeason))->autoRelease(), 2, 1);

    wyIntervalAction *a = wyFadeOut::make(2.0f);
    wyActionCallback callback = {
        NULL,
        onTransEnded
    };
    a->setCallback(&callback, bg);
    
    bg->runAction(a);
    
    sceneState = SC_NORMAL;
    
    sGlobal->accDistance = accDist;

    sGlobal->curAchvShowCount = 0;
#if ANDROID
    //callGC();
#endif
}





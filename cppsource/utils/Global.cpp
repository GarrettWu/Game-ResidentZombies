#include "Global.h"
#include "WiEngine-WiSound.h"
#if IOS
#include "ObjCCalls.h"
#endif

#include "WorldMapScene.h"
#include "UpgradeScene.h"
#include "CGScene.h"

#include "ShopLayer.h"
#include "FreeStuffLayer.h"
#include "MissionsLayer.h"
#include "LoadingScene.h"
#include "SettingsLayer.h"
#include "StatisticsLayer.h"
#include "CEncryptValue.h"

DECLARE_SINGLETON_MEMBER(Global);

Global::Global(void)
{
    mainGameLayer = NULL;
    gameBackgroundLayer = NULL;
    hudLayer = NULL;
    effectLayer = NULL;
	deadNoticeLayer = NULL;
    optionLayer = NULL;
    shopLayer = NULL;
}

Global::~Global(void)
{
    
}

void Global::init() {
    scaleX = wyDevice::winHeight/320.0f*0.75f;
    scaleY = wyDevice::winHeight/320.0f*0.75f;
    
    hero_offset_left = DP(100)*scaleX;
    srand(time(0));
    
    LOGE("org=%d,%d;scale=%f,%f, offset=%f, density=%f", wyDevice::winWidth, wyDevice::winHeight, scaleX, scaleY, hero_offset_left, wyDevice::density);
    
    currentBGMId = -1;
    
    isDebug = true;
    isShowBox2DDebug = true;
    needShowAchv = true;
    
    curLanguage = wyDevice::getLanguage();
    if (strcmp(sGlobal->curLanguage, "zh") == 0) {
        curLanguage = "en";
    }
    else {
        curLanguage = "en";
    }
    
#if IOS
    curLanguage = "en";
#endif
    LOGE("%s", curLanguage);
    if (strcmp(sGlobal->curLanguage, "zh") == 0) {
        uiName = "ui_zh";
    }
    else if (strcmp(sGlobal->curLanguage, "en") == 0) {
        uiName = "ui_en";
    }
    else if (strcmp(sGlobal->curLanguage, "jp") == 0) {
        uiName = "ui_en";
    } else {
        uiName = "ui_en";
    }
    
	virtualWinWidth = DP(600)*scaleX/0.75f;
    borderWidth = MAX(virtualWinWidth, wyDevice::winWidth);
    
    descFontSize = DP(36.0f);
    titleFontSize = DP(64.0f);
    
    tileMapOffsetYExtra = -DP(6)*scaleY;
    
    needPauseGame = false;
    isPlayingBGM = false;
    
    int latestVersion = 11;//10,
    int version = wyPrefs::getInt("version", -1);
    if (version == -1) {
        wyPrefs::clear();
        wyPrefs::setInt("version", latestVersion);
    }
    else if (version != latestVersion) {
        //v11增加
        wyPrefs::setInt("version", latestVersion);
        if (strcmp(wyPrefs::getString(DB_WEAPON_NOTICE_ARRAY, ""), "") == 0) {
            int *temp = new int[ARR_LEN_WEAPON_NUM];
            memset(temp, 0, sizeof(int)*ARR_LEN_WEAPON_NUM);
            arrayToStr(DB_WEAPON_NOTICE_ARRAY, temp, ARR_LEN_WEAPON_NUM);
            delete [] temp;
        }
        if (strcmp(wyPrefs::getString(DB_SKILL_NOTICE_ARRAY, ""), "") == 0) {
            int *temp = new int[ARR_LEN_SKILL_NUM];
            memset(temp, 0, sizeof(int)*ARR_LEN_SKILL_NUM);
            arrayToStr(DB_SKILL_NOTICE_ARRAY, temp, ARR_LEN_SKILL_NUM);
            delete [] temp;
        }
    }
    
    isBoss1Strong = false;
    
    needTrainUpgradeWeapon = false;
    needTrainTool = false;
    
    sGlobal->combatValueToEnemyIdMap = NULL;
    
    isMapCached = false;
    prob_combat_value = NULL;
    isRealEndless = false;
    
    isBoostOn = false;
    boostMulti = 1.0f;
    heroVelocityX = HERO_VEL_X;
    
    skillMulti = NULL;
    hasSkillMulti = NULL;
    skill_gold = NULL;
    skill_cash = NULL;
    
    weapon_atk = NULL;
    weapon_gold = NULL;
    weapon_zuanshi = NULL;
    weapon_firecd = NULL;
    weapon_ammo_gold = NULL;
    
    needOpenQuest = false;
    isFirstStart = true;
    isTutorialFromMenu = false;
    
    font_desc = wyBitmapFont::loadFont("anitile.fnt", false, BFF_ANGELCODE_TXT, wyDevice::density);
    font_title = wyBitmapFont::loadFont("flyingl.fnt", false, BFF_ANGELCODE_TXT, wyDevice::density);
    font_desc_noborder = wyBitmapFont::loadFont("anitile_noborder.fnt", false, BFF_ANGELCODE_TXT, wyDevice::density);
    
    initConsts();
    
    coinChangeListeners = wyArrayNew(20);
    
    isShopOpening = false;
    noNeedToSaveRunDist = false;
    isRatingMe = false;

}

void Global::addCoinChangeListener(CoinChangeListener *listener) {
    for (int i = 0; i < coinChangeListeners->num; i++) {
        CoinChangeListener *ls = (CoinChangeListener*)wyArrayGet(coinChangeListeners, i);
        if (ls == listener) {
//            LOGE("find same listener, no need to add..");
            return;
        }
    }
    wyArrayPush(coinChangeListeners, listener);
}

void Global::removeCoinChangeListener(CoinChangeListener *listener) {
    wyArrayDeleteObj(coinChangeListeners, listener, NULL, NULL);
}

void Global::notifyAllCoinChangeListeners() {
    for (int i = 0; i < coinChangeListeners->num; i++) {
        CoinChangeListener *ls = (CoinChangeListener*)wyArrayGet(coinChangeListeners, i);
        ls->onCoinChanged();
    }
}

void Global::initGame() {
//    wyAudioManager::getInstance()->preloadEffect("se_button.mp3");
    
    bool hasProfile = wyPrefs::getBool(DB_HAS_PORFILE, false);
    
    if (!hasProfile) {
        resetPrefs();
    } else {
        setGold(MyPrefs::getInt(DB_GOLD));
        zuanshi.setValue(MyPrefs::getInt(DB_ZUANSHI_NUM, 0));
        
        s01_grade = new int[ARR_LEN_LEVEL_NUM];
        memset(s01_grade, GRADE_NONE, sizeof(int)*ARR_LEN_LEVEL_NUM);
        s02_grade = new int[ARR_LEN_LEVEL_NUM];
        memset(s02_grade, GRADE_NONE, sizeof(int)*ARR_LEN_LEVEL_NUM);
        s03_grade = new int[ARR_LEN_LEVEL_NUM];
        memset(s03_grade, GRADE_NONE, sizeof(int)*ARR_LEN_LEVEL_NUM);
        
        s01_score = new int[ARR_LEN_LEVEL_NUM];
        memset(s01_score, 0, sizeof(int)*ARR_LEN_LEVEL_NUM);
        s02_score = new int[ARR_LEN_LEVEL_NUM];
        memset(s02_score, 0, sizeof(int)*ARR_LEN_LEVEL_NUM);
        s03_score = new int[ARR_LEN_LEVEL_NUM];
        memset(s03_score, 0, sizeof(int)*ARR_LEN_LEVEL_NUM);
        
        //3个难度
        currentProgress = new int[3];
        memset(currentProgress, 0, sizeof(int)*3);
        
        achv_rec = new int[ARR_LEN_ACHV_NUM];
        memset(achv_rec, 0, sizeof(int)*ARR_LEN_ACHV_NUM);
        achv_progress = new int[ARR_LEN_ACHV_NUM];
        memset(achv_progress, 0, sizeof(int)*ARR_LEN_ACHV_NUM);
        achv_progress_old = new int[ARR_LEN_ACHV_NUM];
        memset(achv_progress_old, 0, sizeof(int)*ARR_LEN_ACHV_NUM);
        achv_progress_temp = new int[ARR_LEN_ACHV_NUM];
        memset(achv_progress_temp, 0, sizeof(int)*ARR_LEN_ACHV_NUM);
        
        skillArray = new CDynamicValueInt[ARR_LEN_SKILL_NUM];
        memset(skillArray, 0, sizeof(CDynamicValueInt)*ARR_LEN_SKILL_NUM);
        
        weaponUnlockArray = new CDynamicValueInt[ARR_LEN_WEAPON_NUM];
        memset(weaponUnlockArray, 0, sizeof(CDynamicValueInt)*ARR_LEN_WEAPON_NUM);
        
        weaponAmmoArray = new int[ARR_LEN_WEAPON_NUM];
        memset(weaponAmmoArray, 0, sizeof(int)*ARR_LEN_WEAPON_NUM);

        weaponNoticeArray = new int[ARR_LEN_WEAPON_NUM];
        memset(weaponNoticeArray, 0, sizeof(int)*ARR_LEN_WEAPON_NUM);
        
        skillNoticeArray = new int[ARR_LEN_SKILL_NUM];
        memset(skillNoticeArray, 0, sizeof(int)*ARR_LEN_SKILL_NUM);
        
        strToArray(MyPrefs::getString(DB_S01_GRADE).c_str(), s01_grade);
        strToArray(MyPrefs::getString(DB_S02_GRADE).c_str(), s02_grade);
        strToArray(MyPrefs::getString(DB_S03_GRADE).c_str(), s03_grade);
        
        strToArray(MyPrefs::getString(DB_S01_SCORE).c_str(), s01_score);
        strToArray(MyPrefs::getString(DB_S02_SCORE).c_str(), s02_score);
        strToArray(MyPrefs::getString(DB_S03_SCORE).c_str(), s03_score);
        
        strToArray(MyPrefs::getString(DB_PROGRESS).c_str(), currentProgress);
        
        strToArray(MyPrefs::getString(DB_ACHV_REC).c_str(), achv_rec);
        strToArray(MyPrefs::getString(DB_ACHV_PROGRESS).c_str(), achv_progress);
        strToArray(MyPrefs::getString(DB_ACHV_PROGRESS).c_str(), achv_progress_old);
        
        strToArrayEncy(MyPrefs::getString(DB_SKILL_ARRAY).c_str(), skillArray);
        strToArrayEncy(MyPrefs::getString(DB_WEAPON_UNLOCK_ARRAY).c_str(), weaponUnlockArray);
        strToArray(MyPrefs::getString(DB_WEAPON_AMMO_ARRAY).c_str(), weaponAmmoArray);
        strToArray(MyPrefs::getString(DB_WEAPON_NOTICE_ARRAY).c_str(), weaponNoticeArray);
        strToArray(MyPrefs::getString(DB_SKILL_NOTICE_ARRAY).c_str(), skillNoticeArray);
        activeSkill = MyPrefs::getInt(DB_ACTIVE_SKILL);
        
        isTrainingEnd = MyPrefs::getInt(DB_IS_TRAINING_END, 0);
        isUpgradeTrained = MyPrefs::getInt(DB_IS_UPGRADE_TRAINED, 0);
        
        activeWeapon = MyPrefs::getInt(DB_ACTIVE_WEAPON);
        
        isSFXOn = wyPrefs::getBool(DB_IS_SFX_ON);
        isBGMOn = wyPrefs::getBool(DB_IS_BGM_ON);
        
        isAdsRemoved = MyPrefs::getInt(DB_IS_ADS_REMOVED, 0);
        isDoubleCoin = MyPrefs::getInt(DB_IS_DOUBLE_COIN, 0);
        
        isToolTrained = MyPrefs::getInt(DB_IS_TOOL_TRAINED, 0);
        isXieZiTrained = MyPrefs::getInt(DB_IS_XIEZI_TRAINED, 0);
        isLaoYingTrained = MyPrefs::getInt(DB_IS_LAOYING_TRAINED, 0);
        isDaBiaoTrained = MyPrefs::getInt(DB_IS_DABIAO_TRAINED, 0);
        
        killedEnemies = MyPrefs::getInt(DB_KILLED_ENEMY, 0);
        totalKilledEnemies = MyPrefs::getInt(DB_TOTAL_KILLED_ENEMY, 0);
        killedBosses = MyPrefs::getInt(DB_KILLED_BOSSES, 0);
        saveMeTimes = MyPrefs::getInt(DB_SAVEME_TIMES, 0);
        runDistance = MyPrefs::getInt(DB_RUN_DISTANCE, 0);
        totalRunDistance = MyPrefs::getInt(DB_TOTAL_RUN_DISTANCE, 0);
        
        distRecNormal = MyPrefs::getInt(DB_DIST_REC_NORMAL, -1);
        distRecHeroic = MyPrefs::getInt(DB_DIST_REC_HEROIC, -1);
        distRecInfernal = MyPrefs::getInt(DB_DIST_REC_INFERNAL, -1);
        
        isFirstPurchaseRewarded = MyPrefs::getInt(DB_IS_FIRST_PURCHASE_REWARDED, 0);
        
        missionId1 = MyPrefs::getInt(DB_MISSION1_ID, 0);
        missionId2 = MyPrefs::getInt(DB_MISSION2_ID, 0);
        missionId3 = MyPrefs::getInt(DB_MISSION3_ID, 0);
        
        isGameComplete = MyPrefs::getInt(DB_IS_GAME_COMPLETE, 0);
        
        totalCollectedCoins = MyPrefs::getInt(DB_TOTAL_COLLECTED_COINS, 0);
        
        isBigPackSoldout = MyPrefs::getInt(DB_IS_BIGPACK_SOLDOUT, 0);
    }
    
    if (isDebug) {
//        weaponUnlockArray[WEAPON_6].setValue(1);
//        weaponAmmoArray[WEAPON_6] = 100000;
//        sGlobal->isDoubleCoin = 1;
    }
    
    initAllWeaponInfo();
    initAllSkillEffect();
    updateCloth();
    initCaches();
    
//    LOGE("enemy hp1:%d", ENEMY_HP[0]);
}

void Global::syncToPrefs() {
    MyPrefs::setInt(DB_GOLD, getGold());
    MyPrefs::setInt(DB_ZUANSHI_NUM, zuanshi.getValue());
    
    arrayToStr(DB_S01_GRADE, s01_grade, ARR_LEN_LEVEL_NUM);
    arrayToStr(DB_S02_GRADE, s02_grade, ARR_LEN_LEVEL_NUM);
    arrayToStr(DB_S03_GRADE, s03_grade, ARR_LEN_LEVEL_NUM);
    
    arrayToStr(DB_S01_SCORE, s01_score, ARR_LEN_LEVEL_NUM);
    arrayToStr(DB_S02_SCORE, s02_score, ARR_LEN_LEVEL_NUM);
    arrayToStr(DB_S03_SCORE, s03_score, ARR_LEN_LEVEL_NUM);
    
    arrayToStr(DB_PROGRESS, currentProgress, 3);
    
    arrayToStr(DB_ACHV_REC, achv_rec, ARR_LEN_ACHV_NUM);
    arrayToStr(DB_ACHV_PROGRESS, achv_progress, ARR_LEN_ACHV_NUM);
    
    arrayToStrEncy(DB_SKILL_ARRAY, skillArray, ARR_LEN_SKILL_NUM);
    
    arrayToStrEncy(DB_WEAPON_UNLOCK_ARRAY, weaponUnlockArray, ARR_LEN_WEAPON_NUM);
    arrayToStr(DB_WEAPON_AMMO_ARRAY, weaponAmmoArray, ARR_LEN_WEAPON_NUM);
    arrayToStr(DB_WEAPON_NOTICE_ARRAY, weaponNoticeArray, ARR_LEN_WEAPON_NUM);
    arrayToStr(DB_SKILL_NOTICE_ARRAY, skillNoticeArray, ARR_LEN_SKILL_NUM);
    
    MyPrefs::setInt(DB_ACTIVE_SKILL, activeSkill);
    
    MyPrefs::setInt(DB_IS_TRAINING_END, isTrainingEnd);
    MyPrefs::setInt(DB_IS_UPGRADE_TRAINED, isUpgradeTrained);
    
    MyPrefs::setInt(DB_ACTIVE_WEAPON, activeWeapon);
    
    wyPrefs::setBool(DB_IS_SFX_ON, isSFXOn);
    wyPrefs::setBool(DB_IS_BGM_ON, isBGMOn);
    
    MyPrefs::setInt(DB_IS_ADS_REMOVED, isAdsRemoved);
    MyPrefs::setInt(DB_IS_DOUBLE_COIN, isDoubleCoin);
    
    MyPrefs::setInt(DB_IS_TOOL_TRAINED, isToolTrained);
    MyPrefs::setInt(DB_IS_XIEZI_TRAINED, isXieZiTrained);
    MyPrefs::setInt(DB_IS_LAOYING_TRAINED, isLaoYingTrained);
    MyPrefs::setInt(DB_IS_DABIAO_TRAINED, isDaBiaoTrained);
    
    MyPrefs::setInt(DB_KILLED_ENEMY, killedEnemies);
    MyPrefs::setInt(DB_TOTAL_KILLED_ENEMY, totalKilledEnemies);
    MyPrefs::setInt(DB_KILLED_BOSSES, killedBosses);
    MyPrefs::setInt(DB_SAVEME_TIMES, saveMeTimes);
    MyPrefs::setInt(DB_RUN_DISTANCE, runDistance);
    MyPrefs::setInt(DB_TOTAL_RUN_DISTANCE, totalRunDistance);
    
    MyPrefs::setInt(DB_DIST_REC_NORMAL, distRecNormal);
    MyPrefs::setInt(DB_DIST_REC_HEROIC, distRecHeroic);
    MyPrefs::setInt(DB_DIST_REC_INFERNAL, distRecInfernal);
    
    MyPrefs::setInt(DB_IS_FIRST_PURCHASE_REWARDED, isFirstPurchaseRewarded);
    
    MyPrefs::setInt(DB_MISSION1_ID, missionId1);
    MyPrefs::setInt(DB_MISSION2_ID, missionId2);
    MyPrefs::setInt(DB_MISSION3_ID, missionId3);
    
    MyPrefs::setInt(DB_IS_GAME_COMPLETE, isGameComplete);
    
    MyPrefs::setInt(DB_TOTAL_COLLECTED_COINS, totalCollectedCoins);
    
    MyPrefs::setInt(DB_IS_BIGPACK_SOLDOUT, isBigPackSoldout);
}

void Global::resetPrefs() {
    setGold(0);
    zuanshi.setValue(0);
    
    //火球
    activeSkill = SKILL_MISSLE;
    //新手关
//    currentProgress = 101;
    
    s01_grade = new int[ARR_LEN_LEVEL_NUM];
    memset(s01_grade, GRADE_NONE, sizeof(int)*ARR_LEN_LEVEL_NUM);
    s02_grade = new int[ARR_LEN_LEVEL_NUM];
    memset(s02_grade, GRADE_NONE, sizeof(int)*ARR_LEN_LEVEL_NUM);
    s03_grade = new int[ARR_LEN_LEVEL_NUM];
    memset(s03_grade, GRADE_NONE, sizeof(int)*ARR_LEN_LEVEL_NUM);
    
    s01_score = new int[ARR_LEN_LEVEL_NUM];
    memset(s01_score, 0, sizeof(int)*ARR_LEN_LEVEL_NUM);
    s02_score = new int[ARR_LEN_LEVEL_NUM];
    memset(s02_score, 0, sizeof(int)*ARR_LEN_LEVEL_NUM);
    s03_score = new int[ARR_LEN_LEVEL_NUM];
    memset(s03_score, 0, sizeof(int)*ARR_LEN_LEVEL_NUM);
    
    currentProgress = new int[3];
    memset(currentProgress, 0, sizeof(int)*3);
    currentProgress[0] = 101;
    currentProgress[1] = 0;
    currentProgress[2] = 0;
    
    achv_rec = new int[ARR_LEN_ACHV_NUM];
    memset(achv_rec, 0, sizeof(int)*ARR_LEN_ACHV_NUM);
    achv_progress = new int[ARR_LEN_ACHV_NUM];
    memset(achv_progress, 0, sizeof(int)*ARR_LEN_ACHV_NUM);
    achv_progress_old = new int[ARR_LEN_ACHV_NUM];
    memset(achv_progress_old, 0, sizeof(int)*ARR_LEN_ACHV_NUM);
    achv_progress_temp = new int[ARR_LEN_ACHV_NUM];
    memset(achv_progress_temp, 0, sizeof(int)*ARR_LEN_ACHV_NUM);
    
    skillArray = new CDynamicValueInt[ARR_LEN_SKILL_NUM];
    memset(skillArray, 0, sizeof(CDynamicValueInt)*ARR_LEN_SKILL_NUM);
    skillArray[0].setValue(1);
    
    weaponUnlockArray = new CDynamicValueInt[ARR_LEN_WEAPON_NUM];
    memset(weaponUnlockArray, 0, sizeof(CDynamicValueInt)*ARR_LEN_WEAPON_NUM);
    weaponUnlockArray[0].setValue(1);
    
    weaponAmmoArray = new int[ARR_LEN_WEAPON_NUM];
    memset(weaponAmmoArray, 0, sizeof(int)*ARR_LEN_WEAPON_NUM);
    
    weaponNoticeArray = new int[ARR_LEN_WEAPON_NUM];
    memset(weaponNoticeArray, 0, sizeof(int)*ARR_LEN_WEAPON_NUM);
    
    skillNoticeArray = new int[ARR_LEN_SKILL_NUM];
    memset(skillNoticeArray, 0, sizeof(int)*ARR_LEN_SKILL_NUM);

    isTrainingEnd = 0;
    isUpgradeTrained = 0;
    
    activeWeapon = WEAPON_UMP;
    
    isSFXOn = true;
    isBGMOn = true;
    
    isAdsRemoved = 0;
    isDoubleCoin = 0;
    
    isToolTrained = 0;
    isXieZiTrained = 0;
    isLaoYingTrained = 0;
    isDaBiaoTrained = 0;
    
    killedEnemies = 0;
    totalKilledEnemies = 0;
    killedBosses = 0;
    saveMeTimes = 0;
    runDistance = 0;
    totalRunDistance = 0;
    
    distRecNormal = -1;
    distRecHeroic = -1;
    distRecInfernal = -1;
    
    isFirstPurchaseRewarded = 0;
    
    isGameComplete = 0;
    totalCollectedCoins = 0;
    isBigPackSoldout = 0;
    
    initMissionIds();
    initAllWeaponInfo();
    initAllSkillEffect();
    
    if (isDebug) {
        //        gold = 999999;
        //        zuanshi = 999999;
//        sGlobal->isDaBiaoTrained = 1;
    }
    
    syncToPrefs();
    wyPrefs::setBool(DB_HAS_PORFILE, true);
    
//    for (int i = 0; i < ARR_LEN_ACHV_NUM; i++) {
//        LOGE("no:%d, seq:%d, rec:%d, progress:%d, progressold:%d, temp:%d", i, ACHV_SEQ[i], sGlobal->achv_rec[ACHV_SEQ[i]], sGlobal->achv_progress[ACHV_SEQ[i]], sGlobal->achv_progress_old[ACHV_SEQ[i]], sGlobal->achv_progress_temp[ACHV_SEQ[i]]);
//    }
}

void Global::updateFireFury() {
    
}

void Global::updateIllusion() {
    
}

void Global::updateSandShield() {
    
}

int Global::getWeaponAtk() {
    //    if (sGlobal->hasSkillMulti[SKILL_10]) {
    //        return round(weapon_atk[activeWeapon] + ceil(sGlobal->skillMulti[SKILL_10] * weapon_atk[activeWeapon]));
    //    } else {
    return round(weapon_atk[activeWeapon]);
    //    }
}

void Global::updateCloth() {
    //    cloth_gold = (cloth_grade + 1) * DATA_CLOTH_UPGRADE_GOLD;
    cloth_hp = HERO_BASE_HP;//round(DATA_CLOTH_INIT_HP * powf(DATA_CLOTH_UPGRADE_HP, cloth_grade));
    if (sGlobal->hasSkillMulti[SKILL_STAMINA]) {
        cloth_hp += sGlobal->skillMulti[SKILL_STAMINA];
    }
}

void Global::updateHead() {

}

void Global::strToArrayEncy(const char * str, CDynamicValueInt *array) {
    std::string content(str);
    int index = content.find(';');
    std::string nextStr;
    int i = 0;
    while (index != -1) {
        nextStr = content.substr(0, index);
        array[i].setValue(atoi(nextStr.c_str()));
        i++;
        content = content.substr(index + 1, content.length());
        index = content.find(';');
    }
}

void Global::arrayToStrEncy(const char * label, CDynamicValueInt *array, int arr_len) {
    char temp[5000];
    sprintf(temp, "%d;", array[0].getValue());
    for (int i = 1; i < arr_len; i++) {
        sprintf(temp, "%s%d;", temp, array[i].getValue());
    }
    MyPrefs::setString(label, temp);
}

void Global::strToArray(const char * str, int *array) {
    std::string content(str);
    int index = content.find(';');
    std::string nextStr;
    int i = 0;
    while (index != -1) {
        nextStr = content.substr(0, index);
        array[i] = atoi(nextStr.c_str());
        i++;
        content = content.substr(index + 1, content.length());
        index = content.find(';');
    }
}

void Global::arrayToStr(const char * label, int *array, int arr_len) {
    char temp[5000];
    sprintf(temp, "%d;", array[0]);
    for (int i = 1; i < arr_len; i++) {
        sprintf(temp, "%s%d;", temp, array[i]);
    }
    MyPrefs::setString(label, temp);
}

void Global::playBGM(int id, bool isForce) {
//    id = 0;
    if ((currentBGMId == id && !isForce) && isPlayingBGM) {
        return;
    } else {
//        if (!((currentBGMId == id) && isForce)) {
            wyAudioManager::getInstance()->stopBackgroundMusic();
            currentBGMId = id;
//        }
    }
    
    if (!sGlobal->isBGMOn) {
//        currentBGMId = -1;
        return;
    }
    
    switch (id) {
        case 0:
            wyAudioManager::getInstance()->playBackgroundMusic("sound/bgm_main.ogg", false, -1);
            break;
        case 1:
        case 2:
        case 3:
            wyAudioManager::getInstance()->playBackgroundMusic("sound/bgm_s01.ogg", false, -1);
            break;
        default:
            break;
    }
    isPlayingBGM = true;
}

void Global::stopBGM() {
    //    currentBGMId = -1;
    prevBGMId = currentBGMId;
    isPlayingBGM = false;
    wyAudioManager::getInstance()->stopBackgroundMusic();
}

void Global::replayBGM() {
    playBGM(prevBGMId);
}

void Global::playEffect(int sfxId) {
    if (!sGlobal->isSFXOn) {
        return;
    }
    
    wyAudioManager::getInstance()->playEffect(SE_FILE_NAME[sfxId], false);
}

void Global::preloadEffect(int sfxId) {
    wyAudioManager::getInstance()->preloadEffect(SE_FILE_NAME[sfxId], false);
}


static void onFadeOutEnd(wyAction* action, void* data) {
    wyNode *container = (wyNode *)data;
    container->getParent()->removeChildLocked(container, true);
    sGlobal->curAchvShowCount = MAX(sGlobal->curAchvShowCount-1, 0);
}

static void onAchievmentShowEnd(wyAction* action, void* data) {
    wyNode *container = (wyNode *)data;
    container->getChildByTag(1)->runAction(wyFadeOut::make(0.3f));
    container->getChildByTag(2)->runAction(wyFadeOut::make(0.3f));
    container->getChildByTag(3)->runAction(wyFadeOut::make(0.3f));
    wyIntervalAction* t = wyDelayTime::make(0.4f);
    wyActionCallback callback = {
        NULL,
        onFadeOutEnd,
        NULL,
    };
    t->setCallback(&callback, container);
    container->runAction(t);
}

void Global::showAchievmentEffect(int achvId, wyNode *parent, int x) {
    if (!needShowAchv) {
        return;
    }
    sGlobal->curAchvShowCount++;
//    LOGE("showAchv:%d", achvId);
    //    if (achvId == ACHV_SHASHOU || achvId == ACHV_GUAIWULIEREN || achvId == ACHV_WOHUILAILE || achvId == ACHV_AGAN || achvId == ACHV_SIBULIAO || achvId == ACHV_PAODEKUAI || achvId == ACHV_SHANDIANZHISHEN || achvId == ACHV_GUAIWULIEREN || achvId == ACHV_FEIXIANGDEMAO || achvId == ACHV_SHAOKAO || achvId == ACHV_PIANRENBUCHANGMING || achvId == ACHV_SUOXIANGPINI || achvId == ACHV_RENZHEXINYANG || achvId == ACHV_CHAOJIWANJIA) {
    //        hasBadge = true;
    //    }
//    wyColor4B color = wyc4b(198, 65, 35, 255);
    wyColor4B color = wyc4b(0, 0, 0, 200);
    float sy = 0.85f;
    float borderScale = DP(6)*sGlobal->scaleX/DP(10);
    float borderWidth = DP(620)*sGlobal->scaleX*0.9f;
    float borderHeight = DP(84)*sGlobal->scaleY*sy;
    wyColor3B blackColor = wyc3b(0, 0, 0);
    
    sGlobal->playEffect(se_upgrade);
    
    wyNode *container = wyNode::make();
//    if (x == -1) {
        
//    } else {
//        container->setPosition(x, wyDevice::winHeight + DP(66)*sGlobal->scaleY);
//    }
    parent->addChildLocked(container, 999);
    
    wyQuadParticleSystem *achv_emitter = wyParticleLoader::load("particle_star.plist");
    achv_emitter->setPosition(0, -DP(106)*sGlobal->scaleY);
    container->addChildLocked(achv_emitter, 0, 0);
    achv_emitter->setScaleX(sGlobal->scaleX/0.75);
    achv_emitter->setScaleY(sGlobal->scaleY/0.75);
    
    wyBitmapFontLabel *achv_title;
    
    char temp[50];
    char temp1[20];
    addComma(sGlobal->ACHV_NUM[achvId*3], temp1);
    sprintf(temp, ACHV_DESC[achvId], temp1);
    
#if ANDROID
    sGlobal->reportGameEvent(EVT_GAME_MISSION_COMPLETE, 0, 0.0f, temp);
#endif
    
    achv_title = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
    achv_title->setScale(SP(18)*sGlobal->scaleX/sGlobal->descFontSize);
    achv_title->setAlignment(wyBitmapFontLabel::LEFT);
    achv_title->setColor(wyc3b(255, 255, 255));
    container->addChildLocked(achv_title, 10, 2);
    achv_title->setAnchor(0, 0.5f);
    
    wySpriteEx *badge = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "mission_complete.png");
    badge->setScaleX(sGlobal->scaleX*0.4f);
    badge->setScaleY(sGlobal->scaleY*0.4f);
    badge->setAnchor(0, 0.5f);
    badge->setPosition(DP(15)*sGlobal->scaleX, borderHeight/2-DP(6)*sGlobal->scaleY);
    container->addChildLocked(badge, 10);
    
    achv_title->setPosition(badge->getPositionX()+badge->getWidth()*badge->getScaleX()+DP(10)*sGlobal->scaleX, badge->getPositionY()-DP(0)*sGlobal->scaleY);
    
    float w = achv_title->getWidth()*achv_title->getScaleX()+achv_title->getPositionX()+DP(40)*sGlobal->scaleX;

    wySpriteEx *frame_down = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_down_10.png");
    frame_down->setScaleX(sGlobal->scaleX*0.9f);
    frame_down->setScaleY(sGlobal->scaleY*sy);
    frame_down->setAnchor(0, 0);
    container->addChildLocked(frame_down, 0, 10);
    frame_down->setColor(color);
    frame_down->setScaleX(w/frame_down->getWidth());
    
    wySpriteEx *frame_left = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_left_16.png");
    frame_left->setScaleX(sGlobal->scaleX*0.7f);
    frame_left->setScaleY(sGlobal->scaleY*sy);
    frame_left->setAnchor(1.0f, 0);
    container->addChildLocked(frame_left, 0, 11);
    frame_left->setColor(color);
    
    wySpriteEx *frame_right = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_right_16.png");
    frame_right->setScaleX(sGlobal->scaleX*0.7f);
    frame_right->setScaleY(sGlobal->scaleY*sy);
    frame_right->setAnchor(0, 0);
    frame_right->setPosition(frame_down->getWidth()*frame_down->getScaleX(), 0);
    container->addChildLocked(frame_right, 0, 12);
    frame_right->setColor(color);
    
    wySpriteEx *frame_up = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "frame_up_10.png");
    frame_up->setScaleX(sGlobal->scaleX*0.9f);
    frame_up->setScaleY(sGlobal->scaleY*sy);
    frame_up->setAnchor(0, 1.0f);
    frame_up->setPosition(0, frame_left->getHeight()*frame_left->getScaleY());
    container->addChildLocked(frame_up, 0, 13);
    frame_up->setColor(color);
    frame_up->setScaleX(w/frame_up->getWidth());
    
    wyColorLayer *frame_bg = wyColorLayer::make();
    frame_bg->setContentSize(frame_down->getWidth()*frame_down->getScaleX(), frame_left->getHeight()*frame_left->getScaleY()-DP(16)*sGlobal->scaleY);
    frame_bg->setAnchor(0, 0);
    frame_bg->setPosition(0, DP(8)*sGlobal->scaleY);
    container->addChildLocked(frame_bg, 0, 14);
    frame_bg->setColor(color);
    
    container->setPosition(wyDevice::winWidth/2-w/2, wyDevice::winHeight + borderHeight/2);
    
    wyIntervalAction* a1 = wyMoveBy::make(1.0f, 0, -DP(105)*sGlobal->scaleY+DP(-70)*sGlobal->scaleY*(sGlobal->curAchvShowCount-1));
    wyIntervalAction *a2 = wyEaseElasticOut::make(0.8f, a1);
    wyIntervalAction* t = wySequence::make(a2, wyDelayTime::make(5.0f), NULL);
    wyActionCallback callback = {
        NULL,
        onAchievmentShowEnd,
        NULL,
    };
    t->setCallback(&callback, container);
    container->runAction(t);
}

void Global::addGold(int gold) {
    offsetGold(gold);
    MyPrefs::setInt(DB_GOLD, getGold());
    
    notifyAllCoinChangeListeners();
}

void Global::addZuanshi(int zuanshi) {
    this->zuanshi.offset(zuanshi);
    MyPrefs::setInt(DB_ZUANSHI_NUM, this->zuanshi.getValue());
    //    LOGE("addZuanShi:%d", zuanshi);
}

void Global::addGem(int zuanshi) {
    this->zuanshi.offset(zuanshi);
    MyPrefs::setInt(DB_ZUANSHI_NUM, this->zuanshi.getValue());
    //    LOGE("addZuanShi:%d", zuanshi);
    
}

void Global::reportGameEvent(int event_id, int i1, float f1, const char *s1, const char *s2) {
#if ANDROID
    char category[50];
    char action[50];
    char label[100];
    int value = 0;
    bool hasEvent = true;
    
    switch (event_id) {
        case EVT_OFFER_MENU:
            sprintf(category, "%s", "IAP");
            sprintf(action, "%s", "ShowOfferwall");
            sprintf(label, "%s", "menu");
            break;
        case EVT_OFFER_ARMS:
            sprintf(category, "%s", "IAP");
            sprintf(action, "%s", "ShowOfferwall");
            sprintf(label, "%s", "arms");
            break;
        case EVT_OFFER_SKILLS:
            sprintf(category, "%s", "IAP");
            sprintf(action, "%s", "ShowOfferwall");
            sprintf(label, "%s", "skills");
            break;
        case EVT_OFFER_COMPLETE:
            sprintf(category, "%s", "IAP");
            sprintf(action, "%s", "CompleteOfferwall");
            sprintf(label, "[%d]", i1);
            break;
            
        case EVT_SHOP_MENU:
            sprintf(category, "%s", "IAP");
            sprintf(action, "%s", "ShowShop");
            sprintf(label, "%s", "menu");
            break;
        case EVT_SHOP_UPGRADE:
            sprintf(category, "%s", "IAP");
            sprintf(action, "%s", "ShowShop");
            sprintf(label, "%s", "upgrade");
            break;
        case EVT_SHOP_INGAME:
            sprintf(category, "%s", "IAP");
            sprintf(action, "%s", "ShowShop");
            sprintf(label, "%s", "ingame");
            break;
        case EVT_SHOP_UPGRADE_LACK:
            sprintf(category, "%s", "IAP");
            sprintf(action, "%s", "ShowShop");
            sprintf(label, "%s", "upgrade_lack");
            break;
            
        case EVT_PRE_BUY:
            sprintf(category, "%s", "IAP");
            sprintf(action, "%s", "PreBuy");
            sprintf(label, "[%.2f]", f1);
            break;
        case EVT_COMPLETE_BUY:
            sprintf(category, "%s", "IAP");
            sprintf(action, "%s", "CompleteBuy");
            sprintf(label, "[%.2f]", f1);
            break;
            
        case EVT_GAME_START:
            sprintf(category, "%s", "Game");
            sprintf(action, "%s", "Start");
            sprintf(label, "[%s][%d]", s1, i1);
            break;
        case EVT_GAME_RESTART:
            sprintf(category, "%s", "Game");
            sprintf(action, "%s", "Restart");
            sprintf(label, "[%s][%d]", s1, i1);
            break;
        case EVT_GAME_SAVEME:
            sprintf(category, "%s", "Game");
            sprintf(action, "%s", "SaveMe");
            sprintf(label, "[%s][%d]", s1, i1);
            break;
        case EVT_GAME_REVIVE:
            sprintf(category, "%s", "Game");
            sprintf(action, "%s", "Revive");
            sprintf(label, "[%s][%d]", s1, i1);
            break;
        case EVT_GAME_COMPLETE:
            sprintf(category, "%s", "Game");
            sprintf(action, "%s", "Complete");
            sprintf(label, "[%s][%d][%s]", s1, i1, s2);
            break;
            
        case EVT_GAME_ENDLESS_START:
            sprintf(category, "%s", "Game");
            sprintf(action, "%s", "EndlessStart");
            sprintf(label, "[%s]", s1);
            break;
        case EVT_GAME_ENDLESS_SAVEME:
            sprintf(category, "%s", "Game");
            sprintf(action, "%s", "EndlessSaveMe");
            sprintf(label, "[%s]", s1);
            break;
        case EVT_GAME_ENDLESS_END:
            sprintf(category, "%s", "Game");
            sprintf(action, "%s", "EndlessEnd");
            sprintf(label, "[%s][%d]", s1, i1);
            break;
            
        case EVT_GAME_MISSION_COMPLETE:
            sprintf(category, "%s", "Game");
            sprintf(action, "%s", "MissionComplete");
            sprintf(label, "[%s]", s1);
            break;
            
        case EVT_UPGRADE_GUN:
            sprintf(category, "%s", "Upgrade");
            sprintf(action, "%s", "Gun");
            sprintf(label, "[%s][%d]", s1, i1);
            break;
        case EVT_UPGRADE_SKILL:
            sprintf(category, "%s", "Upgrade");
            sprintf(action, "%s", "Skills");
            sprintf(label, "[%s][%d]", s1, i1);
            break;
            
        //i1:dayN
        case EVT_DAILYREWARD:
            sprintf(category, "%s", "Extra");
            sprintf(action, "%s", "DailyReward");
            sprintf(label, "[%d]", i1);
            break;
            
        //i1:show time
        case EVT_RATEME_SHOW:
            sprintf(category, "%s", "Extra");
            sprintf(action, "%s", "RateMeShow");
            sprintf(label, "[%d]", i1);
            break;
        case EVT_RATEME_YES:
            sprintf(category, "%s", "Extra");
            sprintf(action, "%s", "RateMeYes");
            sprintf(label, "[%d]", i1);
            break;
        case EVT_RATEME_NEVER:
            sprintf(category, "%s", "Extra");
            sprintf(action, "%s", "RateMeNever");
            sprintf(label, "[%d]", i1);
            break;
        case EVT_RATEME_LATER:
            sprintf(category, "%s", "Extra");
            sprintf(action, "%s", "RateMeLater");
            sprintf(label, "[%d]", i1);
            break;
            
        //[hard][101]
        case EVT_GAME_PROGRESS:
            sprintf(category, "%s", "Extra");
            sprintf(action, "%s", "GameProgress");
            sprintf(label, "[%s][%d]", s1, i1);
            break;
        default:
            hasEvent = false;
            break;
    }

    if (hasEvent) {
//        LOGE("report event: %s, %s, %s, %d", category, action, label, value);
        reportGaEvent(category, action, label, value);
    }
#endif
}

void Global::purchaseGameItemStart(int itemId) {
//    LOGE("start purchase item %d...", itemId);
    //    sGlobal->stopBGM();
#if ANDROID
    startPurchaseGameItem(itemId);
#endif
}

void Global::purchaseGameItemEnd(int itemId) {
//    LOGE("item %d purchased..., goods num:%d", itemId, SHOP_GOODS_NUM[itemId]);
    sGlobal->reportGameEvent(EVT_COMPLETE_BUY, 0, SHOP_GOODS_PRICE[itemId], NULL);
    
    isAdsRemoved = 1;
    MyPrefs::setInt(DB_IS_ADS_REMOVED, isAdsRemoved);
    
    switch (itemId) {
        case SHOP_COIN_SMALL_ID:
            offsetGold(SHOP_GOODS_NUM[SHOP_COIN_SMALL_ID]);
            MyPrefs::setInt(DB_GOLD, getGold());
            break;
        case SHOP_COIN_MIDDLE_ID:
            offsetGold(SHOP_GOODS_NUM[SHOP_COIN_MIDDLE_ID]);
            MyPrefs::setInt(DB_GOLD, getGold());
            break;
        case SHOP_COIN_LARGE_ID:
            offsetGold(SHOP_GOODS_NUM[SHOP_COIN_LARGE_ID]);
            MyPrefs::setInt(DB_GOLD, getGold());
            break;
        case SHOP_CASH_SMALL_ID:
            zuanshi.offset(SHOP_GOODS_NUM[SHOP_CASH_SMALL_ID]);
            MyPrefs::setInt(DB_ZUANSHI_NUM, zuanshi.getValue());
            break;
        case SHOP_CASH_MIDDLE_ID:
            zuanshi.offset(SHOP_GOODS_NUM[SHOP_CASH_MIDDLE_ID]);
            MyPrefs::setInt(DB_ZUANSHI_NUM, zuanshi.getValue());
            break;
        case SHOP_CASH_LARGE_ID:
            zuanshi.offset(SHOP_GOODS_NUM[SHOP_CASH_LARGE_ID]);
            MyPrefs::setInt(DB_ZUANSHI_NUM, zuanshi.getValue());
            break;
        case SHOP_BIGPACK_ID:
            offsetGold(SHOP_GOODS_NUM[SHOP_BIGPACK_ID]);
            zuanshi.offset(SHOP_GOODS_NUM[SHOP_BIGPACK_ID+1]);
            
            MyPrefs::setInt(DB_GOLD, getGold());
            MyPrefs::setInt(DB_ZUANSHI_NUM, zuanshi.getValue());
            
            if (isBigPackSoldout == 0) {
                isBigPackSoldout = 1;
                MyPrefs::setInt(DB_IS_BIGPACK_SOLDOUT, 1);
            }
            break;
        case SHOP_DBCOIN_ID:
            sGlobal->isDoubleCoin = 1;
            MyPrefs::setInt(DB_IS_DOUBLE_COIN, 1);
            break;
        default:
            break;
    }
    
    if (sGlobal->shopLayer != NULL) {
        shopLayer->setPurchaseSuccess(itemId);
    }
    
//    notifyAllPurchaseEndListeners(itemId);
}

void Global::purchaseGameItemEndIOS(int itemId, bool isRestore) {
#if IOS
    
#endif
}

int Global::getTotalScore() {
    int totalScore = 0;
    for (int i = 1; i < 45; i++) {
        totalScore += s01_score[i] + s02_score[i] + s03_score[i];
    }
    //    LOGE("total score:%d", totalScore);
    return totalScore;
}

void Global::calcEnemyCombatValue() {
    std::map< int, vector<int>* > *map = sGlobal->combatValueToEnemyIdMap;
    if (map != NULL) {
        std::map< int, vector<int>* >::iterator iter;
        for (iter = map->begin(); iter != map->end(); ++iter) {
            vector<int> *p = iter->second;
            p->clear();
        }
        delete map;
    }
    
    sGlobal->combatValueToEnemyIdMap = new std::map< int, vector<int>* >();
    map = sGlobal->combatValueToEnemyIdMap;
    
    if (prob_combat_value != NULL) {
        prob_combat_value->clear();
    } else {
        prob_combat_value = new vector<int>;
    }
    
    int realRound;
//    switch (sGlobal->currentEModeDifficulty) {
//        case DIFFICULTY_NORMAL:
//            realRound = sGlobal->combatRound;
//            break;
//        case DIFFICULTY_HEROIC:
//            realRound = sGlobal->combatRound+1;
//            break;
//        case DIFFICULTY_HARD:
//            realRound = sGlobal->combatRound+2;
//            break;
//        default:
//            realRound = sGlobal->combatRound;
//            break;
//    }
    realRound = sGlobal->combatRound;
    float power = 1.0f;
    int *combat_value_array;
    switch (realRound) {
        case 1:
            combat_value_array = ENEMY_COMBAT_VALUE_EASY;
            break;
        case 2:
            combat_value_array = ENEMY_COMBAT_VALUE_HARD;
            break;
        case 3:
            combat_value_array = ENEMY_COMBAT_VALUE_CRAZY;
            break;
        default:
            combat_value_array = ENEMY_COMBAT_VALUE_CRAZY;
            power = (float)pow(ENEMY_COMBAT_POWER, sGlobal->combatRound - 3);
            break;
    }
//    LOGE("realRound:%d", realRound);
    for (int i = 0; i < 29; i++) {
        int combatValue = combat_value_array[i]*power;
        
        //如果战斗值为0
        if (combatValue == 0) {
            continue;
        }
        
        std::map< int, vector<int>* >::iterator itr;
        itr = map->find(combatValue);
        if( itr != map->end() ) {
            vector<int> *p = itr->second;
            p->push_back(i);
//            LOGE("combatValue:%d, id:%d", combatValue, i);
        } else {
            vector<int> *p = new vector<int>();
            p->push_back(i);
            map->insert(std::pair< int, vector<int>* >(combatValue, p));
//            LOGE("combatValue:%d, id:%d", combatValue, i);
            
            vector<int>::iterator it;
            bool isInserted = false;
            for (it = prob_combat_value->begin(); it != prob_combat_value->end(); it++) {
                if (*it > combatValue) {
//                    LOGE("it:%d", *it);
                    isInserted = true;
                    //                        if (it == prob_combat_value->begin()) {
                    prob_combat_value->insert(it, combatValue);
                    //                        } else {
                    //                            prob_combat_value->insert(it-1, combatValue);
                    //                        }
                    break;
                }
            }
            if (!isInserted) {
                //                    LOGE("end it:%d", *prob_combat_value->end());
                prob_combat_value->insert(prob_combat_value->end(), combatValue);
            }
        }
    }
    
//    vector<int>::iterator it;
//    for (it = prob_combat_value->begin(); it != prob_combat_value->end(); it++) {
////        LOGE("prob_combat_value:%d", *it);
//    }
}

void Global::generateTileMapCache() {
    if (isMapCached)
        return;
    
    m_TileMapCache1 = new wyTMXTileMap*[200];
    memset(m_TileMapCache1, 0, sizeof(wyTMXTileMap*)*200);
    m_TileMapCache2 = new wyTMXTileMap*[200];
    memset(m_TileMapCache2, 0, sizeof(wyTMXTileMap*)*200);
    m_TileMapCache3 = new wyTMXTileMap*[200];
    memset(m_TileMapCache3, 0, sizeof(wyTMXTileMap*)*200);
    
    m_TileLayerCache1 = new wyTMXLayer*[200];
    memset(m_TileLayerCache1, 0, sizeof(wyTMXLayer*)*200);
    m_TileLayerCache2 = new wyTMXLayer*[200];
    memset(m_TileLayerCache2, 0, sizeof(wyTMXLayer*)*200);
    m_TileLayerCache3 = new wyTMXLayer*[200];
    memset(m_TileLayerCache3, 0, sizeof(wyTMXLayer*)*200);
    
    for(int i = 0; i < 26; i++) {
        if (i==9 || i==17 || i==18|| i==19) {
            continue;
        }
        char *mapName1 = new char[30];
        sprintf(mapName1, "map/s%02d_%02d.tmx", 1, i);
        char *textureName1 = new char[30];
        sprintf(textureName1, "s%02d.png", 1);
        //        LOGE("generate mapname:%s, texname:%s", mapName1, textureName1);
        wyTexture2D *tex = MyTextureManager::makePNG(textureName1);
        wyTMXTileMap *temp = wyTMXTileMap::make(mapName1, false, tex, NULL);
        m_TileMapCache1[i] = temp;
        temp->retain();
        
        wyTMXLayer *front = temp->getLayer("tile_terrain_deco_front");
        if (front != NULL) {
            front->retain();
            temp->removeChildLocked(front, false);
            m_TileLayerCache1[i] = front;
        }
        
        delete [] mapName1;
        delete [] textureName1;
    }
    
    for(int i = 0; i < 27; i++) {
        if (i==8 || i==9 || i==17 || i==18 || i==19) {
            continue;
        }
        
        char *mapName1 = new char[30];
        sprintf(mapName1, "map/s%02d_%02d.tmx", 2, i);
        char *textureName1 = new char[30];
        sprintf(textureName1, "s%02d.png", 2);
        //        LOGE("generate mapname:%s, texname:%s", mapName1, textureName1);
        wyTexture2D *tex = MyTextureManager::makePNG(textureName1);
        wyTMXTileMap *temp = wyTMXTileMap::make(mapName1, false, tex, NULL);
        m_TileMapCache2[i] = temp;
        temp->retain();
        
        wyTMXLayer *front = temp->getLayer("tile_terrain_deco_front");
        if (front != NULL) {
            front->retain();
            temp->removeChildLocked(front, false);
            m_TileLayerCache2[i] = front;
        }
        
        delete [] mapName1;
        delete [] textureName1;
    }
    
    for(int i = 0; i < 28; i++) {
        if (i==7 || i==8 || i==9 || i==18 || i==19) {
            continue;
        }
        
        char *mapName1 = new char[30];
        sprintf(mapName1, "map/s%02d_%02d.tmx", 3, i);
        char *textureName1 = new char[30];
        sprintf(textureName1, "s%02d.png", 3);
        //        LOGE("generate mapname:%s, texname:%s", mapName1, textureName1);
        wyTexture2D *tex = MyTextureManager::makePNG(textureName1);
        wyTMXTileMap *temp = wyTMXTileMap::make(mapName1, false, tex, NULL);
        m_TileMapCache3[i] = temp;
        temp->retain();
        
        wyTMXLayer *front = temp->getLayer("tile_terrain_deco_front");
        if (front != NULL) {
            front->retain();
            temp->removeChildLocked(front, false);
            m_TileLayerCache3[i] = front;
        }
        
        delete [] mapName1;
        delete [] textureName1;
    }
    
    wyTexture2D *tex = MyTextureManager::makePNG("s01.png");
    wyTMXTileMap *temp = wyTMXTileMap::make("map/s01_start_end.tmx", false, tex, NULL);
    m_TileMapCache1[100] = temp;
    temp->retain();
    wyTMXLayer *front = temp->getLayer("tile_terrain_deco_front");
    if (front != NULL) {
        front->retain();
        temp->removeChildLocked(front, false);
        m_TileLayerCache1[100] = front;
    }
    
    tex = MyTextureManager::makePNG("s02.png");
    temp = wyTMXTileMap::make("map/s02_start_end.tmx", false, tex, NULL);
    m_TileMapCache2[100] = temp;
    temp->retain();
    front = temp->getLayer("tile_terrain_deco_front");
    if (front != NULL) {
        front->retain();
        temp->removeChildLocked(front, false);
        m_TileLayerCache2[100] = front;
    }
    
    tex = MyTextureManager::makePNG("s03.png");
    temp = wyTMXTileMap::make("map/s03_start_end.tmx", false, tex, NULL);
    m_TileMapCache3[100] = temp;
    temp->retain();
    front = temp->getLayer("tile_terrain_deco_front");
    if (front != NULL) {
        front->retain();
        temp->removeChildLocked(front, false);
        m_TileLayerCache3[100] = front;
    }
    
    isMapCached = true;
    
    curMapIndex = -1;
    prevMapIndex = -2;
}

wyTMXTileMap* Global::getMap(int seasonId, int& idx) {
    //    LOGE("seasonId:%d", seasonId);
    int mapIndex;
    int prob = rand()%100;
    int seed = 0;
    int range1, range2, range3;
    if (sGlobal->isInBossBattle) {
        range1 = MAP_DIFFICULTY_BOSS[0];
        range2 = MAP_DIFFICULTY_BOSS[1];
//
//        switch (seasonId) {
//            case 1:
//                do {
//                    switch (seed) {
//                        case 0:
//                            mapIndex = randRangeInt(0, 8);
//                            break;
//                        case 1:
//                            mapIndex = randRangeInt(10, 16);
//                            break;
//                        case 2:
//                            mapIndex = randRangeInt(20, 25);
//                            break;
//                        default:
//                            mapIndex = randRangeInt(0, 8);
//                            break;
//                    }
//                } while (mapIndex == curMapIndex || mapIndex == prevMapIndex);
//                idx = mapIndex;
//                prevMapIndex = curMapIndex;
//                curMapIndex = idx;
//                return m_TileMapCache1[idx];
//                break;
//            case 2:
//                do {
//                    switch (seed) {
//                        case 0:
//                            mapIndex = randRangeInt(0, 7);
//                            break;
//                        case 1:
//                            mapIndex = randRangeInt(10, 16);
//                            break;
//                        case 2:
//                            mapIndex = randRangeInt(20, 26);
//                            break;
//                        default:
//                            mapIndex = randRangeInt(0, 7);
//                            break;
//                    }
//                } while (mapIndex == curMapIndex);
//                idx = mapIndex;
//                prevMapIndex = curMapIndex;
//                curMapIndex = idx;
//                //            LOGE("idx22:%d", idx);
//                return m_TileMapCache2[idx];
//                break;
//            case 3:
//                do {
//                    switch (seed) {
//                        case 0:
//                            mapIndex = randRangeInt(0, 6);
//                            break;
//                        case 1:
//                            mapIndex = randRangeInt(10, 17);
//                            break;
//                        case 2:
//                            mapIndex = randRangeInt(20, 27);
//                            break;
//                        default:
//                            mapIndex = randRangeInt(0, 6);
//                            break;
//                    }
//                } while (mapIndex == curMapIndex);
//                idx = mapIndex;
//                prevMapIndex = curMapIndex;
//                curMapIndex = idx;
//                return m_TileMapCache3[idx];
//                break;
//            default:
//                return NULL;
//                break;
//        }
    }
    else if (sGlobal->combatRound <= MAX_DIFFICULTY_ROUND) {
        range1 = MAP_DIFFICULTY[(sGlobal->combatRound-1)*3];
        range2 = MAP_DIFFICULTY[(sGlobal->combatRound-1)*3+1];
        //        range3 = MAP_DIFFICULTY[(sGlobal->combatRound-1)*3+2];
        
    } else {
        range1 = MAP_DIFFICULTY[(MAX_DIFFICULTY_ROUND-1)*3];
        range2 = MAP_DIFFICULTY[(MAX_DIFFICULTY_ROUND-1)*3+1];
        //        range3 = MAP_DIFFICULTY[(MAX_DIFFICULTY_ROUND-1)*3+2];
    }
    //    LOGE("range1:%d, range2:%d, range3:%d", range1, range2, range3);
    if (prob <= range1) {
        seed = 0;
    }
    else if (prob > range1 && prob <= range1+range2) {
        seed = 1;
    }
    else if (prob > range1+range2) {
        seed = 2;
    } else {
        seed = 0;
    }
    sGlobal->currentMapDifficulty = seed;
    //    LOGE("select difficulty %d map...", seed);
    
    
    switch (seasonId) {
        case 1:
            do {
                switch (seed) {
                    case 0:
                        mapIndex = randRangeInt(0, 8);
                        break;
                    case 1:
                        mapIndex = randRangeInt(10, 16);
                        break;
                    case 2:
                        mapIndex = randRangeInt(20, 25);
                        break;
                    default:
                        mapIndex = randRangeInt(0, 8);
                        break;
                }
            } while (mapIndex == curMapIndex || mapIndex == prevMapIndex);
            idx = mapIndex;
            prevMapIndex = curMapIndex;
            curMapIndex = idx;
            //            LOGE("idx11:%d", idx);
            return m_TileMapCache1[idx];
            break;
        case 2:
            do {
                switch (seed) {
                    case 0:
                        mapIndex = randRangeInt(0, 7);
                        break;
                    case 1:
                        mapIndex = randRangeInt(10, 16);
                        break;
                    case 2:
                        mapIndex = randRangeInt(20, 26);
                        break;
                    default:
                        mapIndex = randRangeInt(0, 7);
                        break;
                }
            } while (mapIndex == curMapIndex);
            idx = mapIndex;
            prevMapIndex = curMapIndex;
            curMapIndex = idx;
            //            LOGE("idx22:%d", idx);
            return m_TileMapCache2[idx];
            break;
        case 3:
            do {
                switch (seed) {
                    case 0:
                        mapIndex = randRangeInt(0, 6);
                        break;
                    case 1:
                        mapIndex = randRangeInt(10, 17);
                        break;
                    case 2:
                        mapIndex = randRangeInt(20, 27);
                        break;
                    default:
                        mapIndex = randRangeInt(0, 6);
                        break;
                }
            } while (mapIndex == curMapIndex);
            idx = mapIndex;
            prevMapIndex = curMapIndex;
            curMapIndex = idx;
            //            LOGE("idx33:%d", idx);
            return m_TileMapCache3[idx];
            break;
        default:
            return NULL;
            break;
    }
}

wyTMXLayer* Global::getMapFront(int seasonId, int idx) {
    switch (seasonId) {
        case 1:
            return m_TileLayerCache1[idx];
            break;
        case 2:
            return m_TileLayerCache2[idx];
            break;
        case 3:
            return m_TileLayerCache3[idx];
            break;
        default:
            return m_TileLayerCache1[idx];
            break;
    }
}

wyTMXTileMap* Global::getSEMap(int seasonId) {
    int index = 100;
    switch (seasonId) {
        case 1:
            return m_TileMapCache1[index];
            break;
        case 2:
            return m_TileMapCache2[index];
            break;
        case 3:
            return m_TileMapCache3[index];
            break;
        default:
            return m_TileMapCache1[index];
            break;
    }
}

wyTMXLayer* Global::getSEMapFront(int seasonId) {
    int index = 100;
    switch (seasonId) {
        case 1:
            return m_TileLayerCache1[index];
            break;
        case 2:
            return m_TileLayerCache2[index];
            break;
        case 3:
            return m_TileLayerCache3[index];
            break;
        default:
            return m_TileLayerCache1[index];
            break;
    }
}

void Global::initEndlessMap(int difficulty) {
    LOGE("set endless difficulty:%d", difficulty);
    sGlobal->elapsedNormalCombat = 1;
    sGlobal->combatPeriod = 1;
    
    switch (difficulty) {
        case DIFFICULTY_NORMAL:
            sGlobal->combatRound = 1;
            break;
        case DIFFICULTY_HEROIC:
            sGlobal->combatRound = 2;
            break;
        case DIFFICULTY_HARD:
            sGlobal->combatRound = 3;
            break;
        default:
            break;
    }
    
    sGlobal->seasonSeq = POSSIBLE_COMBO[rand()%6];
    sGlobal->bossSeq = POSSIBLE_COMBO[rand()%6];
    sGlobal->currentSeasonId = sGlobal->seasonSeq/100;
    sGlobal->currentBossId = sGlobal->bossSeq/100;
    
    sGlobal->accDistance = 0;
    sGlobal->isInBossBattle = false;
    sGlobal->isBossFighted = false;
    sGlobal->curMapIndex = -1;
    sGlobal->isRealEndless = true;
    
    sGlobal->index = sGlobal->currentSeasonId*100;
    
    sGlobal->calcCurrentCombatValue();
    if (sGlobal->combatValueToEnemyIdMap != NULL) {
        delete sGlobal->combatValueToEnemyIdMap;
    }
    sGlobal->combatValueToEnemyIdMap = NULL;
    sGlobal->calcEnemyCombatValue();
    
    isBossFighted = true;
}

void Global::calcCurrentCombatValue() {
    if (sGlobal->combatRound <= MAX_COMBAT_ROUND) {
        sGlobal->currentCombatValue = ROUND_COMBAT[(sGlobal->combatRound-1)*3 + sGlobal->combatPeriod - 1];
    } else {
        sGlobal->currentCombatValue = ROUND_COMBAT[(MAX_COMBAT_ROUND-1)*3 + sGlobal->combatPeriod - 1] * pow(COMBAT_POWER, sGlobal->combatRound - MAX_COMBAT_ROUND);
    }
//    //boss关中，出兵点战斗力减半
//    if (sGlobal->isInBossBattle) {
//        sGlobal->currentCombatValue /= 2;
//    }
    //    LOGE("currentCombatValue:%d", sGlobal->currentCombatValue);
}

int Global::calcAppearChance() {
    int percent;
    if (sGlobal->combatRound <= MAX_APPEAR_ROUND) {
        percent = APPEAR_CHANCE[(sGlobal->combatRound-1)*3 + sGlobal->combatPeriod - 1];
    } else {
        percent = APPEAR_CHANCE[(MAX_COMBAT_ROUND-1)*3 + sGlobal->combatPeriod - 1];
    }
    //boss关中，出兵点出现几率减半
    if (sGlobal->isInBossBattle) {
        percent /= 2;
    }
    if (sGlobal->combatRound == 1) {
        if (sGlobal->currentMapDifficulty == 3) {
            percent /= 2;
        }
        else if (sGlobal->currentMapDifficulty == 2) {
            percent /= 1.5f;
        }
    }
    //    LOGE("AppearChance:%d", percent);
    return percent;
}

void Global::saveDataInGame() {
    MyPrefs::setInt(DB_KILLED_ENEMY, sGlobal->killedEnemies);
    MyPrefs::setInt(DB_TOTAL_KILLED_ENEMY, sGlobal->totalKilledEnemies);
    MyPrefs::setInt(DB_KILLED_BOSSES, sGlobal->killedBosses);
    MyPrefs::setInt(DB_TOTAL_COLLECTED_COINS, sGlobal->totalCollectedCoins);
    
    if (isRealEndless) {
        if (!noNeedToSaveRunDist) {
            sGlobal->runDistance += sGlobal->currentDistance;
            MyPrefs::setInt(DB_RUN_DISTANCE, sGlobal->runDistance);
        } else {
            noNeedToSaveRunDist = false;
        }
        
        sGlobal->totalRunDistance += sGlobal->currentDistance;
        MyPrefs::setInt(DB_TOTAL_RUN_DISTANCE, sGlobal->totalRunDistance);

        sGlobal->arrayToStr(DB_ACHV_REC, sGlobal->achv_rec, ARR_LEN_ACHV_NUM);
        sGlobal->arrayToStr(DB_ACHV_PROGRESS, sGlobal->achv_progress, ARR_LEN_ACHV_NUM);
    }
//    LOGE("targetDistance:%d", sGlobal->achv_progress[ACHV_RUN_1000]);
}

void Global::initAllWeaponInfo() {
    if (weapon_atk != NULL) {
        delete[] weapon_atk;
    }
    if (weapon_gold != NULL) {
        delete[] weapon_gold;
    }
    if (weapon_zuanshi != NULL) {
        delete[] weapon_zuanshi;
    }
    if (weapon_firecd != NULL) {
        delete[] weapon_firecd;
    }
    if (weapon_ammo_gold != NULL) {
        delete[] weapon_ammo_gold;
    }
    weapon_atk = new float[ARR_LEN_WEAPON_NUM];
    weapon_gold = new int[ARR_LEN_WEAPON_NUM];
    weapon_zuanshi = new int[ARR_LEN_WEAPON_NUM];
    weapon_firecd = new float[ARR_LEN_WEAPON_NUM];
    weapon_ammo_gold = new int[ARR_LEN_WEAPON_NUM];
    
    for (int i = 0; i <= 15; i++) {
        initWeaponInfo(i);
    }
}

void Global::initWeaponInfo(int weaponIndex) {
    if (weaponUnlockArray[weaponIndex].getValue() == 0) {
        weapon_atk[weaponIndex] = WEAPON_PARAM[weaponIndex*8+WP_ATK_BASE] + WEAPON_PARAM[weaponIndex*8+WP_ATK_INC];
        weapon_gold[weaponIndex] = WEAPON_PARAM[weaponIndex*8+WP_ULK_GOLD];
        weapon_zuanshi[weaponIndex] = WEAPON_PARAM[weaponIndex*8+WP_ULK_CASH];
    } else {
        int grade = weaponUnlockArray[weaponIndex].getValue();
        weapon_atk[weaponIndex] = WEAPON_PARAM[weaponIndex*8+WP_ATK_BASE] + WEAPON_PARAM[weaponIndex*8+WP_ATK_INC]*grade;
        weapon_gold[weaponIndex] = (WEAPON_GOLD_BASE + WEAPON_GOLD_INC * grade*(grade+1)/2) * WEAPON_PARAM[weaponIndex*8+WP_GOLD_MULTI];
        weapon_zuanshi[weaponIndex] = 0;
    }
    
    weapon_firecd[weaponIndex] = 0.3f * WEAPON_PARAM[0+WP_SPEED] / WEAPON_PARAM[weaponIndex*8+WP_SPEED];
    weapon_ammo_gold[weaponIndex] = WEAPON_PARAM[weaponIndex*8+WP_AMMO_COST] * WEAPON_PARAM[weaponIndex*8+WP_AMMO_AMOUNT];
    
//    LOGE("index:%d; atk:%f; gold:%d; zuanshi:%d; firecd:%f; ammoamount:%d; ammogold:%d;", weaponIndex, weapon_atk[weaponIndex], weapon_gold[weaponIndex], weapon_zuanshi[weaponIndex], weapon_firecd[weaponIndex], (int)WEAPON_PARAM[weaponIndex*8+WP_AMMO_AMOUNT], weapon_ammo_gold[weaponIndex]);
}

void Global::initAllSkillEffect() {
    if (skillMulti != NULL) {
        delete[] skillMulti;
    }
    if (hasSkillMulti != NULL) {
        delete[] hasSkillMulti;
    }
    if (skill_gold != NULL) {
        delete[] skill_gold;
    }
    if (skill_cash != NULL) {
        delete[] skill_cash;
    }
    skillMulti = new float[11];
    hasSkillMulti = new bool[11];
    skill_gold = new int[11];
    skill_cash = new int[11];
    
    for (int i = 0; i < 10; i++) {
        initSkillEffect(i);
    }
}

void Global::initSkillEffect(int skillIndex) {
    if (skillArray[skillIndex].getValue() == 0) {
        hasSkillMulti[skillIndex] = false;
        skillMulti[skillIndex] = 0;
        if (skillIndex == SKILL_SHIELD) {
            skillMulti[SKILL_STUN+1] = 0;
        }
    } else {
        hasSkillMulti[skillIndex] = true;
        switch (skillIndex) {
            case SKILL_MISSLE:
//                skillMulti[skillIndex] = SKILL_MISSLE_BASE_ATK+SKILLS_PARAMS[skillIndex]*(skillArray[skillIndex]-1);
                if (skillArray[skillIndex].getValue() <= 10) {
                    skillMulti[skillIndex] = SKILL_MISSLE_BASE_ATK+ROCKET_ATK_INC_0*(skillArray[skillIndex].getValue());
                }
                else if (skillArray[skillIndex].getValue() > 10 && skillArray[skillIndex].getValue() <= 20) {
                    skillMulti[skillIndex] = SKILL_MISSLE_BASE_ATK+ROCKET_ATK_INC_0*10+ROCKET_ATK_INC_1*(skillArray[skillIndex].getValue()-10);
                }
                else if (skillArray[skillIndex].getValue() > 20 && skillArray[skillIndex].getValue() <= 30) {
                    skillMulti[skillIndex] = SKILL_MISSLE_BASE_ATK+ROCKET_ATK_INC_0*10+ROCKET_ATK_INC_1*10+ROCKET_ATK_INC_2*(skillArray[skillIndex].getValue()-20);
                }
                break;
            case SKILL_ROBOT:
                skillMulti[skillIndex] = SKILL_ROBOT_BASE_ATK+SKILLS_PARAMS[skillIndex]*(skillArray[skillIndex].getValue());
                break;
            case SKILL_SHIELD:
                skillMulti[skillIndex] = SKILL_SHIELD_LAST_BASE+SKILLS_PARAMS[skillIndex]*(skillArray[skillIndex].getValue());
                skillMulti[SKILL_STUN+1] = SKILL_SHIELD_CD_BASE-SKILLS_PARAMS[SKILL_STUN+1]*(skillArray[SKILL_SHIELD].getValue());
                break;
            case SKILL_STAMINA:
                skillMulti[skillIndex] = SKILLS_PARAMS[skillIndex]*skillArray[skillIndex].getValue();
                cloth_hp = HERO_BASE_HP + skillMulti[skillIndex];
                break;
            default:
                skillMulti[skillIndex] = SKILLS_PARAMS[skillIndex]*skillArray[skillIndex].getValue();
                break;
        }
    }
    
    int grade = skillArray[skillIndex].getValue();
    skill_gold[skillIndex] = SKILL_GOLD_BASE + SKILL_GOLD_INC * grade*(grade+1)/2;
    skill_cash[skillIndex] = (SKILL_GOLD_BASE + SKILL_GOLD_INC * grade*(grade+1)/2)/200.0f + 0.5f;
    
//    LOGE("index:%d, flag:%s, multi: %f", skillIndex, hasSkillMulti[skillIndex] ? "true" : "false", skillMulti[skillIndex]);
}

void Global::initMissionIds() {
    missionId1 = 0;
    missionId2 = 1;
    missionId3 = 2;
    
    MyPrefs::setInt(DB_MISSION1_ID, missionId1);
    MyPrefs::setInt(DB_MISSION2_ID, missionId2);
    MyPrefs::setInt(DB_MISSION3_ID, missionId3);
    
    achv_progress[ACHV_SEQ[missionId1]] = 0;
    achv_progress[ACHV_SEQ[missionId2]] = 0;
    achv_progress[ACHV_SEQ[missionId3]] = 0;
    
    achv_progress_old[ACHV_SEQ[missionId1]] = 0;
    achv_progress_old[ACHV_SEQ[missionId2]] = 0;
    achv_progress_old[ACHV_SEQ[missionId3]] = 0;
    
    achv_progress_temp[ACHV_SEQ[missionId1]] = 0;
    achv_progress_temp[ACHV_SEQ[missionId2]] = 0;
    achv_progress_temp[ACHV_SEQ[missionId3]] = 0;
    
    achv_rec[ACHV_SEQ[missionId1]] = ACHV_ACTIVE;
    achv_rec[ACHV_SEQ[missionId2]] = ACHV_ACTIVE;
    achv_rec[ACHV_SEQ[missionId3]] = ACHV_ACTIVE;
}

int Global::getMinRepeatableMId(int currentMaxId) {
    int nextMId = currentMaxId + 1;
    
    if (nextMId < 60) {
//        LOGE("nextMId:%d", nextMId);
        return nextMId;
    } else {
        while (true) {
            int tempId = rand()%8+52;
            if (tempId != missionId1 && tempId != missionId2 && tempId != missionId3) {
                sGlobal->achv_rec[ACHV_SEQ[tempId]] = ACHV_ACTIVE;
//                LOGE("tempId:%d", tempId);
                return tempId;
            }
//            if (sGlobal->achv_rec[ACHV_SEQ[tempId]] == ACHV_INACTIVE) {
//                sGlobal->achv_rec[ACHV_SEQ[tempId]] = ACHV_ACTIVE;
//                LOGE("tempId:%d", tempId);
//                return tempId;
//            }
        }
    }
}

void Global::updateOldMissionStatus(int mid) {
    switch (mid) {
        case 51:
        case 52:
        case 54:
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:
            sGlobal->achv_rec[ACHV_SEQ[mid]] = ACHV_FINISHED;
            break;
        default:
            sGlobal->achv_rec[ACHV_SEQ[mid]] = ACHV_FINISHED;
            break;
    }
}

void Global::updateNewMissionStatus(int mid) {
//    LOGE("sharyu mid:%d, realMid:%d", mid, ACHV_SEQ[mid]);
    switch (ACHV_SEQ[mid]) {
        case ACHV_RUN_1000:
        case ACHV_RUN_5000:
        case ACHV_RUN_20000:
        case ACHV_RUN_50000:
        case ACHV_RUN_100000:
            noNeedToSaveRunDist = true;
            sGlobal->runDistance = 0;
            MyPrefs::setInt(DB_RUN_DISTANCE, 0);
//            LOGE("sharyu:aaa");
            break;
        default:
            break;
    }
    
    achv_progress[ACHV_SEQ[mid]] = 0;
    achv_progress_old[ACHV_SEQ[mid]] = 0;
    achv_progress_temp[ACHV_SEQ[mid]] = 0;
}

void Global::refreshMIds() {
    int oldMId1 = missionId1;
    int oldMId2 = missionId2;
    int oldMId3 = missionId3;
    
    int currentMaxId = MAX(missionId1, MAX(missionId2, missionId3));
    int nextMId;
    
    achv_progress_temp[ACHV_SEQ[missionId1]] = 0;
    achv_progress_temp[ACHV_SEQ[missionId2]] = 0;
    achv_progress_temp[ACHV_SEQ[missionId3]] = 0;
    
    if (sGlobal->achv_rec[ACHV_SEQ[missionId1]] == ACHV_FINISHED) {
        nextMId = getMinRepeatableMId(currentMaxId);
        sGlobal->achv_progress[ACHV_SEQ[nextMId]] = 0;
        missionId1 = nextMId;
        currentMaxId = missionId1;
        
        updateOldMissionStatus(oldMId1);
        updateNewMissionStatus(missionId1);
        
        achv_rec[ACHV_SEQ[missionId1]] = ACHV_ACTIVE;
    }
    
    if (sGlobal->achv_rec[ACHV_SEQ[missionId2]] == ACHV_FINISHED) {
        nextMId = getMinRepeatableMId(currentMaxId);
        sGlobal->achv_progress[ACHV_SEQ[nextMId]] = 0;
        missionId2 = nextMId;
        currentMaxId = missionId2;
        
        updateOldMissionStatus(oldMId2);
        updateNewMissionStatus(missionId2);
        //        achv_progress_old[ACHV_SEQ[missionId2]] = achv_progress[ACHV_SEQ[missionId2]];
        
        achv_rec[ACHV_SEQ[missionId2]] = ACHV_ACTIVE;
    }
    
    if (sGlobal->achv_rec[ACHV_SEQ[missionId3]] == ACHV_FINISHED) {
        nextMId = getMinRepeatableMId(currentMaxId);
        sGlobal->achv_progress[ACHV_SEQ[nextMId]] = 0;
        missionId3 = nextMId;
        currentMaxId = missionId3;
        
        updateOldMissionStatus(oldMId3);
        updateNewMissionStatus(missionId3);
        //        achv_progress_old[ACHV_SEQ[missionId3]] = achv_progress[ACHV_SEQ[missionId3]];
        
        achv_rec[ACHV_SEQ[missionId3]] = ACHV_ACTIVE;
    }
    
    arrayToStr(DB_ACHV_REC, achv_rec, ARR_LEN_ACHV_NUM);
    arrayToStr(DB_ACHV_PROGRESS, achv_progress, ARR_LEN_ACHV_NUM);
    
    MyPrefs::setInt(DB_MISSION1_ID, missionId1);
    MyPrefs::setInt(DB_MISSION2_ID, missionId2);
    MyPrefs::setInt(DB_MISSION3_ID, missionId3);
}

void Global::testMId() {
    if (rand()%3 == 1) {
        sGlobal->achv_rec[missionId1] = ACHV_FINISHED;
//        LOGE("m1 finished...");
    }
    if (rand()%3 == 1) {
        sGlobal->achv_rec[missionId2] = ACHV_FINISHED;
//        LOGE("m2 finished...");
    }
    if (rand()%3 == 1) {
        sGlobal->achv_rec[missionId3] = ACHV_FINISHED;
//        LOGE("m3 finished...");
    }
    
    refreshMIds();
}

void Global::initConsts() {
    wyJSONObject* jo = wyJSONObject::make("data.json", false);
    
    ENEMY_ATK_MULTI = optFloat("ENEMY_ATK_MULTI", jo);
    ENEMY_HP_MULTI = optFloat("ENEMY_HP_MULTI", jo);
    ENEMY_GOLD_MULTI = optFloat("ENEMY_GOLD_MULTI", jo);
    
    SMALL_COIN_GOLD = optInt("SMALL_COIN_GOLD", jo);
    MIDDLE_COIN_GOLD = optInt("MIDDLE_COIN_GOLD", jo);
    LARGE_COIN_GOLD = optInt("LARGE_COIN_GOLD", jo);
    
    WEAPON_MAX_LEVEL = optInt("WEAPON_MAX_LEVEL", jo);
    WEAPON_GOLD_BASE = optInt("WEAPON_GOLD_BASE", jo);
    WEAPON_GOLD_INC = optInt("WEAPON_GOLD_INC", jo);
    
    SKILL_MAX_LEVEL = optInt("SKILL_MAX_LEVEL", jo);
    SKILL_GOLD_BASE = optInt("SKILL_GOLD_BASE", jo);
    SKILL_GOLD_INC = optInt("SKILL_GOLD_INC", jo);
    
    SKILL_MISSLE_BASE_ATK = optInt("SKILL_MISSLE_BASE_ATK", jo);
    SKILL_MISSLE_CD = optFloat("SKILL_MISSLE_CD", jo);
    
    SKILL_ROBOT_BASE_ATK = optInt("SKILL_ROBOT_BASE_ATK", jo);
    SKILL_ROBOT_FIRE_RATE = optInt("SKILL_ROBOT_FIRE_RATE", jo);
    SKILL_ROBOT_LAST = optFloat("SKILL_ROBOT_LAST", jo);
    SKILL_ROBOT_CD = optFloat("SKILL_ROBOT_CD", jo);
    
    SKILL_SHIELD_LAST_BASE = optFloat("SKILL_SHIELD_LAST_BASE", jo);
    SKILL_SHIELD_CD_BASE = optFloat("SKILL_SHIELD_CD_BASE", jo);
    
    SKILL_CRITICAL_MULTI = optFloat("SKILL_CRITICAL_MULTI", jo);
    SKILL_STUN_DUR = optFloat("SKILL_STUN_DUR", jo);
    
    HERO_BASE_HP = optInt("HERO_BASE_HP", jo);
    HERO_BASE_DEF = optInt("HERO_BASE_DEF", jo);
    
    wyJSONArray* jsonArray = optJSONArray("WEAPON_PARAM", jo);
    int len = jsonArray->getLength();
    WEAPON_PARAM = new float[len];
    memset(WEAPON_PARAM, 0, sizeof(float)*len);
    fillJsonFloatArray(jsonArray, WEAPON_PARAM, len);
    
    jsonArray = optJSONArray("WEAPON_ID2SEQ", jo);
    len = jsonArray->getLength();
    WEAPON_ID2SEQ = new int[len];
    memset(WEAPON_ID2SEQ, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, WEAPON_ID2SEQ, len);
    
    WEAPON_SEQ2ID = new int[len];
    memset(WEAPON_SEQ2ID, 0, sizeof(int)*len);
    for (int i = 0; i < len; i++) {
        WEAPON_SEQ2ID[WEAPON_ID2SEQ[i]] = i;
    }
//    for (int i = 0; i < len; i++) {
//        LOGE("WEAPON_SEQ2ID:%d", WEAPON_SEQ2ID[i]);
//    }
    
    //reward数组
    jsonArray = optJSONArray("Reward_Number", jo);
    len = jsonArray->getLength();
    Reward_Number = new int[len];
    memset(Reward_Number, 0, sizeof(float)*len);
    fillJsonIntArray(jsonArray,Reward_Number, len);
    jsonArray = optJSONArray("Reward_Unit", jo);
    len = jsonArray->getLength();
    Reward_Unit = new int[len];
    memset(Reward_Unit, 0, sizeof(float)*len);
    fillJsonIntArray(jsonArray,Reward_Unit, len);
    
    jsonArray = optJSONArray("SKILLS_PARAMS", jo);
    len = jsonArray->getLength();
    SKILLS_PARAMS = new float[len];
    memset(SKILLS_PARAMS, 0, sizeof(float)*len);
    fillJsonFloatArray(jsonArray, SKILLS_PARAMS, len);
    
    jsonArray = optJSONArray("ACHV_NUM", jo);
    len = jsonArray->getLength();
    ACHV_NUM = new int[len];
    memset(ACHV_NUM, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ACHV_NUM, len);
    
    jsonArray = optJSONArray("ACHV_SEQ", jo);
    len = jsonArray->getLength();
    ACHV_SEQ = new int[len];
    memset(ACHV_SEQ, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ACHV_SEQ, len);
    
    LOGE("ACHV:");
    for (int i = 0; i < len; i++) {
        LOGE("%d", ACHV_SEQ[i]);
    }
    LOGE("ACHV end");
    
    MAX_COMBAT_TIME = optInt("MAX_COMBAT_TIME", jo);
    
    MAX_DIFFICULTY_ROUND = optInt("MAX_DIFFICULTY_ROUND", jo);
    jsonArray = optJSONArray("MAP_DIFFICULTY", jo);
    len = jsonArray->getLength();
    MAP_DIFFICULTY = new int[len];
    memset(MAP_DIFFICULTY, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, MAP_DIFFICULTY, len);
    
    jsonArray = optJSONArray("MAP_DIFFICULTY_BOSS", jo);
    len = jsonArray->getLength();
    MAP_DIFFICULTY_BOSS = new int[len];
    memset(MAP_DIFFICULTY_BOSS, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, MAP_DIFFICULTY_BOSS, len);
    
    MAX_COMBAT_ROUND = optInt("MAX_COMBAT_ROUND", jo);
    jsonArray = optJSONArray("ROUND_COMBAT", jo);
    len = jsonArray->getLength();
    ROUND_COMBAT = new int[len];
    memset(ROUND_COMBAT, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ROUND_COMBAT, len);
    
    COMBAT_POWER = optFloat("COMBAT_POWER", jo);
    COMBAT_VAR_MIN = optInt("COMBAT_VAR_MIN", jo);
    COMBAT_VAR_MAX = optInt("COMBAT_VAR_MAX", jo);
    
    MAX_APPEAR_ROUND = optInt("MAX_APPEAR_ROUND", jo);
    jsonArray = optJSONArray("APPEAR_CHANCE", jo);
    len = jsonArray->getLength();
    APPEAR_CHANCE = new int[len];
    memset(APPEAR_CHANCE, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, APPEAR_CHANCE, len);
    
    ENEMY_COMBAT_POWER = optFloat("ENEMY_COMBAT_POWER", jo);
    BOSS_ATK_INTERVAL_MULTI = optFloat("BOSS_ATK_INTERVAL_MULTI", jo);
    BOSS_STUN_TIME_MULTI = optFloat("BOSS_STUN_TIME_MULTI", jo);
    
    jsonArray = optJSONArray("ENEMY_GOLD_EASY", jo);
    len = jsonArray->getLength();
    ENEMY_GOLD_EASY = new int[len];
    memset(ENEMY_GOLD_EASY, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_GOLD_EASY, len);
    
    jsonArray = optJSONArray("ENEMY_HP_EASY", jo);
    len = jsonArray->getLength();
    ENEMY_HP_EASY = new int[len];
    memset(ENEMY_HP_EASY, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_HP_EASY, len);
    
    jsonArray = optJSONArray("ENEMY_HP_EASY_INC", jo);
    len = jsonArray->getLength();
    ENEMY_HP_EASY_INC = new int[len];
    memset(ENEMY_HP_EASY_INC, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_HP_EASY_INC, len);
    
    jsonArray = optJSONArray("ENEMY_ATK_EASY", jo);
    len = jsonArray->getLength();
    ENEMY_ATK_EASY = new int[len];
    memset(ENEMY_ATK_EASY, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_ATK_EASY, len);
    
    
    jsonArray = optJSONArray("ENEMY_COMBAT_VALUE_EASY", jo);
    len = jsonArray->getLength();
    ENEMY_COMBAT_VALUE_EASY = new int[len];
    memset(ENEMY_COMBAT_VALUE_EASY, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_COMBAT_VALUE_EASY, len);
    
    jsonArray = optJSONArray("ENEMY_COMBAT_VALUE_HARD", jo);
    len = jsonArray->getLength();
    ENEMY_COMBAT_VALUE_HARD = new int[len];
    memset(ENEMY_COMBAT_VALUE_HARD, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_COMBAT_VALUE_HARD, len);
    
    jsonArray = optJSONArray("ENEMY_COMBAT_VALUE_CRAZY", jo);
    len = jsonArray->getLength();
    ENEMY_COMBAT_VALUE_CRAZY = new int[len];
    memset(ENEMY_COMBAT_VALUE_CRAZY, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_COMBAT_VALUE_CRAZY, len);
    
    jsonArray = optJSONArray("ENEMY_GOLD_HARD", jo);
    len = jsonArray->getLength();
    ENEMY_GOLD_HARD = new int[len];
    memset(ENEMY_GOLD_HARD, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_GOLD_HARD, len);
    jsonArray = optJSONArray("ENEMY_GOLD_CRAZY", jo);
    len = jsonArray->getLength();
    ENEMY_GOLD_CRAZY = new int[len];
    memset(ENEMY_GOLD_CRAZY, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_GOLD_CRAZY, len);
    
    jsonArray = optJSONArray("ENEMY_HP_HARD", jo);
    len = jsonArray->getLength();
    ENEMY_HP_HARD = new int[len];
    memset(ENEMY_HP_HARD, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_HP_HARD, len);
    
    jsonArray = optJSONArray("ENEMY_HP_HARD_INC", jo);
    len = jsonArray->getLength();
    ENEMY_HP_HARD_INC = new int[len];
    memset(ENEMY_HP_HARD_INC, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_HP_HARD_INC, len);
    
    jsonArray = optJSONArray("ENEMY_HP_CRAZY", jo);
    len = jsonArray->getLength();
    ENEMY_HP_CRAZY = new int[len];
    memset(ENEMY_HP_CRAZY, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_HP_CRAZY, len);
    
    jsonArray = optJSONArray("ENEMY_HP_CRAZY_INC", jo);
    len = jsonArray->getLength();
    ENEMY_HP_CRAZY_INC = new int[len];
    memset(ENEMY_HP_CRAZY_INC, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_HP_CRAZY_INC, len);
    
    jsonArray = optJSONArray("ENEMY_ATK_HARD", jo);
    len = jsonArray->getLength();
    ENEMY_ATK_HARD = new int[len];
    memset(ENEMY_ATK_HARD, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_ATK_HARD, len);
    jsonArray = optJSONArray("ENEMY_ATK_CRAZY", jo);
    len = jsonArray->getLength();
    ENEMY_ATK_CRAZY = new int[len];
    memset(ENEMY_ATK_CRAZY, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_ATK_CRAZY, len);

    jsonArray = optJSONArray("BOSS1_CAIDAO_ATK", jo);
    len = jsonArray->getLength();
    BOSS1_CAIDAO_ATK = new int[len];
    memset(BOSS1_CAIDAO_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS1_CAIDAO_ATK, len);
    jsonArray = optJSONArray("BOSS1_KAN_ATK", jo);
    len = jsonArray->getLength();
    BOSS1_KAN_ATK = new int[len];
    memset(BOSS1_KAN_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS1_KAN_ATK, len);
    jsonArray = optJSONArray("BOSS1_XULI_LIANDAO_HP", jo);
    len = jsonArray->getLength();
    BOSS1_XULI_LIANDAO_HP = new int[len];
    memset(BOSS1_XULI_LIANDAO_HP, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS1_XULI_LIANDAO_HP, len);
    jsonArray = optJSONArray("BOSS1_XULI_LIANDAO_ATK", jo);
    len = jsonArray->getLength();
    BOSS1_XULI_LIANDAO_ATK = new int[len];
    memset(BOSS1_XULI_LIANDAO_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS1_XULI_LIANDAO_ATK, len);
    jsonArray = optJSONArray("BOSS1_HUIXUAN_CAIDAO_HP", jo);
    len = jsonArray->getLength();
    BOSS1_HUIXUAN_CAIDAO_HP = new int[len];
    memset(BOSS1_HUIXUAN_CAIDAO_HP, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS1_HUIXUAN_CAIDAO_HP, len);
    jsonArray = optJSONArray("BOSS1_HUIXUAN_CAIDAO_ATK", jo);
    len = jsonArray->getLength();
    BOSS1_HUIXUAN_CAIDAO_ATK = new int[len];
    memset(BOSS1_HUIXUAN_CAIDAO_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS1_HUIXUAN_CAIDAO_ATK, len);

    jsonArray = optJSONArray("BOSS1_SMALL_CAIDAO_ATK", jo);
    len = jsonArray->getLength();
    BOSS1_SMALL_CAIDAO_ATK = new int[len];
    memset(BOSS1_SMALL_CAIDAO_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS1_SMALL_CAIDAO_ATK, len);
    jsonArray = optJSONArray("BOSS1_SMALL_KAN_ATK", jo);
    len = jsonArray->getLength();
    BOSS1_SMALL_KAN_ATK = new int[len];
    memset(BOSS1_SMALL_KAN_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS1_SMALL_KAN_ATK, len);
    jsonArray = optJSONArray("BOSS1_SMALL_HP", jo);
    len = jsonArray->getLength();
    BOSS1_SMALL_HP = new int[len];
    memset(BOSS1_SMALL_HP, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS1_SMALL_HP, len);
    jsonArray = optJSONArray("BOSS1_SMALL_GOLD", jo);
    len = jsonArray->getLength();
    BOSS1_SMALL_GOLD = new int[len];
    memset(BOSS1_SMALL_GOLD, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS1_SMALL_GOLD, len);
    
    jsonArray = optJSONArray("BOSS2_LANCE_ATK", jo);
    len = jsonArray->getLength();
    BOSS2_LANCE_ATK = new int[len];
    memset(BOSS2_LANCE_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS2_LANCE_ATK, len);
    jsonArray = optJSONArray("BOSS2_CHONGFENG_ATK", jo);
    len = jsonArray->getLength();
    BOSS2_CHONGFENG_ATK = new int[len];
    memset(BOSS2_CHONGFENG_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS2_CHONGFENG_ATK, len);
    jsonArray = optJSONArray("BOSS2_XULI_LASER_HP", jo);
    len = jsonArray->getLength();
    BOSS2_XULI_LASER_HP = new int[len];
    memset(BOSS2_XULI_LASER_HP, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS2_XULI_LASER_HP, len);
    jsonArray = optJSONArray("BOSS2_XULI_LASER_ATK", jo);
    len = jsonArray->getLength();
    BOSS2_XULI_LASER_ATK = new int[len];
    memset(BOSS2_XULI_LASER_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS2_XULI_LASER_ATK, len);
    jsonArray = optJSONArray("BOSS2_SMALL_LANCE_HP", jo);
    len = jsonArray->getLength();
    BOSS2_SMALL_LANCE_HP = new int[len];
    memset(BOSS2_SMALL_LANCE_HP, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS2_SMALL_LANCE_HP, len);
    jsonArray = optJSONArray("BOSS2_SMALL_LANCE_ATK", jo);
    len = jsonArray->getLength();
    BOSS2_SMALL_LANCE_ATK = new int[len];
    memset(BOSS2_SMALL_LANCE_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS2_SMALL_LANCE_ATK, len);

    jsonArray = optJSONArray("BOSS3_IRONFIST_ATK", jo);
    len = jsonArray->getLength();
    BOSS3_IRONFIST_ATK = new int[len];
    memset(BOSS3_IRONFIST_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS3_IRONFIST_ATK, len);
    jsonArray = optJSONArray("BOSS3_KNOCK_ATK", jo);
    len = jsonArray->getLength();
    BOSS3_KNOCK_ATK = new int[len];
    memset(BOSS3_KNOCK_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS3_KNOCK_ATK, len);
    jsonArray = optJSONArray("BOSS3_XULI_FIST_HP", jo);
    len = jsonArray->getLength();
    BOSS3_XULI_FIST_HP = new int[len];
    memset(BOSS3_XULI_FIST_HP, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS3_XULI_FIST_HP, len);
    jsonArray = optJSONArray("BOSS3_XULI_TARGET_HP", jo);
    len = jsonArray->getLength();
    BOSS3_XULI_TARGET_HP = new int[len];
    memset(BOSS3_XULI_TARGET_HP, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS3_XULI_TARGET_HP, len);
    jsonArray = optJSONArray("BOSS3_XULI_FIST_ATK", jo);
    len = jsonArray->getLength();
    BOSS3_XULI_FIST_ATK = new int[len];
    memset(BOSS3_XULI_FIST_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS3_XULI_FIST_ATK, len);
    jsonArray = optJSONArray("BOSS3_MISSLE_HP", jo);
    len = jsonArray->getLength();
    BOSS3_MISSLE_HP = new int[len];
    memset(BOSS3_MISSLE_HP, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS3_MISSLE_HP, len);
    jsonArray = optJSONArray("BOSS3_MISSLE_ATK", jo);
    len = jsonArray->getLength();
    BOSS3_MISSLE_ATK = new int[len];
    memset(BOSS3_MISSLE_ATK, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, BOSS3_MISSLE_ATK, len);

    ENDLESS_HP_POWER = optFloat("ENDLESS_HP_POWER", jo);
    ENDLESS_ATK_POWER = optFloat("ENDLESS_ATK_POWER", jo);
    ENDLESS_GOLD_POWER = optFloat("ENDLESS_GOLD_POWER", jo);
    
    ROCKET_ATK_INC_0 = optFloat("ROCKET_ATK_INC_0", jo);
    ROCKET_ATK_INC_1 = optFloat("ROCKET_ATK_INC_1", jo);
    ROCKET_ATK_INC_2 = optFloat("ROCKET_ATK_INC_2", jo);
    
    jsonArray = optJSONArray("ENEMY_COUNT", jo);
    len = jsonArray->getLength();
    ENEMY_COUNT = new int[len];
    memset(ENEMY_COUNT, 0, sizeof(int)*len);
    fillJsonIntArray(jsonArray, ENEMY_COUNT, len);
    
    HURT_CD = optFloat("HURT_CD", jo);
    SHOOT_CD_MULTI = optFloat("SHOOT_CD_MULTI", jo);
}

//void Global::initJsonIntArray(wyJSONObject* jo, const char *arrayName, int *array) {
//    wyJSONArray* jsonArray = jo->optJSONArray(arrayName);
//    int len = jsonArray->getLength();
//    array = new int[len];
//    memset(array, 0, sizeof(int)*len);
//    for (int i = 0; i < len; i++) {
//        array[i] = jsonArray->optInt(i);
//    }
//}
//
//void Global::initJsonFloatArray(wyJSONObject* jo, const char *arrayName, float *array) {
//    wyJSONArray* jsonArray = jo->optJSONArray(arrayName);
//    int len = jsonArray->getLength();
//    array = new float[len];
//    memset(array, 0, sizeof(float)*len);
//    for (int i = 0; i < len; i++) {
//        array[i] = jsonArray->optFloat(i);
//    }
//}
int Global::optInt(const char* key, wyJSONObject* jo) {
    int result = jo->optInt(key);
//    LOGE("%s:%d", key, result);
    return result;
}

float Global::optFloat(const char* key, wyJSONObject* jo) {
    float result = jo->optFloat(key);
//    LOGE("%s:%f", key, result);
    return result;
}

int Global::optInt(int index, wyJSONArray* jsonArray) {
    int result = jsonArray->optInt(index);
//    LOGE("%d:%d", index, result);
    return result;
}

float Global::optFloat(int index, wyJSONArray* jsonArray) {
    float result = jsonArray->optFloat(index);
//    LOGE("%d:%f", index, result);
    return result;
}

wyJSONArray* Global::optJSONArray(const char* key, wyJSONObject* jo) {
    wyJSONArray* result = jo->optJSONArray(key);
//    LOGE("%s:", key);
    return result;
}

void Global::fillJsonIntArray(wyJSONArray* jsonArray, int *array, int len) {
    for (int i = 0; i < len; i++) {
//        array[i] = jsonArray->optInt(i);
        array[i] = optInt(i, jsonArray);
    }
}

void Global::fillJsonFloatArray(wyJSONArray* jsonArray, float *array, int len) {
    for (int i = 0; i < len; i++) {
//        array[i] = jsonArray->optFloat(i);
        array[i] = optFloat(i, jsonArray);
    }
}

void Global::initCaches() {
    containerCache = wyArrayNew(200);
    //    for (int i = 0; i < 100; i++) {
    //        wyNode* containerSprite = wyNode::make();
    //        containerSprite->setVisible(false);
    //        containerSprite->retain();
    //        wyArrayPush(containerCache, containerSprite);
    //    }
    
    fleshSpriteCache = wyArrayNew(200);
    //    for (int i = 0; i < 100; i++) {
    //        wySpriteEx* fleshSprite = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "1_1.png");
    //        fleshSprite->setVisible(false);
    //        fleshSprite->retain();
    //        wyArrayPush(fleshSpriteCache, fleshSprite);
    //    }
    
    shadowSpriteCache = wyArrayNew(200);
    //    for (int i = 0; i < 100; i++) {
    //        wySpriteEx* shadowSprite = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shadow.png");
    //        shadowSprite->setVisible(false);
    //        shadowSprite->retain();
    //        wyArrayPush(shadowSpriteCache, shadowSprite);
    //    }
    
    bulletSpriteCache = wyArrayNew(200);
    //    for (int i = 0; i < 30; i++) {
    //        wySpriteEx* bulletSprite = wySpriteEx::make(MyTextureManager::makePNG("bullet.png"));
    //        bulletSprite->retain();
    //        wyArrayPush(bulletSpriteCache, bulletSprite);
    //    }
    
    enemy_0_cache = wyArrayNew(200);
    //    for (int i = 0; i < 50; i++) {
    //        wySPXSprite* enemy0Sprite = wySPXSprite::make("enemy_0.sprite", false, MyTextureManager::makePNG("enemy_0.png"), 0);
    //        enemy0Sprite->setForceTickMode(true);
    //        enemy0Sprite->setLoopCount(-1);
    //        enemy0Sprite->retain();
    //        wyArrayPush(enemy_0_cache, enemy0Sprite);
    //    }
    enemy_1_cache = wyArrayNew(200);
    //    for (int i = 0; i < 50; i++) {
    //        wySPXSprite* enemy1Sprite = wySPXSprite::make("enemy_1.sprite", false, MyTextureManager::makePNG("enemy_1.png"), 0);
    //        enemy1Sprite->setForceTickMode(true);
    //        enemy1Sprite->setLoopCount(-1);
    //        enemy1Sprite->retain();
    //        wyArrayPush(enemy_1_cache, enemy1Sprite);
    //    }
    enemy_2_cache = wyArrayNew(200);
    //    for (int i = 0; i < 50; i++) {
    //        wySPXSprite* enemy2Sprite = wySPXSprite::make("enemy_2.sprite", false, MyTextureManager::makePNG("enemy_2.png"), 0);
    //        enemy2Sprite->setForceTickMode(true);
    //        enemy2Sprite->setLoopCount(-1);
    //        enemy2Sprite->retain();
    //        wyArrayPush(enemy_2_cache, enemy2Sprite);
    //    }
    effects_cache = wyArrayNew(200);
    //    for (int i = 0; i < 100; i++) {
    //        wySPXSprite* effectsSprite = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), 0);
    //        effectsSprite->setForceTickMode(true);
    //        effectsSprite->setLoopCount(-1);
    //        effectsSprite->retain();
    //        wyArrayPush(effects_cache, effectsSprite);
    //    }
    
    coin_emitterArray = wyArrayNew(200);
    //    for (int i = 0; i < 20; i++) {
    //        wyParticleSystem *coinEmitter = wyParticleLoader::load("particle_coin.plist");
    //        coinEmitter->setScaleX(sGlobal->scaleX/0.75f);
    //        coinEmitter->setScaleY(sGlobal->scaleY/0.75f);
    //        coinEmitter->retain();
    //        wyArrayPush(coin_emitterArray, coinEmitter);
    //    }
    
    fire_emitterArray = wyArrayNew(200);
    //    for (int i = 0; i < 10; i++) {
    //        wyParticleSystem *fireEmitter = wyParticleLoader::load("particle_torch_fire.plist");
    //        fireEmitter->setScaleX(sGlobal->scaleX*0.5f);
    //        fireEmitter->setScaleY(sGlobal->scaleY*0.8f);
    //        fireEmitter->setPositionType(GROUPED);
    //        fireEmitter->retain();
    //        wyArrayPush(fire_emitterArray, fireEmitter);
    //    }
    
    smoke_emitterArray = wyArrayNew(200);
    //    for (int i = 0; i < 50; i++) {
    //        wyParticleSystem *smokeEmitter = wyParticleLoader::load("particle_rpg_rocket.plist");
    //        smokeEmitter->setScaleX(sGlobal->scaleX/0.75f);
    //        smokeEmitter->setScaleY(sGlobal->scaleY/0.75f);
    //        smokeEmitter->retain();
    //        wyArrayPush(smoke_emitterArray, smokeEmitter);
    //    }
    
    //    if (sGlobal->index == 110 || sGlobal->index == 104) {
    boss1Cache = wyArrayNew(200);
    //        for (int i = 0; i < 10; i++) {
    //            wySPXSprite* bossSprite = wySPXSprite::make("boss1.sprite", false, MyTextureManager::makePNG("boss1.png"), 0);
    //            bossSprite->setForceTickMode(true);
    //            bossSprite->setLoopCount(-1);
    //            bossSprite->retain();
    //            wyArrayPush(boss1Cache, bossSprite);
    //        }
    //    }
    
    boss2Cache = wyArrayNew(200);
    
    boss3Cache = wyArrayNew(200);
    
    dian_cache = wyArrayNew(200);
    
    xuecaoCache = wyArrayNew(100);
}

void Global::resetCaches() {
//    LOGE("reset caches...");
    int curLen = containerCache->num;
    if (curLen < 100) {
//        LOGE("containerCache:%d", curLen);
        for (int i = curLen; i < 100; i++) {
            createSingleNodeForCache(containerCache);
        }
    }
    
    curLen = fleshSpriteCache->num;
    if (curLen < 100) {
//        LOGE("fleshSpriteCache:%d", curLen);
        for (int i = curLen; i < 100; i++) {
            createSingleNodeForCache(fleshSpriteCache);
        }
    }
    
    curLen = shadowSpriteCache->num;
    if (curLen < 100) {
//        LOGE("shadowSpriteCache:%d", curLen);
        for (int i = curLen; i < 100; i++) {
            createSingleNodeForCache(shadowSpriteCache);
        }
    }
    
    curLen = bulletSpriteCache->num;
    if (curLen < 30) {
//        LOGE("bulletSpriteCache:%d", curLen);
        for (int i = curLen-1; i < 30; i++) {
            createSingleNodeForCache(bulletSpriteCache);
        }
    }
    
    curLen = enemy_0_cache->num;
    if (curLen < 50) {
//        LOGE("enemy_0_cache:%d", curLen);
        for (int i = curLen; i < 50; i++) {
            createSingleNodeForCache(enemy_0_cache);
        }
    }
    
    curLen = enemy_1_cache->num;
    if (curLen < 50) {
//        LOGE("enemy_1_cache:%d", curLen);
        for (int i = curLen-1; i < 50; i++) {
            createSingleNodeForCache(enemy_1_cache);
        }
    }
    
    curLen = enemy_2_cache->num;
    if (curLen < 50) {
//        LOGE("enemy_2_cache:%d", curLen);
        for (int i = curLen; i < 50; i++) {
            createSingleNodeForCache(enemy_2_cache);
        }
    }
    
    curLen = effects_cache->num;
    if (curLen < 100) {
//        LOGE("effects_cache:%d", curLen);
        for (int i = curLen; i < 100; i++) {
            createSingleNodeForCache(effects_cache);
        }
    }
    
    curLen = coin_emitterArray->num;
    if (curLen < 20) {
//        LOGE("coin_emitterArray:%d", curLen);
        for (int i = curLen; i < 20; i++) {
            createSingleNodeForCache(coin_emitterArray);
        }
    }
    
    curLen = fire_emitterArray->num;
    if (curLen < 10) {
//        LOGE("fire_emitterArray:%d", curLen);
        for (int i = curLen; i < 10; i++) {
            createSingleNodeForCache(fire_emitterArray);
        }
    }
    
    curLen = smoke_emitterArray->num;
    if (curLen < 50) {
//        LOGE("smoke_emitterArray:%d", curLen);
        for (int i = curLen; i < 50; i++) {
            createSingleNodeForCache(smoke_emitterArray);
        }
    }
    
    //    if (sGlobal->index == 110 || sGlobal->index == 104) {
    curLen = boss1Cache->num;
    if (curLen < 20) {
//        LOGE("boss1Cache:%d", curLen);
        for (int i = curLen; i < 20; i++) {
            createSingleNodeForCache(boss1Cache);
        }
    }
    //    }
    
    curLen = boss2Cache->num;
    if (curLen < 20) {
//        LOGE("boss2Cache:%d", curLen);
        for (int i = curLen; i < 20; i++) {
            createSingleNodeForCache(boss2Cache);
        }
    }
    
    curLen = boss3Cache->num;
    if (curLen < 20) {
//        LOGE("boss3Cache:%d", curLen);
        for (int i = curLen; i < 20; i++) {
            createSingleNodeForCache(boss3Cache);
        }
    }
    
    curLen = dian_cache->num;
    if (curLen < 10) {
        for (int i = curLen; i < 10; i++) {
            createSingleNodeForCache(dian_cache);
        }
    }
    
    curLen = xuecaoCache->num;
    if (curLen < 20) {
        for (int i = curLen; i < 20; i++) {
            createSingleNodeForCache(xuecaoCache);
        }
    }
}

void Global::clearCaches() {
    for(int i = 0; i < containerCache->num; ++i) {
        wyNode* node = (wyNode*)containerCache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("-1 not null");
        }
    }
    wyArrayDestroy(containerCache);
    containerCache = NULL;
    
    for(int i = 0; i < fleshSpriteCache->num; ++i) {
        wyNode* node = (wyNode*)fleshSpriteCache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("0 not null");
        }
    }
    wyArrayDestroy(fleshSpriteCache);
    fleshSpriteCache = NULL;
    
    for(int i = 0; i < shadowSpriteCache->num; ++i) {
        wyNode* node = (wyNode*)shadowSpriteCache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("1 not null");
        }
    }
    wyArrayDestroy(shadowSpriteCache);
    shadowSpriteCache = NULL;
    
    for(int i = 0; i < bulletSpriteCache->num; ++i) {
        wyNode* node = (wyNode*)bulletSpriteCache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("2 not null");
        }
    }
    wyArrayDestroy(bulletSpriteCache);
    bulletSpriteCache = NULL;
    
    for(int i = 0; i < enemy_0_cache->num; ++i) {
        wyNode* node = (wyNode*)enemy_0_cache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("3 not null");
        }
    }
    wyArrayDestroy(enemy_0_cache);
    enemy_0_cache = NULL;
    
    for(int i = 0; i < enemy_1_cache->num; ++i) {
        wyNode* node = (wyNode*)enemy_1_cache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("4 not null");
        }
    }
    wyArrayDestroy(enemy_1_cache);
    enemy_1_cache = NULL;
    
    for(int i = 0; i < enemy_2_cache->num; ++i) {
        wyNode* node = (wyNode*)enemy_2_cache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("5 not null");
        }
    }
    wyArrayDestroy(enemy_2_cache);
    enemy_2_cache = NULL;
    
    for(int i = 0; i < effects_cache->num; ++i) {
        wyNode* node = (wyNode*)effects_cache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("6 not null");
        }
    }
    wyArrayDestroy(effects_cache);
    effects_cache = NULL;
    
    for(int i = 0; i < coin_emitterArray->num; ++i) {
        wyNode* node = (wyNode*)coin_emitterArray->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("7 not null");
        }
    }
    wyArrayDestroy(coin_emitterArray);
    coin_emitterArray = NULL;
    
    for(int i = 0; i < smoke_emitterArray->num; ++i) {
        wyNode* node = (wyNode*)smoke_emitterArray->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("8 not null");
        }
    }
    wyArrayDestroy(smoke_emitterArray);
    smoke_emitterArray = NULL;
    
    //    if (sGlobal->index == 110 || sGlobal->index == 104) {
    for(int i = 0; i < boss1Cache->num; ++i) {
        wyNode* node = (wyNode*)boss1Cache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("9 not null");
        }
    }
    wyArrayDestroy(boss1Cache);
    boss1Cache = NULL;
    //    }
    
    for(int i = 0; i < boss2Cache->num; ++i) {
        wyNode* node = (wyNode*)boss2Cache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("10 not null");
        }
    }
    wyArrayDestroy(boss2Cache);
    boss2Cache = NULL;
    
    for(int i = 0; i < boss3Cache->num; ++i) {
        wyNode* node = (wyNode*)boss3Cache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("11 not null");
        }
    }
    wyArrayDestroy(boss3Cache);
    boss3Cache = NULL;
    
    for(int i = 0; i < dian_cache->num; ++i) {
        wyNode* node = (wyNode*)dian_cache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("12 not null");
        }
    }
    wyArrayDestroy(dian_cache);
    dian_cache = NULL;
    
    for(int i = 0; i < xuecaoCache->num; ++i) {
        wyNode* node = (wyNode*)xuecaoCache->arr[i];
        if (node->getParent() == NULL) {
            node->release();
        } else {
            LOGE("13 not null");
        }
    }
    wyArrayDestroy(xuecaoCache);
    xuecaoCache = NULL;
}

void* Global::popFromCache(wyArray* arr) {
    wyNode *node = (wyNode*)wyArrayPop(arr);
    if (node == NULL) {
        createSingleNodeForCache(arr, true);
        node = (wyNode*)wyArrayPop(arr);
    }
    node->setAlpha(255);
    node->setRotation(0);
    node->setVisible(true);
    node->setPosition(0, 0);
    node->setColor(wyc4b(255, 255, 255, 255));
    node->setAnchor(0.5, 0.5);
//    node->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
    if (arr == enemy_0_cache || arr == enemy_1_cache || arr == enemy_2_cache || arr == effects_cache || arr == dian_cache || arr == boss1Cache  || arr == boss2Cache  || arr == boss3Cache) {
        wySPXSprite *sprite = (wySPXSprite*) node;
        sprite->setFlipX(false);
        sprite->setPaused(false);
        sprite->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
    }
    return node;
}

void Global::pushToCache(wyArray* arr, wyNode *node) {
    if (mainScene != NULL && !mainScene->isQuiting) {
        node->getParent()->removeChildLocked(node, false);
        node->stopAllActions();
        wyArrayPush(arr, node);
    } else {
        LOGE("MainScene quiting, no need to push %s.", node->getClassName());
    }
}

void Global::createSingleNodeForCache(wyArray* arr, bool isExced) {
    wyNode* node;
    if (arr == containerCache) {
        node = wyNode::make();
        if (isExced) {
            LOGE("containerCache exceed..");
        }
    }
    else if (arr == fleshSpriteCache) {
        node = wyZwoptexManager::getInstance()->makeSpriteEx("zombiebodies_pic", "1_1.png");
        if (isExced) {
            LOGE("fleshSpriteCache exceed..");
        }
    }
    else if (arr == shadowSpriteCache) {
        node = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "shadow.png");
        if (isExced) {
            LOGE("shadowSpriteCache exceed..");
        }
    }
    else if (arr == bulletSpriteCache) {
//        node = wySpriteEx::make(MyTextureManager::makePNG("bullet.png"));
        node = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", "bullet3.png");
        if (isExced) {
            LOGE("bulletSpriteCache exceed..");
        }
    }
    else if (arr == enemy_0_cache) {
        wySPXSprite *tempNode = wySPXSprite::make("enemy_0.sprite", false, MyTextureManager::makePNG("enemy_0.png"), 0);
        tempNode->setForceTickMode(true);
        node = tempNode;
        if (isExced) {
            LOGE("enemy_0_cache exceed..");
        }
    }
    else if (arr == enemy_1_cache) {
        wySPXSprite *tempNode = wySPXSprite::make("enemy_1.sprite", false, MyTextureManager::makePNG("enemy_1.png"), 0);
        tempNode->setForceTickMode(true);
        node = tempNode;
        if (isExced) {
            LOGE("enemy_1_cache exceed..");
        }
    }
    else if (arr == enemy_2_cache) {
        wySPXSprite *tempNode = wySPXSprite::make("enemy_2.sprite", false, MyTextureManager::makePNG("enemy_2.png"), 0);
        tempNode->setForceTickMode(true);
        node = tempNode;
        if (isExced) {
            LOGE("enemy_2_cache exceed..");
        }
    }
    else if (arr == effects_cache) {
        wySPXSprite *tempNode = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), 0);
        tempNode->setForceTickMode(true);
        node = tempNode;
        if (isExced) {
            LOGE("effects_cache exceed..");
        }
    }
    else if (arr == coin_emitterArray) {
        node = wyParticleLoader::load("particle_coin.plist");
        if (isExced) {
            LOGE("coin_emitterArray exceed..");
        }
    }
    else if (arr == fire_emitterArray) {
        node = wyParticleLoader::load("particle_torch_fire.plist");
        if (isExced) {
            LOGE("fire_emitterArray exceed..");
        }
    }
    else if (arr == smoke_emitterArray) {
        node = wyParticleLoader::load("particle_rpg_rocket.plist");
        if (isExced) {
            LOGE("smoke_emitterArray exceed..");
        }
    }
    else if (arr == boss1Cache) {
        wySPXSprite *tempNode = wySPXSprite::make("boss1.sprite", false, MyTextureManager::makePNG("boss1.png"), 0);
        tempNode->setForceTickMode(true);
        node = tempNode;
        if (isExced) {
            LOGE("boss1Cache exceed..");
        }
    }
    else if (arr == boss2Cache) {
        wySPXSprite *tempNode = wySPXSprite::make("boss2.sprite", false, MyTextureManager::makePNG("boss2.png"), 0);
        tempNode->setForceTickMode(true);
        node = tempNode;
        if (isExced) {
            LOGE("boss2Cache exceed..");
        }
    }
    else if (arr == boss3Cache) {
        wySPXSprite *tempNode = wySPXSprite::make("boss3.sprite", false, MyTextureManager::makePNG("boss3.png"), 0);
        tempNode->setForceTickMode(true);
        node = tempNode;
        if (isExced) {
            LOGE("boss3Cache exceed..");
        }
    }
    else if (arr == dian_cache) {
        wySPXSprite *tempNode = wySPXSprite::make("dian.sprite", false, MyTextureManager::makePNG("dian.png"), 0);
        tempNode->setForceTickMode(true);
        node = tempNode;
        if (isExced) {
            LOGE("dian_cache exceed..");
        }
    }
    else if (arr == xuecaoCache) {
        node = wyProgressTimer::make(wyZwoptexManager::getInstance()->makeSprite("dialog_deco", "progressbar_upper.png"));
        if (isExced) {
            LOGE("xuecaoCache exceed..");
        }
    }
    
    node->setVisible(false);
    node->retain();
    wyArrayPush(arr, node);
}

void Global::gotoWorldMapScene(bool isExpand) {
#if ANDROID
    hideAd();
#endif
//    sGlobal->playBGM(0);
    wyScene *scene = new WorldMapScene(isExpand);
    scene->autoRelease();
    wyDirector::getInstance()->replaceScene(wyColorFadeTransition::make(1.5f, scene, wyc3b(0, 0, 0)));
}

void Global::gotoMainScene(int mapIndex) {
    wyScene *scene = new MainScene(mapIndex);
    scene->autoRelease();
    wyDirector::getInstance()->replaceScene(wyColorFadeTransition::make(1.5f, scene, wyc3b(0, 0, 0)));
}

void Global::gotoUpgradeScene(int mapIndex) {
#if ANDROID
    hideAd();
#endif
//    sGlobal->playBGM(0);
    wyScene *scene = new UpgradeScene(mapIndex);
    scene->autoRelease();
    wyDirector::getInstance()->replaceScene(wyColorFadeTransition::make(1.5f, scene, wyc3b(0, 0, 0)));
}

void Global::gotoCGScene(bool needUnlockNandu) {
    wyScene *scene = new CGScene(false, needUnlockNandu);
    scene->autoRelease();
    wyDirector::getInstance()->replaceScene(wyColorFadeTransition::make(1.5f, scene, wyc3b(0, 0, 0)));
}

void Global::loadAllImages(bool isResume) {
//    LOGE("loadAllImages start...");
//    if (isShopOpening) {
//        return;
//    }
    
//    LOGE("load all images start...");
//    char now[50];
//    sprintf(now, "%s", getModifyDateString().c_str());
//    time_t nowTime = GetTime(now);
    
    MyTextureManager::makePNG("dialog_deco.png")->load();
    MyTextureManager::makePNG("anitile_noborder.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("anitile.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("bamboo_0.png", WY_TEXTURE_PIXEL_FORMAT_RGB565)->load();
    MyTextureManager::makePNG("bamboo_2.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("suolian.png")->load();
    MyTextureManager::makePNG("boss1.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("vertical_thumb.png")->load();
    MyTextureManager::makePNG("boss2.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("boss3.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("cave_0.png", WY_TEXTURE_PIXEL_FORMAT_RGB565)->load();
    MyTextureManager::makePNG("cave_3.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("effects.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("enemy_0.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("enemy_1.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("enemy_2.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("flyingl.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("gplus_icon_64.png")->load();
    MyTextureManager::makePNG("grass_0.png", WY_TEXTURE_PIXEL_FORMAT_RGB565)->load();
    MyTextureManager::makePNG("grass_4.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("hero.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("s01_shortcut.png")->load();
    MyTextureManager::makePNG("s01.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("s02_shortcut.png")->load();
    MyTextureManager::makePNG("s02.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("s03_shortcut.png")->load();
    MyTextureManager::makePNG("s03.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("sc_upgrade_bg.png", WY_TEXTURE_PIXEL_FORMAT_RGBA5551)->load();
    MyTextureManager::makePNG("start_scene_bg.png")->load();
    MyTextureManager::makePNG("start_scene_tex.png")->load();
    MyTextureManager::makePNG("ui_en.png")->load();
    MyTextureManager::makePNG("zombiebodies_pic.png")->load();
    MyTextureManager::makePNG("hero1.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    MyTextureManager::makePNG("dian.png", WY_TEXTURE_PIXEL_FORMAT_RGBA4444)->load();
    
    wyZwoptexManager::getInstance()->addZwoptex("dialog_deco", "dialog_deco.plist", false, MyTextureManager::makePNG("dialog_deco.png"));
    wyZwoptexManager::getInstance()->addZwoptex("zombiebodies_pic", "zombiebodies_pic.plist", false, MyTextureManager::makePNG("zombiebodies_pic.png"));
    wyZwoptexManager::getInstance()->addZwoptex("ui_en", "ui_en.plist", false, MyTextureManager::makePNG("ui_en.png"));
    
    preloadEffect(se_absorb);
    preloadEffect(se_achievement);
    preloadEffect(se_bomb);
    preloadEffect(se_boost);
    preloadEffect(se_boss1_attack1);
    preloadEffect(se_boss1_attack2);
    preloadEffect(se_boss1_dead);
    preloadEffect(se_boss1_start);
    preloadEffect(se_boss2_attack1);
    preloadEffect(se_boss2_attack2);
    preloadEffect(se_boss2_dead);
    preloadEffect(se_boss2_start);
    preloadEffect(se_boss3_attack1);
    preloadEffect(se_boss3_attack2);
    preloadEffect(se_boss3_dead);
    preloadEffect(se_boss3_start);
    preloadEffect(se_button);
    preloadEffect(se_call_liudan);
    preloadEffect(se_changegun);
    preloadEffect(se_coin);
    preloadEffect(se_devourer_dead);
    preloadEffect(se_doctor_dead);
    preloadEffect(se_fatzombie_dead);
    preloadEffect(se_femalezombie_dead);
    preloadEffect(se_fly_dart);
    preloadEffect(se_ghostbaby_dead);
    preloadEffect(se_gun_gatling);
    preloadEffect(se_gun_laser);
    preloadEffect(se_gun_normal);
    preloadEffect(se_gun_rifle);
    preloadEffect(se_gun_rpg);
    preloadEffect(se_gun_shot);
    preloadEffect(se_heal);
    preloadEffect(se_hit);
    preloadEffect(se_jetpack_jump);
    preloadEffect(se_jetpack);
    preloadEffect(se_jump);
    preloadEffect(se_malezombie_dead);
    preloadEffect(se_menu);
    preloadEffect(se_motocycle);
    preloadEffect(se_musclezombie_dead);
    preloadEffect(se_player_dead);
    preloadEffect(se_player_hurt);
    preloadEffect(se_robert_speak);
    preloadEffect(se_shield_active);
    preloadEffect(se_spittlezombie_dead);
    preloadEffect(se_upgrade);
    preloadEffect(se_wolf_dead);
    
    preloadEffect(se_gun_ak47);
    preloadEffect(se_gun_blasting);
    preloadEffect(se_gun_ds2);
    preloadEffect(se_gun_e3);
    preloadEffect(se_gun_f1);
    preloadEffect(se_gun_glb);
    preloadEffect(se_gun_glr);
    preloadEffect(se_gun_hydra);
    preloadEffect(se_gun_mp5a5);
    preloadEffect(se_gun_torpedo);

//    LOGE("load all images end...");
//    char last[50];
//    sprintf(last, "%s", getModifyDateString().c_str());
//    time_t lastTime = GetTime(last);
//    long timespend = difftime(nowTime, lastTime);
//    LOGE("%s, %s, time spend:%ld", now, last, timespend);
}


void Global::showMenu(wyNode *parent, int menuId) {
    wyLayer *layer = NULL;
    switch (menuId) {
        case MENU_SHOP: {
            ShopLayer *slayer = new ShopLayer();
            slayer->listener = (ShopListener *)parent;
            layer = slayer;
            
            if (isDebug) {
                sGlobal->gold.offset(100000);
                sGlobal->zuanshi.offset(10000);
                
                MyPrefs::setInt(DB_GOLD, gold.getValue());
                MyPrefs::setInt(DB_ZUANSHI_NUM, zuanshi.getValue());
            }
            break;
        }
        case MENU_FREE_STUFF:
            layer = new FreeStuffLayer();
            break;
        case MENU_MISSIONS:
            layer = new MissionsLayer();
            break;
        case MENU_SETTINGS:
            layer = new SettingsLayer();
            break;
        case MENU_STATISTICS:
            layer = new StatisticsLayer();
            break;
        case MENU_TUTORIAL: {
            sGlobal->isRealEndless = false;
            sGlobal->currentSModeDifficulty = DIFFICULTY_NORMAL;
            
            wyDirector* director = wyDirector::getInstance();
            wyScene *scene = new LoadingScene(0, 0);
            scene->autoRelease();
            director->replaceScene(scene);
            
            sGlobal->isTutorialFromMenu = true;
            break;
        }
        case MENU_LEADERBOARDS: {
            int hightscore = MAX(0, MAX(MAX(sGlobal->distRecNormal, sGlobal->distRecHeroic), sGlobal->distRecInfernal));
#if ANDROID
            submitScoreAndShowLB(hightscore);
#endif            
            break;
        }
        default:
            break;
    }
    if (layer != NULL) {
        layer->autoRelease();
        parent->addChildLocked(layer, 9999);
    }
}

bool Global::isSkillUnlockable(int idx) {
    bool canUnlock = true;
    switch (idx) {
        case SKILL_ROBOT:
            if (sGlobal->skillArray[SKILL_MISSLE].getValue() >= SKILL_ROBOT_PRV) {
                canUnlock = true;
            } else {
                canUnlock = false;
            }
            break;
        case SKILL_SHIELD:
            if (sGlobal->skillArray[SKILL_ROBOT].getValue() >= SKILL_SHIELD_PRV) {
                canUnlock = true;
            } else {
                canUnlock = false;
            }
            break;
        case SKILL_ARMOR:
            if (sGlobal->skillArray[SKILL_GREED].getValue() >= SKILL_ARMOR_PRV) {
                canUnlock = true;
            } else {
                canUnlock = false;
            }
            break;
        case SKILL_STAMINA:
            if (sGlobal->skillArray[SKILL_GREED].getValue() >= SKILL_STAMINA_PRV) {
                canUnlock = true;
            } else {
                canUnlock = false;
            }
            break;
        case SKILL_EVASION:
            if (sGlobal->skillArray[SKILL_STAMINA].getValue() >= SKILL_EVASION_PRV) {
                canUnlock = true;
            } else {
                canUnlock = false;
            }
            break;
        case SKILL_CRITICAL:
            if (sGlobal->skillArray[SKILL_FRENZY].getValue() >= SKILL_CRITICAL_PRV) {
                canUnlock = true;
            } else {
                canUnlock = false;
            }
            break;
        case SKILL_STUN:
            if (sGlobal->skillArray[SKILL_CRITICAL].getValue() >= SKILL_STUN_PRV) {
                canUnlock = true;
            } else {
                canUnlock = false;
            }
            break;
        default:
            break;
    }
    return canUnlock;
}

bool Global::hasUnlockableSkills() {
    bool flag = false;
    for (int i = 0; i < 10; i++) {
        if (isSkillNeedToUnlock(i)) {
            return true;
        }
    }
    
    return flag;
}

bool Global::isSkillNeedToUnlock(int idx) {
    switch (idx) {
        case SKILL_GREED:
            if (sGlobal->skillArray[SKILL_GREED].getValue()==0) {
                return true;
            }
            break;
        case SKILL_FRENZY:
            if (sGlobal->skillArray[SKILL_FRENZY].getValue()==0) {
                return true;
            }
            break;
        case SKILL_ROBOT:
            if (sGlobal->skillArray[SKILL_MISSLE].getValue() >= SKILL_ROBOT_PRV && sGlobal->skillArray[SKILL_ROBOT].getValue()==0) {
                return true;
            }
            break;
        case SKILL_SHIELD:
            if (sGlobal->skillArray[SKILL_ROBOT].getValue() >= SKILL_SHIELD_PRV && sGlobal->skillArray[SKILL_SHIELD].getValue()==0) {
                return true;
            }
            break;
        case SKILL_ARMOR:
            if (sGlobal->skillArray[SKILL_GREED].getValue() >= SKILL_ARMOR_PRV && sGlobal->skillArray[SKILL_ARMOR].getValue()==0) {
                return true;
            }
            break;
        case SKILL_STAMINA:
            if (sGlobal->skillArray[SKILL_GREED].getValue() >= SKILL_STAMINA_PRV && sGlobal->skillArray[SKILL_STAMINA].getValue()==0) {
                return true;
            }
            break;
        case SKILL_EVASION:
            if (sGlobal->skillArray[SKILL_STAMINA].getValue() >= SKILL_EVASION_PRV && sGlobal->skillArray[SKILL_EVASION].getValue()==0) {
                return true;
            }
            break;
        case SKILL_CRITICAL:
            if (sGlobal->skillArray[SKILL_FRENZY].getValue() >= SKILL_CRITICAL_PRV && sGlobal->skillArray[SKILL_CRITICAL].getValue()==0) {
                return true;
            }
            break;
        case SKILL_STUN:
            if (sGlobal->skillArray[SKILL_CRITICAL].getValue() >= SKILL_STUN_PRV && sGlobal->skillArray[SKILL_STUN].getValue()==0) {
                return true;
            }
            break;
        default:
            break;
    }
    
    return false;
}

bool Global::hasUpgradeWeapons() {
    bool flag = false;
    for (int i = 0; i < 6; i++) {
        if (canWeaponUpgrade(i)) {
            return true;
        }
    }
    
    return flag;
}

bool Global::canWeaponUpgrade(int idx) {
    if (sGlobal->weaponUnlockArray[idx].getValue() == sGlobal->WEAPON_MAX_LEVEL) {
        return false;
    }
    
    if (sGlobal->weaponUnlockArray[idx].getValue() > 0) {
        return false;
    }
    
    if (sGlobal->weaponNoticeArray[idx] == 1) {
        return false;
    }
    
    if (idx == WEAPON_GLB) {
        return false;
    }
    
    if (isWeaponUlkNeedCash(idx)) {
        if (sGlobal->zuanshi.getValue() >= sGlobal->weapon_zuanshi[idx]) {
            return true;
        } else {
            return false;
        }
    } else {
        if (sGlobal->getGold() >= sGlobal->weapon_gold[idx]) {
            return true;
        } else {
            return false;
        }
    }
    
    return false;
}

bool Global::isWeaponUlkNeedCash(int idx) {
    if (idx == WEAPON_HYDRA || idx == WEAPON_DS2 || idx == WEAPON_RPG || idx == WEAPON_E3 || idx == WEAPON_GLR || idx == WEAPON_TORPEDO) {
        return true;
    }
    return false;
}

bool Global::hasUpgradeSkills() {
    bool flag = false;
    for (int i = 0; i < 10; i++) {
        if (canSkillUpgrade(i)) {
            return true;
        }
    }
    
    return flag;
}

bool Global::canSkillUpgrade(int idx) {
//    if (!isSkillUnlockable(idx)) {
//        return false;
//    }
//    if (sGlobal->skillArray[idx].getValue() == sGlobal->SKILL_MAX_LEVEL) {
//        return false;
//    }
//    
//    if (idx > SKILL_SHIELD) {
//        if (sGlobal->zuanshi.getValue() >= sGlobal->skill_cash[idx]) {
//            return true;
//        } else {
//            return false;
//        }
//    } else {
//        if (sGlobal->getGold() >= sGlobal->skill_gold[idx]) {
//            return true;
//        } else {
//            return false;
//        }
//    }
//    return false;
    
    
    if (!isSkillUnlockable(idx)) {
        return false;
    }
    if (sGlobal->skillArray[idx].getValue() == sGlobal->SKILL_MAX_LEVEL) {
        return false;
    }
    
    if (sGlobal->skillArray[idx].getValue() > 0) {
        return false;
    }
    
    if (sGlobal->skillNoticeArray[idx] == 1) {
        return false;
    }
    
    if (idx > SKILL_SHIELD) {
        if (sGlobal->zuanshi.getValue() >= sGlobal->skill_cash[idx]) {
            return true;
        } else {
            return false;
        }
    } else {
        if (sGlobal->getGold() >= sGlobal->skill_gold[idx]) {
            return true;
        } else {
            return false;
        }
    }
    
    return false;
}

bool Global::isSModeUnlockable(int idx) {
    switch (idx) {
        case DIFFICULTY_NORMAL:
            return false;
            break;
        case DIFFICULTY_HEROIC:
//            LOGE("DIFFICULTY_HEROIC: %d,%d,%d", sGlobal->currentProgress[DIFFICULTY_NORMAL], sGlobal->s03_grade[DIFFICULTY_NORMAL*10+10], sGlobal->currentProgress[DIFFICULTY_HEROIC]);
            if (sGlobal->currentProgress[DIFFICULTY_NORMAL] == 310 && sGlobal->s03_score[DIFFICULTY_NORMAL*10+10] != 0 && sGlobal->currentProgress[DIFFICULTY_HEROIC] == 0) {
                return true;
            }
            break;
        case DIFFICULTY_HARD:
//            LOGE("DIFFICULTY_HARD: %d,%d,%d", sGlobal->currentProgress[DIFFICULTY_HEROIC], sGlobal->s03_grade[DIFFICULTY_HEROIC*10+10], sGlobal->currentProgress[DIFFICULTY_HARD] == 0);
            if (sGlobal->currentProgress[DIFFICULTY_HEROIC] == 310 && sGlobal->s03_score[DIFFICULTY_HEROIC*10+10] != 0 && sGlobal->currentProgress[DIFFICULTY_HARD] == 0) {
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

bool Global::isSModeActive(int idx) {
    if (idx == DIFFICULTY_NORMAL) {
        return true;
    }
    
    if (sGlobal->s03_score[10+(idx-1)*10] != 0) {
        return true;
    } else {
        return false;
    }
}

bool Global::isEModeUnlockable(int idx) {
    switch (idx) {
        case DIFFICULTY_NORMAL:
            if (sGlobal->s02_score[10] != 0 && distRecNormal == -1) {
                return true;
            }
            break;
        case DIFFICULTY_HEROIC:
            if (sGlobal->s02_score[10+DIFFICULTY_HEROIC*10] != 0 && distRecHeroic == -1) {
                return true;
            }
            break;
        case DIFFICULTY_HARD:
            if (sGlobal->s02_score[10+DIFFICULTY_HARD*10] != 0 && distRecInfernal == -1) {
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

bool Global::isEModeActive(int idx) {
    if (sGlobal->s02_score[10+idx*10] != 0) {
        return true;
    } else {
        return false;
    }
}

int Global::countScount(int seasonId, int difficulty) {
    int scount = 0;
    
    switch (seasonId) {
        case 1:
            for (int i = 1; i <= 10; i++) {
                if (sGlobal->s01_grade[i+difficulty*10] == GRADE_S) {
                    scount += 1;
                }
            }
            break;
        case 2:
            for (int i = 1; i <= 10; i++) {
                if (sGlobal->s02_grade[i+difficulty*10] == GRADE_S) {
                    scount += 1;
                }
            }
            break;
        case 3:
            for (int i = 1; i <= 10; i++) {
                if (sGlobal->s03_grade[i+difficulty*10] == GRADE_S) {
                    scount += 1;
                }
            }
            break;
        default:
            break;
    }
//    LOGE("seasonId:%d, difficulty:%d, scount:%d", seasonId, difficulty, scount);
    
    return scount;
}

void Global::loadAllImagesAsync(LoadTexListener *sLayer) {
    LOGE("loadAllImagesAsync start...");
    wyDirector* director = wyDirector::getInstance();
    wyScene *scene = director->getRunningScene();
    wyThread::runThread(wyTargetSelector::make(scene, SEL_p(Global::loadingThread), sLayer));
}


void Global::loadingThread(wyTargetSelector* sel, void *data) {
    LOGE("loadingThread start...");
    loadAllImages(true);
    
    if (data != NULL) {
        wyUtils::runOnGLThread(ResumeGameRunnable::make((LoadTexListener*)data));
    } else {
        wyUtils::runOnGLThread(ResumeGameRunnable::make(NULL));
    }
    
    LOGE("loadingThread end...");
}

void Global::offsetGold(int diffGold) {
    gold.offset(diffGold);
}

void Global::setGold(int value) {
    gold.setValue(value);
}

int Global::getGold() {
    return gold.getValue();
}

wyNode * Global::getArmsSpriteByWeaponIndex(int weaponId) {
    wyNode *icon;
    if (weaponId == WEAPON_UMP || weaponId == WEAPON_M870P || weaponId == WEAPON_AER14 || weaponId == WEAPON_M249 || weaponId == WEAPON_M4A1 || weaponId == WEAPON_RPG) {
        char temp[20];
        sprintf(temp, "weapon_%d.png", weaponId);
        icon = wyZwoptexManager::getInstance()->makeSpriteEx("ui_en", temp);
        icon->setScaleX(sGlobal->scaleX*0.68f);
        icon->setScaleY(sGlobal->scaleY*0.68f);
    } else {
        icon = wySPXSprite::make("hero1.sprite", false, MyTextureManager::makePNG("hero1.png"), weaponId-WEAPON_MP5A5);
        icon->setRotation(-24);
        icon->setScaleX(sGlobal->scaleX*0.7f);
        icon->setScaleY(sGlobal->scaleY*0.7f);
    }

    
    return icon;
}

void ResumeGameRunnable::run() {
    LOGE("load end, onResumeGameEnd...");
#if ANDROID
    onResumeGameEnd();
#endif
    
    if (m_node != NULL) {
        LoadTexListener *ltx = (LoadTexListener *)m_node;
        ltx->onLoadTexEnd();
    }
    
    if (sGlobal->isRatingMe) {
        sGlobal->isRatingMe = false;
        
        VoteUsLayer::showThanksLayer(wyDirector::getInstance()->getRunningScene());
    }
}



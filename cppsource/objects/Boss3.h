//
//  MyContactListener.h
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//
#pragma once

#include "WiEngine.h"
#include "Enemy.h"
#include "Boss1.h"

//boss3行为：
//ATK_1：普通发射铁拳，铁拳可被击毁
//ATK_2: 随机发射若干个火弹。
//ATK_3: 召唤稻草人
//ATK_4: 召唤小怪
//ATK_5: 召唤悬空标
//ATK_6: 超大火球平推

const static int BOSS3_IDLE = 0;
const static int BOSS3_ATK_TIEQUAN_0 = 1;
const static int BOSS3_ATK_XULI_TIEQUAN_0 = 20;
const static int BOSS3_ATK_XULI_TIEQUAN_1 = 21;
const static int BOSS3_ATK_ZA_0 = 30;
const static int BOSS3_ATK_ZA_1 = 31;
const static int BOSS3_ATK_ZA_2 = 32;
const static int BOSS3_ATK_DAODAN_0 = 40;
const static int BOSS3_WALK = 100;
const static int BOSS3_FAKE_DEAD = 101;

const static int BOSS3_ANIM_DAIJI = 0;
const static int BOSS3_ANIM_HURT = 1;
const static int BOSS3_ANIM_ZOULU = 2;
const static int BOSS3_ANIM_FASHE_TIEQUAN = 3;
const static int BOSS3_ANIM_ZA = 4;
const static int BOSS3_ANIM_ZHAOHUAN = 5;
const static int BOSS3_ANIM_SI = 6;
const static int BOSS3_ANIM_QUANTOU = 7;
const static int BOSS3_ANIM_XULI_TIEQUAN = 8;
const static int BOSS3_ANIM_FASHE_TIEQUAN_1 = 9;
const static int BOSS3_ANIM_FASHE_FAILED = 10;
const static int BOSS3_ANIM_FASHE_JIPO_TIEQUAN = 11;

class Boss3 : public Boss {
    
public:
    Boss3(float x, float y);
    ~Boss3();
    
    void update(float dt);
    
    static Boss3 *make(float x, float y) {
        return WYNEW Boss3(x, y);
    }
    
    void doAttack();

    void fakeDead();
    
    void throwCoin();
    
    static Boss3 *instance;
    
    wySPXSprite *deadSprite1, *deadSprite2;
    
    int atk1;
    void initShootTargets();
    void generateNextTarget();
    void attackSuccess();
    void attackFailed();
    float time2, time3, totalTime, stepTime;
    float stepTimer, totalTimer;
    ShootTarget *target1, *target2, *target3;
    bool *shootFlags;
    int curTime, curIndex, curSeq;
    int attackFlag;
    void hideShootTarget();
    bool isShootTargetHide;

    void doAttack_tiequan();
    void doAttack_xulitiequan();
    void doAttack_za();
    void doAttack_daodan();
    
    void shootQuantou();
    int xuliquan_size;
    void shootDaQuantou(int size);
    
    void onTargetShootEnd(int index);
    
    void createDaoDan();
    bool isFiringMissle;
    float firingMissleTimer;
    int fireTimes;
    int daodanCount;
    
    void hurt(float y);
    
    int ironfistAtk, knockAtk, xulifistHP, xulitargetHP, xuliFistAtk, missleHP, missleAtk;
    
    bool isZaing;
    float zaTimer, zaInterval;
    int zaTime;
    int zaCount;
};

#pragma once

#include "WiEngine.h"
#include "BaseObject.h"
#include "Global.h"

static const int APPEAR_NORMAL = 0;
static const int APPEAR_SMOKE = 1;
static const int APPEAR_SMOKE_FIRE = 2;
static const int APPEAR_JUMP_FROM_BACK = 3;
static const int APPEAR_JUMP_FROM_FRONT = 4;
static const int APPEAR_FLY_NORMAL = 5;
static const int CMD_WALK = 6;
static const int CMD_JUMP = 7;
static const int APPEAR_FAZHEN = 8;

static const int DEAD_TYPE_DART = 0;
static const int DEAD_TYPE_DIAN = 1;
static const int DEAD_TYPE_BOSS = 2;
static const int DEAD_TYPE_MOTO = 3;
static const int DEAD_TYPE_MISSLE = 4;

class RemoveMaskBitTask : public wyObject {
public:
    float m_timer;
    float m_duration;
    uint16 m_maskBit;
    
    RemoveMaskBitTask(uint16 maskBit, float duration);
    ~RemoveMaskBitTask();

    bool updateTask(float dt);
};

class Enemy;

static void getRandForce(int deadType, float &velx, float &vely) {
    switch (deadType) {
        case DEAD_TYPE_DART:
            switch (sGlobal->activeWeapon) {
                case WEAPON_M870P:
                case WEAPON_HYDRA:
                    if (rand()%8 == 0) {
                        velx = -randRange(20, 30);
                    } else {
                        velx = randRange(25, 35);
                    }
                    vely = randRange(25, 35);
                    break;
                case WEAPON_M4A1:
                case WEAPON_DS2:
                case WEAPON_TORPEDO:
                    if (rand()%5 == 0) {
                        velx = -randRange(15, 25);
                    } else {
                        velx = randRange(20, 30);
                    }
                    vely = randRange(20, 30);
                    break;
                case WEAPON_RPG:
                case WEAPON_BLASTING:
                    if (rand()%2 == 0) {
                        velx = -randRange(15, 20);
                    } else {
                        velx = randRange(20, 30);
                    }
                    vely = randRange(40, 50);
                default:
                    if (rand()%8 == 0) {
                        velx = -randRange(10, 20);
                    } else {
                        velx = randRange(20, 25);
                    }
                    vely = randRange(20, 25);
                    break;
            }
            break;
        case DEAD_TYPE_MOTO:
            if (rand()%8 == 0) {
                velx = -randRange(10, 20);
            } else {
                velx = randRange(20, 40);
            }
            vely = randRange(20, 40);
            break;
        case DEAD_TYPE_MISSLE:
            if (rand()%2 == 0) {
                velx = -randRange(10, 20);
            } else {
                velx = randRange(10, 20);
            }
            vely = randRange(30, 40);
            break;
        case DEAD_TYPE_DIAN:
            if (rand()%8 == 0) {
                velx = -randRange(5, 10);
            } else {
                velx = randRange(15, 20);
            }
            vely = randRange(20, 25);
            break;
        default:
            break;
    }
}

class Flesh : public BaseObject {
public:
    wySpriteEx *fleshSprite;
    wySpriteEx *bloodSprite1;
    bool isOnGround;
    float halfWidth;
    bool needBloodGround;
    
    static void onFleshEnded(wyAFCSprite* sprite, void* data) {
        Flesh *flesh = (Flesh *)data;
        sGlobal->mainGameLayer->addToDestroy(flesh->body);
    }
    
    Flesh(float x, float y, int i, Enemy *parent, int idx, bool needBlood = true);
    
    ~Flesh() {
        
    }
    
    void update(float dt);
    
    static void *makeFleshes(float x, float y, int count, Enemy *parent, int idx) {
        for (int i = 1; i <= 4; i++) {
            WYNEW Flesh(x, y, i, parent, idx);
        }
    }
    
    void handleCollision(b2Body *actorB);
    
    void destroyMe();
};

class Enemy : public BaseObject {
    
public:
    Enemy(int x, int y, int appearType = APPEAR_NORMAL, wyTMXObjectGroup* objectsGroup = NULL, wyTMXObject* obj = NULL, const char *myCmd = NULL, bool needBlood = true);
    virtual ~Enemy();

    int e_id, u_id;
    
//    wyPoint org_position;
//    wyPoint left_position;
//    wyPoint right_position;
//    int direction;
    
    bool isMoving;
    
    virtual void update(float dt);
    virtual void handleCollision(b2Body *actorB);
    virtual void endContact(b2Body *actorB);
    virtual void beginContact(b2Body *actorB);
    
    virtual void destroyMyself();
    void destroyMyselfWhenNotInit();
    
    wySPXSprite *bloodSprite, *faintSprite;
//    wySpriteEx *bloodPenSprite;
    float bloodPenDistX, bloodPenDistY;
//    wySpriteEx *hitSprite;
//    wyAnimation *hitAnim;
    virtual void hurt(float y);
    virtual void dealHitByDart(float y, b2Body *bulletBody);
    virtual void dealHitByLiudan(b2Body *bulletBody);
    virtual bool canBeHurt();
    virtual bool canBeMissed();
    virtual void dealHitByDaBiao(float y, b2Body *bulletBody);
    
    bool isOnGround;
    
    wySPXSprite *deadSprite;
    virtual void dead();
    
    bool isHurtByFire;
    
    int appearType;
    wySPXSprite *smokeSprite;
    float appearDistance;
    bool isSmokeEnd;
    
    bool isReadyToCmd, isCmdFinished;
    int cmdCount, curCmd;
    int *cmdList;
    float *cmdArg1List;
    float *cmdArg2List;
    float *cmdArg3List;
    float cmdTimer;
    bool isTiming, isDone;
    void doCmd(float dt);
    float jx, jy;
    bool isAppeared;
    bool isJumping;
    float jumpingTimer;
    bool isZorderSet;
    int animJump, animLanding, animDead, animWalk, animWalkBack, animHurt, animIdle, animAtk;
    virtual void playAnim(int idx, bool isAppear = false);
    
    bool isDead;
    bool needDeadBlink, needStay;
    
    int deadType;
    virtual void onDead();
    
    wySPXSprite *yan;
    float yanOffsetX;
    
    wySpriteEx *shadow;
    float shadowPosX, shadowPosY, shadowSX, shadowSY;
    
    float preVelX;
    bool isKnockingBack;
    float knockingBackTimer;
    void knockBack();
    bool canKnockBack;
    
	bool isMasking;
    void addBulletMask(uint16 newMaskBit);
    void clearBulletMask(uint16 newMaskBit);
    void resetMask();
    
    int deadSFX;
    
    int combatValue;
    
    void showCriticalEffect(int hitNum);
    void showDeadGemEffect(int zuanshi);
    void showDeadCoinEffect(int gold);
    
    int prevLoopCount, prevAnimIndex;
    void showDeadEffect();
    void showBloodEffect();
    
    void recordQuest();
    bool needShowDeadEffect;
    
    float orgX, rangeX;
    float nextWanderInterval, wanderTimer;
    bool isLandingEnded;
    bool isWandering;
    void wander(float dt);

    bool isInitToShow;
    void initToShow();
    wySPXSprite* getSpxSprite(bool isGet);
    
    void preInit(int x, int y, int appearType = APPEAR_NORMAL, wyTMXObjectGroup* objectsGroup = NULL, wyTMXObject* obj = NULL, const char *myCmd = NULL, bool needBlood = true);
//    virtual void initData(int x, int y, int appearType = APPEAR_NORMAL, wyTMXObjectGroup* objectsGroup = NULL, wyTMXObject* obj = NULL, const char *myCmd = NULL, bool needBlood = true);
    void afterInit(int x, int y, int appearType = APPEAR_NORMAL, wyTMXObjectGroup* objectsGroup = NULL, wyTMXObject* obj = NULL, const char *myCmd = NULL, bool needBlood = true);
    
    float unitInterval;
    float myScaleX, myScaleY;
    wyArray *spxCacheArray;
    
    bool needBlood;
    bool isAttacking;
    
    bool isStunning;
    float stunTimer;
    void applyStun();
    float faintSpriteX, faintSpriteY, faintSpriteScale;
    bool stunnable;
    
    int contactNum;
    bool isInScreen;
    
    bool needAttack;
    bool isFighted;
    float fightDistance;
    
    bool canWander;
    bool isHurting;
    
    wyArray *removeMaskBitArray;
    
    wyArray *bloodSplitArray;
    void splitBlood(float y);
    
    wyProgressTimer *xuecao;
    float xuecaoSx;
    float xuecaoOfx, xuecaoOfy;
    
    bool isCmdJump, isCmdFly;
};

static void onHurtDelayEnded(wyAction* action, void* data) {
    wyNode* node = (wyNode*)data;
    node->setColor(wyc3b(255, 255, 255));
}

static void onBloodEnded(wyAFCSprite* sprite, void* data) {
//    Enemy* enemy = (Enemy*)data;
    wySPXSprite *bloodSprite = ((wySPXSprite *) data);
    sprite->setVisible(false);
}

static void onDeadEnded(wyAFCSprite* sprite, void* data) {
//    Enemy* enemy = (Enemy*)data;
//    sprite->setVisible(false);
//    sGlobal->mainGameLayer->addToDestroy(enemy->body);
}

static void onBarricadeDeadEnded(wyAFCSprite* sprite, void* data) {
    Enemy* enemy = (Enemy*)data;
    sprite->setVisible(false);
    //    sGlobal->mainGameLayer->addToDestroy(enemy->body);
}

static void onSmokeAppearEnded(wyAFCSprite* sprite, void* data) {
    Enemy *enemy = (Enemy *)data;
    enemy->isSmokeEnd = true;
//    sprite->getParent()->removeChildLocked(sprite, true);
}

static void onHeiDongAppearEnded(wyAction* action, void* data) {
    Enemy *enemy = (Enemy *)data;

    enemy->container->setVisible(true);
    
	enemy->isAppeared = true;
    sGlobal->mainGameLayer->addToEnable(enemy->body);
	enemy->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	enemy->curCmd++;
    
	if (enemy->cmdCount > 0 && !enemy->isCmdFinished && !enemy->isReadyToCmd && enemy->isAppeared) {
		enemy->isReadyToCmd = true;
	}
}

static void onLandingEnded(wyAFCSprite* sprite, void* data) {
    Enemy *enemy = (Enemy *)data;
    if (enemy->cmdCount > 0 && !enemy->isCmdFinished && !enemy->isReadyToCmd && enemy->isAppeared) {
        enemy->isReadyToCmd = true;
//        LOGE("begin cmd");
    }
    if (fabs(enemy->body->GetLinearVelocity().x) > 1.0f) {
        enemy->playAnim(enemy->animWalk);
    } else {
        enemy->playAnim(enemy->animIdle);
    }
    enemy->isLandingEnded = true;
}

static void onEnemyDeadBlinkEnded(wyAction* action, void* data) {
    Enemy *enemy = (Enemy *)data;
    enemy->onDead();
    sGlobal->mainGameLayer->addToDestroy(enemy->body);
}

//static void onEnemyDeadAnimationChanged(wyAFCSprite* sprite, void* data) {
//    Enemy *enemy = (Enemy *)data;
//    if (sprite->getCurrentFrame() == 1) {
//        enemy->showDeadEffect();
//        enemy->showBloodEffect();
//    }
//}
//
//static void onEnemyDeadAnimationEnded(wyAFCSprite* sprite, void* data) {
//    Enemy *enemy = (Enemy *)data;
////    if (enemy->needDeadBlink) {
////        wyIntervalAction *blink = wyBlink::make(0.5f, 5);
////        wyActionCallback callback = {
////            NULL,
////            onEnemyDeadBlinkEnded
////        };
////        blink->setCallback(&callback, enemy);
////        sprite->runAction(blink);
////    } 
////    else if (enemy->needStay){
////        wyIntervalAction *stay = wySequence::make(wyDelayTime::make(1.0f), wyBlink::make(0.5f, 5), NULL);
////        wyActionCallback callback = {
////            NULL,
////            onEnemyDeadBlinkEnded
////        };
////        stay->setCallback(&callback, enemy);
////        sprite->runAction(stay);
////    } else {
////        wyIntervalAction *delay = wyDelayTime::make(0.2f);
////        wyActionCallback callback = {
////            NULL,
////            onEnemyDeadBlinkEnded
////        };
////        delay->setCallback(&callback, enemy);
////        sprite->runAction(delay);
////    }
//    
//    enemy->showDeadEffect();
//    enemy->showBloodEffect();
//}

static void onStartJumpEnded(wyAFCSprite* sprite, void* data) {
    Enemy* enemy = (Enemy*)data;
    if (!enemy->isDead) {
        sprite->setLoopCount(0);
        sprite->playAnimation(enemy->animJump);
    }
}

static void onHurtAnimEnded(wyAFCSprite* sprite, void* data) {
    Enemy* enemy = (Enemy*)data;
    sprite->setAFCSpriteCallback(NULL, NULL);
    if (enemy->isStunning) {
        if (enemy->isOnGround) {
            enemy->playAnim(enemy->animIdle);
        } else {
            enemy->playAnim(enemy->animJump);
        }
    } else {
        if (enemy->prevAnimIndex == enemy->animJump) {
            enemy->playAnim(enemy->animIdle);
        } else {
            sprite->setLoopCount(enemy->unitInterval);
            sprite->setLoopCount(enemy->prevLoopCount);
            sprite->playAnimation(enemy->prevAnimIndex);
        }
    }
    enemy->isHurting = false;
}

static void onBDeadEnded(wyAFCSprite* sprite, void* data) {
    Enemy* enemy = (Enemy*)data;
    sprite->setVisible(false);
    sGlobal->mainGameLayer->addToDestroy(enemy->body);
}

static float getRandomCMD(char *cmd, bool canJump = true, bool canWalk = true, bool isFly = false) {
    float offsetY = 0.0f;
    
    int seed = rand()%3;
    char temp[20] = {0};
    switch (seed) {
        case 0:
//            sprintf(temp, "");
            break;
        case 1: {
            if (!canWalk) {
//                sprintf(temp, "");
                break;
            }
            
            float fx = -randRange(5, 10);
            float duration = randRange(5, 15)/10.0f;
            if (rand()%3 == 0) {
                fx = -fx;
                duration /= 2;
            }
            sprintf(temp, "w,%.1f,%.1f,0;", duration, fx);
            break;
        }
        case 2: {
            if (!canJump) {
//                sprintf(temp, "");
                break;
            }
            
            float fx = randRange(10, 15);
            if (rand()%2 == 0) {
                fx = -fx;
            }
            float fy = randRange(10, 15);
            sprintf(temp, "j,%.1f,%.1f;", fx, fy);
            break;
        }
        default:
//            sprintf(temp, "");
            break;
    }
//    LOGE("temp:%s", temp);
    
    
    float appearRange = randRange(7, 11) / 10.0f;
    if (!isFly) {
        seed = rand()%6;
        switch (seed) {
            case 0:
                sprintf(cmd, "n,%.1f;w,0.5,0,0;%s", appearRange, temp);
                break;
            case 1:
                sprintf(cmd, "s,%.1f;w,0.5,0,0;%s", appearRange, temp);
                break;
            case 2:
                sprintf(cmd, "sf,%.1f;w,0.5,0,0;%s", appearRange, temp);
                break;
            case 3: {
                if (!canJump) {
                    sprintf(cmd, "n,%.1f;w,0.5,0,0;%s", appearRange, temp);
                    break;
                }
                
                float fx = randRange(0, 10);
                if (rand()%2 == 0) {
                    fx = -fx;
                }
                float fy = randRange(30, 40);
                appearRange = randRange(6, 7) / 10.0f;
                sprintf(cmd, "jb,%.1f,%.1f,%.1f;w,0.5,0,0;%s", appearRange, fx, fy, temp);
                
                offsetY = -20;
                break;
            }
            case 4: {
                if (!canJump) {
                    sprintf(cmd, "s,%.1f;w,0.5,0,0;%s", appearRange, temp);
                    break;
                }
                
                float fx = randRange(0, 10);
                if (rand()%2 == 0) {
                    fx = -fx;
                }
                float fy = randRange(30, 40);
                appearRange = randRange(6, 7) / 10.0f;
                sprintf(cmd, "jf,%.1f,%.1f,%.1f;w,0.5,0,0;%s", appearRange, fx, fy, temp);
                
                offsetY = -20;
                break;
            }
            case 5: {
                sprintf(cmd, "fz,%.1f;w,0.5,0,0;%s", 1.0f, temp);
                break;
            }
            default:
                sprintf(cmd, "n;w,0.5,0,0;%s", temp);
                break;
        }
    } else {
        sprintf(cmd, "fn,%.1f;w,0.5,0,0;%s", appearRange, temp);
    }
    
//    offsetY = 0;
//    sprintf(cmd, "jb,%.1f,%.1f,%.1f;w,0.5,0,0;", 0.7f, 0.1f, 50.0f);
//    delete[] &temp;
//    LOGE("offsetY:%f", offsetY);
    
    return offsetY;
}

static void appendWanderCMD(char *cmd, bool isFly) {
    int seed = 0;
    float appearRange = randRange(7, 11) / 10.0f;
    
    if (!isFly) {
        seed = rand()%6;
        switch (seed) {
            case 4: {
                float fx = -randRange(5, 8);
                float duration = randRange(5, 10)/10.0f;
                if (rand()%2 == 0) {
                    fx = -fx;
                }
                sprintf(cmd, "%sw,%.1f,%.1f,0;", cmd, duration, fx);
                break;
            }
            case 5: {
                float fx = randRange(10, 15);
                if (rand()%2 == 0) {
                    fx = -fx;
                }
                float fy = randRange(10, 15);
                sprintf(cmd, "%sj,%.1f,%.1f;", cmd, fx, fy);
                break;
            }
            default:
                break;
        }
    } else {
        seed = rand()%5;
        switch (seed) {
            case 4: {
                float fx = -randRange(5, 10);
                float fy = randRange(5, 10);
                float duration = randRange(10, 20)/10.0f;
                if (rand()%2 == 0) {
                    fx = -fx;
                }
                sprintf(cmd, "%sw,%.1f,%.1f,%.1f;", cmd, duration, fx, fy);
                break;
            }
            default:
                break;
        }
    }

}

static float getRandomCMDInStoryMode(Enemy *baseEnemy, char *cmd, bool isFly = false) {
    float offsetY = 0.0f;
    
    int seed = rand()%2;
    float appearRange = randRange(7, 11) / 10.0f;
    char temp[50] = {0};

    if (baseEnemy->isCmdJump) {
        if (isFly) {
            sprintf(cmd, "fn,%.1f;w,0.5,0,0;%s", appearRange, temp);
            appendWanderCMD(cmd, true);
        } else {
            float fx = randRange(0, 6);
            if (rand()%2 == 0) {
                fx = -fx;
            }
            float fy = baseEnemy->jy + randRange(0, 10);
            appearRange = randRange(6, 10) / 10.0f;
            
            switch (seed) {
                case 0: {
                    sprintf(cmd, "jb,%.1f,%.1f,%.1f;w,0.5,0,0;", appearRange, fx, fy);
                    break;
                }
                case 1: {
                    sprintf(cmd, "jf,%.1f,%.1f,%.1f;w,0.5,0,0;", appearRange, fx, fy);
                    break;
                }
                default:
                    break;
            }
            
            appendWanderCMD(cmd, false);
        }
    }
    else {
        if (!isFly) {
            seed = rand()%6;
            switch (seed) {
                case 0:
                case 1:
                case 2:
                    appearRange = randRange(9, 11) / 10.0f;
                    sprintf(cmd, "n,%.1f;w,0.1,0,0;", appearRange);
                    break;
                case 3:
                    appearRange = randRange(7, 9) / 10.0f;
                    sprintf(cmd, "s,%.1f;w,0.1,0,0;", appearRange);
                    break;
                case 4: {
                    float fx = randRange(0, 10);
                    if (rand()%2 == 0) {
                        fx = -fx;
                    }
                    float fy = randRange(30, 40);
                    appearRange = randRange(6, 7) / 10.0f;
                    sprintf(cmd, "jb,%.1f,%.1f,%.1f;w,0.1,0,0;", appearRange, fx, fy);
                    
                    offsetY = -20;
                    break;
                }
                case 5: {
                    float fx = randRange(0, 10);
                    if (rand()%2 == 0) {
                        fx = -fx;
                    }
                    float fy = randRange(30, 40);
                    appearRange = randRange(6, 7) / 10.0f;
                    sprintf(cmd, "jf,%.1f,%.1f,%.1f;w,0.1,0,0;", appearRange, fx, fy);
                    
                    offsetY = -20;
                    break;
                }
                default:
                    sprintf(cmd, "n;w,0.1,0,0;");
                    break;
            }
            
            appendWanderCMD(cmd, false);
        } else {
            sprintf(cmd, "fn,%.1f;w,0.1,0,0;", appearRange);
            appendWanderCMD(cmd, true);
        }
    }
    
    LOGE("cmd:%s", cmd);
    return offsetY;
}

static float getShiTouCMD(char *cmd, int& isBounce) {
    float ofY = 0;
    
    int seed = rand()%2;
    char temp[20] = {0};
    switch (seed) {
        case 0:
//            sprintf(temp, "");
            isBounce = 0;
            break;
        case 1: {
            float fx = -randRange(5, 20);
            sprintf(temp, "w,%.1f,%.1f,0;", 10.0f, fx);
            
            ofY = DP(randRange(50, 200))*sGlobal->scaleY;
            
            isBounce = 1;
            break;
        }
        default:
//            sprintf(temp, "");
            break;
    }
    
    float appearRange = randRange(10, 11) / 10.0f;
    sprintf(cmd, "n,%.1f;%s", appearRange, temp);
    
    return ofY;
}

static float getXuanKongBiaoCMD(char *cmd) {
    float ofY = 0;
    
    int seed = rand()%2;
    char temp[20] = {0};
    switch (seed) {
        case 0:
//            sprintf(temp, "");
            ofY = DP(randRange(80, 160))*sGlobal->scaleY;
            break;
        case 1: {
            float fx = -randRange(5, 20);
            float fy = -randRange(5, 20);
            if (sGlobal->isRealEndless && sGlobal->isBoostOn) {
                fy *= sGlobal->boostMulti;
            }
            
            if (rand()%3==1) {
                fy = -fy;
            }
            sprintf(temp, "w,%.1f,%.1f,%.1f;", 10.0f, fx, fy);
            
            if (fy > 0) {
                ofY = -DP(randRange(100, 200))*sGlobal->scaleY;
            } else {
                ofY = DP(randRange(300, 400))*sGlobal->scaleY;
            }
            break;
        }
        default:
//            sprintf(temp, "");
            break;
    }
    
    sprintf(cmd, "%s", temp);
    
    return ofY;
}

class Blood : public BaseObject {
    
public:
    wySpriteEx *bloodSprite, *bloodSprite1;
    bool isOnGround;
    int idx;
    float halfWidth, tHeight;
    
//    static void onBloodEnded(wyAFCSprite* sprite, void* data) {
//        Blood *blood = (Blood *)data;
//        sGlobal->mainGameLayer->addToDisable(blood->body);
//    }
    
    Blood(float x, float y, int i, Enemy *parent);
    
    ~Blood() {
        
    }
    
    void update();
    void handleCollision(b2Body *actorB);
    void update(float dt);
    
    static Blood *make(float x, float y, int i, Enemy *parent) {
        return WYNEW Blood(x, y, i, parent);
    }
    
    void beginContact(b2Body *actorB) {
        BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
        int tagB = userDataB->obj_type;
        switch (tagB) {
            case TYPE_GROUND: {

                break;
            }
            default:
                break;
        }
    }
    
    void destroyMe() {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();

        if (bloodSprite != NULL) {
            sGlobal->mainGameLayer->bloodBatchNode->removeChildLocked(bloodSprite, false);
            wyArrayPush(sGlobal->shadowSpriteCache, bloodSprite);
        }
        if (bloodSprite1 != NULL) {
            sGlobal->mainGameLayer->bloodBatchNode->removeChildLocked(bloodSprite1, false);
            wyArrayPush(sGlobal->shadowSpriteCache, bloodSprite1);
        }
        
        world->DestroyBody(body);
        delete this;
        
        sGlobal->mainGameLayer->currentBodies -= 1;
    }
};

class EnemyBulletBase : public BaseObject {
public:
    wyArray *spxCacheArray;
    int animIdle;
    float unitInterval, myScaleX, myScaleY;
    int b_id;
    
    EnemyBulletBase() {
        b_id = 0;
    }
    
    virtual void destroyMyself() {
        
    }
    
    virtual void onHitHero() {
        sGlobal->mainGameLayer->addToDestroy(body);
    }
};

class UnShootableObject : public EnemyBulletBase {
public:
    float boxWidth, boxHeight;
    
    UnShootableObject() {
        
    }
    
    UnShootableObject(wyArray *p_spxCacheArray, int p_animIdle, float p_unitInterval, float p_myScaleX, float p_myScaleY, float p_boxWidth, float p_boxHeight, int attack) : EnemyBulletBase() {
        spxCacheArray = p_spxCacheArray;
        animIdle = p_animIdle;
        unitInterval = p_unitInterval;
        myScaleX = p_myScaleX;
        myScaleY = p_myScaleY;
        boxWidth = p_boxWidth;
        boxHeight = p_boxHeight;
        atk = attack;
    }
    
    ~UnShootableObject() {
        
    }
    
    void postInit() {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        int e_id = -99;

        hp = 1;
        atk = 0;
        gold = 0;
        
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(container);
        
        spxSprite = (wySPXSprite *)sGlobal->popFromCache(spxCacheArray);
        spxSprite->stopAllActions();
        spxSprite->setAFCSpriteCallback(NULL, NULL);
        spxSprite->playAnimation(animIdle);
        spxSprite->setUnitInterval(unitInterval);
        spxSprite->setScaleX(myScaleX);
        spxSprite->setScaleY(myScaleY);
        spxSprite->setVisible(true);
        spxSprite->setAlpha(255);
        sGlobal->mainGameLayer->addChildLocked(spxSprite);
        
        obj_type = TYPE_ENEMY_BULLET;
        
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(DP(60)*sGlobal->scaleY), m_box2d->pixel2Meter(DP(50)*sGlobal->scaleY));
        bd.userData = this;
        bd.type = b2_dynamicBody;
        
        b2PolygonShape staticBox;
        staticBox.SetAsBox(m_box2d->pixel2Meter(boxWidth*sGlobal->scaleX), m_box2d->pixel2Meter(boxHeight*sGlobal->scaleY));
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &staticBox;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.isSensor = true;
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_ENEMY_BULLET];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY_BULLET];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY_BULLET];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);
        body->SetGravityScale(0.0f);

        offsetX = 0;
        offsetY = 0;
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        float dist = m_box2d->meter2Pixel(body->GetPosition().x - sGlobal->mainGameLayer->hero->body->GetPosition().x);
        if (dist < DP(200)) {
            spxSprite->stopAllActions();
            float maxSpeed = 10.0f;
            b2Vec2 desiredVelocity = sGlobal->mainGameLayer->hero->body->GetPosition() - body->GetPosition()-b2Vec2(0,m_box2d->pixel2Meter(DP(20)*sGlobal->scaleY));
            if (maxSpeed > 0)
            {
                desiredVelocity.Normalize();
                desiredVelocity *= maxSpeed;
            }
            b2Vec2 finalVelocity = desiredVelocity - body->GetLinearVelocity();
            b2Vec2 steerVector = b2Vec2(finalVelocity.x, finalVelocity.y);
            body->ApplyLinearImpulse(steerVector, body->GetPosition());

            spxSprite->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
            b2Vec2 vel = body->GetLinearVelocity();
            float angle = atan(vel.y/vel.x);
            if (vel.x > 0) {
                angle += M_PI;
            }
            spxSprite->setRotation(-wyMath::r2d(angle));
        } else {
            body->SetTransform(b2Vec2(m_box2d->pixel2Meter(spxSprite->getPositionX()), m_box2d->pixel2Meter(spxSprite->getPositionY())), -wyMath::d2r(spxSprite->getRotation()));
        }
    }

    static void onDisappearEnd(wyAction* action, void* data) {
        UnShootableObject *enemyBullet = (UnShootableObject *) data;
        
        enemyBullet->spxSprite->stopAllActions();
        enemyBullet->spxSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(enemyBullet->spxCacheArray, enemyBullet->spxSprite);
        
        enemyBullet->container->stopAllActions();
        enemyBullet->container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, enemyBullet->container);
        
        delete enemyBullet;
    }
    
    virtual void destroyMyself() {
        wyFadeOut* fade = wyFadeOut::make(0.1f, true);
        wyActionCallback callback = {
            NULL,
            onDisappearEnd,
            NULL
        };
        fade->setCallback(&callback, this);
        spxSprite->runAction(fade);
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        return;
    }
};

class ShootableObject : public Enemy {
    
public:
    bool isBaoing;
    float boxWidth, boxHeight;
    
    static void onBombEnded(wyAFCSprite* sprite, void* data) {
        ShootableObject *bomb = (ShootableObject *)data;
        bomb->spxSprite->setVisible(false);
        sGlobal->mainGameLayer->addToDestroy(bomb->body);
    }
    
    ShootableObject(wyArray *p_spxCacheArray, int p_animIdle, float p_unitInterval, float p_myScaleX, float p_myScaleY, float p_boxWidth, float p_boxHeight, int attack) : Enemy(0, 0, APPEAR_NORMAL, NULL, NULL, NULL, false) {
        
        spxCacheArray = p_spxCacheArray;
        animIdle = p_animIdle;
        unitInterval = p_unitInterval;
        myScaleX = p_myScaleX;
        myScaleY = p_myScaleY;
        boxWidth = p_boxWidth;
        boxHeight = p_boxHeight;
        atk = attack;
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        int e_id = -100;
        hp = 1;
//        atk = 0;
        gold = 0;
        
        offsetX = 0;
        offsetY = 0;
        
        isBaoing = false;
        
        needShowDeadEffect = false;
        needDeadBlink = false;
        needStay = false;
        
//        unitInterval = 0.05f;
//        myScaleX = sGlobal->scaleX*1.0f;
//        myScaleY = sGlobal->scaleY*1.0f;
//        spxCacheArray = sGlobal->enemy_1_cache;
        
        container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
        container->setVisible(true);
        sGlobal->mainGameLayer->addChildLocked(container, 102);
        
        spxSprite = getSpxSprite(true);
        spxSprite->setPaused(true);
        spxSprite->setUnitInterval(0.1f);
        spxSprite->setLoopCount(0);
        spxSprite->setPosition(0, 0);
        container->addChildLocked(spxSprite);
        
        obj_type = TYPE_ENEMY;
        
        float boxWidth = p_boxWidth;
        float boxHeight = p_boxHeight;
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(0), m_box2d->pixel2Meter(0));
        bd.userData = this;
        bd.type = b2_dynamicBody;
        
        b2PolygonShape staticBox;
        staticBox.SetAsBox(m_box2d->pixel2Meter(boxWidth*sGlobal->scaleX), m_box2d->pixel2Meter(boxHeight*sGlobal->scaleY));
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &staticBox;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.isSensor = false;
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_ENEMY]^CAT_BITS[TYPE_GROUND];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);
        body->SetGravityScale(0);
        
        bloodSprite = NULL;
        smokeSprite = NULL;
        shadow = NULL;
        isInitToShow = true;
        
        stunnable = false;
    }
    
    ~ShootableObject() {
        
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        if (isDead) {
            spxSprite->tick(dt);
            container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
            return;
        }
        
        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
    }
    
    void onHitHero() {
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
        isBaoing = true;
        isDead = true;
        wyAFCSpriteCallback callback = {
            NULL,
            onBombEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(11);
        spxSprite->setPaused(false);
        spxSprite->stopAllActions();
        sGlobal->mainGameLayer->addToDisable(body);
    }
    
    void handleCollision(b2Body *actorB) {
        
    }
    
    void dead() {
        if (isDead)
            return;
        
        if (sGlobal->nearestEnemy == this) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        }
        
        isBaoing = true;
        isDead = true;
        wyAFCSpriteCallback callback = {
            NULL,
            onBombEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(11);
        spxSprite->setPaused(false);
        spxSprite->stopAllActions();
        sGlobal->mainGameLayer->addToDisable(body);
    }
    
    void hurt(float y) {
        
    }
    
    void destroyMyself() {
        spxSprite->stopAllActions();
        spxSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(spxCacheArray, spxSprite);
        
        container->stopAllActions();
        container->removeAllChildren(true);
        sGlobal->pushToCache(sGlobal->containerCache, container);
        
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        delete this;
        
        return;
    }
    
    void beginContact(b2Body *actorB) {
        
    }
    
    void endContact(b2Body *actorB) {
        
    }
    
    bool canBeHurt() {
        return true;
    }
};

class EnemyBullet : public EnemyBulletBase {
public:
    bool needRotate;
    bool isDead;
    
    EnemyBullet() {
        
    }
    
    EnemyBullet(wyArray *p_spxCacheArray, int p_animIdle, float p_unitInterval, float p_myScaleX, float p_myScaleY) : EnemyBulletBase() {
        spxCacheArray = p_spxCacheArray;
        animIdle = p_animIdle;
        unitInterval = p_unitInterval;
        myScaleX = p_myScaleX;
        myScaleY = p_myScaleY;
        
        needRotate = false;
        
        postInit();
    }
    
    ~EnemyBullet() {
        
    }
    
    void postInit() {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        int e_id = ENEMY_XIANJING3;
        
        hp = 1;
        atk = 0;
        gold = 0;
        
        spxSprite = (wySPXSprite *)sGlobal->popFromCache(spxCacheArray);
        spxSprite->setForceTickMode(true);
        spxSprite->stopAllActions();
        spxSprite->setAFCSpriteCallback(NULL, NULL);
        spxSprite->playAnimation(animIdle);
        spxSprite->setUnitInterval(unitInterval);
        spxSprite->setScaleX(myScaleX);
        spxSprite->setScaleY(myScaleY);
        spxSprite->setVisible(true);
        spxSprite->setAlpha(255);
        spxSprite->setPaused(true);
        sGlobal->mainGameLayer->addChildLocked(spxSprite, 101);
        
        obj_type = TYPE_ENEMY_BULLET;
        
        float boxWidth = DP(5)*sGlobal->scaleX;
        float boxHeight = DP(5)*sGlobal->scaleX;
        b2BodyDef bd;
        bd.position.Set(m_box2d->pixel2Meter(DP(60)*sGlobal->scaleY), m_box2d->pixel2Meter(DP(50)*sGlobal->scaleY));
        bd.userData = this;
        bd.type = b2_dynamicBody;
        
        b2PolygonShape staticBox;
        staticBox.SetAsBox(m_box2d->pixel2Meter(boxWidth), m_box2d->pixel2Meter(boxHeight));
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &staticBox;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.isSensor = true;
        fixtureDef.filter.maskBits = MASK_BITS[TYPE_ENEMY_BULLET];
        fixtureDef.filter.categoryBits = CAT_BITS[TYPE_ENEMY_BULLET];
        fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_ENEMY_BULLET];
        
        body = world->CreateBody(&bd);
        body->CreateFixture(&fixtureDef);
        body->SetGravityScale(0.0f);
        
        offsetX = 0;
        offsetY = 0;
        
        isDead = false;
    }
    
    void update(float dt) {
        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        
        if (isDead) {
            spxSprite->tick(dt);
        } else {
            spxSprite->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
            if (needRotate) {
                b2Vec2 vel = body->GetLinearVelocity();
                float angle = atan(vel.y/vel.x);
                spxSprite->setRotation(-wyMath::r2d(angle)+10);
            }
        }


        
//        b2Vec2 vel = body->GetLinearVelocity();
//        float angle = atan(vel.y/vel.x);
//        if (vel.x > 0) {
//            angle += M_PI;
//        }
//        spxSprite->setRotation(-wyMath::r2d(angle));
        
//        float dist = m_box2d->meter2Pixel(body->GetPosition().x - sGlobal->mainGameLayer->hero->body->GetPosition().x);
//        if (dist < DP(200)) {
//            spxSprite->stopAllActions();
//            float maxSpeed = 10.0f;
//            b2Vec2 desiredVelocity = sGlobal->mainGameLayer->hero->body->GetPosition() - body->GetPosition()-b2Vec2(0,m_box2d->pixel2Meter(DP(20)*sGlobal->scaleY));
//            if (maxSpeed > 0)
//            {
//                desiredVelocity.Normalize();
//                desiredVelocity *= maxSpeed;
//            }
//            b2Vec2 finalVelocity = desiredVelocity - body->GetLinearVelocity();
//            b2Vec2 steerVector = b2Vec2(finalVelocity.x, finalVelocity.y);
//            body->ApplyLinearImpulse(steerVector, body->GetPosition());
//            
//            //        if (sub_type == -1) {
//            //            b2Vec2 vel = body->GetLinearVelocity();
//            //            float angle = atan(vel.y/vel.x);
//            //            body->SetTransform(body->GetPosition(), angle);
//            //            spxSprite->setRotation(-angle*57.3f);
//            //        }
//            
//            spxSprite->setPosition(m_box2d->meter2Pixel(body->GetPosition().x), m_box2d->meter2Pixel(body->GetPosition().y));
//            b2Vec2 vel = body->GetLinearVelocity();
//            float angle = atan(vel.y/vel.x);
//            if (vel.x > 0) {
//                angle += M_PI;
//            }
//            spxSprite->setRotation(-wyMath::r2d(angle));
//        } else {
//            body->SetTransform(b2Vec2(m_box2d->pixel2Meter(spxSprite->getPositionX()), m_box2d->pixel2Meter(spxSprite->getPositionY())), -wyMath::d2r(spxSprite->getRotation()));
//        }
    }
    
    static EnemyBullet *make(wyArray *p_spxCacheArray, int p_animIdle, float p_unitInterval, float p_myScaleX, float p_myScaleY) {
        return WYNEW EnemyBullet(p_spxCacheArray, p_animIdle, p_unitInterval, p_myScaleX, p_myScaleY);
    }
    
    static void onDisappearEnd(wyAction* action, void* data) {
//        EnemyBullet *enemyBullet = (EnemyBullet *) data;
//        
//        enemyBullet->spxSprite->stopAllActions();
//        enemyBullet->spxSprite->setAFCSpriteCallback(NULL, NULL);
//        sGlobal->pushToCache(enemyBullet->spxCacheArray, enemyBullet->spxSprite);
//        
//        delete enemyBullet;
    }
    
    void destroyMyself() {
//        wyFadeOut* fade = wyFadeOut::make(0.1f, true);
//        wyActionCallback callback = {
//            NULL,
//            onDisappearEnd,
//            NULL
//        };
//        fade->setCallback(&callback, this);
//        spxSprite->runAction(fade);
        
        spxSprite->stopAllActions();
        spxSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(spxCacheArray, spxSprite);

        wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
        b2World* world = m_box2d->getWorld();
        
        world->DestroyBody(body);
        
        delete this;
        
        return;
    }
    
    static void onBulletAnimEnded(wyAFCSprite* sprite, void* data) {
        EnemyBullet *enemyBullet = (EnemyBullet *) data;
//        LOGE("onBulletAnimEnded");
        enemyBullet->spxSprite->setVisible(false);
        sGlobal->mainGameLayer->addToDestroy(enemyBullet->body);
    }
    
    void onHitHero() {
//        LOGE("hit hero");
        if (isDead) {
            return;
        }
        isDead = true;

        wyAFCSpriteCallback callback = {
            NULL,
            onBulletAnimEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
//        spxSprite->setUnitInterval(1.0f);
//        spxSprite->playAnimation(animIdle);
        spxSprite->setPaused(false);
        
        sGlobal->mainGameLayer->addToDisable(body);
    }
};


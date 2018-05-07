//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "Enemy.h"
#include "Global.h"
#include "HeroBullet.h"

static void onHitAnimationEnded(wyAnimation* anim, void* data) {
    wyNode *s = (wyNode *)data;
    s->setVisible(false);
}

Enemy::Enemy(int x, int y, int appearType, wyTMXObjectGroup* enemyGroup, wyTMXObject* enemy, const char *myCmd, bool needBlood){
    preInit(x, y, appearType, enemyGroup, enemy, myCmd, needBlood);
//    initData(x, y, appearType, enemyGroup, enemy, myCmd, needBlood);
//    afterInit(x, y, appearType, enemyGroup, enemy, myCmd, needBlood);
}

Enemy::~Enemy() {
    if (cmdList!=NULL) {
        delete [] cmdList;
    }
    if (cmdArg1List!=NULL) {
        delete [] cmdArg1List;
    }
    if (cmdArg2List!=NULL) {
        delete [] cmdArg2List;
    }
    if (cmdArg3List!=NULL) {
        delete [] cmdArg3List;
    }
//    LOGE("delete removeMaskBitArray...");
    wyArrayDestroy(removeMaskBitArray);
    if (bloodSplitArray != NULL) {
        wyArrayDestroy(bloodSplitArray);
    }
}

void Enemy::preInit(int x, int y, int appearType, wyTMXObjectGroup* enemyGroup, wyTMXObject* enemy, const char *myCmd, bool needBlood) {
    offsetX = 0;
    offsetY = 0;
    sub_type = -1;
    deadType = DEAD_TYPE_DART;
	isMasking = false;
    deadSFX = -1;
    combatValue = 1;
    needShowDeadEffect = true;
    stunnable = true;
    faintSpriteScale = 0.7*sGlobal->scaleX;
    faintSpriteX = DP(-10)*sGlobal->scaleY;
    faintSpriteY = DP(50)*sGlobal->scaleY;
    shadow = NULL;
    contactNum = 0;
    isHurting = false;
    xuecaoSx = 0;
    xuecaoOfy = 0;
    
    this->needBlood = needBlood;
//    if (needBlood) {
//        bloodSprite = wySPXSprite::make("effects"), MyTextureManager::makePNG("effects")), SPX_PENXUE_1);
//        sGlobal->mainGameLayer->addChildLocked(bloodSprite, 103);
//        bloodSprite->setForceTickMode(true);
//        bloodSprite->setUnitInterval(0.1f);
//        bloodSprite->setLoopCount(0);
//        bloodSprite->setFlipX(false);
//        bloodSprite->setScaleX(1.8f*sGlobal->scaleX);
//        bloodSprite->setScaleY(1.8f*sGlobal->scaleY);
//        bloodSprite->setVisible(false);
//        wyAFCSpriteCallback callback1 = {
//            NULL,
//            onBloodEnded
//        };
//        bloodSprite->setAFCSpriteCallback(&callback1, bloodSprite);
//    }
    
    deadSprite = NULL;
    
    isHurtByFire = false;
    
    this->appearType = appearType;
    appearDistance = -1;
    cmdCount = 0;
    curCmd = 0;
    cmdList = NULL;
    cmdArg1List = NULL;
    cmdArg2List = NULL;
    cmdArg3List = NULL;
    jx = -1;
    jy = -1;
    const char *cmd = NULL;
    if (myCmd != NULL) {
        cmd = myCmd;
    } else {
        if (enemyGroup != NULL && enemy != NULL) {
            cmd = enemyGroup->getObjectProperty(enemy, "cmd");
        }
    }
    char arg0[20];
    float arg1, arg2, arg3;
    
    if (cmd != NULL) {
        //        LOGE("%s", cmd);
        cmdCount = 0;
        curCmd = 0;
        cmdList = new int[10];
        cmdArg1List = new float[10];
        cmdArg2List = new float[10];
        cmdArg3List = new float[10];
        
        std::string line = string(cmd);
        int index = line.find(";");
        while (index != -1) {
            std::string cmd0 = line.substr(0, index);
            int index1 = cmd0.find(",");
            if (index1 == -1) {
                sprintf(arg0, "%s", cmd0.c_str());
                arg1 = -1;
                arg2 = -1;
                arg3 = -1;
            } else {
                sprintf(arg0, "%s", cmd0.substr(0, index1).c_str());
                
                cmd0 = cmd0.substr(index1 + 1, cmd0.length());
                index1 = cmd0.find(",");
                if (index1 == -1) {
                    arg1 = atof(cmd0.c_str());
                    arg2 = -1;
                    arg3 = -1;
                } else {
                    arg1 = atof(cmd0.substr(0, index1).c_str());
                    cmd0 = cmd0.substr(index1 + 1, cmd0.length());
                    index1 = cmd0.find(",");
                    if (index1 == -1) {
                        arg2 = atof(cmd0.c_str());
                        arg3 = -1;
                    } else {
                        arg2 = atof(cmd0.substr(0, index1).c_str());
                        cmd0 = cmd0.substr(index1 + 1, cmd0.length());
                        arg3 = atof(cmd0.c_str());
                    }
                }
            }
            
            if (strcmp(arg0, "n") == 0) {
                cmdList[cmdCount] = APPEAR_NORMAL;
                this->appearType = APPEAR_NORMAL;
                if (fabs(arg1+1.0f) > 0.000001) {
                    appearDistance = sGlobal->virtualWinWidth*arg1;
                } else {
                    appearDistance = arg1;
                }
                cmdArg1List[cmdCount] = appearDistance;
                cmdArg2List[cmdCount] = arg2;
                cmdArg3List[cmdCount] = arg3;
            }
            else if (strcmp(arg0, "fn") == 0) {
                cmdList[cmdCount] = APPEAR_FLY_NORMAL;
                this->appearType = APPEAR_FLY_NORMAL;
                if (fabs(arg1+1.0f) > 0.000001) {
                    appearDistance = sGlobal->virtualWinWidth*arg1;
                } else {
                    appearDistance = arg1;
                }
                cmdArg1List[cmdCount] = appearDistance;
                cmdArg2List[cmdCount] = arg2;
                cmdArg3List[cmdCount] = arg3;
            }
            else if (strcmp(arg0, "s") == 0) {
                cmdList[cmdCount] = APPEAR_SMOKE;
                this->appearType = APPEAR_SMOKE;
                if (fabs(arg1+1.0f) > 0.000001) {
                    appearDistance = sGlobal->virtualWinWidth*arg1;
                } else {
                    appearDistance = arg1;
                }
                cmdArg1List[cmdCount] = appearDistance;
                cmdArg2List[cmdCount] = arg2;
                cmdArg3List[cmdCount] = arg3;
            }
            else if (strcmp(arg0, "sf") == 0) {
                cmdList[cmdCount] = APPEAR_SMOKE_FIRE;
                this->appearType = APPEAR_SMOKE_FIRE;
                if (fabs(arg1+1.0f) > 0.000001) {
                    appearDistance = sGlobal->virtualWinWidth*arg1;
                } else {
                    appearDistance = arg1;
                }
                cmdArg1List[cmdCount] = appearDistance;
                cmdArg2List[cmdCount] = arg2;
                cmdArg3List[cmdCount] = arg3;
            }
            else if (strcmp(arg0, "fz") == 0) {
                cmdList[cmdCount] = APPEAR_FAZHEN;
                this->appearType = APPEAR_FAZHEN;
                if (fabs(arg1+1.0f) > 0.000001) {
                    appearDistance = sGlobal->virtualWinWidth*arg1;
                } else {
                    appearDistance = arg1;
                }
                cmdArg1List[cmdCount] = appearDistance;
                cmdArg2List[cmdCount] = arg2;
                cmdArg3List[cmdCount] = arg3;
            }
            else if (strcmp(arg0, "jb") == 0) {
                cmdList[cmdCount] = APPEAR_JUMP_FROM_BACK;
                this->appearType = APPEAR_JUMP_FROM_BACK;
                if (fabs(arg1+1.0f) > 0.000001) {
                    appearDistance = sGlobal->virtualWinWidth*arg1;
                } else {
                    appearDistance = arg1;
                }
                jx = arg2;
                jy = arg3;
                cmdArg1List[cmdCount] = appearDistance;
                cmdArg2List[cmdCount] = jx;
                cmdArg3List[cmdCount] = jy;
            }
            else if (strcmp(arg0, "jf") == 0) {
                cmdList[cmdCount] = APPEAR_JUMP_FROM_FRONT;
                this->appearType = APPEAR_JUMP_FROM_FRONT;
                if (fabs(arg1+1.0f) > 0.000001) {
                    appearDistance = sGlobal->virtualWinWidth*arg1;
                } else {
                    appearDistance = arg1;
                }
                jx = arg2;
                jy = arg3;
                cmdArg1List[cmdCount] = appearDistance;
                cmdArg2List[cmdCount] = jx;
                cmdArg3List[cmdCount] = jy;
            }
            else if (strcmp(arg0, "w") == 0) {
                cmdList[cmdCount] = CMD_WALK;
                cmdArg1List[cmdCount] = arg1;
                cmdArg2List[cmdCount] = arg2;
                if (fabs(arg3+1.0f) > 0.000001) {
                    cmdArg3List[cmdCount] = arg3;
                } else {
                    cmdArg3List[cmdCount] = 0.0f;
                }
            }
            else if (strcmp(arg0, "j") == 0) {
                cmdList[cmdCount] = CMD_JUMP;
                cmdArg1List[cmdCount] = arg1;
                cmdArg2List[cmdCount] = arg2;
                cmdArg3List[cmdCount] = arg3;
            }
            
            cmdCount++;
            line = line.substr(index + 1, line.length());
            index = line.find(";");
        }
    } else {
        appearDistance = -1;
        cmdCount = 0;
        curCmd = 0;
        cmdList = NULL;
        cmdArg1List = NULL;
        cmdArg2List = NULL;
        cmdArg3List = NULL;
        jx = -1;
        jy = -1;
    }
    
    isJumping = false;
    
    smokeSprite = NULL;
    switch (this->appearType) {
        case APPEAR_SMOKE_FIRE:
        case APPEAR_SMOKE: {
            if (fabs(appearDistance+1.0f) < 0.000001) {
                appearDistance = wyDevice::winWidth-sGlobal->hero_offset_left-randRange(DP(20), DP(30))/0.75*sGlobal->scaleX;
            }
            break;
        }
        case APPEAR_FAZHEN: {
            if (fabs(appearDistance+1.0f) < 0.000001) {
                appearDistance = wyDevice::winWidth-sGlobal->hero_offset_left-randRange(DP(20), DP(30))/0.75*sGlobal->scaleX;
            }
            break;
        }
        case APPEAR_FLY_NORMAL: {
            if (fabs(appearDistance+1.0f) < 0.000001) {
                appearDistance = wyDevice::winWidth;
            }
            isCmdFly = true;
            break;
        }
        case APPEAR_NORMAL: {
            if (fabs(appearDistance+1.0f) < 0.000001) {
                appearDistance = wyDevice::winWidth;
            }
            break;
        }
        case APPEAR_JUMP_FROM_BACK: {
            if (fabs(appearDistance+1.0f) < 0.000001) {
                appearDistance = wyDevice::winWidth-sGlobal->hero_offset_left-randRange(DP(20), DP(30))/0.75*sGlobal->scaleX;
            }
            if (fabs(jx+1.0f) < 0.000001f) {
                jx = 0;
            }
            if (fabs(jy+1.0f) < 0.000001f) {
                jy = 50;
            }
            isJumping = true;
            isCmdJump = true;
            break;
        }
        case APPEAR_JUMP_FROM_FRONT: {
            if (fabs(appearDistance+1.0f) < 0.000001) {
                appearDistance = wyDevice::winWidth-sGlobal->hero_offset_left-randRange(DP(20), DP(30))/0.75*sGlobal->scaleX;
            }
            if (fabs(jx+1.0f) < 0.000001f) {
                jx = 0;
            }
            if (fabs(jy+1.0f) < 0.000001f) {
                jy = 50;
            }
            isJumping = true;
            isCmdJump = true;
            break;
        }
        default:
            break;
    }
    isAppeared = false;
    isSmokeEnd = false;
    
    isReadyToCmd = false;
    if (cmdCount > 0) {
        isCmdFinished = false;
    } else {
        isCmdFinished = true;
    }
    
    jumpingTimer = 0;
	isTiming = false;
	cmdTimer = 0;
	isDone = false;
    isZorderSet = false;
    
    animJump = -1;
    animLanding = -2;
    animDead = -3;
    animWalk = -4;
    animHurt = -5;
    animIdle = -6;
    animWalkBack = -7;
    
    isDead = false;
    needDeadBlink = true;
    needStay = false;
    
    preVelX = -1;
    isKnockingBack = false;
    knockingBackTimer = 0;

    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    rangeX = m_box2d->pixel2Meter(DP(100)*sGlobal->scaleX);
    isLandingEnded = false;
    nextWanderInterval = 0;
    wanderTimer = 0;
    
    isInitToShow = false;
    isAttacking = false;
    
    isStunning = false;
    faintSprite = NULL;
    
    isInScreen = false;
    isWandering = false;
    
    needAttack = (rand()%2 != 0);
    if (sGlobal->index == 0) {
        needAttack = false;
    }
    
    shadowSX = 3.0f;
    shadowSY = 1.6f;
    shadowPosX = 0;
    shadowPosY = 0;
    
    isFighted = false;
    fightDistance = 480;
    
    canKnockBack = true;
    canWander = true;
    
    removeMaskBitArray = wyArrayNew(10);
    
    bloodSplitArray = NULL;
}

void Enemy::afterInit(int x, int y, int appearType, wyTMXObjectGroup* enemyGroup, wyTMXObject* enemy, const char *myCmd, bool needBlood) {
    if (sGlobal->isRealEndless) {
        switch (sGlobal->combatRound) {
            case 1:
                hp = sGlobal->ENEMY_HP_EASY[e_id];
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                break;
            case 2:
                hp = sGlobal->ENEMY_HP_HARD[e_id];
                atk = sGlobal->ENEMY_ATK_HARD[e_id];
                gold = sGlobal->ENEMY_GOLD_HARD[e_id];
                break;
            case 3:
                hp = sGlobal->ENEMY_HP_CRAZY[e_id];
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id];
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id];
                break;
            default:
                hp = sGlobal->ENEMY_HP_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_HP_POWER, sGlobal->combatRound - 3);
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_ATK_POWER, sGlobal->combatRound - 3);
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_GOLD_POWER, sGlobal->combatRound - 3);
                break;
        }
    } else {
        int lvId = (sGlobal->index-1)%10;
        switch (sGlobal->currentSModeDifficulty) {
            case DIFFICULTY_NORMAL:
                hp = sGlobal->ENEMY_HP_EASY[e_id] + sGlobal->ENEMY_HP_EASY_INC[e_id]*lvId;
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                break;
            case DIFFICULTY_HEROIC:
                hp = sGlobal->ENEMY_HP_HARD[e_id] + sGlobal->ENEMY_HP_HARD_INC[e_id]*lvId;
                atk = sGlobal->ENEMY_ATK_HARD[e_id];
                gold = sGlobal->ENEMY_GOLD_HARD[e_id];
                break;
            case DIFFICULTY_HARD:
                hp = sGlobal->ENEMY_HP_CRAZY[e_id] + sGlobal->ENEMY_HP_CRAZY_INC[e_id]*lvId;
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id];
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id];
                break;
            default:
                hp = sGlobal->ENEMY_HP_EASY[e_id] + sGlobal->ENEMY_HP_EASY_INC[e_id]*lvId;
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                break;
        }
    }
    atk *= sGlobal->ENEMY_ATK_MULTI;
    hp *= sGlobal->ENEMY_HP_MULTI;
    gold *= sGlobal->ENEMY_GOLD_MULTI;
    if (sGlobal->isRealEndless) {
//        LOGE("e_id:%d, combat round:%d, hp:%d, atk:%d, gold:%d", e_id, sGlobal->combatRound, hp, atk, gold);
    } else {
//        LOGE("diff:%d, e_id:%d, atk:%d, hp:%d, gold:%d", sGlobal->currentSModeDifficulty, e_id, atk, hp, gold);
    }
    
    maxHp = hp;
    obj_type = TYPE_ENEMY;
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
//    container = wyNode::make();
//    container->setVisible(false);
//    sGlobal->mainGameLayer->addChildLocked(container);
    
    wySPXSprite *enemySprite = getSpxSprite(false);
    spxSprite = enemySprite;
    
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y));
    bd.userData = this;
    bd.type = b2_dynamicBody;
    body = world->CreateBody(&bd);
    body->SetTransform(bd.position, 0);
    
    bodyRect = enemySprite->getFrameRect();
    bodyRect.x *= enemySprite->getScaleX();
    bodyRect.y *= enemySprite->getScaleY();
    bodyRect.width *= enemySprite->getScaleX();
    bodyRect.height *= enemySprite->getScaleY();
    
    spxScaleX = SPX_SCALE_X;
    createFixtureByCurrentFrame();
    body->SetActive(false);
    body->SetAwake(false);
}

void Enemy::initToShow() {
    container = (wyNode *)sGlobal->popFromCache(sGlobal->containerCache);
    container->setVisible(false);
    sGlobal->mainGameLayer->addChildLocked(container);
    
    spxSprite = getSpxSprite(true);
    spxSprite->setForceTickMode(true);
    spxSprite->setPosition(0, 0);
    spxSprite->setVisible(true);
    container->addChildLocked(spxSprite);
    
    if (needBlood) {
        bloodSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        sGlobal->mainGameLayer->addChildLocked(bloodSprite, 103);
        bloodSprite->setForceTickMode(true);
        bloodSprite->setUnitInterval(0.1f);
        bloodSprite->setLoopCount(0);
        bloodSprite->setFlipX(true);
        bloodSprite->setScaleX(2.0f*sGlobal->scaleX);
        bloodSprite->setScaleY(2.4f*sGlobal->scaleY);
        bloodSprite->setVisible(false);
        wyAFCSpriteCallback callback1 = {
            NULL,
            onBloodEnded
        };
        bloodSprite->setAFCSpriteCallback(&callback1, bloodSprite);
        bloodSprite->playAnimation(SPX_PENXUE_1);//
        
        bloodSplitArray = wyArrayNew(10);
    }
    
    switch (appearType) {
        case APPEAR_FAZHEN:
        case APPEAR_SMOKE:
        case APPEAR_SMOKE_FIRE: {
            smokeSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
            smokeSprite->setForceTickMode(true);
            smokeSprite->setUnitInterval(0.1f);
            smokeSprite->setLoopCount(0);
            smokeSprite->setScaleX(sGlobal->scaleX*1.2f);
            smokeSprite->setScaleY(sGlobal->scaleY*1.2f);
            smokeSprite->setVisible(false);
            sGlobal->mainGameLayer->addChildLocked(smokeSprite, 1);
            wyAFCSpriteCallback callback2 = {
                NULL,
                onSmokeAppearEnded
            };
            smokeSprite->setAFCSpriteCallback(&callback2, this);
//            if (appearType == APPEAR_SMOKE_FIRE) {
//                smokeSprite->setColor(wyc4b(200, 0, 0, 220));
//            }
            smokeSprite->playAnimation(SPX_XIAOGUAI_YANWU);
            break;
        }
        default:
            break;
    }
    
//    faintSprite = NULL;
    
    shadow = (wySpriteEx*) sGlobal->popFromCache(sGlobal->shadowSpriteCache);
    shadow->setTextureRect(wyZwoptexManager::getInstance()->getFrameRect("dialog_deco", "shadow.png"));
    shadow->setScaleX(sGlobal->scaleX*shadowSX);
    shadow->setScaleY(sGlobal->scaleY*shadowSY);
    container->addChildLocked(shadow, -1);
    shadow->setVisible(false);
    shadow->setFlipX(false);
    shadow->setAlpha(100);
    shadow->setPosition(shadowPosX, shadowPosY);//-bodyRect.height/2+DP(4)*sGlobal->scaleY
    
    xuecao = NULL;
//    xuecao = (wyProgressTimer*) sGlobal->popFromCache(sGlobal->xuecaoCache);
//    xuecao->setStyle(HORIZONTAL_BAR_LR);
//    xuecao->setAnchor(0.5f, 0);
//    xuecao->setScaleX(xuecaoSx);
//    xuecao->setScaleY(sGlobal->scaleY*0.4f);
//    xuecao->setPercentage(100);
//    container->addChildLocked(xuecao, 200);
//    xuecao->setVisible(true);
//    xuecao->setPosition(xuecaoOfx, xuecaoOfy);
}

wySPXSprite *Enemy::getSpxSprite(bool isGet) {
    wySPXSprite *enemySprite = NULL;
    
//    LOGE("cache len:%d", spxCacheArray->num);
    if (isGet) {
        enemySprite = (wySPXSprite *)sGlobal->popFromCache(spxCacheArray);
    } else {
        enemySprite = (wySPXSprite *)wyArrayGet(spxCacheArray, 0);
    }
    enemySprite->stopAllActions();
    enemySprite->setAFCSpriteCallback(NULL, NULL);
    enemySprite->playAnimation(animIdle);
    enemySprite->setUnitInterval(unitInterval);
    enemySprite->setScaleX(myScaleX);
    enemySprite->setScaleY(myScaleY);
    enemySprite->setRotation(0);
    enemySprite->setAlpha(255);
    enemySprite->setFlipX(false);
    enemySprite->setVisible(true);
    
    return enemySprite;
}

static void onSplitBloodEnded(wyAFCSprite* sprite, void* data) {
    Enemy *enemy = ((Enemy *) data);
    if (sprite != NULL) {
        sprite->setFlipX(false);
        sprite->stopAllActions();
        sprite->setAFCSpriteCallback(NULL, NULL);
        wyArrayDeleteObj(enemy->bloodSplitArray, sprite, NULL, NULL);
        sGlobal->pushToCache(sGlobal->effects_cache, sprite);
    }
}

void Enemy::splitBlood(float y) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    wySPXSprite *blood = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
    container->addChildLocked(blood, 103);
    blood->setForceTickMode(true);
    blood->setUnitInterval(0.1f);
    blood->setLoopCount(0);
    blood->setFlipX(true);
    blood->setScaleX(2.0f*sGlobal->scaleX);
    blood->setScaleY(2.5f*sGlobal->scaleX);
    blood->setVisible(true);
    wyAFCSpriteCallback callback1 = {
        NULL,
        onSplitBloodEnded
    };
    blood->setAFCSpriteCallback(&callback1, this);
    blood->playAnimation(rand()%2+SPX_PENXUE_1);//rand()%2+SPX_PENXUE_1
    int fuhao = rand()%2 == 0 ? 1 : -1;
    blood->setPosition(fuhao*DP(rand()%20)*sGlobal->scaleX, m_box2d->meter2Pixel(y)-container->getPositionY()-offsetY-DP(10)*sGlobal->scaleY);
    
    int rotation = randRange(0,40);
    if (rand()%5 == 0) {
        rotation*=-1;
    }
    blood->setRotation(rotation);
    
    wyArrayPush(bloodSplitArray, blood);
}

void Enemy::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (!isInitToShow) {
        isInitToShow = true;
        initToShow();
        return;
    }
    
    if (!isInScreen && distance < wyDevice::winWidth-sGlobal->hero_offset_left-DP(10)*sGlobal->scaleX) {
        isInScreen = true;
    }
    
    if (sGlobal->mainGameLayer->hero->robertStatus == R_APPEARING && !isDead && isAppeared) {// 
        float dist = distance;//body->GetPosition().x - sGlobal->mainGameLayer->hero->body->GetPosition().x;
//        LOGE("dist:%f, nearest dist: %f", dist, sGlobal->nearestDistance);
        if (dist <= -DP(50)*sGlobal->scaleX) {
            sGlobal->nearestEnemy = NULL;
            sGlobal->nearestDistance = 1000;
        } else {
            if (dist < sGlobal->nearestDistance && dist > 0) {
                sGlobal->nearestEnemy = this;
                sGlobal->nearestDistance = dist;
//                LOGE("nearest %d, %f", e_id, dist);
            }
        }
    }
    
    for (int i = 0; i < bloodSplitArray->num; i++) {
        wySPXSprite *blood = (wySPXSprite*)wyArrayGet(bloodSplitArray, i);
        blood->tick(dt);
    }
    
    if (isKnockingBack) {
        knockingBackTimer += dt;
        if (knockingBackTimer > 0.25f) {
            isKnockingBack = false;
            body->SetLinearVelocity(b2Vec2(preVelX, body->GetLinearVelocity().y));
            knockingBackTimer = 0;
            preVelX = -1;
        } 
        else if (knockingBackTimer > 0.15f){
            body->SetLinearVelocity(b2Vec2(0, body->GetLinearVelocity().y));
        }
    }
    
    float prevY = container->getPositionY();
    
    if (isDead) {
        if (!isKnockingBack) {
            body->SetLinearVelocity(b2Vec2(0,0));
        }
        
		spxSprite->tick(dt);
		container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);

        if (bloodSprite->isVisible()) {
            bloodSprite->tick(dt);
        }
        return;
    }
    
    switch (appearType) {
        case APPEAR_FLY_NORMAL:
        case APPEAR_NORMAL: {
            if (distance < appearDistance) {
                if (!container->isVisible()) {
                    container->setVisible(true);
                    sGlobal->mainGameLayer->addToEnable(body);
                    
                    if (appearType == APPEAR_FLY_NORMAL) {
                        isAppeared = true;
                        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                        curCmd++;
                        
                        if (cmdCount > 0 && !isCmdFinished && !isReadyToCmd && isAppeared) {
                            isReadyToCmd = true;
                        }
                        playAnim(animIdle);
                    }
                }
//                spxSprite->tick(dt);
//                container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
            }
            break;
        }
        case APPEAR_FAZHEN:
        case APPEAR_SMOKE_FIRE:
        case APPEAR_SMOKE: {
            if (distance < appearDistance) {
                if (!isSmokeEnd) {
                    if (smokeSprite != NULL && !smokeSprite->isVisible()) {
                        smokeSprite->setVisible(true);
                        smokeSprite->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY - DP(30)*sGlobal->scaleY);
//                        if (appearType == APPEAR_FAZHEN) {
                            container->setVisible(true);
                            spxSprite->runAction(wyFadeIn::make(0.5f));
                        
                            wyAction *action = wyScaleTo::make(0.5f, 0.6f, 1.0f);
                            wyActionCallback callback = {
                                NULL,
                                onHeiDongAppearEnded,
                                NULL
                            };
                            action->setCallback(&callback, this);
                            container->runAction(action);

//                        }
                    }
                    if (smokeSprite->isVisible()) {
                        smokeSprite->tick(dt);
                    }
                } else {
                    if (smokeSprite != NULL) {
                        smokeSprite->setVisible(false);
                    }
                }
            }
            break;
        }
        case APPEAR_JUMP_FROM_FRONT:
        case APPEAR_JUMP_FROM_BACK: {
            if (distance < appearDistance) {
                if (!container->isVisible()) {
                    container->setVisible(true);
                    sGlobal->mainGameLayer->addToEnable(body);
                    
                    b2Vec2 f = body->GetWorldVector(b2Vec2(jx, jy));
                    b2Vec2 p = body->GetWorldPoint(b2Vec2(0.0f, 0.0f));
                    body->ApplyLinearImpulse(f, p);
                    //LOGE("start jump %f, %f", jx, jy);
                    isJumping = true;
                    jumpingTimer = 0;
                    
                    playAnim(animJump);
                    
                    if (appearType == APPEAR_JUMP_FROM_BACK) {
                        sGlobal->mainGameLayer->reorderChild(container, sGlobal->mainGameLayer->m_TileMap->getZOrder() - 1);
                    }
                }
                if (isJumping) {
                    jumpingTimer += dt;
                    if (jumpingTimer >= 0.5f) {
                        isJumping = false;
                        jumpingTimer = 0;
                    }
                }
//                spxSprite->tick(dt);
                if (appearType == APPEAR_JUMP_FROM_BACK) {
                    if (body->GetLinearVelocity().y < 0 && !isZorderSet) {
                        sGlobal->mainGameLayer->reorderChild(container, sGlobal->mainGameLayer->m_TileMap->getZOrder() + 1);
                        isZorderSet = true;
                    }
                }
//                container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
            }
            break;
        }
        default:
            break;
    }
    
    spxSprite->tick(dt);
    if (spxSprite->isFlipX()) {
        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) - offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
    } else {
        container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
    }
    
//    if (hitSprite->isVisible()) {
////        bloodSprite->tick(dt);
//        hitSprite->setPosition(container->getPositionX(), hitSprite->getPositionY() + (container->getPositionY() - prevY));
//    }
    
    if (bloodSprite != NULL && bloodSprite->isVisible()) {
        bloodSprite->tick(dt);
        bloodSprite->setPosition(container->getPositionX()-DP(0)*sGlobal->scaleX, bloodSprite->getPositionY() + (container->getPositionY() - prevY));
    }
    
    if (isStunning) {
        stunTimer += dt;
        if (stunTimer > sGlobal->SKILL_STUN_DUR) {
            isStunning = false;
            stunTimer = 0;
            faintSprite->setVisible(false);
        } else {
            faintSprite->tick(dt);
        }
    } else {
        doCmd(dt);
        
        if (isLandingEnded && isCmdFinished && !isAttacking && isAppeared && !isHurting) {
            wander(dt);
        }
    }
    
    for (int i = 0; i < removeMaskBitArray->num; i++) {
//        LOGE("removeMaskBitArray:%d", removeMaskBitArray->num);
        RemoveMaskBitTask *task = (RemoveMaskBitTask*)wyArrayGet(removeMaskBitArray, i);
        bool isFinished = task->updateTask(dt);
        if (isFinished) {
            clearBulletMask(task->m_maskBit);
            wyArrayDeleteObj(removeMaskBitArray, task, NULL, NULL);
            task->release();
        }
    }
}

void Enemy::doCmd(float dt) {
    if (isReadyToCmd && !isCmdFinished) {
//		LOGE("do cmd");
        if (curCmd == cmdCount) {
            isCmdFinished = true;
            isTiming = false;
            return;
        }
        
//		LOGE("cmd: %d, %d", curCmd, cmdList[curCmd]);
        switch (cmdList[curCmd]) {
            case CMD_WALK:
                if (isTiming) {
                    cmdTimer += dt;
                    if (cmdTimer >= cmdArg1List[curCmd]) {
                        isTiming = false;
                        cmdTimer = 0;
                        curCmd++;
                        isDone = false;
                        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                        //LOGE("do Move 10 end.");
//                        sGlobal->mainGameLayer->removeChildLocked(run_emitter, true);
//                        run_emitter = NULL;
                        //yan->setVisible(false);
//                        LOGE("cmd: %d", spxSprite->getCurrentAnimationIndex());
                        if (spxSprite->getCurrentAnimationIndex() == animWalk || spxSprite->getCurrentAnimationIndex() == animWalkBack) {
                            playAnim(animIdle);
                        }
                    }
//                    if (run_emitter != NULL) {
//                        run_emitter->setPosition(sprite->getPositionX(), sprite->getPositionY());
//                    }
                } else {
                    isTiming = true;
                    cmdTimer = 0;
                    body->SetLinearVelocity(b2Vec2(cmdArg2List[curCmd], cmdArg3List[curCmd]));
					//if (sub_type != 100) {
					//	if (cmdArg2List[curCmd] > 0.0000001f) {
					//		yan->setFlipX(false);
					//		yanOffsetX = -DP(20)*sGlobal->scaleX;
					//		//                        ((wySPXSprite *) sprite)->setFlipX(true);
					//		yan->setVisible(true);
					//	} 
					//	else if (cmdArg2List[curCmd] < -0.0000001f) {
					//		yan->setFlipX(true);
					//		yanOffsetX = DP(30)*sGlobal->scaleX;
					//		//                        ((wySPXSprite *) sprite)->setFlipX(false);
					//		yan->setVisible(true);
					//	} else {

					//	}
					//}
                    if (cmdArg2List[curCmd] > 0.1f && animWalkBack >= 0) {
                        playAnim(animWalkBack);
                    }
                    else {
                        if (cmdArg2List[curCmd] < -0.1f && animWalk >= 0) {
                            playAnim(animWalk);
                        } else {
                            playAnim(animIdle);
                        }
                    }
                }
                break;
            case CMD_JUMP: {
                if (isTiming) {
                    cmdTimer += dt;
                    if (cmdTimer >= 0.1f) {
                        isTiming = false;
                        cmdTimer = 0;
                    }
                } else {
                    if (!isDone) {
                        b2Vec2 f = body->GetWorldVector(b2Vec2(cmdArg1List[curCmd], cmdArg2List[curCmd]));
                        b2Vec2 p = body->GetWorldPoint(b2Vec2(0.0f, 0.0f));
                        body->ApplyLinearImpulse(f, p);
                        isTiming = true;
                        cmdTimer = 0;
                        //LOGE("do Jump %f, %f", cmdArg1List[curCmd], cmdArg2List[curCmd]);
                        isDone = true;
                        
                        playAnim(animJump);
                    }
                }
                break;
            }
            default:
				//LOGE("???");
                break;
        }
    }
}

void Enemy::wander(float dt) {
    if (!canWander) {
        return;
    }
    
    if ((sGlobal->index == 102 || sGlobal->index == 103) && !sGlobal->mainGameLayer->hero->isRunning) {
        if ((int)fabs(body->GetLinearVelocity().x) != 0) {
            body->SetLinearVelocity(b2Vec2(0, 0));
            spxSprite->setFlipX(false);
            playAnim(animIdle);
        }
        return;
    }
    
    wanderTimer += dt;
    isWandering = true;
    
    int seed = rand()%100;
    if (wanderTimer >= nextWanderInterval) {
        if (seed < 10) {
            body->SetLinearVelocity(b2Vec2(randRange(1, 3), 0));
            playAnim(animWalkBack);
            spxSprite->setFlipX(true);
        }
        else if (seed >= 10 && seed < 50) {
            body->SetLinearVelocity(b2Vec2(-randRange(1, 3), 0));
            playAnim(animWalk);
            spxSprite->setFlipX(false);
        }
        else {
            body->SetLinearVelocity(b2Vec2(0, 0));
            playAnim(animIdle);

            spxSprite->setFlipX((rand()%10 == 0));
        }
        
        nextWanderInterval = rand()%3+1.0f;
        wanderTimer = 0;
    }
}

void Enemy::handleCollision(b2Body *actorB) {
//    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
//    int tagB = userDataB->obj_type;
//    switch (tagB) {
//        case TYPE_GROUND: {
//            if (cmdCount > 0 && !isCmdFinished && !isReadyToCmd) {
//                isReadyToCmd = true;
//            }
//            break;
//        }
//        default:
//            break;
//    }
}

void Enemy::beginContact(b2Body *actorB) {
//    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
    int tagB = userDataB->obj_type;
    switch (tagB) {
        case TYPE_GROUND: {
//            LOGE("vely:%f", body->GetLinearVelocity().y);
            if (body->GetLinearVelocity().y < 0 && (body->GetPosition().y > userDataB->body->GetPosition().y) && !isDead) {
                if (!isAppeared && !isJumping) {
                    if ((this->appearType == APPEAR_SMOKE || this->appearType == APPEAR_SMOKE_FIRE) && !isSmokeEnd)
                        return;
                    isAppeared = true;
                    body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                    curCmd++;
//                    playAnim(animLanding, true);
                    onLandingEnded(spxSprite, this);
//                    LOGE("landing");
                }
                if (isReadyToCmd && cmdList != NULL && cmdList[curCmd] == CMD_JUMP && !isTiming && isDone) {
                    curCmd++;
                    isDone = false;
                    body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
//                    playAnim(animLanding);
                    onLandingEnded(spxSprite, this);
                    //LOGE("do Jump end");
                }
                
//                if (!shadow->isVisible()) {
                    shadow->setVisible(true);
//                }
                
                
//                LOGE("bc...");
            }
            
            onLandingEnded(spxSprite, this);
//            if (!isOnGround) {
//                LOGE("on ground...");
                contactNum++;
                isOnGround = true;
                orgX = body->GetPosition().x;
//            }
            break;
        }
        default:
            break;
    }
}

void Enemy::endContact(b2Body *actorB) {
    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
    int tagB = userDataB->obj_type;
    switch (tagB) {
        case TYPE_GROUND: {
            contactNum--;
            contactNum = MAX(contactNum, 0);
            if (contactNum == 0) {
                isOnGround = false;
                shadow->setVisible(false);
//                LOGE("ec...");
            }
            break;
        }
        default:
            break;
    }
}

void Enemy::destroyMyself() {
//    LOGE("e_id:%d is destroied...", e_id);
    if (sGlobal->nearestEnemy == this) {
        sGlobal->nearestEnemy = NULL;
        sGlobal->nearestDistance = 1000;
    }
//    LOGE("11");
    spxSprite->stopAllActions();
    spxSprite->setAFCSpriteCallback(NULL, NULL);
    sGlobal->pushToCache(spxCacheArray, spxSprite);
    
    if (shadow != NULL) {
        sGlobal->pushToCache(sGlobal->shadowSpriteCache, shadow);
    }
    
    if (bloodSprite != NULL) {
        bloodSprite->stopAllActions();
        bloodSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(sGlobal->effects_cache, bloodSprite);
    }
    
    if (smokeSprite != NULL) {
        smokeSprite->stopAllActions();
        smokeSprite->setAFCSpriteCallback(NULL, NULL);
        sGlobal->pushToCache(sGlobal->effects_cache, smokeSprite);
    }
    
    if (faintSprite != NULL) {
        faintSprite->stopAllActions();
        faintSprite->setAFCSpriteCallback(NULL, NULL);
        faintSprite->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
        sGlobal->pushToCache(sGlobal->effects_cache, faintSprite);
    }
    
    if (xuecao != NULL) {
        sGlobal->pushToCache(sGlobal->xuecaoCache, xuecao);
    }
    
    container->stopAllActions();
    container->removeAllChildren(true);
    sGlobal->pushToCache(sGlobal->containerCache, container);

    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    world->DestroyBody(body);

    this->release();
    
    isDestroied = true;
    
    return;
}

void Enemy::destroyMyselfWhenNotInit() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    world->DestroyBody(body);
    
    this->release();
    
    isDestroied = true;
    
    return;
}

bool Enemy::canBeHurt() {
//    if (appearType == APPEAR_SMOKE || appearType == APPEAR_SMOKE) {
//        if (!isAppeared && !isSmokeEnd) {
//            return false;
//        }
//    }
//    else if (appearType == APPEAR_JUMP_FROM_BACK || appearType == APPEAR_JUMP_FROM_FRONT) {
//        if (body->GetLinearVelocity().y > 0) {
//            return false;
//        }
//    }
    if (!isInScreen) {
        return false;
    }
    
    return true;
}

bool Enemy::canBeMissed() {
    if (appearType == APPEAR_SMOKE || appearType == APPEAR_SMOKE) {
        if (!isAppeared && !isSmokeEnd) {
            return true;
        }
    }
    else if (appearType == APPEAR_JUMP_FROM_BACK || appearType == APPEAR_JUMP_FROM_FRONT) {
        if (!isAppeared && body->GetLinearVelocity().y > 0) {//body->GetLinearVelocity().y > 0
            return true;
        }
    }
    return false;
}

static void onDelayEnd(wyAction* action, void* data) {
    wyNode* node = (wyNode*)data;
    node->setVisible(false);
}

void Enemy::dealHitByDart(float y, b2Body *bulletBody) {
    if (hp < 0 || !canBeHurt()) {
        return;
    }
    
    if (canBeMissed()) {
        return;
    } else {
        HeroBullet *bullet = (HeroBullet *)bulletBody->GetUserData();
        bullet->dealHitEnemy(y, this);
    }
}

void Enemy::dealHitByLiudan(b2Body *bulletBody) {
    if (hp < 0 || isHurtByFire || !canBeHurt() || canBeMissed()) {
        return;
    }
    
    LiuDan *liudan = (LiuDan *)bulletBody->GetUserData();
    
    isHurtByFire = true;
    
    if (liudan->m_isMoto || hp - sGlobal->skillMulti[SKILL_MISSLE] <= 0) {
        hp -= sGlobal->skillMulti[SKILL_MISSLE];
        if (liudan->m_isMoto) {
            deadType = DEAD_TYPE_MOTO;
        } else {
            deadType = DEAD_TYPE_MISSLE;
        }
        dead();
    } else {
        hp -= sGlobal->skillMulti[SKILL_MISSLE];
        
        if (spxSprite != NULL) {
            spxSprite->setColor(wyc3b(255, 0, 0));
            wyIntervalAction *delay = wyDelayTime::make(0.2f);
            wyActionCallback callback = {
                NULL,
                onHurtDelayEnded,
                NULL
            };
            delay->setCallback(&callback, spxSprite);
            delay->setTag(101);
            spxSprite->runAction(delay);
        }
        hurt(0);
    }
}

void Enemy::dealHitByDaBiao(float y, b2Body *bulletBody) {
    if (canBeMissed()) {
        return;
    }
    
    if (e_id == ENEMY_BOSS1 || e_id == ENEMY_BOSS2 || e_id == ENEMY_BOSS3) {
        hp -= hp/10;
    } else {
        deadType = DEAD_TYPE_MISSLE;
        dead();
    }
}

void Enemy::hurt(float y) {
//    bloodSprite->setVisible(true);
//    int seed = rand()%2;
//    int rotation = -randRange(0,30);
//    if (rand()%4 == 0) {
//        rotation*=-1;
//    }
//    bloodSprite->setRotation(rotation);
//    bloodSprite->playAnimation(SPX_PENXUE_1+seed);//SPX_PENXUE_1+seed
//    bloodSprite->setPosition(container->getPositionX(), container->getPositionY()+DP(24)*sGlobal->scaleY);
    
//    spxSprite->setColor(wyc3b(255, 0, 0));
//    wyIntervalAction *delay = wyDelayTime::make(0.3f);
//    wyActionCallback callback = {
//        NULL,
//        onHurtDelayEnded,
//        NULL
//    };
//    delay->setCallback(&callback, spxSprite);
//    spxSprite->runAction(delay);
    
    splitBlood(y);
    playAnim(animHurt);
    
    if (xuecao == NULL) {
        xuecao = (wyProgressTimer*) sGlobal->popFromCache(sGlobal->xuecaoCache);
        xuecao->setStyle(HORIZONTAL_BAR_LR);
        xuecao->setAnchor(0.5f, 0);
        xuecao->setScaleX(xuecaoSx);
        xuecao->setScaleY(sGlobal->scaleY*0.4f);
        xuecao->setPercentage(100);
        container->addChildLocked(xuecao, 200);
        xuecao->setVisible(true);
        xuecao->setPosition(0, xuecaoOfy);
    } else {
        xuecao->setVisible(true);
    }
    xuecao->setPercentage((float)hp/maxHp*100);
}

void Enemy::recordQuest() {
    if (!sGlobal->isRealEndless) {
        return;
    }
    
    if (e_id == ENEMY_BOSS1 || e_id == ENEMY_BOSS2 || e_id == ENEMY_BOSS3) {
        return;
    }
    
    sGlobal->killedEnemies += 1;
    
    for (int i = 0; i < 5; i++) {
        int achvNo = ACHV_KILL_1+i;
        if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
//            //如果是easy难度，不统计20000
//            if (achvNo == ACHV_KILL_20000 && sGlobal->currentEModeDifficulty == DIFFICULTY_NORMAL) {
//                continue;
//            }
//            
//            if (achvNo == ACHV_KILL_50000 && sGlobal->currentEModeDifficulty != DIFFICULTY_HARD) {
//                continue;
//            }
            sGlobal->achv_progress[achvNo]+=1;
            if (sGlobal->achv_progress[achvNo] >= sGlobal->ACHV_NUM[achvNo*3]) {
                sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
            }
        }
    }
    
    for (int i = 0; i < 5; i++) {
        int achvNo = ACHV_KILL_50_ONE+i;
        if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
//            if (achvNo == ACHV_KILL_400_ONE && sGlobal->currentEModeDifficulty == DIFFICULTY_NORMAL) {
//                continue;
//            }
//            if (achvNo == ACHV_KILL_800_ONE && sGlobal->currentEModeDifficulty != DIFFICULTY_HARD) {
//                continue;
//            }
            sGlobal->achv_progress_temp[achvNo]+=1;
            if (sGlobal->achv_progress_temp[achvNo] >= sGlobal->ACHV_NUM[achvNo*3]) {
                sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
            }
        }
    }
    
    if (deadType == DEAD_TYPE_MISSLE) {
        int achvNo = ACHV_MISSLE_100;
        if (sGlobal->achv_rec[achvNo] == ACHV_ACTIVE) {
            sGlobal->achv_progress[achvNo]+=1;
            if (sGlobal->achv_progress[achvNo] >= sGlobal->ACHV_NUM[achvNo*3]) {
                sGlobal->achv_rec[achvNo] = ACHV_FINISHED;
                sGlobal->showAchievmentEffect(achvNo, sGlobal->hudLayer);
                sGlobal->achv_progress[achvNo] = sGlobal->ACHV_NUM[achvNo*3];
            }
        }
    }
}

void Enemy::dead() {
//    LOGE("enemy dead start...");
    if (isDead)
        return;

    if (xuecao != NULL) {
        xuecao->setVisible(false);
    }
    
//    LOGE("enemy dead...");
    if (sGlobal->nearestEnemy == this) {
        sGlobal->nearestEnemy = NULL;
        sGlobal->nearestDistance = 1000;
    }
    
    if (smokeSprite != NULL) {
        smokeSprite->setVisible(false);
        smokeSprite->setAlpha(0);
    }
    
//    if (!bloodSprite->isVisible()) {
        bloodSprite->setVisible(true);
        int seed = rand()%2;
    int rotation = randRange(0,90);
    if (rand()%2 == 0) {
        rotation*=-1;
    }
    bloodSprite->setRotation(rotation);
    bloodSprite->setUnitInterval(0.06f);
    bloodSprite->setScale(2.4f*sGlobal->scaleX);
        bloodSprite->playAnimation(SPX_DEAD_BLOOD);//SPX_PENXUE_1+seed
		bloodSprite->setPosition(container->getPositionX(), container->getPositionY()+DP(30)*sGlobal->scaleY);
//        bloodSprite->runAction(wyMoveBy::make(0.4f, DP(100)*sGlobal->scaleX, DP(0)*sGlobal->scaleY));
//    }
    
    sGlobal->totalKilledEnemies += 1;
    
    recordQuest();

    sGlobal->level_kills += 1;
    
    int extraGold = ceil(this->gold * sGlobal->skillMulti[SKILL_GREED]);
    int finalPlusGold = this->gold * (1+sGlobal->skillMulti[SKILL_GREED]);
    if (sGlobal->isDoubleCoin) {
        finalPlusGold *= 2;
    }
    sGlobal->hudLayer->addGold(finalPlusGold);
    sGlobal->level_enemy_gold += finalPlusGold;
    sGlobal->level_total_gold += finalPlusGold;
    
    if (deadSFX != -1) {
        sGlobal->playEffect(deadSFX);
    }

    showDeadEffect();
    showBloodEffect();
    isDead = true;
    
    if (!isKnockingBack) {
        body->SetLinearVelocity(b2Vec2(0,0));
    }
    body->SetGravityScale(0);

//    if (e_id == ENEMY_BOSS1 || e_id == ENEMY_BOSS2 || e_id == ENEMY_BOSS3) {
//        sGlobal->level_bosses += 1;
//        
//        int zuanshi;
//        if (sGlobal->isRealEndless) {
//            zuanshi = rand()%sGlobal->combatRound + 1;//第几轮就最多获得多少宝石
//        } else {
//            zuanshi = 1;
//        }
//        sGlobal->level_zuanshi += zuanshi;
////        showDeadGemEffect(zuanshi);
//        
//        sGlobal->hudLayer->addZuanshi(zuanshi);
////        sGlobal->effectLayer->showDeadZuanShiEffect(container->getPositionX(), container->getPositionY(), zuanshi);
//    } else {
        sGlobal->level_enemy_kill += 1;
//    }
    
    onDead();
    sGlobal->mainGameLayer->addToDisable(body);
}

void Enemy::playAnim(int idx, bool isAppear) {
    if (isDead)
        return;

    wySPXSprite *enemySprite = ((wySPXSprite *) spxSprite);
    if (enemySprite->getCurrentAnimationIndex() == animAtk && !isStunning) {
        return;
    }
//    if (enemySprite->getCurrentAnimationIndex() == animAtk && isStunning) {
////        LOGE("halt atk when stunning...");;
//    }

    
    if (idx == animJump) {
//        LOGE("anim jump...");
        if (idx < 0) {
            return;
        }
        
        prevAnimIndex = enemySprite->getCurrentAnimationIndex();
        prevLoopCount = enemySprite->getLoopCount();
        if (prevAnimIndex == animJump) {
            return;
        }
        
//        wyAFCSpriteCallback callback = {
//            NULL,
//            onStartJumpEnded
//        };
        enemySprite->setLoopCount(0);
//        enemySprite->setAFCSpriteCallback(&callback, this);
        enemySprite->playAnimation(animJump);
    }
    else if (idx == animLanding) {
//        LOGE("anim landing...");
        if (idx < 0) {
            onLandingEnded(enemySprite, this);
//            landing_emitter = wyParticleLoader::load("particle_landing"));
//            landing_emitter->setScaleX(sGlobal->scaleX*1.0f);
//            landing_emitter->setScaleY(sGlobal->scaleY*1.0f);
//            sGlobal->mainGameLayer->addChildLocked(landing_emitter, 0);
//            landing_emitter->setPositionType(FREE);
//            landing_emitter->setAutoRemoveOnFinish(true);
//            landing_emitter->setPosition(sprite->getPositionX(), sprite->getPositionY()+DP(10)*sGlobal->scaleY);
            return;
        }
//        if (enemySprite->getCurrentAnimationIndex() != animJump) {
//            return;
//        }
        
        prevAnimIndex = enemySprite->getCurrentAnimationIndex();
        prevLoopCount = enemySprite->getLoopCount();
        
        wyAFCSpriteCallback callback = {
            NULL,
            onLandingEnded
        };
        enemySprite->setLoopCount(0);
        enemySprite->setAFCSpriteCallback(&callback, this);
        enemySprite->playAnimation(animLanding);
//        landing_emitter = wyParticleLoader::load("particle_landing"));
//        landing_emitter->setScaleX(sGlobal->scaleX*1.0f);
//        landing_emitter->setScaleY(sGlobal->scaleY*1.0f);
//        sGlobal->mainGameLayer->addChildLocked(landing_emitter, 0);
//        landing_emitter->setPositionType(FREE);
//        landing_emitter->setAutoRemoveOnFinish(true);
//        landing_emitter->setPosition(sprite->getPositionX(), sprite->getPositionY()+DP(10)*sGlobal->scaleY);
    }
    else if (idx == animWalk) {
//        LOGE("anim walk...");
        if (idx < 0) {
//            run_emitter = wyParticleLoader::load("particle_run"));
//            run_emitter->setScaleX(sGlobal->scaleX*1.0f);
//            run_emitter->setScaleY(sGlobal->scaleY*1.0f);
//            sGlobal->mainGameLayer->addChildLocked(run_emitter, 0);
//            run_emitter->setPositionType(FREE);
//            run_emitter->setPosition(sprite->getPositionX(), sprite->getPositionY()+DP(10)*sGlobal->scaleY);
//            yan->setVisible(true);
            return;
        }
        
        prevAnimIndex = enemySprite->getCurrentAnimationIndex();
        prevLoopCount = enemySprite->getLoopCount();
        if (prevAnimIndex == animWalk) {
//            enemySprite->setFlipX(false);
            return;
        }
        
        float interval = MAX(0.1f, 5.0f/MAX(0.001f, fabs(body->GetLinearVelocity().x)) * 0.1f);
        enemySprite->setUnitInterval(interval);
        enemySprite->setLoopCount(-1);
        enemySprite->playAnimation(animWalk);
        enemySprite->setFlipX(false);
//        run_emitter = wyParticleLoader::load("particle_run"));
//        run_emitter->setScaleX(sGlobal->scaleX*1.0f);
//        run_emitter->setScaleY(sGlobal->scaleY*1.0f);
//        sGlobal->mainGameLayer->addChildLocked(run_emitter, 0);
//        run_emitter->setPositionType(FREE);
//        run_emitter->setPosition(sprite->getPositionX(), sprite->getPositionY()+DP(10)*sGlobal->scaleY);
    }
    else if (idx == animWalkBack) {
//        LOGE("anim walkback...");
        if (idx < 0) {
            return;
        }
        
        prevAnimIndex = enemySprite->getCurrentAnimationIndex();
        prevLoopCount = enemySprite->getLoopCount();
        if (prevAnimIndex == animWalkBack) {
//            enemySprite->setFlipX(true);
            return;
        }
        
        float interval = MAX(0.1f, 5.0f/MAX(0.001f, fabs(body->GetLinearVelocity().x)) * 0.1f);//5.0f/fabs(body->GetLinearVelocity().x) * 0.1f;
        enemySprite->setUnitInterval(interval);
        enemySprite->setLoopCount(-1);
        enemySprite->playAnimation(animWalk);
        enemySprite->setFlipX(true);
        
        if (!isCmdFinished) {
            enemySprite->setFlipX(false);
        }
    }
    else if (idx == animIdle) {
        if (idx < 0) {
            return;
        }
        
        prevAnimIndex = enemySprite->getCurrentAnimationIndex();
        prevLoopCount = enemySprite->getLoopCount();
        
        enemySprite->setUnitInterval(unitInterval);
        enemySprite->setLoopCount(-1);
        enemySprite->playAnimation(animIdle);
    }
    else if (idx == animDead) {
////        LOGE("anim dead...");
//        isDead = true;
//        
//        if (idx < 0) {
//            enemySprite->setAFCSpriteCallback(NULL, NULL);
//            onEnemyDeadAnimationEnded(enemySprite, this);
//            return;
//        }
//        
//        prevAnimIndex = enemySprite->getCurrentAnimationIndex();
//        prevLoopCount = enemySprite->getLoopCount();
//        
//        wyAFCSpriteCallback callback = {
//            onEnemyDeadAnimationChanged,
//            NULL,
//        };
//        enemySprite->setLoopCount(0);
//        enemySprite->setUnitInterval(0.1f);
//        enemySprite->setAFCSpriteCallback(&callback, this);
//        enemySprite->playAnimation(animDead);
    }
    else if (idx == animHurt) {
//        LOGE("anim hurt...");
        if (idx < 0) {
//            enemySprite->setAFCSpriteCallback(NULL, NULL);
            return;
        }
        if (enemySprite->getCurrentAnimationIndex() == animHurt) {
            return;
        }
        
        isHurting = true;
        prevAnimIndex = enemySprite->getCurrentAnimationIndex();
//        if (prevAnimIndex == animAtk && !isStunning) {
//            return;
//        }
//        if (prevAnimIndex == animAtk && !isStunning) {
//            LOGE("halt atk when stunning...");;
//        }
        
        prevLoopCount = enemySprite->getLoopCount();
        
        wyAFCSpriteCallback callback = {
            NULL,
            onHurtAnimEnded
        };
        enemySprite->setUnitInterval(0.1f);
        enemySprite->setLoopCount(0);
        enemySprite->setAFCSpriteCallback(&callback, this);
        enemySprite->playAnimation(animHurt);
    }
    
//    if (idx > 0 && idx != animDead) {
//        sGlobal->mainGameLayer->addToRecreateFixture(this);
//    }
}

void Enemy::onDead() {
    
}

void Enemy::knockBack() {
    if (!canKnockBack) {
        return;
    }
    
    isKnockingBack = true;
    knockingBackTimer = 0;
    //yan->setVisible(false);
    if (preVelX == -1) {
        preVelX = body->GetLinearVelocity().x;
    }
    body->SetLinearVelocity(b2Vec2(40.0f, body->GetLinearVelocity().y));
}

void Enemy::addBulletMask(uint16 newMaskBit) {
//    LOGE("addBulletMask:%d", newMaskBit);
    b2Fixture *fixture = body->GetFixtureList();
    b2Fixture *next;
    while (fixture != NULL) {
        next = fixture->GetNext();
        b2Filter filter = fixture->GetFilterData();
        filter.maskBits = filter.maskBits ^ newMaskBit;
        fixture->SetFilterData(filter);
        fixture = next;
    }
    
    RemoveMaskBitTask *task = new RemoveMaskBitTask(newMaskBit, 0.5f);
    task->retain();
    wyArrayPush(removeMaskBitArray, task);
}

void Enemy::clearBulletMask(uint16 newMaskBit) {
//    LOGE("clearBulletMask:%d", newMaskBit);
    b2Fixture *fixture = body->GetFixtureList();
    b2Fixture *next;
    while (fixture != NULL) {
        next = fixture->GetNext();
        b2Filter filter = fixture->GetFilterData();
        filter.maskBits = filter.maskBits + newMaskBit;
        fixture->SetFilterData(filter);
        fixture = next;
    }
}

void Enemy::resetMask() {
    b2Fixture *fixture = body->GetFixtureList();
    b2Fixture *next;
    while (fixture != NULL) {
        next = fixture->GetNext();
        b2Filter filter = fixture->GetFilterData();
        filter.maskBits = MASK_BITS[TYPE_ENEMY];
        fixture->SetFilterData(filter);
        fixture = next;
    }
}

static void removeOnAnimEnded(wyAction* action, void* data) {
    wyNode* node = (wyNode *) data;
    node->getParent()->removeChildLocked(node, true);
}

void Enemy::showCriticalEffect(int hitNum) {
    char temp[10];
    sprintf(temp, "%d!", hitNum);
//    LOGE("showcritical...");
    wyBitmapFontLabel *hitTitle = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
    hitTitle->setScale(SP(30)*sGlobal->scaleX/sGlobal->descFontSize);
    hitTitle->setAlignment(wyBitmapFontLabel::LEFT);
    hitTitle->setColor(wyc3b(255, 0, 0));
//    hitTitle->setPosition(DP(40)*sGlobal->scaleX, 0);
    container->addChildLocked(hitTitle, 103);
    
    hitTitle->setPosition(-DP(10)*sGlobal->scaleX, DP(50)*sGlobal->scaleY);
    
    wyIntervalAction *action = wySequence::make(wyDelayTime::make(0.2f), wyEaseIn::make(0.6f, wyMoveBy::make(0.5f, 0, DP(50)*sGlobal->scaleY)), wyDelayTime::make(1.0f), NULL);
    wyActionCallback callback = {
        NULL,
        removeOnAnimEnded
    };
    action->setCallback(&callback, hitTitle);
    hitTitle->runAction(action);
    hitTitle->runAction(wySequence::make(wyDelayTime::make(0.7f), wyFadeOut::make(0.5f), NULL));
}

void Enemy::showDeadCoinEffect(int gold) {
    wyNode *deadCoinContainer = wyNode::make();
    deadCoinContainer->setPosition(DP(-40)*sGlobal->scaleX, DP(50)*sGlobal->scaleY);
    container->addChildLocked(deadCoinContainer, 103);
    
    wySpriteEx *deadCoin = wyZwoptexManager::getInstance()->makeSpriteEx("dialog_deco", "coin_icon.png");
    deadCoin->setScaleX(0.7f*sGlobal->scaleX);
    deadCoin->setScaleY(0.7f*sGlobal->scaleY);
    deadCoin->setPosition(DP(0)*sGlobal->scaleX, 0);
    deadCoinContainer->addChildLocked(deadCoin);
    
    char temp[5];
    sprintf(temp, "+%d", gold);

    wyBitmapFontLabel *deadCoinTitle = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
    deadCoinTitle->setScale(SP(24)*sGlobal->scaleX/sGlobal->descFontSize);
    deadCoinTitle->setAlignment(wyBitmapFontLabel::LEFT);
//    deadCoinTitle->setColor(goldColor);
    deadCoinTitle->setAnchor(0, 0.5f);
    deadCoinTitle->setPosition(DP(20)*sGlobal->scaleX, DP(0)*sGlobal->scaleY);
    deadCoinContainer->addChildLocked(deadCoinTitle);
    
    wyIntervalAction *action = wySequence::make(wyDelayTime::make(0.2f), wyMoveBy::make(1.0f, 0, DP(40)*sGlobal->scaleY), NULL);
    wyActionCallback callback = {
        NULL,
        removeOnAnimEnded
    };
    action->setCallback(&callback, deadCoinContainer);
    deadCoinContainer->runAction(action);
    deadCoinTitle->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(1.0f), NULL));
    deadCoin->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(1.0f), NULL));
}

void Enemy::showDeadGemEffect(int zuanshi) {
    wyNode *deadZuanshiContainer = wyNode::make();
    deadZuanshiContainer->setPosition(-DP(40)*sGlobal->scaleX, DP(120)*sGlobal->scaleY);
    container->addChildLocked(deadZuanshiContainer, 103);
    
    wySpriteEx *deadZuanshi = wySpriteEx::make(MyTextureManager::makePNG("cash_icon.png"));
    deadZuanshi->setScaleX(0.9f*sGlobal->scaleX);
    deadZuanshi->setScaleY(0.9f*sGlobal->scaleY);
    deadZuanshi->setPosition(DP(0)*sGlobal->scaleX, 0);
    deadZuanshiContainer->addChildLocked(deadZuanshi);
    
    char temp[5];
    sprintf(temp, "+%d", zuanshi);

    wyBitmapFontLabel *deadZuanshiTitle = wyBitmapFontLabel::make(sGlobal->font_desc, temp);
    deadZuanshiTitle->setScale(SP(26)*sGlobal->scaleX/sGlobal->descFontSize);
    deadZuanshiTitle->setAlignment(wyBitmapFontLabel::LEFT);
    deadZuanshiTitle->setColor(zuanshiColor);
    deadZuanshiTitle->setPosition(DP(20)*sGlobal->scaleX, DP(0)*sGlobal->scaleY);
    deadZuanshiTitle->setAnchor(0, 0.5f);
    deadZuanshiContainer->addChildLocked(deadZuanshiTitle);
    
    wyIntervalAction *action = wySequence::make(wyDelayTime::make(0.2f), wyMoveBy::make(2.0f, 0, DP(60)*sGlobal->scaleY), NULL);
    wyActionCallback callback = {
        NULL,
        removeOnAnimEnded
    };
    action->setCallback(&callback, deadZuanshiContainer);
    deadZuanshiContainer->runAction(action);
    deadZuanshiTitle->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(4.0f), NULL));
    deadZuanshi->runAction(wySequence::make(wyDelayTime::make(0.2f), wyFadeOut::make(4.0f), NULL));
}

void Enemy::showDeadEffect() {
    if (!needShowDeadEffect) {
        return;
    }
    
//    container->setVisible(false);
    spxSprite->setVisible(false);
    if (shadow != NULL) {
        shadow->setVisible(false);
    }
    if (faintSprite != NULL) {
        faintSprite->setVisible(false);
    }
    
    int count = 6;
    int no = 1;
    bool needBloodGround = true;
    switch (e_id) {
        case ENEMY_TUZIJIANKE:
        case ENEMY_TUZILANGKE:
            count = 6;
            no = 1;
            break;
        case ENEMY_TUZIGONGJIANSHOU:
            count = 6;
            no = 2;
            break;
        case ENEMY_TUZIFASHI:
            count = 5;
            no = 7;
            break;
        case ENEMY_TUZIJIANSHENG:
        case ENEMY_KULOUWUSHI:
            count = 6;
            no = 3;
            break;
        case ENEMY_KULOUTOUJIZHE:
        case ENEMY_ZHAOHUANSHI:
            count = 7;
            no = 8;
            break;
        case ENEMY_KULOUTOULING:
        case ENEMY_HONGYIWUSHI:
            count = 6;
            no = 10;
            break;
        case ENEMY_KULOUSHIBING:
            count = 6;
            no = 5;
            break;
        case ENEMY_GOUTOUJUNSHI:
        case ENEMY_JIAMIANJUNSHI:
            count = 6;
            no = 9;
            break;
        case ENEMY_HEIYIWUSHI:
            count = 5;
            no = 6;
            break;
        case ENEMY_TUZIGONGJIANSHOUFLY:
        case ENEMY_KULOUSHIBINGFLY:
        case ENEMY_HONGYIWUSHIFLY:
            count = 8;
            no = 4;
            break;
        case ENEMY_ZHANGAI1:
            count = 6;
            no = 13;
            needBlood = false;
            break;
        case ENEMY_ZHANGAI2:
            count = 5;
            no = 12;
            needBloodGround = false;
            break;
        case ENEMY_ZHANGAI3:
            count = 4;
            no = 11;
            needBloodGround = false;
            break;
        default:
            break;
    }
//    Flesh::makeFleshes(container->getPositionX(), container->getPositionY(), count, this, no);
//    for (int i = 1; i <= count; i++) {
//        generateFlesh(container->getPositionX(), container->getPositionY(), i, this, no);
//    }
    
    int totalCount = count;
    if (sGlobal->mainGameLayer->currentBodies > 20 && sGlobal->mainGameLayer->currentBodies <= 30) {
        if (count >= 5) {
            count = 5;
        }
    }
    else if (sGlobal->mainGameLayer->currentBodies > 30 && sGlobal->mainGameLayer->currentBodies <= 50) {
        count = 4;
    }
    else if (sGlobal->mainGameLayer->currentBodies > 50) {
        count = 3;
    }
    
    for (int i = 1; i <= count; i++) {
        if (count == 5 && totalCount > 5 && i == count - 1) {
            WYNEW Flesh(container->getPositionX(), container->getPositionY(), rand()%(totalCount-count+1)+i, this, no, needBloodGround);
            break;
        } else {
            WYNEW Flesh(container->getPositionX(), container->getPositionY(), i, this, no, needBloodGround);
        }
    }
    
}

void Enemy::showBloodEffect() {
    int count = 3;
    if (sGlobal->mainGameLayer->currentBodies > 20 && sGlobal->mainGameLayer->currentBodies <= 30) {
        count = 2;
    }
    else if (sGlobal->mainGameLayer->currentBodies > 30 && sGlobal->mainGameLayer->currentBodies <= 50) {
        count = 1;
    }
    else if (sGlobal->mainGameLayer->currentBodies > 50) {
        count = 0;
    }
    for (int i = 0; i < count; i++) {
        Blood::make(container->getPositionX(), container->getPositionY(), i, this);
    }
}

void Enemy::applyStun() {
    if (faintSprite == NULL) {
        faintSprite = (wySPXSprite *)sGlobal->popFromCache(sGlobal->effects_cache);
        faintSprite->setForceTickMode(true);
        faintSprite->setUnitInterval(0.1f);
        faintSprite->setLoopCount(-1);
        faintSprite->playAnimation(SPX_FAINT);
        faintSprite->setScaleX(faintSpriteScale);
        faintSprite->setScaleY(faintSpriteScale);
        faintSprite->setPosition(faintSpriteX, faintSpriteY);
        container->addChildLocked(faintSprite, 102);
        
        faintSprite->setBlendFunc((wyBlendFunc){ GL_SRC_ALPHA, GL_ONE });
    } else {
        faintSprite->setVisible(true);
    }
    
//    if (isOnGround) {
//        playAnim(animIdle);
//    } else {
//        playAnim(animJump);
//    }
    
    isStunning = true;
    stunTimer = 0;
    
    faintSprite->stopAllActions();
    if (e_id != ENEMY_BOSS1 && e_id != ENEMY_BOSS2 && e_id != ENEMY_BOSS3) {
        body->SetLinearVelocity(b2Vec2(0, 0));
        faintSprite->runAction(wySequence::make(wyDelayTime::make(sGlobal->SKILL_STUN_DUR), wyFadeOut::make(0.05f), wyHide::make(), NULL));
    } else {
        faintSprite->runAction(wySequence::make(wyDelayTime::make(sGlobal->SKILL_STUN_DUR*sGlobal->BOSS_STUN_TIME_MULTI), wyFadeOut::make(0.1f), wyHide::make(), NULL));
    }
}





Flesh::Flesh(float x, float y, int i, Enemy *parent, int idx, bool needBlood) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    this->needBloodGround = needBlood;
    sGlobal->mainGameLayer->currentBodies += 1;

    int e_id = -1;
    hp = 0;
    atk = 0;
    gold = 0;
    
    float ofx = 0, ofy = 10;
    if (idx == 1) {
        switch (i) {
            case 0:
                ofx = DP(4);
                ofy = DP(24);
                break;
            case 1:
                ofx = DP(-47);
                ofy = DP(50);
                break;
            case 2:
                ofx = DP(-30);
                ofy = DP(0);
                break;
            case 3:
                ofx = DP(-35);
                ofy = DP(-15);
                break;
            case 4:
                ofx = DP(0);
                ofy = DP(-20);
                break;
            case 5:
                ofx = DP(-36);
                ofy = DP(-70);
                break;
            case 6:
                ofx = DP(0);
                ofy = DP(-70);
                break;
            default:
                break;
        }
    }
    ofx *= parent->spxSprite->getScaleX();
    ofy *= parent->spxSprite->getScaleY();
    
    char temp[20];
    sprintf(temp, "%d_%d.png", idx, i);

    fleshSprite = (wySpriteEx*) sGlobal->popFromCache(sGlobal->fleshSpriteCache);
    wySpriteFrame *frame = wyZwoptexManager::getInstance()->getSpriteFrame("zombiebodies_pic", temp);
    wyRect r = frame->getRect();
    fleshSprite->setTextureRect(r);
    fleshSprite->setScaleX(parent->spxSprite->getScaleX());
    fleshSprite->setScaleY(parent->spxSprite->getScaleY());
    fleshSprite->setContentSize(r.width*fleshSprite->getScaleX(), r.height*fleshSprite->getScaleY());
    sGlobal->mainGameLayer->fleshBatchNode->addChildLocked(fleshSprite);
    fleshSprite->setVisible(true);
//    fleshSprite->setFlipX(parent->spxSprite->isFlipX());

    container = fleshSprite;

    obj_type = TYPE_BODY_PIECE;
    
    char temp1[20];
    sprintf(temp1, "%d_%d", idx, i);

    wyBox2DPELoader *m_bodyLoader;
    if (idx < 6) {
        m_bodyLoader = sGlobal->mainGameLayer->m_bodyLoader;
    } else {
        m_bodyLoader = sGlobal->mainGameLayer->m_bodyLoader1;
    }
    wyPoint anchorPercent;

    body = m_bodyLoader->createBodyByName(m_box2d, temp1);
    body->SetUserData(this);
    body->SetTransform(b2Vec2(m_box2d->pixel2Meter(x+ofx), m_box2d->pixel2Meter(y+ofy)), 0);

    b2Fixture *fixture = body->GetFixtureList();
    b2Fixture *next;
    while (fixture != NULL) {
        fixture->SetFriction(1.0f);
        fixture->SetDensity(1.0f);
        fixture->SetRestitution(0.0f);

        b2Filter filter = fixture->GetFilterData();
        filter.maskBits = MASK_BITS[TYPE_BODY_PIECE];
        filter.categoryBits = CAT_BITS[TYPE_BODY_PIECE];
        filter.groupIndex = GROUND_INDEX[TYPE_BODY_PIECE];
        fixture->SetFilterData(filter);
        
        next = fixture->GetNext();
        fixture = next;
    }
    
    anchorPercent = m_bodyLoader->getAnchorPercent(temp1);
    fleshSprite->setAnchor(anchorPercent.x, anchorPercent.y);
    fleshSprite->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
    fleshSprite->setRotation(-wyMath::r2d(body->GetAngle()));

    float velx, vely;
    getRandForce(parent->deadType, velx, vely);
//    LOGE("velx:%f, vely:%f", velx, vely);
    
    float mass = body->GetMass();
    velx = velx*mass;
    vely = vely*mass;
    b2Vec2 f = body->GetWorldVector(b2Vec2(velx, vely));
    b2Vec2 p = body->GetWorldCenter();
    float realx = p.x+randRange(0, 3)/10.0f*(rand()%2==0?-1:1);
    float realy = p.y+randRange(0, 3)/10.0f*(rand()%2==0?-1:1);
    body->ApplyLinearImpulse(f, b2Vec2(realx, realy));
    
    bloodSprite1 = NULL;
    
    offsetX = 0;
    offsetY = 0;
    
    isOnGround = false;
    isDestroied = false;
}

void Flesh::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    container->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
    container->setRotation(-wyMath::r2d(body->GetAngle()));
}

void Flesh::handleCollision(b2Body *actorB) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
    int tagB = userDataB->obj_type;
    switch (tagB) {
        case TYPE_GROUND: {
            if (!isOnGround) {
                isOnGround = true;
                if (!needBloodGround) {
                    return;
                } else {
                    bloodSprite1 = (wySpriteEx*) sGlobal->popFromCache(sGlobal->shadowSpriteCache);
                    bloodSprite1->setTextureRect(wyZwoptexManager::getInstance()->getFrameRect("dialog_deco", "blood_ground.png"));
                    bloodSprite1->setScaleX(sGlobal->scaleX*1.0f);
                    bloodSprite1->setScaleY(sGlobal->scaleY*1.0f);
                    bloodSprite1->setPosition(0, -DP(0)*sGlobal->scaleY);
                    bloodSprite1->setVisible(false);
                    bloodSprite1->setAlpha(255);
                    sGlobal->mainGameLayer->bloodBatchNode->addChildLocked(bloodSprite1);
                    halfWidth = DP(33)*bloodSprite1->getScaleX();
                }
                
                if (bloodSprite1->isVisible()) {
                    return;
                }
                
                bloodSprite1->setVisible(true);
                
                float s = randRange(0, 4)*sGlobal->scaleY;
                if (rand()%2 == 0) {
                    s = -s;
                }
                bloodSprite1->setPosition(container->getPositionX(), m_box2d->meter2Pixel(userDataB->body->GetPosition().y)+s);
                
                if (userDataB->sub_type == 987) {
                    return;
                }
                float diff1 = bloodSprite1->getPositionX() + halfWidth - userDataB->endX;
                if (diff1 > 0) {
                    bloodSprite1->setVisible(false);
                }
                
                float diff2 = userDataB->startX - (bloodSprite1->getPositionX() - halfWidth);
                if (diff2 > 0) {
                    bloodSprite1->setVisible(false);
                }
            }
            break;
        }
        default:
            break;
    }
}

void Flesh::destroyMe() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();

    sGlobal->mainGameLayer->fleshBatchNode->removeChildLocked(fleshSprite, false);
    wyArrayPush(sGlobal->fleshSpriteCache, fleshSprite);
    
    if (bloodSprite1 != NULL) {
        sGlobal->mainGameLayer->bloodBatchNode->removeChildLocked(bloodSprite1, false);
        wyArrayPush(sGlobal->shadowSpriteCache, bloodSprite1);
    }

    world->DestroyBody(body);
    delete this;
    
    sGlobal->mainGameLayer->currentBodies -= 1;
}


Blood::Blood(float x, float y, int i, Enemy *parent) {
    idx = i;
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
    sGlobal->mainGameLayer->currentBodies += 1;
    
    y += DP(40)*sGlobal->scaleY;
    
    int e_id = -1;
    hp = 0;
    atk = 0;
    gold = 0;

    bloodSprite = (wySpriteEx*) sGlobal->popFromCache(sGlobal->shadowSpriteCache);
//    switch (i) {
//        case 0:
//            bloodSprite->setTextureRect(wyZwoptexManager::getInstance()->getFrameRect("dialog_deco", "single_blood.png"));
//            bloodSprite->setScaleX(sGlobal->scaleX*0.8f);
//            bloodSprite->setScaleY(sGlobal->scaleY*0.4f);
//            break;
//        case 1:
//            bloodSprite->setTextureRect(wyZwoptexManager::getInstance()->getFrameRect("dialog_deco", "blood_bone.png"));
//            bloodSprite->setScaleX(sGlobal->scaleX*0.8f);
//            bloodSprite->setScaleY(sGlobal->scaleY*0.8f);
//            break;
//        case 2:
//            bloodSprite->setTextureRect(wyZwoptexManager::getInstance()->getFrameRect("dialog_deco", "blood_meat.png"));
//            bloodSprite->setScaleX(sGlobal->scaleX*0.8f);
//            bloodSprite->setScaleY(sGlobal->scaleY*0.8f);
//            break;
//        default:
//            break;
//    }
    
    bloodSprite->setTextureRect(wyZwoptexManager::getInstance()->getFrameRect("dialog_deco", "single_blood.png"));
    bloodSprite->setScaleX(sGlobal->scaleX*0.8f);
    bloodSprite->setScaleY(sGlobal->scaleY*0.4f);
    sGlobal->mainGameLayer->bloodBatchNode->addChildLocked(bloodSprite);
    
    bloodSprite1 = (wySpriteEx*) sGlobal->popFromCache(sGlobal->shadowSpriteCache);
    bloodSprite1->setTextureRect(wyZwoptexManager::getInstance()->getFrameRect("dialog_deco", "blood_ground.png"));
    bloodSprite1->setScaleX(sGlobal->scaleX*1.0f);
    bloodSprite1->setScaleY(sGlobal->scaleY*1.0f);
    sGlobal->mainGameLayer->bloodBatchNode->addChildLocked(bloodSprite1);
    
    halfWidth = DP(33)*bloodSprite1->getScaleX();
    tHeight = DP(10)*bloodSprite1->getScaleY();
    
    obj_type = TYPE_BLOOD;
    
    float boxWidth = DP(2)*sGlobal->scaleX;
    float boxHeight = DP(10)*sGlobal->scaleY;
    
    this->halfBodyHeightInMeter = m_box2d->pixel2Meter(boxHeight);
    
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y));
    bd.userData = this;
    bd.type = b2_dynamicBody;
    
    b2PolygonShape staticBox;
    staticBox.SetAsBox(m_box2d->pixel2Meter(boxWidth), m_box2d->pixel2Meter(boxHeight));
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;
    if (i == 1) {
        fixtureDef.density = 0.0f;
    } else {
        fixtureDef.density = 0.0f;
    }
    fixtureDef.friction = 0.8f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = MASK_BITS[TYPE_BLOOD];
    fixtureDef.filter.categoryBits = CAT_BITS[TYPE_BLOOD];
    fixtureDef.filter.groupIndex = GROUND_INDEX[TYPE_BLOOD];
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.8f);
    
    float velx, vely;
    getRandForce(parent->deadType, velx, vely);
//    LOGE("velx:%f, vely:%f", velx, vely);

    bloodSprite->setFlipX(velx < 0);

    b2Vec2 f = body->GetWorldVector(b2Vec2(velx, vely));
    b2Vec2 p = body->GetWorldCenter();//
    body->ApplyLinearImpulse(f, p);
    
    offsetX = 0;
    offsetY = 0;
    isOnGround = false;
}

void Blood::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    if (!isOnGround) {
        bloodSprite->setPosition(m_box2d->meter2Pixel(body->GetPosition().x) + offsetX, m_box2d->meter2Pixel(body->GetPosition().y) + offsetY);
        
        b2Vec2 vel = body->GetLinearVelocity();
        float angle = atan(vel.y/vel.x);
        bloodSprite->setRotation(-angle*57.3f);
    }
}

void Blood::handleCollision(b2Body *actorB) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
//    b2World* world = m_box2d->getWorld();
    
    BaseObject *userDataB = (BaseObject*)actorB->GetUserData();
    int tagB = userDataB->obj_type;
    switch (tagB) {
        case TYPE_GROUND: {
            if (!isOnGround) {
                isOnGround = true;
                bloodSprite1->setVisible(true);
                
                float s = randRange(0, 4);
                if (rand()%2 == 0) {
                    s = -s;
                }
                bloodSprite1->setPosition(bloodSprite->getPositionX(), m_box2d->meter2Pixel(userDataB->body->GetPosition().y)+s);
                bloodSprite->setVisible(false);
                
                body->SetLinearVelocity(b2Vec2(0,0));
                body->SetGravityScale(0);
                sGlobal->mainGameLayer->addToDisable(body);
                
                float diff1 = bloodSprite1->getPositionX() + halfWidth - userDataB->endX;
                if (diff1 > 0) {
                    bloodSprite1->setScaleX((halfWidth-diff1)/halfWidth - 0.1f);
                    bloodSprite1->setPosition(bloodSprite1->getPositionX()-DP(6)*sGlobal->scaleX, bloodSprite1->getPositionY());
                    bloodSprite1->setVisible(false);
                }
                
                float diff2 = userDataB->startX - (bloodSprite1->getPositionX() - halfWidth);
                if (diff2 > 0) {
                    bloodSprite1->setScaleX((halfWidth-diff2)/halfWidth - 0.1f);
                    bloodSprite1->setPosition(bloodSprite1->getPositionX()+DP(6)*sGlobal->scaleX, bloodSprite1->getPositionY());
                    bloodSprite1->setVisible(false);
                }
            }
            break;
        }
        default:
            break;
    }
}


RemoveMaskBitTask::RemoveMaskBitTask(uint16 maskBit, float duration) {
    this->m_maskBit = maskBit;
    this->m_duration = duration;
    m_timer = 0;
}

RemoveMaskBitTask::~RemoveMaskBitTask() {
    
}

bool RemoveMaskBitTask::updateTask(float dt) {
    m_timer += dt;
    if (m_timer > m_duration) {
        return true;
    }
    return false;
}


//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "Boss1.h"
#include "Global.h"
#include "EnemyFactory.h"
#include "SpittleZombie.h"
#include "MaleZombie.h"
#include "Devourer.h"
#include "FatZombie.h"
#include "Bonus.h"
#include "EndPoint.h"
#include "FlyZombie1.h"
#include "HeroBullet.h"
#include "MainScene.h"

Boss1 *Boss1::instance = NULL;

static void onAFCAnimationFrameChanged(wyAFCSprite* sprite, void* data) {
    Boss1* enemy = (Boss1*)data;
//    LOGE("onAFCAnimationFrameChanged: enemy->status:%d, enemy->isDead:%d", enemy->status, enemy->isDead);
    if (enemy->status == BOSS_FAKE_DEAD || enemy->isDead) {
        return;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    if (enemy->status == BOSS1_ATK_RENG_DAO) {
        if (sprite->getCurrentFrame() == 2) {
            enemy->shootCaiDao(rand()%3+1);
        }
    }
    else if (enemy->status == BOSS1_ATK_KAN_1 && sprite->getCurrentAnimationIndex() == BOSS1_ANIM_KAN) {
        if (sprite->getCurrentFrame() == 2) {
            sGlobal->mainGameLayer->addToRecreateFixture(enemy);
//            enemy->body->SetTransform(b2Vec2(enemy->body->GetPosition().x, enemy->body->GetPosition().y + m_box2d->pixel2Meter(DP(6)*sGlobal->scaleY)), 0);
            if (!sGlobal->isRealEndless) {
                enemy->body->SetGravityScale(1.0f);
            }
            sGlobal->playEffect(se_boss1_attack1);
        }
    }
    else if (enemy->status == BOSS1_ATK_XULI_LIANDAO_1) {
//        if (sprite->getCurrentFrame() == 0) {
//            enemy->throwGouzi();
//        }
    }
    else if (enemy->status == BOSS1_ATK_HUIXUAN_CAIDAO_0) {
        if (sprite->getCurrentFrame() == 2) {
            enemy->throwHuixuanCaidao();
        }
    }
}

static void onAFCAnimationEnded(wyAFCSprite* sprite, void* data) {
    Boss1* enemy = (Boss1*)data;
//    LOGE("onAFCAnimationEnded: enemy->status:%d, enemy->isDead:%d", enemy->status, enemy->isDead);
    if (enemy->status == BOSS_FAKE_DEAD || enemy->isDead) {
        return;
    }
    
    wySPXSprite *enemySprite = ((wySPXSprite *) enemy->spxSprite);
    if (enemy->status == BOSS1_ATK_RENG_DAO) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS1_ANIM_WALK);
        enemy->status = BOSS1_WALK;
    }
    else if (enemy->status == BOSS1_ATK_KAN_1) {
        enemySprite->setUnitInterval(0.2f);
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS1_ANIM_WALK);
        enemy->status = BOSS1_ATK_KAN_2;
        enemy->setCurVel(30.0f, 0.0f);
        sGlobal->mainGameLayer->addToRecreateFixture(enemy);
    }
    else if (enemy->status == BOSS1_ATK_XULI_LIANDAO_1) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS1_ANIM_WALK);
        enemy->status = BOSS1_WALK;
        sGlobal->mainGameLayer->addToRecreateFixture(enemy);
        
        enemy->deleteGouziObj();
    }
    else if (enemy->status == BOSS1_ATK_HUIXUAN_CAIDAO_0) {
        if (enemy->huixuan_times >= 3) {
            enemySprite->setLoopCount(-1);
            enemySprite->setAFCSpriteCallback(NULL, NULL);
            enemySprite->playAnimation(BOSS1_ANIM_WALK);
            enemy->status = BOSS1_WALK;
            enemy->huixuan_times = 0;
        } else {
            enemy->doAttackById(BOSS1_ATK_HUIXUAN_CAIDAO_0);
        }
    }
    else if (enemy->status == BOSS_HURT) {
        enemySprite->setLoopCount(-1);
        enemySprite->setAFCSpriteCallback(NULL, NULL);
        enemySprite->playAnimation(BOSS1_ANIM_WALK);
        enemy->status = BOSS1_WALK;
    }
}

Boss1::Boss1(float x, float y) : Boss(x, y){
    instance = this;
    deadType = DEAD_TYPE_BOSS;
    sub_type = 100;
//    sGlobal->isBoss1Strong=true;
    status_idle = BOSS1_WALK;
    
    e_id = ENEMY_BOSS1;

    if (sGlobal->isRealEndless) {
        int colIndex;
        float power = 1.0f;
        
        switch (sGlobal->combatRound) {
            case 1:
                hp = sGlobal->ENEMY_HP_EASY[e_id];
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                colIndex = COL_NORMAL;
                break;
            case 2:
                hp = sGlobal->ENEMY_HP_HARD[e_id];
                atk = sGlobal->ENEMY_ATK_HARD[e_id];
                gold = sGlobal->ENEMY_GOLD_HARD[e_id];
                colIndex = COL_HARD;
                break;
            case 3:
                hp = sGlobal->ENEMY_HP_CRAZY[e_id];
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id];
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id];
                colIndex = COL_CRAZY;
                break;
            default:
                colIndex = COL_CRAZY;
                power = (float)pow(sGlobal->ENDLESS_ATK_POWER, sGlobal->combatRound - 3);
                hp = sGlobal->ENEMY_HP_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_HP_POWER, sGlobal->combatRound - 3);
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_ATK_POWER, sGlobal->combatRound - 3);
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id] * (float)pow(sGlobal->ENDLESS_GOLD_POWER, sGlobal->combatRound - 3);
                break;
        }
        
        caidaoAtk = sGlobal->BOSS1_CAIDAO_ATK[colIndex]*power;
        kanAtk = sGlobal->BOSS1_KAN_ATK[colIndex]*power;
        xuliliandaoHP = sGlobal->BOSS1_XULI_LIANDAO_HP[colIndex]*power;
        xuliliandaoAtk = sGlobal->BOSS1_XULI_LIANDAO_ATK[colIndex]*power;
        huixuancaidaoHP = sGlobal->BOSS1_HUIXUAN_CAIDAO_HP[colIndex]*power;
        huixuancaidaoAtk = sGlobal->BOSS1_HUIXUAN_CAIDAO_ATK[colIndex]*power;
        
//        LOGE("combat round:%d, hp:%d, atk:%d, gold:%d, caidaoAtk:%d, kanAtk:%d, xuliliandaoHP:%d, xuliliandaoAtk:%d, huixuancaidaoHP:%d, huixuancaidaoAtk:%d", sGlobal->combatRound, hp, atk, gold, caidaoAtk, kanAtk, xuliliandaoHP, xuliliandaoAtk, huixuancaidaoHP, huixuancaidaoAtk);
    } else {
        int colIndex;
        switch (sGlobal->currentSModeDifficulty) {
            case DIFFICULTY_NORMAL:
                hp = sGlobal->ENEMY_HP_EASY[e_id];
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                colIndex = COL_NORMAL;
                break;
            case DIFFICULTY_HEROIC:
                hp = sGlobal->ENEMY_HP_HARD[e_id];
                atk = sGlobal->ENEMY_ATK_HARD[e_id];
                gold = sGlobal->ENEMY_GOLD_HARD[e_id];
                colIndex = COL_HARD;
                break;
            case DIFFICULTY_HARD:
                hp = sGlobal->ENEMY_HP_CRAZY[e_id];
                atk = sGlobal->ENEMY_ATK_CRAZY[e_id];
                gold = sGlobal->ENEMY_GOLD_CRAZY[e_id];
                colIndex = COL_CRAZY;
                break;
            default:
                hp = sGlobal->ENEMY_HP_EASY[e_id];
                atk = sGlobal->ENEMY_ATK_EASY[e_id];
                gold = sGlobal->ENEMY_GOLD_EASY[e_id];
                colIndex = COL_NORMAL;
                break;
        }
        
        if (sGlobal->isBoss1Strong) {
            hp = sGlobal->BOSS1_SMALL_HP[colIndex];
            gold = sGlobal->BOSS1_SMALL_GOLD[colIndex];

            caidaoAtk = sGlobal->BOSS1_SMALL_CAIDAO_ATK[colIndex];
            kanAtk = sGlobal->BOSS1_SMALL_KAN_ATK[colIndex];
            xuliliandaoHP = 0;
            xuliliandaoAtk = 0;
            huixuancaidaoHP = 0;
            huixuancaidaoAtk = 0;
        } else {
            caidaoAtk = sGlobal->BOSS1_CAIDAO_ATK[colIndex];
            kanAtk = sGlobal->BOSS1_KAN_ATK[colIndex];
            xuliliandaoHP = sGlobal->BOSS1_XULI_LIANDAO_HP[colIndex];
            xuliliandaoAtk = sGlobal->BOSS1_XULI_LIANDAO_ATK[colIndex];
            huixuancaidaoHP = sGlobal->BOSS1_HUIXUAN_CAIDAO_HP[colIndex];
            huixuancaidaoAtk = sGlobal->BOSS1_HUIXUAN_CAIDAO_ATK[colIndex];
        }
    }
    atk *= sGlobal->ENEMY_ATK_MULTI;
    hp *= sGlobal->ENEMY_HP_MULTI;
    gold *= sGlobal->ENEMY_GOLD_MULTI;
    
    caidaoAtk *= sGlobal->ENEMY_ATK_MULTI;
    kanAtk *= sGlobal->ENEMY_ATK_MULTI;
    xuliliandaoHP *= sGlobal->ENEMY_ATK_MULTI;
    xuliliandaoAtk *= sGlobal->ENEMY_ATK_MULTI;
    huixuancaidaoHP *= sGlobal->ENEMY_ATK_MULTI;
    huixuancaidaoAtk *= sGlobal->ENEMY_ATK_MULTI;
    
    atk = kanAtk;
//    hp = 100;
//    LOGE("diff:%d, e_id:%d, atk:%d, hp:%d, gold:%d, caidaoAtk:%d, kanAtk:%d, xuliliandaoHP:%d, xuliliandaoAtk:%d, huixuancaidaoHP:%d, huixuancaidaoAtk:%d", sGlobal->currentSModeDifficulty, e_id, atk, hp, gold, caidaoAtk, kanAtk, xuliliandaoHP, xuliliandaoAtk, huixuancaidaoHP, huixuancaidaoAtk);

    maxHp = hp;
    obj_type = TYPE_ENEMY;
    
    if (sGlobal->isBoss1Strong) {
        dialogBox = wySPXSprite::make("start_scene_anim.sprite", false, MyTextureManager::makePNG("start_scene_tex.png"), 10);//wySprite::make(MyTextureManager::makePNG("dialogue_frame.png"));
        dialogBox->setAnchor(0, 1.0f);
        dialogBox->setScaleX(sGlobal->scaleX*2.4f);
        dialogBox->setScaleY(sGlobal->scaleY*2.67f);
        dialogBox->setPosition(-DP(250)*sGlobal->scaleX, DP(190)*sGlobal->scaleY);
        container->addChildLocked(dialogBox,10);
        dialogBox->setVisible(false);
        
        wyColor3B subColor = wyc3b(255, 192, 0);
        
        content = wyBitmapFontLabel::make(sGlobal->font_desc, sGlobal->getStr(STR_BOSS1_TALK));
        content->setScale(SP(20)*sGlobal->scaleX/sGlobal->descFontSize);
        content->setAlignment(wyBitmapFontLabel::LEFT);
//        content->setLineWidth(DP(180)*sGlobal->scaleX/content->getScale());
//        content->setColor(wyc3b(255, 255, 0));
//        content->setAnchor(0, 1.0f);
        content->setColor(wyc3b(255, 85, 24));
        container->addChildLocked(content,10);
        content->setVisible(false);
        content->setPosition(DP(0)*sGlobal->scaleX, DP(130)*sGlobal->scaleY);
        
        hp /= 2;
        
        maxHp = hp;
        
        bossScale = 1.0f;
//        container->setScaleX(bossScale);
//        container->setScaleY(bossScale);
    } else {
        bossScale = 1.0f;
    }
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    b2World* world = m_box2d->getWorld();
    
	spxSprite = (wySPXSprite*)sGlobal->popFromCache(sGlobal->boss1Cache);//wySPXSprite::make("boss1.sprite", false, MyTextureManager::makePNG("boss1.png"), 0);
    spxSprite->setForceTickMode(true);
	spxSprite->setUnitInterval(0.2f);
    spxSprite->setLoopCount(-1);
    spxSprite->setScaleX(sGlobal->scaleX * 0.75f);
    spxSprite->setScaleY(sGlobal->scaleY * 0.75f);
    spxSprite->playAnimation(BOSS1_ANIM_DAIJI);
    container->addChildLocked(spxSprite);
    
    b2BodyDef bd;
    bd.position.Set(m_box2d->pixel2Meter(x), m_box2d->pixel2Meter(y));
    bd.userData = this;
    bd.type = b2_dynamicBody;
    body = world->CreateBody(&bd);
    body->SetTransform(bd.position, 0);
    
    bodyRect = spxSprite->getFrameRect();
    bodyRect.x *= spxSprite->getScaleX()*bossScale;
    bodyRect.y *= spxSprite->getScaleY()*bossScale;
    bodyRect.width *= spxSprite->getScaleX()*bossScale;
    bodyRect.height *= spxSprite->getScaleY()*bossScale;
    
    createFixtureByCurrentFrame(sGlobal->isRealEndless);
    body->SetActive(false);
    body->SetAwake(false);

    if (sGlobal->isRealEndless) {
        body->SetGravityScale(0.0f);
        baseY = m_box2d->pixel2Meter(y+DP(15)*sGlobal->scaleY);
        body->SetTransform(b2Vec2(m_box2d->pixel2Meter(x), baseY), 0);
    }
//    LOGE("boss1:%f,%f,%f", offsetX, offsetY, m_box2d->meter2Pixel(halfBodyHeightInMeter));
    
    isScheduled = false;

    status = BOSS1_IDLE;
    
    animDead = BOSS1_ANIM_SIWANG;
    animWalk = BOSS1_ANIM_WALK;
    animHurt = BOSS1_ANIM_SHOUSHANG;
    
    isDialogShowing = false;
    dialogTimer = 0;
    
//    stayDist = DP(620)*sGlobal->scaleX - bodyRect.width/2 - sGlobal->hero_offset_left - DP(0)*sGlobal->scaleX;
    stayDist = wyDevice::winWidth - sGlobal->hero_offset_left-DP(125)*sGlobal->scaleX;
    
    elapsedAtkTime = 0;
    
    target1 = NULL;
    target2 = NULL;
    target3 = NULL;
    shootFlags = new bool[3];
    
    initGouzi();
    
    huixuan_times = 0;
    shadowScaleX = 6.0f;
    shadowX = DP(20)*sGlobal->scaleX;
    shadowY = -DP(76)*sGlobal->scaleY;
    
    bossBlood->setPosition(DP(20)*sGlobal->scaleX, DP(100)*sGlobal->scaleY);
    
    spxCacheArray = sGlobal->boss1Cache;
    
    faintSpriteScale = 1.5*sGlobal->scaleX;
    faintSpriteX = DP(-10)*sGlobal->scaleY;
    faintSpriteY = DP(100)*sGlobal->scaleY;
}

Boss1::~Boss1() {
    delete [] shootFlags;
    instance = NULL;
}

void Boss1::update(float dt) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    Boss::update(dt);
    
    if (isDead) {
        deadSprite2->tick(dt);
    }
    
    if (status == BOSS1_IDLE && distance < stayDist) {
        spxSprite->playAnimation(BOSS1_ANIM_WALK);
        setCurVel(sGlobal->heroVelocityX, 0.0f);
        isMoving = true;
        status = BOSS1_WALK;
        
        sGlobal->playEffect(se_boss1_start);
        
        if (sGlobal->isBoss1Strong) {
            showDialogBox(STR_BOSS1_TALK);
        }
    }
    
    if (status == BOSS1_ATK_KAN_0 && distance < DP(160) * sGlobal->scaleX) {
        wyAFCSpriteCallback callback = {
            onAFCAnimationFrameChanged,
            onAFCAnimationEnded
        };
        spxSprite->setLoopCount(0);
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(BOSS1_ANIM_KAN);
        setCurVel(sGlobal->heroVelocityX, 0.0f);
        status = BOSS1_ATK_KAN_1;
    }
    
    if (status == BOSS1_ATK_KAN_2) {
        if (distance > stayDist) {
            setCurVel(sGlobal->heroVelocityX, 0.0f);
            status = BOSS1_WALK;
        }
    }
    
//    LOGE("status:%d, isScheduled:%s", status, isScheduled?"true":"false");
    if (status == BOSS1_WALK && !isScheduled && !isDead) {
        scheduleNextAttack();
    }
    
    float heroVelX = sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x;
    if (status != BOSS1_IDLE && fabs(body->GetLinearVelocity().x - relativeVelX - heroVelX ) > 0.0000001f) {
        body->SetLinearVelocity(b2Vec2(heroVelX + relativeVelX, body->GetLinearVelocity().y));
    }
    
    if (isDialogShowing) {
        dialogTimer += dt;
        if (dialogTimer > 3.0f) {
            hideDialogBox();
        }
    }
    
    if (status == BOSS1_ATK_XULI_LIANDAO_0) {
        if (curTime < 2) {
            stepTimer += dt;
            if (stepTimer > stepTime) {
                curTime++;
                generateNextTarget();
            }
        }
        
        if (attackFlag == 0) {
            totalTimer += dt;
            if (totalTimer > totalTime) {
                attackSuccess();
            }
        }
        
        if (target1 != NULL) {
            target1->body->SetTransform(body->GetPosition()+b2Vec2(target1->ofx, target1->ofy), 0);
        }
        
        if (target2 != NULL) {
            target2->body->SetTransform(body->GetPosition()+b2Vec2(target2->ofx, target2->ofy), 0);
        }
        
        if (target3 != NULL) {
            target3->body->SetTransform(body->GetPosition()+b2Vec2(target3->ofx, target3->ofy), 0);
        }
    }
    
    if (sGlobal->isRealEndless) {
        if (fabs(body->GetPosition().y-baseY) > 0.0000001f) {
//            LOGE("reset baseY...");
            body->SetTransform(b2Vec2(body->GetPosition().x, baseY), 0);
        }
    }
}

void Boss1::doAttack() {
    if (status == BOSS_FAKE_DEAD || isDead || sGlobal->mainGameLayer->hero->isDead) {
        return;
    }
    
    isScheduled = false;

//    if (isDialogShowing) {
//        return;
//    }

    int prob = rand()%100;
    bool needSE = true;
    if (sGlobal->isBoss1Strong) {
        if (prob <= 50) {
//            doAttackById(BOSS1_ATK_RENG_DAO);
            doAttack_Rengdao();
        }
        else {
//            doAttackById(BOSS1_ATK_KAN_0);
            doAttack_Kan();
            needSE = false;
        }
    } else {
        if (prob <= 25) {
//            doAttackById(BOSS1_ATK_RENG_DAO);
            doAttack_Rengdao();
        }
        else if (prob > 25 && prob <= 50) {
//            doAttackById(BOSS1_ATK_KAN_0);
            doAttack_Kan();
            needSE = false;
        }
        else if (prob > 50 && prob <= 75) {
//            doAttackById(BOSS1_ATK_HUIXUAN_CAIDAO_0);
            doAttack_RengHuiXuanDao();
            curSeq = POSSIBLE_COMBO[rand()%6];
            curTime = 0;
            huixuan_times = rand()%3+1;
        }
        else {
//            doAttackById(BOSS1_ATK_XULI_LIANDAO_0);
            doAttack_XuliSuolian();
        }
    }

    if (needSE) {
        if (rand()%2 == 0) {
            sGlobal->playEffect(se_boss1_attack1);
        } else {
            sGlobal->playEffect(se_boss1_attack2);
        }
    }
    
    spxSprite->setPaused(false);
}

void Boss1::doAttack_Rengdao() {
    status = BOSS1_ATK_RENG_DAO;

    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    spxSprite->setAFCSpriteCallback(&callback, this);
    spxSprite->setLoopCount(0);
    spxSprite->playAnimation(BOSS1_ANIM_RENG_KANDAO);
    
}

void Boss1::doAttack_RengHuiXuanDao() {
    status = BOSS1_ATK_HUIXUAN_CAIDAO_0;
    
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    spxSprite->setAFCSpriteCallback(&callback, this);
    spxSprite->setLoopCount(0);
    spxSprite->playAnimation(BOSS1_ANIM_RENG_HUIXUAN_DAO);
}

void Boss1::doAttack_XuliSuolian() {
    status = BOSS1_ATK_XULI_LIANDAO_0;
    
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    spxSprite->setAFCSpriteCallback(&callback, this);
    spxSprite->setLoopCount(-1);
    spxSprite->playAnimation(BOSS1_ANIM_XULI_LIANDAO);
    
    initShootTargets();
    
    sGlobal->mainGameLayer->addToRecreateFixture(this);
    
    xuanzhuansuolian->setVisible(true);
    xuanzhuansuolian->runAction(wyRepeatForever::make(wyRotateBy::make(0.35f, -360)));
}

void Boss1::doAttack_Kan() {
    status = BOSS1_ATK_KAN_0;
    
    wyAFCSpriteCallback callback = {
        onAFCAnimationFrameChanged,
        onAFCAnimationEnded
    };
    spxSprite->setAFCSpriteCallback(&callback, this);
    spxSprite->setLoopCount(-1);
    spxSprite->playAnimation(BOSS1_ANIM_DAIJI);
    
    setCurVel(-12, 0.0f);
}

void Boss1::doAttackById(int idx) {
//    LOGE("attack id:%d", idx);
    status = idx;
    int animIndex, loopCount;
    bool needCallBack = false;
    switch (idx) {
        case BOSS1_ATK_RENG_DAO:
            animIndex = BOSS1_ANIM_RENG_KANDAO;
            loopCount = 0;
            needCallBack = true;
            break;
        case BOSS1_ATK_KAN_0:
            animIndex = BOSS1_ANIM_DAIJI;
            loopCount = -1;
            setCurVel(-12, 0.0f);
            
//            spxSprite->setUnitInterval(randRange(2, 5)/10.0f);
            break;
        case BOSS1_ATK_XULI_LIANDAO_0:
            animIndex = BOSS1_ANIM_XULI_LIANDAO;
            loopCount = -1;
            initShootTargets();
            
            sGlobal->mainGameLayer->addToRecreateFixture(this);
            
            xuanzhuansuolian->setVisible(true);
            xuanzhuansuolian->runAction(wyRepeatForever::make(wyRotateBy::make(0.35f, -360)));
            break;
        case BOSS1_ATK_HUIXUAN_CAIDAO_0:
            animIndex = BOSS1_ANIM_RENG_HUIXUAN_DAO;
            loopCount = 0;
            needCallBack = true;
            
            huixuan_times++;
            break;
        default:
            break;
    }
    
//    if (needCallBack) {
        wyAFCSpriteCallback callback = {
            onAFCAnimationFrameChanged,
            onAFCAnimationEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
//    }
    
    spxSprite->setLoopCount(loopCount);
    spxSprite->playAnimation(animIndex);
}

static void onFakeDeadEnded(wyAFCSprite* sprite, void* data) {
    Boss1 *boss1 = (Boss1 *)data;
    boss1->throwCoin();
}

void Boss1::fakeDead() {
    status = BOSS_FAKE_DEAD;
    
    instance = NULL;
    
    deadSprite2 = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), SPX_BOSS_DEAD_BOMB);
    sGlobal->mainGameLayer->addChildLocked(deadSprite2, container->getZOrder());
    deadSprite2->setForceTickMode(true);
    deadSprite2->setUnitInterval(0.2f);
    deadSprite2->setLoopCount(0);
    deadSprite2->setFlipX(false);
    deadSprite2->setScaleX(1.3f*sGlobal->scaleX);
    deadSprite2->setScaleY(1.3f*sGlobal->scaleY);
    deadSprite2->setPosition(container->getPositionX() - DP(0), container->getPositionY()+DP(0));
    deadSprite2->setTag(1);
    wyAFCSpriteCallback callback = {
        NULL,
        onFakeDeadEnded
    };
    deadSprite2->setAFCSpriteCallback(&callback, this);
    
    sGlobal->bossEyeZ = 1;
    sGlobal->playEffect(se_boss1_dead);

    hideGouzi();
    deleteGouziObj();
    
    dead();
}

void Boss1::throwCoin() {
    if (sGlobal->isRealEndless) {
        dead();
        return;
    }
    
    int x, y;
    int x1, y1;
    if (sGlobal->isBoss1Strong) {
        //10*4 = 40
        for (int i = 0; i < 15; i++) {
            x = sGlobal->mainGameLayer->hero->container->getPositionX() + wyDevice::winWidth + DP(300)*sGlobal->scaleX + DP(80)*i*sGlobal->scaleX;
            y = baseY + DP(60)*sGlobal->scaleY;
            BonusCoin::make(x, y, COIN, false, true);
        }
        
        EndPoint::make(x + DP(500)*sGlobal->scaleX, container->getPositionY()-DP(77)*sGlobal->scaleY, true);
    } else {
        //total:180
        //10 coins 40
        for (int i = 0; i < 10; i++) {
            x = sGlobal->mainGameLayer->hero->container->getPositionX() + wyDevice::winWidth + DP(300)*sGlobal->scaleX + DP(80)*i*sGlobal->scaleX;
            y = baseY + DP(60)*sGlobal->scaleY;
            BonusCoin::make(x, y, COIN, false, true);
        }
        //2 big coins 40
        
        for (int i = 1; i < 3; i++) {
            x1 = x + DP(200)*i*sGlobal->scaleX;
            y1 = container->getPositionY() + DP(100)*sGlobal->scaleY;
            BonusCoin::make(x1, y1, BIGCOIN, false, true);
        }
        //1 bigbig coin 100
        BonusCoin::make(x1 + DP(300)*sGlobal->scaleX, container->getPositionY() + DP(300)*sGlobal->scaleY, BIGBIGCOIN, false, true);
        
        EndPoint::make(x1 + DP(500)*sGlobal->scaleX, container->getPositionY()-DP(82)*sGlobal->scaleY, true);
    }

    dead();
}

void Boss1::showDialogBox(int idx) {
    isDialogShowing = true;
    dialogTimer = 0;
    
    content->setText(sGlobal->getStr(idx));
    
//    dialogBox->setVisible(true);
    content->setVisible(true);
//    dialogBox->setPosition(container->getPositionX() - DP(24)*sGlobal->scaleX, container->getPositionY() + DP(70)*sGlobal->scaleY);
//    content->setPosition(dialogBox->getPositionX() - DP(250)*sGlobal->scaleX, dialogBox->getPositionY() + DP(176)*sGlobal->scaleY);
}

void Boss1::hideDialogBox() {
    isDialogShowing = false;
    dialogBox->setVisible(false);
    content->setVisible(false);
}

void Boss1::shootCaiDao(int no) {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

//    CaiDao *enemyBullet = CaiDao::make(sGlobal->boss1Cache, BOSS1_ANIM_CAIDAO, 0.1f, sGlobal->scaleX*0.75f, sGlobal->scaleY*0.75f);
//    enemyBullet->atk = atk;
//    b2Body *bulletBody = enemyBullet->body;
//    bulletBody->SetTransform(b2Vec2(body->GetPosition().x - m_box2d->pixel2Meter(DP(0)*sGlobal->scaleX), body->GetPosition().y + m_box2d->pixel2Meter(DP(20)*sGlobal->scaleX)), 0);
//    bulletBody->SetGravityScale(0.0f);
//
//    enemyBullet->spxSprite->setPosition(m_box2d->meter2Pixel(bulletBody->GetPosition().x), m_box2d->meter2Pixel(bulletBody->GetPosition().y));
//    
//    wyHypotrochoidConfig m_circle;
//    m_circle = wyhcCircle(DP(100)*sGlobal->scaleX, 270, -90, enemyBullet->spxSprite->getPositionX()-DP(100)*sGlobal->scaleX, enemyBullet->spxSprite->getPositionY()+DP(100)*sGlobal->scaleX);
//    wyHypotrochoid* a = wyHypotrochoid::make(1.0f, m_circle);
//    a->setAutoRotate(true, 180);
//    wyIntervalAction *a1 = wySequence::make(wyMoveBy::make(0.1f, -DP(100)*sGlobal->scaleX, 0), a, a->copy(), NULL);
//    enemyBullet->spxSprite->runAction(a1);
//    
//    enemyBullet->container->runAction(wyMoveBy::make(2.1f, m_box2d->meter2Pixel(2.1f*HERO_VEL_X), 0));
    
//    CaiDao *enemyBullet = CaiDao::make(sGlobal->boss1Cache, BOSS1_ANIM_CAIDAO, 0.1f, sGlobal->scaleX*0.75f, sGlobal->scaleY*0.75f);
//    enemyBullet->atk = atk;
//    b2Body *bulletBody = enemyBullet->body;
//    bulletBody->SetTransform(b2Vec2(body->GetPosition().x - m_box2d->pixel2Meter(DP(0)*sGlobal->scaleX), body->GetPosition().y + m_box2d->pixel2Meter(DP(20)*sGlobal->scaleX)), 0);
//    bulletBody->SetGravityScale(0.25f);
//    
//    enemyBullet->container->setPosition(m_box2d->meter2Pixel(bulletBody->GetPosition().x), m_box2d->meter2Pixel(bulletBody->GetPosition().y));
//    
//    float st = (480 - 250*0.75f)/16.0f;
//    float tv = HERO_VEL_X - (HERO_VEL_X-1.0f)/st * m_box2d->pixel2Meter(stayDist);
//    bulletBody->SetLinearVelocity(b2Vec2(tv,20));

    float heroVelX = sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x;
    float ox, oy;
    ox = -m_box2d->pixel2Meter(DP(140)*sGlobal->scaleX);
    oy = m_box2d->pixel2Meter(DP(0)*sGlobal->scaleY);
    
    float stime;// = randRange(5, 10)/10.0f;//(480 - 250*0.75f - 120*0.75f - 112*0.75f)/16.0f/4.5f;
    float tv;// = heroVelX - m_box2d->pixel2Meter(stayDist-DP(120)*sGlobal->scaleX-DP(25)*sGlobal->scaleX)/stime;
    //LOGE("tv:%f, dist:%f, stime:%f", tv, m_box2d->pixel2Meter(stayDist-DP(25)*sGlobal->scaleX), stime);
    
    CaiDao *enemyBullet = new CaiDao(sGlobal->boss1Cache, BOSS1_ANIM_KANDAO, 0.1f, sGlobal->scaleX*0.75f*bossScale, sGlobal->scaleY*0.75f*bossScale);
    enemyBullet->atk = caidaoAtk;
    b2Body *bulletBody = enemyBullet->body;
    bulletBody->SetTransform(b2Vec2(body->GetPosition().x+ox, body->GetPosition().y + oy), 0);
    bulletBody->SetGravityScale(0.25f);
    enemyBullet->container->setPosition(m_box2d->meter2Pixel(bulletBody->GetPosition().x), m_box2d->meter2Pixel(bulletBody->GetPosition().y));

//    enemyBullet->container->setRotation(-20);
    enemyBullet->container->runAction(wyRepeatForever::make(wyRotateBy::make(0.36f, -360)));
    
//    int seed = rand()%100;
//    no = 1;
    switch (no) {
        case 1:
            stime = randRange(5, 10)/10.0f;//(480 - 250*0.75f - 120*0.75f - 112*0.75f)/16.0f/4.5f;
            tv = heroVelX - m_box2d->pixel2Meter(stayDist-DP(120)*sGlobal->scaleX-DP(25)*sGlobal->scaleX)/stime;
            
            bulletBody->SetGravityScale(-0.6f);
            bulletBody->SetLinearVelocity(b2Vec2(tv,-32));
            break;
        case 2:
            stime = 0.82f;//(480 - 250*0.75f - 120*0.75f - 112*0.75f)/16.0f/4.5f;
            tv = heroVelX - m_box2d->pixel2Meter(stayDist-DP(120)*sGlobal->scaleX-DP(25)*sGlobal->scaleX)/stime;
            
            bulletBody->SetGravityScale(0.5f);
            bulletBody->SetLinearVelocity(b2Vec2(tv,randRange(20, 25)));
            break;
        case 3:
            stime = randRange(6, 10)/10.0f;//(480 - 250*0.75f - 120*0.75f - 112*0.75f)/16.0f/4.5f;
            tv = heroVelX - m_box2d->pixel2Meter(stayDist-DP(120)*sGlobal->scaleX-DP(25)*sGlobal->scaleX)/stime;
            
            bulletBody->SetGravityScale(0);
            bulletBody->SetLinearVelocity(b2Vec2(tv, -2));
            break;
        default:
            break;
    }
    
//    bulletBody->SetGravityScale(0);
//    bulletBody->SetLinearVelocity(b2Vec2(tv,0));
}

void Boss1::initShootTargets() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
//    target1 = new ShootTarget(container->getPositionX(), container->getPositionY(), -DP(0)*sGlobal->scaleX, DP(90)*sGlobal->scaleX, 0);
//
//    target2 = new ShootTarget(container->getPositionX(), container->getPositionY(), -DP(0)*sGlobal->scaleX, DP(50)*sGlobal->scaleX, 1);
//    
//    target3 = new ShootTarget(container->getPositionX(), container->getPositionY(), -DP(60)*sGlobal->scaleX, -DP(10)*sGlobal->scaleX, 2);
    
//    shootTargets = new ShootTarget[3];
//    shootTargets[0] = new ShootTarget(container->getPositionX(), container->getPositionY(), -DP(0)*sGlobal->scaleX, DP(90)*sGlobal->scaleX, 0);
//    shootTargets[1] = new ShootTarget(container->getPositionX(), container->getPositionY(), -DP(0)*sGlobal->scaleX, DP(50)*sGlobal->scaleX, 1);
//    shootTargets[2] = new ShootTarget(container->getPositionX(), container->getPositionY(), -DP(60)*sGlobal->scaleX, -DP(10)*sGlobal->scaleX, 2);
    isGouziHide = false;
    
    shootFlags[0] = false;
    shootFlags[1] = false;
    shootFlags[2] = false;
    
    time2 = rand()%10/10.0f;
    time3 = rand()%10/10.0f;
    totalTime = time2 + time3 + 2.0f;
    stepTime = time2;
    
    curSeq = POSSIBLE_COMBO[rand()%6];
    curTime = 0;
    stepTimer = 0;
    totalTimer = 0;
    attackFlag = 0;//0:正在进行；1:成功; 2:失败
    //LOGE("init: seq:%d", curSeq);
    generateNextTarget();
}

void Boss1::generateNextTarget() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    switch (curTime) {
        case 0:
            curIndex = curSeq / 100;
            stepTime = time2;
            break;
        case 1:
            curIndex = curSeq / 10 - curSeq / 100 * 10;
            stepTime = time3;
            break;
        case 2:
            curIndex = curSeq % 10;
            break;
        default:
            break;
    }
    stepTimer = 0;
    
    switch (curIndex) {
        case 1:
            target1 = new ShootTarget(body->GetPosition().x, body->GetPosition().y, m_box2d->pixel2Meter(DP(80)*sGlobal->scaleX*bossScale), m_box2d->pixel2Meter(DP(90)*sGlobal->scaleY*bossScale), 0, this);
            target1->hp = xuliliandaoHP;
            break;
        case 2:
            target2 = new ShootTarget(body->GetPosition().x, body->GetPosition().y, m_box2d->pixel2Meter(DP(60)*sGlobal->scaleX*bossScale), m_box2d->pixel2Meter(DP(35)*sGlobal->scaleY*bossScale), 1, this);
            target2->hp = xuliliandaoHP;
            break;
        case 3:
            target3 = new ShootTarget(body->GetPosition().x, body->GetPosition().y, m_box2d->pixel2Meter(DP(10)*sGlobal->scaleX*bossScale), m_box2d->pixel2Meter(-DP(40)*sGlobal->scaleY*bossScale), 2, this);
            target3->hp = xuliliandaoHP;
            break;
        default:
            break;
    }
    //LOGE("generateNextTarget, curTime:%d, curIndex:%d, stepTime:%f", curTime, curIndex, stepTime);
}

void Boss1::attackSuccess() {
    //LOGE("attack success...");
    attackFlag = 1;
    
    if (!shootFlags[0] || !shootFlags[1] || !shootFlags[2]) {
        if (!shootFlags[0] && !shootFlags[1] && !shootFlags[2]) {
//            LOGE("shoot 3");
            //扔3个钩子
            attackFlag = 3;
        }
//        else if ((!shootFlags[0] && !shootFlags[1]) || (!shootFlags[0] && !shootFlags[2]) || (!shootFlags[1] && !shootFlags[2])) {
//            LOGE("shoot 1");
//            //扔1个钩子
//            attackFlag = 2;
//        }
        else {
//            LOGE("shoot 1");
            //扔1个钩子
            attackFlag = 1;
        }
        spxSprite->setLoopCount(0);
        wyAFCSpriteCallback callback = {
            onAFCAnimationFrameChanged,
            onAFCAnimationEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(BOSS1_ANIM_RENG_SUCCESS);
        status = BOSS1_ATK_XULI_LIANDAO_1;
        throwGouzi();
    }
    
    hideGouzi();
}

void Boss1::attackFailed() {
//    LOGE("attack failed..");
    attackFlag = -1;
    xuanzhuansuolian->stopAllActions();
    xuanzhuansuolian->setVisible(false);
    
    sGlobal->playEffect(se_boss1_attack2);
    
//    delete[] shootFlags;
//    shootFlags = NULL;
}

void Boss1::onTargetShootEnd(int index) {
    if (isGouziHide) {
//        LOGE("gouzi has hidden...");
        return;
    }
    if (attackFlag == 1) {
//        LOGE("boss1 atk success, return...");
        return;
    }
    
    //LOGE("%d has shooted...", index);
    switch (index) {
        case 0:
            target1 = NULL;
            shootFlags[0] = true;
            break;
        case 1:
            target2 = NULL;
            shootFlags[1] = true;
            break;
        case 2:
            target3 = NULL;
            shootFlags[2] = true;
            break;
        default:
            break;
    }
    
    if (shootFlags[0] && shootFlags[1] && shootFlags[2]) {
        attackFailed();
        
        spxSprite->setLoopCount(0);
        wyAFCSpriteCallback callback = {
            NULL,
            onAFCAnimationEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->playAnimation(BOSS1_ANIM_RENG_FAILED);
        status = BOSS1_ATK_XULI_LIANDAO_1;
    }
}

void Boss1::initGouzi() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    gouziLength = stayDist-DP(180)*sGlobal->scaleX*bossScale;
    
    //gouzi1 向下
    gouziContainer1 = wyNode::make();
    gouziContainer1->setAnchor(1.0, 0.5f);
    container->addChildLocked(gouziContainer1, -1);
    gouziContainer1->setPosition(DP(-20)*sGlobal->scaleX*bossScale, DP(45)*sGlobal->scaleY*bossScale);
    gouziContainer1->setRotation(-10);
    
    suolian1 = wyTiledSprite::make(MyTextureManager::makePNG("suolian.png"));
    suolian1->setScaleY(sGlobal->scaleY*0.8f*bossScale);
    suolian1->setContentSize(gouziLength, DP(30));
    suolian1->setTileDirection(true, false);
    suolian1->setAnchor(0.0f, 0.5f);
    suolian1->setSpacing(-DP(6)*sGlobal->scaleX, 0);
    gouziContainer1->addChildLocked(suolian1, -1);
    suolian1->setVisible(false);

    gouzi1 = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), BOSS1_ANIM_LIANDAO);
    gouzi1->setForceTickMode(true);
    gouzi1->setScaleX(sGlobal->scaleX * 0.6f*bossScale);
    gouzi1->setScaleY(sGlobal->scaleY * 0.6f*bossScale);
    gouzi1->setAnchor(1.0f, 0.5f);
    gouziContainer1->addChildLocked(gouzi1);
    gouzi1->setVisible(false);
    
    //gouzi2 向上
    gouziContainer2 = wyNode::make();
    gouziContainer2->setAnchor(1.0, 0.5f);
    container->addChildLocked(gouziContainer2, -1);
    gouziContainer2->setPosition(DP(-20)*sGlobal->scaleX*bossScale, DP(45)*sGlobal->scaleY*bossScale);
    gouziContainer2->setRotation(10);
    
    suolian2 = wyTiledSprite::make(MyTextureManager::makePNG("suolian.png"));
    suolian2->setScaleY(sGlobal->scaleY*0.8f*bossScale);
    suolian2->setContentSize(gouziLength, DP(30));
    suolian2->setTileDirection(true, false);
    suolian2->setAnchor(0.0f, 0.5f);
    suolian2->setSpacing(-DP(6)*sGlobal->scaleX, 0);
    gouziContainer2->addChildLocked(suolian2, -1);
    suolian2->setVisible(false);
    
    gouzi2 = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), BOSS1_ANIM_LIANDAO);
    gouzi2->setForceTickMode(true);
    gouzi2->setScaleX(sGlobal->scaleX * 0.6f*bossScale);
    gouzi2->setScaleY(sGlobal->scaleY * 0.6f*bossScale);
    gouzi2->setAnchor(1.0f, 0.5f);
    gouziContainer2->addChildLocked(gouzi2);
    gouzi2->setVisible(false);
    
    //gouzi3 水平
    gouziContainer3 = wyNode::make();
    gouziContainer3->setAnchor(1.0, 0.5f);
    container->addChildLocked(gouziContainer3, -1);
    gouziContainer3->setPosition(DP(-20)*sGlobal->scaleX*bossScale, DP(45)*sGlobal->scaleY*bossScale);
    
    suolian3 = wyTiledSprite::make(MyTextureManager::makePNG("suolian.png"));
    suolian3->setScaleY(sGlobal->scaleY*0.8f*bossScale);
    suolian3->setContentSize(gouziLength, DP(30));
    suolian3->setTileDirection(true, false);
    suolian3->setAnchor(0.0f, 0.5f);
    suolian3->setSpacing(-DP(6)*sGlobal->scaleX, 0);
    gouziContainer3->addChildLocked(suolian3, -1);
    suolian3->setVisible(false);
    
    gouzi3 = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), BOSS1_ANIM_LIANDAO);
    gouzi3->setForceTickMode(true);
    gouzi3->setScaleX(sGlobal->scaleX * 0.6f*bossScale);
    gouzi3->setScaleY(sGlobal->scaleY * 0.6f*bossScale);
    gouzi3->setAnchor(1.0f, 0.5f);
    gouziContainer3->addChildLocked(gouzi3);
    gouzi3->setVisible(false);
    
    //旋转镰刀
    xuanzhuansuolian = wyNode::make();
    xuanzhuansuolian->setVisible(false);
    xuanzhuansuolian->setPosition(DP(85)*sGlobal->scaleY*bossScale, DP(125)*sGlobal->scaleY*bossScale);
    container->addChildLocked(xuanzhuansuolian, -1);
    
    wyTiledSprite *suolian = wyTiledSprite::make(MyTextureManager::makePNG("suolian.png"));
    suolian->setScaleY(sGlobal->scaleY*0.8f*bossScale);
    suolian->setContentSize(DP(50)*sGlobal->scaleX*bossScale, DP(30));
    suolian->setTileDirection(true, false);
    suolian->setAnchor(0.0f, 0.5f);
    suolian->setSpacing(-DP(6)*sGlobal->scaleX, 0);
    suolian->setPosition(-DP(50)*sGlobal->scaleX*bossScale, 0);
    xuanzhuansuolian->addChildLocked(suolian, -1);
    
    wySPXSprite *gouzi = wySPXSprite::make("effects.sprite", false, MyTextureManager::makePNG("effects.png"), BOSS1_ANIM_LIANDAO);
    gouzi->setForceTickMode(true);
    gouzi->setScaleX(sGlobal->scaleX * 0.7f*bossScale);
    gouzi->setScaleY(sGlobal->scaleY * 0.7f*bossScale);
    gouzi->setAnchor(1.0f, 0.5f);
    gouzi->setPosition(-DP(50)*sGlobal->scaleX, 0);
    xuanzhuansuolian->addChildLocked(gouzi);
    
    gouziObj1 = NULL;
    gouziObj2 = NULL;
    gouziObj3 = NULL;
}

static void onGouZiEnd(wyAction* action, void* data) {
    EnemyBulletBase* bullet = (EnemyBulletBase*)data;
    bullet->destroyMyself();
}

void Boss1::hideGouzi() {
    isGouziHide = true;
    
    xuanzhuansuolian->setVisible(false);
    xuanzhuansuolian->stopAllActions();
    
    if (target1 != NULL) {
//        target1->body->SetAwake(false);
//        target1->body->SetActive(false);
        sGlobal->mainGameLayer->addToDestroy(target1->body);
        
        target1 = NULL;
    }
    if (target2 != NULL) {
//        target2->body->SetAwake(false);
//        target2->body->SetActive(false);
        sGlobal->mainGameLayer->addToDestroy(target2->body);
        
        target2 = NULL;
    }
    if (target3 != NULL) {
//        target3->body->SetAwake(false);
//        target3->body->SetActive(false);
        sGlobal->mainGameLayer->addToDestroy(target3->body);
        
        target3 = NULL;
    }
    
//    if (shootFlags != NULL) {
//        LOGE("aaa");
//        delete[] shootFlags;
//        shootFlags = NULL;
//    }
}

void Boss1::throwGouzi() {
    if (attackFlag == 3) {
        //gouzi1
        suolian1->setVisible(true);
        gouzi1->setVisible(true);
        
        suolian1->setPosition(-DP(130)*sGlobal->scaleX*bossScale, DP(0)*sGlobal->scaleY);
        gouzi1->setPosition(-DP(130)*sGlobal->scaleX*bossScale, DP(0)*sGlobal->scaleY);
        
        suolian1->runAction(wyMoveBy::make(0.7f, -gouziLength, 0));
        gouzi1->runAction(wyMoveBy::make(0.7f, -gouziLength, 0));
        
        suolian1->runAction(wySequence::make(wyDelayTime::make(0.7f), wyMoveBy::make(0.3f, gouziLength, 0), wyHide::make(), NULL));
        gouzi1->runAction(wySequence::make(wyDelayTime::make(0.7f), wyMoveBy::make(0.3f, gouziLength, 0), wyHide::make(), NULL));
        
        gouziObj1 = new GouziObject(container->getPositionX()+gouzi1->getPositionX(), container->getPositionY()+gouzi1->getPositionY(), xuliliandaoAtk, gouzi1, -DP(50)*sGlobal->scaleX*bossScale, -DP(0)*sGlobal->scaleX, 0, 10);
        
        //gouzi2
        suolian2->setVisible(true);
        gouzi2->setVisible(true);
        
        suolian2->setPosition(-DP(130)*sGlobal->scaleX*bossScale, DP(0)*sGlobal->scaleY);
        gouzi2->setPosition(-DP(130)*sGlobal->scaleX*bossScale, DP(0)*sGlobal->scaleY);
        
        suolian2->runAction(wyMoveBy::make(0.7f, -gouziLength, 0));
        gouzi2->runAction(wyMoveBy::make(0.7f, -gouziLength, 0));
        
        suolian2->runAction(wySequence::make(wyDelayTime::make(0.7f), wyMoveBy::make(0.3f, gouziLength, 0), wyHide::make(), NULL));
        gouzi2->runAction(wySequence::make(wyDelayTime::make(0.7f), wyMoveBy::make(0.3f, gouziLength, 0), wyHide::make(), NULL));
        
        gouziObj2 = new GouziObject(container->getPositionX()+gouzi2->getPositionX(), container->getPositionY()+gouzi2->getPositionY(), xuliliandaoAtk, gouzi2, -DP(50)*sGlobal->scaleX*bossScale, -DP(20)*sGlobal->scaleX*bossScale, 1, -10);
        
    }
    
    //gouzi3
    suolian3->setVisible(true);
    gouzi3->setVisible(true);
    
    suolian3->setPosition(-DP(130)*sGlobal->scaleX*bossScale, DP(0)*sGlobal->scaleY);
    gouzi3->setPosition(-DP(130)*sGlobal->scaleX*bossScale, DP(0)*sGlobal->scaleY);
    
    suolian3->runAction(wyMoveBy::make(0.7f, -gouziLength, 0));
    gouzi3->runAction(wyMoveBy::make(0.7f, -gouziLength, 0));
    
    suolian3->runAction(wySequence::make(wyDelayTime::make(0.7f), wyMoveBy::make(0.3f, gouziLength, 0), wyHide::make(), NULL));
    gouzi3->runAction(wySequence::make(wyDelayTime::make(0.7f), wyMoveBy::make(0.3f, gouziLength, 0), wyHide::make(), NULL));
    
    gouziObj3 = new GouziObject(container->getPositionX()+gouzi3->getPositionX(), container->getPositionY()+gouzi3->getPositionY(), xuliliandaoAtk, gouzi3, -DP(50)*sGlobal->scaleX*bossScale, -DP(20)*sGlobal->scaleX*bossScale, 2, 0);
}

void Boss1::deleteGouziObj() {
    if (gouziObj1 != NULL) {
        sGlobal->mainGameLayer->addToDestroy(gouziObj1->body);
    }
    
    if (gouziObj2 != NULL) {
        sGlobal->mainGameLayer->addToDestroy(gouziObj2->body);
    }
    
    if (gouziObj3 != NULL) {
        sGlobal->mainGameLayer->addToDestroy(gouziObj3->body);
    }
}

void Boss1::throwHuixuanCaidao() {
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;
    
    HuixuanCaidao *enemyBullet = new HuixuanCaidao(sGlobal->boss1Cache, BOSS1_ANIM_CAIDAO, 0.1f, 0.7*sGlobal->scaleX, 0.7*sGlobal->scaleY, DP(20), DP(20), huixuancaidaoAtk);
    enemyBullet->hp = huixuancaidaoHP;
    
    b2Body *bulletBody = enemyBullet->body;
    bulletBody->SetGravityScale(0.0f);
    
    float startX, startY, endX, endY, c1X, c1Y, c2X, c2Y;
    
    switch (curTime) {
        case 0:
            curIndex = curSeq / 100;
            break;
        case 1:
            curIndex = curSeq / 10 - curSeq / 100 * 10;
            break;
        case 2:
            curIndex = curSeq % 10;
            break;
        default:
            break;
    }
    
//    curIndex = 3;
    float heroVelx = sGlobal->mainGameLayer->hero->body->GetLinearVelocity().x;
    float speedScale = 1.0f;
    if (sGlobal->mainGameLayer->hero->isBoosting) {
        speedScale = 2.0f;
    }
    switch (curIndex) {
        case 1: {
            startX = m_box2d->meter2Pixel(body->GetPosition().x)-DP(55)*sGlobal->scaleX*speedScale;
            startY = m_box2d->meter2Pixel(body->GetPosition().y)+DP(50)*sGlobal->scaleY;
            endX = m_box2d->meter2Pixel(sGlobal->mainGameLayer->hero->body->GetPosition().x) + DP(0)*sGlobal->scaleX;
            endY = startY + DP(500)*sGlobal->scaleY;
            c1X = startX+DP(600)*sGlobal->scaleX*speedScale;
            c1Y = startY+DP(1000)*sGlobal->scaleY;
            c2X = endX+DP(1600)*sGlobal->scaleX*speedScale;
            c2Y = endY-DP(1500)*sGlobal->scaleY;
            
            wyBezierConfig m_config = wybcCubic(startX,
                                                startY,
                                                endX,
                                                endY,
                                                c1X,
                                                c1Y,
                                                c2X,
                                                c2Y);
            wyBezier* a = wyBezier::make(4.5f, m_config);
            enemyBullet->container->runAction(a);
            bulletBody->SetTransform(b2Vec2(m_box2d->pixel2Meter(startX), m_box2d->pixel2Meter(startY)), 0);
            enemyBullet->container->setPosition(startX, startY);
            
            enemyBullet->spxSprite->runAction(wyMoveBy::make(4.5f, m_box2d->meter2Pixel(heroVelx/4.5f*4.5f), 0));//DP(200)*sGlobal->scaleX
            break;
        }
        case 2: {
            startX = m_box2d->meter2Pixel(body->GetPosition().x)-DP(55)*sGlobal->scaleX*speedScale;
            startY = m_box2d->meter2Pixel(body->GetPosition().y)+DP(50)*sGlobal->scaleY;
            endX = m_box2d->meter2Pixel(sGlobal->mainGameLayer->hero->body->GetPosition().x) + DP(0)*sGlobal->scaleX;
            endY = startY + DP(760)*sGlobal->scaleY;
            c1X = startX+DP(600)*sGlobal->scaleX*speedScale;
            c1Y = startY+DP(800)*sGlobal->scaleY;
            c2X = endX+DP(2000)*sGlobal->scaleX*speedScale;
            c2Y = endY-DP(2000)*sGlobal->scaleY;
            
            wyBezierConfig m_config = wybcCubic(startX,
                                                startY,
                                                endX,
                                                endY,
                                                c1X,
                                                c1Y,
                                                c2X,
                                                c2Y);
            wyBezier* a = wyBezier::make(6.5f, m_config);
            enemyBullet->container->runAction(a);
            bulletBody->SetTransform(b2Vec2(m_box2d->pixel2Meter(startX), m_box2d->pixel2Meter(startY)), 0);
            enemyBullet->container->setPosition(startX, startY);
            
            enemyBullet->spxSprite->runAction(wyMoveBy::make(6.5f, m_box2d->meter2Pixel(heroVelx/3.3f*6.5f), 0));
            break;
        }
        case 3: {
            startX = m_box2d->meter2Pixel(body->GetPosition().x)-DP(55)*sGlobal->scaleX*speedScale;
            startY = m_box2d->meter2Pixel(body->GetPosition().y)+DP(50)*sGlobal->scaleY;
            endX = startX + DP(800)*sGlobal->scaleX*speedScale;
            endY = startY + DP(100)*sGlobal->scaleY;
            c1X = startX+DP(200)*sGlobal->scaleX*speedScale;
            c1Y = startY+DP(400)*sGlobal->scaleY;
            c2X = endX-DP(100)*sGlobal->scaleX*speedScale;
            c2Y = endY+DP(400)*sGlobal->scaleY;
            
            wyBezierConfig m_config = wybcCubic(startX,
                                                startY,
                                                endX,
                                                endY,
                                                c1X,
                                                c1Y,
                                                c2X,
                                                c2Y);
            wyBezier* a = wyBezier::make(4.0f, m_config);
//            enemyBullet->container->runAction(a);
            bulletBody->SetTransform(b2Vec2(m_box2d->pixel2Meter(startX), m_box2d->pixel2Meter(startY)), 0);
            enemyBullet->container->setPosition(startX, startY);
            
            sGlobal->mainGameLayer->m_config = m_config;
            
            startX = endX;
            startY = endY;
            endX = m_box2d->meter2Pixel(sGlobal->mainGameLayer->hero->body->GetPosition().x) + DP(500)*sGlobal->scaleX*speedScale;
            endY = startY - DP(400)*sGlobal->scaleY;
            c1X = startX-DP(200)*sGlobal->scaleX*speedScale;
            c1Y = startY+DP(20)*sGlobal->scaleY;
            c2X = endX+DP(200)*sGlobal->scaleX*speedScale;
            c2Y = endY+DP(200)*sGlobal->scaleY;
            
            m_config = wybcCubic(startX,
                                                startY,
                                                endX,
                                                endY,
                                                c1X,
                                                c1Y,
                                                c2X,
                                                c2Y);
            wyBezier* a1 = wyBezier::make(4.0f, m_config);
            enemyBullet->container->runAction(wySequence::make(a, a1, NULL));//
            
            
            enemyBullet->spxSprite->runAction(wyMoveBy::make(8.0f, m_box2d->meter2Pixel(heroVelx/1.7f*8.0f), 0));//DP(1200)*sGlobal->scaleX
            
            sGlobal->mainGameLayer->m_config1 = m_config;
//            sGlobal->mainGameLayer->drawDebug = true;
            break;
        }
        default:
            break;
    }
    curTime++;
    
    enemyBullet->spxSprite->runAction(wyRepeatForever::make(wyRotateBy::make(0.5f, -360)));
}

void Boss1::hurt(float y) {
    Boss::hurt(y);
    
    wyBox2D *m_box2d = sGlobal->mainGameLayer->m_box2d;

    if (status == BOSS1_WALK && !isRecreatingFixture && rand()%5 == 0) { // && rand()%3 == 0
        wyAFCSpriteCallback callback = {
            NULL,
            onAFCAnimationEnded
        };
        spxSprite->setAFCSpriteCallback(&callback, this);
        spxSprite->setLoopCount(0);
        spxSprite->playAnimation(BOSS1_ANIM_SHOUSHANG);
        status = BOSS_HURT;
//        isScheduled = false;
        
//        sGlobal->playEffect(se_boss1_attack2);
    }
}


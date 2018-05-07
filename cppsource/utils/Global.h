#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "Singleton.h"
#include "MainGameLayer.h"
#include "GameBackgroundLayer.h"
#include "HudLayer.h"
#include "EffectLayer.h"
#include <string>
#include "WiEngine-WiSound.h"
#include "DeadNoticeLayer.h"
#include "OptionLayer.h"
#include "mtbase64.h"
//#include "ShopLayer.h"
#include "MyTextureManager.h"
#if ANDROID
#include "com_wiyun_engine_skeleton_Skeleton.h"
#endif
#if IOS
#include "ObjCCalls.h"
#endif

#include "wyJSONArray.h"
#include "wyJSONObject.h"
#include "MainScene.h"
#include "ControlButton.h"
#include "CoinChangeListener.h"
#include "CEncryptValue.h"

//=======================game strings==================================
static const char * EQUIP_WEAPON_NAME[16] = {
    "ump",
    "m870p",
    "aer14",
    "m249",
    "m4a1",
    "rpg",
    
    "mp5a5",
    "ak47",
    "hydra",
    "glb",
    "glr",
    "f1",
    
    "e3",
    "ds2",
    "torpedo",
    "blasting"
};
static const char * EQUIP_WEAPON_DESC[16] = {
    "basic weapon you can find, free to shoot.",
    "powerful weapon, but sometimes you may wonder if it is stuck.",
    "concentrates a stream of powerful light that penetrates flesh and blood.",
    "it seems never stop, better to prepare more ammo.",
    "feel the fury of cerberus, dead!",
    "penta-kill!",
    
    "advanced easy weapon, free to shoot.",
    "selective-fire, gas-operated assault rifle, most popular in the world.",
    "named after the monster. and it’s as lethal as that one.",
    "secret military prototype, high-tech style.",
    "enhanced version laser, a bit sacrifice of shooting rate.",
    "it’s fast, through it’s not that good looking.",
    
    "ultimate auto-fire weapon, fast and deadly.",
    "one shot, three kills.",
    "you got this, you want nothing more.",
    "lighter rocket launcher, area of effect."
};
static const char * EQUIP_WEAPON_DESC_SUB[16] = {
    "*unlimited ammo*",
    "*knock back*",
    "*pierce*",
    "*continuous fire*",
    "*multi shot*",
    "*area damage*",
    "*unlimited ammo*",
    "*knock back*",
    "*knock back*",
    "*pierce*",
    "*pierce*",
    "*continuous fire*",
    "*continuous fire*",
    "*multi shot*",
    "*multi shot*",
    "*area damage*"
};

static const char * SKILLS_NAME[10] = {"missile" ,"robot", "shield", "greed", "armor", "stamina", "evasion", "frenzy", "critical", "stun"};
static const char * SKILLS_DESC[10] = {
    "emits several missiles. upon impact they damage massive enemies with enormous explosions.",
    "summons the war machine to aid. the fatal robot lasts 10s, and attacks zombies automatically.",
    "concentrating high energy around the hero grants him a shield that prevents from external damages.",
    "the hero becomes more eager in finding money. gets extra coin rewards.",
    "thickens body armor to protect the hero from damages.",
    "gets extra hp for each upgrade.",
    "trains body flexibility, getting a certain chance to evade attacks.",
    "awakes the savagery of the hero, increasing shooting speed.",
    "using high-explosive bullets, each shoot has a chance to deal critical attack.",
    "gives a chance that attacks will stun the opponents for a while.",
};

static const char * ACHV_DESC[60] = {
    "1st killing",
    "kill %s zombies",
    "kill %s zombies",
    "kill %s zombies",
    "kill %s zombies",
    
    "kill %s zombies in one run",
    "kill %s zombies in one run",
    "kill %s zombies in one run",
    "kill %s zombies in one run",
    "kill %s zombies in one run",
    
    "kill %s bosses",
    "kill %s bosses",
    "kill %s bosses",
    "kill %s bosses",
    "kill %s bosses",
    
    "kill %s bosses in one run",
    "kill %s bosses in one run",
    "kill %s bosses in one run",
    "kill %s bosses in one run",
    "kill %s bosses in one run",
    
    "kill %s zombies with missle",
    "activate robot %s times",
    "use shield to prevent damage %s times",
    
    "a gun level %s",
    "a gun level %s",
    "a gun level %s",
    "a gun level %s",
    
    "an active skill level %s",
    "an active skill level %s",
    "an active skill level %s",
    "an active skill level %s",
    
    "a passive skill level %s",
    "a passive skill level %s",
    "a passive skill level %s",
    "a passive skill level %s",
    
    "all upgrades level %s",
    
    "run %sm with no shoots",
    "use 'save me'",
    "use 'save me' %s times",
    "%sm collecting no coins",
    
    "run %sm",
    "run %sm",
    "run %sm",
    "run %sm",
    "run %sm",
    "reach %sm in one run",
    "reach %sm in one run",
    "reach %sm in one run",
    "reach %sm in one run",
    "reach %sm in one run",
    
    "collect %s coin",
    "collect %s coin",
    "collect %s coin",
    "collect %s coin",
    "collect %s coin",
    "collect %s coin in one run",
    "collect %s coin in one run",
    "collect %s coin in one run",
    "collect %s coin in one run",
    "collect %s coin in one run",
};

static const int STR_INTRO_1_1_1 = 0;
static const int STR_INTRO_1_1_2 = 1;
static const int STR_INTRO_1_2 = 2;

static const int STR_INTRO_2_1 = 3;
static const int STR_INTRO_2_2 = 4;
static const int STR_INTRO_2_3 = 5;

static const int STR_INTRO_3_1 = 6;
static const int STR_INTRO_3_2 = 7;

static const int STR_INTRO_4_1 = 8;
static const int STR_INTRO_4_2 = 9;

static const int STR_INTRO_SWITCH_GUN = 10;
static const int STR_INTRO_RIFLE = 11;

static const int STR_INTRO_FINISH = 12;
static const int STR_INTRO_FAILED = 13;

static const int STR_INTRO_MOTO = 14;
static const int STR_INTRO_MISSLE = 15;
static const int STR_INTRO_JETPACK = 16;

static const int STR_INTRO_UPGRADE_WEAPON = 17;
static const int STR_INTRO_GOTONEXT = 18;
static const int STR_INTRO_UPGRADE_SKILL = 19;

static const char * INTRO_STR[] = {
    //intro_1
    "press left-corner button to jump,",
    "hold the button will jump higher.",
    "good!",
    //intro_2
    "you have a bigger challenge",
    "in the air, you can press jump again to jump a second time.",
    "well done!",
    //intro_3
    "press right-corner button to shoot.",
    "great!",
    //intro_4
    "call missle to deal with bunch of zombies.",
    "wow!",
    //intro swtich gun
    "when you have a new gun, you can switch by pressing this button.",
    "kill the fat zombie with rifle!",
    //女人说话
    "ok, you've learnt how to fight. let's go & shoot zombies down!",
    //教学关中失败
    "please try again.",
    
    //摩托车介绍
    "ride motocycle to crash zombies!",
    //导弹群介绍
    "call missile group!",
    //飞行器介绍
    "fly with jetpack!",

    //升级枪介绍
    "buy new weapons and upgrade their power.",
    //去下个关卡介绍
    "go to next level.",
    //升级技能介绍
    "upgrade your skills. you need to upgrade basic skills to unlock others.",
};

static const int STR_TUTORIAL_TITLE = 0;
static const int STR_LEVEL_TITLE = 1;
static const int STR_MISSION_FAILED_TITLE = 2;
static const int STR_MISSION_FAILED_DESC = 3;
static const int STR_HIGHESTSCORE = 4;

static const int STR_BOSS1_TALK = 5;

static const int STR_ULK_ENDLESS_EASY = 6;
static const int STR_ULK_ENDLESS_HARD = 7;
static const int STR_ULK_ENDLESS_CRAZY = 8;

static const int STR_ULK_SMODE_HARD = 9;
static const int STR_ULK_SMODE_CRAZY = 10;

static const int STR_SHOP_COIN_LACK = 11;
static const int STR_SHOP_CASH_LACK = 12;

static const int STR_DIF_ULK = 13;
static const int STR_STORYMODE_COMPLETE = 14;

static const int STR_SHOP_SOLDOUT = 15;

static const int STR_LOSS_PROGRESS = 16;

static const int STR_ULK_2ND_STADGE = 17;
static const int STR_ULK_3RD_STADGE = 18;

static const char * OTHER_STR[67] = {
    "tutorial",
    "level %d-%d",
    "mission failed",
    "you can't just stop here!upgrade weapons and skills and try it again!",
    "highest score",
    "ah! fresh meat!",
    
    //无尽关无法解锁弹框提示
    "you have to complete stage 2 of the corresponding story mode first.",
    "you have to complete stage 2 of the corresponding story mode first.",
    "you have to complete stage 2 of the corresponding story mode first.",
    
    //storymode后两个难度无法解锁提示
    "you have to complete easy mode first.",
    "you have to complete hard mode first.",
    
    //购买时金币或钞票不足提示
    "not enough coins. enter the shop to buy more?",
    "not enough cash. enter the shop to buy more?",
    
    //回到首页面，提示新难度解锁
    "you have unlocked new contents. go try them.",
    //crazy3-10结束，回到首页面，提示所有难度都完成。
    "congratulations, you have completed story mode! get more challenges in endless mode!",
    
    //商店促销项卖完后，点击提示
    "sorry, sold out.",
    
    //过checkpoint后，死亡界面，点击upgrade提示
    "upgrade will lose your progress in this level. are you sure to continue?",
    
    "you have to complete stage 1.",
    "you have to complete stage 2.",
};

static const char * TIPS_STR[] = {
    "buying and upgrading weapons and skills will get you empowered.",
    "on really dangerous positions, the right skill may save your life.",
    "try to choose your own best upgrading route.",
    "bosses are tough, but some weapons and skills may be more effective.",
	"want more challenges? try endless mode.",
	"boosts are randomly dropped. you may get a better one next time.",
	"enter the shop, if you feel lack of money in the game."
    
};

//=======================strings===============================
static const int se_absorb = 0;
static const int se_achievement = 1;
static const int se_bomb = 2;
static const int se_boost = 3;
static const int se_boss1_attack1 = 4;
static const int se_boss1_attack2 = 5;
static const int se_boss1_dead = 6;
static const int se_boss1_start = 7;
static const int se_boss2_attack1 = 8;
static const int se_boss2_attack2 = 9;
static const int se_boss2_dead = 10;
static const int se_boss2_start = 11;
static const int se_boss3_attack1 = 12;
static const int se_boss3_attack2 = 13;
static const int se_boss3_dead = 14;
static const int se_boss3_start = 15;
static const int se_button = 16;
static const int se_call_liudan = 17;
static const int se_changegun = 18;
static const int se_coin = 19;
static const int se_devourer_dead = 20;
static const int se_doctor_dead = 21;
static const int se_fatzombie_dead = 22;
static const int se_femalezombie_dead = 23;
static const int se_fly_dart = 24;
static const int se_ghostbaby_dead = 25;
static const int se_gun_gatling = 26;
static const int se_gun_laser = 27;
static const int se_gun_normal = 28;
static const int se_gun_rifle = 29;
static const int se_gun_rpg = 30;
static const int se_gun_shot = 31;
static const int se_heal = 32;
static const int se_hit = 33;
static const int se_jetpack_jump = 34;
static const int se_jetpack = 35;
static const int se_jump = 36;
static const int se_malezombie_dead = 37;
static const int se_menu = 38;
static const int se_motocycle = 39;
static const int se_musclezombie_dead = 40;
static const int se_player_dead = 41;
static const int se_player_hurt = 42;
static const int se_robert_speak = 43;
static const int se_shield_active = 44;
static const int se_spittlezombie_dead = 45;
static const int se_upgrade = 46;
static const int se_wolf_dead = 47;

static const int se_gun_ak47 = 48;
static const int se_gun_blasting = 49;
static const int se_gun_ds2 = 50;
static const int se_gun_e3 = 51;
static const int se_gun_f1 = 52;
static const int se_gun_glb = 53;
static const int se_gun_glr = 54;
static const int se_gun_hydra = 55;
static const int se_gun_mp5a5 = 56;
static const int se_gun_torpedo = 57;

static const char * SE_FILE_NAME[] = {
    "sound/se_absorb.ogg",
    "sound/se_achievement.ogg",
    "sound/se_bomb.ogg",
    "sound/se_boost.ogg",
    "sound/se_boss1_attack1.ogg",
    "sound/se_boss1_attack2.ogg",
    "sound/se_boss1_dead.ogg",
    "sound/se_boss1_start.ogg",
    "sound/se_boss2_attack1.ogg",
    "sound/se_boss2_attack2.ogg",
    "sound/se_boss2_dead.ogg",
    "sound/se_boss2_start.ogg",
    "sound/se_boss3_attack1.ogg",
    "sound/se_boss3_attack2.ogg",
    "sound/se_boss3_dead.ogg",
    "sound/se_boss3_start.ogg",
    "sound/se_button.ogg",
    "sound/se_call_liudan.ogg",
    "sound/se_changegun.ogg",
    "sound/se_coin.ogg",
    "sound/se_devourer_dead.ogg",
    "sound/se_doctor_dead.ogg",
    "sound/se_fatzombie_dead.ogg",
    "sound/se_femalezombie_dead.ogg",
    "sound/se_fly_dart.ogg",
    "sound/se_ghostbaby_dead.ogg",
    "sound/se_gun_gatling.ogg",
    "sound/se_gun_laser.ogg",
    "sound/se_gun_normal.ogg",
    "sound/se_gun_rifle.ogg",
    "sound/se_gun_rpg.ogg",
    "sound/se_gun_shot.ogg",
    "sound/se_heal.ogg",
    "sound/se_hit.ogg",
    "sound/se_jetpack_jump.ogg",
    "sound/se_jetpack.ogg",
    "sound/se_jump.ogg",
    "sound/se_malezombie_dead.ogg",
    "sound/se_menu.ogg",
    "sound/se_motocycle.ogg",
    "sound/se_musclezombie_dead.ogg",
    "sound/se_player_dead.ogg",
    "sound/se_player_hurt.ogg",
    "sound/se_robert_speak.ogg",
    "sound/se_shield_active.ogg",
    "sound/se_spittlezombie_dead.ogg",
    "sound/se_upgrade.ogg",
    "sound/se_wolf_dead.ogg",
    
    "sound/se_gun_ak47.ogg",
    "sound/se_gun_blasting.ogg",
    "sound/se_gun_ds2.ogg",
    "sound/se_gun_e3.ogg",
    "sound/se_gun_f1.ogg",
    "sound/se_gun_glb.ogg",
    "sound/se_gun_glr.ogg",
    "sound/se_gun_hydra.ogg",
    "sound/se_gun_mp5a5.ogg",
    "sound/se_gun_torpedo.ogg",
};

static const int MENU_SHOP = 0;
static const int MENU_FREE_STUFF = 1;
//static const int MENU_CHALLENGE = 2;
static const int MENU_MISSIONS = 2;
static const int MENU_STATISTICS = 3;
static const int MENU_LEADERBOARDS = 4;
//static const int MENU_CREDITS = 6;
static const int MENU_SETTINGS = 5;
//static const int MENU_MOREGAMES = 6;
static const int MENU_TUTORIAL = 6;
static const int MENU_DAILY_REWARD = 7;

//================================================================================================================
//achviements
static const int ACHV_KILL_1 = 0;
static const int ACHV_KILL_1000 = 1;
static const int ACHV_KILL_5000 = 2;
static const int ACHV_KILL_20000 = 3;
static const int ACHV_KILL_50000 = 4;

static const int ACHV_KILL_50_ONE = 5;
static const int ACHV_KILL_100_ONE = 6;
static const int ACHV_KILL_200_ONE = 7;
static const int ACHV_KILL_400_ONE = 8;
static const int ACHV_KILL_800_ONE = 9;

static const int ACHV_BOSS_3 = 10;
static const int ACHV_BOSS_10 = 11;
static const int ACHV_BOSS_30 = 12;
static const int ACHV_BOSS_100 = 13;
static const int ACHV_BOSS_300 = 14;

static const int ACHV_BOSS_2_ONE = 15;
static const int ACHV_BOSS_4_ONE = 16;
static const int ACHV_BOSS_8_ONE = 17;
static const int ACHV_BOSS_16_ONE = 18;
static const int ACHV_BOSS_24_ONE = 19;

static const int ACHV_MISSLE_100 = 20;
static const int ACHV_ROBOT_20 = 21;
static const int ACHV_SHIELD_10 = 22;

static const int ACHV_GUN_3 = 23;
static const int ACHV_GUN_5 = 24;
static const int ACHV_GUN_8 = 25;
static const int ACHV_GUN_10 = 26;

static const int ACHV_ASKILL_5 = 27;
static const int ACHV_ASKILL_12 = 28;
static const int ACHV_ASKILL_20 = 29;
static const int ACHV_ASKILL_30 = 30;

static const int ACHV_PSKILL_5 = 31;
static const int ACHV_PSKILL_12 = 32;
static const int ACHV_PSKILL_20 = 33;
static const int ACHV_PSKILL_30 = 34;

static const int ACHV_ALL_SKILL_30 = 35;

static const int ACHV_RUN_300_NOSHOOT = 36;
static const int ACHV_SAVEME_1 = 37;
static const int ACHV_SAVEME_9 = 38;
static const int ACHV_RUN_100_NOCOIN = 39;

static const int ACHV_RUN_1000 = 40;
static const int ACHV_RUN_5000 = 41;
static const int ACHV_RUN_20000 = 42;
static const int ACHV_RUN_50000 = 43;
static const int ACHV_RUN_100000 = 44;

static const int ACHV_RUN_400_ONE = 45;
static const int ACHV_RUN_800_ONE = 46;
static const int ACHV_RUN_1600_ONE = 47;
static const int ACHV_RUN_3200_ONE = 48;
static const int ACHV_RUN_4800_ONE = 49;

static const int ACHV_COIN_1000 = 50;
static const int ACHV_COIN_5000 = 51;
static const int ACHV_COIN_20000 = 52;
static const int ACHV_COIN_50000 = 53;
static const int ACHV_COIN_100000 = 54;

static const int ACHV_COIN_400_ONE = 55;
static const int ACHV_COIN_800_ONE = 56;
static const int ACHV_COIN_1600_ONE = 57;
static const int ACHV_COIN_3200_ONE = 58;
static const int ACHV_COIN_4800_ONE = 59;

static const int ACHV_INACTIVE = 0;
static const int ACHV_ACTIVE = 1;
static const int ACHV_FINISHED = 2;

static const int SKILL_ROBOT_PRV = 4;
static const int SKILL_SHIELD_PRV = 8;
static const int SKILL_ARMOR_PRV = 4;
static const int SKILL_STAMINA_PRV = 2;
static const int SKILL_EVASION_PRV = 6;
static const int SKILL_CRITICAL_PRV = 6;
static const int SKILL_STUN_PRV = 8;

static const int HERO_DEAD_TYPE_FALL = 0;
static const int HERO_DEAD_TYPE_HURT = 1;

//季节在轮次中出现的次序
static int POSSIBLE_COMBO[6] = {
    123, 132, 213, 231, 312, 321};

static const float SPX_SCALE_X = 0.8f;

static const int WP_ATK_BASE = 0;
static const int WP_SPEED = 1;
static const int WP_ATK_INC = 2;
static const int WP_ULK_GOLD = 3;
static const int WP_ULK_CASH = 4;
static const int WP_GOLD_MULTI = 5;
static const int WP_AMMO_COST = 6;
static const int WP_AMMO_AMOUNT = 7;

static const int ARR_LEN_LEVEL_NUM = 50;//8*3=24
static const int ARR_LEN_ACHV_NUM = 60;
static const int ARR_LEN_SKILL_NUM = 10;
static const int ARR_LEN_WEAPON_NUM = 50;

static const int TYPE_GROUND = 0;
static const int TYPE_WORLD_BOUND = 1;
static const int TYPE_ENEMY = 2;
static const int TYPE_BULLET = 3;
static const int TYPE_MAO = 4;
static const int TYPE_ENEMY_BULLET = 5;
static const int TYPE_TRAP = 6;
static const int TYPE_BARRICADE = 7;
static const int TYPE_BONUS = 8;
static const int TYPE_BOX = 9;
static const int TYPE_INTRO = 10;
static const int TYPE_ENDPOINT = 11;
static const int TYPE_LIGHTING = 12;
static const int TYPE_SHOCK_WAVE = 13;
static const int TYPE_BIG_FIRE = 14;
static const int TYPE_WATER_BALL = 15;
static const int TYPE_DABIAO = 16;
static const int TYPE_CHONGZHUANG = 17;
static const int TYPE_CHECKPOINT = 18;
static const int TYPE_NPC = 19;
static const int TYPE_BOMB = 20;
static const int TYPE_CANNON = 21;
static const int TYPE_BLOOD = 22;
static const int TYPE_MISSLE = 23;
static const int TYPE_LIUDAN = 24;
static const int TYPE_BODY_SENSOR = 25;
static const int TYPE_BODY_PIECE = 26;
static const int TYPE_MAP_DECO = 27;
static const int TYPE_HERO_HURT = 28;
static const int TYPE_GROUND_SENSOR = 29;

static const int CAT_BITS[30] = {
    2,//TYPE_GROUND
    2,//TYPE_WORLD_BOUND
    4,//TYPE_ENEMY
    2048,//TYPE_BULLET
    8,//TYPE_MAO
    16,//TYPE_ENEMY_BULLET
    32,//TYPE_TRAP
    64,//TYPE_BARRICADE
    32,//TYPE_BONUS
    1,//TYPE_BOX===============================no
    32,//TYPE_INTRO
    32,//TYPE_ENDPOINT
    32,//TYPE_LIGHTING
    32,//TYPE_SHOCK_WAVE
    1,//TYPE_BIG_FIRE===============================no
    1,//TYPE_WATER_BALL===============================no
    1,//TYPE_DABIAO===============================no
    32,//TYPE_CHONGZHUANG
    32,//TYPE_CHECKPOINT
    1,//TYPE_NPC===============================no
    128,//TYPE_BOMB
    1,//TYPE_CANNON===============================no
    256,//TYPE_BLOOD
    512,//TYPE_MISSLE
    512,//TYPE_LIUDAN
    256,//TYPE_BODY_SENSOR
    256,//TYPE_BODY_PIECE
    1,//TYPE_MAP_DECO
    1024,
    256,
};

static const int MASK_BITS[30] = {
    0xFFFF,//TYPE_GROUND
    0xFFFF,//TYPE_WORLD_BOUND
    0xFFFF,//TYPE_ENEMY
    CAT_BITS[TYPE_ENEMY]+CAT_BITS[TYPE_BARRICADE],//TYPE_BULLET
    0xFFFF^CAT_BITS[TYPE_BULLET]^CAT_BITS[TYPE_MISSLE]^CAT_BITS[TYPE_LIUDAN]^CAT_BITS[TYPE_BODY_SENSOR]^CAT_BITS[TYPE_BODY_PIECE]^CAT_BITS[TYPE_MAP_DECO]^CAT_BITS[TYPE_ENEMY_BULLET],//TYPE_MAO
    CAT_BITS[TYPE_HERO_HURT],//TYPE_ENEMY_BULLET
    CAT_BITS[TYPE_HERO_HURT],//TYPE_TRAP
    CAT_BITS[TYPE_MAO]+CAT_BITS[TYPE_BULLET]+CAT_BITS[TYPE_HERO_HURT],//TYPE_BARRICADE
    CAT_BITS[TYPE_MAO],//TYPE_BONUS
    1,//TYPE_BOX
    CAT_BITS[TYPE_MAO],//TYPE_INTRO
    CAT_BITS[TYPE_MAO],//TYPE_ENDPOINT
    CAT_BITS[TYPE_HERO_HURT],//TYPE_LIGHTING
    CAT_BITS[TYPE_HERO_HURT],//TYPE_SHOCK_WAVE
    1,//TYPE_BIG_FIRE
    1,//TYPE_WATER_BALL
    1,//TYPE_DABIAO
    CAT_BITS[TYPE_HERO_HURT],//TYPE_CHONGZHUANG
    CAT_BITS[TYPE_MAO],//TYPE_CHECKPOINT
    1,//TYPE_NPC
    CAT_BITS[TYPE_MAO]+CAT_BITS[TYPE_GROUND]+CAT_BITS[TYPE_HERO_HURT],//TYPE_BOMB
    1,//TYPE_CANNON
    CAT_BITS[TYPE_GROUND],//TYPE_BLOOD
    CAT_BITS[TYPE_ENEMY]+CAT_BITS[TYPE_BARRICADE]+CAT_BITS[TYPE_GROUND],//TYPE_MISSLE
    CAT_BITS[TYPE_ENEMY]+CAT_BITS[TYPE_BARRICADE],//TYPE_LIUDAN
    CAT_BITS[TYPE_GROUND],//TYPE_BODY_SENSOR
    CAT_BITS[TYPE_GROUND],//TYPE_BODY_PIECE
    0,//TYPE_MAP_DECO
    0xFFFF^CAT_BITS[TYPE_BULLET]^CAT_BITS[TYPE_MISSLE]^CAT_BITS[TYPE_LIUDAN]^CAT_BITS[TYPE_BODY_SENSOR]^CAT_BITS[TYPE_BODY_PIECE]^CAT_BITS[TYPE_MAP_DECO]^CAT_BITS[TYPE_MAO]^CAT_BITS[TYPE_GROUND]^CAT_BITS[TYPE_BONUS]^CAT_BITS[TYPE_INTRO]^CAT_BITS[TYPE_ENDPOINT]^CAT_BITS[TYPE_CHECKPOINT],
    CAT_BITS[TYPE_GROUND],//TYPE_GROUND_SENSOR
};

//static const int MASK_BITS[28] = {
//    CAT_BITS[TYPE_ENEMY]+CAT_BITS[TYPE_MAO]+CAT_BITS[TYPE_BOMB]+CAT_BITS[TYPE_BLOOD]+CAT_BITS[TYPE_MISSLE]+CAT_BITS[TYPE_LIUDAN]+CAT_BITS[TYPE_BODY_SENSOR]+CAT_BITS[TYPE_BODY_PIECE],//TYPE_GROUND
//    CAT_BITS[TYPE_ENEMY]+CAT_BITS[TYPE_MAO]+CAT_BITS[TYPE_BOMB]+CAT_BITS[TYPE_BLOOD]+CAT_BITS[TYPE_MISSLE]+CAT_BITS[TYPE_LIUDAN]+CAT_BITS[TYPE_BODY_SENSOR]+CAT_BITS[TYPE_BODY_PIECE],//TYPE_WORLD_BOUND
//    CAT_BITS[TYPE_GROUND]+CAT_BITS[TYPE_BULLET]+CAT_BITS[TYPE_MAO]+CAT_BITS[TYPE_MISSLE]+CAT_BITS[TYPE_LIUDAN],//TYPE_ENEMY
//    CAT_BITS[TYPE_ENEMY]+CAT_BITS[TYPE_BARRICADE],//TYPE_BULLET
//    0xFFFF^CAT_BITS[TYPE_BULLET]^CAT_BITS[TYPE_MISSLE]^CAT_BITS[TYPE_LIUDAN]^CAT_BITS[TYPE_BODY_SENSOR]^CAT_BITS[TYPE_BODY_PIECE]^CAT_BITS[TYPE_MAP_DECO],//TYPE_MAO
//    CAT_BITS[TYPE_MAO],//TYPE_ENEMY_BULLET
//    CAT_BITS[TYPE_MAO],//TYPE_TRAP
//    CAT_BITS[TYPE_MAO]+CAT_BITS[TYPE_BULLET],//TYPE_BARRICADE
//    CAT_BITS[TYPE_MAO],//TYPE_BONUS
//    1,//TYPE_BOX
//    CAT_BITS[TYPE_MAO],//TYPE_INTRO
//    CAT_BITS[TYPE_MAO],//TYPE_ENDPOINT
//    CAT_BITS[TYPE_MAO],//TYPE_LIGHTING
//    CAT_BITS[TYPE_MAO],//TYPE_SHOCK_WAVE
//    1,//TYPE_BIG_FIRE
//    1,//TYPE_WATER_BALL
//    1,//TYPE_DABIAO
//    CAT_BITS[TYPE_MAO],//TYPE_CHONGZHUANG
//    CAT_BITS[TYPE_MAO],//TYPE_CHECKPOINT
//    1,//TYPE_NPC
//    CAT_BITS[TYPE_MAO]+CAT_BITS[TYPE_GROUND],//TYPE_BOMB
//    1,//TYPE_CANNON
//    CAT_BITS[TYPE_GROUND],//TYPE_BLOOD
//    CAT_BITS[TYPE_ENEMY]+CAT_BITS[TYPE_BARRICADE],//TYPE_MISSLE
//    CAT_BITS[TYPE_ENEMY]+CAT_BITS[TYPE_BARRICADE]+CAT_BITS[TYPE_GROUND],//TYPE_LIUDAN
//    CAT_BITS[TYPE_GROUND],//TYPE_BODY_SENSOR
//    CAT_BITS[TYPE_GROUND],//TYPE_BODY_PIECE
//    0,//TYPE_MAP_DECO
//};

static const int GROUND_INDEX[30] = {
    -1,//TYPE_GROUND
    -1,//TYPE_WORLD_BOUND
    -2,//TYPE_ENEMY
    -3,//TYPE_BULLET
    -4,//TYPE_MAO
    -2,//TYPE_ENEMY_BULLET
    -2,//TYPE_TRAP
    -2,//TYPE_BARRICADE
    -2,//TYPE_BONUS
    1,//TYPE_BOX
    -2,//TYPE_INTRO
    -1,//TYPE_ENDPOINT
    -2,//TYPE_LIGHTING
    -2,//TYPE_SHOCK_WAVE
    1,//TYPE_BIG_FIRE
    1,//TYPE_WATER_BALL
    1,//TYPE_DABIAO
    -2,//TYPE_CHONGZHUANG
    -1,//TYPE_CHECKPOINT
    1,//TYPE_NPC
    -2,//TYPE_BOMB
    1,//TYPE_CANNON
    -2,//TYPE_BLOOD
    -3,//TYPE_MISSLE
    -3,//TYPE_LIUDAN
    -3,//TYPE_BODY_SENSOR
    -3,//TYPE_BODY_PIECE
    -3,//TYPE_MAP_DECO
    -4,
    -3,//TYPE_GROUND_SENSOR
};

static const int SPX_RABBIT_SWORD_MAN_0 = 0;
static const int SPX_RABBIT_SWORD_MAN_1 = 1;

static const int SPX_RABBIT_ARCHER_0 = 0;
static const int SPX_RABBIT_ARCHER_1 = 1;

static const int SPX_SKELETON_SOLDIER = 0;
static const int SPX_SKELETON_SPECULATOR = 1;
static const int SPX_DELUYI = 2;

static const int SPX_SKELETON_SAMURAI = 0;
static const int SPX_SKELETON_CHIEF = 1;

static const int SPX_DOG_MAGE_0 = 0;
static const int SPX_DOG_MAGE_1 = 1;

static const int COIN = 0;
static const int BIGCOIN = 1;
static const int BIGBIGCOIN = 2;

static const int BONUS_YING = 0;
static const int BONUS_BIAO = 1;
static const int BONUS_XIEZI = 2;
static const int BONUS_XUE = 3;

static const int SPX_HERO_BOUNDING_BOX = 0;

static const int SPX_HERO_R_LEG_RUN = 1;
static const int SPX_HERO_L_LEG_RUN = 2;
static const int SPX_HERO_R_LEG_IDLE = 3;
static const int SPX_HERO_L_LEG_IDLE = 4;

static const int SPX_HERO_R_LEG_JUMP1 = 5;
static const int SPX_HERO_L_LEG_JUMP1 = 6;
static const int SPX_HERO_R_LEG_JUMP2 = 7;
static const int SPX_HERO_L_LEG_JUMP2 = 8;
static const int SPX_HERO_R_LEG_JUMP3 = 9;
static const int SPX_HERO_L_LEG_JUMP3 = 10;
static const int SPX_HERO_R_LEG_JUMP4 = 11;
static const int SPX_HERO_L_LEG_JUMP4 = 12;

static const int SPX_HERO_R_LEG_LAND = 13;
static const int SPX_HERO_L_LEG_LAND = 14;

static const int SPX_HERO_R_LEG_DB_JUMP = 15;
static const int SPX_HERO_L_LEG_DB_JUMP = 16;

static const int SPX_HERO_R_LEG_FALL = 17;
static const int SPX_HERO_L_LEG_FALL = 18;

static const int SPX_MOTO_BODY = 19;
static const int SPX_MOTO_B_WHEEL = 20;
static const int SPX_MOTO_F_WHEEL = 21;
static const int SPX_MOTO_R_LEG = 22;

static const int SPX_NORMAL_GUN_IDLE = 23;
static const int SPX_NORMAL_GUN_FIRE = 24;

static const int SPX_RIFLE_GUN_IDLE = 25;
static const int SPX_RIFLE_GUN_FIRE = 26;

static const int SPX_LASER_GUN_IDLE = 27;
static const int SPX_LASER_GUN_FIRE = 28;
static const int SPX_LASER_GUN_LASER = 29;

static const int SPX_MACHINE_GUN_IDLE = 30;
static const int SPX_MACHINE_GUN_FIRE = 31;

static const int SPX_SHOT_GUN_IDLE = 32;
static const int SPX_SHOT_GUN_FIRE = 33;

static const int SPX_RPG_IDLE = 34;
static const int SPX_RPG_FIRE = 35;
static const int SPX_RPG_ROCKET = 36;

static const int SPX_SHIELD_0 = 37;
static const int SPX_SHIELD_1 = 38;
static const int SPX_SHIELD_2 = 39;

static const int SPX_HERO_R_LEG_HURT = 40;
static const int SPX_HERO_L_LEG_HURT = 41;

static const int SPX_HERO_NGUN_SI = 42;
static const int SPX_HERO_RIFLE_SI = 43;
static const int SPX_HERO_LASER_SI = 44;
static const int SPX_HERO_MGUN_SI = 45;
static const int SPX_HERO_SHOTGUN_SI = 46;
static const int SPX_HERO_RPG_SI = 47;

static const int SPX_HERO_R_LEG_SI = 48;
static const int SPX_HERO_L_LEG_SI = 49;

static const int SPX_HERO_NGUN_FH = 50;
static const int SPX_HERO_RIFLE_FH = 51;
static const int SPX_HERO_LASER_FH = 52;
static const int SPX_HERO_MGUN_FH = 53;
static const int SPX_HERO_SHOTGUN_FH = 54;
static const int SPX_HERO_RPG_FH = 55;

static const int SPX_HERO_R_LEG_FH = 56;
static const int SPX_HERO_L_LEG_FH = 57;

static const int SPX_LASER_LIGHT = 58;

//hero1动画编号：
static const int SPX_MP5A5_IDLE = 10;
static const int SPX_MP5A5_FIRE = 11;
static const int SPX_MP5A5_SI = 12;
static const int SPX_MP5A5_FH = 13;

static const int SPX_AK47_IDLE = 14;
static const int SPX_AK47_FIRE = 15;
static const int SPX_AK47_SI = 16;
static const int SPX_AK47_FH = 17;

static const int SPX_HYDRA_IDLE = 18;
static const int SPX_HYDRA_FIRE = 19;
static const int SPX_HYDRA_SI = 20;
static const int SPX_HYDRA_FH = 21;

static const int SPX_GLB_IDLE = 22;
static const int SPX_GLB_FIRE = 23;
static const int SPX_GLB_SI = 24;
static const int SPX_GLB_FH = 25;
static const int SPX_GLB_LIGHT1 = 26;
static const int SPX_GLB_LIGHT2 = 27;

static const int SPX_GLR_IDLE = 28;
static const int SPX_GLR_FIRE = 29;
static const int SPX_GLR_SI = 30;
static const int SPX_GLR_FH = 31;
static const int SPX_GLR_LIGHT1 = 32;
static const int SPX_GLR_LIGHT2 = 33;

static const int SPX_AER14_LIGHT1 = 34;
static const int SPX_AER14_LIGHT2 = 35;

static const int SPX_F1_IDLE = 36;
static const int SPX_F1_FIRE = 37;
static const int SPX_F1_SI = 38;
static const int SPX_F1_FH = 39;
static const int SPX_F1_LIGHT1 = 40;

static const int SPX_E3_IDLE = 41;
static const int SPX_E3_FIRE = 42;
static const int SPX_E3_SI = 43;
static const int SPX_E3_FH = 44;

static const int SPX_DS2_IDLE = 45;
static const int SPX_DS2_FIRE = 46;
static const int SPX_DS2_SI = 47;
static const int SPX_DS2_FH = 48;

static const int SPX_TORPEDO_IDLE = 49;
static const int SPX_TORPEDO_FIRE = 50;
static const int SPX_TORPEDO_SI = 51;
static const int SPX_TORPEDO_FH = 52;

static const int SPX_BLASTING_IDLE = 53;
static const int SPX_BLASTING_FIRE = 54;
static const int SPX_BLASTING_SI = 55;
static const int SPX_BLASTING_FH = 56;

static const int SPX_BLASTING_LIGHT = 57;

//effects spx序号
static const int SPX_SHITOU = 0;
static const int SPX_XUANKONGBIAO = 1;
static const int SPX_QIYOUTONG = 2;
static const int SPX_MUZHUANG = 3;
static const int SPX_DILEI = 4;
static const int SPX_DILEI_BAOZHA = 5;
static const int SPX_WUTI_BAOZHA = 6;
static const int SPX_DICI = 7;
static const int SPX_HUOJIAN = 8;
static const int SPX_JIQUN_HUOJIAN_BAOZHA = 9;
static const int SPX_HUOJIAN_BAOZHA = 10;
static const int SPX_XIAOGUAI_YANWU = 11;
static const int SPX_PENXUE_1 = 12;
static const int SPX_PENXUE_2 = 13;
static const int SPX_DANYAOKU = 14;
static const int SPX_JIZHONG_WUTI = 15;

static const int SPX_LIANDAO = 16;
static const int SPX_SUOLIAN = 17;
static const int SPX_NVREN_DAIJI = 18;
static const int SPX_NVREN_FEIWEN = 19;

static const int SPX_FAINT = 20;

static const int SPX_DILEI_LIGHT = 21;
static const int SPX_MISSLE_1 = 22;

static const int SPX_HERO_SHIELD_0 = 23;
static const int SPX_HERO_SHIELD_1 = 24;
static const int SPX_HERO_SHIELD_2 = 25;

static const int SPX_DILEI_LIGHT_1 = 26;

static const int SPX_COIN_SMALL = 27;
static const int SPX_COIN_MIDDLE = 28;
static const int SPX_COIN_BIG = 29;

static const int SPX_DEAD_BLOOD = 30;
static const int SPX_BOSS_DEAD_BOMB = 31;


static const char * DB_HAS_PORFILE = "a";
static const char * DB_GOLD = "b";
static const char * DB_S01_GRADE = "c";
static const char * DB_S02_GRADE = "d";
static const char * DB_S03_GRADE = "e";
static const char * DB_S01_SCORE = "f";
static const char * DB_S02_SCORE = "g";
static const char * DB_S03_SCORE = "h";
static const char * DB_S01_TIMES = "i";
static const char * DB_S02_TIMES = "j";
static const char * DB_S03_TIMES = "k";
static const char * DB_PROGRESS = "l";
static const char * DB_HEAD = "m";
static const char * DB_WEAPON = "n";
static const char * DB_CLOTH = "o";
static const char * DB_FIRE_FURY = "p";
static const char * DB_ILLUSION = "q";
static const char * DB_SAND_SHIELD = "r";
static const char * DB_ACTIVE_SKILL = "s";
static const char * DB_XUEPING = "t";
static const char * DB_XIEZI = "u";
static const char * DB_DABIAO = "v";
static const char * DB_LAOYING = "w";
static const char * DB_ACTIVE_TOOL = "x";
static const char * DB_ACTIVE_WEAPON = "y";
static const char * DB_IS_SUPER_WEAPON_UNLOCKED = "z";
static const char * DB_IS_SFX_ON = "aa";
static const char * DB_IS_BGM_ON = "bb";

static const char * DB_ENCY_LOCK = "cc";

static const char * DB_IS_GUIDE_READ = "dd";
static const char * DB_IS_TRAINING_END = "ee";
static const char * DB_IS_FIRST_SCENE_NOTICED = "ff";
static const char * DB_IS_LEVEL_1_FORCE_NEXT = "gg";
static const char * DB_IS_UPGRADE_TRAINED = "hh";

static const char * DB_IS_YING_TRAINED = "ii";

static const char * DB_IS_ILLUSION_TRAINED = "jj";
static const char * DB_IS_ILLUSION_UNLOCKED = "kk";

static const char * DB_IS_TUOLUO_TRAINED = "ll";
static const char * DB_IS_TUOLUO_UNLOCKED = "mm";

static const char * DB_LAST_PLAY_TIME = "nn";
static const char * DB_TIME_SPEND = "oo";

static const char * DB_IS_TRAINING_LEVEL_NOTICED = "pp";

static const char * DB_IS_ADS_REMOVED = "qq";

static const char * DB_IS_TOOL_SWITCH_TRAINED = "rr";

static const char * DB_IS_BOSS1_TALKED = "ss";

static const char * DB_IS_TOOL_TRAINED = "tt";

static const char * DB_IS_XIEZI_TRAINED = "uu";

static const char * DB_IS_LAOYING_TRAINED = "vv";

static const char * DB_IAP_GOLD = "ww";

static const char * DB_IS_DABIAO_TRAINED = "xx";

static const char * DB_BOSS1_KILLED_TIMES = "aaa";
static const char * DB_BOSS2_KILLED_TIMES = "bbb";
static const char * DB_BOSS3_KILLED_TIMES = "ccc";
static const char * DB_FIRE_FURY_KILLS = "ddd";
static const char * DB_ILLUSION_CAST_TIMES = "eee";
static const char * DB_BOUNCING_TIMES = "fff";
static const char * DB_MAX_COMBO = "ggg";
static const char * DB_ACHV_REC = "hhh";
static const char * DB_KILLED_ENEMIES = "iii";
//新成就
static const char * DB_IS_NEW_ACHV = "jjj";
static const char * DB_KILLED_ENEMY = "kkk";
static const char * DB_TOTAL_KILLED_ENEMY = "lll";
static const char * DB_KILLED_BOSSES = "mmm";
static const char * DB_SAVEME_TIMES = "nnn";
static const char * DB_RUN_DISTANCE = "ooo";
static const char * DB_TOTAL_RUN_DISTANCE = "ppp";
static const char * DB_XUEPING_TIMES = "qqq";
static const char * DB_XIEZI_TIMES = "rrr";
static const char * DB_DABIAO_TIMES = "sss";
static const char * DB_LAOYING_TIMES = "ttt";

//static const char * DB_IS_HEROTU_UNLOCKED = "aaaa";
//static const char * DB_IS_HEROSHUI_UNLOCKED = "bbbb";
//static const char * DB_IS_HEROFENG_UNLOCKED = "cccc";
//static const char * DB_IS_HEROHUO_UNLOCKED = "dddd";
//static const char * DB_ACTIVE_ELEMENT = "eeee";
static const char * DB_SKILL_ARRAY = "ffff";
static const char * DB_ZUANSHI_NUM = "gggg";
//static const char * DB_IS_WEAPN_2_UNLOCKED = "hhhh";
//static const char * DB_IS_WEAPN_3_UNLOCKED = "iiii";
//static const char * DB_IS_WEAPN_4_UNLOCKED = "jjjj";
//static const char * DB_IS_ELEMENT_NAVED = "kkkk";
//static const char * DB_IS_ENDLESS_NAVED = "llll";

static const char * DB_DIST_REC_NORMAL = "mmmm";
static const char * DB_DIST_REC_HEROIC = "nnnn";
static const char * DB_DIST_REC_INFERNAL = "oooo";

static const char * DB_IS_FIRST_PURCHASE_REWARDED = "pppp";

//static const char * DB_IS_WEAPN_5_UNLOCKED = "qqqq";
//static const char * DB_IS_WEAPN_6_UNLOCKED = "rrrr";

static const char * DB_WEAPON_UNLOCK_ARRAY = "ssss";
static const char * DB_WEAPON_AMMO_ARRAY = "tttt";

static const char * DB_MISSION1_ID = "uuuu";
static const char * DB_MISSION2_ID = "vvvv";
static const char * DB_MISSION3_ID = "wwww";

static const char * DB_ACHV_PROGRESS = "xxxx";

static const char * DB_IS_GAME_COMPLETE = "yyyy";

static const char * DB_IS_SWITCHGUN_TRAINED = "zzzz";

static const char * DB_TOTAL_COLLECTED_COINS = "aaaaa";
static const char * DB_IS_BIGPACK_SOLDOUT = "bbbbb";

static const char * DB_WEAPON_NOTICE_ARRAY = "ccccc";
static const char * DB_SKILL_NOTICE_ARRAY = "ddddd";

static const char * DB_IS_DOUBLE_COIN = "eeeee";

//ENEMY s01
static const int ENEMY_TUZIJIANKE = 0;
static const int ENEMY_TUZIGONGJIANSHOU = 1;
static const int ENEMY_TUZILANGKE = 2;
static const int ENEMY_TUZIFASHI = 3;
static const int ENEMY_TUZIJIANSHENG = 4;
static const int ENEMY_TUZIGONGJIANSHOUFLY = 5;
static const int ENEMY_BOSS1 = 6;
static const int ENEMY_XIANJING1 = 7;
static const int ENEMY_ZHANGAI1 = 8;

//ENEMY s02
static const int ENEMY_KULOUSHIBING = 9;
static const int ENEMY_KULOUWUSHI = 10;
static const int ENEMY_KULOUTOULING = 11;
static const int ENEMY_KULOUTOUJIZHE = 12;
static const int ENEMY_KULOUSHIBINGFLY = 13;
static const int ENEMY_BOSS2 = 14;
static const int ENEMY_XIANJING2 = 15;
static const int ENEMY_ZHANGAI2 = 16;

//ENEMY s03
static const int ENEMY_HONGYIWUSHI = 17;
static const int ENEMY_HEIYIWUSHI = 18;
static const int ENEMY_ZHAOHUANSHI = 19;
static const int ENEMY_GOUTOUJUNSHI = 20;
static const int ENEMY_JIAMIANJUNSHI = 21;
static const int ENEMY_HONGYIWUSHIFLY = 22;
static const int ENEMY_BOSS3 = 23;
static const int ENEMY_XIANJING3 = 24;
static const int ENEMY_ZHANGAI3 = 25;
 
static const int ENEMY_XUANKONGBIAO = 26;

static const int ENEMY_TUZIGONGJIANSHOUGAODI = 100;
static const int ENEMY_JIANSHEN_JIANKE3 = 101;
static const int ENEMY_KULOU3 = 102;

static const int ENEMY_NEED_BLOOD = 103;



static const int GRADE_NONE = 0;
static const int GRADE_C = 1;
static const int GRADE_B = 2;
static const int GRADE_A = 3;
static const int GRADE_S = 4;



static float HERO_VEL_X = 12.0f;
static const float BULLET_VEL_X = 20.0f;
static const float JUMP_F_Y = 30.0f;//43
static const float JUMP_HOLD_F_Y = 54.0f;//0
static const float HERO_FLY_Y_UP = 15.0f;
static const float HERO_FLY_Y_DOWN = -15.0f;
static const float HERO_FLY_TIME = 10.0f;
static const float HERO_SAVE_ME_FLY_TIME = 5.0f;
static const float HERO_BOOST_TIME = 8.0f;
static const float HERO_YING_MAX_HEIGHT = 580;
static const float HERO_BOOTST_EYE = 80;
static const float HERO_BOOTST_EYE_BASEY_DIV = 1.5f;

static const int PARENT_WORLD_MAP = 0;
static const int PARENT_LEVEL_UP = 1;
static const int PARENT_MAIN_SCENE = 2;

static const int SCENE_MAIN = 0;
static const int SCENE_UPGRADE = 1;
static const int SCENE_MENU = 2;
static const int SCENE_WORLD_MAP = 3;

static const int WEAPON_UMP = 0;
static const int WEAPON_M870P = 1;
static const int WEAPON_AER14 = 2;
static const int WEAPON_M249 = 3;
static const int WEAPON_M4A1 = 4;
static const int WEAPON_RPG = 5;

static const int WEAPON_MP5A5 = 6;
static const int WEAPON_AK47 = 7;
static const int WEAPON_HYDRA = 8;
static const int WEAPON_GLB = 9;
static const int WEAPON_GLR = 10;
static const int WEAPON_F1 = 11;

static const int WEAPON_E3 = 12;
static const int WEAPON_DS2 = 13;
static const int WEAPON_TORPEDO = 14;
static const int WEAPON_BLASTING = 15;

static const int WEAPON_RPG_BOMB = 500;//火箭筒爆炸
static const int WEAPON_ROBOT_BULLET = 600;//机器人子弹
static const int WEAPON_BLASTING_BOMB = 700;//火箭筒爆炸


//shop
static const int SHOP_COIN_SMALL_ID = 0;
static const int SHOP_COIN_MIDDLE_ID = 1;
static const int SHOP_COIN_LARGE_ID = 2;
static const int SHOP_CASH_SMALL_ID = 3;
static const int SHOP_CASH_MIDDLE_ID = 4;
static const int SHOP_CASH_LARGE_ID = 5;

static const int SHOP_BIGPACK_ID = 6;

static const int SHOP_DBCOIN_ID = 7;

static const int SHOP_GOODS_NUM[] = {
    6000, 18500, 45000, 45, 150, 450, 3000, 15, 2};

static const float SHOP_GOODS_PRICE[] = {
    1.99, 4.99, 9.99, 2.99, 7.99, 19.99, 0.99, 4.99};



static const int EVT_OFFER_MENU = 0;
static const int EVT_OFFER_ARMS = 1;
static const int EVT_OFFER_SKILLS= 2;
static const int EVT_OFFER_COMPLETE= 3;

static const int EVT_SHOP_MENU= 4;
static const int EVT_SHOP_UPGRADE= 5;
static const int EVT_SHOP_INGAME= 6;
static const int EVT_SHOP_UPGRADE_LACK= 7;

static const int EVT_PRE_BUY= 8;
static const int EVT_COMPLETE_BUY= 9;

static const int EVT_GAME_START= 10;
static const int EVT_GAME_RESTART= 11;
static const int EVT_GAME_SAVEME= 12;
static const int EVT_GAME_REVIVE= 13;
static const int EVT_GAME_COMPLETE= 14;

static const int EVT_GAME_ENDLESS_START= 15;
static const int EVT_GAME_ENDLESS_SAVEME= 16;
static const int EVT_GAME_ENDLESS_END= 17;

static const int EVT_GAME_MISSION_COMPLETE= 18;

static const int EVT_UPGRADE_GUN= 19;
static const int EVT_UPGRADE_SKILL= 20;

static const int EVT_RATEME_SHOW = 21;
static const int EVT_RATEME_YES = 22;
static const int EVT_RATEME_LATER = 23;
static const int EVT_RATEME_NEVER = 24;

static const int EVT_DAILYREWARD = 25;

static const int EVT_GAME_PROGRESS = 26;

static const int DIFFICULTY_NORMAL = 0;
static const int DIFFICULTY_HEROIC = 1;
static const int DIFFICULTY_HARD = 2;

static const char * DIFFICULTY_STR[] = {
    "easy",
    "hard",
    "crazy",
};

static const int SKILL_MISSLE = 0;
static const int SKILL_ROBOT = 1;
static const int SKILL_SHIELD = 2;
static const int SKILL_GREED = 3;
static const int SKILL_ARMOR = 4;
static const int SKILL_STAMINA = 5;
static const int SKILL_EVASION = 6;
static const int SKILL_FRENZY = 7;
static const int SKILL_CRITICAL = 8;
static const int SKILL_STUN = 9;

static const int COL_NORMAL = 0;
static const int COL_HARD = 1;
static const int COL_CRAZY = 2;
static const int COL_FIX = 3;

#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
#define CC_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

class Global : public Singleton<Global>
{
    DECLARE_SINGLETON_CLASS(Global);
    
public:
    MainGameLayer *mainGameLayer;
    GameBackgroundLayer *gameBackgroundLayer;
    HudLayer *hudLayer;
    EffectLayer *effectLayer;
	DeadNoticeLayer *deadNoticeLayer;
    OptionLayer *optionLayer;
    MainScene *mainScene;
    ShopLayer *shopLayer;
    
    wyArray *coinChangeListeners;
    void addCoinChangeListener(CoinChangeListener *listener);
    void removeCoinChangeListener(CoinChangeListener *listener);
    void notifyAllCoinChangeListeners();

    float tileMapWidthInPixels, tileMapWidthInPixels1, realEndlessMapWidth, baseTileMapWidthInPixels;
    float tileMapHeightInPixels;
    float tileWidth;
    float mapOffsetX;
    float singleMapWidth;
    
    float tileMapOffsetY, tileMapOffsetYExtra;
    
    bool isOnGround;
    int jumpTime, jumpTime1;
    
    float scaleX;
    float scaleY;
    
    void init();
    
    int index;
    
    float hero_offset_left;
    
    float borderHeight, bgWidth, bgHeight;
    
    wyParticleSystem *emitter;
    
    CDynamicValueInt gold, iapGold;
    int *s01_grade, *s02_grade, *s03_grade;
    int *s01_score, *s02_score, *s03_score;
    int *currentProgress;
    int activeSkill, activeTool, activeWeapon;//currentProgress
    int tempSkill, tempTool;

    int fire_fury_gold, illusion_gold, sand_shield_gold;
    int fire_fury_zuanshi, illusion_zuanshi, sand_shield_zuanshi;
    int fire_fury_atk, illusion_number, illusion_atk_percent, sand_shield_life;
    float sand_shield_cd;
    
    int tool_xueping_number, tool_xiezi_number, tool_dabiao_number, tool_laoying_number;
    
    int head_grade, cloth_grade;
    int head_gold, cloth_gold;
    int head_zuanshi, cloth_zuanshi;
    int head_def, cloth_hp;
    int *weapon_gold, *weapon_zuanshi, *weapon_ammo_gold;
    float *weapon_atk, *weapon_firecd;
    
    int *skill_gold, *skill_cash;
    float *skillMulti;
    bool *hasSkillMulti;
    
    int *achv_rec, *achv_progress, *achv_progress_old, *achv_progress_temp;
    int isGuideRead, isTrainingEnd, isUpgradeTrained, isToolTrained, isXieZiTrained, isLaoYingTrained, isDaBiaoTrained;
    
    void initGame();
    
    void syncToPrefs();
    void resetPrefs();
    
    void updateFireFury();
    void updateIllusion();
    void updateSandShield();
    
    void initAllWeaponInfo();
    void initWeaponInfo(int weaponIndex);
    void updateWeapon();
    void updateCloth();
    void updateHead();

    void strToArray(const char * str, int *array);
    void arrayToStr(const char * label, int *array, int arr_len);
    
    void strToArrayEncy(const char * str, CDynamicValueInt *array);
    void arrayToStrEncy(const char * label, CDynamicValueInt *array, int arr_len);

    int currentBGMId, prevBGMId;
    bool isPlayingBGM;
    void playBGM(int id, bool isForce = false);
    void playEffect(int sfxId);
    void preloadEffect(int sfxId);
    void stopBGM();
    void replayBGM();
    
    int level_gold_normal;
    int level_total_gold;
    int level_gold, level_kills, level_enemy_kill, level_enemy_gold, level_bosses, level_zuanshi;
    int total_gold, total_enemies;
    int curHP;
    
//    int achv_killedEnemies, achv_boss1KilledTimes, achv_boss2KilledTimes, achv_boss3KilledTimes, achv_fireFuryKills, achv_illusionCastTimes, achv_bouncingTimes, achv_maxCombo;
    
    int curAchvShowCount;
    void showAchievmentEffect(int achvId, wyNode *parent, int x = -1);
    
    float heroEyeZ, bossEyeZ;
    
    bool isDebug, isShowBox2DDebug, isShowAchv, isSFXOn, isBGMOn;
    bool isShadowTimerDestroied;
    int step;
    
    const char *curLanguage, *uiName;
    
    float descFontSize, titleFontSize;
    
    int mapPieceNumber;
    
    bool hasReachCheckPoint, hasReviveFromCheckPoint;
    int prev_gold, prev_kills, prev_enemygold, prev_gold_normal;
    float checkPointX, checkPointY;
    int checkPointIdx;
    int curMapPieceIndex;
    
    bool needShowAchv;
	float virtualWinWidth, borderWidth;
    
    int isAdsRemoved;
    
    bool needPauseGame;
    
    void addGold(int gold);
    
    int deadType;
    float dead_position_x;
    void reportGameEvent(int event_id, int i1=0, float f1=0, const char *s1=NULL, const char *s2=NULL);
    
    int virtualGoodId;
    void purchaseGameItemStart(int itemId);
    void purchaseGameItemEnd(int itemId);
    void purchaseGameItemEndIOS(int itemId, bool isRestore);
    
    int getTotalScore();
    
    bool isBoss1Strong;
    
    bool needTrainUpgradeWeapon, needTrainTool;
    bool needShowOfferWall;
    
    bool isRealEndless;
    int currentCombatValue;
    int elapsedNormalCombat;
    bool isInBossCombat;
    int combatRound;
    int combatPeriod;
    int seasonSeq, bossSeq;
    int currentSeasonId, currentBossId, currentEModeDifficulty;
    std::map< int, vector<int>* > *combatValueToEnemyIdMap;
    void calcEnemyCombatValue();
    void calcCurrentCombatValue();
    int calcAppearChance();
    
    int currentSModeDifficulty;
    
    wyTMXTileMap **m_TileMapCache1, **m_TileMapCache2, **m_TileMapCache3;
    wyTMXLayer **m_TileLayerCache1, **m_TileLayerCache2, **m_TileLayerCache3;
    void generateTileMapCache();
    wyTMXTileMap* getMap(int seasonId, int& idx);
    wyTMXLayer* getMapFront(int seasonId, int idx);
    bool isMapCached;
    int accDistance;
    bool isInBossBattle, isBossFighted;
    
    CDynamicValueInt *skillArray;
    CDynamicValueInt zuanshi;
    int *skillNoticeArray;
    
    CDynamicValueInt *weaponUnlockArray;
    int *weaponNoticeArray;
    int *weaponAmmoArray;
    
    int isNewAchv;
    int killedEnemies, totalKilledEnemies, killedBosses, saveMeTimes, runDistance, totalRunDistance, xuepingTimes, xieziTimes, dabiaoTimes, laoyingTimes, fireFuryKills, illusionTimes, whrilwindTimes;
    int totalCollectedCoins;
    
    int curMapIndex, prevMapIndex;
    vector<int> *prob_combat_value;
    int currentDistance;
    
    void initEndlessMap(int difficulty);
    void initSkillEffect(int skillIndex);
    void initAllSkillEffect();
    int extraGold;
    int getWeaponAtk();
    
    wyTMXLayer* getSEMapFront(int seasonId);
    wyTMXTileMap* getSEMap(int seasonId);
    bool hasBadge;
    bool isBoostOn;
    float boostMulti;
    float heroVelocityX;
    
    bool needOpenQuest;
    int distRecNormal, distRecHeroic, distRecInfernal;
    int currentMapDifficulty;
    int isFirstPurchaseRewarded;
    
    void saveDataInGame();
    
    BaseObject *nearestEnemy;
    float nearestDistance;
    
    int missionId1, missionId2, missionId3;
    void initMissionIds();
    void refreshMIds();
    int getMinRepeatableMId(int currentMaxId);
    void updateOldMissionStatus(int mid);
    void updateNewMissionStatus(int mid);
    void testMId();
    
    void initConsts();
    int optInt(const char* key, wyJSONObject* jo);
    float optFloat(const char* key, wyJSONObject* jo);
    int optInt(int index, wyJSONArray* jo);
    float optFloat(int index, wyJSONArray* jo);
    
    wyJSONArray* optJSONArray(const char* key, wyJSONObject* jo);
    
    Global(void);
    ~Global(void);
    
    void addZuanshi(int zuanshi);
    void addGem(int gem);
    
    int getRES(const char *res) {
        char temp[50];
        if (strcmp(curLanguage, "zh") == 0) {
            sprintf(temp, "%s_zh", res);
            return RES(temp);
        }
        return RES(res);
    }
    
    const char * getRESstr(const char *res) {
        std::string temp;
        if (strcmp(curLanguage, "zh") == 0) {
            temp += "_zh.png";
            return temp.c_str();
        }
        return res;
    }
    
    const char * getIntroStr(int index) {
        return INTRO_STR[index];
    }
    
    const char * getStr(int index) {
//        if (strcmp(curLanguage, "zh") == 0) {
//            return STR_ZH[index];
//        }
//        else {
            return OTHER_STR[index];
//        }
    }
    
    uint16 seqNo;
    void resetSeqNo() {
        seqNo = CAT_BITS[TYPE_BULLET];
    }
    uint16 getSeqNo() {
        if (seqNo == 0x8000) {
            seqNo = CAT_BITS[TYPE_BULLET];
        } else {
            seqNo *= 2;
        }
        return seqNo;
    }
    
    wyBitmapFont *font_desc, *font_title, *font_desc_noborder;

    //目标值，cash奖励，序号
    int *ACHV_NUM;
    
    int *ACHV_SEQ;
    
    //武器属性定义
    int WEAPON_MAX_LEVEL;
    
    int WEAPON_GOLD_BASE;
    int WEAPON_GOLD_INC;
    
    float *WEAPON_PARAM;
    int *WEAPON_ID2SEQ, *WEAPON_SEQ2ID;
    
    //技能属性定义
    int SKILL_GOLD_BASE;
    int SKILL_GOLD_INC;
    
    int SKILL_MAX_LEVEL;
    
    float SKILL_MISSLE_BASE_ATK;
    float SKILL_MISSLE_CD;
    
    float SKILL_ROBOT_BASE_ATK;
    float SKILL_ROBOT_FIRE_RATE;
    float SKILL_ROBOT_LAST;
    float SKILL_ROBOT_CD;
    
    float SKILL_SHIELD_LAST_BASE;
    float SKILL_SHIELD_CD_BASE;
    
    float SKILL_CRITICAL_MULTI;
    float SKILL_STUN_DUR;
    
    int HERO_BASE_HP;
    int HERO_BASE_DEF;
    
    float *SKILLS_PARAMS;
    
    //每个季节经历几张地图
    int MAX_COMBAT_TIME;
    
    //最大保持round序号
    int MAX_DIFFICULTY_ROUND;
    int *MAP_DIFFICULTY;
    //boss出现时地图难度分布
    int *MAP_DIFFICULTY_BOSS;
    
    //最大保持round序号
    int MAX_COMBAT_ROUND;
    int *ROUND_COMBAT;
    float COMBAT_POWER;
    int COMBAT_VAR_MIN;//combat值最小浮动％
    int COMBAT_VAR_MAX;//combat值最大浮动％
    
    //最大保持round序号
    int MAX_APPEAR_ROUND;
    int *APPEAR_CHANCE;
    
    //boss1
    int *BOSS1_CAIDAO_ATK;
    int *BOSS1_KAN_ATK;
    int *BOSS1_XULI_LIANDAO_HP;
    int *BOSS1_XULI_LIANDAO_ATK;
    int *BOSS1_HUIXUAN_CAIDAO_HP;
    int *BOSS1_HUIXUAN_CAIDAO_ATK;
    
    //boss1 small
    int *BOSS1_SMALL_CAIDAO_ATK;
    int *BOSS1_SMALL_KAN_ATK;
    int *BOSS1_SMALL_HP;
    int *BOSS1_SMALL_GOLD;
    
    //boss2
    int *BOSS2_LANCE_ATK;
    int *BOSS2_CHONGFENG_ATK;
    int *BOSS2_XULI_LASER_HP;
    int *BOSS2_XULI_LASER_ATK;
    int *BOSS2_SMALL_LANCE_HP;
    int *BOSS2_SMALL_LANCE_ATK;
    
    //boss3
    int *BOSS3_IRONFIST_ATK;
    int *BOSS3_KNOCK_ATK;
    int *BOSS3_XULI_FIST_HP;
    int *BOSS3_XULI_TARGET_HP;
    int *BOSS3_XULI_FIST_ATK;
    int *BOSS3_MISSLE_HP;
    int *BOSS3_MISSLE_ATK;
    
    //boss无尽关中攻击间隔缩减系数
    float BOSS_ATK_INTERVAL_MULTI;
    float BOSS_STUN_TIME_MULTI;
    
    int *ENEMY_COMBAT_VALUE_EASY;
    int *ENEMY_COMBAT_VALUE_HARD;
    int *ENEMY_COMBAT_VALUE_CRAZY;
    
    int SMALL_COIN_GOLD, MIDDLE_COIN_GOLD, LARGE_COIN_GOLD;
    
    float ENEMY_ATK_MULTI, ENEMY_HP_MULTI, ENEMY_GOLD_MULTI;

    int *ENEMY_GOLD_EASY, *ENEMY_GOLD_HARD, *ENEMY_GOLD_CRAZY;
    int *ENEMY_HP_EASY, *ENEMY_HP_HARD, *ENEMY_HP_CRAZY;
    int *ENEMY_ATK_EASY, *ENEMY_ATK_HARD, *ENEMY_ATK_CRAZY;
    int *ENEMY_HP_EASY_INC, *ENEMY_HP_HARD_INC, *ENEMY_HP_CRAZY_INC;
    
    //每轮后combat值和攻防增加系数
    float ENEMY_COMBAT_POWER;
    float ENDLESS_HP_POWER, ENDLESS_ATK_POWER, ENDLESS_GOLD_POWER;
    
    float ROCKET_ATK_INC_0, ROCKET_ATK_INC_1, ROCKET_ATK_INC_2;
    
    //连续登陆奖励
    int *Reward_Number;
    int *Reward_Unit;
    
    int *ENEMY_COUNT;
    float HURT_CD;
    float SHOOT_CD_MULTI;
    //================================================================================================================
    void fillJsonIntArray(wyJSONArray* jsonArray, int *array, int len);
    void fillJsonFloatArray(wyJSONArray* jsonArray, float *array, int len);
//    void initJsonIntArray(wyJSONObject* jo, const char *arrayName, int *array);
//    void initJsonFloatArray(wyJSONObject* jo, const char *arrayName, float *array);
    
    wyArray* containerCache;
    wyArray* bulletSpriteCache;
    wyArray* fleshSpriteCache;
    wyArray* shadowSpriteCache;
    wyArray *enemy_0_cache, *enemy_1_cache, *enemy_2_cache, *effects_cache, *dian_cache;
    wyArray *coin_emitterArray, *fire_emitterArray, *smoke_emitterArray;
    wyArray *boss1Cache, *boss2Cache, *boss3Cache;
    wyArray* xuecaoCache;
    void initCaches();
    void clearCaches();
    void resetCaches();
    
    void* popFromCache(wyArray* arr);
    void pushToCache(wyArray* arr, wyNode *node);
    void createSingleNodeForCache(wyArray* arr, bool isExced = false);
    
    void gotoWorldMapScene(bool isExpand);
    void gotoMainScene(int mapIndex);
    void gotoUpgradeScene(int mapIndex);
    void gotoCGScene(bool needUnlockNandu = false);
    bool isFirstStart;

    void loadAllImages(bool isResume = false);
    
    void showMenu(wyNode *parent, int menuId);
    bool isTutorialFromMenu;
    
    bool isQuestRecorded;
    bool isSkillUnlockable(int idx);
    bool hasUnlockableSkills();
    bool isSkillNeedToUnlock(int idx);
    
    bool hasUpgradeWeapons();
    bool canWeaponUpgrade(int idx);
    bool isWeaponUlkNeedCash(int idx);
    bool hasUpgradeSkills();
    bool canSkillUpgrade(int idx);
    
    bool isSModeUnlockable(int idx);
    bool isSModeActive(int idx);
    bool isEModeUnlockable(int idx);
    bool isEModeActive(int idx);
    
    int isGameComplete;
    int isBigPackSoldout;
    
    int countScount(int seasonId, int difficulty);
    
    bool isShopOpening;
    
    void loadAllImagesAsync(LoadTexListener *sLayer = NULL);
    void loadingThread(wyTargetSelector* sel, void *data);
    
    bool needReloadTex;
    bool noNeedToSaveRunDist;
    
    void offsetGold(int diffGold);
    void setGold(int gold);
    int getGold();
    
    wyNode * getArmsSpriteByWeaponIndex(int weaponId);
    bool isRatingMe;
    
    bool isDoubleCoin;
};

#define sGlobal Global::instance()

class ResumeGameRunnable : public wyRunnable {
private:
    LoadTexListener* m_node;
    
protected:
    ResumeGameRunnable(LoadTexListener* s) {
        if (s!=NULL) {
            m_node = s;
//            s->retain();
        } else {
            m_node = NULL;
        }
    }
    
public:
    virtual ~ResumeGameRunnable() {
        if (m_node!= NULL) {
//            m_node->release();
        }
    }
    
    static ResumeGameRunnable* make(LoadTexListener* s) {
        ResumeGameRunnable* r = new ResumeGameRunnable(s);
        return (ResumeGameRunnable*)r->autoRelease();
    }
    
    virtual void run();
};

class MyPrefs : public wyPrefs
{
public:
    static int getInt(const char* key, int defValue = 0) {
        std::string decodedData = MTBase64::decode(wyPrefs::getString(key, ""));
        if (decodedData.length() > 0) {
            return atoi(decodedData.c_str());
        } else {
            return defValue;
        }
    }
    
	static void setInt(const char* key, int value) {
        char buf[50];
        sprintf(buf, "%d", value);
        std::string data(buf);
        std::string base64Data = MTBase64::encode(data, true);
        wyPrefs::setString(key, base64Data.c_str());
    }
    
    static std::string getString(const char* key, const char* defValue = NULL) {
        std::string decodedData = MTBase64::decode(wyPrefs::getString(key, ""));
        if (decodedData.length() > 0) {
            return decodedData;
        } else {
            return string(defValue);
        }
    }

	static void setString(const char* key, const char* value) {
        std::string base64Data = MTBase64::encode(value, true);
        wyPrefs::setString(key, base64Data.c_str());
    }
};


static float frand() {
    return (float) rand() / RAND_MAX;
}

static float frand_unit() {
    return 2.0f * rand() / RAND_MAX - 1.0f;
}

static bool isIntersect(wyRect r1, wyRect r2) {
	return !(r1.x > r2.x + r2.width ||
             r1.x + r1.width < r2.x ||
             r1.y > r2.y + r2.height ||
             r1.y + r1.height < r2.y);
}

static float randRange(int min, int max) {
    int result = min*100 + rand() % ((max-min)*100 + 1);
    return result / 100.0f;
}

static int randRangeInt(int min, int max) {
    int result = min + rand() % ((max-min) + 1);
    return result;
}

static int randRange(int max) {
    int result = rand() % max;
    return result;
}

static float randRangeScope(int scope) {
    int result = rand()%scope;
    if (rand()%2 == 0) {
        result = -result;
    }
    return result;
}

static void addComma(int number, char *temp)
{
	char str[20];
	sprintf(str, "%d", number);
    
	string tempStr = string(str);
    
	string::iterator it = tempStr.end();
	if (tempStr.size() > 3)
	{
		it = it - 3;
		it = tempStr.insert(it, ',');
	}
	if (tempStr.size() > 7)
	{
		it = it - 3;
		it = tempStr.insert(it, ',');
	}
	if (tempStr.size() > 11)
	{
		it = it - 3;
		it = tempStr.insert(it, ',');
	}
    
	sprintf(temp, "%s", tempStr.c_str());
}

static void addComma(const char *str, char *temp)
{
	string tempStr = string(str);
    
	string::iterator it = tempStr.end();
	if (tempStr.size() > 3)
	{
		it = it - 3;
		it = tempStr.insert(it, ',');
	}
	if (tempStr.size() > 7)
	{
		it = it - 3;
		it = tempStr.insert(it, ',');
	}
	if (tempStr.size() > 11)
	{
		it = it - 3;
		it = tempStr.insert(it, ',');
	}
    
	sprintf(temp, "%s", tempStr.c_str());
}

static std::string getModifyDateString() {
    time_t t = time(0);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y/%m/%d %H:%M:%S", localtime(&t));
    std::string ret(tmp);
    return ret;
}

static std::string TrimString(const std::string& str) {
	std::string t = str;
    
	std::string::iterator i;
	for (i = t.begin(); i != t.end(); i++) {
		if (*i != ' ') {//!isspace(*i)
			t.erase(t.begin(), i);
			break;
		}
	}
    
	if (i == t.end()) {
		return t;
	}
    
	for (i = t.end() - 1; i != t.begin(); --i) {
		if (*i != ' ') {//!isspace(*i)
			t.erase(i + 1, t.end());
			break;
		}
	}
    
	return t;
}

static std::vector<std::string> SplitString(const char* src,const char separator)
{
	std::vector<std::string> valueArray;
	if(0 == separator)
		return valueArray;
	char buffer[2560];
	//memset(buffer,0,sizeof(buffer));
	size_t strLength = strlen(src);
	if(strLength == 0 || strLength >= sizeof(buffer))
		return valueArray;
	strncpy(buffer,src,sizeof(buffer));
	std::string strValue;
	char* pos = buffer;
	char* valuepos = pos;
    
	for(; pos < buffer + strLength; pos++)
	{
		if(*pos == separator)
		{
			*pos = 0;
			strValue = valuepos;
			strValue = TrimString(strValue.c_str());
			if(strValue.size() > 0)
				valueArray.push_back(strValue);
			valuepos = pos+1;
			if(valuepos > buffer + strLength)
				break;
		}
	}
	if(valuepos < buffer + strLength
       && strlen(valuepos) > 0)
	{
		strValue = TrimString(valuepos);
		if(strValue.size() > 0)
			valueArray.push_back(strValue);
	}
	return valueArray;
}

static time_t GetTime(const char *szTime)
{
	std::vector<std::string> vecTime = SplitString(szTime, ' ');
	if(vecTime.size() != 2)
	{
		return -1;
	}
    
	std::vector<std::string> vecYMD = SplitString(vecTime[0].c_str(), '/');
	if(vecYMD.size() != 3)
	{
		return -1;
	}
    
	std::vector<std::string> vecHMS = SplitString(vecTime[1].c_str(), ':');
	if(vecHMS.size() != 3)
	{
		return -1;
	}
    
	struct tm stTime;
	memset((void *)&stTime, 0, sizeof(stTime));
	stTime.tm_year = atoi(vecYMD[0].c_str()) - 1900;
	stTime.tm_mon = atoi(vecYMD[1].c_str()) -1;
	stTime.tm_mday = atoi(vecYMD[2].c_str());
	stTime.tm_hour = atoi(vecHMS[0].c_str());
	stTime.tm_min = atoi(vecHMS[1].c_str());
	stTime.tm_sec = atoi(vecHMS[2].c_str());
	time_t ret = mktime(&stTime);
	return ret;
}

static void updateSpendTime() {
    time_t nowTime = GetTime(getModifyDateString().c_str());
    //%Y/%m/%d %H:%M:%S
    
    time_t lastTime = GetTime(wyPrefs::getString(DB_LAST_PLAY_TIME));
    
    long timespend = MAX(0, wyPrefs::getInt(DB_TIME_SPEND) + difftime(nowTime, lastTime));
    wyPrefs::setInt(DB_TIME_SPEND, timespend);
}

static void setPressColor(wyNode *node) {
    wyColor3B oldColor = node->getColor();
    wyColorHSV hsv = wyc3b2hsv(oldColor);
    hsv.v = MAX(0, hsv.v - 0.2f);
    wyColor3B c = wychsv2c3b(hsv);
    node->setColor(c);
}

static void setDisableColor(wyNode *node) {
    wyColor3B oldColor = node->getColor();
    wyColorHSV hsv = wyc3b2hsv(oldColor);
//    LOGE("h:%f,s:%f,v:%f", hsv.h, hsv.s, hsv.v);
    hsv.v = MAX(0, hsv.v - 0.35f);
    wyColor3B c = wychsv2c3b(hsv);
    node->setColor(c);
}

static void setNormalColor(wyNode *node) {
    wyColor3B oldColor = node->getColor();
    wyColorHSV hsv = wyc3b2hsv(oldColor);
    hsv.v = MAX(0, hsv.v + 0.35f);
    wyColor3B c = wychsv2c3b(hsv);
    node->setColor(c);
}

static void setEmitterColor(int index, wyParticleSystem *emitter) {
//    switch (index) {
//        case ELEMENT_TU:
//            emitter->setStartColorVariance(0.85f, 0.6f, 0.3f, 1, 0.2f, 0.2f, 0.2f, 0.5f);
//            break;
//        case ELEMENT_SHUI:
//            emitter->setStartColorVariance(0.22f, 0.78f, 1.0f, 1, 0.2f, 0.2f, 0.2f, 0.5f);
//            break;
//        case ELEMENT_FENG:
//            emitter->setStartColorVariance(0.61f, 0.75f, 0.0f, 1, 0.2f, 0.2f, 0.2f, 0.5f);
//            break;
//        case ELEMENT_HUO:
//            emitter->setStartColorVariance(0.78f, 0.0f, 0.18f, 1, 0.2f, 0.2f, 0.2f, 0.5f);
//            break;
//        default:
//            break;
//    }
}

static float RandomFloat(float lo, float hi) {
    float r = (float) rand();
    r /= RAND_MAX;
    r = (hi - lo) * r + lo;
    return r;
}

template <class T>
int getArrayLen(T& array) {
    return (sizeof(array) / sizeof(array[0]));
}

static wyColor3B mainColor = wyc3b(255, 72, 0);
static wyColor3B subColor = wyc3b(255, 192, 0);
static wyColor3B goldColor = wyc3b(255, 255, 8);
static wyColor3B zuanshiColor = wyc3b(124, 205, 255);//wyc3b(23, 167, 255);
static wyColor3B hpColor = wyc3b(255, 100, 100);

#endif
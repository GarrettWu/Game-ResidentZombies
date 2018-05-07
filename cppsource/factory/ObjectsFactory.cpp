//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#include "ObjectsFactory.h"
#include "Global.h"
#include "Trap.h"
#include "Barricade.h"
#include "BaseObject.h"
#include "Bonus.h"
#include "Box.h"
#include "Intro.h"
#include "EndPoint.h"
#include "XuanKongBiao.h"
#include "Deco.h"

DECLARE_SINGLETON_MEMBER(ObjectsFactory);

ObjectsFactory::ObjectsFactory(){
    
}

ObjectsFactory::~ObjectsFactory() {
    
}

void ObjectsFactory::createObjects(wyTMXObjectGroup* objectsGroup) {
//    int count = 0;
    for (int i = 0; i < objectsGroup->getObjectCount(); i++) {
        wyTMXObject* obj = objectsGroup->getObjectAt(i);
		if (obj == NULL)
			continue;
        float x = atof(objectsGroup->getObjectProperty(obj, "x")) * sGlobal->scaleX + sGlobal->mapOffsetX;
        float y = sGlobal->tileMapHeightInPixels - atof(objectsGroup->getObjectProperty(obj, "y")) * sGlobal->scaleY;
//        float width = atof(objectsGroup->getObjectProperty(obj, "width"));
//        float height = atof(objectsGroup->getObjectProperty(obj, "height"));
        const char *type = objectsGroup->getObjectProperty(obj, "type");
        
        if (strcmp(type, "summonCoin") == 0) {
            int prob = rand()%100;
            if (prob < 50) {
                generateRandomCoins(type, x, y + sGlobal->tileMapOffsetY, objectsGroup, obj);
            }
        }
        else if (strcmp(type, "summonItem") == 0) {
            generateRandomItem(type, x, y + sGlobal->tileMapOffsetY, objectsGroup, obj);
        }
        else if (strcmp(type, "summonTrap") == 0) {
            generateRandomTraps(type, x, y + sGlobal->tileMapOffsetY, objectsGroup, obj);
        }
        else {
            createObjectByType(type, x, y + sGlobal->tileMapOffsetY, objectsGroup, obj);
//            count++;
        }
    }
//    LOGE("obj count:%d", count);
//    if (sGlobal->index == 110) {
//        sGlobal->total_gold = 500;
//    }
//    if (sGlobal->index == 210) {
//        sGlobal->total_gold = 600;
//    }
//    if (sGlobal->index == 310) {
//        sGlobal->total_gold = 700;
//    }
}

void ObjectsFactory::createObjectByType(const char *type, float x, float y, wyTMXObjectGroup* objectsGroup, wyTMXObject* obj) {
    if (type != NULL) {
        if (strcmp(type, "end") == 0) {
            EndPoint::make(x, y, objectsGroup, obj);
        }
        else if (strcmp(type, "checkPoint") == 0) {
            CheckPoint::make(x, y, sGlobal->mainGameLayer->isFromCheckPoint);
            sGlobal->checkPointX = x;
            sGlobal->checkPointY = y;
            sGlobal->checkPointIdx = sGlobal->curMapPieceIndex;
        }
        else if (strcmp(type, "biao") == 0) {
            BonusExtra::make(x, y, BONUS_BIAO, false);
        }
        else if (strcmp(type, "xiezi") == 0) {
            BonusExtra::make(x, y, BONUS_XIEZI, false);
        }
        else if (strcmp(type, "xue") == 0) {
            BonusExtra::make(x, y, BONUS_XUE, false);
        }
        else if (strcmp(type, "ying") == 0) {
            BonusExtra::make(x, y, BONUS_YING, false);
        }
        else if (strcmp(type, "coin") == 0) {
            BonusCoin::make(x, y, COIN, false);
        }
        else if (strcmp(type, "bigcoin") == 0) {
            BonusCoin::make(x, y, BIGCOIN, false);
        }
        else if (strcmp(type, "bigbigcoin") == 0) {
            BonusCoin::make(x, y, BIGBIGCOIN, false);
        }
        else if (strcmp(type, "light") == 0) {
            if (rand()%2 == 0) {
                DecoLight::make(x, y);
            }
        }
        else if (strcmp(type, "torch") == 0) {
            DecoTorch::make(x, y);
        }
        else if (strcmp(type, "barricade") == 0) {
            Barricade::make(x, y);
        }
        else if (strcmp(type, "barricade2") == 0) {
            Barricade_S02::make(x, y, objectsGroup, obj);
        }
        else if (strcmp(type, "barricade3") == 0) {
            Barricade_S03::make(x, y);
        }
        else if (strcmp(type, "trap") == 0) {
            Trap::make(x, y);
        }
        else if (strcmp(type, "trap2") == 0) {
            Trap_S03::make(x, y);
        }
        else if (strcmp(type, "trap3") == 0) {
            Trap_S03::make(x, y);//地雷
        }
        else if (strcmp(type, "xuankongbiao") == 0) {
            XuanKongBiao::make(x, y, objectsGroup, obj);
        }
        else if (strcmp(type, "intro") == 0) {
            Intro::make(x, y, objectsGroup, obj);
        }
        else if (strcmp(type, "box") == 0) {
            generateBox(x, y, objectsGroup, obj);
        }
    }
}

void ObjectsFactory::generateRandomCoins(const char *type, float x, float y, wyTMXObjectGroup *objectsGroup, wyTMXObject *obj) {
//    1个小金币       10	0～10
//    2个小金币一字	10	10～20
//    3个小金币品字形	5	20～25
//    3个小金币斜45度	10	25～35
//    3个小金币一字	5	35～40
//    4个小金币菱形	10	40～50
//    4个小金币口形	5	50～55
//    5个小金币菱形	10	55～65
//    5个小金币W形    5	65～70
//    6个小金币两列	10	70～80
//    6个小金币两行	5	80～85
//    1中金币        10	85～95
//    1大金币        5	95～100
    int prob = rand()%100;
    if (prob > 80) {
        return;
    }
    
    x = x + randRangeScope(2)*sGlobal->tileWidth;
    y = y + DP(randRangeScope(10))*sGlobal->scaleY;
    int seed = rand()%100;
    if (seed <= 10) {
        BonusCoin::make(x, y, COIN, false);
    }
    else if (seed > 10 && seed <= 20) {
        BonusCoin::make(x, y, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y, COIN, false);
    }
    else if (seed > 20 && seed <= 25) {
        BonusCoin::make(x, y, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y+sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth*2, y, COIN, false);
    }
    else if (seed > 25 && seed <= 35) {
        BonusCoin::make(x, y, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y+sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth*2, y+sGlobal->tileWidth*2, COIN, false);
    }
    else if (seed > 35 && seed <= 40) {
        BonusCoin::make(x, y, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth*2, y, COIN, false);
    }
    else if (seed > 40 && seed <= 50) {
        BonusCoin::make(x-sGlobal->tileWidth, y, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y, COIN, false);
        BonusCoin::make(x, y+sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x, y-sGlobal->tileWidth, COIN, false);
    }
    else if (seed > 50 && seed <= 55) {
        BonusCoin::make(x-sGlobal->tileWidth, y+sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x-sGlobal->tileWidth, y-sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y+sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y-sGlobal->tileWidth, COIN, false);
    }
    else if (seed > 55 && seed <= 65) {
        BonusCoin::make(x, y, COIN, false);
        BonusCoin::make(x-sGlobal->tileWidth, y, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y, COIN, false);
        BonusCoin::make(x, y+sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x, y-sGlobal->tileWidth, COIN, false);
    }
    else if (seed > 65 && seed <= 70) {
        BonusCoin::make(x, y, COIN, false);
        BonusCoin::make(x-sGlobal->tileWidth, y-sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x-sGlobal->tileWidth*2, y, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y-sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth*2, y, COIN, false);
    }
    else if (seed > 70 && seed <= 80) {
        BonusCoin::make(x, y, COIN, false);
        BonusCoin::make(x-sGlobal->tileWidth, y, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y, COIN, false);
        BonusCoin::make(x-sGlobal->tileWidth, y+sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x, y+sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y+sGlobal->tileWidth, COIN, false);
    }
    else if (seed > 80 && seed <= 85) {
        BonusCoin::make(x, y, COIN, false);
        BonusCoin::make(x, y+sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x, y-sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y+sGlobal->tileWidth, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y, COIN, false);
        BonusCoin::make(x+sGlobal->tileWidth, y-sGlobal->tileWidth, COIN, false);
    }
    else if (seed > 85 && seed <= 95) {
        BonusCoin::make(x, y, BIGCOIN, false);
    }
    else if (seed > 95 && seed <= 100) {
        BonusCoin::make(x, y, BIGBIGCOIN, false);
    }
}

void ObjectsFactory::generateRandomItem(const char *type, float x, float y, wyTMXObjectGroup *objectsGroup, wyTMXObject *obj) {
    x = x + randRangeScope(2)*sGlobal->tileWidth;
    y = y + DP(randRangeScope(10))*sGlobal->scaleY;
    int seed = rand()%100;
    if (seed <= 5) {
        BonusExtra::make(x, y, BONUS_XUE, false);
    }
    else if (seed > 5 && seed <= 8) {
        BonusExtra::make(x, y, BONUS_XIEZI, false);
    }
    else if (seed > 8 && seed <= 11) {
        BonusExtra::make(x, y, BONUS_YING, false);
    }
    else if (seed > 11 && seed <= 14) {
        BonusExtra::make(x, y, BONUS_BIAO, false);
    }
}

void ObjectsFactory::generateRandomTraps(const char *type, float x, float y, wyTMXObjectGroup *objectsGroup, wyTMXObject *obj) {
    x = x + randRangeScope(2)*sGlobal->tileWidth;
    int seed = rand()%100;
    if (seed <= 7) {
        Trap::make(x, y);
    }
    else if (seed > 7 && seed <= 15) {
        Trap_S03::make(x, y);
    }
    else if (seed > 15 && seed <= 20) {
        Barricade::make(x, y);
    }
    else if (seed > 20 && seed <= 25) {
        char cmd[50];
        int isBounce = 0;
        float ofY = getShiTouCMD(cmd, isBounce);
        Barricade_S02::make(x, y+ofY, objectsGroup, obj, cmd, 1, isBounce);
    }
    else if (seed > 25 && seed <= 30) {
        Barricade_S03::make(x, y);
    }
    else if (seed > 35 && seed <= 40) {
        char cmd[50];
        float ofY = getXuanKongBiaoCMD(cmd);
        XuanKongBiao::make(x, y+ofY, objectsGroup, obj, cmd);
    }
}

void ObjectsFactory::generateBox(float x, float y, wyTMXObjectGroup *objectsGroup, wyTMXObject *obj) {
    const char *item = objectsGroup->getObjectProperty(obj, "item");
    if (item != NULL) {
        if (strcmp(item, "coin") == 0) {
            getBonus(0, x, y);
        }
        else if (strcmp(item, "bigcoin") == 0) {
            getBonus(1, x, y);
        }
        else if (strcmp(item, "bigbigcoin") == 0) {
            getBonus(2, x, y);
        }
        else if (strcmp(item, "biao") == 0) {
            getBonus(3, x, y);
        }
        else if (strcmp(item, "xiezi") == 0) {
            getBonus(4, x, y);
        }
        else if (strcmp(item, "xue") == 0) {
            getBonus(5, x, y);
        }
        else if (strcmp(item, "ying") == 0) {
            getBonus(6, x, y);
        }
    } else {
        int seed = rand()%5+2;
        getBonus(seed, x, y);//
    }
}

void ObjectsFactory::getBonus(int type, int x, int y) {
//    LOGE("box generate item:%d", type);
    switch (type) {
        case 0:
            BonusCoin::make(x, y + DP(0)*sGlobal->scaleY, COIN, true);
//            sGlobal->total_gold += 10;
            break;
        case 1:
            BonusCoin::make(x, y + DP(0)*sGlobal->scaleY, BIGCOIN, true);
//            sGlobal->total_gold += 50;
            break;
        case 2:
            BonusCoin::make(x, y + DP(0)*sGlobal->scaleY, BIGBIGCOIN, true);
//            sGlobal->total_gold += 100;
            break;
        case 3:
            BonusExtra::make(x, y + DP(0)*sGlobal->scaleY, BONUS_BIAO, false);
            break;
        case 4:
            BonusExtra::make(x, y + DP(0)*sGlobal->scaleY, BONUS_XIEZI, false);
            break;
        case 5:
            BonusExtra::make(x, y + DP(0)*sGlobal->scaleY, BONUS_XUE, false);
            break;
        case 6:
            BonusExtra::make(x, y + DP(0)*sGlobal->scaleY, BONUS_YING, false);
            break;
        default:
            break;
    }
}




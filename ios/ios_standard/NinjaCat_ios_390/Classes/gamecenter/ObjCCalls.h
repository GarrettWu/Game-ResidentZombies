//PrintMessage.h
#ifndef __PRINTMESSAGE_H__
#define __PRINTMESSAGE_H__

#include <stddef.h>

#define IAP_REMOVE_ADS @"com.cafgame.nyankoninja.iap.removeAd"//$0.99
#define IAP_SMALL_GOLD_PACK @"com.cafgame.nyankoninja.iap.gold0" //$1.99
#define IAP_MIDDLE_GOLD_PACK @"com.cafgame.nyankoninja.iap.gold1" //$14.99
#define IAP_LARGE_GOLD_PACK @"com.cafgame.nyankoninja.iap.gold2" //$29.99
#define IAP_SMALL_ITEM_BOX @"com.cafgame.nyankoninja.iap.item0" //$5.99
#define IAP_MIDDLE_ITEM_PACK @"com.cafgame.nyankoninja.iap.item1" //$14.99
#define IAP_LARGE_ITEM_PACK @"com.cafgame.nyankoninja.iap.item2" //$29.99
#define IAP_SMALL_GIFT_PACK @"com.cafgame.nyankoninja.iap.gift0" //$9.99
#define IAP_MIDDLE_GIFT_PACK @"com.cafgame.nyankoninja.iap.gift1" //$49.99
#define IAP_SUPER_WEAPON @"com.cafgame.nyankoninja.iap.superweapon" //$3.99

class ObjCCalls
{
public:
//    void OpenURL(const char *url);
//    void ShowAchievements();
//    bool IsGameCenterAPIAvailable();

    UIViewController *tempVC;
    
    void reportScore(int score);
    void showLeaderboard();
    void reportAchievment(int achvId, double percents);
    void showAchievment();
    
    void showAd();
    void hideAd();
    
    void showOfferWall();
    void reportEvent(const char* event_id, const char* label);
    
    static ObjCCalls *instance_;
    static ObjCCalls *getInstance() {
        if (instance_ == NULL) {
            instance_ = new ObjCCalls();
        }
        return instance_;
    }
    
    static void destroy() {
        if (instance_ != NULL) {
            delete instance_;
            instance_ = NULL;
        }
    }
    
    void loadingThreadEntry(wyTargetSelector* sel);
    void loadingThreadEntry1(wyTargetSelector* sel);
    
    void queryAndSpendScore();
    void buyItem(int itemId);
    void buyItemEnd(int itemId, bool isRestore = false);
    
    void restoreAllPurchases();
};

#endif//__PRINTMESSAGE_H__
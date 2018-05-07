//PrinceMessage.mm (.mm means its objective-c++?  Which means you can access it in c++?  This file can combine c++ and objective c?)
#include "ObjCCalls.h" 
#include "ObjCCalls_objc.h" 
#import "GameCenterManager.h" 
#import "GLViewController.h"
#import "NinjaCat_ios_390AppDelegate.h"
#import "Global.h"
#import "IAPUtil.h"
#import "Flurry.h"
#import "CafPaid.h"
#import "TapjoyConnect.h"

ObjCCalls *ObjCCalls::instance_ = NULL;

//void ObjCCalls::OpenURL(const char *url)
//{
//    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]]];
//}
//
//void ObjCCalls::ShowAchievements()
//{
//    [ObjCCalls_Objc ShowAchievements];
//}
//
//bool ObjCCalls::IsGameCenterAPIAvailable()
//{
//    // Check for presence of GKLocalPlayer class.
//    BOOL localPlayerClassAvailable = (NSClassFromString(@"GKLocalPlayer")) != nil;
// 
//    // The device must be running iOS 4.1 or later.
//    NSString *reqSysVer = @"4.1";
//    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
//    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
// 
//    return (localPlayerClassAvailable && osVersionSupported);
//}


//so.boo.ninjiacat.highscore
//com.cafgame.nyankoninja.leaderboard.highscore
void ObjCCalls::reportScore(int rawScore) {
    [[GameCenterManager sharedManager] saveAndReportScore:rawScore leaderboard:@"so.boo.ninjiacat.highscore"];
    
//    GKScore *score = [[[GKScore alloc]
//                       initWithCategory:@"com.mtstudio.ninjacat.leaderboard.highscore"] autorelease];
//    score.value = rawScore;
//    [score reportScoreWithCompletionHandler:^(NSError *error) {
//        dispatch_async(dispatch_get_main_queue(), ^(void)
//                       {
//                           
//                       }); }];
}

//void ObjCCalls::reportAchievement() {
//    [[GameCenterManager sharedManager] saveAndReportAchievement:@"1000Points" percentComplete:50];
//}
//
void ObjCCalls::showLeaderboard() {
    [ObjCCalls_Objc ShowLeaderBoard];
}

void ObjCCalls::reportAchievment(int achvId, double percent) {
    NSString* identifer;
    switch (achvId) {
        case 0:
            identifer = @"com.mtstudio.ninjacat.firstblood";
            break;
            
        default:
            break;
    }
    [[GameCenterManager sharedManager] saveAndReportAchievement:identifer percentComplete:percent];
}

void ObjCCalls::showAchievment() {
    [ObjCCalls_Objc ShowAchievements];
}

void ObjCCalls::showAd() {
    if (sGlobal->isAdsRemoved) {
        return;
    }
    [[GLViewController instance] showAd];
}

void ObjCCalls::loadingThreadEntry(wyTargetSelector* sel) {
    [[GLViewController instance] showAd];
}

void ObjCCalls::hideAd() {
    [[GLViewController instance] hideAd];
}
void ObjCCalls::loadingThreadEntry1(wyTargetSelector* sel) {
    [[GLViewController instance] hideAd];
}

void ObjCCalls::showOfferWall() {
//    [[ApuLmmobAdWall instance] pushLmmobAdWall];
    [TapjoyConnect showOffersWithViewController:([GLViewController instance])];
}

void ObjCCalls::queryAndSpendScore() {
//    [[LmmobAdWallSDK defaultSDK] ScoreQuery];
//    [TapjoyConnect getTapPoints];
}

void ObjCCalls::reportEvent(const char* event_id, const char* label) {
//    [ApuNetworkSend point:1 pointdata:0];
    NSString *evt_name = [NSString stringWithUTF8String:(event_id)];
    NSString *evt_content = [NSString stringWithUTF8String:(label)];
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
                          evt_content, @"content", nil];
    [Flurry logEvent:evt_name withParameters:dict];
}

void ObjCCalls::buyItem(int itemId) {
    NSString *key;
    int gold;
    switch (itemId) {
//        case SHOP_REMOVE_AD_ID:
//            key = [NSString stringWithFormat:IAP_REMOVE_ADS];
//            gold = 99;
//            break;
//        case SHOP_COIN_SMALL_ID:
//            key = [NSString stringWithFormat:IAP_SMALL_GOLD_PACK];
//            gold = 199;
//            break;
//        case SHOP_COIN_MIDDLE_ID:
//            key = [NSString stringWithFormat:IAP_MIDDLE_GOLD_PACK];
//            gold = 1499;
//            break;
//        case SHOP_COIN_LARGE_ID:
//            key = [NSString stringWithFormat:IAP_LARGE_GOLD_PACK];
//            gold = 2999;
//            break;
//        case SHOP_TOOL_SMALL_ID:
//            key = [NSString stringWithFormat:IAP_SMALL_ITEM_BOX];
//            gold = 599;
//            break;
//        case SHOP_TOOL_MIDDLE_ID:
//            key = [NSString stringWithFormat:IAP_MIDDLE_ITEM_PACK];
//            gold = 1499;
//            break;
//        case SHOP_TOOL_LARGE_ID:
//            key = [NSString stringWithFormat:IAP_LARGE_ITEM_PACK];
//            gold = 2999;
//            break;
//        case SHOP_GIFT_SMALL_ID:
//            key = [NSString stringWithFormat:IAP_SMALL_GIFT_PACK];
//            gold = 999;
//            break;
//        case SHOP_GIFT_MIDDLE_ID:
//            key = [NSString stringWithFormat:IAP_MIDDLE_GIFT_PACK];
//            gold = 4999;
//            break;
//        case SHOP_EXPLOSIVE_DART_ID:
//            key = [NSString stringWithFormat:IAP_SUPER_WEAPON];
//            gold = 399;
            break;
        default:
            break;
    }
    [[NinjaCat_ios_390AppDelegate instance] paid:key money:gold];
//    [[IAPUtil shared] buy:(IAP0p99)];
}

void ObjCCalls::buyItemEnd(int itemId, bool isRestore) {
    sGlobal->purchaseGameItemEndIOS(itemId, isRestore);
}

void ObjCCalls::restoreAllPurchases() {
    [[CafPaid shared] restorePurchases];
}



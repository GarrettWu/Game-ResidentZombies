//
//  NinjaCat_ios_390AppDelegate.m
//  NinjaCat_ios_390
//
//  Created by  on 12-3-6.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import "NinjaCat_ios_390AppDelegate.h"
#import "GLViewController.h"
#import "wyDirector.h"
#import "GameCenterManager.h"
#import "Global.h"
#if IOS
#include "ObjCCalls.h"
#import "CafPaid.h"
#import "CafAd.h"
#endif
#import "Flurry.h"
#import "TapjoyConnect.h"

#define FLURRY_APP_KEY @"GDSR8GYXGWJHSFYNYBKY"
#define TAPJOY_APP_ID @"c3c7d3c1-f8cd-4923-9323-cd7d1c29f57c"
#define TAPJOY_SECRET_KEY @"V2qRTmZeozBEPN3zQxDT"

@implementation NinjaCat_ios_390AppDelegate

static NinjaCat_ios_390AppDelegate *instance_ = nil;

+(id) instance
{
	return instance_;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    instance_ = self;

    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0){
        [self.window addSubview: self.viewController.view];
    } else {
        [self.window setRootViewController:self.viewController];
    }
	[self.window makeKeyAndVisible];

    [[GameCenterManager sharedManager] initGameCenter];
    
//    [[CafAd sharedInstance] createAdBannerView];
//    [[CafAd sharedInstance] hideBannerView];
    
//    [TapjoyConnect requestTapjoyConnect:TAPJOY_APP_ID secretKey:TAPJOY_SECRET_KEY];
    
    [Flurry startSession:FLURRY_APP_KEY];
}

- (void)applicationWillResignActive:(UIApplication *)application {
	wyDirector* director = wyDirector::getInstanceNoCreate();
	if(director) {
		director->pause();
    }
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
	wyDirector* director = wyDirector::getInstanceNoCreate();
	if(director)
		director->resume();
}

- (void)dealloc
{
    updateSpendTime();
    
    instance_ = nil;
    
    [self.window release];
    [self.viewController release];

    [super dealloc];
}

- (void)lmmobCallBack:(NSNumber *)score {
	LOGE("score:%d", score.integerValue);
    sGlobal->addGold(score.integerValue);
}

- (void)paid:(NSString *)item money:(int)money {
    [[CafPaid shared] paid:item money:money target:self selector:@selector(payCallBack:isOk:)];
}

- (void)payCallBack:(NSString *)pid isOk:(NSString*)isOk {
    NSLog(@"Product ID:%@, %@", pid, isOk);
    if ([isOk isEqualToString:(@"NO")]) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable (@"Purchase failed, please try again later...",@"Localizable", nil) message:nil delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [alert show];
        return;
    } else {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable (@"Congratulations! Purchase succeed.",@"Localizable", nil) message:nil delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [alert show];
    }
    
    int realItemId = -1;
//    if ([pid isEqualToString:IAP_REMOVE_ADS]) {
//        realItemId = SHOP_REMOVE_AD_ID;
//    }
//    else
//    if ([pid isEqualToString:IAP_SMALL_GOLD_PACK]) {
//        realItemId = SHOP_COIN_SMALL_ID;
//    }
//    else if ([pid isEqualToString:IAP_MIDDLE_GOLD_PACK]) {
//        realItemId = SHOP_COIN_MIDDLE_ID;
//    }
//    else if ([pid isEqualToString:IAP_LARGE_GOLD_PACK]) {
//        realItemId = SHOP_COIN_LARGE_ID;
//    }
//    else if ([pid isEqualToString:IAP_SMALL_ITEM_BOX]) {
//        realItemId = SHOP_TOOL_SMALL_ID;
//    }
//    else if ([pid isEqualToString:IAP_MIDDLE_ITEM_PACK]) {
//        realItemId = SHOP_TOOL_MIDDLE_ID;
//    }
//    else if ([pid isEqualToString:IAP_LARGE_ITEM_PACK]) {
//        realItemId = SHOP_TOOL_LARGE_ID;
//    }
//    else if ([pid isEqualToString:IAP_SMALL_GIFT_PACK]) {
//        realItemId = SHOP_GIFT_SMALL_ID;
//    }
//    else if ([pid isEqualToString:IAP_MIDDLE_GIFT_PACK]) {
//        realItemId = SHOP_GIFT_MIDDLE_ID;
//    }
//    else if ([pid isEqualToString:IAP_SUPER_WEAPON]) {
//        realItemId = SHOP_EXPLOSIVE_DART_ID;
//    }
    ObjCCalls::getInstance()->buyItemEnd(realItemId);
}

- (void)checkRestoreGoodsTypes:(NSMutableArray *)array {
    for (NSString *str in array) {
//        if ([str isEqualToString:IAP_REMOVE_ADS]) {
//            ObjCCalls::getInstance()->buyItemEnd(SHOP_REMOVE_AD_ID, true);
//        }
//        else
        if ([str isEqualToString:IAP_SUPER_WEAPON]) {
//            ObjCCalls::getInstance()->buyItemEnd(SHOP_EXPLOSIVE_DART_ID, true);
        }
    }
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable (@"Purchase restore succeed!",@"Localizable", nil) message:nil delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
    [alert show];
    
//    if (sGlobal->shopLayer != NULL) {
//        sGlobal->shopLayer->updateRestoredItems();
//    }
}

@end

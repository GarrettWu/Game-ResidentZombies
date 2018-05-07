#import "iAd/ADBannerView.h"
#import "GADBannerViewDelegate.h"
#import "GADRequest.h"

#define ADMOB_KEY @"a1507cdeef2926e"
#define IAD_KEY @"com.cafgame.nyankoninja.iap.gift1" //$49.99

@interface CafAd : UIViewController<ADBannerViewDelegate, GADBannerViewDelegate>{
    ADBannerView *adView;
    GADBannerView *adBanner;
    bool bShowIAD;
    bool bValidIAD;
    bool receivedIAD;
    
    int receiveAdmob;
    
    int showState;
    
    int isIADError;
    int isAdmobError;
}

- (void)createAdBannerView;
+ (CafAd *)sharedInstance;
- (GADRequest *)createRequest;
- (void) hideBannerView;
- (void) unhideBannerView;

- (void) receiveADmob;
@end
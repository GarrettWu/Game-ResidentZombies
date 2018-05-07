#import "CafAd.h"
#import "GADBannerView.h"
#import "GADRequest.h"
#import "GLViewController.h"

@implementation CafAd

static CafAd *cafAd = nil;

+ (CafAd *) sharedInstance {
    if (!cafAd) {
        cafAd = [[CafAd alloc] init];
    }
    return cafAd;
}

- (void)createAdBannerView {
    bValidIAD = false;
    receiveAdmob = 0;
    
    //同时创建iAd和admob，它们都去请求广告
    Class classAdBannerView = NSClassFromString(@"ADBannerView");
    if (classAdBannerView != nil)
    {
        adView = [[classAdBannerView alloc] init];
        [adView setFrame:CGRectMake(0, 0, 320, 50)];
        
        [adView setDelegate:self];
        
        adView.hidden = TRUE;
        bShowIAD = true;
        receivedIAD = false;
        
        [[[GLViewController instance] view] addSubview:adView];
    }
    
    CGPoint origin = CGPointMake(0.0, 0.0);
    
    // Use predefined GADAdSize constants to define the GADBannerView.
    adBanner = [[[GADBannerView alloc] initWithAdSize:kGADAdSizeBanner
                                               origin:origin]
                autorelease];
    
    adBanner.adUnitID = ADMOB_KEY;
    [adBanner setDelegate:self];
    [adBanner setRootViewController:self];
    [[[GLViewController instance] view] addSubview:adBanner];
    
    [adBanner loadRequest:[self createRequest]];
    
    showState = 0;
    
    isIADError = 0;
    isAdmobError = 0;
}

- (BOOL)bannerViewActionShouldBegin:(ADBannerView *)banner
               willLeaveApplication:(BOOL)willLeave
{
    return YES;
}

- (void)bannerViewDidLoadAd:(ADBannerView *)banner
{
    if (!bShowIAD)
        return;
    NSLog(@"%d",adView.bannerLoaded);
    adView.hidden = NO;
    NSLog(@"did load");
    
    //    [adView.superview bringSubviewToFront:adView];
    
    //收到iAd广告，就把admob广告窗口关了
    if (adBanner != nil)
    {
        adBanner.delegate = nil;
        [adBanner removeFromSuperview];
        adBanner = nil;
    }
    
    receivedIAD = true;
    
    if (showState == 1) {
        if (receivedIAD) {
            adView.hidden = NO;
        } else {
            adView.hidden = YES;
        }
    }
    
    isIADError = 0;
}

- (void)bannerView:(ADBannerView *)banner
didFailToReceiveAdWithError:(NSError *)error
{
    isIADError = 1;
    adView.hidden = YES;
}

- (GADRequest *)createRequest {
    GADRequest *request = [GADRequest request];
    
    return request;
}

#pragma mark GADBannerViewDelegate impl

// We've received an ad successfully.
- (void)adViewDidReceiveAd:(GADBannerView *)adView {
    NSLog(@"Received ad successfully");
    //    [adBanner.superview bringSubviewToFront:adBanner];
    
    [self receiveADmob];
    
    if (showState == 1) {
        if (adBanner != nil) {
            if (receiveAdmob == 0) {
                adBanner.hidden = YES;
            } else {
                adBanner.hidden = NO;
            }
        }
        bShowIAD = true;
    }
    
    isAdmobError = 0;
}

- (void) receiveADmob
{
    //收到admob广告5次了，说明到目前未知还是没有收到iAd，这样说明你收不到iAd广告就可以关闭iAd了
    receiveAdmob++;
    if (receiveAdmob > 5)
    {
        [adView setDelegate:nil];
    }
    
}

- (void)adView:(GADBannerView *)view
didFailToReceiveAdWithError:(GADRequestError *)error {
    NSLog(@"Failed to receive ad with error: %@", [error localizedFailureReason]);
    
    isAdmobError = 1;
}

- (void) hideBannerView
{
    showState = 0;
    
    adView.hidden = YES;
    
    if (adBanner != nil)
        adBanner.hidden = YES;
    bShowIAD = false;
}

- (void) unhideBannerView
{
    showState = 1;
    
    if (receivedIAD && isIADError == 0) {
        adView.hidden = NO;
    } else {
        adView.hidden = YES;
    }
    
    if (adBanner != nil) {
        if (receiveAdmob == 0 && isAdmobError == 0) {
            adBanner.hidden = YES;
        } else {
            adBanner.hidden = NO;
        }
    }
    bShowIAD = true;
}

-(void)dealloc{
    
    [adView removeFromSuperview];
    [super dealloc];
}

@end
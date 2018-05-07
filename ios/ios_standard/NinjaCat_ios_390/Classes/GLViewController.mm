//
//  GLViewController.m
//  NinjaCat_ios_390
//
//  Created by  on 12-3-6.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import "GLViewController.h"
#import "WiEngine.h"
#import "LogoScene.h"
#include "Global.h"
#import "CafAd.h"

// you should change it to 1 if you use landscape orientation
#define LANDSCAPE 1

@implementation GLViewController

static GLViewController *instance_ = nil;

+(GLViewController *) instance
{
	return instance_;
}

- (void)viewDidLoad {
    instance_ = self;
    
    isAdShowing = false;

    CGRect rect = [[UIScreen mainScreen] bounds];
    LOGE("mainScreen: width:%f, height:%f", rect.size.width, rect.size.height);
    LOGE("mainScreen: x:%f, y:%f, scale:%f", rect.origin.x, rect.origin.y, [[UIScreen mainScreen] scale]);
    
    int w = rect.size.width * [[UIScreen mainScreen] scale];
    int h = rect.size.height * [[UIScreen mainScreen] scale];
    
    int width = MAX(w, h);
    int height = MIN(w, h);
    LOGE("width:%d, height:%d", width, height);
    
	((WYEAGLView*)self.view).delegate = self;
	
	/*
	 * You should uncomment following code if you want to
	 * use base size adaption mode. To know what is base size
	 * adaption mode, view WiYun blog: http://blog.wiyun.com/?p=949
 	 */
	wyDirector* director = wyDirector::getInstance();
    director->setScaleMode(SCALE_MODE_BASE_SIZE_FIT_XY);
    director->setBaseSize(width, height);
}

- (void)dealloc {
    instance_ = nil;
	((WYEAGLView*)self.view).delegate = nil;
    [super dealloc];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
#if LANDSCAPE
	return UIInterfaceOrientationIsLandscape(toInterfaceOrientation);
#else
	return UIInterfaceOrientationIsPortrait(toInterfaceOrientation);
#endif
}

- (NSUInteger) supportedInterfaceOrientations{
    return UIInterfaceOrientationMaskLandscape;
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)eaglView:(WYEAGLView*)v frameBufferCreatedWithWidth:(int)width height:(int)height {
//    wyTextureManager::getInstance()->setTexturePixelFormat(WY_TEXTURE_PIXEL_FORMAT_RGBA4444);
    LOGE("width:%d, height:%d", width, height);
    
	wyDirector* director = wyDirector::getInstance();
	if(director->getRunningScene() == NULL) {
        sGlobal->init();
        
//        wyTextureManager::getInstance()->setTexturePixelFormat(WY_TEXTURE_PIXEL_FORMAT_RGBA4444);
        
//		director->setShowFPS(false);
        director->setShowFPS(true);
//        director->setMaxFrameRate(40);
//        director->setTickFactor(0.1);
		//director->setProjection(PROJECTION_2D);
		// run with it
		wyScene* scene = new LogoScene();
		director->runWithScene(scene);
		scene->release();
	}
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	
	wyDirector* director = wyDirector::getInstance();
	director->resume();
}

- (void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:animated];
	
	wyDirector* director = wyDirector::getInstance();
	director->pause();
}

- (void)showAd {
//    if (isAdShowing)
//        return;
//    
//    isAdShowing = true;
//    [[CafAd sharedInstance] unhideBannerView];
}

- (void)hideAd {
//    if (!isAdShowing)
//        return;
//
//    [[CafAd sharedInstance] hideBannerView];
//    isAdShowing = false;
}

@end

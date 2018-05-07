#import "ObjCCalls_objc.h" 
#import <stdio.h>
#import "WYEAGLView.h"
#include "ViewController.h"

@implementation ObjCCalls_Objc

UIViewController *g_myViewController;


+ (void) ShowLeaderBoard
{
    //if (hasGameCenter)
    GKLeaderboardViewController *leaderBoard = [[GKLeaderboardViewController alloc] init];
    if (leaderBoard != nil)
    {
        leaderBoard.leaderboardDelegate = self;

        // Create an additional UIViewController to attach the GKAchievementViewController to
        UIViewController *myViewController = [[UIViewController alloc] init];

        g_myViewController = myViewController;
        
        UIApplication* clientApp = [UIApplication sharedApplication];
        
        UIWindow* topWindow = [clientApp keyWindow];
        
        if (!topWindow) {
            topWindow = [[clientApp windows] objectAtIndex:0];
        }
        
        [topWindow addSubview:myViewController.view];
        
        [myViewController presentModalViewController:leaderBoard animated:YES];
    }
    [leaderBoard release];
    
//    ViewController *myViewController = [[ViewController alloc] init];
//    
//    g_myViewController = myViewController;
//    
//    UIApplication* clientApp = [UIApplication sharedApplication];
//    
//    UIWindow* topWindow = [clientApp keyWindow];
//    
//    if (!topWindow) {
//        topWindow = [[clientApp windows] objectAtIndex:0];
//    }
//    
//    [topWindow addSubview:myViewController.view];
    
//    [myViewController presentModalViewController:leaderBoard animated:YES];
}

+ (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController
{
    [g_myViewController dismissModalViewControllerAnimated:NO];
    [g_myViewController.view removeFromSuperview];
}

+ (void) ShowAchievements
{
    GKAchievementViewController *achievment = [[GKAchievementViewController alloc] init];
    if (achievment != nil)
    {
        achievment.achievementDelegate = self;

        UIViewController *myViewController = [[UIViewController alloc] init];
        
        g_myViewController = myViewController;
        
        UIApplication* clientApp = [UIApplication sharedApplication];
        
        UIWindow* topWindow = [clientApp keyWindow];
        
        if (!topWindow) {
            topWindow = [[clientApp windows] objectAtIndex:0];
        }
        
        [topWindow addSubview:myViewController.view];
        
        [myViewController presentModalViewController:achievment animated:YES];
    }
    [achievment release];
}

+ (void)achievementViewControllerDidFinish:(GKAchievementViewController *)viewController
{
    [g_myViewController dismissModalViewControllerAnimated:NO];
    [g_myViewController.view removeFromSuperview];
}

@end
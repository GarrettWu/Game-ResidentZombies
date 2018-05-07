#import <Foundation/NSObject.h>
#import <GameKit/GameKit.h>
#import <GameKit/GKAchievementViewController.h>

#import <UIKit/UIKit.h>


@interface ObjCCalls_Objc: NSObject {

    UIViewController *myViewController;
}

+ (void) ShowAchievements;
+ (void) ShowLeaderBoard;

@end


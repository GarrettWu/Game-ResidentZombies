#import "FMUIWebViewBridge.h"
#import "Global.h"

@implementation FMUIWebViewBridge
- (id)init{
    self = [super init];
    if (self) {
        // init code here.
    }
    return self;
}

- (void)dealloc{
    [mBackButton release];
    [mToolbar release];
    [mWebView release];
    [mView release];
    [super dealloc];
}

-(void) setLayerWebView : (FMLayerWebView*) iLayerWebView URLString:(const char*) urlString{
    mLayerWebView = iLayerWebView;
    wySize size = mLayerWebView->getContentSize();
    size.height /= size.height/320;
    mView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, size.width , size.height)];
    // create webView
    //Bottom size
    int wBottomMargin = size.height*0.12;
    int wWebViewHeight = size.height - wBottomMargin;
    NSLog(@"wBottomMargin:%d, wWebViewHeight:%d", wBottomMargin, wWebViewHeight);
    mWebView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, size.width, wWebViewHeight)];
    mWebView.delegate = self;
    
    NSString *urlBase = [NSString stringWithCString:urlString encoding:NSUTF8StringEncoding];
    [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlBase ]]];
    [mWebView setUserInteractionEnabled:NO]; //don't let the user scroll while things are
    
    //create a tool bar for the bottom of the screen to hold the back button
    mToolbar = [UIToolbar new];
    [mToolbar setFrame:CGRectMake(0, wWebViewHeight, size.width, wBottomMargin)];
    mToolbar.barStyle = UIBarStyleBlackOpaque;
    //Create a button
    mBackButton = [[UIBarButtonItem alloc] initWithTitle:@"Back"
                                                   style: UIBarButtonItemStyleDone
                                                  target: self
                                                  action:@selector(backClicked:)];
    
    //[backButton setBounds:CGRectMake(0.0, 0.0, 95.0, 34.0)];
    [mToolbar setItems:[NSArray arrayWithObjects:mBackButton,nil] animated:YES];
    
    [mView addSubview:mWebView];
    [mView addSubview:mToolbar];
    //    [mToolbar release];
    
    // add the webView to the view
    
    CATransition *animation = [CATransition animation];
    animation.duration = 0.5f;
    animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
	animation.fillMode = kCAFillModeForwards;
    animation.type = kCATransitionMoveIn;
    animation.subtype = kCATransitionFromTop;
    [mView.layer addAnimation:animation forKey:@"animation"];
    animation.removedOnCompletion = YES;
    
    [wyDirector::getInstance()->getGLView() addSubview:mView];
    
}

- (void)webViewDidStartLoad:(UIWebView *)thisWebView {
    
}

- (void)webViewDidFinishLoad:(UIWebView *)thisWebView{
    [mWebView setUserInteractionEnabled:YES];
    mLayerWebView->webViewDidFinishLoad();
}

- (void)webView:(UIWebView *)thisWebView didFailLoadWithError:(NSError *)error {
    if ([error code] != -999 && error != NULL) { //error -999 happens when the user clicks on something before it's done loading.
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Network Error" message:@"Unable to load the page. Please keep network connection."
                                                       delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        
        [alert show];
        [alert release];
    }
    
}

-(void) backClicked:(id)sender {
    //    CATransition *animation = [CATransition animation];
    //    animation.delegate = self;
    //    animation.duration = 0.5f;
    //    animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
    //	animation.fillMode = kCAFillModeForwards;
    //    animation.type = kCATransitionMoveIn;
    //    animation.subtype = kCATransitionFromBottom;
    //    [mView.layer addAnimation:animation forKey:@"animation"];
    
    CGPoint initialCenter = mView.center;
    [UIView beginAnimations:@"animation" context:NULL];
    [UIView setAnimationDuration:0.5f];
    [UIView setAnimationDelegate:self];
    mView.center = CGPointMake(initialCenter.x, initialCenter.y + 480); // to go down
    [UIView setAnimationDidStopSelector:@selector(aMethodToBeCalledAfterAnimationEnd:finished:context:)];
    [UIView commitAnimations];
}

- (void)aMethodToBeCalledAfterAnimationEnd:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context{
    
    mWebView.delegate = nil;
    [mToolbar removeFromSuperview];
    [mWebView removeFromSuperview];
    [mView removeFromSuperview];
    mLayerWebView->onBackbuttonClick();
}

- (void)animationDidStop:(CAAnimation *)theAnimation finished:(BOOL)flag
{
    mWebView.delegate = nil;
    [mToolbar removeFromSuperview];
    [mWebView removeFromSuperview];
    [mView removeFromSuperview];
    mLayerWebView->onBackbuttonClick();
}

@end
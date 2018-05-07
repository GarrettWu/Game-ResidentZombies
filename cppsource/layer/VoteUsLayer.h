#ifndef _VoteUsLayer_h
#define _VoteUsLayer_h

#include "WiEngine.h"
#include "Global.h"
#include "LevelSelectLayer.h"

static const char * RATEME_PLAYTIMES = "rateme_playtimes";
static const char * RATEME_NEVER = "rateme_never";
static const char * RATEME_LATER = "rateme_later";
static const char * RATEME_YES = "rateme_yes";
static const char * RATEME_TIMES = "rateme_times";

class VoteUsLayer : public wyLayer {
public:
    wyNode *container;
    int parentType, prevId;
    
    ~VoteUsLayer() {
        
    }
    
    void onEnter();
    
    VoteUsLayer();
    
    void onYesButtonClick(wyTargetSelector* ts);
    void onLaterButtonClick(wyTargetSelector* ts);
    void onNeverButtonClick(wyTargetSelector* ts);
    
    void onCloseButtonClick(wyTargetSelector* ts);
    bool touchesBegan(wyMotionEvent& event);
    bool touchesMoved(wyMotionEvent& event);
    bool touchesEnded(wyMotionEvent& e);
    wyButton *yesBtn, *laterBtn,*neverBtn;
    bool isEnterAnimEnd;
    
    //弹出VoteUsLayer页面
    static void showVoteUsLayer(wyNode *pa){
        if (wyPrefs::getInt(RATEME_NEVER, 0) == 1) {
            LOGE("never rate me!!!");
            return;
        }
        
        if (wyPrefs::getInt(RATEME_YES, 0) == 1) {
            LOGE("already rated!!!");
            return;
        }
        
        if (wyPrefs::getInt(RATEME_LATER, 0) == 1) {
            LOGE("50%% chance to rate...");
            if (rand()%100 < 30) {
                wyLayer *layer = (wyLayer*) (new VoteUsLayer())->autoRelease();
                pa->addChildLocked(layer, 101, 0);
            }
        } else {
            int playTimes = wyPrefs::getInt(RATEME_PLAYTIMES);
            if (playTimes >= 5) {
                wyLayer *layer = (wyLayer*) (new VoteUsLayer())->autoRelease();
                pa->addChildLocked(layer, 101, 0);
            } else {
                wyPrefs::setInt(RATEME_PLAYTIMES, playTimes);
            }
        }
    }
    
    static void showThanksLayer(wyNode *parent) {
        PopupDialog *popupDialog = new PopupDialog();
        popupDialog->autoRelease();
        popupDialog->showOnlyOkBtn();
        popupDialog->setContent("thanks for your appreciation! we'll do the best to make this game better!");
        parent->addChildLocked(popupDialog, 102, 0);
    }
    
    static void updatePlayTimes() {
        int playTimes = wyPrefs::getInt(RATEME_PLAYTIMES);
        wyPrefs::setInt(RATEME_PLAYTIMES, MIN(playTimes+1, 5));
        LOGE("playtimes:%d", wyPrefs::getInt(RATEME_PLAYTIMES));
    }
};

#endif




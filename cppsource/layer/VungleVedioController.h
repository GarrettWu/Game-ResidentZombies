#ifndef _VungleVedioController_h
#define _VungleVedioController_h

#include "WiEngine.h"
#include "Global.h"

class VungleVedioController : public wyLayer {
public:
    ~VungleVedioController() {
        
    }
    
    void onEnter();
    
    VungleVedioController();

    static int playTimes;
    static int period;
    static int completeTimes;
    
    static void init(){
        playTimes = 0;
        period = 0;
        completeTimes = 0;
        wyPrefs::setInt("PLAY_VUNGLE", 0);
    }
    
    static void updatePlayTimes() {
        playTimes++;
    }
    
    static void playVedioWhenLevelComplete(){
#if ANDROID
        //付费用户直接跳过
        if (sGlobal->isAdsRemoved) {
            return;
        }
        
        if (wyPrefs::getInt("PLAY_VUNGLE", 0) == 1) {
            playVungleVedio();
            return;
        }
        
        completeTimes++;
        if (period == 0) {
            if (playTimes >= 3) {
                playVungleVedio();
                
                playTimes = 0;
                period = 1;
                completeTimes = 0;
                
                wyPrefs::setInt("PLAY_VUNGLE", 1);
            }
        }
        else if (period == 1) {
            if (playTimes >= 5 && completeTimes >= 2) {
                playVungleVedio();
                
                playTimes = 0;
                period = 2;
                
                wyPrefs::setInt("PLAY_VUNGLE", 1);
            }
        }
#endif
    }
};

#endif




#ifndef _DailyRewardLayer_h
#define _DailyRewardLayer_h

#include "WiEngine.h"
#include "Global.h"
#include <sys/time.h>

typedef enum{
    BG,
    REWARD_LABEL,
    DAY_LABEL
}Reward_Tag;

static long long getCurrentTimeForDailyReward(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long long currentTime=tv.tv_sec*1000+tv.tv_usec/1000;
    
    return currentTime;
}

static wyColor3B grayColor = wyc3b(142, 142, 142);
static wyColor3B orangeColor = wyc3b(198, 65, 33);

static long long aDay = 24*60*60*1000;
static long long tDay = 48*60*60*1000;

//for test
//static long aDay = 5*1000;
//static long tDay = 10*1000;

class DailyRewardLayer : public wyLayer {
public:
    wyNode *container;
    int parentType, prevId;
    
    ~DailyRewardLayer() {
        
    }
    
    void onEnter();
    
    DailyRewardLayer(bool isDay5Reached, bool isPlaced = false);
    
    void onCloseButtonClick(wyTargetSelector* ts);
    
    bool touchesBegan(wyMotionEvent& event);
    bool touchesMoved(wyMotionEvent& event);
    bool touchesEnded(wyMotionEvent& e);
    ControlButton *okBtn, *cancelBtn;
    bool isEnterAnimEnd;
    void showParticle(wySpriteEx *sprite);
    wySpriteEx* getRewardSprite(int dayIndex, bool isUnlock);
    void runUnlockRewardEffect(wyTargetSelector *ts);
    int currentLoginDay;
    
    bool isPlaced;
    
    wyBitmapFontLabel *countDownLabel;
    void updateCountDown(wyTargetSelector *ts);
    long long lastPlayTime;
    void getRealReward();
    bool isFromMenu;
    
    void showNextDay(wyTargetSelector *ts, int dayIndex);
    
    static bool loginDay(wyNode *pa){
        bool result = false;

        bool isDay5Reached = false;
        
        long long now=getCurrentTimeForDailyReward();
        LOGE("now:%lld, last:%lld, sub:%lld", now, wyPrefs::getInt64("lastTime", 0), (now-wyPrefs::getInt64("lastTime", 0))/1000);
        if(!wyPrefs::getInt64("lastTime")){
            wyPrefs::setInt64("lastTime", now);
            wyPrefs::setInt("loginDay",1);
            
            DailyRewardLayer *layer = new DailyRewardLayer(isDay5Reached);
            layer->autoRelease();
            pa->addChildLocked(layer, 101, 0);
            
            result = true;
        }else{
            long long sub=now-wyPrefs::getInt64("lastTime");
            if (sub < 0) {
                wyPrefs::setInt("loginDay",1);
                wyPrefs::setInt64("lastTime", now);
            }
            //86400000
            if(sub>aDay&&sub<=tDay){
                int nextDay = wyPrefs::getInt("loginDay")+1;
                if (nextDay > 5) {
                    nextDay = 5;
                    isDay5Reached = true;
                } else {
                    isDay5Reached = false;
                }
                
                wyPrefs::setInt("loginDay", nextDay);
                wyPrefs::setInt64("lastTime", now);
                
                DailyRewardLayer *layer = new DailyRewardLayer(isDay5Reached);
                layer->autoRelease();
                pa->addChildLocked(layer, 101, 0);
                
                result = true;
            }
            else if (sub>tDay) {
                wyPrefs::setInt("loginDay",1);
                wyPrefs::setInt64("lastTime", now);
                
                DailyRewardLayer *layer = new DailyRewardLayer(false);
                layer->autoRelease();
                pa->addChildLocked(layer, 101, 0);
                
                result = true;
            }
        }
        return result;
    }
};


#endif


#include "com_wiyun_engine_skeleton_Skeleton.h"
#include "WiEngine.h"
#include "LogoScene.h"
#include "Global.h"
#include "JniHelper.h"

JNIEXPORT void JNICALL Java_com_wiyun_engine_skeleton_Skeleton_nativeStart
(JNIEnv * env, jobject thiz) {
	sGlobal->init();
	//sGlobal->initGame();
    
	wyDirector* director = wyDirector::getInstance();
	director->setShowFPS(false);
	//director->setMaxFrameRate(40);
	//director->setTickFactor(0.2);
	//director->set2DProjection();
    
	// run with first scene
	LogoScene* scene = new LogoScene();	 
	director->runWithScene(scene);
	scene->release();
}

JNIEXPORT void JNICALL Java_com_wiyun_engine_skeleton_Skeleton_addGold
(JNIEnv * env, jobject thiz, jint goldnum) {
  	int gold = goldnum;
  	//LOGE("add gold:%d", gold);
	sGlobal->addGold(gold);
}

JNIEXPORT void JNICALL Java_com_wiyun_engine_skeleton_Skeleton_addGem
(JNIEnv * env, jobject thiz, jint gemnum) {
  	int gem = gemnum;
  	//LOGE("add gold:%d", gold);
	sGlobal->addGem(gem);
}

JNIEXPORT void JNICALL Java_com_wiyun_engine_skeleton_Skeleton_purchaseItem
(JNIEnv * env, jobject thiz, jint itemId) {
  	int item = itemId;
	sGlobal->purchaseGameItemEnd(item);
}

JNIEXPORT void JNICALL Java_com_wiyun_engine_skeleton_Skeleton_replayBGM
(JNIEnv * env, jobject thiz, jint itemId) {
	sGlobal->replayBGM();
}

JNIEXPORT void JNICALL Java_com_wiyun_engine_skeleton_Skeleton_onResumeGame
(JNIEnv * env, jobject thiz) {
//	sGlobal->loadAllImages(true);
    sGlobal->loadAllImagesAsync();
}

JNIEXPORT int JNICALL Java_com_wiyun_engine_skeleton_Skeleton_isShopOpening
(JNIEnv * env, jobject thiz) {
	if (sGlobal->isShopOpening) {
        return 1;
    } else {
        return 0;
    }
}

JNIEXPORT void JNICALL Java_com_wiyun_engine_skeleton_Skeleton_setShopNeedReloadTex
(JNIEnv * env, jobject thiz) {
	sGlobal->needReloadTex = true;
}

//JNIEXPORT int JNICALL Java_com_wiyun_engine_skeleton_Skeleton_checkPackageNameJNI
//(JNIEnv * env, jobject thiz) {
////    jclass android_content_Context = env->GetObjectClass(env, thiz);  //or use FindClass
////    jmethodID midGetPackageName = env->GetMethodID(env,  android_content_Context, "getPackageName", "()Ljava/lang/String");
////    jstring packageName = env->CallObjectMethod(env, thiz, midGetPackageName);
//    
//    JniMethodInfo methodInfo;
//    if (! JniHelper::getMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "getGamePackageName",
//                                         "()Ljava/lang/String")) {
//        return 0;
//    }
//    jstring packageName = (jstring)methodInfo.env->CallObjectMethod(methodInfo.classID, methodInfo.methodID);
//    methodInfo.env->DeleteLocalRef(methodInfo.classID);
//    
//    const char *pName = env->GetStringUTFChars(packageName, 0);
//    int result = 0;
//    if (strcmp(pName, "com.cafgame.residentzombies") == 1) {
//        result = 1;
//    }
//    LOGE("result:%d", result);
//    env->ReleaseStringUTFChars(packageName, pName);
////    env->DeleteLocalRef(packageName);
//}

void showAd(int type) {
	if (sGlobal->isAdsRemoved == 1)
		return;
    
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "showAd", 
                                         "(I)V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, type);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void hideAd() {
	if (sGlobal->isAdsRemoved == 1)
		return;
		
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "hideAd", 
                                         "()V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void showExit() {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "showExit", 
                                         "()V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void showOfferWall(int type) {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "showOfferWall", 
                                         "(I)V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, type);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void reportEvent(const char* event_id, const char* label) {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "reportEvent", 
                                         "(Ljava/lang/String;Ljava/lang/String;)V")) 
    {
        return;
    }
    jstring jevent_id = methodInfo.env->NewStringUTF(event_id);
    jstring jlabel = methodInfo.env->NewStringUTF(label);
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jevent_id, jlabel);
    methodInfo.env->DeleteLocalRef(jevent_id);
    methodInfo.env->DeleteLocalRef(jlabel);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void startPurchaseGameItem(int itemId) {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "startPurchaseGameItem", 
                                         "(I)V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, itemId);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void showWebUrl(const char *url) {
	JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "showWebUrl", 
                                         "(Ljava/lang/String;)V")) 
    {
        return;
    }
    jstring jurl = methodInfo.env->NewStringUTF(url);
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jurl);
    methodInfo.env->DeleteLocalRef(jurl);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void showChartBoost() {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "showChartBoost", 
                                         "()V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void callGC() {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "callGC", 
                                         "()V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void reportGaEvent(const char* category, const char* action, const char *label, int value) {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "reportGaEvent",
                                         "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V"))
    {
        return;
    }
    jstring jcategoy = methodInfo.env->NewStringUTF(category);
    jstring jaction = methodInfo.env->NewStringUTF(action);
    jstring jlabel = methodInfo.env->NewStringUTF(label);

    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jcategoy, jaction, jlabel, value);
    methodInfo.env->DeleteLocalRef(jcategoy);
    methodInfo.env->DeleteLocalRef(jaction);
    methodInfo.env->DeleteLocalRef(jlabel);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void showLeaderBoard() {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "showLeaderBoard",
                                         "()V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void submitScore(int score) {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "submitScoreToLeaderBoard",
                                         "(I)V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, score);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void submitScoreAndShowLB(int score) {
//    LOGE("aaa, submit score:%d", score);
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "submitScoreAndShowLB",
                                         "(I)V")) {
        return;
    }
//    jvalue jv;
//    jv.j = score;
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, score);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void onResumeGameEnd() {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "onResumeGameEnd",
                                         "()V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void showUIProgress() {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "showUIProgress",
                                         "()V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void hideUIProgress() {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "hideUIProgress",
                                         "()V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

int checkGamePackageName() {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "checkGamePackageName",
                                         "()I")) {
        return 0;
    }
    int ret = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
    LOGE("ret:%d", ret);
    return ret;
}

int checkSignature() {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "checkSignature",
                                         "()I")) {
        return 0;
    }
    int ret = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
    LOGE("ret:%d", ret);
    return ret;
}

void showRateMeGoogle() {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "showRateMeForGooglePlay",
                                         "()V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void playVungleVedio() {
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/wiyun/engine/skeleton/Skeleton", "playVungleVedio",
                                         "()V")) {
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}












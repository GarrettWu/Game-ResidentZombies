LOCAL_PATH := $(call my-dir)

###########################################################
## Find all of files under the named directories.
###########################################################



#################################################################################
# build wiengineskeleton lib
#################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := wiengineskeleton

LOCAL_CFLAGS += -Wno-psabi

LOCAL_LDLIBS := -L$(LOCAL_PATH)/../../WiEngine_library/libs/$(TARGET_ARCH_ABI) -llog -lGLESv1_CM  -lwiengine -lwisound -lbox2d -ldl

LOCAL_CPPFLAGS += -fexceptions
LOCAL_C_INCLUDES := $(LOCAL_PATH) \
	$(LOCAL_PATH)/../../WiEngine_library/jni \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/actions \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/afcanim \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/astar \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/bitmapfont \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/box2d \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/common \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/dialog \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/easeactions \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/events \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/filters \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/grid \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/gridactions \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/json \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/mfs \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/nodes \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/opengl \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/particle \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/persistence \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/tmx \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/transitions \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/types \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/winetwork \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/wisound \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/utils \
	$(LOCAL_PATH)/../../WiEngine_library/jni/WiEngine/include/skeletalanim \
	$(LOCAL_PATH)/../../WiEngine_library/jni/yajl/include \
	$(LOCAL_PATH)/../../WiEngine_library/jni/libxml2/include \
	$(LOCAL_PATH)/../../../cppsource/custom \
	$(LOCAL_PATH)/../../../cppsource/factory \
	$(LOCAL_PATH)/../../../cppsource/layer \
	$(LOCAL_PATH)/../../../cppsource/objects \
	$(LOCAL_PATH)/../../../cppsource/scene \
	$(LOCAL_PATH)/../../../cppsource/utils \
	$(LOCAL_PATH)/../../../cppsource \

LOCAL_SRC_FILES := ./com_wiyun_engine_skeleton_Skeleton.cpp \
../../../cppsource/custom/ArmsPageControl.cpp \
../../../cppsource/custom/MyDotPageIndicator.cpp \
../../../cppsource/custom/ControlButton.cpp \
../../../cppsource/custom/MyContactListener.cpp \
../../../cppsource/custom/ShPageControl.cpp \
../../../cppsource/factory/EnemyFactory.cpp \
../../../cppsource/factory/ObjectsFactory.cpp \
../../../cppsource/layer/VungleVedioController.cpp \
../../../cppsource/layer/DailyRewardLayer.cpp \
../../../cppsource/layer/VoteUsLayer.cpp \
../../../cppsource/layer/QuestLayer.cpp \
../../../cppsource/layer/DeadNoticeLayer.cpp \
../../../cppsource/layer/DialogBgLayer.cpp \
../../../cppsource/layer/EffectLayer.cpp \
../../../cppsource/layer/GameBackgroundLayer.cpp \
../../../cppsource/layer/GameCompleteLayer.cpp \
../../../cppsource/layer/HudLayer.cpp \
../../../cppsource/layer/LevelSelectLayer.cpp \
../../../cppsource/layer/MainGameLayer.cpp \
../../../cppsource/layer/OptionLayer.cpp \
../../../cppsource/layer/ShopLayer.cpp \
../../../cppsource/layer/AllMenusLayer.cpp \
../../../cppsource/layer/FreeStuffLayer.cpp \
../../../cppsource/layer/MissionsLayer.cpp \
../../../cppsource/layer/SettingsLayer.cpp \
../../../cppsource/layer/StatisticsLayer.cpp \
../../../cppsource/layer/BaseMenuLayer.cpp \
../../../cppsource/objects/Barricade.cpp \
../../../cppsource/objects/BaseObject.cpp \
../../../cppsource/objects/WolfZombie.cpp \
../../../cppsource/objects/Bonus.cpp \
../../../cppsource/objects/Boss1.cpp \
../../../cppsource/objects/Boss2.cpp \
../../../cppsource/objects/Boss3.cpp \
../../../cppsource/objects/Box.cpp \
../../../cppsource/objects/Deco.cpp \
../../../cppsource/objects/Doctor1.cpp \
../../../cppsource/objects/Doctor2.cpp \
../../../cppsource/objects/FemaleZombie.cpp \
../../../cppsource/objects/EndPoint.cpp \
../../../cppsource/objects/Enemy.cpp \
../../../cppsource/objects/Hero.cpp \
../../../cppsource/objects/HeroBullet.cpp \
../../../cppsource/objects/Intro.cpp \
../../../cppsource/objects/NPC.cpp \
../../../cppsource/objects/SpittleZombie.cpp \
../../../cppsource/objects/FlyZombie1.cpp \
../../../cppsource/objects/FatZombie.cpp \
../../../cppsource/objects/Devourer.cpp \
../../../cppsource/objects/MaleZombie.cpp \
../../../cppsource/objects/MuscleZombie.cpp \
../../../cppsource/objects/FlyZombie2.cpp \
../../../cppsource/objects/MuscleZombie1.cpp \
../../../cppsource/objects/GhostBaby.cpp \
../../../cppsource/objects/FlyZombie3.cpp \
../../../cppsource/objects/Trap.cpp \
../../../cppsource/objects/XuanKongBiao.cpp \
../../../cppsource/scene/WorldMapScene.cpp \
../../../cppsource/scene/UpgradeScene.cpp \
../../../cppsource/scene/CGScene.cpp \
../../../cppsource/scene/LoadingScene.cpp \
../../../cppsource/scene/LogoScene.cpp \
../../../cppsource/scene/MainScene.cpp \
../../../cppsource/utils/MyTextureManager.cpp \
../../../cppsource/utils/Global.cpp \
../../../cppsource/utils/mtbase64.cpp \
./JniHelper.cpp \

include $(BUILD_SHARED_LIBRARY)

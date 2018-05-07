# set params
ANDROID_NDK_ROOT=/Users/sharyu/Documents/android/android-ndk-r9b
WI_ENGINE_ROOT=/Users/sharyu/Documents/work/zendroid_svn/ResidentZombies/ResidentZombiesV1.0/branches/residentzombies_ft_1.3/code/android
GAME_ROOT=$WI_ENGINE_ROOT/ResidentZombies_Android_feelingtouch
LIBRARY_ROOT=$WI_ENGINE_ROOT/WiEngine_library/libs

# ndk-build
$ANDROID_NDK_ROOT/ndk-build -C $GAME_ROOT $*

# copy libs
cp $LIBRARY_ROOT/armeabi/libbox2d.so $GAME_ROOT/libs/armeabi/
cp $LIBRARY_ROOT/armeabi/libwiengine.so $GAME_ROOT/libs/armeabi/
cp $LIBRARY_ROOT/armeabi/libwinetwork.so $GAME_ROOT/libs/armeabi/
cp $LIBRARY_ROOT/armeabi/libwisound.so $GAME_ROOT/libs/armeabi/

#cp $LIBRARY_ROOT/armeabi/libbox2d.so $GAME_ROOT/libs/armeabi-v7a/
#cp $LIBRARY_ROOT/armeabi/libwiengine.so $GAME_ROOT/libs/armeabi-v7a/
#cp $LIBRARY_ROOT/armeabi/libwinetwork.so $GAME_ROOT/libs/armeabi-v7a/
#cp $LIBRARY_ROOT/armeabi/libwisound.so $GAME_ROOT/libs/armeabi-v7a/

#cp $LIBRARY_ROOT/armeabi/libbox2d.so $GAME_ROOT/libs/x86/
#cp $LIBRARY_ROOT/armeabi/libwiengine.so $GAME_ROOT/libs/x86/
#cp $LIBRARY_ROOT/armeabi/libwinetwork.so $GAME_ROOT/libs/x86/
#cp $LIBRARY_ROOT/armeabi/libwisound.so $GAME_ROOT/libs/x86/

#cp $LIBRARY_ROOT/armeabi/libbox2d.so $GAME_ROOT/libs/mips/
#cp $LIBRARY_ROOT/armeabi/libwiengine.so $GAME_ROOT/libs/mips/
#cp $LIBRARY_ROOT/armeabi/libwinetwork.so $GAME_ROOT/libs/mips/
#cp $LIBRARY_ROOT/armeabi/libwisound.so $GAME_ROOT/libs/mips/


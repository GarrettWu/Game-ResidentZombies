# Build all arch ndk supported
APP_ABI := armeabi

# enable rtti
APP_STL := gnustl_static
APP_CPPFLAGS := -frtti
APP_OPTIM := debug
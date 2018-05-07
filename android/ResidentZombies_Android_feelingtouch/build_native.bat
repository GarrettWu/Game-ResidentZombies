:: set params
set ANDROID_NDK_ROOT=D:\Android\android-ndk-r8
set WI_ENGINE_ROOT=F:\mt_svn_co\branches\zombie
set GAME_ROOT="%WI_ENGINE_ROOT%"\ResidentZombies_Android_feelingtouch
set LIBRARY_ROOT="%WI_ENGINE_ROOT%"\WiEngine_library\libs

:: invoke ndk-build
call "%ANDROID_NDK_ROOT%"\ndk-build

:: copy libs
copy /y "%LIBRARY_ROOT%"\armeabi\libbox2d.so "%GAME_ROOT%"\libs\armeabi\
copy /y "%LIBRARY_ROOT%"\armeabi\libwiengine.so "%GAME_ROOT%"\libs\armeabi\
copy /y "%LIBRARY_ROOT%"\armeabi\libwinetwork.so "%GAME_ROOT%"\libs\armeabi\
copy /y "%LIBRARY_ROOT%"\armeabi\libwisound.so "%GAME_ROOT%"\libs\armeabi\

::copy /y "%LIBRARY_ROOT%"\armeabi-v7a\libaal_honeycomb.so "%GAME_ROOT%"\libs\armeabi-v7a\
::copy /y "%LIBRARY_ROOT%"\armeabi-v7a\libaal_jellybean.so "%GAME_ROOT%"\libs\armeabi-v7a\
::copy /y "%LIBRARY_ROOT%"\armeabi-v7a\libaal.so "%GAME_ROOT%"\libs\armeabi-v7a\
::copy /y "%LIBRARY_ROOT%"\armeabi-v7a\libbox2d.so "%GAME_ROOT%"\libs\armeabi-v7a\
::copy /y "%LIBRARY_ROOT%"\armeabi-v7a\libjson.so "%GAME_ROOT%"\libs\armeabi-v7a\
::copy /y "%LIBRARY_ROOT%"\armeabi-v7a\libwiengine.so "%GAME_ROOT%"\libs\armeabi-v7a\
::copy /y "%LIBRARY_ROOT%"\armeabi-v7a\libwinetwork.so "%GAME_ROOT%"\libs\armeabi-v7a\
::copy /y "%LIBRARY_ROOT%"\armeabi-v7a\libwiskia.so "%GAME_ROOT%"\libs\armeabi-v7a\
::copy /y "%LIBRARY_ROOT%"\armeabi-v7a\libwisound.so "%GAME_ROOT%"\libs\armeabi-v7a\
::copy /y "%LIBRARY_ROOT%"\armeabi-v7a\libxml2.so "%GAME_ROOT%"\libs\armeabi-v7a\

::copy /y "%LIBRARY_ROOT%"\mips\libaal_honeycomb.so "%GAME_ROOT%"\libs\mips\
::copy /y "%LIBRARY_ROOT%"\mips\libaal.so "%GAME_ROOT%"\libs\mips\
::copy /y "%LIBRARY_ROOT%"\mips\libbox2d.so "%GAME_ROOT%"\libs\mips\
::copy /y "%LIBRARY_ROOT%"\mips\libjson.so "%GAME_ROOT%"\libs\mips\
::copy /y "%LIBRARY_ROOT%"\mips\libwiengine.so "%GAME_ROOT%"\libs\mips\
::copy /y "%LIBRARY_ROOT%"\mips\libwinetwork.so "%GAME_ROOT%"\libs\mips\
::copy /y "%LIBRARY_ROOT%"\mips\libwiskia.so "%GAME_ROOT%"\libs\mips\
::copy /y "%LIBRARY_ROOT%"\mips\libwisound.so "%GAME_ROOT%"\libs\mips\
::copy /y "%LIBRARY_ROOT%"\mips\libxml2.so "%GAME_ROOT%"\libs\mips\

::copy /y "%LIBRARY_ROOT%"\x86\libaal_honeycomb.so "%GAME_ROOT%"\libs\x86\
::copy /y "%LIBRARY_ROOT%"\x86\libaal.so "%GAME_ROOT%"\libs\x86\
::copy /y "%LIBRARY_ROOT%"\x86\libbox2d.so "%GAME_ROOT%"\libs\x86\
::copy /y "%LIBRARY_ROOT%"\x86\libjson.so "%GAME_ROOT%"\libs\x86\
::copy /y "%LIBRARY_ROOT%"\x86\libwiengine.so "%GAME_ROOT%"\libs\x86\
::copy /y "%LIBRARY_ROOT%"\x86\libwinetwork.so "%GAME_ROOT%"\libs\x86\
::copy /y "%LIBRARY_ROOT%"\x86\libwiskia.so "%GAME_ROOT%"\libs\x86\
::copy /y "%LIBRARY_ROOT%"\x86\libwisound.so "%GAME_ROOT%"\libs\x86\
::copy /y "%LIBRARY_ROOT%"\x86\libxml2.so "%GAME_ROOT%"\libs\x86\
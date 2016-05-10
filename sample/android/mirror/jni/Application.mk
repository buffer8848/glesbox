APP_PLATFORM := android-18
NDK_TOOLCHAIN_VERSION := 4.8
APP_ABI := armeabi-v7a x86
APP_STL := gnustl_static
#APP_OPTIM := debug
APP_OPTIM := release

LOCAL_CPPFLAGS 		:= -O3 -fno-rtti -fno-exceptions -finline-functions -funswitch-loops -fgcse-after-reload -pipe -flto -std=c++11
#LOCAL_CPPFLAGS 	:= -O0 -g -frtti -fexceptions -finline-functions -funswitch-loops -fgcse-after-reload -pipe -std=c++11
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
	LOCAL_CPPFLAGS 	+= -mfpu=neon
endif
LOCAL_CPPFLAGS 		+= -D_ANDROID

LOCAL_PATH			:= $(call my-dir)

ROOT_INCLUDE := ../../../
ROOT_PATH := ../../../../

include $(CLEAR_VARS)
LOCAL_MODULE 	:= libyuv
LOCAL_SRC_FILES := $(ROOT_PATH)/tools/libyuv/lib/android/$(TARGET_ARCH_ABI)/libyuv.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE 	:= libgb
LOCAL_SRC_FILES := $(ROOT_PATH)/libs/$(TARGET_ARCH_ABI)/libgb_static.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    	:= mirror

LOCAL_C_INCLUDES 	:= $(ROOT_INCLUDE)/include/
LOCAL_C_INCLUDES 	+= $(ROOT_INCLUDE)/src/
LOCAL_C_INCLUDES 	+= $(ROOT_INCLUDE)/tools/libyuv/include/

LOCAL_SRC_FILES 	+= native.cpp

LOCAL_STATIC_LIBRARIES := libgb
LOCAL_STATIC_LIBRARIES += libyuv

LOCAL_CPPFLAGS := -std=c++11
LOCAL_LDLIBS := -lz -llog -lEGL -ljnigraphics -lGLESv2 -landroid
include $(BUILD_SHARED_LIBRARY)

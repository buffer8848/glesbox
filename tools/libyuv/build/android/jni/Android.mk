
LOCAL_PATH := $(call my-dir)

common_INCLUDE_PATH := ../../
common_SRC_PATH := ../../../

common_SRC_FILES := \
	$(common_SRC_PATH)/src/compare.cc                           \
	$(common_SRC_PATH)/src/compare_common.cc                    \
	$(common_SRC_PATH)/src/compare_gcc.cc                    	\
	$(common_SRC_PATH)/src/compare_neon.cc                      \
	$(common_SRC_PATH)/src/compare_neon64.cc                    \
	$(common_SRC_PATH)/src/compare_win.cc                       \
	$(common_SRC_PATH)/src/convert.cc                           \
	$(common_SRC_PATH)/src/convert_argb.cc                      \
	$(common_SRC_PATH)/src/convert_from.cc                      \
	$(common_SRC_PATH)/src/convert_from_argb.cc                 \
	$(common_SRC_PATH)/src/convert_jpeg.cc                      \
	$(common_SRC_PATH)/src/convert_to_argb.cc                   \
	$(common_SRC_PATH)/src/convert_to_i420.cc                   \
	$(common_SRC_PATH)/src/cpu_id.cc                            \
	$(common_SRC_PATH)/src/mjpeg_decoder.cc                     \
	$(common_SRC_PATH)/src/mjpeg_validate.cc                    \
	$(common_SRC_PATH)/src/planar_functions.cc                  \
	$(common_SRC_PATH)/src/rotate.cc                            \
	$(common_SRC_PATH)/src/rotate_argb.cc                       \
	$(common_SRC_PATH)/src/rotate_mips.cc                       \
	$(common_SRC_PATH)/src/rotate_neon.cc                       \
	$(common_SRC_PATH)/src/rotate_neon64.cc                     \
	$(common_SRC_PATH)/src/row_any.cc                           \
	$(common_SRC_PATH)/src/row_common.cc                        \
	$(common_SRC_PATH)/src/row_gcc.cc                        	\
	$(common_SRC_PATH)/src/row_mips.cc                          \
	$(common_SRC_PATH)/src/row_neon.cc                          \
	$(common_SRC_PATH)/src/row_neon64.cc                        \
	$(common_SRC_PATH)/src/row_win.cc                           \
	$(common_SRC_PATH)/src/scale.cc                             \
	$(common_SRC_PATH)/src/scale_any.cc                        	\
	$(common_SRC_PATH)/src/scale_argb.cc                        \
	$(common_SRC_PATH)/src/scale_common.cc                      \
	$(common_SRC_PATH)/src/scale_gcc.cc                        	\
	$(common_SRC_PATH)/src/scale_mips.cc                        \
	$(common_SRC_PATH)/src/scale_neon.cc                        \
	$(common_SRC_PATH)/src/scale_neon64.cc                      \
	$(common_SRC_PATH)/src/scale_win.cc                         \
	$(common_SRC_PATH)/src/video_common.cc

common_CFLAGS := -fexceptions
common_LDFLAGS :=

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
	common_CFLAGS += -march=armv7-a -mfloat-abi=softfp -mfpu=neon
	common_LDFLAGS += -Wl,--fix-cortex-a8
endif


common_C_INCLUDES = $(common_INCLUDE_PATH)/include

include $(CLEAR_VARS)
LOCAL_MODULE:= libyuv
LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES := $(common_SRC_FILES)
LOCAL_CFLAGS += $(common_CFLAGS)
LOCAL_LDFLAGS += $(common_LDFLAGS)
LOCAL_C_INCLUDES += $(common_C_INCLUDES)

include $(BUILD_STATIC_LIBRARY)

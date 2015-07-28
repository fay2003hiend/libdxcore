LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := dxcore
LOCAL_LDLIBS := -llog -lz
LOCAL_SRC_FILES := dxcore.cpp \
	sha.c \
	ioapi.c \
	unzip.c
include $(BUILD_SHARED_LIBRARY)

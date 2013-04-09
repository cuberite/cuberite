LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := mcserver



LOCAL_SRC_FILES := $(shell find ../CryptoPP ../lua-5.1.4 ../jsoncpp-src-0.5.0 ../zlib-1.2.7 ../source ../squirrel_3_0_1_stable ../tolua++-1.0.93 ../iniFile ../WebServer ../expat '(' -name '*.cpp' -o -name '*.c' ')')
LOCAL_SRC_FILES := $(filter-out %SquirrelFunctions.cpp %SquirrelBindings.cpp %cPlugin_Squirrel.cpp %cSquirrelCommandBinder.cpp %minigzip.c %lua.c %tolua.c %toluabind.c %LeakFinder.cpp %StackWalker.cpp %example.c,$(LOCAL_SRC_FILES))
LOCAL_SRC_FILES := $(patsubst %.cpp,../%.cpp,$(LOCAL_SRC_FILES))
LOCAL_SRC_FILES := $(patsubst %.c,../%.c,$(LOCAL_SRC_FILES))
LOCAL_SRC_FILES += app-android.cpp ToJava.cpp

LOCAL_CFLAGS := -DANDROID_NDK \
				-O3 \
				-funroll-loops \
				-mfloat-abi=softfp -mfpu=neon \
				-fexceptions \


LOCAL_STATIC_LIBRARIES := cpufeatures

LOCAL_C_INCLUDES := ../source \
					../source/md5 \
					../WebServer \
					../source/packets \
					../source/items \
					../source/blocks \
					../tolua++-1.0.93/src/lib \
					../lua-5.1.4/src \
					../zlib-1.2.7 \
					../iniFile \
					../tolua++-1.0.93/include \
					../jsoncpp-src-0.5.0/include \
					../jsoncpp-src-0.5.0/src/lib_json \
					../squirrel_3_0_1_stable/include \
					../squirrel_3_0_1_stable \
					../squirrel_3_0_1_stable/sqrat \
					../expat/ \
					.. \


LOCAL_LDLIBS := -ldl -llog

include $(BUILD_SHARED_LIBRARY)
$(call import-module,cpufeatures)

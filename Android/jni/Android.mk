LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := mcserver



LOCAL_SRC_FILES := $(shell find ../CryptoPP ../lua ../jsoncpp ../zlib ../src ../tolua++ ../iniFile ../expat ../md5 ../sqlite ../luaexpat '(' -name '*.cpp' -o -name '*.c' ')')
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

LOCAL_C_INCLUDES := ../src \
					../src/packets \
					../src/items \
					../src/blocks \
					../tolua++/src/lib \
					../lua/src \
					../zlib-1.2.7 \
					../iniFile \
					../tolua++/include \
					../jsoncpp/include \
					../jsoncpp/src/lib_json \
					../expat/ \
					../md5/ \
					../sqlite/ \
					../luaexpat/ \
					.. \


LOCAL_LDLIBS := -ldl -llog

include $(BUILD_SHARED_LIBRARY)
$(call import-module,cpufeatures)

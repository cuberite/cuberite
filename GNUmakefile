###################################################
#
# Makefile for MCServer
# Creator: tedik
#
###################################################
#
# Info:
# This makefile is gnu-make spacific, other make systems needn't understand it
# This makefile generates include-file dependencies into *.d files in each build and then reuses these dependencies in the following builds
#
# Usage:
# To make a release build, call "make release=1"
# To make a debug build, call "make"
# To make a 32-bit build on 64-bit OS, pass the addm32=1 flag
# To build with clang, you need to add disableasm=1 flag
#
###################################################

#
# Macros
#

# allow user to override compiler
# if no compiler is specified make specifies cc
ifeq ($(CC),cc)
CC = /usr/bin/g++
endif

all: MCServer/MCServer





###################################################
# Set the variables used for compiling, based on the build mode requested:
# CC_OPTIONS  ... options for the C code compiler
# CXX_OPTIONS ... options for the C++ code compiler
# LNK_OPTIONS ... options for the linker
# LNK_LIBS    ... libraries to link in
#   -- according to http://stackoverflow.com/questions/6183899/undefined-reference-to-dlopen, libs must come after all sources
# BUILDDIR    ... folder where the intermediate object files are built

ifeq ($(release),1)
################
# release build - fastest run-time, no gdb support
################
CC_OPTIONS = -s -g -O3 -DNDEBUG
CXX_OPTIONS = -s -g -O3 -DNDEBUG
LNK_OPTIONS = -pthread -O3
BUILDDIR = build/release/

else
ifeq ($(profile),1)
################
# profile build - a release build with symbols and profiling engine built in
################
CC_OPTIONS = -s -g -ggdb -O3 -pg -DNDEBUG
CXX_OPTIONS = -s -g -ggdb -O3 -pg -DNDEBUG
LNK_OPTIONS = -pthread -ggdb -O3 -pg
BUILDDIR = build/profile/

else
ifeq ($(pedantic),1)
################
# pedantic build - basically a debug build with lots of warnings
################
CC_OPTIONS = -s -g -ggdb -D_DEBUG -Wall -Wextra -pedantic -ansi -Wno-long-long
CXX_OPTIONS = -s -g -ggdb -D_DEBUG -Wall -Wextra -pedantic -ansi -Wno-long-long
LNK_OPTIONS = -pthread -ggdb
BUILDDIR = build/pedantic/

else
################
# debug build - fully traceable by gdb in C++ code, slowest
# Since C code is used only for supporting libraries (zlib, lua), it is still O3-optimized
################
CC_OPTIONS = -s -ggdb -g -D_DEBUG -O3
CXX_OPTIONS = -s -ggdb -g -D_DEBUG
LNK_OPTIONS = -pthread -g -ggdb
BUILDDIR = build/debug/
endif
endif
endif



###################################################
# Set the link libraries based on the OS
# Linux uses libdl
# FreeBSD uses libltdl
# TODO: other OSs?

UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
	LNK_LIBS = -lstdc++ -ldl
else
	LNK_LIBS = -lstdc++ -lltdl
endif





###################################################
# 32-bit build override in 64-bit build environments

ifeq ($(addm32),1)
CC_OPTIONS += -m32
CXX_OPTIONS += -m32
LNK_OPTIONS += -m32
endif






###################################################
# Clang doesn't seem to support CryptoPP's assembly mode, disable  it for now (CryptoPP 5.6.2)

ifeq ($(disableasm),1)
	CC_OPTIONS += -DCRYPTOPP_DISABLE_ASM
	CXX_OPTIONS += -DCRYPTOPP_DISABLE_ASM
endif





###################################################
# INCLUDE directories for MCServer
#

INCLUDE = -I.\
		-Isource\
		-Isource/md5\
		-IWebServer\
		-Isource/items\
		-Isource/blocks\
		-Itolua++-1.0.93/src/lib\
		-Ilua-5.1.4/src\
		-Izlib-1.2.7\
		-IiniFile\
		-Itolua++-1.0.93/include\
		-Ijsoncpp-src-0.5.0/include\
		-Ijsoncpp-src-0.5.0/src/lib_json\
		-Iexpat





###################################################
# Build MCServer
#

# 2012_11_08 _X: Removed: squirrel_3_0_1_stable 

SOURCES := $(shell find CryptoPP lua-5.1.4 jsoncpp-src-0.5.0 zlib-1.2.7 source tolua++-1.0.93 iniFile WebServer expat '(' -name '*.cpp' -o -name '*.c' ')')
SOURCES := $(filter-out %minigzip.c %lua.c %tolua.c %toluabind.c %LeakFinder.cpp %StackWalker.cpp %example.c,$(SOURCES))
OBJECTS := $(patsubst %.c,$(BUILDDIR)%.o,$(SOURCES))
OBJECTS := $(patsubst %.cpp,$(BUILDDIR)%.o,$(OBJECTS))

-include $(patsubst %.o,%.d,$(OBJECTS))

MCServer/MCServer : $(OBJECTS)
	$(CC) $(LNK_OPTIONS) $(OBJECTS) $(LNK_LIBS) -o MCServer/MCServer

clean : 
		rm -rf $(BUILDDIR) MCServer/MCServer

install : MCServer
		cp MCServer MCServer





###################################################
# Build the parts of MCServer
#
# options used:
#  -x c  ... compile as C code
#  -c    ... compile but do not link
#  -MM   ... generate a list of includes

$(BUILDDIR)%.o: %.c
	@mkdir -p $(dir $@) 
	$(CC) $(CC_OPTIONS) -x c -c $(INCLUDE) $< -o $@
	@$(CC) $(CC_OPTIONS) -x c -MM $(INCLUDE) $< > $(patsubst %.o,%.d,$@)
	@mv -f $(patsubst %.o,%.d,$@) $(patsubst %.o,%.d,$@).tmp
	@sed -e "s|.*:|$(BUILDDIR)$*.o:|" < $(patsubst %.o,%.d,$@).tmp > $(patsubst %.o,%.d,$@)
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst %.o,%.d,$@).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst %.o,%.d,$@)
	@rm -f $(patsubst %.o,%.d,$@).tmp

$(BUILDDIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CXX_OPTIONS) -c $(INCLUDE) $< -o $@
	@$(CC) $(CXX_OPTIONS) -MM $(INCLUDE) $< > $(patsubst %.o,%.d,$@)
	@mv -f $(patsubst %.o,%.d,$@) $(patsubst %.o,%.d,$@).tmp
	@sed -e "s|.*:|$(BUILDDIR)$*.o:|" < $(patsubst %.o,%.d,$@).tmp > $(patsubst %.o,%.d,$@)
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst %.o,%.d,$@).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst %.o,%.d,$@)
	@rm -f $(patsubst %.o,%.d,$@).tmp

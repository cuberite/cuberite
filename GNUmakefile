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
#
###################################################

#
# Macros
#

CC = /usr/bin/g++


all: MCServer/MCServer





###################################################
# Set the variables used for compiling, based on the build mode requested:
# CC_OPTIONS  ... options for the C code compiler
# CXX_OPTIONS ... options for the C++ code compiler
# LNK_OPTIONS ... options for the linker
# LNK_LIBS    ... libraries to link in
#   -- according to http://stackoverflow.com/questions/6183899/undefined-reference-to-dlopen, libs must come after all sources
# BUILDDIR    ... folder where the intermediate object files are built

LNK_LIBS = -lstdc++ -ldl

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



################
# 32-bit build override in 64-bit build environments
#   - so that BearBin doesn't need to modify his makefile after each makefile change :)
################
ifeq ($(addm32),1)
CC_OPTIONS += -m32
CXX_OPTIONS += -m32
LNK_OPTIONS += -m32
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


# 2012_11_08 _X: Removed, see Globals.h
#		-Isquirrel_3_0_1_stable/include\
#		-Isquirrel_3_0_1_stable\
#		-Isquirrel_3_0_1_stable/sqrat





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

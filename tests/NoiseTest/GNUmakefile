###################################################
#
# Makefile for NoiseTest
# Creator: xoft
#
###################################################
#
# Usage:
# To make a release build, call "make"
# To make a debug build, call "make debug=1"
#
###################################################

#
# Macros
#

CC = /usr/bin/g++


all: NoiseTest





###################################################
# Set the variables used for compiling, based on the build mode requested:
# CC_OPTIONS  ... options for the C code compiler
# CXX_OPTIONS ... options for the C++ code compiler
# LNK_OPTIONS ... options for the linker
# LNK_LIBS    ... libraries to link in
#   -- according to https://stackoverflow.com/questions/6183899/undefined-reference-to-dlopen, libs must come after all sources
# BUILDDIR    ... folder where the intermediate object files are built

LNK_LIBS = -lstdc++ -ldl

ifeq ($(debug),1)
################
# debug build - fully traceable by gdb in C++ code, slowest
# Since C code is used only for supporting libraries (zlib, lua), it is still O3-optimized
################
CC_OPTIONS = -s -ggdb -g -D_DEBUG -O3
CXX_OPTIONS = -s -ggdb -g -D_DEBUG
LNK_OPTIONS = -pthread -g -ggdb
BUILDDIR = build/debug/

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
# release build - fastest run-time, no gdb support
################
CC_OPTIONS = -s -g -O3 -DNDEBUG
CXX_OPTIONS = -s -g -O3 -DNDEBUG
LNK_OPTIONS = -pthread -O3
BUILDDIR = build/release/
endif
endif
endif





###################################################
# INCLUDE directories
#

INCLUDE = -I.\
		-I../../source\





###################################################
# Build NoiseTest
#

SOURCES = NoiseTest.cpp

SHAREDSOURCES = \
	source/Log.cpp \
	source/MCLogger.cpp \
	source/Noise.cpp \
	source/StringUtils.cpp \
	source/OSSupport/CriticalSection.cpp \
	source/OSSupport/File.cpp \
	source/OSSupport/IsThread.cpp \
	source/OSSupport/MakeDir.cpp \

OBJECTS := $(patsubst %.c,$(BUILDDIR)%.o,$(SOURCES))
OBJECTS := $(patsubst %.cpp,$(BUILDDIR)%.o,$(OBJECTS))

SHAREDOBJECTS := $(patsubst %.c,$(BUILDDIR)%.o,$(SHAREDSOURCES))
SHAREDOBJECTS := $(patsubst %.cpp,$(BUILDDIR)%.o,$(SHAREDOBJECTS))

-include $(patsubst %.o,%.d,$(OBJECTS))
-include $(patsubst %.o,%.d,$(SHAREDOBJECTS))

NoiseTest : $(OBJECTS) $(SHAREDOBJECTS)
	$(CC) $(LNK_OPTIONS) $(OBJECTS) $(SHAREDOBJECTS) $(LNK_LIBS) -o NoiseTest

clean :
		rm -rf $(BUILDDIR) NoiseTest





###################################################
# Build the parts of Cuberite
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

$(BUILDDIR)source/%.o: ../../source/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CXX_OPTIONS) -c $(INCLUDE) $< -o $@
	@$(CC) $(CXX_OPTIONS) -MM $(INCLUDE) $< > $(patsubst %.o,%.d,$@)
	@mv -f $(patsubst %.o,%.d,$@) $(patsubst %.o,%.d,$@).tmp
	@sed -e "s|.*:|$(BUILDDIR)$*.o:|" < $(patsubst %.o,%.d,$@).tmp > $(patsubst %.o,%.d,$@)
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst %.o,%.d,$@).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst %.o,%.d,$@)
	@rm -f $(patsubst %.o,%.d,$@).tmp

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

CC = gcc-4.8


all: NoiseTest





###################################################
# Set the variables used for compiling, based on the build mode requested:
# CC_OPTIONS  ... options for the C code compiler
# CXX_OPTIONS ... options for the C++ code compiler
# LNK_OPTIONS ... options for the linker
# LNK_LIBS    ... libraries to link in
#   -- according to http://stackoverflow.com/questions/6183899/undefined-reference-to-dlopen, libs must come after all sources
# BUILDDIR    ... folder where the intermediate object files are built

LNK_LIBS = -lstdc++ -ldl

ifeq ($(debug),1)
################
# debug build - fully traceable by gdb in C++ code, slowest
# Since C code is used only for supporting libraries (zlib, lua), it is still O3-optimized
################
CC_OPTIONS =  -ggdb -g -D_DEBUG -O3
CXX_OPTIONS = -ggdb -g -D_DEBUG -std=c++11
LNK_OPTIONS = -pthread -g -ggdb
BUILDDIR = build/debug/

else
ifeq ($(profile),1)
################
# profile build - a release build with symbols and profiling engine built in
################
CC_OPTIONS = -g -ggdb -O3 -pg -DNDEBUG
CXX_OPTIONS = -g -ggdb -O3 -pg -DNDEBUG
LNK_OPTIONS = -pthread -ggdb -O3 -pg
BUILDDIR = build/profile/

else
ifeq ($(pedantic),1)
################
# pedantic build - basically a debug build with lots of warnings
################
CC_OPTIONS = -g -ggdb -D_DEBUG -Wall -Wextra -pedantic -ansi -Wno-long-long
CXX_OPTIONS = -g -ggdb -D_DEBUG -Wall -Wextra -pedantic -ansi -Wno-long-long
LNK_OPTIONS = -pthread -ggdb
BUILDDIR = build/pedantic/

else
################
# release build - fastest run-time, no gdb support
################
CC_OPTIONS =  -g -O3 -DNDEBUG
CXX_OPTIONS = -g -O3 -DNDEBUG -std=c++11 -march=native
LNK_OPTIONS = -pthread -O3
BUILDDIR = build/release/
endif
endif
endif





###################################################
# INCLUDE directories
#

INCLUDE = -I.\
		-I../../src\





###################################################
# Build NoiseTest
#

SOURCES = NoiseTest.cpp OldNoise.cpp \

#	src/Log.cpp
#	src/MCLogger.cpp 
SHAREDSOURCES = \
	src/Noise/Noise.cpp \
	src/StringUtils.cpp \
	src/Logger.cpp \
	src/OSSupport/CriticalSection.cpp \
	src/OSSupport/File.cpp 
#	src/OSSupport/IsThread.cpp

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

$(BUILDDIR)src/%.o: ../../src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CXX_OPTIONS) -c $(INCLUDE) $< -o $@
	@$(CC) $(CXX_OPTIONS) -MM $(INCLUDE) $< > $(patsubst %.o,%.d,$@)
	@mv -f $(patsubst %.o,%.d,$@) $(patsubst %.o,%.d,$@).tmp
	@sed -e "s|.*:|$(BUILDDIR)$*.o:|" < $(patsubst %.o,%.d,$@).tmp > $(patsubst %.o,%.d,$@)
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst %.o,%.d,$@).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst %.o,%.d,$@)
	@rm -f $(patsubst %.o,%.d,$@).tmp

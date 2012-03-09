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


all: MCServer

ifeq ($(release),1)
CC_OPTIONS = -s -O3
CCE_OPTIONS = -s -x c -O3
LNK_OPTIONS = -lstdc++ -pthread -O3
BUILDDIR = build/release/
else
CC_OPTIONS = -s -ggdb -D_DEBUG
CCE_OPTIONS = -s -x c -ggdb -D_DEBUG
LNK_OPTIONS = -lstdc++ -pthread -ggdb
BUILDDIR = build/debug/
endif


#
# INCLUDE directories for MCServer
#

INCLUDE = -I.\
		-Isource\
		-Isource/md5\
		-IWebServer\
		-Isource/packets\
		-Itolua++-1.0.93/src/lib\
		-Ilua-5.1.4/src\
		-Izlib-1.2.5\
		-IiniFile\
		-Itolua++-1.0.93/include\
		-Ijsoncpp-src-0.5.0/include\
		-Ijsoncpp-src-0.5.0/src/lib_json

#
# Build MCServer
#

SOURCES := $(shell find lua-5.1.4 jsoncpp-src-0.5.0 zlib-1.2.5 source tolua++-1.0.93 iniFile WebServer '(' -name '*.cpp' -o -name '*.c' ')')
SOURCES := $(filter-out %minigzip.c %lua.c %tolua.c %toluabind.c %LeakFinder.cpp %StackWalker.cpp %example.c,$(SOURCES))
OBJECTS := $(patsubst %.c,$(BUILDDIR)%.o,$(SOURCES))
OBJECTS := $(patsubst %.cpp,$(BUILDDIR)%.o,$(OBJECTS))

-include $(patsubst %.o,%.d,$(OBJECTS))

MCServer : $(OBJECTS)
	$(CC) $(LNK_OPTIONS) $(OBJECTS) -o MCServer

clean : 
		rm -rf $(BUILDDIR) MCServer

install : MCServer
		cp MCServer MCServer

#
# Build the parts of MCServer
#

$(BUILDDIR)%.o: %.c
	@mkdir -p $(dir $@) 
	$(CC) $(CCE_OPTIONS) -c $(INCLUDE) $< -o $@
	@$(CC) $(CC_OPTIONS) -MM $(INCLUDE) $< > $(patsubst %.o,%.d,$@)
	@mv -f $(patsubst %.o,%.d,$@) $(patsubst %.o,%.d,$@).tmp
	@sed -e "s|.*:|$(BUILDDIR)$*.o:|" < $(patsubst %.o,%.d,$@).tmp > $(patsubst %.o,%.d,$@)
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst %.o,%.d,$@).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst %.o,%.d,$@)
	@rm -f $(patsubst %.o,%.d,$@).tmp

$(BUILDDIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CC_OPTIONS) -c $(INCLUDE) $< -o $@
	@$(CC) $(CC_OPTIONS) -MM $(INCLUDE) $< > $(patsubst %.o,%.d,$@)
	@mv -f $(patsubst %.o,%.d,$@) $(patsubst %.o,%.d,$@).tmp
	@sed -e "s|.*:|$(BUILDDIR)$*.o:|" < $(patsubst %.o,%.d,$@).tmp > $(patsubst %.o,%.d,$@)
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst %.o,%.d,$@).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst %.o,%.d,$@)
	@rm -f $(patsubst %.o,%.d,$@).tmp

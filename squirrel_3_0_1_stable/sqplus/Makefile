SQUIRREL= ..

OUT= $(SQUIRREL)/lib/libsqplus.a
INCDIRS= -I$(SQUIRREL)/include -I. -Iinclude 

ALLSRCS = $(wildcard *.cpp)
WIN32SRCS = $(wildcard *Win32*)
SRCS = $(filter-out $(WIN32SRCS),$(ALLSRCS))

sqplus:
#	g++ -fno-rtti -c $(SRCS) $(INCDIRS)
	g++ -O3  -fno-rtti -Os -c $(SRCS) $(INCDIRS)
	ar rc $(OUT) *.o
	rm *.o


#g++ -O3  -fno-rtti -Os -c $(SRCS) $(INCDIRS)
#g++ -ggdb  -fno-rtti -c $(SRCS) $(INCDIRS)

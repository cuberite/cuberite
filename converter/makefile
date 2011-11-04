CC = /usr/bin/g++
CC_OPTIONS = -O2 -s
CCE_OPTIONS = -O2 -s -x c


INCLUDE = -I.\
	-I../zlib-1.2.5\
	-Isource

denotch: \
		build/main.o\
		build/cDeNotch.o\
		build/cNBTData.o\
		build/cTimer.o\
		build/cQuicksort.o\
		build/adler32.o\
		build/compress.o\
		build/crc32.o\
		build/deflate.o\
		build/gzclose.o\
		build/gzlib.o\
		build/gzread.o\
		build/gzwrite.o\
		build/infback.o\
		build/inffast.o\
		build/inflate.o\
		build/inftrees.o\
		build/trees.o\
		build/uncompr.o\
		build/zutil.o
	$(CC) $(LNK_OPTIONS) \
                build/main.o\
                build/cDeNotch.o\
                build/cNBTData.o\
                build/cTimer.o\
                build/cQuicksort.o\
                build/adler32.o\
                build/compress.o\
                build/crc32.o\
                build/deflate.o\
                build/gzclose.o\
                build/gzlib.o\
                build/gzread.o\
                build/gzwrite.o\
                build/infback.o\
                build/inffast.o\
                build/inflate.o\
                build/inftrees.o\
                build/trees.o\
                build/uncompr.o\
                build/zutil.o\
		-o denotch

clean:
		rm \
                build/main.o\
                build/cDeNotch.o\
                build/cNBTData.o\
                build/cTimer.o\
                build/cQuicksort.o\
                build/adler32.o\
                build/compress.o\
                build/crc32.o\
                build/deflate.o\
                build/gzclose.o\
                build/gzlib.o\
                build/gzread.o\
                build/gzwrite.o\
                build/infback.o\
                build/inffast.o\
                build/inflate.o\
                build/inftrees.o\
                build/trees.o\
                build/uncompr.o\
                build/zutil.o\

install : denotch
	cp denotch denotch

build/main.o : source/main.cpp
	$(CC) $(CC_OPTIONS) source/main.cpp -c $(INCLUDE) -o build/main.o


build/cDeNotch.o : source/cDeNotch.cpp
	$(CC) $(CC_OPTIONS) source/cDeNotch.cpp -c $(INCLUDE) -o build/cDeNotch.o


build/cNBTData.o : source/cNBTData.cpp
	$(CC) $(CC_OPTIONS) source/cNBTData.cpp -c $(INCLUDE) -o build/cNBTData.o


build/cTimer.o : source/cTimer.cpp
	$(CC) $(CC_OPTIONS) source/cTimer.cpp -c $(INCLUDE) -o build/cTimer.o


build/cQuicksort.o : source/cQuicksort.cpp
	$(CC) $(CC_OPTIONS) source/cQuicksort.cpp -c $(INCLUDE) -o build/cQuicksort.o


# Item # 104 -- adler32 --
build/adler32.o : ../zlib-1.2.5/adler32.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/adler32.c -c $(INCLUDE) -o build/adler32.o


# Item # 105 -- compress --
build/compress.o : ../zlib-1.2.5/compress.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/compress.c -c $(INCLUDE) -o build/compress.o


# Item # 106 -- crc32 --
build/crc32.o : ../zlib-1.2.5/crc32.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/crc32.c -c $(INCLUDE) -o build/crc32.o


# Item # 107 -- deflate --
build/deflate.o : ../zlib-1.2.5/deflate.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/deflate.c -c $(INCLUDE) -o build/deflate.o


# Item # 108 -- gzclose --
build/gzclose.o : ../zlib-1.2.5/gzclose.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/gzclose.c -c $(INCLUDE) -o build/gzclose.o


# Item # 109 -- gzlib --
build/gzlib.o : ../zlib-1.2.5/gzlib.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/gzlib.c -c $(INCLUDE) -o build/gzlib.o


# Item # 110 -- gzread --
build/gzread.o : ../zlib-1.2.5/gzread.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/gzread.c -c $(INCLUDE) -o build/gzread.o


# Item # 111 -- gzwrite --
build/gzwrite.o : ../zlib-1.2.5/gzwrite.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/gzwrite.c -c $(INCLUDE) -o build/gzwrite.o


# Item # 112 -- infback --
build/infback.o : ../zlib-1.2.5/infback.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/infback.c -c $(INCLUDE) -o build/infback.o


# Item # 113 -- inffast --
build/inffast.o : ../zlib-1.2.5/inffast.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/inffast.c -c $(INCLUDE) -o build/inffast.o


# Item # 114 -- inflate --
build/inflate.o : ../zlib-1.2.5/inflate.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/inflate.c -c $(INCLUDE) -o build/inflate.o


# Item # 115 -- inftrees --
build/inftrees.o : ../zlib-1.2.5/inftrees.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/inftrees.c -c $(INCLUDE) -o build/inftrees.o


# Item # 116 -- trees --
build/trees.o : ../zlib-1.2.5/trees.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/trees.c -c $(INCLUDE) -o build/trees.o


# Item # 117 -- uncompr --
build/uncompr.o : ../zlib-1.2.5/uncompr.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/uncompr.c -c $(INCLUDE) -o build/uncompr.o


# Item # 118 -- zutil --
build/zutil.o : ../zlib-1.2.5/zutil.c
	$(CC) $(CCE_OPTIONS) ../zlib-1.2.5/zutil.c -c $(INCLUDE) -o build/zutil.o

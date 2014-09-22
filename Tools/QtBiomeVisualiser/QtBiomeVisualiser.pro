#-------------------------------------------------
#
# Project created by QtCreator 2014-09-11T15:22:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtBiomeVisualiser
TEMPLATE = app


SOURCES += main.cpp\
		MainWindow.cpp \
    BiomeView.cpp \
    ../../src/Generating/BioGen.cpp \
    ../../src/VoronoiMap.cpp \
    ../../src/Noise.cpp \
    ../../src/StringUtils.cpp \
    ../../src/LoggerListeners.cpp \
    ../../src/Logger.cpp \
    ../../lib/inifile/iniFile.cpp \
    ../../src/OSSupport/File.cpp \
    ../../src/OSSupport/CriticalSection.cpp \
    ../../src/OSSupport/IsThread.cpp \
    ../../src/BiomeDef.cpp \
    ChunkCache.cpp \
    Chunk.cpp \
    ChunkSource.cpp \
    ChunkLoader.cpp \
    ../../src/StringCompression.cpp \
    ../../src/WorldStorage/FastNBT.cpp \
    ../../lib/zlib/adler32.c \
    ../../lib/zlib/compress.c \
    ../../lib/zlib/crc32.c \
    ../../lib/zlib/deflate.c \
    ../../lib/zlib/gzclose.c \
    ../../lib/zlib/gzlib.c \
    ../../lib/zlib/gzread.c \
    ../../lib/zlib/gzwrite.c \
    ../../lib/zlib/infback.c \
    ../../lib/zlib/inffast.c \
    ../../lib/zlib/inflate.c \
    ../../lib/zlib/inftrees.c \
    ../../lib/zlib/trees.c \
    ../../lib/zlib/uncompr.c \
    ../../lib/zlib/zutil.c \
    GeneratorSetup.cpp

HEADERS  += MainWindow.h \
		Globals.h \
		BiomeView.h \
    ../../src/Generating/BioGen.h \
    ../../src/VoronoiMap.h \
    ../../src/Noise.h \
    ../../src/StringUtils.h \
    ../../src/LoggerListeners.h \
    ../../src/Logger.h \
    ../../lib/inifile/iniFile.h \
    ../../src/OSSupport/File.h \
    ../../src/OSSupport/CriticalSection.h \
    ../../src/OSSupport/IsThread.h \
    ../../src/BiomeDef.h \
    ChunkCache.h \
    Chunk.h \
    ChunkSource.h \
    ChunkLoader.h \
    ../../src/StringCompression.h \
    ../../src/WorldStorage/FastNBT.h \
    ../../lib/zlib/crc32.h \
    ../../lib/zlib/deflate.h \
    ../../lib/zlib/gzguts.h \
    ../../lib/zlib/inffast.h \
    ../../lib/zlib/inffixed.h \
    ../../lib/zlib/inflate.h \
    ../../lib/zlib/inftrees.h \
    ../../lib/zlib/trees.h \
    ../../lib/zlib/zconf.h \
    ../../lib/zlib/zlib.h \
    ../../lib/zlib/zutil.h \
    GeneratorSetup.h

INCLUDEPATH += $$_PRO_FILE_PWD_ \
	$$_PRO_FILE_PWD_/../../src \
	$$_PRO_FILE_PWD_/../../lib


CONFIG += C++11

OTHER_FILES +=



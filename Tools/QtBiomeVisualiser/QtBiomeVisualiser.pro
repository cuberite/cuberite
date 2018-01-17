#-------------------------------------------------
#
# Project created by QtCreator 2014-09-11T15:22:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtBiomeVisualiser
TEMPLATE = app


SOURCES += \
	MainWindow.cpp \
	BiomeView.cpp \
	../../src/Generating/BioGen.cpp \
	../../src/VoronoiMap.cpp \
	../../src/Noise/Noise.cpp \
	../../src/StringUtils.cpp \
	../../src/LoggerListeners.cpp \
	../../src/Logger.cpp \
	../../src/IniFile.cpp \
	../../src/OSSupport/Event.cpp \
	../../src/OSSupport/File.cpp \
	../../src/OSSupport/CriticalSection.cpp \
	../../src/OSSupport/IsThread.cpp \
	../../src/OSSupport/StackTrace.cpp \
	../../src/BiomeDef.cpp \
	../../src/StackWalker.cpp \
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
	GeneratorSetup.cpp \
	QtBiomeVisualiser.cpp \
	QtChunk.cpp \
	RegionCache.cpp \
	Region.cpp \
	ChunkSource.cpp \
	RegionLoader.cpp



HEADERS += \
	MainWindow.h \
	QtChunk.h \
	Globals.h \
	BiomeView.h \
	../../src/Generating/BioGen.h \
	../../src/Generating/IntGen.h \
	../../src/Generating/ProtIntGen.h \
	../../src/VoronoiMap.h \
	../../src/Noise/Noise.h \
	../../src/StringUtils.h \
	../../src/LoggerListeners.h \
	../../src/Logger.h \
	../../src/IniFile.h \
	../../src/OSSupport/Event.h \
	../../src/OSSupport/File.h \
	../../src/OSSupport/CriticalSection.h \
	../../src/OSSupport/IsThread.h \
	../../src/OSSupport/StackTrace.h \
	../../src/BiomeDef.h \
	../../src/StackWalker.h \
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
	GeneratorSetup.h \
	QtChunk.h \
	RegionCache.h \
	Region.h \
	ChunkSource.h \
	RegionLoader.h



INCLUDEPATH += $$_PRO_FILE_PWD_ \
	$$_PRO_FILE_PWD_/../../lib \
	$$_PRO_FILE_PWD_/../../lib/jsoncpp/include \
	$$_PRO_FILE_PWD_/../../lib/polarssl/include \
	$$_PRO_FILE_PWD_/../../lib/sqlite \
	$$_PRO_FILE_PWD_/../../lib/SQLiteCpp/include \
	$$_PRO_FILE_PWD_/../../



CONFIG += C++11

OTHER_FILES +=

win* {
	# Add the advapi32 library for windows compiles; needed for the StackWalker:
	LIBS += advapi32.lib

	#	StackWalker doesn't like how Qt inconsistently defines only UNICODE, but not _UNICODE:
	DEFINES -= UNICODE
}






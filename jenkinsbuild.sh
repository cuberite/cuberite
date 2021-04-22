#!/usr/bin/env bash

set -ex

# Parse arguments.
while [[ $# -gt 1 ]]
do
key="$1"
case $key in
    -s|--server-name)
    SERVERNAME="$2"
    shift
    ;;
    -t|--target)
    TARGET="$2"
    shift
    ;;
    -c|--compiler)
    CCOMP="$2"
    shift
    ;;
    -cxx|--cxx-compiler)
    CXXCOMP="$2"
    shift
    ;;
    -m|--compile-mode)
    COMPILEMODE="-DCMAKE_BUILD_TYPE=$2"
    shift
    ;;
    -n|--build-number)
    BUILDID="$2"
    shift
    ;;
    -p|--toolchain-file)
    TOOLCHAINFILE="-DCMAKE_TOOLCHAIN_FILE=$2"
    shift
    ;;
    -b|--branch)
    BRANCH="$2"
    shift
    ;;
    -32|--force-32)
    FORCE32="-DFORCE_32=$2"
    shift
    ;;
    *)
    ;;
esac
shift
done

git submodule update --init

# Set up build information.
export CUBERITE_BUILD_SERIES_NAME="$SERVERNAME $TARGET $COMPILEMODE ($BRANCH)"
export CUBERITE_BUILD_ID="$BUILDID"
export CUBERITE_BUILD_DATETIME="`date`"

if [ -x "$(command -v ccache)" ]
then
	export CCACHE_CPP2=true
	CACHE_ARGS="-DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache"
fi

# Build
CXX=$CXXCOMP CC=$CCOMP cmake . -DNO_NATIVE_OPTIMIZATION=1 ${CACHE_ARGS} ${TOOLCHAINFILE} ${COMPILEMODE} ${FORCE32}
make -j 2


# Package Server
echo Cuberite "$CUBERITE_BUILD_SERIES_NAME-$CUBERITE_BUILD_ID\n$BUILD_URL" > Server/buildinfo.txt

# h: dereference (archive file/folder instead of symlink)
# z: gzip (compress)
# c: create
# v: verbose
# T: files-from (list of server files accepted for release archives)
# f: file (output file location)
pushd Server
tar -hzcv --exclude .git -T Install/UnixExecutables.list -f ../Cuberite.tar.gz
popd
sha1sum Cuberite.tar.gz > Cuberite.tar.gz.sha1

# Package ProtoProxy
# This tool is very out of date, uncomment when it's being worked on again
# pushd Tools/ProtoProxy
# sha1sum ProtoProxy > ProtoProxy.sha1
# popd

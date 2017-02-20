#!/bin/sh

{
# NOTE: compile.sh looks for this file in order to determine if this is the Cuberite folder.
# Please modify compile.sh if you want to rename or remove this file.
# This file was chosen arbitrarily and it is a good enough indicator that we are in the Cuberite folder.

set -e

KERNEL=$(uname -s)

echo "Identifying kernel: $KERNEL"

if [ "$KERNEL" = "Linux" ]; then
	PLATFORM=$(uname -m)

	echo "Identifying platform: $PLATFORM"

	case $PLATFORM in
		"i686") DOWNLOADURL="https://builds.cuberite.org/job/Cuberite%20Linux%20x86%20Master/lastSuccessfulBuild/artifact/Cuberite.tar.gz" ;;
		"x86_64") DOWNLOADURL="https://builds.cuberite.org/job/Cuberite%20Linux%20x64%20Master/lastSuccessfulBuild/artifact/Cuberite.tar.gz" ;;
		# Assume that all arm devices are a raspi for now.
		arm*) DOWNLOADURL="https://builds.cuberite.org/job/Cuberite%20Linux%20raspi-armhf%20Master/lastSuccessfulBuild/artifact/Cuberite.tar.gz"
	esac
elif [ "$KERNEL" = "Darwin" ]; then
	DOWNLOADURL="https://builds.cuberite.org/job/Cuberite%20OSX%20x64%20Master/lastSuccessfulBuild/artifact/Cuberite.tar.gz"
elif [ "$KERNEL" = "FreeBSD" ]; then
	DOWNLOADURL="https://builds.cuberite.org/job/Cuberite%20FreeBSD%20x64%20Master/lastSuccessfulBuild/artifact/Cuberite.tar.gz"
else
	echo "Unsupported kernel."
	exit 1
fi

echo "Downloading precompiled binaries."
curl -s $DOWNLOADURL | tar -xzf -
echo "Done."

echo "Cuberite is now installed, run using 'cd Server; ./Cuberite'."

}

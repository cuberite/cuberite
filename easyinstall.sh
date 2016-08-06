#!/bin/sh

{
# NOTE: compile.sh looks for this file in order to determine if this is the Cuberite folder.
# Please modify compile.sh if you want to rename or remove this file.
# This file was chosen arbitrarily and it is a good enough indicator that we are in the Cuberite folder.

set -e

PLATFORM=$(uname -m)

echo "Identifying platform: $PLATFORM"
case $PLATFORM in
	"i686") DOWNLOADURL="https://builds.cuberite.org/job/Cuberite%20Linux%20x86%20Master/lastSuccessfulBuild/artifact/Cuberite.tar.gz" ;;
	"x86_64") DOWNLOADURL="https://builds.cuberite.org/job/Cuberite%20Linux%20x64%20Master/lastSuccessfulBuild/artifact/Cuberite.tar.gz" ;;
	# Assume that all arm devices are a raspi for now.
	arm*) DOWNLOADURL="https://builds.cuberite.org/job/Cuberite%20Linux%20raspi-armhf%20Master/lastSuccessfulBuild/artifact/Cuberite.tar.gz"
esac

echo "Downloading precompiled binaries."
curl -s $DOWNLOADURL | tar -xzf -
echo "Done."

echo "Cuberite is now installed, run using 'cd Server; ./Cuberite'."

}

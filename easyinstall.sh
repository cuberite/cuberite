#!/bin/sh

PLATFORM=$(uname -m)

echo "Identifying platform: $PLATFORM"
case $PLATFORM in 
	"i686") DOWNLOADURL="http://builds.cuberite.org/job/MCServer%20Linux%20x86/lastSuccessfulBuild/artifact/MCServer.tar.gz" ;;
	"x86_64") DOWNLOADURL="http://builds.cuberite.org/job/MCServer%20Linux%20x64/lastSuccessfulBuild/artifact/MCServer.tar.gz" ;;
	# Assume that all arm devices are a raspi for now.
	arm*) DOWNLOADURL="http://builds.cuberite.org/job/MCServer%20Linux%20armhf/lastSuccessfulBuild/artifact/MCServer/MCServer.tar.gz"
esac

echo "Downloading precompiled binaries."
curl -s $DOWNLOADURL | tar -xzf -
echo "Done."

echo "MCServer is now installed, run using 'cd MCServer; ./MCServer'."

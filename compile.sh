#!/bin/sh
#|| goto :windows_detected
{ # put the whole thing in a block so as not to behave weirdly if interrupted
set -e

#TODO command line parameter handling for non-interactive mode.

# Do we already have a repo?
if [ -d .git ] && [ -f easyinstall.sh ] && [ -f src/BlockArea.cpp ]; then # A good enough indicator that we're in the Cuberite git repo.
	cd ../
	echo "Cuberite repository detected. This should make the process faster, especially if you compiled before."
fi

# Error functions.
error ()
{
	echo
	echo "-----------------"
	echo "Script aborted, reason:"
	echo "$@"
	exit 1
}

missingDepsExit ()
{
	if [ "$1" != "" ]; then
		echo "You can install the missing depndencies via:"
		echo "$1"
	fi
	echo
	echo "Please install the dependencies, then come back."
	echo
	exit 2
}


# Echo: Greetings.
echo "
Hello, this script will download and compile Cuberite.
On subsequent runs, it will update Cuberite.
The compilation and download will occur in the current directory.
If you're updating, you should run: <Path to Cuberite>/compile.sh
Compiling from source takes time, but it usually generates faster
executables. If you prefer ready-to-use binaries or if you want
more info, please visit:  http://cuberite.org/"

### Dependency checks start. ###
MISSING_PACKAGES=""

# Most distros have the following default package and executable names.
GCC_EXE_NAME="g++"
CLANG_EXE_NAME="clang"
COMPILER_PACKAGE_NAME="gcc g++"

# Left side: Executable Name, Right side: Package Name. Note that this is TAB delimited. Spaces will not work.
PROGRAMS='git	git
make	make
cmake	cmake'

# If any OS deviates from the defaults, detect the OS here, and change PROGRAMS, COMPILER_PACKAGE_NAME, etc. as needed.

# Fedora, CentOS, RHEL, Mageia, openSUSE, Mandriva
if (rpm --help > /dev/null 2> /dev/null); then
	COMPILER_PACKAGE_NAME="gcc-c++"
fi

# Compiler check.
GCC_EXISTS=0
CLANG_EXISTS=0
$GCC_EXE_NAME --help > /dev/null 2> /dev/null && GCC_EXISTS=1
$CLANG_EXE_NAME --help > /dev/null 2> /dev/null && CLANG_EXISTS=1
if [ "$GCC_EXISTS" -eq 0 ] && [ "$CLANG_EXISTS" -eq 0 ]; then
	MISSING_PACKAGES=" $COMPILER_PACKAGE_NAME"
fi

# Depdendency check.
checkPackages ()
{
	# note that IFS is a TAB here!
	echo "$PROGRAMS" | while IFS='	' read EXE_NAME PACKAGE_NAME __trash__; do
		command -v $EXE_NAME > /dev/null 2> /dev/null || printf %s " $PACKAGE_NAME"
	done
}
MISSING_PACKAGES="$MISSING_PACKAGES`checkPackages`"

if [ "$MISSING_PACKAGES" != "" ]; then
	echo
	echo "-----------------"
	echo "You have missing compilation dependencies:"
	echo $MISSING_PACKAGES
	echo

	# apt-get guide.
	apt-get --help > /dev/null 2> /dev/null && \
	missingDepsExit "sudo apt-get install $MISSING_PACKAGES"

	# yum guide.
	yum --help > /dev/null 2> /dev/null && \
	missingDepsExit "sudo yum install $MISSING_PACKAGES"

	# zypper guide.
	zypper --help > /dev/null 2> /dev/null && \
	missingDepsExit "sudo zypper install $MISSING_PACKAGES"

	# pacman guide.
	pacman --help > /dev/null 2> /dev/null && \
	missingDepsExit "sudo pacman -S $MISSING_PACKAGES"

	# urpmi guide.
	urpmi --help > /dev/null 2> /dev/null && \
	missingDepsExit "sudo urpmi $MISSING_PACKAGES"

	missingDepsExit ""
fi
### Dependency checks end. ###

# Bypass Branch choice and choose master. Because it's the only branch right now.
BRANCH="master"

### Inactive code start. ###
inactiveCode ()
{

# Echo: Branch choice.
echo "
You can choose between 3 branches:
* (S)Stable:   Choose the stable branch if you want the most
               reliable server.

* (T)Testing:  The testing branch is less stable,
               but using it and reporting bugs helps us a lot!

* (D)Dev:      The least stable of the three. (Master branch)
               Choose the development branch if you want to try new,
               bleeding-edge features.
"


# Input: Branch choice.
printf %s "Choose the branch (s/t/d): "
read BRANCH
case $BRANCH in
	s|S)
		error "We don't have a stable branch yet, please use testing, sorry."
		;;
	t|T)
		BRANCH="testing"
		;;
	d|D)
		BRANCH="master"
		;;
	*)
		error "Unrecognized user input."
		;;
esac

}
### Inactive code end. ###

# Echo: Compile mode choice.
echo "
Choose compile mode:
* (N)Normal:  Compiles normally.
              Generates the fastest build.

* (D)Debug:   Compiles in debug mode.
              Makes your console and crashes more verbose.  
              A bit slower than Normal mode. If you plan to help
              development by reporting bugs, this is preferred.
		

Note that the script will connect to the internet in order to fetch
code after this step. It will then compile your program.
"

# Input: Compile mode choice.
printf %s "Choose compile mode: (n/d): "
read BUILDTYPE
case $BUILDTYPE in
	d|D)
		BUILDTYPE="Debug"
		;;
	n|N)
		BUILDTYPE="Release"
		;;
	*)
		error "Unrecognized user input."
		;;
esac

# Echo: Downloading began.
echo
echo " --- Downloading Cuberite's source code from the $BRANCH branch..."


if [ ! -d cuberite ]; then
	# Git: Clone.
	echo " --- Looks like your first run, cloning the whole code..."
	git clone https://github.com/cuberite/cuberite.git
	cd cuberite
else
	# Git: Fetch.
	cd cuberite
	echo " --- Updating the $BRANCH branch..."
	git fetch origin "$BRANCH" || error "git fetch failed"
	git checkout "$BRANCH" || error "git checkout failed"
	git merge "origin/$BRANCH" || error "git merge failed"
fi

# Git: Submodules.
echo " --- Updating submodules..."
git submodule update --init


# Cmake.
echo " --- Running cmake..."
if [ ! -d build-cuberite ]; then mkdir build-cuberite; fi
cd build-cuberite
cmake .. -DCMAKE_BUILD_TYPE="$BUILDTYPE" || error "cmake failed"


# Make.
echo " --- Compiling..."
make -j 2 || error "Compiling failed"
echo


# Echo: Compilation complete.
cd ../Server
echo
echo "-----------------"
echo "Compilation done!"
echo
echo "Cuberite awaits you at:"
if [ "$BUILDTYPE" = "Debug" ]; then
	echo "$PWD/Cuberite_debug"
else
	echo "$PWD/Cuberite"
fi
cd ..
echo "
You can always update Cuberite by executing:
`pwd`/compile.sh

Enjoy :)"
exit 0

:windows_detected
@echo off
rem Putting echo "Foo" in Windows CMD gives you "Foo" -- with quotes.
echo This script is not available for Windows yet, sorry.
echo You can still download the Windows binaries from: http://cuberite.org
echo You can also manually compile for Windows. See: https://github.com/cuberite/cuberite
rem windows_exit
exit
}

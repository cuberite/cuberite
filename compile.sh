#!/bin/sh
#|| goto :windows_detected
{ # put the whole thing in a block so as not to behave weirdly if interrupted
set -e

# Do we already have a repo?
if [ \( -d .git \) -a \( -f easyinstall.sh \) -a \( -f src/BlockArea.cpp \) ]; then # A good enough indicator that we're in the Cuberite git repo.
cd ../
echo "Cuberite repository detected. This should make the process faster, especially if you compiled before."
fi

# Error functions.
error ()
{
	echo
	echo "-----------------"
	echo "Script aborted, reason:"
	echo $1
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
echo
echo "Hello, this script will download and compile Cuberite."
echo "On subsequent runs, it will update Cuberite."
echo "The compilation and download will occur in the current directory."
echo "If you're updating, you should run <Path to Cuberite>/cuberite/compile.sh"
echo "Compiling from source takes time, but it usually generates faster executables."
echo "If you prefer ready-to-use binaries or if you want more info, please visit:"
echo "http://cuberite.org/"

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

# If any OS deviates from the defaults, detect the OS here, and change PROGRAMS,COMPILER_PACKAGE_NAME, etc. as needed.

# Fedora, CentOS, RHEL, Mageia, openSUSE, Mandriva
if (rpm --help > /dev/null 2> /dev/null); then
	COMPILER_PACKAGE_NAME="gcc-c++"
fi

# Compiler check.
GCC_EXISTS=0
CLANG_EXISTS=0
$GCC_EXE_NAME --help > /dev/null 2> /dev/null && GCC_EXISTS=1
$CLANG_EXE_NAME --help > /dev/null 2> /dev/null && CLANG_EXISTS=1
if [ $GCC_EXISTS -eq 0 -a $CLANG_EXISTS -eq 0 ]; then
	MISSING_PACKAGES=" $COMPILER_PACKAGE_NAME"
fi

# Depdendency check.
checkPackages ()
{
	echo "$PROGRAMS" | while read line; do
		EXE_NAME=`echo "$line" | cut -f 1`
		PACKAGE_NAME=`echo "$line" | cut -f 2`
		$EXE_NAME --help > /dev/null 2> /dev/null || echo -n " $PACKAGE_NAME"		
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
	missingDepsExit "sudo apt-get install$MISSING_PACKAGES"

	# yum guide.
	yum --help > /dev/null 2> /dev/null && \
	missingDepsExit "sudo yum install$MISSING_PACKAGES"

	# zypper guide.
	zypper --help > /dev/null 2> /dev/null && \
	missingDepsExit "sudo zypper install$MISSING_PACKAGES"

	# pacman guide.
	pacman --help > /dev/null 2> /dev/null && \
	missingDepsExit "sudo pacman -S$MISSING_PACKAGES"

	# urpmi guide.
	urpmi --help > /dev/null 2> /dev/null && \
	missingDepsExit "sudo urpmi$MISSING_PACKAGES"

	missingDepsExit ""
fi
### Dependency checks end. ###

# Bypass Branch choice and choose master. Because it's the only branch right now.
BRANCH="master"

### Inactive code start. ###
inactiveCode ()
{

# Echo: Branch choice.
echo
echo "You can choose between 2 branches:"
echo "* (S)Stable:	(Coming soon) Choose the stable branch if you want the most reliable server."
echo "		As of now, Stable is not yet available, please use testing instead."
echo
echo "* (T)Testing:	The testing branch is less stable,"
echo "		but using it and finding and reporting bugs helps us a lot!"
echo
echo "* (D)Dev:	The least stable of the three. (Master branch)"
echo "		Choose the development branch if you are feeling adventurous and"
echo "		want to try new, bleeding edge features."
echo


# Input: Branch choice.
echo -n "Choose the branch (s/t/d): "
read BRANCH
if [ \( "$BRANCH" = "s" \) -o \( "$BRANCH" = "S" \) ]; then
	#BRANCH="stable"
	error "We don't have a stable branch yet, please use testing, sorry."
elif [ \( $BRANCH = "t" \) -o \( $BRANCH = "T" \) ]; then
	BRANCH="testing"
elif [ \( $BRANCH = "d" \) -o \( $BRANCH = "D" \) ]; then
	BRANCH="master"
else
	error "Unrecognized user input."
fi

}
### Inactive code end. ###

# Echo: Compile mode choice.
echo
echo "Choose compile mode:"
echo "* (N)Normal:	Compiles normally."
echo
echo "* (D)Debug:	Compiles in debug mode. Makes your console and crashes much more verbose."
echo "		But it costs performance."
echo
echo "Note that the script will connect to the internet in order to fetch code after this step."
echo "It will then compile your program."
echo

# Input: Compile mode choice.
echo -n "Choose compile mode: (n/d): "
read BUILDTYPE
if [ \( "$BUILDTYPE" = "d" \) -o \( "$BUILDTYPE" = "D" \) ]; then
	BUILDTYPE="Debug"
elif [ \( "$BUILDTYPE" = "n" \) -o \( "$BUILDTYPE" = "N" \) ]; then
	BUILDTYPE="Release"
else
	error "Unrecognized user input."
fi


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
	git fetch origin $BRANCH || error "git fetch failed"
	git checkout $BRANCH || error "git checkout failed"
	git merge origin/$BRANCH || error "git merge failed"
fi

# Git: Submodules.
echo " --- Updating submodules..."
git submodule update --init


# Cmake.
echo " --- Running cmake..."
if [ ! -d build-cuberite ]; then mkdir build-cuberite; fi
cd build-cuberite
cmake .. -DCMAKE_BUILD_TYPE=$BUILDTYPE || error "cmake failed"


# Make.
echo " --- Compiling..."
make -j`nproc` || error "Compiling failed"
echo


# Echo: Compilation complete.
cd ../Server
echo
echo "-----------------"
echo "Compilation done!"
echo
echo "Cuberite awaits you at:"
if [ "$BUILDTYPE" = "Debug" ]; then
	echo "`pwd`/Cuberite_debug"
else
	echo "`pwd`/Cuberite"
fi
echo
echo "Enjoy :)"
exit 0

:windows_detected
echo "This script is not available for Windows yet, sorry."
echo "You can still download the Windows binaries from: http://cuberite.org"
echo "You can also manually compile for Windows. See: https://github.com/cuberite/cuberite"
}

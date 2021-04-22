#!/bin/sh
#|| goto :windows_detected
{ # put the whole thing in a block so as not to behave weirdly if interrupted
set -e

# Global variables:
# CHOICE_BUILDTYPE  - Either "Release" or "Debug".
# CHOICE_THREADS    - A numerical value, the amount of threads to be used for the make command.
# CHOICE_BRANCH     - The branch to use. Currently locked on "master".
# STATE_INTERACTIVE - 1 If we're running interactively. 0 otherwise.
# STATE_NEW         - Whether this is the first run. If 1, then no GIT repo exists yet. 0 otherwise.

# Constants:
DEFAULT_BUILDTYPE="Release" # Other options: "Debug"
DEFAULT_BRANCH="master"     # Other options: None currently
DEFAULT_THREADS=1

# Constants not modifiable through command line:
UPSTREAM_REPO="origin"
UPSTREAM_LINK="https://github.com/cuberite/cuberite.git"

#=================== Error functions ===================


errorCompile ()
{
	echo
	echoInt "-----------------"
	echo "Compilation failed. Failed command:"
	echo "$@"
	exit 1
}

errorGit ()
{
	echo
	echoInt "-----------------"
	echo "Code fetch failed. (Check your network connection). Failed command:"
	echo "$@"
	exit 2
}

errorDependencies ()
{
	# The error messages are complex and OS-dependant, and are printed in the dependencies section before this is called.
	exit 3
}

errorArguments ()
{
	echo "Usage: ./compile.sh [options]"
	echo "Compiles Cuberite. Updates the GIT repository if needed, and downloads it if it does not exist."
	echo "Runs interactively, unless one or more options are specified."
	echo
	echo "options:"
	echo "  -m  The compilation mode. Either \"Release\" or \"Debug\". Defaults to \"$DEFAULT_BUILDTYPE\""
	echo '  -t  The number of threads to use for compiling'
	echo "      If unspecified, a default of $DEFAULT_THREADS threads is used. The special value AUTO attempts to set the number of"
	echo '      compilation threads equal to the number of CPU threads.'
	echo '  -b  The branch to compile. (Currently unused and pinned to MASTER)'
	echo '  -n yes: Prevent interactive mode. Unnecessary in combination with other arguments.'
	echo '          Use without any other argument to build with the default settings.'
	echo '  -d yes: Dry run. Print the chosen settings and exit'
	echo
	echo "Usage examples:"
	echo "  ./compile.sh"
	echo "  ./compile.sh -m Debug"
	echo "  ./compile.sh -m Release -t 2"
	echo
	echo "Return codes: (non 0 returns are accompanied by useful stderr info)"
	echo "0 - Success              - Success! Code was updated and compiled"
	echo "1 - Compilation failed   - cmake, make, or source code issue"
	echo "2 - Code fetch failed    - Network issue or, far more rarely, a git issue"
	echo "3 - Dependencies missing - Some compilation tools are missing"
	echo "4 - Bad arguments        - Bad commandline arguments were passed"
	echo "5 - Bad user input       - Invalid user input in interactive mode"
	echo "6 - other                - An error not listed above"
	exit 4
}

errorInput ()
{
	echo
	echoInt "-----------------"
	echo "Unrecognized user input"
	echo "$@"
	exit 5
}

errorOther ()
{
	echo
	echoInt "-----------------"
	echo "$@"
	exit 6
}


#=================== Echo functions ===================


echoInt () # echo only if interactive mode.
{
	if [ $STATE_INTERACTIVE -eq 1 ]; then
		echo "$1"
	fi
}

echoErr () # Echo to stderr.
{
	echo "$1" 1>&2
}


#=================== Commandline Parsing ===================


STATE_INTERACTIVE=1 # Interactive, unless one or more command line options are passed.
while getopts ":m:t:b:d:n:" name; do
	value=$OPTARG
	STATE_INTERACTIVE=0
	case "$name" in
	m)
		if [ ! -z "$CHOICE_BUILDTYPE" ]; then errorArguments; fi # Argument duplication.
		if [ "$value" = "Debug" ] || [ "$value" = "Release" ]; then
			CHOICE_BUILDTYPE="$value"
		else
			errorArguments
		fi
	;;
	t)
		if [ ! -z "$CHOICE_THREADS" ]; then errorArguments; fi # Argument duplication.
		if [ "$value" -gt 0 ] 2>/dev/null || [ "$value" = "AUTO" ]; then # If a positive integer or the special value "AUTO".
			CHOICE_THREADS="$value"
		else
			errorArguments
		fi
	;;
	b)
		if [ ! -z "$CHOICE_BRANCH" ]; then errorArguments; fi # Argument duplication.
		CHOICE_BRANCH=1 # Only used for dupe checking, overridden below.
		echoErr "Warning: The -b option is currently unused, it was ignored"
	;;
	d)
		if [ ! -z "$DRY_RUN" ]; then errorArguments; fi # Argument duplication.
		DRY_RUN="yes"
	;;
	n)
		if [ "$dummy" = "1" ]; then errorArguments; fi # Argument duplication.
		dummy=1 # we just want to disable interactive mode, passing an argument already did this. No need to do anything.
	;;
	*)
		errorArguments
	;;
	esac
done

if [ -z "$DRY_RUN" ]; then DRY_RUN="no"; fi

#=================== Dependency checks and greeting ===================


# Do we already have a repo?
checkCuberiteDir ()
{
	[ -d .git ] && [ -f easyinstall.sh ] && [ -f src/BlockArea.cpp ] # A good enough indicator that we're in the Cuberite git repo.
}

STATE_NEW=1
if checkCuberiteDir; then # Check if we're in the Cuberite directory...
	STATE_NEW=0
elif [ -d cuberite ]; then # If there's a directory named "cuberite"...
	cd cuberite
	if checkCuberiteDir; then # Check if we're in the Cuberite directory...
		STATE_NEW=0
	else
		errorOther "A directory is named 'cuberite' which has no Cuberite assets exists. Please run the script elsewhere or move/delete that directory."
	fi

fi

if [ $STATE_NEW -eq 0 ]; then
	echoInt "Cuberite repository detected. This should make the process faster, especially if you compiled before."
fi

# Echo: Greetings.
echoInt "

Hello, this script will download and compile Cuberite.
On subsequent runs, it will update Cuberite.
The compilation and download will occur in the current directory.
If you're updating, you should run: <Path to Cuberite>/compile.sh
Compiling from source takes time, but it usually generates faster
executables. If you prefer ready-to-use binaries or if you want
more info, please visit:  https://cuberite.org/"

doDependencyCheck()
{
	MISSING_PACKAGES=""

	# Most distros have the following default compiler names.
	GCC_EXE_NAME="g++"
	CLANG_EXE_NAME="clang"
	COMPILER_PACKAGE_NAME="gcc g++"

	# Most distros have the following package and executable names.
	# Left side: Executable Name, Right side: Package Name. Note that this is SPACE delimited now, unlike in the past.
	PROGRAMS='git git
	make make
	cmake cmake'

	# If any OS deviates from the defaults, we detect the OS here, and change PROGRAMS, COMPILER_PACKAGE_NAME, etc. as needed.

	# Fedora, CentOS, RHEL, Mageia, openSUSE, Mandriva.
	if (rpm --help > /dev/null 2> /dev/null); then
		COMPILER_PACKAGE_NAME="gcc-c++"
	fi

	# Make sure at least one compiler exists.
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
		echo "$PROGRAMS" | while read line; do
			EXE_NAME=`echo "$line" | cut -f 1 -d " "`
			PACKAGE_NAME=`echo "$line" | cut -f 2 -d " "`
			command -v $EXE_NAME > /dev/null 2> /dev/null || printf %s " $PACKAGE_NAME"
		done
	}
	MISSING_PACKAGES="$MISSING_PACKAGES`checkPackages`"
	missingDepsExit ()
	{
		if [ "$1" != "" ]; then
			echoErr "You can install the missing depndencies via:"
			echoErr "$1"
		fi
		echoErr
		echoErr "Please install the dependencies, then come back."
		echoErr
		errorDependencies
	}

	if [ "$MISSING_PACKAGES" != "" ]; then
		echoInt
		echoInt "-----------------"
		echoErr "You have missing compilation dependencies:"
		echoErr $MISSING_PACKAGES
		echoErr

		# apt-get guide.
		apt-get --help > /dev/null 2> /dev/null && \
		missingDepsExit "apt-get install$MISSING_PACKAGES"

		# dnf guide.
		dnf --help > /dev/null 2> /dev/null && \
		missingDepsExit "dnf install$MISSING_PACKAGES"

		# zypper guide.
		zypper --help > /dev/null 2> /dev/null && \
		missingDepsExit "zypper install$MISSING_PACKAGES"

		# pacman guide.
		pacman --help > /dev/null 2> /dev/null && \
		missingDepsExit "pacman -S$MISSING_PACKAGES"

		# urpmi guide.
		urpmi --help > /dev/null 2> /dev/null && \
		missingDepsExit "urpmi$MISSING_PACKAGES"

		missingDepsExit ""
	fi
}
doDependencyCheck


#=================== Choice: Branch (Currently unused and simply skipped) ===================


# Bypass Branch choice and choose master. Because it's the only branch right now.
CHOICE_BRANCH=$DEFAULT_BRANCH

### Inactive code start. ###
inactiveCode ()
{

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


printf %s "Choose the branch (s/t/d): "
read CHOICE_BRANCH
case $CHOICE_BRANCH in
	s|S)
		errorOther "We don't have a stable branch yet, please use testing, sorry."
		;;
	t|T)
		CHOICE_BRANCH="testing"
		;;
	d|D)
		CHOICE_BRANCH="master"
		;;
	*)
		errorInput
		;;
esac

}
### Inactive code end. ###


#=================== Choice: Compile mode ===================


if [ $STATE_INTERACTIVE -eq 1 ]; then
	echo "
	Choose compile mode:
	* (R)Release: Compiles normally.
		      Generates the fastest build.

	* (D)Debug:   Compiles in debug mode.
		      Makes your console and crashes more verbose.
		      A bit slower than Release mode. If you plan to help
		      development by reporting bugs, this is preferred.

	"

	printf %s "Choose compile mode: (r/d) (Default: \"$DEFAULT_BUILDTYPE\"): "
	read CHOICE_BUILDTYPE
	case $CHOICE_BUILDTYPE in
		d|D)
			CHOICE_BUILDTYPE="Debug"
			;;
		r|N)
			CHOICE_BUILDTYPE="Release"
			;;
	esac
fi

if [ -z "$CHOICE_BUILDTYPE" ]; then # No buildtype specified.
	CHOICE_BUILDTYPE="$DEFAULT_BUILDTYPE"
fi


#=================== Choice: Thread amount ===================



numberOfThreads()
{
	KERNEL=`uname -s`

	if [ "$KERNEL" = "Linux" ] || [ "$KERNEL" = "Darwin" ]; then
		echo `getconf _NPROCESSORS_ONLN`
	elif [ "$KERNEL" = "FreeBSD" ]; then
		echo `getconf NPROCESSORS_ONLN`
	else
		echo "unknown"
	fi
}

CPU_THREAD_COUNT=`numberOfThreads`

if [ $STATE_INTERACTIVE -eq 1 ]; then
	echo ""
	echo "Choose the number of compilation threads."

	if [ "$CPU_THREAD_COUNT" = "unknown" ]; then
		echo "Could not detect the number of CPU threads."
	elif [ "$CPU_THREAD_COUNT" -eq 1 ]; then
		echo "You have 1 thread."
	else
		echo "You have $CPU_THREAD_COUNT CPU threads."
	fi

	echo "If you have enough RAM, it is wise to choose your CPU's thread count. "
	echo "Otherwise choose lower. Old Raspberry Pis should choose 1. If in doubt, choose 1."
	printf %s "Please enter the number of compilation threads to use (Default: $DEFAULT_THREADS): "
	read CHOICE_THREADS
fi

if [ -z "$CHOICE_THREADS" ] 2> /dev/null; then
	CHOICE_THREADS="$DEFAULT_THREADS"
elif [ "$CHOICE_THREADS" = "AUTO" ] 2> /dev/null; then
	if [ $CPU_THREAD_COUNT = "unknown" ]; then
		CHOICE_THREADS="$DEFAULT_THREADS"
		echo "WARNING: could not detect number of threads. Using the default ($DEFAULT_THREADS) ." >&2
	else
		CHOICE_THREADS="$CPU_THREAD_COUNT"
	fi
elif [ "$CHOICE_THREADS" -lt 0 ] 2> /dev/null; then
	errorInput
fi

#=================== Print settings summary  ===================


if [ "$STATE_NEW" = 1 ]; then
	previousCompilation="Not detected. We are assuming this is the first compile.sh run."
else
	previousCompilation="Detected. This should make fetching and compiling faster."
fi

THREAD_WARNING=""
if [ "$CPU_THREAD_COUNT" != "unknown" ] && [ "$CPU_THREAD_COUNT" -lt "$CHOICE_THREADS" ]; then
	THREAD_WARNING=" - Warning: More threads assigned than there are CPU threads."
fi

echo ""
echoInt "#### Settings Summary ####"
echo "Build Type:           " "$CHOICE_BUILDTYPE"
echo "Branch:               " "$CHOICE_BRANCH" "(Currently the only choice)"
echo "Compilation threads:  " "$CHOICE_THREADS$THREAD_WARNING"
echo "CPU Threads:          " "$CPU_THREAD_COUNT"
echo "Previous compilation: " "$previousCompilation"
echo "Upstream Link:        " "$UPSTREAM_LINK"
echo "Upstream Repo:        " "$UPSTREAM_REPO"

if [ "$DRY_RUN" = "yes" ]; then
	echo "This is a dry run. Exiting now."
	exit 0;
fi

# Ask the user's permission to connect to the net.
if [ $STATE_INTERACTIVE -eq 1 ]; then
	echo
	echo "After pressing ENTER, the script will connect to $UPSTREAM_LINK"
	echo "to check for updates and/or fetch code. It will then compile your program."
	echo "If you compiled before, make sure you're in the proper directory and that \"Previous compilation\" is detected."
	printf $s "Press ENTER to continue... "
	read dummy
fi


#=================== Code download / update via git ===================


echoInt
echoInt " --- Downloading Cuberite's source code from the $CHOICE_BRANCH branch..."


if [ $STATE_NEW -eq 1 ]; then
	# Git: Clone.
	echo " --- Looks like your first run, cloning the whole code..."
	git clone  --depth 1 "$UPSTREAM_LINK" -b "$CHOICE_BRANCH" || errorGit "git clone  --depth 1 $UPSTREAM_LINK -b $CHOICE_BRANCH"
	cd cuberite
else
	# Git: Fetch.
	echo " --- Updating the $CHOICE_BRANCH branch..."
	git fetch "$UPSTREAM_REPO" "$CHOICE_BRANCH" || errorGit "git fetch $UPSTREAM_REPO $CHOICE_BRANCH"
	git checkout "$CHOICE_BRANCH" || errorGit "git checkout $CHOICE_BRANCH"
	git merge "$UPSTREAM_REPO"/"$CHOICE_BRANCH" || errorGit "git merge $UPSTREAM_REPO/$CHOICE_BRANCH"
fi

# Git: Submodules.
echo " --- Updating submodules..."
git submodule sync
git submodule update --init


#=================== Compilation via cmake and make ===================


# Cmake.
echo " --- Running cmake..."
if [ ! -d build-cuberite ]; then mkdir build-cuberite; fi
cd build-cuberite
cmake .. -DCMAKE_BUILD_TYPE="$CHOICE_BUILDTYPE" || errorCompile "cmake .. -DCMAKE_BUILD_TYPE=$CHOICE_BUILDTYPE"


# Make.
echo " --- Compiling..."
make -j "$CHOICE_THREADS" || errorCompile "make -j $CHOICE_THREADS"
echo


#=================== Print success message ===================


cd Server
echo
echo "-----------------"
echo "Compilation done!"
echo
echo "Cuberite awaits you at:"
echo "$PWD/Cuberite"

cd ../..
echo "
You can always update Cuberite by executing:
$PWD/compile.sh

Enjoy :)"
exit 0


#=================== Windows fallback ===================


# Called via hack in line 2.
:windows_detected
@echo off
cls
echo This script is not available for Windows yet, sorry.
echo You can still download the Windows binaries from: https://cuberite.org/
echo You can also manually compile for Windows. See: https://github.com/cuberite/cuberite
rem windows_exit
goto :EOF
}

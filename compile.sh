#|| goto :windows_detected

# Do we already have a repo?
if [[ -d .git && -f easyinstall.sh && -f src/BlockArea.cpp ]]; then # A good enough indicator that we're in the Cuberite git repo.
cd ../
echo "Cuberite repository detected. This should make the process faster, especially if you compiled before."
fi

# Error functions.
function error
{
	echo
	echo "-----------------"
	echo "Script aborted, reason:"
	echo $1
	exit -1
}

function missingDepsExit
{
	echo
	echo "Please install the dependencies, then come back."
	echo
	exit -2
}


# Echo: Greetings.
echo
echo "Hello, this script will download and compile Cuberite."
echo "On subsequent runs, it will update your Cuberite."
echo "The compilation and download will occur in the current directory."
echo "If you're updating, you should run <Path to Cuberite>/cuberite/compile.sh"
echo "Compiling from source takes time, but it usually generates better executables."
echo "If you prefer ready-to-use binaries or if you want more info, please visit:"
echo "http://cuberite.org/"

MISSING_PROGRAMS=""

# Compiler check.
GCC_EXISTS=0
CLANG_EXISTS=0
g++ --help > /dev/null 2> /dev/null && GCC_EXISTS=1
clang --help > /dev/null 2> /dev/null && CLANG_EXISTS=1
if [[ $GCC_EXISTS == 0 && $CLANG_EXISTS == 0 ]]; then
MISSING_PROGRAMS="gcc g++"
fi

# Depdendency check.
while read program; do
$program --help > /dev/null 2> /dev/null || MISSING_PROGRAMS="$MISSING_PROGRAMS $program"
done <<"EOF"
git
make
cmake
EOF
if [[ $MISSING_PROGRAMS != "" ]]; then
	echo
	echo "-----------------"
	echo "You have missing compilation dependencies:"
	echo $MISSING_PROGRAMS
	echo

	# apt-get guide.
	apt-get --help > /dev/null 2> /dev/null && \
	echo "You can install the missing depndencies via:" && \
	echo -n "sudo apt-get install " && echo $MISSING_PROGRAMS && missingDepsExit

	# yum guide.
	yum --help > /dev/null 2> /dev/null && \
	echo "You can install the missing depndencies via:" && \
	echo -n "sudo yum install " && echo $MISSING_PROGRAMS && missingDepsExit

	# rpm guide.
	rpm --help > /dev/null 2> /dev/null && \
	echo "You can install the missing depndencies via:" && \
	echo -n "sudo rpm -i " && echo $MISSING_PROGRAMS && missingDepsExit

	# pacman guide.
	pacman --help > /dev/null 2> /dev/null && \
	echo "You can install the missing depndencies via:" && \
	echo -n "sudo pacman -S " && echo $MISSING_PROGRAMS && missingDepsExit

	missingDepsExit
fi

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
if [[ ($BRANCH == "s") || ($BRANCH == "S" ) ]]; then
	#BRANCH="stable"
	error "We don't have a stable branch yet, please use testing, sorry."
elif [[ ($BRANCH == "t") || ($BRANCH == "T" ) ]]; then
	BRANCH="testing"
elif [[ ($BRANCH == "d") || ($BRANCH == "D" ) ]]; then
	BRANCH="master"
else
	error "Unrecognized user input."
fi

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
if [[ ($BUILDTYPE == "d") || ($BUILDTYPE == "D") ]]; then
	BUILDTYPE="Debug"
elif [[ ($BUILDTYPE == "n") || ($BUILDTYPE == "N") ]]; then
	BUILDTYPE="Release"
else
	error "Unrecognized user input."
fi


# Echo: Downloading began.
echo
echo " --- Downloading Cuberite's source code from the $BRANCH branch..."


# Git: Clone.
if [ ! -d cuberite ]; then
	echo " --- Looks like your first run, cloning the whole code..."
	git clone https://github.com/cuberite/cuberite.git
fi


# Git: Fetch.
pushd cuberite
echo " --- Updating the $BRANCH branch..."
git fetch origin $BRANCH || error "git fetch failed"
git checkout $BRANCH || error "git checkout failed"
git merge origin/$BRANCH || error "git merge failed"


# Git: Submodules.
echo " --- Updating submodules..."
git submodule update --init


# Cmake.
echo " --- Running cmake..."
if [ ! -d build-cuberite ]; then mkdir build-cuberite; fi
pushd build-cuberite
cmake .. -DCMAKE_BUILD_TYPE=$BUILDTYPE || error "cmake failed"


# Make.
echo " --- Compiling..."
make -j`nproc` || error "Compiling failed"
echo


# Echo: Compilation complete.
popd > /dev/null
pushd Server > /dev/null
echo
echo "-----------------"
echo "Compilation done!"
echo
echo "Cuberite awaits you at:"
if [[ $BUILDTYPE == "Debug" ]]; then
echo "`pwd`/Cuberite_debug"
else
echo "`pwd`/Cuberite"
fi
echo
echo "Enjoy :)"
popd > /dev/null
popd > /dev/null
exit 0

:windows_detected
echo "This script is not available for Windows yet, sorry."
echo "You can still download the Windows binaries from: http://cuberite.org"

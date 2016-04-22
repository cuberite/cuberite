COMPILING
=========

To compile Cuberite from source, you need the following set of software:

 * CMake
 * Platform-specific make tool \(windows would be MSVC, linux/osx GNU make, etc.\)
 * C compiler
 * modern C++ compiler and linker

To contribute code, you also need a Git client.

Windows
-------

We use Microsoft Visual Studio for Windows compilation. It is possible to use other toolchains, but we don't test against them and they aren't supported. Visual Studio 2013 Express for Desktop is being actively used for development.

You can find download links for VS2013 Express here: http://go.microsoft.com/?linkid=9832280

Next, you need to download and install CMake. Download from here: http://cmake.org/cmake/resources/software.html . You should download a full installation package, so that the installer will set everything up for you (especially the paths).

To contribute your changes to the source back to the repository, you need a Git client. Options are:

 * MsysGit: http://msysgit.github.io/
 * GitHub windows client: http://windows.github.com/
 * TortoiseGit: http://code.google.com/p/tortoisegit/

Alternatively, if you want only to compile the source, without contributing, you can [download the sources in a ZIP file directly from GitHub](https://github.com/cuberite/cuberite/archive/master.zip).

If you're using Git to get the source, use the following command to set up the local workspace correctly:

```
git clone --recursive https://github.com/cuberite/cuberite.git
```

Now that you have the source, it's time to prepare the project files for your favorite compiler. Open a command window in the folder with the source and type in `cmake .` . This will run CMake, it will auto-detect your Visual Studio version and produce the appropriate project and solution files.

Finally, open the newly created file, `Cuberite.sln`, in your Visual Studio.

If you want to run Cuberite from within VS, you need to first make sure that it will be run with the correct home folder. Normally this happens automatically, but for some Visual Studio versions the process doesn't stick. Right-click on the Cuberite project in the Solution Explorer tool window, and select Properties. In the dialog, navigate to Configuration properties -> Debugging in the tree on the left, then make sure the value `Working Directory` is set to `../Server`. If you don't do this, the server won't be able to find crafting recipes, item names or plugins.

### Release configuration ###

To make Visual Studio produce the version with the best performance, you will need to select a Release configuration. Go to menu Build -> Configuration Manager, and in the opened dialog, change the top left combo box (Active solution configuration) to Release. Close the dialog and build the solution. The resulting executable is called `Cuberite.exe` in the `Server` folder.

### Debug configuration ###

In order to tinker with the code, you'll more than likely need to use the debugging features of your IDE. To make them the easiest to use, you should switch to the Debug configuration - this provides the highest level of information while debugging, for the price of the executable being 2 - 10 times slower. Go to menu Build -> Configuration Manager, and in the opened dialog, change the top left combo box (Active solution configuration) to Debug. Close the dialog and build the solution. The resulting executable is called `Cuberite_debug.exe` in the `Server` folder.

OSX
---

Install git from its [website](http://git-scm.com) or homebrew: `brew install git`.

Install Xcode (commandline tools are recommended) from the App Store or [the website](https://developer.apple.com/downloads).

Install CMake from its [website](http://cmake.org) or homebrew: `brew install cmake`.

### Getting the sources ###

```
git clone --recursive https://github.com/cuberite/cuberite.git
```

### Building ###

Follow the instructions at [CMake on Unix-based platforms](#cmake-on-unix-based-platforms), using Xcode as cmake's generator. If no generator is specified, CMake will use the Makefile generator, in which case you must build with the `make` command.

After doing so, run the command `xcodebuild lib/polarssl/POLARSSL.xcodeproj` in the build directory, in order to build polarssl, a library that is required by Cuberite. Lastly, run the command `xcodebuild` to build Cuberite. Optionally, you may open the project files for polarssl and then Cuberite in Xcode and build there.


Linux, FreeBSD etc.
-------------------

Install git, make, cmake and clang (or gcc), using your platform's package manager.  debian/ubuntu:

```
sudo apt-get install git make cmake clang
```
modern C++ compiler and linker (Clang 3.4+, GCC 4.8+, or VS 2013+).

### Getting the source ###

```
git clone --recursive https://github.com/cuberite/cuberite.git
```

### Building ###

Run the following commands to build Cuberite:

```
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make -j`nproc`
```

This will build Cuberite in release mode, which is better for almost all cases. For more `cmake` options, or for building in debug mode, see the section below.


CMake on Unix-based platforms
-----------------------------

### Release Mode ###

Release mode is preferred for almost all cases, it has much better speed and less console spam. However, if you are developing Cuberite actively, debug mode might be better.

Assuming you are in the Cuberite folder created in the initial setup step, you need to run these commands:

```
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
```

NOTE: CMake can generate project files for many different programs, such as Xcode, eclipse, and ninja. To use a different generator, first type `cmake --help`, and at the end, cmake will output the different generators that are available. To specify one, add `-G` followed by the name of the generator, in the `cmake` command. Note that the name is case-sensitive.

The executable will be built in the `cuberite/Server` folder and will be named `Cuberite`.

### Debug Mode ###

Debug mode is useful if you want more debugging information about Cuberite while it's running or if you want to use a debugger like GDB to debug issues and crashes.

Assuming you are in the Cuberite folder created in the Getting the sources step, you need to run these commands:

```
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
```

NOTE: CMake can generate project files for many different programs, such as Xcode, eclipse, and ninja. To use a different generator, first type `cmake --help`, and at the end, cmake will output the different generators that are available. To specify one, add `-G` followed by the name of the generator, in the `cmake` command. Note that the name is case-sensitive.

The executable will be built in the `cuberite/Server` folder and will be named `Cuberite_debug`.

### 32 Bit Mode switch ###

This is useful if you want to compile Cuberite on an x64 (64-bit Intel) machine but want to use on an x86 (32-bit Intel) machine. This switch can be used with debug or release mode. Simply add:

    -DFORCE_32=1

to your cmake command and 32 bit will be forced.

### Compiling for another computer ###

When cross-compiling for another computer it is important to set cross compiling mode. This tells the compiler not to optimise for your machine. This switch can be used with debug or release mode. To enable, simply add:

    -DCROSSCOMPILE=1

to your cmake command.

Note that real cross-compilation is hard at this moment, because an executable generated during the build process is required for later stages, so if your computer cannot run executables it generates the build will not work.

### List of all build flags ###

Cuberite's build process supports a large number of flags for customising the builds. Use these flags by adding `-DFlag_name=Value` to the cmake configuration command. For example to enable test generation using the `SELF_TEST` flag add: `-DSELF_TEST=ON`

###### BUILD_TOOLS
Adds the Cuberite tools to the build. At the moment only MCADefrag and ProtoProxy are added. Define as ON to enable. Define as OFF to disable. 

###### BUILD_UNSTABLE_TOOLS
Adds tools that are not working yet to the build. Currently this is only the Generator Performance Test. Used for developing these tools. Define as ON to enable. Define as OFF to disable. 

###### SELF_TEST
Enables generation of tests and self-test startup code. Tests can be run with ctest and with makefiles make test. Define as ON to enable. Define as OFF to disable. 

###### FORCE_32
Forces the build to use 32 bit builds on *nix systems. Define as ON to enable. Define as OFF to disable.
 
###### CROSSCOMPILE
Disables optimizations for the build host. This is important when building on a different machine from the one you will run Cuberite on as the build machine may support instructions the final machine does not. This flag only has any effect on linux. Define as ON to enable. Define as OFF to disable. 



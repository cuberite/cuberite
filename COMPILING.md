Compiling Cuberite
==================

To compile Cuberite from source, you need the following set of software:

 - CMake
 - Platform-specific make tool (Windows would be MSVC, Linux/macOS GNU make, etc.)
 - C compiler
 - Modern C++17 capable compiler and linker

To contribute code, you also need a Git client.

Windows
-------

We use Microsoft Visual Studio for Windows compilation. It is possible to use other toolchains, but we don't test against them and they aren't supported. Visual Studio 2017 Community and above are being actively used for development.

You can find download links for VS2019 Community [here][1].

Next, you need to download and install [CMake][2]. You should download a full installation package, so that the installer will set everything up for you (especially the paths).

To contribute your changes to the source back to the repository, you need a Git client. Options are:

 - [Git for Windows][3]
 - [GitHub Desktop][4]
 - [TortoiseGit][5]

Alternatively, if you want only to compile the source, without contributing, you can [download the sources in a ZIP file directly from GitHub][6].

If you're using Git to get the source, use the following command to set up the local workspace correctly:

```
git clone --recursive https://github.com/cuberite/cuberite.git
```

Now that you have the source, it's time to prepare the project files for your favorite compiler. Open a command window in the folder with the source and type in `cmake .` . This will run CMake, it will auto-detect your Visual Studio version and produce the appropriate project and solution files.

Finally, open the newly created file, `Cuberite.sln`, in your Visual Studio.

If you want to run Cuberite from within VS, you need to first make sure that it will be run with the correct home folder. Normally this happens automatically, but for some Visual Studio versions the process doesn't stick. Right-click on the Cuberite project in the Solution Explorer tool window, and select Properties. In the dialog, navigate to Configuration properties -> Debugging in the tree on the left, then make sure the value `Working Directory` is set to `../Server`. If you don't do this, the server won't be able to find crafting recipes, item names or plugins.

### Release Configuration

To make Visual Studio produce the version with the best performance, you will need to select a Release configuration. Go to menu Build -> Configuration Manager, and in the opened dialog, change the top left combo box (Active solution configuration) to Release. Close the dialog and build the solution. The resulting executable is called `Cuberite.exe` in the `Server` folder.

### Debug Configuration

In order to tinker with the code, you'll more than likely need to use the debugging features of your IDE. To make them the easiest to use, you should switch to the Debug configuration - this provides the highest level of information while debugging, for the price of the executable being 2 - 10 times slower. Go to menu Build -> Configuration Manager, and in the opened dialog, change the top left combo box (Active solution configuration) to Debug. Close the dialog and build the solution. The resulting executable is called `Cuberite_debug.exe` in the `Server` folder.

macOS
-----

 - Install git from its [website][7] or homebrew: `brew install git`.

 - Install Xcode (commandline tools are recommended) from the App Store or [the website][8].

 - Install CMake from its [website][9] or homebrew: `brew install cmake`.

Cuberite requires Xcode 11.3 or newer.

### Getting the Source

```
git clone --recursive https://github.com/cuberite/cuberite.git
```

### Building

Follow the instructions at [CMake on Unix-based platforms](#cmake-on-unix-based-platforms), using Xcode as cmake's generator. If no generator is specified, CMake will use the Makefile generator, in which case you must build with the `make` command.

After doing so, run the command `xcodebuild lib/polarssl/POLARSSL.xcodeproj` in the build directory, in order to build polarssl, a library that is required by Cuberite. Lastly, run the command `xcodebuild` to build Cuberite. Optionally, you may open the project files for polarssl and then Cuberite in Xcode and build there.


Linux, FreeBSD etc.
-------------------

Install git, make, cmake and clang (or gcc), using your platform's package manager. Debian/Ubuntu:

```
sudo apt-get install git make cmake clang
```
Ensure that you have modern C++ compiler and linker (Clang 7.0+, GCC 7.4+, or VS 2017+).
Cuberite also requires CMake 3.13 or newer.  You might find that your distribution
defaults are too out of date, in which case you may need to add a new `apt` source,
or download directly from the projects' websites:

 - [Clang][Clang-download]
 - [CMake][2]

### Getting the Source

```
git clone --recursive https://github.com/cuberite/cuberite.git
```

### Building

Run the following commands to build Cuberite:

```
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make -j`nproc`
```

This will build Cuberite in release mode, which is better for almost all cases. For more `cmake` options, or for building in debug mode, see [CMake on Unix-based platforms](#cmake-on-unix-based-platforms).

Android
-------

It is required that users obtain the latest copies of:

 - [The Android Native Development Kit (NDK)][10]
 - [Lua (download a binary)][11]

Windows users may optionally install the [Ninja build system][12] for improved build speeds.

### Getting the Source

```
git clone --recursive https://github.com/cuberite/cuberite.git
```

### Configuration

From the `android` subdirectory:

```
cmake . -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=16 -DCMAKE_BUILD_TYPE=Release -DCMAKE_ANDROID_ARCH_ABI=armeabi -DCMAKE_ANDROID_NDK=""
```
where `CMAKE_ANDROID_NDK` should be the absolute path to where the Android NDK is installed.

#### Generators to Use

On Linux, the default Make is suggested. No additional parameters are required for this option.

Windows users may make use of Visual Studio to compile for Android, but CMake requires the presence of Nvidia CodeWorks/Nsight Tegra, which can be a hassle to install.

The easiest generator to use seems to be the NDK-bundled Make, to be specified:
 * `-G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=""`
where `CMAKE_MAKE_PROGRAM` should be the absolute path to the `make` program, found under the `prebuilt/windows-*/bin` subdirectory in the NDK folder.

The next easiest generator is Ninja, which additionally offers multithreaded builds, to be specified:
 * `-G "Ninja"`

#### Additional ABI Options

For additional ABI options, visit: https://cmake.org/cmake/help/latest/variable/CMAKE_ANDROID_ARCH_ABI.html

Please note that certain ABIs require specific [API levels](#api-level-requirements).

#### API Level Requirements

The minimum API level is 16 in the verbatim copy of this folder, due to the inclusion of position independent compilation. Additonally, API level 21 or higher is needed for 64 bit ABIs as earlier versions have no support for this architecture.

To lower these requirements to run on very old devices, it is necessary to select a compatible ABI, and disable position independent code generation.

### Building

From the `android` subdirectory:

```
cmake --build .
```

If the build succeeded, an Android-launchable binary will have been generated under the `Server` directory. However, since this directory does not have any supporting files, they must be copied from the parent folder's `Server` directory.

To use it in the official Android app, compress the aforementioned `Server` directory into a Zip file, and transfer it to the phone on which the app is installed.

#### Using the Compile Script on Linux

Linux users are entitled to use the compile script, which provides some easy to use options and also contains instructions for using the binaries in the official Android app.

When running the compile script, make sure to have the necessary build tools installed and run the compile script as following:

```
NDK="path/to/ndk/root" CMAKE="path/to/cmake/executable" android/compile.sh <abi|all|clean>
```

The NDK variable must be set to the path to the NDK root, CMAKE to a call of the cmake binary used for compiling. If the cmake binary is in the PATH, a simple `CMAKE=cmake` is enough. As last parameter you either have to enter a correct ABI (see https://cmake.org/cmake/help/latest/variable/CMAKE_ANDROID_ARCH_ABI.html) or either all or clean. Clean will cause the script to remove the android-build directory, all will compile and zip all Cuberite for all 7 ABIs and create a zip archive of the android/Server direcory for use in the official Android app. If you are unsure which ABI is required for your phone, open the official Android app and navigate to "Settings" and "Install". It will show you the required ABI. Additional parameters may be given through enviroment variables, namely TYPE="" as Release or Debug (defaults to Release) and THREADS="4" as the number of threads used for compilation (defaults to 4).

### Running the Executables on a Device

Note the locations to which the Zip files were transferred. Open the official Android app, and select "Settings", then "Install", and finally select the Zip files.

Cuberite for Android is now ready to use.

CMake on Unix-based platforms
-----------------------------

### Release Mode

Release mode is preferred for almost all cases, it has much better speed and less console spam. However, if you are developing Cuberite actively, debug mode might be better.

Assuming you are in the Cuberite folder created in the initial setup step, you need to run these commands:

```
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
```

NOTE: CMake can generate project files for many different programs, such as Xcode, eclipse, and ninja. To use a different generator, first type `cmake --help`, and at the end, cmake will output the different generators that are available. To specify one, add `-G` followed by the name of the generator, in the `cmake` command. Note that the name is case-sensitive.

The executable will be built in the `cuberite/Server` folder and will be named `Cuberite`.

### Debug Mode

Debug mode is useful if you want more debugging information about Cuberite while it's running or if you want to use a debugger like GDB to debug issues and crashes.

Assuming you are in the Cuberite folder created in the Getting the sources step, you need to run these commands:

```
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
```

NOTE: CMake can generate project files for many different programs, such as Xcode, eclipse, and ninja. To use a different generator, first type `cmake --help`, and at the end, cmake will output the different generators that are available. To specify one, add `-G` followed by the name of the generator, in the `cmake` command. Note that the name is case-sensitive.

The executable will be built in the `cuberite/Server` folder and will be named `Cuberite_debug`.

### 32 Bit Mode Switch

This is useful if you want to compile Cuberite on an x64 (64-bit Intel) machine but want to use on an x86 (32-bit Intel) machine. This switch can be used with debug or release mode. Simply add:

    -DFORCE_32=1

to your cmake command and 32 bit will be forced.

### Compiling for Another Computer of the Same Architecture

When cross-compiling for another computer of the same architecture it is important to set the NO_NATIVE_OPTIMIZATION flag. This tells the compiler not to optimise for your machine. This switch can be used with debug or release mode. To enable, simply add:

    -DNO_NATIVE_OPTIMIZATION=1

to your cmake command.

### List of All Build Flags

Cuberite's build process supports a large number of flags for customising the builds. Use these flags by adding `-DFlag_name=Value` to the cmake configuration command. For example to enable test generation using the `SELF_TEST` flag add: `-DSELF_TEST=ON`

###### BUILD_TOOLS
Adds the Cuberite tools to the build. At the moment only MCADefrag and ProtoProxy are added. Define as ON to enable. Define as OFF to disable.

###### BUILD_UNSTABLE_TOOLS
Adds tools that are not working yet to the build. Currently this is only the Generator Performance Test. Used for developing these tools. Define as ON to enable. Define as OFF to disable.

###### SELF_TEST
Enables generation of tests and self-test startup code. Tests can be run with ctest and with makefiles make test. Define as ON to enable. Define as OFF to disable.

###### FORCE_32
Forces the build to use 32 bit builds on *nix systems. Define as ON to enable. Define as OFF to disable.

###### NO_NATIVE_OPTIMIZATION
Disables optimizations for the build host. This is important when building on a different machine from the one you will run Cuberite on as the build machine may support instructions the final machine does not. This flag only has any effect on linux. Define as ON to enable. Define as OFF to disable.

###### DISABLE_SYSTEM_LUA
Disables use of the system lua, uses a compiled version instead. Useful if compiling on a system that doesn't already have lua installed. This option is incompatible with cross-compilation.

###### UNITY_BUILDS
Controls use of unity builds, an optimisation that improves compile times at the expense of system compatibility and error message utility. Some systems may need to have this disabled in order to compile properly. Unity builds are only supported on CMake versions >= 3.16, if you have an old version unity builds will always be disabled and this flag has no effect.

###### PRECOMPILE_HEADERS
Controls use of precompiled headers, an optimisation that improves compile times at the expense of system compatibility. Some systems may need to have this disabled in order to compile properly. Precompiled headers are only supported on CMake versions >= 3.16, if you have an old version precompiled headers will always be disabled and this flag has no effect.

###### WHOLE_PROGRAM_OPTIMISATION
Controls use of link time optimisation (LTO), which slightly improves the generated binary file at the expense of compilation speed and system compatibility. Some systems may need to have this disabled in order to compile properly.


[1]: https://www.visualstudio.com/downloads/
[2]: https://cmake.org/download/
[3]: https://git-for-windows.github.io/
[4]: https://desktop.github.com/
[5]: https://tortoisegit.org/
[6]: https://github.com/cuberite/cuberite/archive/master.zip
[7]: https://git-scm.com/
[8]: https://developer.apple.com/downloads
[9]: https://cmake.org/
[10]: https://developer.android.com/ndk/downloads/index.html
[11]: https://www.lua.org/download.html
[12]: https://github.com/ninja-build/ninja/releases
[clang-download]: https://releases.llvm.org/download.html

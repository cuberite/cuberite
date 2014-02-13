# COMPILING #

To compile MCServer from source, you need CMake and make, as well as a C compiler, C++ compiler and linker. To contribute code, you also need a Git client.

## Windows ##

We use Microsoft Visual Studio for Windows compilation. It is possible to use other toolchains, but it isn't tested nor supported. Visual Studio versions 2008 Express and 2013 Express are being actively used for development, so either should work.
You can find download links for VS2008 Express here: http://stackoverflow.com/questions/15318560/visual-c-2008-express-download-link-dead
And VS2013 Express here: http://www.microsoft.com/en-us/download/details.aspx?id=40787

Next, you need to download and install CMake. Download from here: http://cmake.org/cmake/resources/software.html . You should download a full installation package, so that the installer will set everything up for you (especially the paths).

To contribute your changes to the sources back to the repository, you need a Git client. Options are:
- MsysGit: http://msysgit.github.io/
- GitHub windows client: http://windows.github.com/
- TortoiseGit: http://code.google.com/p/tortoisegit/

Alternatively, if you want only to compile the source, without contributing, you can download the sources in a ZIP file directly from GitHub: https://github.com/mc-server/MCServer/archive/master.zip
If you're using Git to get the sources, use the following set of commands to set up the local workspace correctly:
```
mkdir MCServer
cd MCServer
git clone https://github.com/mc-server/MCServer.git .
git submodule init
git submodule update
```

Now that you have the sources, it's time to prepare the project files for your favorite compiler. Open a command window in the folder with the sources and type in `cmake .` . This will run CMake, it will auto-detect your Visual Studio version and produce the appropriate project and solution files.

Finally, open the newly created file, `MCServer.sln`, in your Visual Studio.

If you want to run MCServer from within the IDE, you need to first make sure that it will be run with the correct home folder. Normally this happens automatically, but for some Visual Studio versions the process doesn't stick. Right-click on the MCServer project in the Solution Explorer tool window, and select Properties. In the dialog, navigate to Configuration properties -> Debugging in the tree on the left, then make sure the value `Working Directory` is set to `../MCServer`. If you don't do this, the server won't be able to find crafting recipes, item names or plugins.

### Release configuration ###

To make Visual Studio produce the version with the best performance, you will need to select a Release configuration. Go to menu Build -> Configuration Manager, and in the opened dialog, change the top left combo box (Active solution configuration) to Release. Close the dialog and build the solution. The resulting executable is called `MCServer.exe` in the `MCServer` folder.

### Debug configuration ###

In order to tinker with the code, you'll more than likely need to use the debugging features of the IDE. To make them the easiest to use, you should switch to the Debug configuration - this provides the highest level of information while debugging, for the price of the executable being 2 - 10 times slower. Go to menu Build -> Configuration Manager, and in the opened dialog, change the top left combo box (Active solution configuration) to Debug. Close the dialog and build the solution. The resulting executable is called `MCServer_debug.exe` in the `MCServer` folder.

### Profiling ###

It is possible to use an external profiler to learn more about how the code performs. You can find more information about the techniques in the article here: http://www.codeproject.com/Articles/144643/Profiling-of-C-Applications-in-Visual-Studio-for-F

There's a script file, `MCServer/profile_run.cmd` that encapsulates most of the profiling work, have a look at the comments at the top of that script for details on how to get it to work. You'll need to change to a profiled configuration (both debug and release can be profiled).

## Linux, MacOS, FreeBSD etc. ##

Install git, cmake and gcc or clang, using your platform's package manager:
```
sudo apt-get install git cmake gcc g++
```

### Getting the sources ###
```
mkdir MCServer
cd MCServer
git clone https://github.com/mc-server/MCServer.git .
git submodule init
git submodule update
```

### Release Mode ###

Release mode is preferred for almost all cases, it has much better speed and less console spam. However, if you are developing MCServer actively, debug mode might be better.

Assuming you are in the MCServer folder created in the initial setup step, you need to run these commands:
```
mkdir Release
cd Release
cmake . -DCMAKE_BUILD_TYPE=RELEASE .. && make
```
The executable will be built in the `MCServer/MCServer` folder and will be named `MCServer`.

### Debug Mode ###

Debug mode is useful if you want more debugging information about MCServer while it's running or if you want to use a debugger like GDB to debug issues and crashes.

Assuming you are in the MCServer folder created in the Getting the sources step, you need to run these commands:
```
mkdir Debug
cd Debug
cmake . -DCMAKE_BUILD_TYPE=DEBUG && make`
```
The executable will be built in the `MCServer/MCServer` folder and will be named `MCServer_debug`.
    
### 32 Bit Mode switch ###

This is useful if you want to compile MCServer on an x64 (64-bit Intel) machine but want to use on an x86 (32-bit Intel) machine. This switch can be used with debug or release mode. Simply add:

    -DFORCE_32=1
    
to your cmake command and 32 bit will be forced.

### Compiling for another computer ###


When cross-compiling for another computer it is important to set cross compiling mode. This tells the compiler not to optimise for your machine. This switch can be used with debug or release mode. To enable, simply add:

    -DCROSSCOMPILE=1

to your cmake command.

Note that cross-compilation is probably broken at this moment, since the build requires running an executable that it has built, as part of the build process.

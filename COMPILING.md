COMPILING
=========

To compile MCServer on *nix, you need CMake and make, as well as a C compiler, C++ compiler and linker.

Release Mode
------------

Release mode is preferred for almost all cases, it has much better speed and less console spam. However, if you are developing MCServer actively, debug mode might be better.

    cmake . -DCMAKE_BUILD_TYPE=RELEASE && make
    
Debug Mode
----------

Debug mode is useful if you want more debugging information about MCServer as it's running or if you want to use a debugger like GDB to debug issues and crashes.

    cmake . -DCMAKE_BUILD_TYPE=DEBUG && make
    
32 Bit Mode
-----------

This is useful if you want to compile MCServer to use on another 32-bit machine. It can be used with debug or release mode. To use 32 bit mode, simply add:

    -DFORCE_32=1
    
to your cmake command and 32 bit will be forced.

Compiling for another computer
------------------------------

When compiling for another computer it is important to set cross compiling mode. This tells the compiler not to optimise for your machine. It can be used with debug or release mode. To enable simply add:

    -DCROSSCOMPILE=1

to your cmake command.

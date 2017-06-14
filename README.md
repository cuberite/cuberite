Cuberite [![Build Status](https://img.shields.io/travis/cuberite/cuberite/master.svg?style=flat)](https://travis-ci.org/cuberite/cuberite) [![Coverity Scan Build Status](https://img.shields.io/coverity/scan/1930.svg)](https://scan.coverity.com/projects/1930)
========

Cuberite is a Minecraft-compatible multiplayer game server that is written in C++ and designed to be efficient with memory and CPU, as well as having a flexible Lua Plugin API. Cuberite is compatible with the vanilla Minecraft client.

Cuberite can run on Windows, *nix and Android operating systems. This includes Android phones and tablets as well as Raspberry Pis.

We currently support Release 1.8 - 1.12 Minecraft protocol versions.

Subscribe to [the newsletter](https://cuberite.org/news/#subscribe) for important updates and project news.

Installation
------------

There are several ways to obtain Cuberite.

#### Binaries
 - The easiest method is downloading for Windows or Linux from the [Project site](https://cuberite.org/).
 - You can use the EasyInstall script for Linux and macOS, which automatically downloads the correct binary. The script is described below.
 - You can also obtain a binary from the [buildserver archive](https://builds.cuberite.org/).

##### The EasyInstall script
This script will download the correct binary from the project site.

    curl -sSfL https://download.cuberite.org | sh

#### Compiling
 - You can compile automatically for Linux / *nix with the `compile.sh` script. The script is described below.
 - You can also compile manually. See [COMPILING.md](https://github.com/cuberite/cuberite/blob/master/COMPILING.md).

Compiling may provide better performance (1.5-3x as fast) and it supports more operating systems.

##### The compile.sh script
This script downloads the source code and compiles it. The script is smart enough to notify you of missing dependencies and instructing you on how to install them. The script doesn't work for Windows.

    sh -c "$(wget -O - https://compile.cuberite.org)"

#### Hosted services
 - Hosted Cuberite is available via [Gamocosm](https://gamocosm.com/).

Contributing
------------

Cuberite is licensed under the Apache License V2, and we welcome anybody to fork and submit a Pull Request back with their changes, and if you want to join as a permanent member we can add you to the team.

Cuberite is developed in C++ and Lua. To contribute code, please check out [GETTING-STARTED.md](https://github.com/cuberite/cuberite/blob/master/GETTING-STARTED.md) and [CONTRIBUTING.md](https://github.com/cuberite/cuberite/blob/master/CONTRIBUTING.md) for more details.

Plugins are written in Lua. You can contribute by developing plugins and submitting them to [the forum](https://forum.cuberite.org/forum-2.html). Please check out our [plugin introduction guide](http://api-docs.cuberite.org/Writing-a-Cuberite-plugin.html) for more info.

If you are not a programmer, you can help by testing Cuberite and reporting bugs. See [TESTING.md](https://github.com/cuberite/cuberite/blob/master/TESTING.md) for details.

You can also help with documentation by contributing to the [User's Manual](https://github.com/cuberite/users-manual).

Other Stuff
-----------

For other stuff, check out the [homepage](https://cuberite.org/), the [Users' Manual](https://book.cuberite.org/),
the [forums](https://forum.cuberite.org/), and the [Plugin API](https://api.cuberite.org/).

[Support Us on Bountysource](https://bountysource.com/teams/cuberite)

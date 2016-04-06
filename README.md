Cuberite [![Build Status](https://img.shields.io/travis/cuberite/cuberite/master.svg?style=flat)](https://travis-ci.org/cuberite/cuberite) [![Coverity Scan Build Status](https://img.shields.io/coverity/scan/1930.svg)](https://scan.coverity.com/projects/1930)
========

Cuberite is a Minecraft-compatible multiplayer game server that is written in C++ and designed to be efficient with memory and CPU, as well as having a flexible Lua Plugin API. Cuberite is compatible with the vanilla Minecraft client.

Cuberite can run on Windows, *nix and Android operating systems. This includes Android phones and tablets as well as Raspberry Pis.

We currently support Release 1.7 and 1.8 (not beta) Minecraft protocol versions.

Subscribe to [the newsletter](http://cuberite.org/news/) for important updates and project news.

Installation
------------

There are several ways to obtain Cuberite.

#### Binaries
 - The easiest method is downloading for Windows or Linux from the [Project site](http://cuberite.org/).
 - You can use the EasyInstall script for Linux, which automatically downloads the correct binary. The script is described below.
 - You can also obtain a binary from the [buildserver archive](https://builds.cuberite.org/).

##### The EasyInstall script
This Linux script will download the correct binary from the project site.

    sh -c "$(wget -O - https://raw.githubusercontent.com/cuberite/cuberite/master/easyinstall.sh)"

#### Compiling
 - You can compile automatically for Linux / *nix with the `compile.sh` script. The script is described below.
 - You can also compile manually. See [COMPILING.md](https://github.com/cuberite/cuberite/blob/master/COMPILING.md).

Compiling may provide better performance (1.5-3x as fast) and it supports more operating systems.

##### The compile.sh script
This script downloads the source code and compiles it. The script is smart enough to notify you of missing dependencies and instructing you on how to install them. The script doesn't work for Windows.

    sh -c "$(wget -O - https://raw.githubusercontent.com/cuberite/cuberite/master/compile.sh)"

#### Hosted services
 - Hosted Cuberite is available DIY on DigitalOcean: [![Install on DigitalOcean](http://doinstall.bearbin.net/button.svg)](http://doinstall.bearbin.net/install?url=https://github.com/cuberite/cuberite) and [Gamocosm](https://gamocosm.com/) also offers Cuberite support.

Contributing
------------

Cuberite is licensed under the Apache License V2, and we welcome anybody to fork and submit a Pull Request back with their changes, and if you want to join as a permanent member we can add you to the team.

Check out the [CONTRIBUTING.md](https://github.com/cuberite/cuberite/blob/master/CONTRIBUTING.md) file for more details.

Other Stuff
-----------

For other stuff, including plugins and discussion, check out the [forums](https://forum.cuberite.org/) and [Plugin API](http://api-docs.cuberite.org/).

[Support Us on Bountysource](https://bountysource.com/teams/cuberite)

Travis CI: [![travis build status shield](https://img.shields.io/travis/cuberite/cuberite.svg)](https://travis-ci.org/cuberite/cuberite)

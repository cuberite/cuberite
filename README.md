# Cuberite

[![Build Status](https://img.shields.io/travis/cuberite/cuberite/master.svg?label=Travis)](https://travis-ci.org/cuberite/cuberite)
[![Circle CI Build Status](https://img.shields.io/circleci/project/github/cuberite/cuberite/master.svg?label=Circle)](https://circleci.com/gh/cuberite/cuberite)
[![AppVeyor Build Status](https://img.shields.io/appveyor/ci/cuberite/cuberite/master.svg?label=AppVeyor)](https://ci.appveyor.com/project/cuberite/cuberite)

Cuberite is a Minecraft-compatible multiplayer game server that is written in C++ and designed to be efficient with memory and CPU, as well as having a flexible Lua Plugin API. Cuberite is compatible with the Java Edition Minecraft client.

Cuberite runs on Windows, *nix and Android operating systems. This includes Android phones and tablets as well as Raspberry Pis.

Currently we support Release 1.8 - 1.12.2 Minecraft protocol versions.

Subscribe to [the newsletter][1] for important updates and project news.

## Installation

There are several ways to obtain Cuberite.

### Binaries

- The easiest method is downloading for Windows or Linux from the [website][2].
- You can use the EasyInstall script for Linux and macOS, which automatically downloads the correct binary. The script is described below.

#### The EasyInstall script

This script will download the correct binary from the project site.

    curl -sSfL https://download.cuberite.org | sh

### Compiling

- You can compile automatically for Linux, macOS and FreeBSD with the `compile.sh` script. The script is described below.
- You can also compile manually. See [COMPILING.md][4].

Compiling may provide better performance (1.5-3x as fast) and it supports more operating systems.

#### The compile.sh script

This script downloads the source code and compiles it. The script is smart enough to notify you of missing dependencies and instructing you on how to install them. The script doesn't work for Windows.

Using curl:

    sh -c "$(curl -sSfL -o - https://compile.cuberite.org)"

Or using wget:

    sh -c "$(wget -O - https://compile.cuberite.org)"

### Hosted services

- Hosted Cuberite is available via [Gamocosm][5].

## Contributing

Cuberite is licensed under the Apache License V2, and we welcome anybody to fork and submit a Pull Request back with their changes, and if you want to join as a permanent member we can add you to the team.

Cuberite is developed in C++ and Lua. To contribute code, please check out [GETTING-STARTED.md][6] and [CONTRIBUTING.md][7] for more details.

Plugins are written in Lua. You can contribute by developing plugins and submitting them to the [plugin repository][8] or the [forum][9]. Please check out our [plugin introduction guide][10] for more info.

If you are not a programmer, you can help by testing Cuberite and reporting bugs. See [TESTING.md][11] for details.

You can also help with documentation by contributing to the [User's Manual][12].

## Other Stuff

For other stuff, check out the [homepage][13], the [Users' Manual][14], the [forums][15], and the [Plugin API][16].

Support the Cuberite development team on [Liberapay][17]

[1]: https://cuberite.org/news/#subscribe
[2]: https://cuberite.org/
[4]: https://github.com/cuberite/cuberite/blob/master/COMPILING.md
[5]: https://gamocosm.com/
[6]: https://github.com/cuberite/cuberite/blob/master/GETTING-STARTED.md
[7]: https://github.com/cuberite/cuberite/blob/master/CONTRIBUTING.md
[8]: https://plugins.cuberite.org/
[9]: https://forum.cuberite.org/forum-2.html
[10]: https://api.cuberite.org/Writing-a-Cuberite-plugin.html
[11]: https://github.com/cuberite/cuberite/blob/master/TESTING.md
[12]: https://github.com/cuberite/users-manual
[13]: https://cuberite.org/
[14]: https://book.cuberite.org/
[15]: https://forum.cuberite.org/
[16]: https://api.cuberite.org/
[17]: https://liberapay.com/Cuberite

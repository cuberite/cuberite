Cuberite [![Build Status](http://img.shields.io/travis/cuberite/cuberite/master.svg?style=flat)](https://travis-ci.org/cuberite/cuberite) [![Coverity Scan Build Status](https://img.shields.io/coverity/scan/1930.svg)](https://scan.coverity.com/projects/1930) [![weekly tips](http://img.shields.io/gratipay/cuberite.svg?style=flat)](http://gratipay.com/cuberite)
========

Cuberite is a Minecraft-compatible multiplayer game server that is written in C++ and designed to be efficient with memory and CPU, as well as having a flexible Lua Plugin API. Cuberite is compatible with the vanilla Minecraft client.

Cuberite can run on Windows, *nix and Android operating systems. This includes Android phones and tablets as well as Raspberry Pis. 

We currently support Release 1.7 and 1.8 (not beta) Minecraft protocol versions.

Subscribe to [the newsletter](http://newsletter.cuberite.org/subscribe.htm) for important updates and project news.

Installation
------------
Hosted Cuberite is available DIY on DigitalOcean: [![Install on DigitalOcean](http://doinstall.bearbin.net/button.svg)](http://doinstall.bearbin.net/install?url=https://github.com/cuberite/cuberite) and [Gamocosm](https://gamocosm.com) also offers Cuberite support.

For Linux there is an easy installation method, just run this in your terminal:

    curl -s https://raw.githubusercontent.com/cuberite/cuberite/master/easyinstall.sh | sh

For Windows, you just need to download a file and extract it:

 - [Windows 32 bit](http://builds.cuberite.org/job/Cuberite%20Windows%20x86%20Master/lastSuccessfulBuild/artifact/Install/Cuberite.zip)
 - [Windows 64 bit](http://builds.cuberite.org/job/Cuberite%20Windows%20x64%20Master/lastSuccessfulBuild/artifact/Install/Cuberite.zip)

For other operating systems you need to download and compile yourself. This can be done either manually, or with this automatic script:

    bash -c "$(wget -O - https://raw.githubusercontent.com/cuberite/cuberite/master/compile.sh)"

There is also an archive of binary builds on the buildserver: http://builds.cuberite.org

Compiling the server yourself has other benefits: you may get better performance performance (1.5-3x as fast) and it supports more operating systems. See the [COMPILING.md](https://github.com/cuberite/cuberite/blob/master/COMPILING.md) file for more details.

Contributing
------------

Cuberite is licensed under the Apache License V2, and we welcome anybody to fork and submit a Pull Request back with their changes, and if you want to join as a permanent member we can add you to the team.

Check out the [CONTRIBUTING.md](https://github.com/cuberite/cuberite/blob/master/CONTRIBUTING.md) file for more details.

Other Stuff
-----------

For other stuff, including plugins and discussion, check out the [forums](http://forum.mc-server.org) and [Plugin API](http://api-docs.cuberite.org).

Support Us on Gratipay: [![gratipay shield](http://img.shields.io/gratipay/cuberite.svg)](https://www.gratipay.com/cuberite)

Travis CI: [![travis build status shield](http://img.shields.io/travis/cuberite/cuberite.svg)](https://travis-ci.org/cuberite/cuberite)


MCServer [![Build Status](http://img.shields.io/travis/mc-server/MCServer/master.svg?style=flat)](https://travis-ci.org/mc-server/MCServer) [![Coverity Scan Build Status](https://img.shields.io/coverity/scan/1930.svg)](https://scan.coverity.com/projects/1930) [![weekly tips](http://img.shields.io/gratipay/cuberite_team.svg?style=flat)](http://gratipay.com/cuberite_team)
========

MCServer is a Minecraft-compatible multiplayer game server that is written in C++ and designed to be efficient with memory and CPU, as well as having a flexible Lua Plugin API. MCServer is compatible with the vanilla Minecraft client.

MCServer can run on Windows, *nix and Android operating systems. This includes Android phones and tablets as well as Raspberry Pis. 

We currently support Release 1.7 and 1.8 (not beta) Minecraft protocol versions.

Subscribe to [the newsletter](http://newsletter.cuberite.org/subscribe.htm) for important updates and project news.

Installation
------------
Hosted MCServer is available DIY on DigitalOcean: [![Install on DigitalOcean](http://doinstall.bearbin.net/button.svg)](http://doinstall.bearbin.net/install?url=https://github.com/mc-server/MCServer) and [Gamososm](https://gamocosm.com) also offers MCServer support.

For Linux there is an easy installation method, just run this in your terminal:

    curl -s https://raw.githubusercontent.com/mc-server/MCServer/master/easyinstall.sh | sh

For Windows, you just need to download a file and extract it:

 - [Windows 32 bit](http://builds.cuberite.org/job/MCServer%20Windows%20x86/lastSuccessfulBuild/artifact/Install/MCServer.zip)
 - [Windows 64 bit](http://builds.cuberite.org/job/MCServer%20Windows%20x64/lastSuccessfulBuild/artifact/Install/MCServer.zip)

For other operating systems you need to download and compile yourself. There is also an archive of binary builds on the buildserver: http://builds.cuberite.org

Compiling the server yourself has other benefits: you may get better performance performance (1.5-3x as fast) and it supports more operating systems. See the [COMPILING.md](https://github.com/mc-server/MCServer/blob/master/COMPILING.md) file for more details.

Contributing
------------

MCServer is licensed under the Apache License V2, and we welcome anybody to fork and submit a Pull Request back with their changes, and if you want to join as a permanent member we can add you to the team.

Check out the [CONTRIBUTING.md](https://github.com/mc-server/MCServer/blob/master/CONTRIBUTING.md) file for more details.

Other Stuff
-----------

For other stuff, including plugins and discussion, check the [forums](http://forum.mc-server.org) and [Plugin API](http://mc-server.xoft.cz/LuaAPI/).

Earn bitcoins for commits or donate to reward the MCServer developers: [![tip for next commit](http://tip4commit.com/projects/74.svg)](http://tip4commit.com/projects/74)

Support Us on Gratipay: [![Support via Gratipay](http://img.shields.io/gittip/cuberite_team.svg)](https://www.gratipay.com/cuberite_team)

Travis CI: [![Build Status](http://img.shields.io/travis/mc-server/MCServer.svg)](https://travis-ci.org/mc-server/MCServer)


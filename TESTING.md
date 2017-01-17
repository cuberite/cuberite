Contributing by testing Cuberite
===============================

You can help us even if you are not a programmer! This document explains how.
If you would like to help with coding instead, see `README.md` and `GETTING-STARTING.md`.

Here are things you can help with without being a programmer:
 - Run and test Cuberite. Report issues, bugs, crashes, and stacktraces at the [issue tracker](https://github.com/cuberite/cuberite/issues). This is best done in debug mode.
 - Test unmerged versions (known as branches or Pull Requests) which are marked with the [ingame-testing-needed](https://github.com/cuberite/cuberite/pulls?q=is%3Aopen+is%3Apr+label%3Astatus%2Fingame-testing-needed) label. 
   Note that code marked with `ingame-testing-needed` is not always available, and sometimes the above list will be empty.

Please use the issue tracker search feature and make sure your issue has not been reported before!

Obtaining Debug builds
----------------------

When testing Cuberite, debug builds are preferred because they output detailed info about the problem that had occured at the server console.
Currently, we do not provide debug builds. You will have to compile Cuberite yourself to obtain them. See the compilation instructions.
You can also test with regular builds.

Testing unmerged pull requests
------------------------------

To test unmerged branches, pick one from the [ingame-testing-needed](https://github.com/cuberite/cuberite/pulls?q=is%3Aopen+is%3Apr+label%3Aingame-testing-needed) list, and follow the download instructions.
If you are familiar with git, you can also fetch and compile the unmerged branch instead.

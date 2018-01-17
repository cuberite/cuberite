Contributing by Testing Cuberite
================================

You can help us even if you are not a programmer! This document explains how.
If you would like to help with coding instead, see [GETTING-STARTED.md][1] and [CONTRIBUTING.md][2].

Here are things you can help with without being a programmer:
 - Run and test Cuberite. Report issues, bugs, crashes, and stacktraces at the [issue tracker][3]. This is best done in debug mode.
 - Test unmerged versions (known as branches or Pull Requests) which are marked with the [ingame-testing-needed][4] label. 
   Note that code marked with `ingame-testing-needed` is not always available, and sometimes the above list will be empty.

Please use the issue tracker search feature and make sure your issue has not been reported before!

Obtaining Debug Builds
----------------------

When testing Cuberite, debug builds are preferred because they output detailed info about the problem that had occured at the server console.
Currently, we do not provide debug builds. You will have to compile Cuberite yourself to obtain them. See the compilation instructions.
You can also test with regular builds.

Testing Unmerged Pull Requests
------------------------------

To test unmerged branches, pick one from the ingame-testing-needed list, and follow the download instructions.
If you are familiar with git, you can also fetch and compile the unmerged branch instead.

[1]: https://github.com/cuberite/cuberite/blob/master/GETTING-STARTED.md
[2]: https://github.com/cuberite/cuberite/blob/master/CONTRIBUTING.md
[3]: https://github.com/cuberite/cuberite/issues
[4]: https://github.com/cuberite/cuberite/pulls?q=is%3Aopen+is%3Apr+label%3Astatus%2Fingame-testing-needed

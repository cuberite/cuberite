 #!/usr/bin/env bash
 
cmake . -DBUILD_TOOLS=1 -DSELF_TEST=1;
make -j 2;
make -j 2 test;
cd MCServer/;
echo stop |  gcov $MCSERVER_PATH;

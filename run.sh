set -e
make -j 4
cd Server
gdb -ex run Cuberite_debug
cd ..

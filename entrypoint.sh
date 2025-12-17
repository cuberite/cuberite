#!/bin/bash
set -e

# Copy default files to /server if they don't exist
echo "Syncing default files..."
rsync -a --ignore-existing /app/ /server/

# Always update the binary to match the image version
cp /app/Cuberite /server/Cuberite

# Ensure permissions
chown -R cuberite:cuberite /server

# If the first argument is a flag, prepend the executable
if [ "${1:0:1}" = '-' ]; then
    set -- ./Cuberite "$@"
fi

# If we are running Cuberite, use gosu to drop privileges
if [ "$1" = './Cuberite' ]; then
    exec gosu cuberite "$@"
fi

# Otherwise just exec the command
exec "$@"

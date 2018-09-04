#!/bin/sh

echo "Collecting shell files, This may take a while ..."

# For some reason shellcheck reports SC2016 for the '-exec sh ..' part,
# even though that is not the case.
# shellcheck disable=SC2016
SHELL_FILES=$(find . \
		   -type f \
		   \! \( -path './.git/*' -o -path './lib/*' -o -path './Server/Plugins/*' \) \
		   -exec sh -c 'head -n 1 "$1" | grep bin/sh>/dev/null' _ {}  \; -print)

# We actually need word splitting
# shellcheck disable=SC2086
exec shellcheck --shell=sh $SHELL_FILES

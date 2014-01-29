Code Stuff
----------

 * Because some devs use MSVC2008, we use C++03 - no C++11 magic for now at least :(
 * Use the provided wrappers for OS stuff:
  - Threading is done by inheriting from cIsThread, thread synchronization through cCriticalSection, cSemaphore and cEvent, file access and filesystem operations through the cFile class, high-precision timers through cTimer, high-precision sleep through cSleep
 * No magic numbers, use named constants:
  - E_ITEM_XXX, E_BLOCK_XXX and E_META_XXX for items and blocks
  - E_ENTITY_TYPE_XXX for mob types
  - dimNether, dimOverworld and dimEnd for world dimension
  - gmSurvival, gmCreative, gmAdventure for game modes
  - wSunny, wRain, wThunderstorm for weather
  - cChunkDef::Width, cChunkDef::Height for chunk dimensions (C++)
  - etc.
 * Instead of checking for specific value, use Is functions, if available:
  - cPlayer:IsGameModeCreative() instead of (cPlayer:GetGameMode() == gmCreative)
 * Please use tabs for indentation and spaces for alignment. This means that if it's at line start, it's a tab; if it's in the middle of a line, it's a space
 * Alpha-sort stuff that makes sense alpha-sorting - long lists of similar items etc.
 * Keep individual functions spaced out by 5 empty lines, this enhances readability and makes navigation in the source file easier.
 * Add those extra parentheses to conditions, especially in C++
  - "if ((a == 1) && ((b == 2) || (c == 3)))" instead of ambiguous "if (a == 1 && b == 2 || c == 3)"
  - This helps prevent mistakes such as "if (a & 1 == 0)"
 * White space is free, so use it freely
  - "freely" as in "plentifully", not "arbitrarily"
 * Each and every control statement deserves its braces. This helps maintainability later on when the file is edited, lines added or removed - the control logic doesn't break so easily.
 * Please leave the first line of all source files blank, to get around an IDE bug.
 * Also leave the last line of all source files blank (GCC and GIT can complain otherwise)


Copyright
---------

Your work should be licensed under the Apache license, and you should add yourself to the CONTRIBUTORS file.

If your work is not licensed under the Apache license, then it must be compatible and marked as such. Note that only plugins may choose a different license; MC-server's internals need to be single-license.

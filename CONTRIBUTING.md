Code Stuff
----------

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


Copyright
---------

Your work should be licensed under the Apache license, and you should add yourself to the CONTRIBUTORS file.

If your work is not licensed under the Apache license, then it must be compatible and marked as such. Note that only plugins may choose a different license; MC-server's internals need to be single-license.

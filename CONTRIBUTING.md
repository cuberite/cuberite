Code Stuff
----------

 * We use C++03 with some C++11 extensions (ask if you think that something would be useful)
 * Use the provided wrappers for OS stuff:
  - Threading is done by inheriting from `cIsThread`, thread synchronization through `cCriticalSection`, `cSemaphore` and `cEvent`, file access and filesystem operations through the `cFile` class, high-precision timers through `cTimer`, high-precision sleep through `cSleep`
 * No magic numbers, use named constants:
  - `E_ITEM_XXX`, `E_BLOCK_XXX` and `E_META_XXX` for items and blocks
  - `cEntity::etXXX` for entity types, `cMonster::mtXXX` for mob types
  - `dimNether`, `dimOverworld` and `dimEnd` for world dimension
  - `gmSurvival`, `gmCreative`, `gmAdventure` for game modes
  - `wSunny`, `wRain`, `wThunderstorm` for weather
  - `cChunkDef::Width`, `cChunkDef::Height` for chunk dimensions (C++)
  - etc.
 * Instead of checking for a specific value, use an `IsXXX` function, if available:
  - `cPlayer:IsGameModeCreative()` instead of` (cPlayer:GetGameMode() == gmCreative)` (the player can also inherit the gamemode from the world, which the value-d condition doesn't catch)
 * Please use **tabs for indentation and spaces for alignment**. This means that if it's at line start, it's a tab; if it's in the middle of a line, it's a space
 * Alpha-sort stuff that makes sense alpha-sorting - long lists of similar items etc.
 * Keep individual functions spaced out by 5 empty lines, this enhances readability and makes navigation in the source file easier.
 * Add those extra parentheses to conditions, especially in C++
  - `if ((a == 1) && ((b == 2) || (c == 3)))` instead of ambiguous `if (a == 1 && b == 2 || c == 3)`
  - This helps prevent mistakes such as `if (a & 1 == 0)`
 * White space is free, so use it freely
  - "freely" as in "plentifully", not "arbitrarily"
 * All `case` statements inside a `switch` need an extra indent.
 * Each and every control statement deserves its braces. This helps maintainability later on when the file is edited, lines added or removed - the control logic doesn't break so easily.
  - The only exception: a `switch` statement with all `case` statements being a single short statement is allowed to use the short brace-less form.
  - These two rules really mean that indent is governed by braces
 * Add an empty last line in all source files (GCC and GIT can complain otherwise)
 * All new public functions in all classes need documenting comments on what they do and what behavior they follow, use doxy-comments formatted as `/** Description */`. Do not use asterisks on additional lines in multi-line comments.
 * Use spaces after the comment markers: `// Comment` instead of `//Comment`


Copyright
---------

Your work must be licensed at least under the Apache license.

You can add yourself to the CONTRIBUTORS file if you wish.

**PLUGINS ONLY**: If your plugin is not licensed under the Apache license, then it must be compatible and marked as such. This is only valid for the plugins included within the MCServer source; plugins developed on separate repositories can use whatever license they want.

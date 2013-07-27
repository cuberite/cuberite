Core Plugin (Forked)
===========

A fork of MCServer's Core plugin.

**New Features:**
* Simplified commands, such as 'gotoworld' -> 'portal'
* Simplified and combined LUA files, such as 'listworlds.lua & gotoworld.lua' -> 'worlds-portal.lua'
* Fixed 'tp' command not working due to typography errors
* Fixed 'arithmetic on nil value' on startup due to inactivation of world limiter
* Massive overhaul / redesign of webadmin GUI interface.
  * Added jQuery transition effect
  * Completely redesigned CSS
  * Added new logo
  * Made HTML5 compliant
* Beautified 'help' menu
* Rewrite of death messages - fixed strange grammar and edited to more faithfully reflect Vanilla
* Added 'unban' console command

**How to Use**

Simply copy all LUA files into Plugins/Core (delete existing files first, except banned.ini and whitelist.ini!)
Then, copy webadmin to MCServer root directory (delete existing directory first!)

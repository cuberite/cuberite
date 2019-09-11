project(BlockTypePaletteGenerator)

file(COPY .
	DESTINATION ./
	PATTERN lib/* EXCLUDE
	PATTERN CMakeLists.txt EXCLUDE
	PATTERN Generator
	PATTERN *.bat
	PATTERN *.lua)

file(COPY
	lib/lunajson/src/lunajson
	lib/init.lua
	DESTINATION lib/ )

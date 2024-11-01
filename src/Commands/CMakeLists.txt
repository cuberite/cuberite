target_sources(
	${CMAKE_PROJECT_NAME} PRIVATE

	CommandManager.cpp

	CommandManager.h
    BasicStringReader.h
    CommandArguments.h
	CommandException.h
	CommandExecutionContext.h
)

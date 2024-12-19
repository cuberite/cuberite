#pragma once
#include "Protocol/Packetizer.h"
#include "BasicStringReader.h"
#include "CommandArguments.h"

class cCommandArgument;

class cCommandManager
{
public:
	enum class eCommandSuggestionType : std::uint8_t
	{
		None,
		AskServer,
		AllRecipes,
		AvailableSounds,
		SummonableEntities
	};
	enum class eNodeType : std::uint8_t
	{
		Root = 0,
		Literal,
		Argument
	};
	class cCommandNode
	{
		typedef std::vector<cCommandNode> CommandNodeList;
		typedef cFunctionRef<bool(cCommandExecutionContext & a_Ctx)> CommandExecutor;
		typedef std::shared_ptr<cCommandArgument> CmdArgPtr;
	public:
		/** Constructs a root node */
		cCommandNode();

		/** Adds the passed node to the current node's children */
		cCommandNode * Then(const cCommandNode * a_ChildNode);

		/* Constructs a literal node */
		static cCommandNode Literal(const AString & a_Name);

		static cCommandNode Argument(const AString & a_Name, CmdArgPtr a_Argument);

		//  TODO: implement CommandNode validation function

		/** Makes the nodes up to this point executable. The passed function is executed */
		cCommandNode * Executable(CommandExecutor a_Executioner);

		/** Parses the string as a command and executes it. Returns true if the parsing and execution succeeds */
		bool Parse(BasicStringReader & a_Command, cCommandExecutionContext & a_Ctx);

		/**  Writes the current node and all its children to the given packet in the format required by the CommandTree command. The current node must be a root node */
		void WriteCommandTree(cPacketizer & a_Packet, const cProtocol & a_Protocol);

	protected:
		cCommandNode(eNodeType a_Type, const CommandNodeList & a_ChildrenNodes, cCommandNode * a_RedirectNode, CmdArgPtr a_ParserArgument, const AString & a_Name, eCommandSuggestionType a_SuggestionType, bool a_IsExecutable, CommandExecutor a_Executioner);

		cCommandNode * GetLiteralCommandNode(const AString & a_NodeName);

		cCommandNode * GetNextPotentialNode(BasicStringReader & a_Reader, cCommandExecutionContext & a_Ctx);
		#if 0  // Disables copy constructors
			cCommandNode() = default;
			~cCommandNode() = default;

			cCommandNode(const cCommandNode &) = delete;
			void operator=(const cCommandNode &x) = delete;
		#endif

	private:

		/** Does most of the actual writing. Called recursively. */
		void WriteCommandTreeInternal(cPacketizer & a_Packet, std::map<cCommandNode *, UInt32> & a_Map, const cProtocol & a_Protocol);

		/* Assign ids in order given by CollectChildren */
		static std::map<cCommandNode *, UInt32> ComputeChildrenIds(cCommandNode & a_Node);

		/** Put the pointer of the given node and of all it children recursively in a list in a depth first order */
		static std::vector<cCommandNode *> CollectChildren(std::vector<cCommandNode *> & a_List, cCommandNode & a_Node);

		bool m_IsExecutable;
		CommandExecutor m_Executioner;
		eNodeType m_Type;
		CommandNodeList m_ChildrenNodes;
		cCommandNode * m_RedirectNode;
		CmdArgPtr m_Argument;
		AString m_Name;
		eCommandSuggestionType m_SuggestionType;
	};
};



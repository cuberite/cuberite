/*
** See Copyright Notice in license.html
*/

#define ParserType	"Expat"

#define StartCdataKey			"StartCdataSection"
#define EndCdataKey			"EndCdataSection"
#define CharDataKey			"CharacterData"
#define CommentKey			"Comment"
#define DefaultKey			"Default"
#define DefaultExpandKey		"DefaultExpand"
#define StartElementKey			"StartElement"
#define EndElementKey			"EndElement"
#define ExternalEntityKey		"ExternalEntityRef"
#define StartNamespaceDeclKey		"StartNamespaceDecl"
#define EndNamespaceDeclKey		"EndNamespaceDecl"
#define NotationDeclKey			"NotationDecl"
#define NotStandaloneKey		"NotStandalone"
#define ProcessingInstructionKey	"ProcessingInstruction"
#define UnparsedEntityDeclKey		"UnparsedEntityDecl"
#define StartDoctypeDeclKey		"StartDoctypeDecl"

int luaopen_lxp (lua_State *L);

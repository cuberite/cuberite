// SignEntity.h

// Declares the cSignEntity class representing a single sign in the world





#pragma once

#include "BlockEntity.h"





// tolua_begin

class cSignEntity :
	public cBlockEntity
{
	// tolua_end

	using Super = cBlockEntity;

public:  // tolua_export

	/** Creates a new empty sign entity at the specified block coords and block type (wall or standing). a_World may be nullptr */
	cSignEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);

	// tolua_begin

	/** Sets all the sign's lines */
	void SetLines(const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4);

	/** Sets individual line (zero-based index) */
	void SetLine(int a_Index, const AString & a_Line);

	/** Retrieves individual line (zero-based index) */
	AString GetLine(int a_Index) const;

	// tolua_end

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;

private:

	AString  m_Line[4];
} ;  // tolua_export

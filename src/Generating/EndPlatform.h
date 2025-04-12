#pragma once

#include "World.h"

class cEndPlatform
{
	public:
		/** Generates the End Obsidian Platform that players spawn on. It is called by cEntity::DetectPortal */
		static void Generate(cWorld * m_EndWorld);
};

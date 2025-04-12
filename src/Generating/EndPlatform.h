#pragma once

#include "World.h"

class EndPlatform
{
	public:
		/** Generates the End Obsidian Platform that players spawn on. It is called by cEntity::DetectPortal */
		static void Generate(cWorld * m_EndWorld);
};


// ScoreboardSerializer.cpp


#include "Globals.h"
#include "ScoreboardSerializer.h"
#include "../StringCompression.h"
#include "zlib/zlib.h"
#include "FastNBT.h"

#include "../Scoreboard.h"





cScoreboardSerializer::cScoreboardSerializer(const AString & a_WorldName, cScoreboard * a_Scoreboard):
	m_Scoreboard(a_Scoreboard)
{
	AString DataPath;
	Printf(DataPath, "%s%cdata", a_WorldName.c_str(), cFile::PathSeparator);

	m_Path = DataPath + cFile::PathSeparator + "scoreboard.dat";

	cFile::CreateFolder(FILE_IO_PREFIX + DataPath);
}





bool cScoreboardSerializer::Load(void)
{
	AString Data = cFile::ReadWholeFile(FILE_IO_PREFIX + m_Path);
	if (Data.empty())
	{
		return false;
	}

	AString Uncompressed;
	int res = UncompressStringGZIP(Data.data(), Data.size(), Uncompressed);

	if (res != Z_OK)
	{
		return false;
	}

	// Parse the NBT data:
	cParsedNBT NBT(Uncompressed.data(), Uncompressed.size());
	if (!NBT.IsValid())
	{
		// NBT Parsing failed
		return false;
	}

	return LoadScoreboardFromNBT(NBT);
}





bool cScoreboardSerializer::Save(void)
{
	cFastNBTWriter Writer;

	SaveScoreboardToNBT(Writer);

	Writer.Finish();

	#ifdef _DEBUG
	cParsedNBT TestParse(Writer.GetResult().data(), Writer.GetResult().size());
	ASSERT(TestParse.IsValid());
	#endif  // _DEBUG

	cFile File;
	if (!File.Open(FILE_IO_PREFIX + m_Path, cFile::fmWrite))
	{
		return false;
	}

	AString Compressed;
	int res = CompressStringGZIP(Writer.GetResult().data(), Writer.GetResult().size(), Compressed);

	if (res != Z_OK)
	{
		return false;
	}

	File.Write(Compressed.data(), Compressed.size());
	File.Close();

	return true;
}





void cScoreboardSerializer::SaveScoreboardToNBT(cFastNBTWriter & a_Writer)
{
	a_Writer.BeginCompound("data");

	a_Writer.BeginList("Objectives", TAG_Compound);

	for (cScoreboard::cObjectiveMap::const_iterator it = m_Scoreboard->m_Objectives.begin(); it != m_Scoreboard->m_Objectives.end(); ++it)
	{
		const cObjective & Objective = it->second;

		a_Writer.BeginCompound("");

		a_Writer.AddString("CriteriaName", cObjective::CriteriaToString(Objective.GetType()));

		a_Writer.AddString("DisplayName", Objective.GetDisplayName());
		a_Writer.AddString("Name", it->first);
		a_Writer.AddString("RenderType", (Objective.GetDisplayType() == cObjective::dispHearts) ? "hearts" : "integer");

		a_Writer.EndCompound();
	}

	a_Writer.EndList();  // Objectives

	a_Writer.BeginList("PlayerScores", TAG_Compound);

	for (cScoreboard::cObjectiveMap::const_iterator it = m_Scoreboard->m_Objectives.begin(); it != m_Scoreboard->m_Objectives.end(); ++it)
	{
		const cObjective & Objective = it->second;

		for (cObjective::cScoreMap::const_iterator it2 = Objective.m_Scores.begin(); it2 != Objective.m_Scores.end(); ++it2)
		{
			a_Writer.BeginCompound("");

			a_Writer.AddInt("Score", it2->second);

			a_Writer.AddString("Name", it2->first);
			a_Writer.AddString("Objective", it->first);

			a_Writer.EndCompound();
		}
	}

	a_Writer.EndList();  // PlayerScores

	a_Writer.BeginList("Teams", TAG_Compound);

	for (cScoreboard::cTeamMap::const_iterator it = m_Scoreboard->m_Teams.begin(); it != m_Scoreboard->m_Teams.end(); ++it)
	{
		const cTeam & Team = it->second;

		a_Writer.BeginCompound("");

		a_Writer.AddByte("AllowFriendlyFire",     Team.AllowsFriendlyFire()      ? 1 : 0);
		a_Writer.AddByte("SeeFriendlyInvisibles", Team.CanSeeFriendlyInvisible() ? 1 : 0);

		a_Writer.AddString("DisplayName", Team.GetDisplayName());
		a_Writer.AddString("Name", it->first);

		a_Writer.AddString("Prefix", Team.GetPrefix());
		a_Writer.AddString("Suffix", Team.GetSuffix());

		a_Writer.BeginList("Players", TAG_String);

		for (cTeam::cPlayerNameSet::const_iterator it2 = Team.m_Players.begin(); it2 != Team.m_Players.end(); ++it2)
		{
			a_Writer.AddString("", *it2);
		}

		a_Writer.EndList();

		a_Writer.EndCompound();
	}

	a_Writer.EndList();  // Teams

	a_Writer.BeginCompound("DisplaySlots");

	for (int DisplaySlot = 0; DisplaySlot < cScoreboard::dsCount; ++DisplaySlot)
	{
		cObjective * Objective = m_Scoreboard->GetObjectiveIn(static_cast<cScoreboard::eDisplaySlot>(DisplaySlot));
		a_Writer.AddString(Printf("slot_%d", DisplaySlot), (Objective == nullptr) ? "" : Objective->GetName());
	}

	a_Writer.EndCompound();  // DisplaySlots

	a_Writer.EndCompound();  // Data
}





bool cScoreboardSerializer::LoadScoreboardFromNBT(const cParsedNBT & a_NBT)
{
	int Data = a_NBT.FindChildByName(0, "data");
	if (Data < 0)
	{
		return false;
	}

	int Objectives = a_NBT.FindChildByName(Data, "Objectives");
	if (Objectives < 0)
	{
		return false;
	}

	for (int Child = a_NBT.GetFirstChild(Objectives); Child >= 0; Child = a_NBT.GetNextSibling(Child))
	{
		AString CriteriaName, DisplayName, Name, RenderType;

		int CurrLine = a_NBT.FindChildByName(Child, "CriteriaName");
		if (CurrLine >= 0)
		{
			CriteriaName = a_NBT.GetString(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "DisplayName");
		if (CurrLine >= 0)
		{
			DisplayName = a_NBT.GetString(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "Name");
		if (CurrLine >= 0)
		{
			Name = a_NBT.GetString(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "RenderType");
		if (CurrLine >= 0)
		{
			RenderType = a_NBT.GetString(CurrLine);
		}

		cObjective::Criteria Type = cObjective::StringToCriteria(CriteriaName);

		cObjective * Objective = m_Scoreboard->RegisterObjective(Name, DisplayName, Type);
		if (RenderType == "hearts")
		{
			Objective->SetDisplayType(cObjective::dispHearts);
		}
	}

	int PlayerScores = a_NBT.FindChildByName(Data, "PlayerScores");
	if (PlayerScores < 0)
	{
		return false;
	}

	for (int Child = a_NBT.GetFirstChild(PlayerScores); Child >= 0; Child = a_NBT.GetNextSibling(Child))
	{
		AString Name, ObjectiveName;

		cObjective::Score Score = 0;

		int CurrLine = a_NBT.FindChildByName(Child, "Score");
		if (CurrLine >= 0)
		{
			Score = a_NBT.GetInt(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "Name");
		if (CurrLine >= 0)
		{
			Name = a_NBT.GetString(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "Objective");
		if (CurrLine >= 0)
		{
			ObjectiveName = a_NBT.GetString(CurrLine);
		}

		cObjective * Objective = m_Scoreboard->GetObjective(ObjectiveName);

		if (Objective)
		{
			Objective->SetScore(Name, Score);
		}
	}

	int Teams = a_NBT.FindChildByName(Data, "Teams");
	if (Teams < 0)
	{
		return false;
	}

	for (int Child = a_NBT.GetFirstChild(Teams); Child >= 0; Child = a_NBT.GetNextSibling(Child))
	{
		AString Name, DisplayName, Prefix, Suffix;

		bool AllowsFriendlyFire = true, CanSeeFriendlyInvisible = false;

		int CurrLine = a_NBT.FindChildByName(Child, "Name");
		if ((CurrLine >= 0) && (a_NBT.GetType(CurrLine) == TAG_String))
		{
			Name = a_NBT.GetString(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "DisplayName");
		if ((CurrLine >= 0) && (a_NBT.GetType(CurrLine) == TAG_String))
		{
			DisplayName = a_NBT.GetString(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "Prefix");
		if ((CurrLine >= 0) && (a_NBT.GetType(CurrLine) == TAG_String))
		{
			Prefix = a_NBT.GetString(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "Suffix");
		if ((CurrLine >= 0) && (a_NBT.GetType(CurrLine) == TAG_String))
		{
			Suffix = a_NBT.GetString(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "AllowFriendlyFire");
		if ((CurrLine >= 0) && (a_NBT.GetType(CurrLine) == TAG_Int))
		{
			AllowsFriendlyFire = (a_NBT.GetInt(CurrLine) != 0);
		}

		CurrLine = a_NBT.FindChildByName(Child, "SeeFriendlyInvisibles");
		if ((CurrLine >= 0) && (a_NBT.GetType(CurrLine) == TAG_Int))
		{
			CanSeeFriendlyInvisible = (a_NBT.GetInt(CurrLine) != 0);
		}

		cTeam * Team = m_Scoreboard->RegisterTeam(Name, DisplayName, Prefix, Suffix);

		Team->SetFriendlyFire(AllowsFriendlyFire);
		Team->SetCanSeeFriendlyInvisible(CanSeeFriendlyInvisible);

		int Players = a_NBT.FindChildByName(Child, "Players");
		if (Players < 0)
		{
			continue;
		}

		for (int ChildB = a_NBT.GetFirstChild(Players); ChildB >= 0; ChildB = a_NBT.GetNextSibling(ChildB))
		{
			Team->AddPlayer(a_NBT.GetString(ChildB));
		}
	}

	int DisplaySlots = a_NBT.FindChildByName(Data, "DisplaySlots");
	if (DisplaySlots < 0)
	{
		return false;
	}

	for (int DisplaySlot = 0; DisplaySlot < cScoreboard::dsCount; ++DisplaySlot)
	{
		int CurrLine = a_NBT.FindChildByName(DisplaySlots, Printf("slot_%d", DisplaySlot));
		if (CurrLine >= 0)
		{
			AString Name = a_NBT.GetString(CurrLine);
			m_Scoreboard->SetDisplay(Name, static_cast<cScoreboard::eDisplaySlot>(DisplaySlot));
		}
	}

	return true;
}









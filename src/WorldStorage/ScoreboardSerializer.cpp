
// ScoreboardSerializer.cpp


#include "Globals.h"
#include "ScoreboardSerializer.h"
#include "../StringCompression.h"
#include "zlib/zlib.h"
#include "FastNBT.h"

#include "../Scoreboard.h"




#define SCOREBOARD_INFLATE_MAX 16 KiB





cScoreboardSerializer::cScoreboardSerializer(const AString & a_WorldName, cScoreboard* a_ScoreBoard)
	: m_ScoreBoard(a_ScoreBoard)
{
	Printf(m_Path, "%s/data/scoreboard.dat", a_WorldName.c_str());
}





bool cScoreboardSerializer::Load(void)
{
	cFile File;

	if (!File.Open(m_Path, cFile::fmReadWrite))
	{
		return false;
	}

	AString Data;

	File.ReadRestOfFile(Data);

	File.Close();

	char Uncompressed[SCOREBOARD_INFLATE_MAX];
	z_stream strm;
	strm.zalloc = (alloc_func)NULL;
	strm.zfree = (free_func)NULL;
	strm.opaque = NULL;
	inflateInit(&strm);
	strm.next_out  = (Bytef *)Uncompressed;
	strm.avail_out = sizeof(Uncompressed);
	strm.next_in   = (Bytef *)Data.data();
	strm.avail_in  = Data.size();
	int res = inflate(&strm, Z_FINISH);
	inflateEnd(&strm);
	if (res != Z_STREAM_END)
	{
		return false;
	}
	
	// Parse the NBT data:
	cParsedNBT NBT(Uncompressed, strm.total_out);
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

	Writer.BeginCompound("");
	
	SaveScoreboardToNBT(Writer);

	Writer.EndCompound();
	Writer.Finish();
	
	#ifdef _DEBUG
	cParsedNBT TestParse(Writer.GetResult().data(), Writer.GetResult().size());
	ASSERT(TestParse.IsValid());
	#endif  // _DEBUG
	
	gzFile gz = gzopen((FILE_IO_PREFIX + m_Path).c_str(), "wb");
	if (gz != NULL)
	{
		gzwrite(gz, Writer.GetResult().data(), Writer.GetResult().size());
	}
	gzclose(gz);

	return true;
}





void cScoreboardSerializer::SaveScoreboardToNBT(cFastNBTWriter & a_Writer)
{
	a_Writer.BeginCompound("Data");
		a_Writer.BeginList("Objectives", TAG_Compound);

		a_Writer.EndList();

		a_Writer.BeginList("PlayerScores", TAG_Compound);

		a_Writer.EndList();

		a_Writer.BeginList("Teams", TAG_Compound);

		a_Writer.EndList();
	a_Writer.EndCompound();

	a_Writer.BeginCompound("DisplaySlots");

	a_Writer.EndCompound();
}





bool cScoreboardSerializer::LoadScoreboardFromNBT(const cParsedNBT & a_NBT)
{
	int Data = a_NBT.FindChildByName(0, "Data");
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
		AString CriteriaName, DisplayName, Name;

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

		cObjective::eType Type = cObjective::StringToType(CriteriaName);

		m_ScoreBoard->RegisterObjective(Name, DisplayName, Type);
	}

	int PlayerScores = a_NBT.FindChildByName(Data, "PlayerScores");
	if (PlayerScores < 0)
	{
		return false;
	}

	for (int Child = a_NBT.GetFirstChild(PlayerScores); Child >= 0; Child = a_NBT.GetNextSibling(Child))
	{
		AString Name, ObjectiveName;

		cObjective::Score Score;

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

		cObjective * Objective = m_ScoreBoard->GetObjective(ObjectiveName);

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

		bool AllowsFriendlyFire, CanSeeFriendlyInvisible;

		int CurrLine = a_NBT.FindChildByName(Child, "Name");
		if (CurrLine >= 0)
		{
			Name = a_NBT.GetInt(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "DisplayName");
		if (CurrLine >= 0)
		{
			DisplayName = a_NBT.GetInt(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "Prefix");
		if (CurrLine >= 0)
		{
			Prefix = a_NBT.GetInt(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "Suffix");
		if (CurrLine >= 0)
		{
			Suffix = a_NBT.GetInt(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "AllowFriendlyFire");
		if (CurrLine >= 0)
		{
			AllowsFriendlyFire = a_NBT.GetInt(CurrLine);
		}

		CurrLine = a_NBT.FindChildByName(Child, "SeeFriendlyInvisibles");
		if (CurrLine >= 0)
		{
			CanSeeFriendlyInvisible = a_NBT.GetInt(CurrLine);
		}

		cTeam * Team = m_ScoreBoard->RegisterTeam(Name, DisplayName, Prefix, Suffix);

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

	int DisplaySlots = a_NBT.FindChildByName(0, "DisplaySlots");
	if (DisplaySlots < 0)
	{
		return false;
	}

	int CurrLine = a_NBT.FindChildByName(DisplaySlots, "slot_0");
	if (CurrLine >= 0)
	{
		AString Name = a_NBT.GetString(CurrLine);

		m_ScoreBoard->SetDisplay(Name, cScoreboard::E_DISPLAY_SLOT_LIST);
	}

	CurrLine = a_NBT.FindChildByName(DisplaySlots, "slot_1");
	if (CurrLine >= 0)
	{
		AString Name = a_NBT.GetString(CurrLine);

		m_ScoreBoard->SetDisplay(Name, cScoreboard::E_DISPLAY_SLOT_SIDEBAR);
	}

	CurrLine = a_NBT.FindChildByName(DisplaySlots, "slot_2");
	if (CurrLine >= 0)
	{
		AString Name = a_NBT.GetString(CurrLine);

		m_ScoreBoard->SetDisplay(Name, cScoreboard::E_DISPLAY_SLOT_NAME);
	}

	return true;
}









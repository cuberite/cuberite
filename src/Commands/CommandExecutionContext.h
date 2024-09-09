#pragma once
#include "../Entities/Player.h"
#include "../StringUtils.h"
#include "CommandException.h"
#include <any>

class cCommandExecutionContext
{

  public:
	cCommandExecutionContext(cPlayer* const a_PlayerExecutor) :
		a_PlayerExecutor(a_PlayerExecutor)
	{
	}

	void AddValue(const AString& a_Name, const std::any& a_Value)
	{
		m_ArgumentMap[a_Name] = a_Value;
	}

	std::any GetValue(const AString& a_Name)
	{
		return m_ArgumentMap[a_Name];		
	}

	//TODO: add support for command block and all entities
	void SendFeedback(const AString& a_ToSend) const
	{
		if (a_PlayerExecutor != nullptr)
		{
			a_PlayerExecutor->SendMessageInfo(a_ToSend);
		}
		else
		{
			LOGD(a_ToSend);
		}
	}

	cPlayer* GetPlayer() const
	{
		if (a_PlayerExecutor == nullptr)
		{
			throw cCommandExecutionException("Player Entity required");
		}
		return a_PlayerExecutor;		
	}

  private:
	std::map<AString, std::any> m_ArgumentMap;
	cPlayer* a_PlayerExecutor;
};

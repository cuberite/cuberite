
#pragma once

class cClientAction
{
public:
	enum class Type
	{
		StatusRequest,
		StatusPing,
		LoginStart,
		LoginConfirmation,
		Animation,
		LeftClick,
		RightClick,
		Chat,
		SetLocale,
		SetViewDistance,
		Respawn,
		StatsRequest,
		Achivement,
		CreativeInventory,
		SetProtocolVersion
	};

	cClientAction(Type a_Type) : m_Type(a_Type) {}

	Type GetType() const { return m_Type; }

private:
	Type m_Type;
};

class cSimpleAction : public cClientAction
{
public:
	cSimpleAction(Type a_type) : cClientAction(a_type) {}
};

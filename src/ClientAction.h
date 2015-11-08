
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
		SetProtocolVersion,
		KeepAlive,
		UseEntity,
		PlayerPos,
		PlayerLook,
		SlotSelected,
		EntityCrouch,
		EntityLeaveBed,
		EntitySprinting,
		Unmount,
		SteerVehicle,
		WindowClose,
		WindowClick,
		EnchantItem,
		UpdateSign,
		PlayerAbilities,
		TabComplete,
		PluginMessage,
		CommandBlockChange,
		UnhandledCommandBlockCommand,
		SetBrand,
		BeaconSelection,
		AnvilItemName,
		NPCTrade,
		SetIPString,
		SetUUID,
		SetProperties
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

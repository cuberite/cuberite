
#include "Globals.h"
#include "BlockEntityTagSerializer.h"
#include "../ItemGrid.h"





void BlockEntityTagSerializer::WriteToJson(Json::Value & a_TagJson, const cItem & a_Item, const cItemGrid & a_Items)
{
	if (!a_Item.IsCustomNameEmpty())
	{
		a_TagJson["CustomName"] = a_Item.m_CustomName;
	}
	a_TagJson["Lock"] = "";
	a_TagJson["Items"] = Json::Value(Json::arrayValue);
	for (int i = 0; i < a_Items.GetNumSlots(); i++)
	{
		Json::Value Slot;
		a_Items.GetSlot(i).GetJson(Slot);
		a_TagJson["Items"].append(Slot);
	}
}





void BlockEntityTagSerializer::ParseFromJson(const Json::Value & a_Value, cItemGrid & a_Items)
{
	int SlotIdx = 0;
	for (auto & Node : a_Value["Items"])
	{
		cItem Item;
		Item.FromJson(Node);
		a_Items.SetSlot(SlotIdx, Item);
		SlotIdx++;
	}
}





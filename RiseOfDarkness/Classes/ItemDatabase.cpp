#include "ItemDatabase.h"

ItemDatabase::ItemDatabase()
{
}

ItemDatabase::~ItemDatabase()
{
}

void ItemDatabase::Init()
{
	for (int i = 1; i < 21; i++)
	{
		items.push_back(new Item("sprites/item/items_weapon_"+
			(i<10?"0"+std::to_string(i):std::to_string(i))
			, i, "item description", 50, 0, ItemType::weapon));
	}
	
}

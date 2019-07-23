#include "ItemDatabase.h"

ItemDatabase::ItemDatabase()
{
}

ItemDatabase::~ItemDatabase()
{
}

void ItemDatabase::Init()
{
	/*for (int i = 1; i < 2; i++)
	{
		items.push_back(new Item("sprites/item/items_potion_"+
			(i<10?"0"+std::to_string(i):std::to_string(i))
			, i, "Restores 50 HP", 50, 0, ItemType::weapon));
	}*/
	items.push_back(new Item("sprites/item/items_potion_01"
		
		, 0, "Restores 50 HP", 50, 0, ItemType::potion));
	items.push_back(new Item("sprites/item/items_potion_02"

		, 1, "Restores 50 MP", 50, 0, ItemType::potion));
}

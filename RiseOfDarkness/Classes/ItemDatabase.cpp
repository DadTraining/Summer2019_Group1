#include "ItemDatabase.h"

ItemDatabase::ItemDatabase()
{
}

ItemDatabase::~ItemDatabase()
{
}

void ItemDatabase::Init()
{
	items.push_back(new Item("sprites/item/items_potion_01"
		
		, 0, "Restores 50 HP", 50, 0, ItemType::potion));
	items.push_back(new Item("sprites/item/items_potion_02"

		, 1, "Restores 50 MP", 50, 0, ItemType::potion));
	items.push_back(new Item("sprites/item/items_potion_03"

		, 2, "Restores 50 HP", 50, 0, ItemType::potion));
	items.push_back(new Item("sprites/item/items_potion_04"

		, 3, "Restores 50 HP", 50, 0, ItemType::potion));
	for (int i = items.size(); i < 21; i++)
	{
		items.push_back(new Item("sprites/item/items_weapon_"+
			(i<10?"0"+std::to_string(i):std::to_string(i))
			, i, "Increase attack damage", 10, 0, ItemType::weapon));
	}
}

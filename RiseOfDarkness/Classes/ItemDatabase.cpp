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

		, 2, "Apple", 50, 0, ItemType::potion));
	items.push_back(new Item("sprites/item/items_potion_04"

		, 3, "Restores 50 HP", 50, 0, ItemType::potion));
	for (int i = items.size(); i < 10; i++)
	{
		items.push_back(new Item("sprites/item/items_sword_"+
			(i+1<10?"0"+std::to_string(i+1):std::to_string(i+1))
			, i, "Increase attack damage", 10, 0, ItemType::weapon, WeaponType::sword));
	}
	for (int i = items.size(); i < 15; i++)
	{
		items.push_back(new Item("sprites/item/items_bow_" +
			(i+1<10 ? "0" + std::to_string(i+1) : std::to_string(i+1))
			, i, "Increase bow attack damage", 10, 0, ItemType::weapon, WeaponType::bow));
	}
	for (int i = items.size(); i < 18; i++)
	{
		items.push_back(new Item("sprites/item/items_arrow_" +
			(i+1<10 ? "0" + std::to_string(i+1) : std::to_string(i+1))
			, i, "Arrow effect", 10, 0, ItemType::arrow, WeaponType::arrow));
	}
	items.push_back(new Item("sprites/item/box"));
}

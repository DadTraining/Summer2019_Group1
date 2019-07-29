#include "ItemDatabase.h"

ItemDatabase::ItemDatabase()
{
}

ItemDatabase::~ItemDatabase()
{
}

void ItemDatabase::Init()
{
	items.push_back(new Item("sprites/item/itemIcon/items (92)"
		
		, 0, "Restores 50 HP", 50, 10, ItemType::potion));
	items.push_back(new Item("sprites/item/itemIcon/items (3)"

		, 1, "Restores 50 MP", 50, 10, ItemType::potion));
	items.push_back(new Item("sprites/item/itemIcon/items (91)"

		, 2, "Apple", 50, 10, ItemType::potion));
	items.push_back(new Item("sprites/item/itemIcon/items (4)"

		, 3, "Restores 50 HP", 50, 10, ItemType::potion));
	for (int i = 7; i < 22; i++)
	{
		items.push_back(new Item("sprites/item/itemIcon/items ("+
			std::to_string(i)+")"
			, i, "Increase attack damage", 10, 100, ItemType::weapon, WeaponType::sword));
	}
	for (int i = 23; i < 38; i++)
	{
		items.push_back(new Item("sprites/item/itemIcon/items (" +
			std::to_string(i)+")"
			, i, "Increase bow attack damage", 10, 100, ItemType::weapon, WeaponType::bow));
	}
	for (int i = 70; i < 79; i++)
	{
		items.push_back(new Item("sprites/item/itemIcon/items (" +
			std::to_string(i) +")"
			, i, "Arrow effect", 10, 2, ItemType::arrow, WeaponType::arrow));
	}
	items.push_back(new Item("sprites/item/box"));
}

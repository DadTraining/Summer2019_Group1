#include "ItemDatabase.h"
#include "cocos2d.h"
USING_NS_CC;
ItemDatabase::ItemDatabase()
{
}

ItemDatabase::~ItemDatabase()
{
}

void ItemDatabase::Init()
{
	std::string data, temp;
	int num;
	int id;
	data = FileUtils::getInstance()->getStringFromFile("res/sprites/item/itemData.bin");
	std::istringstream iss(data);
	// Armor
	iss >> num;
	for (int i = 1; i <= num; i++)
	{
		iss >> id;
		iss >> temp;
		temp.replace(temp.find("__"), 2, " ");
		items.push_back(new Item(
			temp, id, "Armor", 20, 50, ItemType::armor)
		);
	}
	iss >> num;
	for (int i = 1; i <= num; i++)
	{
		iss >> id;
		iss >> temp;
		temp.replace(temp.find("__"), 2, " ");
		items.push_back(new Item(
			temp, id, "Arrow", 10, 2, ItemType::arrow, WeaponType::arrow)
		);
	}
	iss >> num;
	for (int i = 1; i <= num; i++)
	{
		iss >> id;
		iss >> temp;
		temp.replace(temp.find("__"), 2, " ");
		items.push_back(new Item(
			temp, id, "Boots", 5, 30, ItemType::boots)
		);
	}
	iss >> num;
	for (int i = 1; i <= num; i++)
	{
		iss >> id;
		iss >> temp;
		temp.replace(temp.find("__"), 2, " ");
		items.push_back(new Item(
			temp, id, "Bow", 10, 100, ItemType::weapon, WeaponType::bow)
		);
	}
	iss >> num;
	for (int i = 1; i <= num; i++)
	{
		iss >> id;
		iss >> temp;
		temp.replace(temp.find("__"), 2, " ");
		items.push_back(new Item(
			temp, id, "Potion", 50, 10, ItemType::potion)
		);
	}
	iss >> num;
	for (int i = 1; i <= num; i++)
	{
		iss >> id;
		iss >> temp;
		temp.replace(temp.find("__"), 2, " ");
		items.push_back(new Item(
			temp, id, "Sword", 10, 1000, ItemType::weapon, WeaponType::sword)
		);
	}
	
	items.push_back(new Item("sprites/item/box"));
}

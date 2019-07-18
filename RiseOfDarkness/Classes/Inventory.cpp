#include "Item.h"
#include "Inventory.h"
#include "Armor.h"
#include "Potion.h"
#include "Weapon.h"

using namespace std;
Inventory::Inventory(Sprite* sprite)
{
	capacity = 24;
}

Inventory::~Inventory()
{
}

void Inventory::Init()
{
	
}

void Inventory::AddItem(int)
{

}

void Inventory::DropItem(int)
{
}

void Inventory::UseItem(int)
{
}

void Inventory::SetCapacity(int)
{
}

int Inventory::GetCapacity()
{
	return capacity;
}

std::vector<Item*> Inventory::GetItems()
{
	return std::vector<Item*>();
}

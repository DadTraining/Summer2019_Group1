#include "Item.h"
#include "Inventory.h"
#include "Armor.h"
#include "Potion.h"
#include "Weapon.h"

using namespace std;
Inventory::Inventory()
{
	capacity = 24;
}

Inventory::~Inventory()
{
}

void Inventory::Init()
{
	items.reserve(capacity);
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

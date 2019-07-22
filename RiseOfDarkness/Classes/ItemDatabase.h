#include "Item.h"

class ItemDatabase
{
public:
	ItemDatabase();
	~ItemDatabase();
	void Init();
	std::vector<Item*> items;
};

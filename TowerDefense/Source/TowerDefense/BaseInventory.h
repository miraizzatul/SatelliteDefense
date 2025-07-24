#pragma once
#include <vector>

struct LootItem;

class BaseInventory
{
public:
	void AddItem(const LootItem& item);
	void DebugInventory();

private:
	std::vector<LootItem> items;
};
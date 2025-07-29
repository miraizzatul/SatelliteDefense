#pragma once
#include <vector>
#include <string>

struct LootItem;

class BaseInventory
{
public:
	void AddItem(const LootItem& item);
	const std::vector<LootItem>& GetItems() const;
	void DebugInventory();
	bool HasItem(const LootItem* itemToLookFor) const;
	bool ConsumeItem(const LootItem& itemToConsume);

private:
	std::vector<LootItem> items;
};
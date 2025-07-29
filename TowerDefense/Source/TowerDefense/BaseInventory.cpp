#include "BaseInventory.h"
#include "BaseActor.h"
#include <SDL3/SDL.h>

void BaseInventory::AddItem(const LootItem& item)
{
	for (LootItem& i : items)
	{
		if (i.name == item.name)
		{
			i.quantity += item.quantity;
			return;
		}
	}
	items.push_back(item);
}

const std::vector<LootItem>& BaseInventory::GetItems() const
{
	return items;
}

void BaseInventory::DebugInventory()
{
	for (const LootItem& item : items)
	{
		SDL_Log("Item: %s x%d", item.name.c_str(), item.quantity);
	}
}

bool BaseInventory::HasItem(const LootItem* itemToLookFor) const
{
	for (const LootItem& item : items)
	{
		if (item.name == itemToLookFor->name && item.quantity >= itemToLookFor->quantity)
		{
			return true;
		}
	}
	return false;
}

bool BaseInventory::ConsumeItem(const LootItem& itemToConsume)
{
	for (LootItem& item : items)
	{
		if (item.name == itemToConsume.name && item.quantity >= itemToConsume.quantity)
		{
			item.quantity -= itemToConsume.quantity;
			if (item.quantity == 0)
			{
				//remove all items that is equal to item
				items.erase(std::remove(items.begin(), items.end(), item), items.end());
			}
			return true;
		}
	}
	return false;
}

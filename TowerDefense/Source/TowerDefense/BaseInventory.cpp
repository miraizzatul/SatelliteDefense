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

void BaseInventory::DebugInventory()
{
	for (const auto& item : items)
	{
		SDL_Log("Item: %s x%d", item.name.c_str(), item.quantity);
	}
}

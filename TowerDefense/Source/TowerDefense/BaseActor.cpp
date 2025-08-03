#include "BaseActor.h"
#include <SDL3/SDL_log.h>

BaseActor::BaseActor(int id, SDL_Color* newColor, EventHandler<BaseActor&>* onDestroyed)
	:objectID(id), actorColor(*newColor), OnDestroyed(onDestroyed)
{
}

int BaseActor::GetID() const
{
	return objectID;
}

Faction BaseActor::GetFaction() const
{
	return faction;
}

void BaseActor::AddLoot(const LootItem& item)
{
	loot.push_back(item);
}

const std::vector<LootItem>& BaseActor::GetLoot() const
{
	return loot;
}

void BaseActor::TakeDamage(float amount)
{
	currentHP -= amount;
	if (currentHP <= 0.f)
	{
		isDestroyed = true;
		if (IsDestroyed() && !hasBroadcasted)
		{
			hasBroadcasted = true;
			if (OnDestroyed)
			{
				OnDestroyed->Broadcast(*this);
				SDL_Log("Destroyed");
			}
		}
	}
}

float BaseActor::GetHealth() const
{
	return currentHP;
}

float BaseActor::GetMaxHealth() const
{
	return maxHP;
}

bool BaseActor::IsDestroyed() const
{
	return isDestroyed;
}

SDL_Color BaseActor::GetActorColor() const
{
	return actorColor;
}

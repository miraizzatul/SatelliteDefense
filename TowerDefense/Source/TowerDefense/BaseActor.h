#pragma once
#include <string>
#include <vector>
#include "EventHandler.h"

enum class Faction
{
	Neutral,
	Player,
	Enemy
};

struct LootItem
{
	std::string name;
	int quantity = 1;
	bool rareLoot = false;

	//if (lootitem a == lootitem b)
	friend bool operator==(const LootItem& a, const LootItem& b)
	{
		return a.name == b.name;//just compare names for now
	}
};

class BaseActor
{
public:
	BaseActor(int id, EventHandler<BaseActor&>* onDestroyed);
	virtual ~BaseActor() = default;

	int GetID() const;
	Faction GetFaction() const;

	void AddLoot(const LootItem& item);
	const std::vector<LootItem>& GetLoot() const;

	virtual void TakeDamage(float amount);
	float GetHealth() const;
	float GetMaxHealth() const;
	bool IsDestroyed() const;

private:
	int objectID;
	bool isDestroyed = false;
	
protected:
	Faction faction = Faction::Player; //set player by default
	bool hasBroadcasted = false;
	float currentHP = 100.f;
	float maxHP = 100.f;

	EventHandler<BaseActor&>* OnDestroyed = nullptr;
	std::vector<LootItem> loot;
};

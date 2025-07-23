#pragma once

enum class Faction
{
	Neutral,
	Player,
	Enemy
};

class BaseActor
{
public:
	BaseActor(int id) : objectID(id) {}
	virtual ~BaseActor() = default;

	int GetID() const { return objectID; }

	Faction GetFaction() const { return faction; }

private:

	int objectID;
	
protected:

	Faction faction = Faction::Player; //set player by default
};

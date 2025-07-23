#pragma once

class BaseActor
{
public:
	BaseActor(int id) : objectID(id) {}
	virtual ~BaseActor() = default;

	int GetID() const { return objectID; }

private:

	int objectID;
};

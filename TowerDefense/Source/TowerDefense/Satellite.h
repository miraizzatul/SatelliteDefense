#pragma once

#include "Tower.h"

class EventHandler;

class Satellite : public Tower
{
public:

	Satellite(float x, float y, float size, float range, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int id, EventHandler* onLoseGame = nullptr);
	void TakeDamage(float amount) override;

private:
	EventHandler* gameLose = nullptr;
	bool hasBroadcasted = false;
};

#pragma once

#include "Tower.h"
#include "BaseActor.h"

class Satellite : public Tower
{
public:
	Satellite(float x, float y, float size, float range, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int id, EventHandler<BaseActor&>* onDestroyed);
	void TakeDamage(float amount) override;

private:
	EventHandler<BaseActor&>* gameLose = nullptr;
};

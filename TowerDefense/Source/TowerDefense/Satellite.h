#pragma once

#include "Tower.h"
#include "BaseActor.h"

class Satellite : public Tower
{
public:
	Satellite(float x, float y, float size, float range, int id, SDL_Color* newColor, EventHandler<BaseActor&>* onDestroyed);
	void TakeDamage(float amount) override;

private:
	EventHandler<BaseActor&>* gameLose = nullptr;
};

#include "Satellite.h"
#include "EventHandler.h"

Satellite::Satellite(float x, float y, float size, float range, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int id, EventHandler* onLoseGame)
	:Tower(x,y,size,range, r, g, b, a, id), gameLose(onLoseGame)
{
	canDealDamage = false;
	currentHP = 200.f;
}

void Satellite::TakeDamage(float amount)
{
	Tower::TakeDamage(amount);

	SDL_Log("Current satellite HP: %f", currentHP); 
	if (IsDestroyed() && !hasBroadcasted)
	{
		hasBroadcasted = true;
		if (gameLose)
		{
			gameLose->Broadcast();
			SDL_Log("Destroyed");
		}
	}
}

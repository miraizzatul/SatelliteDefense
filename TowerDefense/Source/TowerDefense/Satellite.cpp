#include "Satellite.h"
//#include "EventHandler.h"

Satellite::Satellite(float x, float y, float size, float range, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int id, EventHandler<BaseActor&>* onDestroyed)
	:Tower(x,y,size,range, r, g, b, a, id, onDestroyed)
{
	canDealDamage = false;
	currentHP = 200.f;
}

void Satellite::TakeDamage(float amount)
{
	BaseActor::TakeDamage(amount);

	SDL_Log("Current satellite HP: %f", currentHP); 
}
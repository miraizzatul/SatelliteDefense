#include "Satellite.h"
//#include "EventHandler.h"

Satellite::Satellite(float x, float y, float size, float range, int id, SDL_Color* newColor, EventHandler<BaseActor&>* onDestroyed)
	:Tower(x,y,size,range, id, newColor, onDestroyed)
{
	canDealDamage = false;
	currentHP = 200.f;
}

void Satellite::TakeDamage(float amount)
{
	BaseActor::TakeDamage(amount);

	SDL_Log("Current satellite HP: %f", currentHP); 
}
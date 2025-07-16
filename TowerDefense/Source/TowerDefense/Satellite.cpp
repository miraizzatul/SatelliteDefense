#include "Satellite.h"

Satellite::Satellite(float x, float y, float size, float range, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	:Tower(x,y,size,range, r, g, b, a)
{
	canDealDamage = false;
	currentHP = 200.f;
}

bool Satellite::IsDestroyed() const
{
	if(Tower::IsDestroyed())
	{
		return true;
	}
	return Tower::IsDestroyed();
}

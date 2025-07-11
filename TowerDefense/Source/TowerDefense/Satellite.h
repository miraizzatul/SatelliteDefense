#pragma once

#include "Tower.h"

class Satellite : public Tower
{
public:
	Satellite(float x, float y, float size, float range, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
};

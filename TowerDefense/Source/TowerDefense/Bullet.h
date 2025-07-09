#pragma once
#include <SDL3/SDL.h>

class Bullet
{
public:
	Bullet(float x, float y, float targetX, float targetY);

	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer) const;
	bool IsOffScreen(int screenWidth, int screenHeight) const;

	SDL_FPoint GetPosition() const;

private:
	float x, y;
	float vx, vy;
	float speed = 400.f;
	float radius = 5.f;
};

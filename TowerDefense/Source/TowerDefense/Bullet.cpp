#include "Bullet.h"
#include <cmath>

Bullet::Bullet(float x, float y, float targetX, float targetY):x(x), y(y)
{
	float dx = targetX - x;
	float dy = targetY - y;
	float length = std::sqrt(dx * dx + dy * dy);

	if (length != 0.f)
	{
		vx = (dx / length) * speed;
		vy = (dy / length) * speed;
	}
	else
	{
		vx = vy = 0.f;
	}
}

void Bullet::Update(float deltaTime)
{
	x += vx * deltaTime;
	y += vy * deltaTime;
}

void Bullet::Render(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
	SDL_FRect rect = { x - radius, y - radius,radius * 2,radius * 2 };
	SDL_RenderFillRect(renderer, &rect);
}

bool Bullet::IsOffScreen(int screenWidth, int screenHeight) const
{
	return (x<0 || x>screenWidth || y<0 || y>screenHeight);
}

SDL_FPoint Bullet::GetPosition() const
{
	return { x,y };
}


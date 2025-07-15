#include "Bullet.h"
#include <cmath>
#include "Enemy.h"

Bullet::Bullet():x(0), y(0), vx(0), vy(0), isActive(false)
{
	
}

void Bullet::Update(float deltaTime, std::vector<Enemy>& enemies)
{
	if (!isActive) return;
	x += vx * deltaTime;
	y += vy * deltaTime;
	
	for (Enemy& enemy : enemies)
	{
		if (!enemy.IsAlive()) continue;

		//get center position

		float dx = (enemy.GetRect().x + enemy.GetRect().w / 2) - GetPosition().x;
		float dy = (enemy.GetRect().y + enemy.GetRect().h / 2) - GetPosition().y;

		float distSquared = dx * dx + dy * dy;
		float combinedRadius = enemy.GetRect().w + radius;

		if (distSquared <= combinedRadius * combinedRadius)
		{
			enemy.TakeDamage(damage);
			Deactivate();
			break;
		}
	}
}

void Bullet::Render(SDL_Renderer* renderer) const
{
	if (!isActive) return;
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

void Bullet::Activate(float startX, float startY, float targetX, float targetY, int ID)
{
	x = startX;
	y = startY;
	onwerID = ID;
	isActive = true;

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


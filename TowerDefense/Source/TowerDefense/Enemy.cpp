#include "Enemy.h"
#include <cmath>

Enemy::Enemy(const std::vector<SDL_FPoint>& path)
{
	rect = { path[0].x, path[0].y, 30.f, 30.f }; //start position and size
	speed = 100.f; //movement speed
	targetIndex = 1; //start by moving toward the second point
}

void Enemy::Update(float deltaTime, const std::vector<SDL_FPoint>& path)
{
	if (targetIndex >= path.size())
		return;

	SDL_FPoint target = path[targetIndex];
	float dx = target.x - rect.x;
	float dy = target.y - rect.y;
	float dist = std::sqrt(dx * dx + dy * dy); //use phytagorean theorem to get distance to next waypoint

	if (dist > 0.1f) //if enemy is already close to target, stop moving to avoid jittering
	{
		rect.x += (dx / dist) * speed * deltaTime;
		rect.y += (dy / dist) * speed * deltaTime;
	}
	if (dist < 2.f) //snap to target and go to the next point
	{
		rect.x = target.x;
		rect.y = target.y;
		targetIndex++; //move to the next waypoint
	}
}

void Enemy::Render(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);// bright red
	SDL_RenderFillRect(renderer, &rect); //draw enemy
}

bool Enemy::ReachedEnd(const std::vector<SDL_FPoint>& path)
{
	return targetIndex >= path.size();//returns true if enemy reached final waypoint
}

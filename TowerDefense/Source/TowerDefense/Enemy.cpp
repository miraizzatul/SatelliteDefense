#include "Enemy.h"
#include <cmath>
#include "Tower.h"

Enemy::Enemy(SDL_FPoint start, SDL_FPoint goal)
{
	rect = { start.x, start.y, 30.f, 30.f }; //start position and size
	this->goal = goal;
	speed = 100.f; //movement speed
}

void Enemy::Update(float deltaTime, const std::vector<Tower>& towers)
{
	const Tower* closestTower = FindClosestTower(rect, towers);
	if (closestTower)
	{
		SDL_FRect tRect = closestTower->GetRect();
		float targetX = tRect.x + tRect.w / 2;
		float targetY = tRect.y + tRect.h / 2;

		float dx = targetX - (rect.x + rect.w / 2);
		float dy = targetY - (rect.y + rect.h / 2);
		float length = std::sqrt(dx * dx + dy * dy); //distance to target
		if (length != 0)
		{
			dx /= length;
			dy /= length;
		}

		float speed = 50.f; //pixels per second
		rect.x += dx * speed * deltaTime;
		rect.y += dy * speed * deltaTime;
	}
	else
	{
		SDL_FPoint dir = { goal.x - rect.x,goal.y - rect.y };
		float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		if (length > 1e-3f)
		{
			dir.x /= length;
			dir.y /= length;
			rect.x += dir.x * speed * deltaTime;
			rect.y += dir.y * speed * deltaTime;
		}
	}
	//if (targetIndex >= path.size())
	//	return;

	//SDL_FPoint target = path[targetIndex];
	//float dx = target.x - rect.x; //get direction to x target
	//float dy = target.y - rect.y; //get direction to y target
	//float dist = std::sqrt(dx * dx + dy * dy); //use phytagorean theorem to get distance to next waypoint

	////Moves enemy toward target waypoint
	////if enemy is already close to target (dist > 0.1), stop moving to avoid jittering, 
	//// basically saying dont move unless we're clearly not there yet
	//if (dist > 0.1f) 
	//{
	//	rect.x += (dx / dist) * speed * deltaTime;
	//	rect.y += (dy / dist) * speed * deltaTime;
	//}
	//if (dist < 2.f) //snap to target when close enough and go to the next point
	//{
	//	rect.x = target.x;
	//	rect.y = target.y;
	//	targetIndex++; //move to the next waypoint
	//}
}

void Enemy::Render(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);// bright red
	SDL_RenderFillRect(renderer, &rect); //draw enemy
}

bool Enemy::ReachedEnd(const std::vector<SDL_FPoint>& path)
{
	SDL_FPoint dir = { goal.x - rect.x, goal.y - rect.y };

	return (std::sqrt(dir.x * dir.x + dir.y * dir.y) < 5.f);//returns true if enemy reached final waypoint
}

SDL_FRect Enemy::GetRect() const
{
	return rect;
}

const Tower* Enemy::FindClosestTower(const SDL_FRect& enemyRect, const std::vector<Tower>& towers)
{
	const Tower* closest = nullptr;
	float closestDist = std::numeric_limits<float>::max();

	//get location of the enemy
	float ex = enemyRect.x + enemyRect.w / 2;
	float ey = enemyRect.y + enemyRect.h / 2;

	for (const Tower& tower : towers)
	{
		//get location of the tower
		SDL_FRect tRect = tower.GetRect();
		float tx = tRect.x + tRect.w / 2;
		float ty = tRect.y + tRect.h / 2;

		float dx = tx - ex;
		float dy = ty - ey;
		float dist = std::sqrt(dx * dx + dy + dy);

		if (dist < closestDist)
		{
			closestDist = dist;
			closest = &tower;
		}
	}
	return closest;
}

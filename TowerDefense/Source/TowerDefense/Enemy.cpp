#include "Enemy.h"
#include <cmath>
#include "Tower.h"

Enemy::Enemy(SDL_FPoint start, int id, EventHandler<BaseActor&>* onDestroyed)
	:BaseActor(id, onDestroyed)
{
	currentHP = 30.f;
	maxHP = 0.f;
	faction = Faction::Enemy;
	SDL_Log("Enemy ID = %d", GetID());
	rect = { start.x, start.y, radius, radius }; //start position and size
	speed = 100.f; //movement speed
	AddLoot({ "Gold", 5 });
	AddLoot({ "Scrap Metal", 1 });
}

void Enemy::Update(float deltaTime, std::vector<std::unique_ptr<Tower>>& towers)
{
	if (ReachedEnd() || IsDestroyed()) return;

	isAttacking = false;
	attackTimer -= deltaTime;

	Tower* closestTower = FindClosestTower(rect, towers);
	if (closestTower)
	{
		SDL_FRect tRect = closestTower->GetRect();
		float targetX = tRect.x + tRect.w / 2;
		float targetY = tRect.y + tRect.h / 2;

		float dx = targetX - (rect.x + rect.w / 2); //get direction to x target
		float dy = targetY - (rect.y + rect.h / 2); //get direction to y target
		float length = std::sqrt(dx * dx + dy * dy); //distance to target
		if (!isAttacking && (length > attackRange))
		{
			dx /= length;
			dy /= length;

			//Moves enemy toward target
			rect.x += dx * speed * deltaTime;
			rect.y += dy * speed * deltaTime;
		}
		else
		{
			if (attackTimer <= 0.f)
			{
				closestTower->TakeDamage(damage);
				attackTimer = attackCooldown;
			}
		}
	}
	/*else
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
	}*/
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

bool Enemy::ReachedEnd() const
{
	SDL_FPoint dir = { goal.x - rect.x, goal.y - rect.y };

	return (std::sqrt(dir.x * dir.x + dir.y * dir.y) < 5.f);//returns true if enemy reached final waypoint
}

SDL_FRect Enemy::GetRect() const
{
	return rect;
}

Tower* Enemy::FindClosestTower(const SDL_FRect& enemyRect, std::vector<std::unique_ptr<Tower>>& towers)
{
	Tower* closest = nullptr;
	float closestDist = std::numeric_limits<float>::max();

	//get location of the enemy
	float ex = enemyRect.x + enemyRect.w / 2;
	float ey = enemyRect.y + enemyRect.h / 2;

	for (const auto& tower : towers)
	{
		if(tower->IsDestroyed() || tower->GetFaction() == this->faction) continue;

		//get location of the tower
		SDL_FRect tRect = tower->GetRect();
		float tx = tRect.x + tRect.w / 2;
		float ty = tRect.y + tRect.h / 2;

		float dx = tx - ex;
		float dy = ty - ey;
		float dist = std::sqrt(dx * dx + dy + dy);

		if (dist < closestDist)
		{
			closestDist = dist;
			closest = tower.get(); //store raw pointer
		}
	}
	return closest;
}

bool Enemy::ResolveStructureCollision(const SDL_FRect& thisRect, float radius, std::vector<std::unique_ptr<Tower>>& towers)
{
	SDL_FPoint center = { thisRect.x + thisRect.w / 2.f,thisRect.y + thisRect.h / 2.f };

	for (const auto& tower : towers)
	{
		float closestX = SDL_clamp(center.x, tower->GetRect().x, tower->GetRect().x + tower->GetRect().w);
		float closestY = SDL_clamp(center.y, tower->GetRect().y, tower->GetRect().y + tower->GetRect().h);

		float dx = center.x - closestX;
		float dy = center.y - closestY;

		if ((dx * dx + dy * dy) < (radius * radius))
		{
			return true;
		}
	}
	return false;
}

void Enemy::ResolveEnemyCollision(std::vector<Enemy>& allEnemies, std::vector<std::unique_ptr<Tower>>& allTowers)
{
	for (Enemy& other : allEnemies)
	{
		if(&other == this || other.IsDestroyed()) continue;

		//Calculates center-to-center distance between the two enemies.
		float dx = (rect.x + rect.w / 2) - (other.rect.x + other.rect.w / 2);
		float dy = (rect.y + rect.h / 2) - (other.rect.y + other.rect.h / 2);
		float distSquared = dx * dx + dy * dy;

		float minDist = radius + other.radius;//how far apart they need to be avoid overlapping

		//if overlapping
		if (distSquared < minDist * minDist && distSquared>0.0001f) // too close
		{
			float dist = std::sqrt(distSquared);
			float overlap = 0.5f * (minDist - dist);// how much two enemies are intersecting

			//(dx, dy) normalize direction vector from other enemy to this enemy
			dx /= dist;
			dy /= dist;

			// Compute potential push amounts with damping clamp to avoid extreme jitter
			float pushX = SDL_clamp(dx * overlap, -damping, damping);
			float pushY = SDL_clamp(dy * overlap, -damping, damping);


			//push both enemies away from each other and creates sliding effect: if blocked in one direction, 
			// enemy can still move in other direction

			//current enemy

			SDL_FPoint originalPos = { rect.x, rect.y };

			rect.x += pushX;//moves enemy along x axis, then checks if its collides with tower
			if (ResolveStructureCollision(rect, radius, allTowers))
			{
				rect.x = originalPos.x;//undo x if blocked
			}

			rect.y += pushY;//moves enemy along y axis, then checks if its collides with tower
			if (ResolveStructureCollision(rect, radius, allTowers))
			{
				rect.y = originalPos.y;//undo y if blocked
			}

			//other enemy

			SDL_FPoint otherOriginalPos = { other.rect.x, other.rect.y };

			other.rect.x -= pushX;//moves other enemy along -x axis, then checks if its collides with tower
			if (ResolveStructureCollision(other.rect, other.radius, allTowers))
			{
				other.rect.x = otherOriginalPos.x;//undo -x if blocked
			}

			other.rect.y -= pushY;//moves other enemy along -y axis, then checks if its collides with tower
			if (ResolveStructureCollision(other.rect, other.radius, allTowers))
			{
				other.rect.y = otherOriginalPos.y;//undo -y if blocked
			}
		}
	}
}

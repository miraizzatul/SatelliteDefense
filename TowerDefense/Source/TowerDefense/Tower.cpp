#include "Tower.h"
#include <cmath>
#include "Bullet.h"
#include "Enemy.h"

Tower::Tower(float x, float y, float size, float range, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int id)
	:BaseActor(id), range(range), attackCooldown(1.f), timeSinceLastShot(0.f), target(nullptr)
{
	rect = { x, y, size, size };
	red = r, green = g, blue = b, alpha = a;
	SDL_Log("Tower ID = %d", GetID());
}

void Tower::Update(float deltaTime, const std::vector<Enemy>& enemies, std::vector<Bullet>& bullets)
{
	if (!canDealDamage) return;

	timeSinceLastShot += deltaTime;

	//Find closest enemy in range
	float closestDist = range;
	target = nullptr;

	for (const Enemy& enemy : enemies)
	{
		if (enemy.GetFaction() == this->faction) continue;//skip allies

		SDL_FRect enemyRect = enemy.GetRect();

		//get distance between current location to target enemy location
		float dx = (enemyRect.x + enemyRect.w / 2) - (rect.x + rect.w / 2);
		float dy = (enemyRect.y + enemyRect.h / 2) - (rect.y + rect.h / 2);
		float dist = std::sqrt(dx * dx + dy * dy);

		if (dist < closestDist)
		{
			closestDist = dist;
			target = &enemy;
		}
	}
	if (target && timeSinceLastShot >= attackCooldown)
	{
		//calculate bullet spawn point(center of tower)
		float startX = rect.x + rect.w / 2;
		float startY = rect.y + rect.h / 2;

		//calculate bullet target point(center of enemy)
		SDL_FRect enemyRect = target->GetRect();
		float targetX = enemyRect.x + enemyRect.w / 2;
		float targetY= enemyRect.y + enemyRect.h / 2;

		for (Bullet& bullet : bullets)
		{
			if (!bullet.IsActive())
			{
				bullet.Activate(startX, startY, targetX, targetY, GetID());
				timeSinceLastShot = 0.f;
				break;
			}
		}
	}
}

void Tower::Render(SDL_Renderer* renderer, bool showRange) const
{
	//tower square
	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
	SDL_RenderFillRect(renderer, &rect);

	if (showRange)
	{
		//draw circle range (debug)
		int centerX = static_cast<int>(rect.x + rect.w / 2);
		int centerY = static_cast<int>(rect.y + rect.h / 2);
		int r = static_cast<int>(range);

		SDL_SetRenderDrawColor(renderer, 0, 200, 0, 64);
		for (int w = 0; w < r * 2; w++)
		{
			for (int h = 0; h < r * 2; h++)
			{
				int dx = r - w;
				int dy = r - h;
				if ((dx * dx + dy * dy) <= (r * r))
				{
					SDL_RenderPoint(renderer, float(centerX + dx), float(centerY + dy));
				}
			}
		}
	}
}

void Tower::Move(float dx, float dy)
{
	//move the tower
	rect.x += dx;
	rect.y += dy;
}

void Tower::ClampPosition(int screenWidth, int screenHeight)
{
	if (rect.x < 0) rect.x = 0;//left edge
	if (rect.y < 0) rect.y = 0;//top edge
	if (rect.x + rect.w > screenWidth)rect.x = screenWidth - rect.w;//(Tower.x+Tower.w) right edge of rectangle
	if (rect.y + rect.w > screenHeight)rect.y = screenHeight- rect.h;//(Tower.y+Tower.h) bottom edge of rectangle
}

void Tower::StartRepairTower(float xLocation, float yLocation)
{
	if (xLocation >= rect.x && xLocation <= rect.x + rect.w && xLocation >= rect.y && yLocation <= rect.y + rect.h)
	{
		Repair(25.f);
		SDL_Log("Repaired tower to %.1f / %.1f", currentHP, maxHP);
	}
}

void Tower::TakeDamage(float amount)
{
	currentHP -= amount;
	if (currentHP <= 0.f)
	{
		isDestroyed = true;
	}
}

void Tower::Repair(float amount)
{
	currentHP += amount;
	if (currentHP > maxHP)
	{
		currentHP = maxHP;
	}
}

SDL_FRect Tower::GetRect() const
{
	return rect;
}

bool Tower::IsDestroyed() const
{
	return isDestroyed;
}

float Tower::GetHealth() const
{
	return currentHP;
}

float Tower::GetMaxHealth() const
{
	return maxHP;
}

bool Tower::IsRepairable() const
{
	return isRepairable;
}

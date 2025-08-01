#include "Tower.h"
#include <cmath>
#include "Bullet.h"
#include "Enemy.h"

Tower::Tower(float x, float y, float size, float range, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int id, EventHandler<BaseActor&>* onDestroyed)
	:BaseActor(id, onDestroyed), range(range), attackCooldown(1.f), timeSinceLastShot(0.f), target(nullptr)
{
	rect = { x, y, size, size };
	red = r, green = g, blue = b, alpha = a;
	faction = Faction::Player;
	currentHP = 100.f;
	maxHP = 100.f;
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

		//get distance between current location to target enemy location
		float dx = enemy.GetPosition().x - GetPosition().x;
		float dy = enemy.GetPosition().y - GetPosition().y;
		float dist = std::sqrt(dx * dx + dy * dy);

		if (dist < closestDist)
		{
			closestDist = dist;
			target = &enemy;
		}
	}
	if (target && timeSinceLastShot >= attackCooldown)
	{
		//calculate current bullet spawn point(center of tower)
		float startX = GetPosition().x;
		float startY = GetPosition().y;

		//calculate bullet target point(center of enemy)
		float targetX = target->GetPosition().x;
		float targetY= target->GetPosition().y;

		for (Bullet& bullet : bullets)
		{
			if (!bullet.IsActive())
			{
				bullet.Activate(startX, startY, targetX, targetY, this);
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
		int centerX = static_cast<int>(GetPosition().x);
		int centerY = static_cast<int>(GetPosition().y);
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

SDL_FPoint Tower::GetPosition() const
{
	return SDL_FPoint{ rect.x + rect.w / 2,rect.y + rect.h / 2 };
}

bool Tower::IsRepairable() const
{
	return isRepairable;
}

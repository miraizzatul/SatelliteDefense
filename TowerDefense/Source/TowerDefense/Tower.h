#pragma once
#include <SDL3/SDL.h>
#include "BaseActor.h"
#include <vector>

class Enemy;
class Bullet;

class Tower : public BaseActor
{
public:
	Tower(float x, float y, float size, float range, int id, SDL_Color* newColor, EventHandler<BaseActor&>* onDestroyed);
	virtual ~Tower() = default;

	void FindTarget(const std::vector<Enemy>& enemies);
	void Update(float deltaTime, const std::vector<Enemy>& enemies, std::vector<Bullet>& bullets);
	void Render(SDL_Renderer* renderer, bool showRange = false) const;
	void Move(float dx, float dy);
	void ClampPosition(int screenWidth, int screenHeight);
	void StartRepairTower(float xLocation, float yLocation);

	virtual void Repair(float amount);

	SDL_FRect GetRect() const;
	SDL_FPoint GetPosition() const;
	float GetHealth() const;
	float GetMaxHealth() const;
	bool IsRepairable() const;
	float DistanceToTarget(const SDL_FPoint& a, const SDL_FPoint& b);

private:
	SDL_FRect rect;
	float range;
	float attackCooldown;
	float timeSinceLastShot;

protected: 
	bool canDealDamage = true;
	bool isRepairable = true;

	const Enemy* target; //pointer to target enemy
};

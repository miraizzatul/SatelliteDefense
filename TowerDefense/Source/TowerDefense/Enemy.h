#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>

class Tower;

class Enemy
{
public:

	//takes a path and init the enemy's position at first waypoint
	Enemy(SDL_FPoint start);

	// moves enemy along the path
	void Update(float deltaTime, std::vector<std::unique_ptr<Tower>>& towers);

	void TakeDamage(float amount);

	//draws enemy as red rectangle on screen
	void Render(SDL_Renderer* renderer) const;

	bool ReachedEnd() const;

	bool IsAlive() const;

	SDL_FRect GetRect() const; //encapsulated enemy access

	void EnemyOverlapBlock(std::vector<Enemy>& allEnemies);

private:

	//position & size of enemy (x,y,w,h)
	SDL_FRect rect;

	float currentHP = 30.f;
	float maxHP = 0.f;

	//movement speed (pixels per second)
	float speed;

	//main goal
	SDL_FPoint goal;
	
	//index of current waypoint the enemy is moving toward
	int targetIndex;

	//Find the closest towers to enemy
	Tower* FindClosestTower(const SDL_FRect& enemyRect, std::vector<std::unique_ptr<Tower>>& towers);

	float attackCooldown = 1.f;
	float attackTimer = 0.f;
	float attackRange = 30.f;
	float damage = 10.f;
	float radius = 15.f;

	bool isAlive = true;
};
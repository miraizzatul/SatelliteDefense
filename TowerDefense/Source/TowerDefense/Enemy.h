#pragma once
#include <SDL3/SDL.h>
#include "BaseActor.h"
#include <vector>
#include <memory>
#include "EventHandler.h"

class Tower;

class Enemy : public BaseActor
{
public:

	//takes a path and init the enemy's position at first waypoint
	Enemy(SDL_FPoint start,int id, EventHandler<BaseActor&>* onDestroyed);

	// moves enemy along the path
	void Update(float deltaTime, std::vector<std::unique_ptr<Tower>>& towers);

	//draws enemy as red rectangle on screen
	void Render(SDL_Renderer* renderer) const;

	bool ReachedEnd() const;
	SDL_FRect GetRect() const; //encapsulated enemy access
	void ResolveEnemyCollision(std::vector<Enemy>& allEnemies, std::vector<std::unique_ptr<Tower>>& allTowers);

private:
	//position & size of enemy (x,y,w,h)
	SDL_FRect rect;

	//movement speed (pixels per second)
	float speed;

	//main goal
	SDL_FPoint goal;
	
	//index of current waypoint the enemy is moving toward
	int targetIndex;

	//Find the closest towers to enemy
	Tower* FindClosestTower(const SDL_FRect& enemyRect, std::vector<std::unique_ptr<Tower>>& towers);

	bool ResolveStructureCollision(const SDL_FRect& thisRect, float radius, std::vector<std::unique_ptr<Tower>>& towers);

	bool isAttacking = false;
	float attackCooldown = 1.f;
	float attackTimer = 0.f;
	float attackRange = 30.f; 
	float damage = 10.f;
	float radius = 15.f;
	float damping = 0.8f; // reduce push force to smooth out movement
};
#pragma once
#include <SDL3/SDL.h>
#include <vector>

class Tower;

class Enemy
{
public:

	//takes a path and init the enemy's position at first waypoint
	Enemy(SDL_FPoint start, SDL_FPoint goal);

	// moves enemy along the path
	void Update(float deltaTime, const std::vector<Tower>& towers);

	//draws enemy as red rectangle on screen
	void Render(SDL_Renderer* renderer) const;

	bool ReachedEnd(const std::vector<SDL_FPoint>& path);

	SDL_FRect GetRect() const; //encapsulated enemy access

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
	const Tower* FindClosestTower(const SDL_FRect& enemyRect, const std::vector<Tower>& towers);
};
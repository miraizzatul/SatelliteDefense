#pragma once
#include <SDL3/SDL.h>
#include <vector>

class Enemy
{
public:

	//takes a path and init the enemy's position at first waypoint
	Enemy(const std::vector<SDL_FPoint>& path);

	// moves enemy along the path
	void Update(float deltaTime, const std::vector<SDL_FPoint>& path);

	//draws enemy as red rectangle on screen
	void Render(SDL_Renderer* renderer) const;

	bool ReachedEnd(const std::vector<SDL_FPoint>& path);

	SDL_FRect GetRect() const; //encapsulated enemy access

private:

	//position & size of enemy (x,y,w,h)
	SDL_FRect rect;

	//movement speed (pixels per second)
	float speed;
	
	//index of current waypoint the enemy is moving toward
	int targetIndex;
};
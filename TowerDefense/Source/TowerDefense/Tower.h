#pragma once
#include <SDL3/SDL.h>
#include <vector>

class Enemy;
class Bullet;

class Tower
{
public:
	Tower(float x, float y, float size, float range);

	void Update(float deltaTime, const std::vector<Enemy>& enemies, std::vector<Bullet>& bullets);
	void Render(SDL_Renderer* renderer, bool showRange = false) const;
	void Move(float dx, float dy);
	void ClampPosition(int screenWidth, int screenHeight);

	SDL_FRect GetRect() const;

private:
	SDL_FRect rect;
	float range;
	float attackCooldown;
	float timeSinceLastShot;

	const Enemy* target; //pointer to target enemy
};

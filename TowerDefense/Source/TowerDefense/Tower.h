#pragma once
#include <SDL3/SDL.h>
#include <vector>

class Enemy;
class Bullet;

class Tower
{
public:
	Tower(float x, float y, float size, float range, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	void Update(float deltaTime, const std::vector<Enemy>& enemies, std::vector<Bullet>& bullets);
	void Render(SDL_Renderer* renderer, bool showRange = false) const;
	void Move(float dx, float dy);
	void ClampPosition(int screenWidth, int screenHeight);
	void TakeDamage(float amount);

	SDL_FRect GetRect() const;
	bool IsDestroyed() const;

	//float maxHP = 0.f;

private:
	SDL_FRect rect;
	float range;
	float attackCooldown;
	float timeSinceLastShot;
	bool isDestroyed = false;

protected: 
	//SDL_FPoint size = 
	bool canDealDamage = true;
	float currentHP = 100.f;

	//temp: tower colour
	Uint8 red = 0, green = 0, blue = 0, alpha= 0;

	const Enemy* target; //pointer to target enemy
};

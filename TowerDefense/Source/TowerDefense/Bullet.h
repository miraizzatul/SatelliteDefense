#pragma once
#include <SDL3/SDL.h>
#include <vector>

class Enemy;

class Bullet
{
public:
	Bullet();

	void Update(float deltaTime, std::vector<Enemy>& enemies);
	void Render(SDL_Renderer* renderer) const;
	bool IsOffScreen(int screenWidth, int screenHeight) const;

	SDL_FPoint GetPosition() const;

	void Activate(float startX, float startY, float targetX, float targetY, int ID);
	void Deactivate() { isActive = false; }
	bool IsActive() const { return isActive; }
	int GetOwnerID() const { return onwerID; }

private:
	float x = 0, y = 0;
	float vx = 0, vy = 0;
	float speed = 400.f;
	float radius = 5.f;
	bool isActive = false;
	int onwerID = -1;
	float damage = 30.f;
};

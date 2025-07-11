#pragma once
#include <SDL3/SDL.h>

class Bullet
{
public:
	Bullet(float x, float y, float targetX, float targetY);

	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer) const;
	bool IsOffScreen(int screenWidth, int screenHeight) const;

	SDL_FPoint GetPosition() const;

	void Activate(float startX, float startY, float targetX, float targetY);
	void Deactivate() { isActive = false; }
	bool IsActive() const { return isActive; }

private:
	float x, y;
	float vx, vy;
	float speed = 400.f;
	float radius = 5.f;
	bool isActive = false;
	int onwerID = -1;
};

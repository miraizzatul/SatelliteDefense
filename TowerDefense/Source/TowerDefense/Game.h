#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "Enemy.h"
#include "Tower.h"

class Game
{
public:
	Game();
	~Game();

	bool Init();
	void Run();
	void Shutdown();

private:
	void ProcessInput();
	void Update(float deltaTime);
	void Render();
	void SpawnEnemy(float deltaTime);
	void ClampTowerMovement(float deltaTime);

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool running = false;
	bool keys[SDL_SCANCODE_COUNT] = { false };// track held keys

	Uint64 now = 0;// get timestamp
	Uint64 last = 0;
	float deltaTime = 0.f;

	float speed = 200.f; // pixels per second

	float enemySpawnTimer = 0.f;
	const float spawnInterval = 2.f;

	std::vector<Enemy> enemies;
	std::vector<Tower> towers;
	std::vector<SDL_FPoint> path;

	const int windowWidth = 800;
	const int windowHeight = 600;

};

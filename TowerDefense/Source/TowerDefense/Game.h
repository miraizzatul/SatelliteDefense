#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include<memory>

class Enemy;
class Tower;
class Bullet;

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
	void PlaceTowerOnGrid(float mouseX, float mouseY);

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
	std::vector<std::unique_ptr<Tower>> towers; //allows storing both tower and satellite objects and auto cleanup memory
	std::vector<Bullet> bullets;

	const int windowWidth = 800;
	const int windowHeight = 600;

	Tower* goal = nullptr;

	//Define grid settings
	const int tileCols = 10;
	const int tileRows = 8;
	const float tileWidth = 800.f / tileCols;
	const float tileHeight = 600.f / tileRows;

};

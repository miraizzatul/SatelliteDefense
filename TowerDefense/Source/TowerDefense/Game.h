#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "EventHandler.h"

class BaseActor;
class Enemy;
class Tower;
class Bullet;

enum class GameState
{
	MainMenu,
	Playing,
	GameOver,
	Quit
};

struct Button
{
	SDL_FRect rect;
	SDL_Color color;
	std::string label;
};

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
	void HandleGameplayInput(SDL_Event& event); //gameplay specific logic input
	void InitMainMenu();
	void InitGameOverMenu();
	void StartMainMenu();
	void StartGame();
	void RestartGame();
	
	void SpawnEnemy(float deltaTime);
	void ClampTowerMovement(float deltaTime);
	void PlaceTowerOnGrid(float mouseX, float mouseY);

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool running = false;
	bool keys[SDL_SCANCODE_COUNT] = { false };// track held keys

	void Render();
	void RenderMainMenu();
	void RenderGameOver();
	void RenderButtonLabel(const Button& btn, TTF_Font* font);

	Uint64 now = 0;// get timestamp
	Uint64 last = 0;
	float deltaTime = 0.f;

	float speed = 200.f; // pixels per second

	float enemySpawnTimer = 0.f;
	const float spawnInterval = 2.f;

	//global map of objectID
	std::unordered_map<int, BaseActor*> idToObject;
	int nextID = 1; // global next ID

	std::vector<Enemy> enemies;
	std::vector<std::unique_ptr<Tower>> towers; //allows storing both tower and satellite objects and auto cleanup memory

	const int windowWidth = 800;
	const int windowHeight = 600;

	EventHandler gameEventHandler;

	GameState currentState = GameState::MainMenu;
	GameState previousState;

	bool loseGame = false;

	//Define grid settings
	const int tileCols = 10;
	const int tileRows = 8;
	const float tileWidth = 800.f / tileCols;
	const float tileHeight = 600.f / tileRows;

	//define buttons
	std::vector<Button> menuButtons;
	std::vector<Button> gameOverButtons;
	TTF_Font* font = nullptr;
};

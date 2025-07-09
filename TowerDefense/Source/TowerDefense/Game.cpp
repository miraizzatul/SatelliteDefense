#include "Game.h"
#include <SDL3/SDL.h> //include the sdl header

Game::Game()
{
 }

Game::~Game()
{
}

bool Game::Init()
{
	//initialize sdl3
	SDL_Init(SDL_INIT_VIDEO);

	//Create sdl3 window (80*600)
	window = SDL_CreateWindow("Tower Defense", windowWidth, windowHeight,SDL_WINDOW_RESIZABLE|SDL_EVENT_WINDOW_SHOWN);
	if (!window)
	{
		SDL_Log("CreateWindow Error: %s", SDL_GetError());
		return false;
	}

	//set vsync before creating renderer
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	renderer = SDL_CreateRenderer(window, nullptr);

	//check if renderer is invalid
	if (!renderer)
	{
		SDL_Log("Renderer failed: %s", SDL_GetError());
		return false;
	}

	//set white bg colour (R,G,B,A(0-255)
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	//clear the renderer (fills with draw colour
	SDL_RenderClear(renderer);

	//present the renderer to the window
	SDL_RenderPresent(renderer);

	// Set up enemy path
	path = {{50.f, 50.f}, {300.f, 50.f}, {300.f, 300.f}, {600.f, 300.f}, {600.f, 500.f}};

	// add one enemy
	enemies.emplace_back(path);

	// Add one tower
	towers.emplace_back(400.f, 200.f, 40.f, 150.f);

	running = true;// keeps running until the window is closed
	now = SDL_GetPerformanceCounter();// get timestamp
	return true;
}

void Game::Run()
{
	SDL_Log("Game::Run() starting");

	while (running)
	{
		//prevent the loop from hogging the cpu since sometimes cpu might run too fast and render frames too quicky
		SDL_Delay(1);

		last = now;
		now = SDL_GetPerformanceCounter();//current timestamp

		//SDL_GetPerformanceFrequency gives us number of ticks per second
		deltaTime = (now - last) / (float)SDL_GetPerformanceFrequency();

		ProcessInput();
		Update(deltaTime);
		Render();
	}
}

void Game::Shutdown()
{
	//cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)//checks for quit events (like clicking the X button)
		{
			running = false;
		}
		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			keys[event.key.scancode] = true;
			if (event.key.scancode == SDL_SCANCODE_ESCAPE) //check for keyboard input if escape key is pressed
			{
				SDL_Log("Escape key pressed — exiting...");
				running = false;
			}
		}
		else if (event.type == SDL_EVENT_KEY_UP)
		{
			keys[event.key.scancode] = false;
		}
		else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				float mouseX = static_cast<float>(event.button.x);
				float mouseY = static_cast<float>(event.button.y);
				PlaceTowerOnGrid(mouseX, mouseY);
			}
		}
	}
}

void Game::Update(float deltaTime)
{
	//ClampTowerMovement(deltaTime);
	SpawnEnemy(deltaTime);

	//Update all towers to check for enemies and shoot
	for (auto& tower : towers)
		tower.Update(deltaTime, enemies, bullets);

	for (auto& e : enemies)
		e.Update(deltaTime, path);

	//update bullets
	for (auto& bullet : bullets)
		bullet.Update(deltaTime);

	//remove bullets that are offscreen
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [this](const Bullet& b) {
		return b.IsOffScreen(windowWidth, windowHeight);
	}), bullets.end());
}

void Game::Render()
{
	//draw scene
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);//set draw colour dark grey

	//clear the renderer (fills with draw colour
	SDL_RenderClear(renderer);

	//draw simple grid tiles
	for (int row = 0; row < tileRows; ++row)
	{
		for (int col = 0; col < tileCols; ++col)
		{
			SDL_FRect tileRect = {
				col * tileWidth,
				row * tileHeight,
				tileWidth,
				tileHeight
			};

			//Alternate light and dark tiles
			if ((row + col) % 2 == 0)
				SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255); // light gray
			else
				SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255); // dark gray

			SDL_RenderFillRect(renderer, &tileRect);
		}
	}
	//create green tower colour
	SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

	for (auto& tower : towers)
		tower.Render(renderer);

	//render all enemies
	for (auto& enemy : enemies)
		enemy.Render(renderer);

	//bullets
	for (auto& bullet : bullets)
		bullet.Render(renderer);

	SDL_RenderPresent(renderer);//present the result to the window
}

void Game::SpawnEnemy(float deltaTime)
{
	enemySpawnTimer += deltaTime;
	if (enemySpawnTimer >= spawnInterval)
	{
		enemies.emplace_back(path);
		enemySpawnTimer = 0.f;
	}
}

void Game::ClampTowerMovement(float deltaTime)
{
	if (towers.empty()) return;

	Tower& playerTower = towers[0]; // Control the first tower
	
	//movement logic
	if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP]) playerTower.Move(0, -speed * deltaTime);
	if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]) playerTower.Move(0, speed * deltaTime);
	if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) playerTower.Move(-speed * deltaTime, 0);
	if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) playerTower.Move(speed * deltaTime, 0);

	//Clamp the tower from going offscreen
	playerTower.ClampPosition(windowWidth, windowHeight);
}

void Game::PlaceTowerOnGrid(float mouseX, float mouseY)
{
	//snap mouse click to the top left of the grid cell
	int col = static_cast<int>(mouseX / tileWidth);
	int row = static_cast<int>(mouseY / tileHeight);

	float snappedX = col * tileWidth;
	float snappedY = row * tileHeight;
	
	//define tower size and range
	float towerSize = 40.f;
	float towerRange = 150.f;

	for (const auto& tower : towers)
	{
		SDL_FRect rect = tower.GetRect();
		if (snappedX == rect.x && snappedY == rect.y) 
		{
			return; //tower already placed here
		}
	}
	towers.emplace_back(snappedX, snappedY, towerSize, towerRange);

}

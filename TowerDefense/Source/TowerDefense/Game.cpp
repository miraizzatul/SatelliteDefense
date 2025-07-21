#include "Game.h"
#include <SDL3/SDL.h> //include the sdl header
#include <SDL3_ttf/SDL_ttf.h>
#include "Enemy.h"
#include "Tower.h"
#include "Satellite.h"
#include "Bullet.h"

std::vector<Bullet> bullets(100);

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

	TTF_Init();

	font = TTF_OpenFont("STENCIL.ttf", 24);
	if (!font) { SDL_Log("Font error: %s", SDL_GetError()); return false; }

	//Create sdl3 window (80*600)
	window = SDL_CreateWindow("Satellite Defense", windowWidth, windowHeight,SDL_WINDOW_RESIZABLE|SDL_EVENT_WINDOW_SHOWN);
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

	running = true;// keeps running until the window is closed
	now = SDL_GetPerformanceCounter();// get timestamp
	return true;
}

void Game::Run()
{
	SDL_Log("Game::Run() starting");

	StartMainMenu();

	gameEventHandler.Bind([&]() {
		SDL_Log("Game Over!!");
		loseGame = true;
		currentState = GameState::GameOver;
	});

	const Uint64 targetFrameDuration = SDL_GetPerformanceFrequency() / 60;
	Uint64 frameStart, frameEnd, elapsed, delayTicks;
	Uint32 delayMs;

	while (running)
	{
		if (currentState == GameState::Quit)
		{
			SDL_Log("Game::Run() closing");
			running = false;
			continue;
		}

		frameStart = SDL_GetPerformanceCounter();

		//Timing for deltaTime
		last = now;
		now = frameStart;

		//SDL_GetPerformanceFrequency gives us number of ticks per second
		deltaTime = (now - last) / (float)SDL_GetPerformanceFrequency();

		ProcessInput();

		switch (currentState)
		{
		case GameState::MainMenu:
			RenderMainMenu();//just draw the button, no need to init main menu and recreate the buttons again
			break;
		case GameState::Playing:
			Update(deltaTime);
			Render();
			break;
		case GameState::GameOver:
			RenderGameOver();
			break;
		}

		frameEnd = SDL_GetPerformanceCounter();
		elapsed = frameEnd - frameStart;

		if (elapsed < targetFrameDuration)
		{
			delayTicks = targetFrameDuration - elapsed;

			//convert to Uint32
			delayMs = static_cast<Uint32>((delayTicks * 1000 / SDL_GetPerformanceFrequency()));

			//clamp the value just in case
			if (delayMs > 1000) delayMs = 1000;
				SDL_Delay(delayMs);
		}
	}
}

void Game::Shutdown()
{
	//cleanup
	TTF_CloseFont(font);
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
			currentState = GameState::Quit;
		}
		switch (currentState)
		{
		case GameState::MainMenu:
			if (event.type == SDL_EVENT_KEY_DOWN)
			{
				if (event.key.scancode == SDL_SCANCODE_RETURN)
				{
					StartGame(); //new function to begin gameplay
				}
				else if (event.key.scancode == SDL_SCANCODE_Q || event.key.scancode == SDL_SCANCODE_ESCAPE)
				{
					currentState = GameState::Quit;
				}
			}
			break;

		case GameState::Playing:
			HandleGameplayInput(event);// Original input logic
			break;

		case GameState::GameOver:
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_R)
			{
				RestartGame();
			}
			else if (event.key.scancode == SDL_SCANCODE_Q || event.key.scancode == SDL_SCANCODE_ESCAPE)
			{
				currentState = GameState::Quit;
			}
			break;
		}
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			float mx = static_cast<float>(event.button.x);
			float my = static_cast<float>(event.button.y);

			for (const Button& btn : menuButtons)
			{
				if (mx >= btn.rect.x && mx <= btn.rect.x + btn.rect.w && my >= btn.rect.y && my <= btn.rect.y + btn.rect.h)
				{
					if (btn.label == "start")
					{
						StartGame(); //new function to begin gameplay
					}
					else if (btn.label == "quit")
					{
						currentState = GameState::Quit;
					}
				}
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
		tower->Update(deltaTime, enemies, bullets);

	for (auto& e : enemies)
		e.Update(deltaTime, towers);
	
	//resolve overlaps after all enemies have moved
	for (auto& e : enemies)
		e.ResolveEnemyCollision(enemies, towers);

	//update bullets
	for (auto& bullet : bullets)
	{
		if (!bullet.IsActive()) continue;

		bullet.Update(deltaTime, enemies);

		if (bullet.IsOffScreen(windowWidth, windowHeight))
		{
			bullet.Deactivate(); //reuse bullet instead
		}
	}

	//remove towers that are destroyed
	towers.erase(std::remove_if(towers.begin(), towers.end(),
		[](std::unique_ptr<Tower>& t) { return t->IsDestroyed(); }), towers.end());

	//remove enemies that are destroyed
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [this](const Enemy& e) {
		return !e.IsAlive();
	}), enemies.end());
}

void Game::HandleGameplayInput(SDL_Event& event)
{
	if (event.type == SDL_EVENT_KEY_DOWN)
	{
		keys[event.key.scancode] = true;
		if (event.key.scancode == SDL_SCANCODE_ESCAPE) //check for keyboard input if escape key is pressed
		{
			SDL_Log("Escape key pressed — exiting...");
			currentState = GameState::MainMenu;
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

void Game::InitMainMenu()
{
	menuButtons.clear();

	Button startButton;
	startButton.rect = { 300, 200, 200, 60 };
	startButton.color = { 0, 200, 0, 255 };
	startButton.label = "start";

	Button quitButton;
	quitButton.rect = { 300, 300, 200, 60 };
	quitButton.color = { 200, 0, 0, 255 };
	quitButton.label = "quit";

	menuButtons.push_back(startButton);
	menuButtons.push_back(quitButton);
}

void Game::StartMainMenu()
{
	InitMainMenu();//called only once
	currentState = GameState::MainMenu;
}

void Game::StartGame()
{
	SDL_Log("Starting Game...");
	currentState = GameState::Playing;
	loseGame = false;
	towers.clear();
	enemies.clear();

	//set satellite at the center
	towers.emplace_back(std::make_unique<Satellite>(windowWidth / 2.f, windowHeight / 2.f, 30.f, 150.f,
		static_cast<Uint8>(121), static_cast<Uint8>(209), static_cast<Uint8>(145), static_cast<Uint8>(255), &gameEventHandler));
}

void Game::RestartGame()
{
	SDL_Log("Restarting Game...");
	StartGame();
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

	for (auto& tower : towers)
		tower->Render(renderer);

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
		SDL_FPoint spawnPos;
		int side = rand() % 4; // 0 = top, 1 = bottom, 2 = left, 3 = right
		switch (side)
		{
		case 0: //Top
			spawnPos = { static_cast<float>(rand() % windowWidth), 0.f };
			break;
		case 1: //Bottom
			spawnPos = { static_cast<float>(rand() % windowWidth), static_cast<float>(rand() % windowHeight) };
			break;
		case 2: //Left
			spawnPos = { 0.f, static_cast<float>(rand() % windowHeight)};
			break;
		case 3: //Right
			spawnPos = { static_cast<float>(rand() % windowWidth), static_cast<float>(rand() % windowHeight)};
			break;
		}
		enemies.emplace_back(spawnPos);
		enemySpawnTimer = 0.f;
	}
}

void Game::ClampTowerMovement(float deltaTime)
{
	if (towers.empty()) return;

	Tower& playerTower = *towers[0]; // Control the first tower
	
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
		SDL_FRect rect = tower->GetRect();
		if (snappedX == rect.x && snappedY == rect.y) 
		{
			return; //tower already placed here
		}
	}
	towers.emplace_back(std::make_unique<Tower>
		(snappedX, snappedY, towerSize, towerRange,
		static_cast<Uint8>(0), static_cast<Uint8>(200), 
			static_cast<Uint8>(0), static_cast<Uint8>(255)));
}

void Game::RenderMainMenu()
{
	//main menu bg
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//buttons
	for (const auto& btn : menuButtons)
	{
		SDL_SetRenderDrawColor(renderer, btn.color.r, btn.color.g, btn.color.b, btn.color.a);
		SDL_RenderFillRect(renderer, &btn.rect);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//draw button borders
		SDL_RenderRect(renderer, &btn.rect);
		RenderButtonLabel(btn, font);
	}
	
	// Draw title and instructions (in real game, use SDL_ttf for text)
	SDL_Log("Main Menu - Press Enter to Start");
	SDL_RenderPresent(renderer);
}

void Game::RenderGameOver()
{
	SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_Log("Game Over - Press R TO Restart");

	SDL_RenderPresent(renderer);
}

void Game::RenderButtonLabel(const Button& btn, TTF_Font* font)
{
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface* surface = TTF_RenderText_Solid(font, btn.label.c_str(),0, white);

	if (!surface) return;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) return;

	SDL_FRect textRect;
	textRect.w = static_cast<float>(surface->w);
	textRect.h = static_cast<float>(surface->h);
	textRect.x = btn.rect.x + (btn.rect.w - textRect.w) / 2;
	textRect.y = btn.rect.y + (btn.rect.h - textRect.h) / 2;

	SDL_RenderTexture(renderer, texture, nullptr, &textRect);

	SDL_DestroyTexture(texture);
	SDL_DestroySurface(surface);

}

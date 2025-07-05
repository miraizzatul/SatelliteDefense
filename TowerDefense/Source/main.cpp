#include <SDL3/SDL.h> //include the sdl header

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO); //initialize sdl3

	//---------------------window setup----------------------//
	//Create sdl3 window (80*600)
	SDL_Window* Window = SDL_CreateWindow("Tower Defense",800, 600, SDL_EVENT_WINDOW_SHOWN);

	//set vsync before creating renderer
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

	//create a renderer
	SDL_Renderer* Renderer = SDL_CreateRenderer(Window, nullptr);

	//check if renderer is invalid
	if (!Renderer)
	{
		SDL_Log("Renderer failed: %s", SDL_GetError());
		SDL_DestroyWindow(Window);
		SDL_Quit();
		return 1;
	}

	//set white bg colour (R,G,B,A(0-255)
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);

	//clear the renderer (fills with draw colour
	SDL_RenderClear(Renderer);

	//present the renderer to the window
	SDL_RenderPresent(Renderer);

	//keeps running until the window is closed
	bool running = true;
	SDL_Event event;

	//------------------end window setup--------------------------//

	//----------------------------------------------input movements

	bool keys[SDL_SCANCODE_COUNT] = { false };//track held keys
	Uint64 now = SDL_GetPerformanceCounter();// get timestamp
	Uint64 last = 0;
	float deltaTime = 0.f;
	float speed = 200.f; // pixels per second

	//-------------------------------------------end input movements

	//Define grid settings
	const int tileCols = 10;
	const int tileRows = 8;
	const float tileWidth = 800.f / tileCols;
	const float tileHeight = 600.f / tileRows;

	//set rectangle location and size (x,y,w,h)
	SDL_FRect Tower = { 100.f, 250.f, 50.f, 100.f };

	while (running) //keeps the window running
	{
		SDL_Delay(1);//prevent the loop from hogging the cpu since sometimes cpu might run too fast and render frames too quicky

		last = now;
		now = SDL_GetPerformanceCounter();//current timestamp
		deltaTime = (now - last) / (float)SDL_GetPerformanceFrequency(); //SDL_GetPerformanceFrequency gives us number of ticks per second

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
		}
		//movement logic
		if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP]) Tower.y -= speed * deltaTime;
		if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]) Tower.y += speed * deltaTime;
		if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) Tower.x -= speed * deltaTime;
		if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) Tower.x += speed * deltaTime;

		//Clamp the tower from going offscreen
		if (Tower.x < 0) Tower.x = 0;//left edge
		if (Tower.y < 0) Tower.y = 0;//top edge
		if (Tower.x + Tower.w > 800)Tower.x = 800 - Tower.w;//(Tower.x+Tower.w) right edge of rectangle
		if (Tower.y + Tower.h > 600)Tower.y = 600 - Tower.h;//(Tower.y+Tower.h) bottom edge of rectangle

		SDL_SetRenderDrawColor(Renderer, 30, 30, 30, 255);//set draw colour dark grey
		SDL_RenderClear(Renderer);//clear the screen with grey colour

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
				if((row+col) %2 ==0)
					SDL_SetRenderDrawColor(Renderer, 220, 220, 220, 255); // light gray
				else
					SDL_SetRenderDrawColor(Renderer, 180, 180, 180, 255); // dark gray

				SDL_RenderFillRect(Renderer, &tileRect);
			}
		}
		//create green tower colour
		SDL_SetRenderDrawColor(Renderer, 0, 200, 0, 255);

		//render the tower on screen
		SDL_RenderFillRect(Renderer, &Tower);

		SDL_RenderPresent(Renderer);//present the result to the window
	}

	//cleanup
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();

	return 0;
}
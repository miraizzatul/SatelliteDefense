#include "HUD.h"

HUD::HUD(SDL_Renderer* renderer, TTF_Font* font, int windowWidth, int windowHeight)
	:renderer(renderer), font(font), windowWidth(windowWidth), windowHeight(windowHeight)
{

}

void HUD::RenderHealthBar(int health, int maxHealth)
{
	float percent = (float)health / maxHealth;
	SDL_FRect bg = { 20, 20, 200, 20 };
	SDL_FRect fill = bg;
	fill.w *= percent;

	SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255); SDL_RenderFillRect(renderer, &bg);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); SDL_RenderFillRect(renderer, &fill);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); SDL_RenderRect(renderer, &bg);
}

void HUD::RenderInventory(const BaseInventory& inventory)
{
	float x = 20, y = 50;
	for (const auto& item : inventory.GetItems()) 
	{
		DrawText(item.name + " x" + std::to_string(item.quantity), x, y);
		y += 20;
	}
}

void HUD::RenderMiniMap(const std::vector<std::unique_ptr<Tower>>& towers, const std::vector<Enemy>& enemies, float worldWidth, float worldHeight)
{
	const float minimapW = 200.f, minimapH = 150.f;
	SDL_FRect rect = { 10, windowHeight - minimapH - 10, minimapW, minimapH }; //set minimap location at the bottom left of the screen

	//converts world units into minimap units e.g. if the world is 1000 wide and the minimap is 200, scaleX = 0.2.
	float scaleX = minimapW / worldWidth;
	float scaleY = minimapH / worldHeight;

	SDL_SetRenderDrawColor(renderer, 20, 20, 20, 180); SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); SDL_RenderRect(renderer, &rect);

	for (const auto& t : towers)
	{
		SDL_FPoint p = t->GetPosition();
		SDL_FRect dot = { rect.x + p.x * scaleX - 2, rect.y + p.y * scaleY - 2, 4, 4 };// 4x4 pixel dot size (scaleX-2 centers the 4x4 dot)
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &dot);
	}
	for (const auto& e : enemies)
	{
		SDL_FPoint p = e.GetPosition();
		SDL_FRect dot = { rect.x + p.x * scaleX - 2, rect.y + p.y * scaleY - 2, 4, 4 };// 4x4 pixel dot size
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &dot);
	}

}

void HUD::RenderAll(int health, int maxHealth, const BaseInventory& inventory, const std::vector<std::unique_ptr<Tower>>& towers, const std::vector<Enemy>& enemies, float worldWidth, float worldHeight)
{
	//RenderHealthBar(health, maxHealth);
	RenderInventory(inventory);
	RenderMiniMap(towers, enemies, worldWidth, worldHeight);
}

void HUD::UpdateWindowSize(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
}

void HUD::DrawText(const std::string& text, float x, float y)
{
	SDL_Color white = { 255, 255, 255, 255 };//white
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), 0, white);
	if (!surface) return;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) return;

	SDL_FRect dstRect;
	dstRect.x = x;
	dstRect.y = y;
	dstRect.w = static_cast<float>(surface->w);
	dstRect.h = static_cast<float>(surface->h);

	SDL_RenderTexture(renderer, texture, nullptr, &dstRect);

	SDL_DestroyTexture(texture);
	SDL_DestroySurface(surface);
}

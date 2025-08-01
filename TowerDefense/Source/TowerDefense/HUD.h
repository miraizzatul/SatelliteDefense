#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include "Tower.h"
#include "Enemy.h"
#include "BaseInventory.h"

class HUD
{
public:
	HUD(SDL_Renderer* renderer, TTF_Font* font, int windowWidth, int windowHeight);


	void RenderHealthBar(int health, int maxHealth);
	void RenderInventory(const BaseInventory& inventory);
	void RenderMiniMap(const std::vector<std::unique_ptr<Tower>>& towers, const std::vector<Enemy>& enemies, float worldWidth, float worldHeight);

	void RenderAll(int health, int maxHealth, const BaseInventory& inventory, const std::vector<std::unique_ptr<Tower>>& towers, const std::vector<Enemy>& enemies, float worldWidth, float worldHeight);

	void UpdateWindowSize(int w, int h);//call this to resize window

private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	int windowWidth, windowHeight;

	void DrawText(const std::string& text, float x, float y);
};


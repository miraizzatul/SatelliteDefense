#pragma once

#include <SDL3/SDL.h>

class Texture
{
public:
	Texture(SDL_Texture* t = nullptr);
	~Texture();//destructor

	//move constructor = steal the resource from another object
	Texture(Texture&& o) noexcept;
	//move assignment operator= replace an existing resource with one stolen from another
	Texture& operator=(Texture&& o) noexcept;

	//delete copy for SDL_Texture to make the object non copyable
	Texture(const Texture&) = delete;//copy constructor = to make a new independent copy of the resource
	//copy assignment = replace an existing resource with a copy of another
	Texture& operator=(const Texture&) = delete;

	SDL_Texture* get() const;

private:
	SDL_Texture* tex = nullptr;
};
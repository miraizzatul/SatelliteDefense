#include "Texture.h"

Texture::Texture(SDL_Texture* t)
	:tex(t)
{

}

Texture::Texture(Texture&& o) noexcept
	:tex(o.tex)
{
	o.tex = nullptr;
}

SDL_Texture* Texture::get() const
{
	return tex;
}

Texture& Texture::operator=(Texture&& o) noexcept
{
	if (this != &o)
	{
		if (tex)
			SDL_DestroyTexture(tex);

		tex = o.tex;
		o.tex = nullptr;
	}
	return *this;
}

Texture::~Texture()
{
	if (tex)
		SDL_DestroyTexture(tex);
}

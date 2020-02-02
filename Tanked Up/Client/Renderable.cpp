#include "Renderable.h"



Renderable::Renderable()
{
}


Renderable::~Renderable()
{
	SDL_DestroyTexture(m_texture);
}
//Transparent color will deafult to r = 0, g = 255, b = 0 unless set to something besides -999
void Renderable::InitRender(const std::string path, SDL_Renderer* renderer, Uint32 transparentColor) {
	SDL_Surface* surface = SDL_LoadBMP(path.c_str());
	if (surface != nullptr)
	{
		SDL_SetColorKey(surface, SDL_TRUE, (transparentColor == -999 ? SDL_MapRGB(surface->format, 0, 255, 0) : transparentColor));
		m_shouldRender = true;
		m_texture = SDL_CreateTextureFromSurface(renderer, surface);
		m_destination.w = surface->w;
		m_destination.h = surface->h;
	}
	else
	{
		std::cout << SDL_GetError() << std::endl;
		std::cout << "Source is nullptr" << std::endl;
	}
	SDL_FreeSurface(surface);
}

SDL_Rect Renderable::GetDestination()
{
	return m_destination;
}

Vector2 Renderable::GetTextureSize() {
	return textureSize;
}

void Renderable::Render(SDL_Renderer* renderer) {
	if (m_shouldRender)
	{
		SDL_RenderCopyEx(renderer, m_texture, NULL, &m_destination, ((int)currentAngle), NULL, SDL_FLIP_NONE);
	}
}
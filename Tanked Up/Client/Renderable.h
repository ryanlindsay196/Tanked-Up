#pragma once
#include "Vector2.h"
#include "SDL.h"
#include <iostream>

class Renderable
{
private:
	SDL_Texture* m_texture;
	Vector2 textureSize;
protected:

	bool m_shouldRender = false;
	SDL_Rect m_destination;
	float currentAngle = 0;
	int lastCurrentAngle = 0;
public:
	Renderable();
	~Renderable();
	virtual void Render(SDL_Renderer* renderer);
	Vector2 GetTextureSize();
	void InitRender(const std::string path, SDL_Renderer* renderer, Uint32 transparentColor = -999);

	SDL_Rect GetDestination();
};


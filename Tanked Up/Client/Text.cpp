#include "Text.h"
#include "ClickableUI.h"

Text::Text()
{

}

Text::Text(Vector2 position, bool centered)
	: ClickableUI(position, centered)
{
	if (TTF_Init() < 0)
		std::cout << "Error: " << TTF_GetError() << std::endl;
	font = TTF_OpenFont("Fonts/Adventure.otf", 20);

}

Text::~Text()
{

}

void Text::Initialize()
{
	if (TTF_Init() < 0)
		std::cout << "Error: " << TTF_GetError() << std::endl;
	font = TTF_OpenFont("Fonts/Adventure.otf", 20);
}

void Text::SetText(char * in_Text)
{
	text = in_Text;
}

void Text::Render(SDL_Renderer * renderer)
{
	//Renderable::Render(renderer);

	SDL_Color color = { 144, 77, 255, 255 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, (char*)text.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect textRect;
	textRect.x = (int)m_position.X;
	textRect.y = (int)m_position.Y;
	//textRect.y = 0;

	SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);

	SDL_RenderCopy(renderer, texture, NULL, &textRect);


	SDL_FreeSurface(textSurface);
	textSurface = nullptr;
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

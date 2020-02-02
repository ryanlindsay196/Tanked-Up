#include "TextBox.h"
#include "SDL_ttf.h"

TextBox::TextBox()
{
	Initialize();
}

TextBox::TextBox(Vector2 position, bool centered)
{
	Initialize();
	SetPosition(position, centered);
	initialOffset = position;
}

TextBox::~TextBox()
{

}

void TextBox::Initialize()
{
	text = "";

	if (TTF_Init() < 0)
		std::cout << "Error: " << TTF_GetError() << std::endl;
	font = TTF_OpenFont("Fonts/Adventure.otf", 20);

}

void TextBox::GetKeyboardInput(SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN)
	{
		std::string keyStr = SDL_GetKeyName(e->key.keysym.sym);
		if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_BACKSPACE && text.length() > 0)//if a key is down, backspace is pressed, and text isn't empty
			text = text.substr(0, text.length() - 1);
		else if(keyStr.length() == 1)
		{
			text += SDL_GetKeyName(e->key.keysym.sym);
		}
	}

	
}

void TextBox::OnClick()
{
	isSelected = true;
	//std::cout << "TextBox::OnClick()::Clicked on textbox\n";
}

void TextBox::Render(SDL_Renderer* renderer)
{
	Renderable::Render(renderer);

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

std::string TextBox::GetText()
{
	return text;
}

//void TextBox::Update(float gameTime, SDL_Event* e)
//{
//	if (e->type == SDL_MOUSEBUTTONDOWN)
//	{
//		if (e->button.button == SDL_BUTTON_LEFT)
//		{
//			std::cout << "Do a thing\n";
//		}
//	}
//}
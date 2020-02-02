#pragma once
#include "ClickableUI.h"
#include "SDL_ttf.h"

class Text : public ClickableUI
{
public:
	Text();
	virtual ~Text();

private:
	std::string text;
	TTF_Font* font = nullptr;
public:
	Text(Vector2 position, bool centered);
	void Initialize();
	void SetText(char* in_Text);
	int cursorPosition;

	void Render(SDL_Renderer* renderer) override;
};


#pragma once
//#include "Thing.h"
#include "ClickableUI.h"
#include "SDL_ttf.h"

//class TTF_Font;

class TextBox : public ClickableUI
{
private:
	std::string text;
	TTF_Font* font = nullptr;
public:
	TextBox();
	TextBox(Vector2 position, bool centered);
	virtual ~TextBox();
	void Initialize();
	void GetKeyboardInput(SDL_Event* e) override;
	void OnClick() override;

	int cursorPosition;

	void Render(SDL_Renderer* renderer) override;

	std::string GetText();

	//void Update(float gameTime, SDL_Event* e) override;
};
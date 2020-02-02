#pragma once
//#include "Thing.h"
#include "ClickableUI.h"
#include "SDL_ttf.h"

//class TTF_Font;

class Button : public ClickableUI
{
private:
	std::string text;
	TTF_Font* font = nullptr;
public:
	Button();
	Button(Vector2 position, bool centered);
	virtual ~Button();
	void Initialize();
	void SetText(char* in_Text);
	void OnClick() override;

	int cursorPosition;

	void Render(SDL_Renderer* renderer) override;

	//void Update(float gameTime, SDL_Event* e) override;
}; 
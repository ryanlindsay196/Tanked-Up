#pragma once
#include "Screen.h"

class QueryLoader;

class StartScreen : public Screen
{
public:
	StartScreen(SDL_Renderer* in_Renderer, ScreenStack* in_ScreenStack);
	~StartScreen();

	bool Update(float gameTime, SDL_Event* e) override;
	void Initialize() override;
	void CleanUp() override;

protected:
	QueryLoader* checkSessionQuery;
};
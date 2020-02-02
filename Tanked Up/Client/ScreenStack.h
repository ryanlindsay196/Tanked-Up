#pragma once

#include "Screen.h"
#include <vector>

class ScreenStack
{
private:
	std::vector<Screen*> screens;
	SDL_Renderer* renderer;

public:
	ScreenStack(SDL_Renderer* in_Renderer);
	Screen* GetCurrentScreen();
	bool Update(float gameTime, SDL_Event* e);
	void Render();
	std::vector<Renderable*> GetRenderables();
	std::vector<Thing*> GetThings();

	//template <typename T>
	void AddScreen(Screen* screen);
	void RemoveScreen(int screenToRemove);
	void RemoveLastScreen();
	void FindScreenOfTypeOnStack(Screen* screenType);
	int CurrentScreenIndex();
};
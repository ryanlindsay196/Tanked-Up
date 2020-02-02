#include "ScreenStack.h"
#include<iostream>
ScreenStack::ScreenStack(SDL_Renderer* in_Renderer)
{
	renderer = in_Renderer;
}

bool ScreenStack::Update(float gameTime, SDL_Event* e)
{
	bool value = false;
	if (&screens != nullptr)
	{
		value = GetCurrentScreen()->Update(gameTime, e);
	}
	else
	{
		printf("screens vector address is null.");
	}
	


	//Check new screen to load
	Screen* screenToLoad = GetCurrentScreen()->GetNextScreenToLoad();
	if (screenToLoad != nullptr)
	{
		AddScreen(screenToLoad);
		screenToLoad->Initialize();
	}
	return value;
}

void ScreenStack::Render() {
	if (&screens != nullptr)
	{
		if (screens.size() > 0)
		{
			GetCurrentScreen()->Render();
		}
		else
		{
			std::cout << "No screen to render" << std::endl;
		}
	}
	else
	{
		printf("screens vector address is null.");
	}

	
}

std::vector<Renderable*> ScreenStack::GetRenderables()
{
	return GetCurrentScreen()->GetRenderables();
}

std::vector<Thing*> ScreenStack::GetThings()
{
	return GetCurrentScreen()->GetThings();
}

void ScreenStack::AddScreen(Screen* screen)
{
	printf("Loading screen(via add screen) %s\n", typeid(*screen).name());
	screens.push_back(screen);	
}

void ScreenStack::RemoveScreen(int screenToRemove)
{
	std::cout << "ScreenStack::RemoveScreen(int)::Not implemented yet\n";
}

void ScreenStack::RemoveLastScreen()
{
	GetCurrentScreen()->CleanUp();
	screens.pop_back();
}

void ScreenStack::FindScreenOfTypeOnStack(Screen* screenType)
{
	if (&screens != nullptr)
	{
		int i = screens.size() - 1;
		std::cout << "Looking for " << typeid(*screenType).name() << "\n";
		bool result = typeid(*screenType).name() != typeid(*screens[i]).name();
		while (result && i > 0)
		{
			result = typeid(*screenType).name() != typeid(*screens[i]).name();
			printf("Comparing %s and %s with result of %s\n", typeid(*screenType).name(), typeid(*screens[i]).name(), result > 0 ? "true" : "false");
			i--;
		}
		if (i <= 0)
		{
			i = 1;
		}
		while (screens.size() > i)
		{
			GetCurrentScreen()->CleanUp();
			screens.pop_back();
		}
		printf("loading screen %s\n", typeid(*GetCurrentScreen()).name());
	}
	else
	{
		printf("screens vector address is null.");
	}
	

}

int ScreenStack::CurrentScreenIndex()
{
	return (int)screens.size();
}

Screen* ScreenStack::GetCurrentScreen() {
	return screens[screens.size() - 1];
}



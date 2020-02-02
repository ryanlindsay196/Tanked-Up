#pragma once
#include"Vector2.h"
#include <vector>
#include "SDL.h"
class Thing;
class Renderable;
class ClickableUI;
class ScreenStack;

class Screen
{
protected:
	std::vector<Thing*> things;
	std::vector<Renderable*> renderables;

	std::vector<ClickableUI*> uiThings;
	std::vector<Renderable*> uiRenderables;

	Screen* nextScreenToLoad;

protected:
		SDL_Renderer* renderer;

		struct ThingGroup
		{
			Vector2 position;
			std::vector<ClickableUI*> uiThings;
		};
public:
	Screen(SDL_Renderer* in_Renderer, ScreenStack* in_ScreenStack);
	virtual ~Screen();
	virtual void Initialize();
	virtual void CleanUp();
	virtual bool Update(float gameTime, SDL_Event* e);
	virtual void Render();
	Thing* LoadNewThing(Thing* in_Thing, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor);
	Renderable* LoadNewRenderable(Renderable* in_Renderable, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor);
	Thing* LoadNewThingUI(ClickableUI* in_Thing, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor);
	Renderable* LoadNewRenderableUI(Renderable* in_Renderable, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor);
	Thing* LoadNewThingUI(ClickableUI* in_Thing, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor, ThingGroup& uiGroup);
	Renderable* LoadNewRenderableUI(Renderable* in_Renderable, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor, ThingGroup& uiGroup);

	void TranslateUIThingGroupPosition(ThingGroup& thingGroup, Vector2 movePosition);
	void SetUIThingGroupPosition(ThingGroup& thingGroup, Vector2 movePosition);
	void LoadNewScreen(Screen* screen);

public:
	Screen* GetNextScreenToLoad();
	std::vector<Thing*> GetThings();
	std::vector<Renderable*> GetRenderables();

	ClickableUI* lastClickedThing;
	ScreenStack* screenStack;
};
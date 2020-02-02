#include "Screen.h"
#include "ScreenStack.h"
#include "ClickableUI.h"
#include "Renderable.h"
#include "Thing.h"

Screen::Screen(SDL_Renderer* in_Renderer, ScreenStack* in_ScreenStack)
{
	renderer = in_Renderer;
	
	if (in_ScreenStack == nullptr)
	{
		printf("Screenstack is nullptr(Screen)\n");
	}
	else
	{
		screenStack = in_ScreenStack;
	}
	
	
}

Screen::~Screen()
{

}

bool Screen::Update(float gameTime, SDL_Event* e)
{
	//std::cout << "Screen::Update(float)::Updating screen; This should be a child of screen\n";

	if (lastClickedThing != nullptr && lastClickedThing->deselectsAfterFrame)
	{
		lastClickedThing->isSelected = false;
		lastClickedThing = nullptr;
	}
	
	if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		if(lastClickedThing != nullptr)
			lastClickedThing->isSelected = false;
		lastClickedThing = nullptr;
		for (int i = 0; i < uiThings.size(); i++)
		{
			if (e->button.button == SDL_BUTTON_LEFT)
			{
				if (e->button.x > uiThings[i]->GetDestination().x && e->button.x < uiThings[i]->GetDestination().x + uiThings[i]->GetDestination().w)
				{
					if (e->button.y > uiThings[i]->GetDestination().y && e->button.y < uiThings[i]->GetDestination().y + uiThings[i]->GetDestination().h)
					{
						lastClickedThing = uiThings[i];
						uiThings[i]->OnClick();
						break;
					}
				}
			}
		}
	}
	if (lastClickedThing != nullptr)
	{//if a clickableUIThing is selected
		lastClickedThing->GetKeyboardInput(e);
	}
	return false;//returning true will allow the play loop to exit
}


//template <typename T>
Thing* Screen::LoadNewThing(Thing* in_Thing, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor)
{
	renderer = in_Renderer;
	if (in_Thing == nullptr)
	{
		Thing* newThing = new Thing();
		newThing->InitRender(in_Path, renderer, transparentColor);
		things.push_back(newThing);
		renderables.push_back(newThing);
		return newThing;
	}
	else
	{
		in_Thing->InitRender(in_Path, renderer, transparentColor);
		things.push_back(in_Thing);
		renderables.push_back(in_Thing);
		return in_Thing;
	}
}

Renderable* Screen::LoadNewRenderable(Renderable* in_Renderable, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor)
{
	renderer = in_Renderer;
	if (in_Renderable == nullptr)
	{
		Renderable* newRenderable = new Renderable();
		newRenderable->InitRender(in_Path, renderer, transparentColor);
		renderables.push_back(newRenderable);
		return newRenderable;
	}
	else
	{
		in_Renderable->InitRender(in_Path, renderer, transparentColor);
		renderables.push_back(in_Renderable);
		return in_Renderable;
	}
}

Thing* Screen::LoadNewThingUI(ClickableUI* in_Thing, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor)
{
	in_Thing = (ClickableUI*)LoadNewThing(in_Thing, in_Path, in_Renderer, transparentColor);
	uiThings.push_back(in_Thing);
	uiRenderables.push_back(in_Thing);

	return in_Thing;
}

Renderable* Screen::LoadNewRenderableUI(Renderable* in_Renderable, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor)
{
	in_Renderable = LoadNewRenderable(in_Renderable, in_Path, in_Renderer, transparentColor);
	uiRenderables.push_back(in_Renderable);
	
	return in_Renderable;
}

Thing* Screen::LoadNewThingUI(ClickableUI* in_Thing, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor, ThingGroup& uiGroup)
{
	in_Thing = (ClickableUI*)LoadNewThing(in_Thing, in_Path, in_Renderer, transparentColor);
	uiThings.push_back(in_Thing);
	uiRenderables.push_back(in_Thing);
	uiGroup.uiThings.push_back(in_Thing);
	
	return in_Thing;
}

Renderable* Screen::LoadNewRenderableUI(Renderable* in_Renderable, std::string in_Path, SDL_Renderer* in_Renderer, Uint32 transparentColor, ThingGroup& uiGroup)
{
	in_Renderable = LoadNewRenderable(in_Renderable, in_Path, in_Renderer, transparentColor);
	uiRenderables.push_back(in_Renderable);
	uiGroup.uiThings.push_back((ClickableUI*)in_Renderable);

	return in_Renderable;
}

void Screen::TranslateUIThingGroupPosition(ThingGroup& thingGroup, Vector2 movePosition)
{
	for (int i = 0; i < thingGroup.uiThings.size(); i++)
	{
		thingGroup.uiThings[i]->SetPosition(Vector2(thingGroup.uiThings[i]->m_position.X + movePosition.X, thingGroup.uiThings[i]->m_position.Y + movePosition.Y), false);
		thingGroup.position += movePosition;
	}
}

void Screen::SetUIThingGroupPosition(ThingGroup& thingGroup, Vector2 movePosition)
{
	for (int i = 0; i < thingGroup.uiThings.size(); i++)
	{
		Vector2 offset = thingGroup.uiThings[i]->m_position + thingGroup.position;
		thingGroup.uiThings[i]->SetPosition(thingGroup.position + thingGroup.uiThings[i]->initialOffset, false);
	}
	thingGroup.position = movePosition;
}

void Screen::LoadNewScreen(Screen* screen)
{
	nextScreenToLoad = screen;
}

Screen* Screen::GetNextScreenToLoad()
{
	Screen* returnScreen = nextScreenToLoad;
	nextScreenToLoad = nullptr;//Set to null so you can go back a screen and not automatically reload the nextScreenToLoad
	return returnScreen;
}
void Screen::Render() {
	SDL_RenderClear(renderer);
	for (unsigned int i = 0; i < renderables.size(); i++)
	{
		renderables[i]->Render(renderer);
	}
	SDL_RenderPresent(renderer);
}

std::vector<Thing*> Screen::GetThings()
{
	return things;
}

std::vector<Renderable*> Screen::GetRenderables()
{
	return renderables;
}

void Screen::Initialize() {}
void Screen::CleanUp() {

	
	for (size_t i = 0; i < renderables.size(); i++)
	{
		if (renderables[i] != nullptr)
		{
			delete(renderables[i]);
			renderables[i] = nullptr;
		}
	}
}
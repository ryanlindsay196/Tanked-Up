#include "EndScreen.h"
#include "StartScreen.h"
#include "Renderable.h"
#include "Thing.h"
#include "QueryLoader.h"
#include <stdio.h>
#include "ScreenStack.h"
#include <iostream>

using namespace rapidjson;
using namespace std;

EndScreen::EndScreen(SDL_Renderer* in_Renderer, ScreenStack* in_ScreenStack)
	: Screen(in_Renderer, in_ScreenStack)
{
	if (in_ScreenStack == nullptr)
	{
		printf("screenstack being passed in is nullptr\n");
	}
}

EndScreen::EndScreen(SDL_Renderer* in_Renderer, QueryLoader* matchDataLoader, ScreenStack* in_ScreenStack)
	: Screen(in_Renderer, in_ScreenStack)
{
	if (in_ScreenStack == nullptr)
	{
		printf("screenstack being passed in is nullptr(exapnded constructor)\n");
	}
	matchData.hostKills = std::stoi(matchDataLoader->LoadValueChars("HostKills"));
	matchData.hostDeaths = std::stoi(matchDataLoader->LoadValueChars("HostDeaths"));
	matchData.clientKills = std::stoi(matchDataLoader->LoadValueChars("ClientKills"));
	matchData.clientDeaths = std::stoi(matchDataLoader->LoadValueChars("ClientDeaths"));

}

EndScreen::~EndScreen()
{

}

void EndScreen::Initialize() {
	LoadNewRenderable(nullptr, "img/EndScreenTank.bmp", renderer, Uint32(1));

}
void EndScreen::CleanUp() {
	Screen::CleanUp();
}

bool EndScreen::Update(float gameTime, SDL_Event* e)
{
	if (e->key.type == SDL_KEYDOWN)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_SPACE:
			std::cout << "EndScreen::Update(float)::Move to next screen\n";
			screenStack->FindScreenOfTypeOnStack(new StartScreen(renderer, screenStack));
			break;
		default:
			//std::cout << "EndScreen::Update(float)::Not moving to next screen\n";
			break;
		}
	}
	

	return false; //returning true will allow the play loop to exit if 
	//ALL SCREENS SHOULD RECEIVE INPUT
	//ALL SCREENS SHOULD BE ABLE TO INSTANTIATE ANOTHER SCREEN
}
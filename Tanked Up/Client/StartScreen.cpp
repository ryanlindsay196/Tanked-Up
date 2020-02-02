#include "StartScreen.h"
#include "LoginScreen.h"
#include "Tank.h"
#include "Renderable.h"
#include "Thing.h"
#include "QueryLoader.h"
#include <stdio.h>
#include <iostream>
#include "GameScreen.h"
#include "FindMatchScreen.h"

using namespace std;

StartScreen::StartScreen(SDL_Renderer* in_Renderer, ScreenStack* in_ScreenStack)
	: Screen(in_Renderer, in_ScreenStack)
{
}

StartScreen::~StartScreen()
{

}

void StartScreen::Initialize() {
	LoadNewRenderable(nullptr, "img/StartScreenTank.bmp", renderer, Uint32(1));
	LoadNewThing(nullptr, "img/StartScreenTank.bmp", renderer, Uint32(1));
	
	ifstream sessionIDFile;
	sessionIDFile.open("Session.txt");
	//std::string sessionString = std::getline((char*)sessionString.c_str(), 1);// std::getline(std::cin, "Session.txt");
	std::string sessionString{ istreambuf_iterator<char>(sessionIDFile), istreambuf_iterator<char>() };

	std::string linkToLoad = "http://ec2-3-82-247-86.compute-1.amazonaws.com/CheckSession.php?SessionID=" + sessionString;
	if (/*sessionString != NULL && sessionString != nullptr && */sessionString.length() > 1)
	{
		checkSessionQuery = new QueryLoader();
		checkSessionQuery->LoadPage((char*)linkToLoad.c_str());
		if(checkSessionQuery->ReturnedStringFromWebPage() == "\nTRUE")
		{//Session ID is valid
			LoadNewScreen(new FindMatchScreen(renderer, screenStack));
			std::cout << "StartScreen::Initialize()::Session ID is valid. Load new page\n";
		}
		//If we have a valid query
			//Load the match finder screen
		//}
		//catch()
		//{

		//}
	}
}
void StartScreen::CleanUp() {
	delete checkSessionQuery;
	Screen::CleanUp();
}

bool StartScreen::Update(float gameTime, SDL_Event* e)
{
	if (e->key.type == SDL_KEYDOWN)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_SPACE:
			std::cout << "StartScreen::Update(float)::Move to next screen\n";
			LoadNewScreen(new LoginScreen(renderer, screenStack));
			break;
		default:
			//std::cout << "StartScreen::Update(float)::Not moving to next screen\n";
			break;
		}
	}
	
	return false; //returning true will allow the play loop to exit if 
	//ALL SCREENS SHOULD RECEIVE INPUT
	//ALL SCREENS SHOULD BE ABLE TO INSTANTIATE ANOTHER SCREEN
}
#include "GameScreen.h"
#include "GameState.h"
#include "ClickableUI.h"
#include "Renderable.h"
#include "Thing.h"

GameScreen::GameScreen(SDL_Renderer* in_Renderer, bool _isHost, std::string _hostName, int _portNum, ScreenStack* in_ScreenStack)
	: Screen(in_Renderer, in_ScreenStack)
{
	if (in_ScreenStack == nullptr)
	{
		printf("Screenstack is nullptr(GameScreen)\n");
	}
	isHost = _isHost;
	hostName = _hostName;
	portNum = _portNum;
	
}


GameScreen::~GameScreen()
{

}

bool GameScreen::Update(float gameTime, SDL_Event* e) {
	
	//core loops
	if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		if (lastClickedThing != nullptr)
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

	bool quit = GameState::GetGameState()->CheckInput(gameTime, e);
	if (screenStack != nullptr)
	{
		GameState::GetGameState()->Update(gameTime, screenStack);
	}
	else
	{
		printf("screenstack is null\n");
	}
	
	return quit;
	
}
void GameScreen::Render() 
{
	GameState::GetGameState()->RenderGame(renderer);
}
void GameScreen::Initialize() {
	std::string m =
		"####################"//0
		"#__________________#"//1
		"#__________________#"//2
		"#____##______##____#"//3
		"#____##______##____#"//4
		"#__________________#"//5
		"#__________________#"//6
		"#__________________#"//7
		"#__________________#"//8
		"#__________________#"//9
		"#____##______##____#"//10
		"#____##______##____#"//11
		"#__________________#"//12
		"#__________________#"//13
		"####################";//14

	//Initialize the game state
	GameState::GetGameState()->Initialize(renderer, this);
	//Set up server or client (depends on isHost value)
	GameState::GetGameState()->InitConnection(isHost, hostName, portNum);

	//Create level
	GameState::GetGameState()->LoadInLevel(m, 20, 15, renderer);
	//load players
	GameState::GetGameState()->CreateTestPlayers(isHost);
	//GameState::GetGameState()->OpenLoadingScreen("img/ConnectingScreen.bmp");
	
}
void GameScreen::CleanUp() {
	
	GameState::GetGameState()->CleanUp();

	Screen::CleanUp();
}
#include "GameState.h"
#include "Wall.h"
#include "Tank.h"
#include "Networker.h"
#include "Thing.h"
#include "Player.h"
#include "JsonFormatter.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "Timer.h"
#include <iostream>
#include<fstream>
#include "QueryLoader.h"
#include "EndScreen.h"


GameState::GameState()
{

}


GameState::~GameState()
{
	
}

void GameState::StartGame(){}
void GameState::SendHighScore(){} 
void GameState::Pause(bool pauseVal){
	m_paused = pauseVal;
}
bool GameState::GetPaused() { return m_paused; }
void GameState::LoadInLevel(std::string& map, int tileCountHorizontal, int tileCountVertical, SDL_Renderer* r)
{
	Renderable* background = new Thing();
	background->InitRender("img/background.bmp", r);
	Renderables.push_back(background);	
	int xwidth = 32;
	int ywidth = 32;
	int col = 0;
	int row = 0;
	Vector2 v;
	for (auto i = map.begin(); i != map.end(); i++)
	{
		if (col == tileCountHorizontal)
		{
			col = 0;
			row++;
		}
		
		switch (*i)
		{
		case '#':
			Wall* w;
			w = new Wall();
			//refactor at some point to only use one texture		
			AddThing(w, "img/Wall.bmp");
			w->SetPosition((float)(xwidth * col), (float)(ywidth * row), false);
			w->m_ShouldCollisionCheck = false;
			w->Name = "Wall " + std::to_string(col) + ", " + std::to_string(row);
			col++;
			break;
		case '$':
			Wall* movingWall;
			v = Vector2((float)(xwidth * col), (float)(ywidth * row));
			movingWall = new Wall();
			//refactor at some point to only use one texture			
			AddThing(movingWall, "img/Wall.bmp");
			movingWall->SetPosition((float)(xwidth * col), (float)(ywidth * row), false);
			movingWall->SetPath({ v + Vector2(xwidth * 5, 0) ,  v + Vector2(xwidth * 9, 0), v + Vector2(xwidth * 9, ywidth), v + Vector2(xwidth * 7, ywidth) }, Obstacle::MoveType::InverseAtEnd, 100);
			movingWall->Name = "Moving Wall " + std::to_string(col) + ", " + std::to_string(row);
			movingWallsData.push_back(movingWall);
			col++;
			break;
		case'_':
				col++;
				break;
		default:
			break;
		}
	}
}
void GameState::OpenLoadingScreen(std::string _path)
{
	Renderable* loadingScreen = new Renderable();
	loadingScreen->InitRender(_path, rendererRef);
	Renderables.push_back(loadingScreen);
}
//Returns the index of the created player
Player* GameState::CreatePlayer(const std::string& loginName, bool host){
	Player* p = new Player(loginName, host);
	Players.push_back(p);
	return p;
}
void GameState::Update(float deltaTime, ScreenStack* in_ScreenStack){
	if (!m_allowPlay)
	{
		checkForPartyTime(deltaTime);
		NetworkingUpdate(deltaTime);
		return;
	}
	else
	{	
		if (delayEndGame->GetShouldCountDown() && delayEndGame->CountDown(deltaTime))//if (delayEndGame->CountDownAutoCheckBool(deltaTime))
		{
			sendEndGameData(in_ScreenStack);
		}
	}


	CollisionCheck();

	for (auto i = Things.cbegin(); i != Things.cend(); ++i)
	{
		(*i)->Update(deltaTime);
	}
	obstacleDataSendRecieve(deltaTime);
	NetworkingUpdate(deltaTime);

}

void GameState::obstacleDataSendRecieve(float dt) {

	if (m_isHost)
	{
		if (ObstacleDataSendRate->CountDown(dt))
		{
			std::string sendData = setObstacleDataToSend();
			networkingHandler->SendData(sendData, false);
		}
	}
	else
	{
		loadObstacleData();
	}
}
bool GameState::CheckInput(float dt, SDL_Event* e) {
	if ((*e).type == SDL_KEYDOWN)
	{

		if (m_allowPlay)
		{
			for (int i = 0; i < GameState::GetGameState()->Players.size(); i++)
			{
				GameState::GetGameState()->Players[i]->TakeInput(e->key.keysym.sym, dt, false);
			}
		}
		
		switch (e->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			return true;
			break;
		default:
			break;
		}
	}
	else if (e->type == SDL_KEYUP && m_allowPlay)
	{
		for (int i = 0; i < GameState::GetGameState()->Players.size(); i++)
		{
			GameState::GetGameState()->Players[i]->TakeInput(e->key.keysym.sym, dt, true);
		}
	}

	return false;
}

std::string GameState::setObstacleDataToSend() {
	if (m_isHost)
	{
		std::string toSend;
		JsonFormatter jf = JsonFormatter();
		
		
		std::vector<float> xs;
		std::vector<float> ys;
		std::vector<int> currentPos;
		for (unsigned int i = 0; i < movingWallsData.size(); ++i)
		{
			xs.push_back(movingWallsData[i]->m_position.X);
			ys.push_back(movingWallsData[i]->m_position.Y);
			currentPos.push_back(movingWallsData[i]->GetCurrentIndex());
		}
		jf.FormatDataToSendBegin(toSend, JsonFormatter::ID::Obstacles);
		jf.FormatDataToSend(toSend, "positions_x", xs);
		jf.FormatDataToSend(toSend, "positions_y", ys);
		jf.FormatDataToSend(toSend, "currentIndex", currentPos);
		jf.FormatDataToSendEnd(toSend);
		return toSend;
	}
	return "";
}
void GameState::loadObstacleData() {
	

	std::string jsonString = networkingHandler->GetObstacleData();
	if (jsonString != "")
	{
		rapidjson::Document d;
		if (d.Parse(jsonString.c_str()).HasParseError()) {
			std::cout << " Obstacle parse error occured" << std::endl;
		}
		else
		{
			rapidjson::Value xs = d["positions_x"].GetArray();
			rapidjson::Value ys = d["positions_y"].GetArray();
			rapidjson::Value ind = d["currentIndex"].GetArray();
			
			for (unsigned int i = 0; i < movingWallsData.size(); ++i)
			{
				movingWallsData[i]->SetPosition(xs[i].GetFloat(), ys[i].GetFloat(), false);
				movingWallsData[i]->SetCurrentIndex(ind[i].GetInt());
			}
		}
	}
}
void GameState::AddThing(Thing* t, const std::string& path, Uint32 transparentColor) {

	if (rendererRef != nullptr)
	{
		Renderables.push_back(t);
		Things.push_back(t);
		t->InitRender(path, rendererRef, transparentColor);
	}
	else
	{
		printf("Render ref is null\n");
	}
}
void GameState::AddThing(Thing* t) {

	Renderables.push_back(t);
	Things.push_back(t);
}

void GameState::RenderGame(SDL_Renderer* renderer) {
	SDL_RenderClear(renderer);
	for (unsigned int i = 0; i < Renderables.size(); i++)
	{
		Renderables[i]->Render(renderer);
	}
	SDL_RenderPresent(renderer);
}
void GameState::Initialize(SDL_Renderer* renderer, Screen* _screen)
{

	rendererRef = renderer;
	myScreen = _screen;
	ObstacleDataSendRate = new Timer(3);
	queryRateInSeconds = new Timer(0.5f);
	delayEndGame = new Timer(1.0f);
	delayEndGame->SetShouldCountDown(false);
	readyUpQueryLoader = new QueryLoader();
	
}

void GameState::InitConnection(bool isServer, std::string hostName, int port) {
	if (networkingHandler == nullptr)
	{
		m_isHost = isServer;
		networkingHandler = new Networker();
		networkingHandler->CreateHost(isServer, hostName, port);
		printf("Network handler created\n");
	}
	else
	{
		printf("Network handler already created!\n");
	}
}
void GameState::SendData(std::string& jsonString, bool reliable, unsigned long int _packageNum) {
	if (networkingHandler != nullptr)
	{
		networkingHandler->SendData(jsonString, reliable, _packageNum);
	}
	else
	{
		printf("Cannot send data networkingHandler is null.\n");
	}
}

void GameState::NetworkingUpdate(float dt) {
	if (networkingHandler != nullptr)
	{
		networkingHandler->EnetDataUpdate(dt);
		networkingHandler->ClientAttemptReconnect(dt);
	}
	else
	{
		printf("networkingHandler is null.\n");
	}
}
bool GameState::GetIsHost() { return m_isHost; }
std::string GameState::GetJsonTankData() { return networkingHandler->GetJSonOfOtherTank(); }
void GameState::checkForPartyTime(float dt) 
{
	if (queryRateInSeconds->CountDown(dt))
	{
		if (networkingHandler != nullptr && networkingHandler->GetIsConnected() || m_testSinglePlayer)
		{
			if (!m_poppedWaitingScreen)
			{
				Renderable* r = Renderables[Renderables.size() - 1];
				Renderables.pop_back();
				delete(r);
				m_poppedWaitingScreen = true;
			}
			
			std::string id = "";
			std::ifstream myFile;
			std::string path = "Session.txt";
			myFile.open(path);
			char output[100];
			if (myFile.is_open())
			{
				myFile.getline(output, 100);
				id = std::string(output);
			}
			else
			{
				std::string errorMessage = "Could not open file " + path;
				printf(errorMessage.c_str());
			}
			myFile.close();

			std::string pageToLoad = "http://ec2-3-82-247-86.compute-1.amazonaws.com/ReadyUp.php?SessionID=" + id;

			readyUpQueryLoader->LoadPage((char*)pageToLoad.c_str());

			if(readyUpQueryLoader->ReturnedStringFromWebPage() == (char*)"connected" || m_testSinglePlayer)
			{
				m_allowPlay = true;
			}
		}
	}
}
void GameState::sendEndGameData(ScreenStack* in_ScreenStack) 
{
	printf("Entered sendEndGameData()\n");
	int localKills, localDeaths, localWon = 0;
	for (int i = 0; i < Players.size(); i++)
	{
		if (Players[i]->GetLocal())
		{
			Players[i]->GetAvatar()->GetEndGameData(localKills, localDeaths, localWon);
			break;
		}
	}

		std::string id = "";
		std::ifstream myFile;
		std::string path = "Session.txt";
		myFile.open(path);
		char output[100];
		if (myFile.is_open())
		{
			myFile.getline(output, 100);
			id = std::string(output);
		}
		else
		{
			std::string errorMessage = "Could not open file " + path;
			printf(errorMessage.c_str());
		}
		myFile.close();

		std::string linkToLoad = "http://ec2-3-82-247-86.compute-1.amazonaws.com/EndMatch.php?SessionID=" + id + "&Kills=" + std::to_string(localKills) + "&Deaths=" + std::to_string(localDeaths) + "&Win=" + std::to_string(localWon);
		printf(linkToLoad.c_str());
		
		readyUpQueryLoader->LoadPage((char*)linkToLoad.c_str());
		if (readyUpQueryLoader->ReturnedStringFromWebPage() != "Needs other player's match data" && readyUpQueryLoader->ReturnedStringFromWebPage() != "Invalid Match Results")
		{
			//std::cout << readyUpQueryLoader->ReturnedStringFromWebPage() << std::endl;
			myScreen->LoadNewScreen(new EndScreen(rendererRef, readyUpQueryLoader, in_ScreenStack));
			delayEndGame->SetShouldCountDown(false);
		}
		if (readyUpQueryLoader->ReturnedStringFromWebPage() == "Invalid Match Results")
		{
			std::cout << "Invalid Match Results\n";
			myScreen->LoadNewScreen(new EndScreen(rendererRef, in_ScreenStack));
		}
}

void GameState::PrepToSendEndGameData() 
{
	delayEndGame->SetShouldCountDown(true);
}

void GameState::CreateTestPlayers(bool isHost) {
	{
		Player* p = CreatePlayer("Hugh Test", (isHost ? true : false));		
		AddThing(p->CreatePlayerAvatar(288, 306, false), "img/Tank_2.bmp");
	}

	{
		Player* p = CreatePlayer("Lola Test", (isHost ? false : true));
		AddThing(p->CreatePlayerAvatar(288, 112, true), "img/Tank_1.bmp");
		
	}
}

void GameState::CollisionCheck() 
{
	//refactor so that the first loop goes backwards removing the entry at the end with each loop.  Thus each loop of the second loop will be shorter.
	std::vector<Thing*> thingCopy = Things;
	Thing* currentCheck = nullptr;
	for (std::vector<Thing*>::const_reverse_iterator i = thingCopy.crbegin(); i != thingCopy.crend(); ++i)
	{
			currentCheck = (*i);
			//thingCopy.erase();
			for (auto j = thingCopy.begin(); j < thingCopy.end(); j++)
			{
				
				if (currentCheck != (*j) && currentCheck->CollideCheck(*j))
				{					
					//if it collides once we ignore the rest of the vector
					break;
				}
				
				
			}
	}
}

bool GameState::GetTestingSinglePlayer() {
	return m_testSinglePlayer;
}

void GameState::CleanUp() {


	//clear tanks up
	for (int i = Players.size() -1; i > -1; i--)
	{
	Tank* t = Players[i]->GetAvatar();
		if (t != nullptr)
			t->CleanUp();
		delete(Players[i]);
		Players[i] = nullptr;
	}
	Players.clear();
	//clear renderables
	for (int i = Renderables.size() -1; i > -1; i--)
	{
		delete(Renderables[i]);
		Renderables[i] = nullptr;
	}
	Things.clear();
	Renderables.clear();
	networkingHandler->CleanUp();
	delete(networkingHandler);
	networkingHandler = nullptr;
	//delete timers
	delete(ObstacleDataSendRate);
	ObstacleDataSendRate = nullptr;
	delete(queryRateInSeconds);
	queryRateInSeconds = nullptr;
	delete(delayEndGame);
	delayEndGame = nullptr;	
	m_allowPlay = false;
	m_poppedWaitingScreen = false;
	
}
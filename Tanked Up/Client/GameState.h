#pragma once
#include <Vector>
#include "SDL_events.h"
#include <stdio.h>
#include "SDL.h"
#include "ScreenStack.h"

class Renderable;
class HUD;
class Networker;
class Player;
class Thing;
class Wall;
class Timer;
class QueryLoader;
class Screen;

class GameState
{
private:
		const bool m_testSinglePlayer = false;
		bool m_allowPlay = false;
		bool m_paused = false;
		bool m_isHost = true;

		bool m_poppedWaitingScreen = false;
		std::string levelLayout = "";
		SDL_Renderer* rendererRef;
		SDL_Event e;
		Networker* networkingHandler = nullptr;
		Timer* ObstacleDataSendRate =  nullptr;
		Timer* queryRateInSeconds = nullptr;
		Timer* delayEndGame = nullptr;
		QueryLoader* readyUpQueryLoader;
		Screen* myScreen;
		std::vector<Wall*> movingWallsData; //data that is sent to clients from host to synch
		void obstacleDataSendRecieve(float dt);
		void checkForPartyTime(float dt);//at intervals will query server to ask if both clients have connected.
		void sendEndGameData(ScreenStack* in_ScreenStack);
		
public:
	std::vector<Renderable*> Renderables;
	std::vector<Thing*> Things;
	std::vector<Player*> Players;
	HUD* hud;
	
	GameState();
	~GameState();
	void Initialize(SDL_Renderer* renderer, Screen* _screen);
	void StartGame();
	void LoadInLevel(std::string& map, int tileCountHorizontal, int tileCountVeritcal, SDL_Renderer* r);
	void Update(float deltaTime, ScreenStack* in_ScreenStack);
	void RenderGame(SDL_Renderer* renderer);

	void Pause(bool pauseVal);
	bool GetPaused();

	bool GetTestingSinglePlayer();

	void AddThing(Thing* t, const std::string& path = "", Uint32 transparentColor = -999);
	void AddThing(Thing* t);
	void CleanUp();

	void CollisionCheck();

	void CreateTestPlayers(bool isHost);
	Player* CreatePlayer(const std::string& loginName, bool host);

	void InitConnection(bool isServer, std::string hostName, int port);
	void NetworkingUpdate(float dt);
	void SendData(std::string& jsonString, bool reliable, unsigned long int _packageNum = 0);
	std::string setObstacleDataToSend();

	bool GetIsHost();
	void loadObstacleData();
	std::string GetJsonTankData();
	
	
	void SendHighScore();
	void PrepToSendEndGameData();

	bool CheckInput(float dt, SDL_Event* e);
	void OpenLoadingScreen(std::string _path);
	static GameState* GetGameState() {
		static GameState PsudoSingleton;
		return &PsudoSingleton;
	}
};


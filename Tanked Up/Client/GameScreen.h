#pragma once
#include "Screen.h"
class GameScreen :
	public Screen
{
	bool isHost;
	std::string hostName;
	int portNum;
public:
	GameScreen(SDL_Renderer* in_Renderer, bool _isHost, std::string _hostName, int _portNum, ScreenStack* in_ScreenStack);
	~GameScreen();
	bool Update(float gameTime, SDL_Event* e) override;
	void Render() override;
	void Initialize() override;
	void CleanUp() override;
};


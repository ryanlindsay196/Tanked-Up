#pragma once
#include "Screen.h"

class QueryLoader;

class FindMatchScreen : public Screen
{
public:
	FindMatchScreen(SDL_Renderer* in_Renderer, ScreenStack* in_ScreenStack);
	~FindMatchScreen();

	bool Update(float gameTime, SDL_Event* e) override;
	void CleanUp() override;
private:

	std::string sessionID;
	QueryLoader* queryLoader;
	float serverPingFrequency;
	float serverPingTimer;

	void CheckForFoundMatch();
};
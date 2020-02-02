#pragma once
#include "Screen.h"

class QueryLoader;

class EndScreen : public Screen
{
public:
	EndScreen(SDL_Renderer* in_Renderer, ScreenStack* in_ScreenStack);
	EndScreen(SDL_Renderer* in_Renderer, QueryLoader* matchDataLoader, ScreenStack* in_ScreenStack);
	~EndScreen();

	struct MatchData
	{
		int hostKills;
		int hostDeaths;
		int clientKills;
		int clientDeaths;
		bool won;
	};
	MatchData matchData;

	bool Update(float gameTime, SDL_Event* e) override;
	void Initialize() override;
	void CleanUp() override;

protected:
};

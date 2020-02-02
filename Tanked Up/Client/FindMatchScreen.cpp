#include "FindMatchScreen.h"
#include "QueryLoader.h"
#include "GameScreen.h"

using namespace std;
extern std::string g_hostName;

FindMatchScreen::FindMatchScreen(SDL_Renderer * in_Renderer, ScreenStack* in_ScreenStack)
	: Screen(in_Renderer, in_ScreenStack)
{
	
	serverPingFrequency = 5;
	serverPingTimer = serverPingFrequency;

	ifstream sessionIDFile;
	sessionIDFile.open("Session.txt", 0);
	getline(sessionIDFile, sessionID);
	cout << sessionID;
	sessionIDFile.close();
	std::string linkToLoad = "http://ec2-3-82-247-86.compute-1.amazonaws.com/AddUserToQueue.php?SessionID=" + sessionID;
	queryLoader = new QueryLoader();
	queryLoader->LoadPage((char*)linkToLoad.c_str());
}

FindMatchScreen::~FindMatchScreen()
{
}

bool FindMatchScreen::Update(float gameTime, SDL_Event * e)
{
	serverPingTimer += gameTime;
	if (serverPingTimer > serverPingFrequency)
	{
		CheckForFoundMatch();
	}
	return false;
}

void FindMatchScreen::CheckForFoundMatch()
{
	std::string linkToLoad = "http://ec2-3-82-247-86.compute-1.amazonaws.com/CheckForMatch.php?SessionID=" + sessionID + "&IP=" + g_hostName;
	queryLoader->LoadPage((char*)linkToLoad.c_str());
	//cout << queryLoader->ReturnedStringFromWebPage();
	serverPingTimer = 0;

	if (queryLoader->ReturnedStringFromWebPage() != "NO VALID MATCHUPS")
	{
		if (queryLoader->LoadValueBool("IsHost") == true)
		{//if found match as host
			nextScreenToLoad = new GameScreen(renderer, true, queryLoader->LoadValueChars("IPAddress"), 1234, screenStack);
		}
		else
		{//if found match as client
			nextScreenToLoad = new GameScreen(renderer, false, queryLoader->LoadValueChars("IPAddress"), 1234, screenStack);
		}
	}
}
void FindMatchScreen::CleanUp(){
	delete(queryLoader);
	Screen::CleanUp();
}
#include "LoginScreen.h"
#include "QueryLoader.h"
#include "TextBox.h"
#include "Button.h"
#include <stdio.h>
#include "Vector2.h"
#include <string>
#include "FindMatchScreen.h"

using namespace std;

LoginScreen::LoginScreen(SDL_Renderer* in_Renderer, ScreenStack* in_ScreenStack)
	: Screen(in_Renderer, in_ScreenStack)
{
	loginPageLoader = new QueryLoader();

	login_UserNameTextBox = new TextBox(Vector2(240, 40), false);
	login_PasswordTextBox = new TextBox(Vector2(240, 80), false);

	createUser_UserNameTextBox = new TextBox(Vector2(640, 40), false);
	createUser_PasswordTextBox = new TextBox(Vector2(640, 80), false);
	createUser_ConfirmPasswordTextBox = new TextBox(Vector2(640, 120), false);

	confirmUserDataButton = new Button(Vector2(200, 400), false);
	switchToLoginButton = new Button(Vector2(50, 300), false);
	switchToCreateUserButton = new Button(Vector2(350, 300), false);

	login_UserNameTextBox = (TextBox*)LoadNewThingUI(login_UserNameTextBox, "img/TextBox.bmp", in_Renderer, Uint32(1), loginScreenThings);
	login_PasswordTextBox = (TextBox*)LoadNewThingUI(login_PasswordTextBox, "img/TextBox.bmp", in_Renderer, Uint32(1), loginScreenThings);
	
	createUser_UserNameTextBox = (TextBox*)LoadNewThingUI(createUser_UserNameTextBox, "img/TextBox.bmp", in_Renderer, Uint32(1), createUserScreenThings);
	createUser_PasswordTextBox = (TextBox*)LoadNewThingUI(createUser_PasswordTextBox, "img/TextBox.bmp", in_Renderer, Uint32(1), createUserScreenThings);
	createUser_ConfirmPasswordTextBox = (TextBox*)LoadNewThingUI(createUser_ConfirmPasswordTextBox, "img/TextBox.bmp", in_Renderer, Uint32(1), createUserScreenThings);




	createUserNameText = new Text(Vector2(640 + 180, 40), false);
	createPasswordText = new Text(Vector2(640 + 180, 80), false);
	confirmPasswordText = new Text(Vector2(640 + 180, 120), false);
	userNameText = new Text(Vector2(240 + 180, 40), false);
	passwordText = new Text(Vector2(240 + 180, 80), false);

	createUserNameText = (Text*)LoadNewThingUI(createUserNameText, "img/TextBox.bmp", in_Renderer, Uint32(1), createUserScreenThings);
	createPasswordText = (Text*)LoadNewThingUI(createPasswordText, "img/TextBox.bmp", in_Renderer, Uint32(1), createUserScreenThings);
	confirmPasswordText = (Text*)LoadNewThingUI(confirmPasswordText, "img/TextBox.bmp", in_Renderer, Uint32(1), createUserScreenThings);
	userNameText = (Text*)LoadNewThingUI(userNameText, "img/TextBox.bmp", in_Renderer, Uint32(1), loginScreenThings);
	passwordText = (Text*)LoadNewThingUI(passwordText, "img/TextBox.bmp", in_Renderer, Uint32(1), loginScreenThings);
	
	createUserNameText->SetText((char*)"Create User Name");
	createPasswordText->SetText((char*)"Create Password");
	confirmPasswordText->SetText((char*)"Confirm Password");
	userNameText->SetText((char*)"Enter User Name");
	passwordText->SetText((char*)"Enter Password");

	//Confirm Button
	confirmUserDataButton = (Button*)LoadNewThingUI(confirmUserDataButton, "img/DoneLogin.bmp", in_Renderer, Uint32(1));

	//Switch to login button
	switchToLoginButton = (Button*)LoadNewThingUI(switchToLoginButton, "img/Login.bmp", renderer, Uint32(1));

	//switch to create user button
	switchToCreateUserButton = (Button*)LoadNewThingUI(switchToCreateUserButton, "img/CreateUser.bmp", renderer, Uint32(1));
}

LoginScreen::~LoginScreen()
{

}

void LoginScreen::SwitchSubScreen()
{
	switch (pageToLoad)
	{
	case PageToLoad::CreateUser:
		SetUIThingGroupPosition(loginScreenThings, Vector2::Lerp(loginScreenThings.position, Vector2(-240.f * 2, loginScreenThings.position.Y), 0.1f, 0.2f));
		SetUIThingGroupPosition(createUserScreenThings, Vector2::Lerp(createUserScreenThings.position, Vector2(-240.f * 2, createUserScreenThings.position.Y), 0.1f, 0.2f));

		break;
	case PageToLoad::Login:
		SetUIThingGroupPosition(loginScreenThings, Vector2::Lerp(loginScreenThings.position, Vector2(0.f, loginScreenThings.position.Y), 0.1f, 0.2f));
		SetUIThingGroupPosition(createUserScreenThings, Vector2::Lerp(createUserScreenThings.position, Vector2(0.f, createUserScreenThings.position.Y), 0.1f, 0.2f));
		break;
	}
}


bool LoginScreen::Update(float gameTime, SDL_Event* e)
{
	//std::cout << "LoginScreen::Update(float,SDL_Event*)::Updating login screen\n";

	//login_PasswordTextBox->SetPosition(login_PasswordTextBox->m_position + Vector2(1, 1));
	Screen::Update(gameTime, e);
	SwitchSubScreen();
	if (switchToLoginButton->isSelected)
	{
		pageToLoad = PageToLoad::Login;
	}
	else if (switchToCreateUserButton->isSelected)
	{
		pageToLoad = PageToLoad::CreateUser;
	}
	if (confirmUserDataButton->isSelected && (sessionString == "" || sessionString == NULL || sessionString == nullptr) && (strlen(login_UserNameTextBox->GetText().c_str()) > 0 && strlen(login_PasswordTextBox->GetText().c_str()) > 0))//Confirm Button Clicked
	{
		std::string pageName = "";

		if (pageToLoad == PageToLoad::Login)
		{
			pageName = "LogIn";
		}
		else if (pageToLoad == PageToLoad::CreateUser)
		{
			pageName = "CreateUser";
		}
		std::string linkToLoad = "http://ec2-3-82-247-86.compute-1.amazonaws.com/" + pageName + ".php?username=" + login_UserNameTextBox->GetText() + "&Password=" + login_PasswordTextBox->GetText();
		SendLoginInfo((char*)linkToLoad.c_str());
	}
	return false;
}


void LoginScreen::SendLoginInfo(char* in_Endpoint)
{
	loginPageLoader->LoadPage(in_Endpoint);
	sessionString = loginPageLoader->LoadValueChars((char*)"SessionString");
	ofstream sessionIDFile;
	sessionIDFile.open("Session.txt");
	sessionIDFile << sessionString;
	sessionIDFile.close();

	if (strlen(sessionString) > 0)
	{
		std::cout << "LoginScreen::SendLoginInfo(char*)::Valid session string created\n";
		LoadNewScreen(new FindMatchScreen(renderer, screenStack));
	}
}
#pragma once
#include "Screen.h"
#include "Text.h"
//#include "QueryLoader.h"
//#include "TextBox.h"
class QueryLoader;
class TextBox;
class Button;

class LoginScreen : public Screen
{
private:
	char* sessionString;

	QueryLoader* loginPageLoader;
	enum PageToLoad {Login, CreateUser};
	PageToLoad pageToLoad;
	void SwitchSubScreen();

#pragma region UI elements
#pragma region TextBoxes
	TextBox* createUser_UserNameTextBox;
	TextBox* createUser_PasswordTextBox;
	TextBox* createUser_ConfirmPasswordTextBox;

	TextBox* login_UserNameTextBox;
	TextBox* login_PasswordTextBox;
#pragma endregion

#pragma region Buttons
	Button* confirmUserDataButton;
	Button* switchToLoginButton;
	Button* switchToCreateUserButton;
//switch to create user button
//Move create user button
#pragma endregion

#pragma region Text
	Text* createUserNameText;
	Text* createPasswordText;
	Text* confirmPasswordText;

	Text* userNameText;
	Text* passwordText;

#pragma endregion
#pragma endregion


	ThingGroup loginScreenThings;
	//std::vector<Thing*> loginScreenThings;
	ThingGroup createUserScreenThings;
	//std::vector<Thing*> createUserScreenThings;
public:
	LoginScreen(SDL_Renderer* in_Renderer, ScreenStack* in_ScreenStack);
	~LoginScreen();

	bool Update(float gameTime, SDL_Event* e) override;

	void SendLoginInfo(char* in_Endpoint);

};
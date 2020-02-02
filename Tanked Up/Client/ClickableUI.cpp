#include "ClickableUI.h"

ClickableUI::ClickableUI()
{
	initialOffset = Vector2(0, 0);
	isSelected = false;
	deselectsAfterFrame = false;
}

ClickableUI::ClickableUI(Vector2 position, bool centered)
{
	SetPosition(position, centered);
	initialOffset = position;
	deselectsAfterFrame = false;
}

ClickableUI::~ClickableUI()
{

}

void ClickableUI::OnClick()
{
	std::cout << "ClickableUI::OnClick()::Override function in child class\n";
}

void ClickableUI::GetKeyboardInput(SDL_Event* e)
{
	//std::cout << "ClickableUI::GetKeyboardInput(SDL_Event)::Function needs to be overridden\n";
}

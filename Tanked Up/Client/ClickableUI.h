#pragma once
#include "Thing.h"

class ClickableUI : public Thing
{
public:
	ClickableUI();
	ClickableUI(Vector2 position, bool centered);
	virtual ~ClickableUI();
	virtual void OnClick();
	virtual void GetKeyboardInput(SDL_Event* e);

	bool isSelected, deselectsAfterFrame;
	Vector2 initialOffset;
};
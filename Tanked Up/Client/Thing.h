#pragma once
#include "Renderable.h"


class Thing : public Renderable
{
private:

protected:
	bool m_shouldUpdate = true;
	bool m_isColliding = false;
	
	void updatePosition();
public:
	Thing();
	bool m_ShouldCollisionCheck = true;
	bool m_IsSolid = true;
	Vector2 m_position;
	Vector2 m_positionPrevious;
	virtual ~Thing();
	
	virtual void Update(float deltaTime);
	
	void SetPosition(float x, float y, bool centered = true);
	void SetPosition(Vector2 pos, bool centered = true);
	virtual bool CollideCheck(Thing* other);
	std::string Name = "";
};


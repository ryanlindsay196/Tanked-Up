#include "Thing.h"

Thing::Thing()
{
}


Thing::~Thing()
{
	
}


void Thing::Update(float deltaTime) {
	if (m_shouldUpdate)
	{
		updatePosition();
	}
}

void Thing::updatePosition() {
	if (m_isColliding)
	{
		m_position.X = m_positionPrevious.X;
		m_position.Y = m_positionPrevious.Y;
		m_isColliding = false;
	}
	else
	{
		m_positionPrevious.X = m_position.X;
		m_positionPrevious.Y = m_position.Y;
		if (m_position.X != m_destination.x || m_position.Y != m_destination.y)
		{
			m_destination.x = (int)m_position.X;
			m_destination.y = (int)m_position.Y;

		}
	}
}

void Thing::SetPosition(float x, float y, bool centered) {

	m_position.X = (x - (centered ? m_destination.w * 0.5f : 0));
	m_position.Y = (y - (centered ? m_destination.h * 0.5f : 0));
	
	m_destination.x = (int)m_position.X;
	m_destination.y = (int)m_position.Y;
}
void Thing::SetPosition(Vector2 pos, bool centered) {
	SetPosition(pos.X, pos.Y, centered);
}
bool Thing::CollideCheck(Thing* other) {
	
	if (m_ShouldCollisionCheck && other->m_IsSolid)
	{
		/*int leftSide = (int(m_destination.x - m_destination.w  * 0.49f));
		int rightSide = (int(m_destination.x + m_destination.w * 0.49f));
		int upSide = (int(m_destination.y - m_destination.h * 0.49f));
		int downSide = (int(m_destination.y + m_destination.h * 0.49f));
		if (other->m_destination.x > leftSide && other->m_destination.x < rightSide &&
			other->m_destination.y > upSide && other->m_destination.y < downSide)
		{
			std::cout << Name << " is colliding with " << other->Name << std::endl;
			m_isColliding = true;
			return true;
		}*/

		if (Vector2::GetDistanceLessThan(Vector2(other->m_position.X + other->m_destination.w * 0.5f, other->m_position.Y + other->m_destination.h * 0.5f), Vector2(m_position.X + m_destination.w * 0.5f, m_position.Y  + m_destination.h * 0.5f), m_destination.w * 0.75f))
		{
			//std::cout << Name << " is colliding with " << other->Name << std::endl;
			m_isColliding = true;
			return true;
		}

	}
	m_isColliding = false;
	return false;
}

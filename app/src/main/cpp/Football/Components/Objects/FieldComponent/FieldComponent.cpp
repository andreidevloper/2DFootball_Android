#include "FootballStd.h"
#include "FieldComponent.h"

FieldComponent::FieldComponent(Context* contex_t) : LogicComponent(contex_t)
{
	m_Center = Vector2(0.0f, 0.0f);

}

void FieldComponent::RegisterObject(Context* contex_t)
{
	contex_t->RegisterFactory<FieldComponent>();
}


void FieldComponent::InitializeAllDelegates()
{

}

void FieldComponent::DestroyAllDelegates()
{

}

void FieldComponent::Start()
{
	// Component has been inserted into its scene node. Subscribe to events now
	InitializeAllDelegates();

	m_FieldUpSide = 28.0f;
	m_FieldDownSide = -28.0f;
	m_FieldLeftSide = -45.0f;
	m_FieldRightSide = 45.0f;

	m_GoalWidth = GetHeight() / 3.0f;
	m_GoalAreaHeight = m_GoalWidth * 1.6f;
	m_GoalAreaWidth = m_GoalWidth * 0.75f;
}

bool FieldComponent::IsInside(const Vector3& pos)
{
	return pos.x_ >= m_FieldLeftSide && pos.x_ <= m_FieldRightSide && pos.y_ >= m_FieldDownSide && pos.y_ <= m_FieldUpSide;
}

bool FieldComponent::IsOutSide(const Vector3& pos)
{
	return pos.y_ < m_FieldDownSide || pos.y_ > m_FieldUpSide;
}

bool FieldComponent::IsOutLeft(const Vector3& pos)
{
	return pos.x_ < m_FieldLeftSide;
}

bool FieldComponent::IsOutRight(const Vector3& pos)
{
	return pos.x_ > m_FieldRightSide;
}

bool FieldComponent::IsGoalRight(const Vector3& pos)
{
	return IsOutRight(pos) && (pos.y_ > (m_Center.y_ - m_GoalWidth / 2)) && (pos.y_ < (m_Center.y_ + m_GoalWidth / 2));
}

bool FieldComponent::IsGoalLeft(const Vector3& pos)
{
	return IsOutLeft(pos) && (pos.y_ > (m_Center.y_ - m_GoalWidth / 2)) && (pos.y_ < (m_Center.y_ + m_GoalWidth / 2));
}
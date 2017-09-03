#include "FootballStd.h"
#include "../FieldComponent/FieldComponent.h"
#include "BallComponent.h"

BallComponent::BallComponent(Context* context) : LogicComponent(context)
{
	//m_pDebugRenderer = node_->GetScene()->GetComponent<DebugRenderer>();
	m_Radius = 1.0f;
	m_Color = Color::YELLOW;
	m_Speed = 0;
	m_Slowdown = 1.2f;
	m_Dir = 0;

	InitializeAllDelegates();
}

void BallComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<BallComponent>();
}


void BallComponent::InitializeAllDelegates()
{
	SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(BallComponent, HandlePostRenderUpdate));
}

void BallComponent::DestroyAllDelegates()
{

}

void BallComponent::DelayedStart()
{

	m_pFieldComponent = node_->GetScene()->GetChild("Field")->GetComponent<FieldComponent>();

}

void BallComponent::Draw()
{

}

void BallComponent::Move(float timeStep)
{
	if (m_Speed <= 0.0f)
	{
		return;
	}

	Vector3 position = node_->GetPosition();
	
	bool is_in = m_pFieldComponent->IsInside(position);

	position.x_ += (float)(m_Speed * cos(m_Dir) * timeStep);
	position.y_ += (float)(m_Speed * sin(m_Dir) * timeStep);

	bool will_be_in = m_pFieldComponent->IsInside(position);
	m_Speed = m_Speed < m_Slowdown ? 0 : m_Speed - m_Slowdown;
	

	Vector2 fieldCenter = m_pFieldComponent->GetCenter();
	Vector2 goalArea = m_pFieldComponent->GetGoalArea();

	if (m_pFieldComponent->IsGoalLeft(position))
	{
		position.x_ = m_pFieldComponent->GetLeftSide() - 3.0f;
		node_->SetPosition(position);
		return;
	}

	if (m_pFieldComponent->IsGoalRight(position))
	{
		position.x_ = m_pFieldComponent->GetRightSide() + 3.0f;
		node_->SetPosition(position);
		return;
	}

	if (is_in && !will_be_in)
	{
		if (m_pFieldComponent->IsOutSide(position))
		{
			m_Dir = 2 * PI - m_Dir;
		}

		if (m_pFieldComponent->IsOutRight(position))
		{
			position.x_ = m_pFieldComponent->GetRightSide() - goalArea.x_;
			position.y_ = fieldCenter.y_;
			m_Speed = 0.0f;
		}

		if (m_pFieldComponent->IsOutLeft(position))
		{
			position.x_ = m_pFieldComponent->GetLeftSide() + goalArea.x_;
			position.y_ = fieldCenter.y_;
			m_Speed = 0.0f;
		}
	}

	//if (GLOBAL->m_GameState == GS_FIRST_KICK)
	//{
		//URHO3D_LOGINFO("Ball dir = " + String(m_Dir));
		//URHO3D_LOGINFO("Ball speed = " + String(m_Speed));
	//}

	node_->SetPosition(position);
}

void BallComponent::Update(float timeStep)
{
	//Move(timeStep);
}

void BallComponent::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{
	Draw();
}

void BallComponent::SetSpeed(float s)
{ 
	m_Speed = s; 
}
float BallComponent::GetSpeed()
{ 
	return m_Speed;
}
void BallComponent::SetDir(double d)
{ 
	m_Dir = d;
}
double BallComponent::GetDir()
{ 
	return m_Dir; 
}
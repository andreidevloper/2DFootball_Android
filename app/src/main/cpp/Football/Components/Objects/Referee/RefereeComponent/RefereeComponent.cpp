#include "FootballStd.h"

#include "../../FieldComponent/FieldComponent.h"
#include "../../BallComponent/BallComponent.h"

#include "RefereeComponent.h"

RefereeComponent::RefereeComponent(Context* context) : HumanComponent(context)
{
	m_HumanType = htype::tReferee;
}

void RefereeComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<RefereeComponent>();
}


void RefereeComponent::InitializeAllDelegates()
{

}

void RefereeComponent::DestroyAllDelegates()
{

}

void RefereeComponent::Start()
{
	// Component has been inserted into its scene node. Subscribe to events now
	InitializeAllDelegates();
	Node* ballNode = GetScene()->GetChild("Ball", true);

	m_pBallComponent = ballNode->GetComponent<BallComponent>();

}

void RefereeComponent::VMove(float timeStep)
{
	Vector3 ballCoord = m_pBallComponent->GetNode()->GetPosition();
	Vector3 currentPos = node_->GetPosition();

	m_Dir = Direction(Vector2(currentPos.x_, currentPos.y_), Vector2(ballCoord.x_, ballCoord.y_));
	m_Dir = Disperse(m_Dir, PI / 8);
	if (Distance(Vector2(currentPos.x_, currentPos.y_), Vector2(ballCoord.x_, ballCoord.y_)) < 10.0f) 
	{ 
		m_Speed = 3.0f + Random(3.0f); 
		m_Dir = -m_Dir;
	}
	else
	{
		m_Speed = 3.0f + Random(3.0f);
	}

	currentPos.x_ += m_Speed * cos(m_Dir) * timeStep;
	currentPos.y_ += m_Speed * sin(m_Dir) * timeStep;

	node_->SetPosition(currentPos);
}

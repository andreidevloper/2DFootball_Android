#include "FootballStd.h"

#include "../../FieldComponent/FieldComponent.h"
#include "../../BallComponent/BallComponent.h"

#include "AssRefereeComponent.h"

AssRefereeComponent::AssRefereeComponent(Context* context) : HumanComponent(context)
{
	m_HumanType = htype::tAssReferee;
}

void AssRefereeComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<AssRefereeComponent>();
}


void AssRefereeComponent::InitializeAllDelegates()
{

}

void AssRefereeComponent::DestroyAllDelegates()
{

}

void AssRefereeComponent::Start()
{
	// Component has been inserted into its scene node. Subscribe to events now
	InitializeAllDelegates();
	Node* ballNode = GetScene()->GetChild("Ball", true);

	m_pBallComponent = ballNode->GetComponent<BallComponent>();
}

void AssRefereeComponent::VMove(float timeStep)
{
	Vector3 ballCoord = m_pBallComponent->GetNode()->GetPosition();
	Vector3 currentPos = node_->GetPosition();

	float delta = Random(Abs(ballCoord.x_ - currentPos.x_));
	delta = delta > 5 ? 5 : delta;

	if (currentPos.x_ < ballCoord.x_) currentPos.x_ += m_Speed + delta * timeStep;
	if (currentPos.x_ > ballCoord.x_) currentPos.x_ -= m_Speed + delta * timeStep;

	node_->SetPosition(currentPos);
}
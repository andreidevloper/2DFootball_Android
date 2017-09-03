#include "FootballStd.h"

#include "../../FieldComponent/FieldComponent.h"
#include "../../BallComponent/BallComponent.h"

#include "ForwardComponent.h"

ForwardComponent::ForwardComponent(Context* context) : PlayerComponent(context)
{
	m_Speed = 10.0f;
	m_HumanType = htype::tForward;
}


ForwardComponent::ForwardComponent(TeamNumber teamNumber, int playerNumber, Context* context) : PlayerComponent(teamNumber, playerNumber, context)
{
	m_Speed = 10.0f;
	m_HumanType = htype::tForward;
}

void ForwardComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<ForwardComponent>();
}


void ForwardComponent::InitializeAllDelegates()
{

}

void ForwardComponent::DestroyAllDelegates()
{

}

void ForwardComponent::DelayedStart()
{
	PlayerComponent::DelayedStart();
}

void ForwardComponent::VMove(float timeStep)
{
	PlayerComponent::VMove(timeStep);
}

void ForwardComponent::VKick()
{
	PlayerComponent::VKick();

	//Vector2 fieldCenter = m_pFieldComponent->GetCenter();
	//Vector3 ballCoord = m_pBallComponent->GetNode()->GetPosition();

	//if (m_TeamNumber == TeamNumber::Left)
	//{
	//	fieldCenter.x_ = m_pFieldComponent->GetLeftSide();
	//}
	//else
	//{
	//	fieldCenter.x_ = m_pFieldComponent->GetRightSide();
	//}

	//Vector3 delta;
	//delta.x_ = fieldCenter.x_ - ballCoord.x_;
	//delta.y_ = fieldCenter.y_ - ballCoord.y_;
	//delta.z_ = PLAYER_DEFAULT_Z_COORD;

	//if (delta.x_ == 0.0f)
	//{
	//	m_Dir = (float)PI / 2;
	//}
	//else
	//{
	//	m_Dir = atan(double(delta.y_) / delta.x_);
	//}

	//if (fieldCenter.x_ > ballCoord.x_)
	//{
	//	m_Dir += (float)PI;
	//}

	//m_Dir += 1.0f / (Random(90.0f) + 10.0f);

	//m_pBallComponent->SetDir(m_Dir);
	//m_pBallComponent->SetSpeed(30.0f);

	//auto soundSource = node_->GetOrCreateComponent<SoundSource>();
	//soundSource->Play(CACHE->GetResource<Sound>("Audio/Kick2.wav"));

	
}


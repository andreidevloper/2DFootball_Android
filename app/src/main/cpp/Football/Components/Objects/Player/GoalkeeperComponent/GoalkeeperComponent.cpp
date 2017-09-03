#include "FootballStd.h"

#include "Events/FootballEvents.h"

#include "../../FieldComponent/FieldComponent.h"
#include "../../BallComponent/BallComponent.h"

#include "GoalkeeperComponent.h"

GoalkeeperComponent::GoalkeeperComponent(Context* context) : PlayerComponent(context)
{

	m_HumanType = htype::tGoalkeeper;
}


GoalkeeperComponent::GoalkeeperComponent(TeamNumber teamNumber, int playerNumber, Context* context) : PlayerComponent(teamNumber, playerNumber, context)
{
	m_HumanType = htype::tGoalkeeper;
}

void GoalkeeperComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<GoalkeeperComponent>();
}


void GoalkeeperComponent::InitializeAllDelegates()
{

}

void GoalkeeperComponent::DestroyAllDelegates()
{

}

void GoalkeeperComponent::DelayedStart()
{
	PlayerComponent::DelayedStart();

	Vector2 fieldCenter = m_pFieldComponent->GetCenter();
	Vector2 goalArea = m_pFieldComponent->GetGoalArea();

	if (m_TeamNumber == TeamNumber::Left)
	{
		IntRect fieldArea = m_pFieldComponent->GetArea();
		m_Area = IntRect((int)fieldArea.left_, (int)fieldCenter.y_ + (int)goalArea.y_ / 2, (int)fieldArea.left_ + (int)goalArea.x_, (int)fieldCenter.y_ - (int)goalArea.y_ / 2);
	}
	else
	{
		IntRect fieldArea = m_pFieldComponent->GetArea();
		m_Area = IntRect((int)fieldArea.right_ - (int)goalArea.x_, (int)fieldCenter.y_ + (int)goalArea.y_ / 2, (int)fieldArea.right_ - (int)goalArea.x_, (int)fieldCenter.y_ - (int)goalArea.y_ / 2);
	}
}

void GoalkeeperComponent::VMove(float timeStep)
{
	m_KickDelay += timeStep;
	float close = m_Radius;

	Vector3 ballCoord = m_pBallComponent->GetNode()->GetPosition();
	Vector3 currentPos = node_->GetPosition();

	m_Speed = 4.0f + Random(5);
	m_Dir = Direction(Vector2(currentPos.x_, currentPos.y_), Vector2(ballCoord.x_, ballCoord.y_));
	m_Dir = Disperse(m_Dir, (float)PI / 4.0f);

	currentPos.x_ += m_Speed * cos(m_Dir) * timeStep;
	currentPos.y_ += m_Speed * sin(m_Dir) * timeStep;

	if (m_TeamNumber == TeamNumber::Right && currentPos.x_ < m_Area.left_ + 0.5f)
	{
		currentPos.x_ = m_Area.left_ + 0.5f; //+ m_Radius / 2.0f;
	}
	
	if (m_TeamNumber == TeamNumber::Left && currentPos.x_ > m_Area.right_ - 0.5f)
	{
		currentPos.x_ = m_Area.right_ - 0.5f; //- m_Radius / 2.0f;
	}
	
	if (currentPos.y_ > m_Area.top_)
	{
		currentPos.y_ = (float)m_Area.top_; //- m_Radius / 2.0f;
	}
	
	if (currentPos.y_ < m_Area.bottom_)
		currentPos.y_ = (float)m_Area.bottom_; //+ m_Radius / 2.0f;

	node_->SetPosition(currentPos);

	AvoidCollision(timeStep);

	if (Distance(Vector2(currentPos.x_, currentPos.y_), Vector2(ballCoord.x_, ballCoord.y_)) < close && m_KickDelay >= 0.1f)
	{
		VKick();
		m_KickDelay = 0.0f;
	}
}

void GoalkeeperComponent::VKick()
{
	Vector2 fieldCenter = m_pFieldComponent->GetCenter();
	Vector3 ballCoord = m_pBallComponent->GetNode()->GetPosition();

	Vector3 currentPos = node_->GetPosition();
	float dirBall;

	if (m_TeamNumber == TeamNumber::Left)
	{
		fieldCenter.x_ = m_pFieldComponent->GetRightSide();
	}
	else
	{
		fieldCenter.x_ = m_pFieldComponent->GetLeftSide();
	}

	dirBall = Direction(Vector2(ballCoord.x_, ballCoord.y_), Vector2(fieldCenter.x_, fieldCenter.y_));
	dirBall = Disperse(dirBall, (float)PI / 4.0f);
		
	m_pBallComponent->SetDir(dirBall);
	m_pBallComponent->SetSpeed(80.0f);

	auto soundSource = node_->GetOrCreateComponent<SoundSource>();
	soundSource->Play(g_pApp->GetResCache()->GetResource<Sound>("Audio/Kick3.wav"));

    VariantMap data;
    data[COMMENT::P_COMMENT] = String("Team = " + String((int)m_TeamNumber) + " Number = " + String(m_Number));
    SendEvent(E_COMMENT, data);

}


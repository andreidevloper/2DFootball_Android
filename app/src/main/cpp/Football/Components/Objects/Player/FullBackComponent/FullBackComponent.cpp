#include "FootballStd.h"

#include "Events/FootballEvents.h"
#include "../../FieldComponent/FieldComponent.h"
#include "../../BallComponent/BallComponent.h"

#include "FullBackComponent.h"

FullBackComponent::FullBackComponent(Context* context) : PlayerComponent(context)
{
	m_Speed = 5.0f;
	m_HumanType = htype::tFullBack;
}


FullBackComponent::FullBackComponent(TeamNumber teamNumber, int playerNumber, Context* context) : PlayerComponent(teamNumber, playerNumber, context)
{
	m_Speed = 5.0f;
	m_HumanType = htype::tFullBack;
}

void FullBackComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<FullBackComponent>();
}


void FullBackComponent::InitializeAllDelegates()
{

}

void FullBackComponent::DestroyAllDelegates()
{

}

void FullBackComponent::DelayedStart()
{
	PlayerComponent::DelayedStart();

	Vector2 fieldCenter = m_pFieldComponent->GetCenter();
	if (m_TeamNumber == TeamNumber::Left)
	{
		IntRect fieldArea = m_pFieldComponent->GetArea();
		m_Area = IntRect((int)fieldArea.left_, (int)fieldArea.top_, (int)fieldCenter.x_, (int)fieldArea.bottom_);
	}
	else
	{
		IntRect fieldArea = m_pFieldComponent->GetArea();
		m_Area = IntRect((int)fieldCenter.x_, (int)fieldArea.top_, (int)fieldArea.right_, (int)fieldArea.bottom_);
	}
}

void FullBackComponent::VMove(float timeStep)
{
	if (!m_bIsInGame)
	{
		MoveToBench();
		return;
	}

	float close = 2 * m_Radius;

	Vector2 fieldCenter = m_pFieldComponent->GetCenter();
	Vector3 ballCoord = m_pBallComponent->GetNode()->GetPosition();

	Vector3 delta;
	delta.x_ = ballCoord.x_;
	delta.y_ = ballCoord.y_;
	delta.z_ = PLAYER_DEFAULT_Z_COORD;

	Vector3 currentPos = node_->GetPosition();

	m_Energy = m_Energy > 0 ? m_Energy - timeStep : 0;

	m_Speed = 3.0f + Random(m_Energy / 1000);

	if (m_TeamNumber == TeamNumber::Left && ballCoord.x_ > fieldCenter.x_) 
	{ 
		float center = m_pFieldComponent->GetWidth() / 2.0f;
		delta.x_ = Disperse(fieldCenter.x_ - center / 2.0f, center);
		delta.y_ = Random(m_pFieldComponent->GetDownSide(), m_pFieldComponent->GetUpSide());
	}

	if (m_TeamNumber == TeamNumber::Right && ballCoord.x_ < fieldCenter.x_)
	{
		float center = m_pFieldComponent->GetWidth() / 2.0f;
		delta.x_ = Disperse(fieldCenter.x_ + center / 2.0f, center);
		delta.y_ = Random(m_pFieldComponent->GetDownSide(), m_pFieldComponent->GetUpSide());
	}

	m_Dir = Direction(Vector2(currentPos.x_, currentPos.y_), Vector2(delta.x_, delta.y_));
	//m_Dir = Disperse(m_Dir, (float)PI / 6.0f);

	currentPos.x_ += m_Speed * cos(m_Dir) * timeStep;
	currentPos.y_ += m_Speed * sin(m_Dir) * timeStep;

	if (m_TeamNumber == TeamNumber::Right && currentPos.x_ < m_Area.left_)
	{
		currentPos.x_ = m_Area.left_ + 2 * m_Radius + 1.0f;
	}

	if (m_TeamNumber == TeamNumber::Left && currentPos.x_ > m_Area.right_)
	{
		currentPos.x_ = m_Area.right_ - 2 * m_Radius;
	}
		
	node_->SetPosition(currentPos);

	AvoidCollision(timeStep);

	if (Distance(Vector2(currentPos.x_, currentPos.y_), Vector2(ballCoord.x_, ballCoord.y_)) < close)
	{
		VKick();
	}
}

void FullBackComponent::VKick()
{
	Vector2 fieldCenter = m_pFieldComponent->GetCenter();
	Vector3 ballCoord = m_pBallComponent->GetNode()->GetPosition();

	if (m_TeamNumber == TeamNumber::Left)
	{
		fieldCenter.x_ = m_pFieldComponent->GetRightSide();
	}
	else
	{
		fieldCenter.x_ = m_pFieldComponent->GetLeftSide();
	}

	float dirBall = Direction(Vector2(ballCoord.x_, ballCoord.y_), Vector2(fieldCenter.x_, fieldCenter.y_));
	dirBall = Disperse(dirBall, (float)PI / 7.0);
	m_pBallComponent->SetDir(dirBall);
	m_pBallComponent->SetSpeed(Disperse(10.0f, 20.0f) + 60.0f);
	
	auto soundSource = node_->GetOrCreateComponent<SoundSource>();
	soundSource->Play(g_pApp->GetResCache()->GetResource<Sound>("Audio/Kick1.wav"));

    VariantMap data;
    data[COMMENT::P_COMMENT] = String("Team = " + String((int)m_TeamNumber) + " Number = " + String(m_Number));
    SendEvent(E_COMMENT, data);
}


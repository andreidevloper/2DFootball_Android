#include "FootballStd.h"

#include "Events/FootballEvents.h"
#include "../../FieldComponent/FieldComponent.h"
#include "../../BallComponent/BallComponent.h"

#include "HalfBackComponent.h"

HalfBackComponent::HalfBackComponent(Context* context) : PlayerComponent(context)
{
	m_Speed = 7.0f;
	m_HumanType = htype::tHalfBack;
}


HalfBackComponent::HalfBackComponent(TeamNumber teamNumber, int playerNumber, Context* context) : PlayerComponent(teamNumber, playerNumber, context)
{
	m_Speed = 7.0f;
	m_HumanType = htype::tHalfBack;
}

void HalfBackComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<HalfBackComponent>();
}

void HalfBackComponent::VKick()
{
	Vector2 fieldCenter = m_pFieldComponent->GetCenter();
	Vector3 ballCoord = m_pBallComponent->GetNode()->GetPosition();

	Vector3 delta;
	delta.y_ = fieldCenter.y_;
	delta.z_ = PLAYER_DEFAULT_Z_COORD;

	Vector3 currentPos = node_->GetPosition();
	float speedBall = 0.0f;
	float dirBall = 0.0f;
	float dirDisperse = (float)PI / 8;
	float kickSpeed = 40.0f;

	if (m_TeamNumber == TeamNumber::Left)
	{
		delta.x_ = m_pFieldComponent->GetRightSide();
	}
	else
	{
		delta.x_ = m_pFieldComponent->GetLeftSide();
	}
	delta.y_ = ballCoord.y_;


	if (Distance(Vector2(ballCoord.x_, ballCoord.y_), Vector2(delta.x_, delta.y_)) <= 44.0f)
	{
		delta.y_ = fieldCenter.y_;
		dirDisperse = 0;
	}

	dirBall = Direction(Vector2(ballCoord.x_, ballCoord.y_), Vector2(delta.x_, delta.y_));
	dirBall = Disperse(dirBall, dirDisperse);
	m_pBallComponent->SetDir(dirBall);
	m_pBallComponent->SetSpeed(Disperse(kickSpeed, kickSpeed / 2) + 10.0f);
	
	auto soundSource = node_->GetOrCreateComponent<SoundSource>();
	soundSource->Play(g_pApp->GetResCache()->GetResource<Sound>("Audio/Kick2.wav"));


    VariantMap data;
    data[COMMENT::P_COMMENT] = String("Team = " + String((int)m_TeamNumber) + " Number = " + String(m_Number));
    SendEvent(E_COMMENT, data);
}


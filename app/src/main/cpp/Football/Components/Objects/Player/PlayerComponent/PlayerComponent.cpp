#include "FootballStd.h"
#include "Events/FootballEvents.h"
#include "../../FieldComponent/FieldComponent.h"
#include "../../BallComponent/BallComponent.h"
#include "GameLogic/FootballLogic.h"

PlayerComponent::PlayerComponent(Context* context) : HumanComponent(context)
{
    m_pFootballLogic = (FootballLogic*)g_pApp->GetGameLogic();
	m_TeamNumber = TeamNumber::Invalid;
	m_Number = 0;
	m_bIsInGame = true;
	m_HumanType = htype::tForward;
	m_pTeamContainer = NULL;
	m_Area = IntRect();
	m_KickDelay = 0.0f;
}

PlayerComponent::PlayerComponent(TeamNumber teamNumber, int playerNumber, Context* context) : HumanComponent(context)
{
    m_pFootballLogic = (FootballLogic*)g_pApp->GetGameLogic();
	m_TeamNumber = teamNumber;
	m_Number = playerNumber;
	m_bIsInGame = true;
	m_HumanType = htype::tForward;
	m_pTeamContainer = NULL;
	m_Area = IntRect();
	m_KickDelay = 0.0f;
}

void PlayerComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<PlayerComponent>();
}

void PlayerComponent::DelayedStart()
{
	Node* fieldNode = GetScene()->GetChild("Field", true);
	Node* ballNode = GetScene()->GetChild("Ball", true);

	m_pBallComponent = ballNode->GetComponent<BallComponent>();
	m_pFieldComponent = fieldNode->GetComponent<FieldComponent>();

	m_Area = m_pFieldComponent->GetArea();
}


void PlayerComponent::VDraw(float tiemStep)
{

}

void PlayerComponent::VMove(float timeStep)
{
	m_KickDelay += timeStep;
	if (!m_bIsInGame)
	{
		MoveToBench();
		return;
	}

	float close = m_Radius / 1.5f;

	Vector2 fieldCenter = m_pFieldComponent->GetCenter();
	Vector3 ballCoord = m_pBallComponent->GetNode()->GetPosition();

	Vector3 delta;
	delta.x_ = ballCoord.x_;
	delta.y_ = ballCoord.y_;
	delta.z_ = PLAYER_DEFAULT_Z_COORD;

	Vector3 currentPos = node_->GetPosition();

	bool ownSide = (m_TeamNumber == TeamNumber::Left && currentPos.x_ < fieldCenter.x_) || (m_TeamNumber == TeamNumber::Right && currentPos.x_ > fieldCenter.x_);

	if (ownSide && Distance(Vector2(currentPos.x_, currentPos.y_), Vector2(ballCoord.x_, ballCoord.y_)) > 30.0f )
	{
		delta.y_ = Disperse(delta.y_, 50);
	}
	
	m_Energy = m_Energy > 0 ? m_Energy - timeStep : 0;

	m_Speed = 12.5f + Random(m_Energy / 1000);
	m_Dir = Direction(Vector2(currentPos.x_, currentPos.y_), Vector2(delta.x_, delta.y_));
	m_Dir = Disperse(m_Dir, (float) PI / 2.0f);

	currentPos.x_ += m_Speed * cos(m_Dir) * timeStep;
	currentPos.y_ += m_Speed * sin(m_Dir) * timeStep;

	node_->SetPosition(currentPos);

	AvoidCollision(timeStep);

	if (Distance(Vector2(currentPos.x_, currentPos.y_), Vector2(ballCoord.x_, ballCoord.y_)) < close && m_KickDelay >= 0.1f)
	{
		VKick();
		m_KickDelay = 0.0f;
	}
}


void PlayerComponent::VKick()
{
	Vector2 fieldCenter = m_pFieldComponent->GetCenter();
	Vector3 ballCoord = m_pBallComponent->GetNode()->GetPosition();

	Vector3 delta;
	delta.y_ = fieldCenter.y_;
	delta.z_ = PLAYER_DEFAULT_Z_COORD;

	Vector3 currentPos = node_->GetPosition();
	float speedBall;

	if (m_TeamNumber == TeamNumber::Left)
	{
		delta.x_ = m_pFieldComponent->GetRightSide();
	}
	else
	{
		delta.x_ = m_pFieldComponent->GetLeftSide();
	}

	speedBall = Disperse(60.0f, 20.0f);

	bool ownSide = (m_TeamNumber == TeamNumber::Left && currentPos.x_ < fieldCenter.x_) || (m_TeamNumber == TeamNumber::Right && currentPos.x_ > fieldCenter.x_);

	Vector3 partnerPos;
	bool result = Partner(partnerPos);

	if (result)
	{  
		float speedPartner = sqrt(2 * Distance(Vector2(currentPos.x_, currentPos.y_), Vector2(partnerPos.x_, partnerPos.y_)) + 20.25f) - 0.5f;

	/*	if (ownSide) 
		{
			delta.x_ = partnerPos.x_;  
			delta.y_ = partnerPos.y_; 
			speedBall = speedPartner;
		}
		else 
		{*/
			if ((m_TeamNumber == TeamNumber::Left && partnerPos.x_ > (currentPos.x_ + 2.0f)) || (m_TeamNumber == TeamNumber::Right && partnerPos.x_ < (currentPos.x_ - 2.0f)))
			{
				delta.x_ = partnerPos.x_;
				delta.y_ = partnerPos.y_;
				speedBall = speedPartner + Disperse(30, 15) + 30.0f;
				//URHO3D_LOGINFO("Pass speed = " + String(speedBall));
			}
		//}



		if (m_pFootballLogic->GetGameState() == GS_FIRST_KICK)
		{
			delta.x_ = partnerPos.x_;
			delta.y_ = partnerPos.y_;
			speedBall = speedPartner + Disperse(20, 20.0f) + 40.0f;
			m_pFootballLogic->SetNeededGameState(GS_GAMEPLAY);
		}
	} 

	float dirBall;
	dirBall = Direction(Vector2(ballCoord.x_, ballCoord.y_), Vector2(delta.x_, delta.y_));
	
	dirBall = Disperse(dirBall, (float)PI / 20.0f);
	/*if (GLOBAL->m_GameState == GS_FIRST_KICK)
	{
		URHO3D_LOGINFO("dirball = " + String(dirBall));
		URHO3D_LOGINFO("speedball = " + String(speedBall));
	}*/

	m_pBallComponent->SetDir(dirBall);
	SetDir(dirBall);

	m_pBallComponent->SetSpeed(speedBall);

	//if (GLOBAL->m_GameState == GS_FIRST_KICK)
	//{
	//	Vector3 newPos = m_pBallComponent->GetNode()->GetPosition();
	//	newPos.x_ += (float)(speedBall * cos(dirBall) * 0.02f);
	//	newPos.y_ += (float)(speedBall * sin(dirBall) * 0.02f);
	//	m_pBallComponent->GetNode()->SetPosition(newPos);
	//}

	auto soundSource = node_->GetOrCreateComponent<SoundSource>();
	soundSource->Play(g_pApp->GetResCache()->GetResource<Sound>("Audio/Kick1.wav"));

    VariantMap data;
    data[COMMENT::P_COMMENT] = String("Team = " + String((int)m_TeamNumber) + " Number = " + String(m_Number));
    SendEvent(E_COMMENT, data);
}

void PlayerComponent::AvoidCollision(float timeStep)
{
	float lim = m_Radius;

	const Vector<Node*>& team1 = m_pFootballLogic->GetFirstTeam();
	const Vector<Node*>& team2 = m_pFootballLogic->GetSecondTeam();
	const Vector<Node*>& referees = m_pFootballLogic->GetReferees();

	int nPos = team1.Size() + team2.Size();

	Vector3* occupiedPos = new Vector3[nPos];

	int j = 0;
	for (unsigned int i = 0; i < team1.Size(); i++)
	{
		if (team1[i]->GetName() == node_->GetName())
		{
			continue;
		}

		PlayerComponent* playerComponent = (PlayerComponent*)team1[i]->GetComponent(team1[i]->GetVar("playerType").GetStringHash());
		if (!playerComponent->IsInGame())
		{
			continue;
		}

		Vector3 pos = team1[i]->GetPosition();
		occupiedPos[j] = pos;
		++j;
	}

	for (unsigned int i = 0; i < team2.Size(); i++)
	{
		if (team2[i]->GetName() == node_->GetName())
		{
			continue;
		}

		PlayerComponent* playerComponent = (PlayerComponent*)team2[i]->GetComponent(team2[i]->GetVar("playerType").GetStringHash());
		if (!playerComponent->IsInGame())
		{
			continue;
		}

		Vector3 pos = team2[i]->GetPosition();
		occupiedPos[j] = pos;
		++j;
	}

	occupiedPos[j] = referees[0]->GetPosition();
	
	int attempts = 10000;
	bool tooClose = false;

	Vector3 currentPos = node_->GetPosition();
	Vector3 avoid = currentPos; 
	float speedAvoid = m_Speed;
	float dirAvoid = m_Dir;
	float dist = 0.0f;

	nPos = j + 1;
	do
	{
		for (int i = 0; i < nPos; i++)
		{
			tooClose = Distance(Vector2(avoid.x_, avoid.y_), Vector2(occupiedPos[i].x_, occupiedPos[i].y_)) < lim;
			dist = Distance(Vector2(avoid.x_, avoid.y_), Vector2(occupiedPos[i].x_, occupiedPos[i].y_));
			if (tooClose)
			{
				--attempts;
				speedAvoid = speedAvoid + dist - lim;
				//disperse(m_Speed, lim);
				dirAvoid = Disperse(m_Dir, (float)PI / 18.0f);
				avoid.x_ = currentPos.x_ + speedAvoid * (float)cos(dirAvoid); //* (timeStep + 0.01f);
				avoid.y_ = currentPos.y_ + speedAvoid * (float)sin(dirAvoid); //* (timeStep + 0.01f);
				break;
			}
		}
	} 
	while (tooClose && attempts > 0);
	
	node_->SetPosition(avoid);
	
	m_Speed = speedAvoid;
	m_Dir = dirAvoid;
	delete[] occupiedPos;
}

void PlayerComponent::MoveToBench()
{
	Vector2 fieldCenter = m_pFieldComponent->GetCenter();
	if (m_TeamNumber == TeamNumber::Left)
	{
		node_->SetPosition(Vector3(fieldCenter.x_ - m_Number * 3 * m_Radius, m_pFieldComponent->GetUpSide() + m_Radius, PLAYER_DEFAULT_Z_COORD));
	}
	else
	{
		node_->SetPosition(Vector3(fieldCenter.x_ + m_Number * 3 * m_Radius, m_pFieldComponent->GetUpSide() + m_Radius, PLAYER_DEFAULT_Z_COORD));
	}
}

bool PlayerComponent::Partner(Vector3& partnerPos)
{
	//int dist[7];
	Vector3 p[6];
	int n = 0;

	Vector<Node*>& teamContainer = (*m_pTeamContainer);
	for (unsigned int i = 0; i < teamContainer.Size(); i++)
	{
		if (teamContainer[i]->GetName() == node_->GetName())
		{
			continue;
		}

		PlayerComponent* playerComponent = (PlayerComponent*)teamContainer[i]->GetComponent(teamContainer[i]->GetVar("playerType").GetStringHash());
		if (!playerComponent->IsInGame())
		{
			continue;
		}

		if (playerComponent->GetHumanType() == tGoalkeeper)
		{
			continue;
		}

		Vector3 pos = teamContainer[i]->GetPosition();
		p[n] = pos;
		++n;
	}


	if (n == 0) // no partners
	{ 
		return false;
	} 

	Vector3 currentPos = node_->GetPosition();
	float d = 0.0f;
	float mindist = Distance(Vector2(currentPos.x_, currentPos.y_), Vector2(p[0].x_, p[0].y_));
	int np = 0;
	for (int i = 0; i < n; i++)
	{
		d = Distance(Vector2(currentPos.x_, currentPos.y_), Vector2(p[i].x_, p[i].y_));
		if (d < mindist)
		{
			mindist = d;
			np = i;
		}
	}

	partnerPos = p[np];
	return true;
}


#include "FootballStd.h"

#include "GameView/FootballView.h"

#include "Components/Objects/BallComponent/BallComponent.h"
#include "Components/Objects/CameraComponent/CameraComponent.h"
#include "Components/Objects/FieldComponent/FieldComponent.h"
#include "Components/Objects/HumanComponent/HumanComponent.h"

#include "Components/Objects/Player/ForwardComponent/ForwardComponent.h"
#include "Components/Objects/Player/FullBackComponent/FullBackComponent.h"
#include "Components/Objects/Player/GoalkeeperComponent/GoalkeeperComponent.h"
#include "Components/Objects/Player/HalfBackComponent/HalfBackComponent.h"
#include "Components/Objects/Player/PlayerComponent/PlayerComponent.h"
#include "Components/Objects/Player/PlayerFactory/PlayerFactory.h"

#include "Components/Objects/Referee/AssRefereeComponent/AssRefereeComponent.h"
#include "Components/Objects/Referee/RefereeComponent/RefereeComponent.h"

#include "Events/FootballEvents.h"

#include "FootballLogic.h"

// For sleep
#include <unistd.h>
#include <Events/FootballEvents.h>

FootballLogic::FootballLogic(Context *context) : BaseGameLogic(context),
                                                 m_LeftTeamEnergy(0.0f),
                                                 m_RightTeamEnergy(0.0f),
                                                 m_Time(0.0f),
                                                 m_LeftTeamScore(0),
                                                 m_RightTeamScore(0),
                                                 m_bIsScoreTextDirty(true),
                                                 m_bIsTimeTextDirty(true),
                                                 m_bIsEnergyTextDirty(true),
                                                 m_bIsCommentTextDirty(true),
                                                 m_CommentText(""),
                                                 m_bIsGameStarted(false),
                                                 m_SimulationSlowDownTime(0.0f),
                                                 m_ShowTime(SHOW_TIME),
                                                 m_bIsShowEnabled(true),
                                                 m_GameState(GS_INTRO),
                                                 m_NeededGameState(GS_INTRO)
{
    SetRandomSeed(Time::GetSystemTime());
    m_pPlayerFactory = new PlayerFactory(context_);
}

FootballLogic::~FootballLogic()
{

}

bool FootballLogic::VInitialize()
{
	if (!BaseGameLogic::VInitialize())
	{
		return false;
	}

	g_pApp->GetGameOptions().SetSceneName("Scenes/2D_Training_Field.xml");

    SubscribeToEvent(E_STOP_SIMULATION, URHO3D_HANDLER(FootballLogic, HandleStopSimulationDelegate));
    SubscribeToEvent(E_INTRO_SIMULATION, URHO3D_HANDLER(FootballLogic, HandleIntroSimulationDelegate));
    SubscribeToEvent(E_FIRST_KICK, URHO3D_HANDLER(FootballLogic, HandleFirstKickDelegate));
    SubscribeToEvent(E_COMMENT, URHO3D_HANDLER(FootballLogic, HandleCommentDelegate));

    SubscribeToEvent(E_PLAYER_CHECKBOX_PRESSED, URHO3D_HANDLER(FootballLogic, HandlePlayerCheckBoxDelegate));
    SubscribeToEvent(E_SIMULATION_SLIDER_CHANGE, URHO3D_HANDLER(FootballLogic, HandleSimulationValueDelegate));
    SubscribeToEvent(E_ENERGY_VALUE_CHANGE, URHO3D_HANDLER(FootballLogic, HandleEnergyValueDelegate));

	return true;
}

void FootballLogic::VShutdown()
{
	BaseGameLogic::VShutdown();
	SAFE_DELETE(m_pPlayerFactory);
}

void FootballLogic::VChangeState(BaseGameState newState)
{
	BaseGameLogic::VChangeState(newState);

	switch (newState)
	{
		case BGS_WaitingForPlayers:
		{
			// spawn all local players (should only be one, though we might support more in the future)
			assert(m_ExpectedPlayers == 1);
			for (int i = 0; i < m_ExpectedPlayers; i++)
			{
				SharedPtr<IGameView> playerView(new FootballView(context_, g_pApp->GetRenderer()));
				VAddView(playerView);
			}
			break;
		}

		case BGS_SpawningPlayersActors:
		{
			for (auto it = m_GameViews.Begin(); it != m_GameViews.End(); ++it)
			{
				SharedPtr<IGameView> pView = *it;
				if (pView->VGetType() == GameView_Human)
				{
				
				}
			}
			break;
		}
	}

}

void FootballLogic::VAddView(SharedPtr<IGameView> pView, NodeId nodeId)
{
	BaseGameLogic::VAddView(pView, nodeId);

	SharedPtr<FootballView> view = DynamicCast<FootballView>(pView);
	if (view.NotNull())
	{
		// New local human is attached to the game
		m_HumanPlayersAttached++;
	}
}

void FootballLogic::VInitializeComponents()
{
    BallComponent::RegisterObject(context_);
    FieldComponent::RegisterObject(context_);
    HumanComponent::RegisterObject(context_);
    PlayerFactory::RegisterObjects(context_);

    ForwardComponent::RegisterObject(context_);
    FullBackComponent::RegisterObject(context_);
    GoalkeeperComponent::RegisterObject(context_);
    HalfBackComponent::RegisterObject(context_);
    PlayerComponent::RegisterObject(context_);

    AssRefereeComponent::RegisterObject(context_);
    RefereeComponent::RegisterObject(context_);

    CameraComponent::RegisterObject(context_);
}

void FootballLogic::VPreUpdate(float timeStep)
{
    if(m_State != BGS_Running)
        return;

    if (m_GameState == m_NeededGameState)
        return;

    if (m_NeededGameState == GS_STOP)
    {
        SendEvent(E_STOP_SIMULATION);
    }
    else if (m_NeededGameState == GS_INTRO)
    {
        SendEvent(E_INTRO_SIMULATION);
    }
    else if (m_NeededGameState == GS_FIRST_KICK)
    {
        SendEvent(E_FIRST_KICK);
    }
    else if (m_NeededGameState == GS_RESTART_GAME)
    {
        SendEvent(E_RESTART_SIMULATION);
    }

    m_GameState = m_NeededGameState;
}

void FootballLogic::VOnUpdate(float timeStep)
{
    BaseGameLogic::VOnUpdate(timeStep);

    if(m_State == BGS_Running)
    {
        switch (m_GameState)
        {
            case GS_GOAL:
            {
                m_NeededGameState = GS_RESTART_GAME;
                return;
            }

            case GS_RESTART_GAME:
            {
                m_ShowTime -= timeStep;
                if (!m_bIsShowEnabled)
                {
                    m_NeededGameState = GS_FIRST_KICK;
                }
                else if (m_ShowTime <= 0.0f)
                {
                    m_NeededGameState = GS_FIRST_KICK;
                    m_ShowTime = SHOW_TIME;
                }
                return;
            }
        }

        if (CheckLeftGoal())
        {
            m_NeededGameState = GS_GOAL;
        }

        if (CheckRightGoal())
        {
            m_NeededGameState = GS_GOAL;
        }

        if ((m_GameState == GS_GAMEPLAY || m_GameState == GS_FIRST_KICK) && m_GameState != GS_RESTART_GAME && m_bIsGameStarted)
        {
            IncreaseTime(timeStep);
            MoveAll(timeStep);
        }
    }
}

bool FootballLogic::VLoadGameDelegate(String pLevelData)
{
    CreateField();
    CreateBall();
    CreateTeams();

    SetPosition0();

	return true;
}

void FootballLogic::CreateField()
{
    Node* fieldNode = m_pScene->CreateChild("Field");
    fieldNode->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    fieldNode->SetRotation(Quaternion(-90.0f, 0.0f, 0.0f));
    fieldNode->SetScale(Vector3(100.0f, 80.0f, 64.0f));
    StaticModel* model = fieldNode->CreateComponent<StaticModel>();
    model->SetModel(g_pApp->GetResCache()->GetResource<Model>("Models/Plane.mdl"));
    model->SetMaterial(g_pApp->GetResCache()->GetResource<Material>("Materials/Field.xml"));
    fieldNode->CreateComponent<FieldComponent>();
}

void FootballLogic::CreateBall()
{
    m_pBall = m_pScene->CreateChild("Ball");
    m_pBall->SetPosition(Vector3(0.0f, 0.0f, PLAYER_DEFAULT_Z_COORD));
    m_pBall->SetScale(Vector3(1.0f, 1.0f, 1.0f));
    m_pBall->SetRotation(Quaternion::IDENTITY);
    StaticModel* model = m_pBall->CreateComponent<StaticModel>();
    model->SetModel(g_pApp->GetResCache()->GetResource<Model>("Models/Sphere.mdl"));
    model->SetMaterial(g_pApp->GetResCache()->GetResource<Material>("Materials/Ball.xml"));
    m_pBall->CreateComponent<BallComponent>();
}

void FootballLogic::CreateTeams()
{
    int n = 0;

    // Создаем первую комманду
    Node* newPlayer;
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tForward, "Left_Player_1", TeamNumber::Left, ++n, &m_TeamFirst);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tForward, "Left_Player_2", TeamNumber::Left, ++n, &m_TeamFirst);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tForward, "Left_Player_3", TeamNumber::Left, ++n, &m_TeamFirst);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tHalfBack, "Left_Player_4", TeamNumber::Left, ++n, &m_TeamFirst);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tFullBack, "Left_Player_5", TeamNumber::Left, ++n, &m_TeamFirst);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tFullBack, "Left_Player_6", TeamNumber::Left, ++n, &m_TeamFirst);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tGoalkeeper, "Left_Player_7", TeamNumber::Left, ++n, &m_TeamFirst);

    n = 0;
    // Создаем вторую комманду
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tForward, "Right_Player_1", TeamNumber::Right, ++n, &m_TeamSecond);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tForward, "Right_Player_2", TeamNumber::Right, ++n, &m_TeamSecond);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tForward, "Right_Player_3", TeamNumber::Right, ++n, &m_TeamSecond);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tHalfBack, "Right_Player_4", TeamNumber::Right, ++n, &m_TeamSecond);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tFullBack, "Right_Player_5", TeamNumber::Right, ++n, &m_TeamSecond);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tFullBack, "Right_Player_6", TeamNumber::Right, ++n, &m_TeamSecond);
    newPlayer = m_pPlayerFactory->CreatePlayer(m_pScene, htype::tGoalkeeper, "Right_Player_7", TeamNumber::Right, ++n, &m_TeamSecond);

    Node* newReferee;
    newReferee = m_pPlayerFactory->CreateReferee(m_pScene, htype::tReferee, "Referee_1", &m_Referee);
    newReferee = m_pPlayerFactory->CreateReferee(m_pScene, htype::tAssReferee, "Ass_Referee_1", &m_AssReferee);
    newReferee = m_pPlayerFactory->CreateReferee(m_pScene, htype::tAssReferee, "Ass_Referee_2", &m_AssReferee);

    SetPosition0();
}


void FootballLogic::StartGame()
{
    SetScore(TeamNumber::Left, 0);
    SetScore(TeamNumber::Right, 0);
    SetLeftTeamEnergy(0);
    SetRightTeamEnergy(0);
    SetTime(0.0f);
    SetCommentText(" ");

    Node* field = m_pScene->GetChild("Field");
    FieldComponent* fieldComponent = field->GetComponent<FieldComponent>();
    Vector2 fieldCenter = fieldComponent->GetCenter();

    Node* ball = m_pScene->GetChild("Ball");
    BallComponent* ballComponent = ball->GetComponent<BallComponent>();
    ball->SetPosition(fieldCenter);
    ballComponent->SetSpeed(0.0f);

    m_AssReferee[0]->SetPosition(Vector3(fieldCenter.x_, fieldComponent->GetUpSide() + 1.0f, PLAYER_DEFAULT_Z_COORD));
    m_AssReferee[1]->SetPosition(Vector3(fieldCenter.x_, fieldComponent->GetDownSide() - 1.0f, PLAYER_DEFAULT_Z_COORD));

    if (Random(2))
    {
        SetPosition1();
    }
    else
    {
        SetPosition2();
    }

    for (auto it = m_TeamFirst.Begin(); it != m_TeamFirst.End(); it++)
    {
        PlayerComponent* playerComponent = (PlayerComponent*)(*it)->GetComponent((*it)->GetVar("playerType").GetStringHash());
        playerComponent->SetEnergy(Random(2000.0f) + 2000.0f);
    }

    for (auto it = m_TeamSecond.Begin(); it != m_TeamSecond.End(); it++)
    {
        PlayerComponent* playerComponent = (PlayerComponent*)(*it)->GetComponent((*it)->GetVar("playerType").GetStringHash());
        playerComponent->SetEnergy(Random(2000.0f) + 2000.0f);
    }

    auto soundSource = m_Referee[0]->GetOrCreateComponent<SoundSource>();
    soundSource->Play(g_pApp->GetResCache()->GetResource<Sound>("Audio/whistle.wav"));

    m_bIsGameStarted = true;
}

void FootballLogic::SetPosition0()
{
    Node* ball = m_pScene->GetChild("Ball");
    Node* field = m_pScene->GetChild("Field");
    FieldComponent* fieldComponent = field->GetComponent<FieldComponent>();
    Vector2 fieldCenter = fieldComponent->GetCenter();

    ball->SetPosition(fieldCenter);
    float y;

    y = fieldCenter.y_ + 3 * m_TeamFirst.Size() / 2;
    for (unsigned int i = 0; i < m_TeamFirst.Size(); i++)
    {
        m_TeamFirst[i]->SetPosition(Vector3(fieldCenter.x_ - 5, y, PLAYER_DEFAULT_Z_COORD));
        y -= 3;
    }

    y = fieldCenter.y_ + 3 * m_TeamFirst.Size() / 2;
    for (unsigned int i = 0; i < m_TeamFirst.Size(); i++)
    {
        m_TeamSecond[i]->SetPosition(Vector3(fieldCenter.x_ + 5, y, PLAYER_DEFAULT_Z_COORD));
        y -= 3;
    }

    m_Referee[0]->SetPosition(Vector3(0.0f, fieldCenter.y_ + 10, PLAYER_DEFAULT_Z_COORD));

    m_AssReferee[0]->SetPosition(Vector3(fieldCenter.x_ + 3, fieldCenter.y_ + 12, PLAYER_DEFAULT_Z_COORD));
    m_AssReferee[1]->SetPosition(Vector3(fieldCenter.x_ - 3, fieldCenter.y_ + 12, PLAYER_DEFAULT_Z_COORD));
}

void FootballLogic::SetPosition1()
{
    Node* field = m_pScene->GetChild("Field");
    FieldComponent* fieldComponent = field->GetComponent<FieldComponent>();

    Vector2 fieldCenter = fieldComponent->GetCenter();

    m_Referee[0]->SetPosition(Vector3(5.0f, fieldCenter.y_ + 10.0f, PLAYER_DEFAULT_Z_COORD));

    m_TeamFirst[0]->SetPosition(Vector3(fieldCenter.x_ - 0.5f, fieldCenter.y_, PLAYER_DEFAULT_Z_COORD));
    for (unsigned int i = 1; i < m_TeamFirst.Size(); i++)
    {
        PlayerComponent* playerComponent = (PlayerComponent*)(m_TeamFirst[i]->GetComponent(m_TeamFirst[i]->GetVar("playerType").GetStringHash()));
        if (!playerComponent->IsInGame())
        {
            continue;
        }

        if (playerComponent->GetHumanType() == tGoalkeeper)
        {
            m_TeamFirst[i]->SetPosition(Vector3(fieldComponent->GetLeftSide(), fieldCenter.y_, PLAYER_DEFAULT_Z_COORD));
        }
        else
        {
            m_TeamFirst[i]->SetPosition(Vector3(fieldCenter.x_ - Random(fieldComponent->GetWidth() / 2.0f - 15.0f), Random(fieldComponent->GetDownSide() + 1.0f, fieldComponent->GetUpSide() - 1.0f), PLAYER_DEFAULT_Z_COORD));
        }
    }


    for (unsigned int i = 0; i < m_TeamFirst.Size(); i++)
    {
        PlayerComponent* playerComponent = (PlayerComponent*)(m_TeamFirst[i]->GetComponent(m_TeamSecond[i]->GetVar("playerType").GetStringHash()));

        if (!playerComponent->IsInGame())
        {
            continue;
        }

        if (playerComponent->GetHumanType() == tGoalkeeper)
        {
            m_TeamSecond[i]->SetPosition(Vector3(fieldComponent->GetRightSide(), fieldCenter.y_, PLAYER_DEFAULT_Z_COORD));
        }
        else
        {
            m_TeamSecond[i]->SetPosition(Vector3(fieldCenter.x_ + Random(fieldComponent->GetWidth() / 2.0f - 15.0f), Random(fieldComponent->GetDownSide() + 1.0f, fieldComponent->GetUpSide() - 1.0f), PLAYER_DEFAULT_Z_COORD));
        }
    }
}

void FootballLogic::SetPosition2()
{
    Node* field = m_pScene->GetChild("Field");
    FieldComponent* fieldComponent = field->GetComponent<FieldComponent>();

    Vector2 fieldCenter = fieldComponent->GetCenter();

    m_Referee[0]->SetPosition(Vector3(5.0f, fieldCenter.y_ + 10.0f, PLAYER_DEFAULT_Z_COORD));

    for (unsigned int i = 0; i < m_TeamFirst.Size(); i++)
    {
        PlayerComponent* playerComponent = (PlayerComponent*)(m_TeamFirst[i]->GetComponent(m_TeamFirst[i]->GetVar("playerType").GetStringHash()));
        if (!playerComponent->IsInGame())
        {
            continue;
        }

        if (playerComponent->GetHumanType() == tGoalkeeper)
        {
            m_TeamFirst[i]->SetPosition(Vector3(fieldComponent->GetLeftSide(), fieldCenter.y_, PLAYER_DEFAULT_Z_COORD));
        }
        else
        {
            m_TeamFirst[i]->SetPosition(Vector3(fieldCenter.x_ - Random(fieldComponent->GetWidth() / 2 - 15.0f), Random(fieldComponent->GetDownSide() + 1.0f, fieldComponent->GetUpSide() - 1.0f), PLAYER_DEFAULT_Z_COORD));
        }
    }

    m_TeamSecond[0]->SetPosition(Vector3(fieldCenter.x_ + 0.5f, fieldCenter.y_, PLAYER_DEFAULT_Z_COORD));
    for (unsigned int i = 1; i < m_TeamFirst.Size(); i++)
    {
        PlayerComponent* playerComponent = (PlayerComponent*)(m_TeamFirst[i]->GetComponent(m_TeamSecond[i]->GetVar("playerType").GetStringHash()));

        if (!playerComponent->IsInGame())
        {
            continue;
        }

        if (playerComponent->GetHumanType() == tGoalkeeper)
        {
            m_TeamSecond[i]->SetPosition(Vector3(fieldComponent->GetRightSide(), fieldCenter.y_, PLAYER_DEFAULT_Z_COORD));
        }
        else
        {
            m_TeamSecond[i]->SetPosition(Vector3(fieldCenter.x_ + Random(fieldComponent->GetWidth() / 2 - 15.0f), Random(fieldComponent->GetDownSide() + 1.0f, fieldComponent->GetUpSide() - 1.0f), PLAYER_DEFAULT_Z_COORD));
        }
    }
}

bool FootballLogic::CheckLeftGoal()
{
    Node* field = m_pScene->GetChild("Field");
    Node* ball = m_pScene->GetChild("Ball");
    FieldComponent* fieldComponent = field->GetComponent<FieldComponent>();
    BallComponent* ballComponent = ball->GetComponent<BallComponent>();
    if (fieldComponent->IsGoalLeft(ball->GetPosition()))
    {
        sleep(1);
        IncreaseScore(TeamNumber::Right);
        SetCommentText("***** GOAL!!! *****");

        auto soundSource = m_Referee[0]->GetOrCreateComponent<SoundSource>();
        soundSource->Play(g_pApp->GetResCache()->GetResource<Sound>("Audio/whistle.wav"));
        soundSource->Play(g_pApp->GetResCache()->GetResource<Sound>("Audio/mencheer.wav"));
        SetPosition1();
        return true;
    }
    return false;
}

bool FootballLogic::CheckRightGoal()
{
    Node* field = m_pScene->GetChild("Field");
    Node* ball = m_pScene->GetChild("Ball");
    FieldComponent* fieldComponent = field->GetComponent<FieldComponent>();
    if (fieldComponent->IsGoalRight(ball->GetPosition()))
    {
        sleep(1);
        IncreaseScore(TeamNumber::Left);
        SetCommentText("***** GOAL!!! *****");

        auto soundSource = m_Referee[0]->GetOrCreateComponent<SoundSource>();
        soundSource->Play(g_pApp->GetResCache()->GetResource<Sound>("Audio/whistle.wav"));
        soundSource->Play(g_pApp->GetResCache()->GetResource<Sound>("Audio/mencheer.wav"));
        SetPosition2();
        return true;
    }
    return false;
}

void FootballLogic::MoveAll(float timeStep)
{
    float currentTimeStep = timeStep;
    float slowDown = currentTimeStep - m_SimulationSlowDownTime;

    BallComponent* ballComponent = m_pBall->GetComponent<BallComponent>();
    ballComponent->Move(slowDown);

    for (auto it = m_Referee.Begin(); it != m_Referee.End(); it++)
    {
        HumanComponent* humanComponent = (HumanComponent*)(*it)->GetComponent((*it)->GetVar("refereeType").GetStringHash());
        humanComponent->VMove(slowDown);
    }

    for (auto it = m_AssReferee.Begin(); it != m_AssReferee.End(); it++)
    {
        HumanComponent* humanComponent = (HumanComponent*)(*it)->GetComponent((*it)->GetVar("refereeType").GetStringHash());
        humanComponent->VMove(slowDown);
    }

    for (auto it = m_TeamFirst.Begin(); it != m_TeamFirst.End(); it++)
    {
        PlayerComponent* playerComponent = (PlayerComponent*)(*it)->GetComponent((*it)->GetVar("playerType").GetStringHash());
        playerComponent->VMove(slowDown);
    }

    for (auto it = m_TeamSecond.Begin(); it != m_TeamSecond.End(); it++)
    {
        PlayerComponent* playerComponent = (PlayerComponent*)(*it)->GetComponent((*it)->GetVar("playerType").GetStringHash());
        playerComponent->VMove(slowDown);
    }
}

void FootballLogic::HandleEnergyValueDelegate(StringHash eventType, VariantMap& eventData)
{
    float energy = eventData[ENERGY_VALUE_CHANGE::P_VALUE].GetFloat();
    TeamNumber team = (TeamNumber)eventData[ENERGY_VALUE_CHANGE::P_TEAM].GetInt();
    if (team == TeamNumber::Left)
    {
        float energyPortion = energy / m_TeamFirst.Size();
        for (auto it = m_TeamFirst.Begin(); it != m_TeamFirst.End(); it++)
        {
            PlayerComponent* playerComponent = (PlayerComponent*)(*it)->GetComponent((*it)->GetVar("playerType").GetStringHash());
            playerComponent->SetEnergy(energyPortion);
        }
    }
    else
    {
        float energyPortion = energy / m_TeamFirst.Size();
        for (auto it = m_TeamFirst.Begin(); it != m_TeamFirst.End(); it++)
        {
            PlayerComponent* playerComponent = (PlayerComponent*)(*it)->GetComponent((*it)->GetVar("playerType").GetStringHash());
            playerComponent->SetEnergy(energyPortion);
        }
    }
}

void FootballLogic::HandlePlayerCheckBoxDelegate(StringHash eventType, VariantMap& eventData)
{
    int playerNumber = eventData[PLAYER_CHECKBOX_PRESSED::P_PLAYER_NUMBER].GetInt();
    TeamNumber team = (TeamNumber)eventData[PLAYER_CHECKBOX_PRESSED::P_TEAM].GetInt();
    bool value = eventData[PLAYER_CHECKBOX_PRESSED::P_VALUE].GetBool();
    if (team == TeamNumber::Left)
    {
        playerNumber--;
        Node* player = m_TeamFirst[playerNumber];
        PlayerComponent* playerComponent = (PlayerComponent*)player->GetComponent(player->GetVar("playerType").GetStringHash());
        playerComponent->SetInGame(value);
    }
    else
    {
        playerNumber--;
        Node* player = m_TeamSecond[playerNumber];
        PlayerComponent* playerComponent = (PlayerComponent*)player->GetComponent(player->GetVar("playerType").GetStringHash());
        playerComponent->SetInGame(value);
    }
}

void FootballLogic::HandleSimulationValueDelegate(StringHash eventType, VariantMap &eventData)
{
    float value = eventData[SIMULATION_SLIDER_CHANGE::P_VALUE].GetFloat();
    m_SimulationSlowDownTime = ((600 - 10 * value) / 600.0f) / 100.0f;
}

void FootballLogic::HandleStopSimulationDelegate(StringHash eventType, VariantMap& eventData)
{
    m_bIsGameStarted = false;
    SetCommentText("Pause...");
}

void FootballLogic::HandleIntroSimulationDelegate(StringHash eventType, VariantMap& eventData)
{
    SetScore(TeamNumber::Left, 0);
    SetScore(TeamNumber::Right, 0);
    SetLeftTeamEnergy(0);
    SetRightTeamEnergy(0);
    SetTime(0.0f);
    SetCommentText(" ");
    SetPosition0();
}

void FootballLogic::HandleFirstKickDelegate(StringHash eventType, VariantMap& eventData)
{
    if (!m_bIsGameStarted)
    {
        StartGame();
    }
    else
    {
        Node* field = m_pScene->GetChild("Field");
        Node* ball = m_pScene->GetChild("Ball");
        FieldComponent* fieldComponent = field->GetComponent<FieldComponent>();
        BallComponent* ballComponent = ball->GetComponent<BallComponent>();
        ball->SetPosition(fieldComponent->GetCenter());
        ballComponent->SetSpeed(0.0f);
        SetCommentText("");
    }
}

void FootballLogic::HandleCommentDelegate(StringHash eventType, VariantMap& eventData)
{
    String comment = eventData[COMMENT::P_COMMENT].GetString();
    SetCommentText(comment);
}

float FootballLogic::CalcLeftTeamEnergy()
{
    m_LeftTeamEnergy = 0.0f;
    for (auto it = m_TeamFirst.Begin(); it < m_TeamFirst.End(); it++)
    {
        PlayerComponent* playerComponent = (PlayerComponent*)(*it)->GetComponent((*it)->GetVar("playerType").GetStringHash());
        m_LeftTeamEnergy += playerComponent->GetEnergy();
    }

    return m_LeftTeamEnergy;
}

float FootballLogic::CalcRightTeamEnergy()
{
    m_RightTeamEnergy = 0.0f;
    for (auto it = m_TeamSecond.Begin(); it < m_TeamSecond.End(); it++)
    {
        PlayerComponent* playerComponent = (PlayerComponent*)(*it)->GetComponent((*it)->GetVar("playerType").GetStringHash());
        m_RightTeamEnergy += playerComponent->GetEnergy();
    }

    return m_RightTeamEnergy;
}

void FootballLogic::SetCommentText(String text)
{
    if (text != m_CommentText)
    {
        m_CommentText = text;
        m_bIsCommentTextDirty = true;
    }
}

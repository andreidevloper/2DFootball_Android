#include "FootballStd.h"

#include "Components/Objects/HumanComponent/HumanComponent.h"
#include "Components/Objects/Player/PlayerComponent/PlayerComponent.h"
#include "Components/Objects/Player/ForwardComponent/ForwardComponent.h"
#include "Components/Objects/Player/FullBackComponent/FullBackComponent.h"
#include "Components/Objects/Player/GoalkeeperComponent/GoalkeeperComponent.h"
#include "Components/Objects/Player/HalfBackComponent/HalfBackComponent.h"

#include "Components/Objects/Referee/AssRefereeComponent/AssRefereeComponent.h"
#include "Components/Objects/Referee/RefereeComponent/RefereeComponent.h"

#include "PlayerFactory.h"

PlayerFactory::PlayerFactory(Context* context) : Object(context)
{

}

void PlayerFactory::RegisterObjects(Context* context)
{
	HumanComponent::RegisterObject(context);
	PlayerComponent::RegisterObject(context);
	ForwardComponent::RegisterObject(context);
	FullBackComponent::RegisterObject(context);
	GoalkeeperComponent::RegisterObject(context);
	HalfBackComponent::RegisterObject(context);

	RefereeComponent::RegisterObject(context);
	AssRefereeComponent::RegisterObject(context);
}

Node* PlayerFactory::CreatePlayer(Scene* scene, htype playerType, String name, TeamNumber team, int number, Vector<Node*>* teamContainer)
{
	Node* newPlayer = NULL;
	switch (playerType)
	{
		case tForward:
		{
			newPlayer = CreateNewPlayer(scene, name);
			
			ForwardComponent* forwardComponent = newPlayer->CreateComponent<ForwardComponent>();
			forwardComponent->SetTeamNumber(team, number);
			forwardComponent->SetTeam(teamContainer);

			newPlayer->SetVar("playerType", forwardComponent->GetType());
			SetTeamColor(newPlayer, team);

			break;
		}

		case tHalfBack:
		{
			newPlayer = CreateNewPlayer(scene, name);

			HalfBackComponent* halfbackComponent = newPlayer->CreateComponent<HalfBackComponent>();
			halfbackComponent->SetTeamNumber(team, number);
			halfbackComponent->SetTeam(teamContainer);

			newPlayer->SetVar("playerType", halfbackComponent->GetType());
			SetTeamColor(newPlayer, team);
			break;
		}

		case tQuarterBack:
		{
			// Not support yet
			break;
		}

		case tFullBack:
		{
			newPlayer = CreateNewPlayer(scene, name);

			FullBackComponent* fullBackComponent = newPlayer->CreateComponent<FullBackComponent>();
			fullBackComponent->SetTeamNumber(team, number);
			fullBackComponent->SetTeam(teamContainer);

			newPlayer->SetVar("playerType", fullBackComponent->GetType());
			SetTeamColor(newPlayer, team);
			break;
		}

		case tGoalkeeper:
		{
			newPlayer = CreateNewPlayer(scene, name);

				
			GoalkeeperComponent* goalkeeperComponent = new GoalkeeperComponent(team, number, context_);
			newPlayer->AddComponent(goalkeeperComponent, scene->GetFreeComponentID(CreateMode::LOCAL), CreateMode::LOCAL);
			//newPlayer->CreateComponent<GoalkeeperComponent>();
			//goalkeeperComponent->SetTeamNumber(team, number);
			//goalkeeperComponent->SetTeam(teamContainer);

			StaticModel* model = newPlayer->GetComponent<StaticModel>();
			if (team == TeamNumber::Left)
			{
				model->SetMaterial(g_pApp->GetResCache()->GetResource<Material>("Materials/Team_One_Gk.xml"));
			}
			else
			{
				model->SetMaterial(g_pApp->GetResCache()->GetResource<Material>("Materials/Team_Two_Gk.xml"));
			}

			newPlayer->SetVar("playerType", goalkeeperComponent->GetType());

			break;
		}

		case tGirl:
		{
			// Not support yet
			break;
		}
	};

	if (newPlayer)
	{
		teamContainer->Push(newPlayer);
	}
	
	return newPlayer;
}

Node* PlayerFactory::CreateReferee(Scene* scene, htype playerType, String name, Vector<Node*>* refereeContainer)
{
	Node* newReferee = NULL;
	switch (playerType)
	{
		case tReferee:
		{
			newReferee = CreateNewReferee(scene, name, "Materials/Referee.xml");
			RefereeComponent* refereeComponent = newReferee->CreateComponent<RefereeComponent>();
			newReferee->SetVar("refereeType", refereeComponent->GetType());
			break;
		}

		case tAssReferee:
		{
			newReferee = CreateNewReferee(scene, name, "Materials/AssReferee.xml");
			AssRefereeComponent* refereeComponent = newReferee->CreateComponent<AssRefereeComponent>();
			newReferee->SetVar("refereeType", refereeComponent->GetType());
			break;
		}

		default:
		{
			return NULL;
		}
	};

	if (newReferee)
	{
		refereeContainer->Push(newReferee);
	}

	return newReferee;
}

Node* PlayerFactory::CreateNewPlayer(Scene* scene, String name)
{
	Node* playerNode = scene->CreateChild(name);
	
	playerNode->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	StaticModel* model = playerNode->CreateComponent<StaticModel>();
	model->SetModel(g_pApp->GetResCache()->GetResource<Model>("Models/Sphere.mdl"));

	return playerNode;
}

void PlayerFactory::SetTeamColor(Node* node, TeamNumber team)
{
	StaticModel* model = node->GetComponent<StaticModel>();

	if (team == TeamNumber::Left)
	{
		model->SetMaterial(g_pApp->GetResCache()->GetResource<Material>("Materials/Team_One.xml"));
	}
	else
	{
		model->SetMaterial(g_pApp->GetResCache()->GetResource<Material>("Materials/Team_Two.xml"));
	}
}

Node* PlayerFactory::CreateNewReferee(Scene* scene, String name, String material)
{
	Node* refereeNode = scene->CreateChild(name);
	refereeNode->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	StaticModel* model = refereeNode->CreateComponent<StaticModel>();
	model->SetModel(g_pApp->GetResCache()->GetResource<Model>("Models/Sphere.mdl"));
	model->SetMaterial(g_pApp->GetResCache()->GetResource<Material>(material));

	return refereeNode;
}
#pragma once

#include "../../HumanComponent/HumanComponent.h"

class PlayerFactory : public Object
{
	URHO3D_OBJECT(PlayerFactory, Object)

public:
	// private constructor.
	PlayerFactory(Context* context);
	static void RegisterObjects(Context* context);

	Node* CreatePlayer(Scene* scene, htype playerType, String name, TeamNumber team, int number, Vector<Node*>* teamContainer = NULL);
	Node* CreateReferee(Scene* scene, htype playerType, String name, Vector<Node*>* refereeContainer = NULL);
	
private:
	// Player utility functions
	Node* CreateNewPlayer(Scene* scene, String name);
	StaticModel* CreatePlayerModel(Node* playerNode);
	void SetTeamColor(Node* node, TeamNumber number);
	
	// Referee utility functions
	Node* CreateNewReferee(Scene* scene, String name, String material);



};

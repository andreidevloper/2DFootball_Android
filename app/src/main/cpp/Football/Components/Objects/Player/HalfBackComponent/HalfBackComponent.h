#pragma once

#include "../PlayerComponent/PlayerComponent.h"

class HalfBackComponent : public PlayerComponent
{
	URHO3D_OBJECT(HalfBackComponent, PlayerComponent)

public:
	// Construct.
	HalfBackComponent(Context* context);
	HalfBackComponent(TeamNumber teamNumber, int playerNumber, Context* context);
	
	// Register object factory and URHO3D_ATTRIBUTEs.
	static void RegisterObject(Context* context);
	virtual void VKick();

};

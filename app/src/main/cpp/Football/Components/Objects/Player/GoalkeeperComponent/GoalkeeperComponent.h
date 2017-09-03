#pragma once

#include "../PlayerComponent/PlayerComponent.h"

class GoalkeeperComponent : public PlayerComponent
{
	URHO3D_OBJECT(GoalkeeperComponent, PlayerComponent)

public:
	// Construct.
	GoalkeeperComponent(Context* context);
	GoalkeeperComponent(TeamNumber teamNumber, int playerNumber, Context* context);
	
	// Register object factory and URHO3D_ATTRIBUTEs.
	static void RegisterObject(Context* context);
	virtual void DelayedStart();

	virtual void VMove(float timeStep);
	virtual void VKick();

private:
	void InitializeAllDelegates();				// Register all delegates
	void DestroyAllDelegates();					// Unsubscribe from all events

private:

};

#pragma once

#include "../PlayerComponent/PlayerComponent.h"

class FullBackComponent : public PlayerComponent
{
	URHO3D_OBJECT(FullBackComponent, PlayerComponent)

public:
	// Construct.
	FullBackComponent(Context* context);
	FullBackComponent(TeamNumber teamNumber, int playerNumber, Context* context);
	
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

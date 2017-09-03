#pragma once

#include "../PlayerComponent/PlayerComponent.h"

class ForwardComponent : public PlayerComponent
{
	URHO3D_OBJECT(ForwardComponent, PlayerComponent)

public:
	// Construct.
	ForwardComponent(Context* context);
	ForwardComponent(TeamNumber teamNumber, int playerNumber, Context* context);

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

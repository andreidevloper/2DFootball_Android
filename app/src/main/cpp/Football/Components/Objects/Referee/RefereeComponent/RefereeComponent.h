#pragma once

#include "Components/Objects/HumanComponent/HumanComponent.h"

class RefereeComponent : public HumanComponent
{
	URHO3D_OBJECT(RefereeComponent, HumanComponent)
public:
	// Construct.
	RefereeComponent(Context* context);

	// Register object factory and URHO3D_ATTRIBUTEs.
	static void RegisterObject(Context* context);
	void Start();

	virtual void VMove(float timeStep);

	// Getter/Setters


private:
	void InitializeAllDelegates();				// Register all delegates
	void DestroyAllDelegates();					// Unsubscribe from all events

protected:
	BallComponent* m_pBallComponent;
};

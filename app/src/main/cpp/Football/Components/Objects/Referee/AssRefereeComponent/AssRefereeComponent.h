#pragma once

#include "Components/Objects/HumanComponent/HumanComponent.h"

class AssRefereeComponent : public HumanComponent
{
	URHO3D_OBJECT(AssRefereeComponent, HumanComponent)
public:
	// Construct.
	AssRefereeComponent(Context* context);

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

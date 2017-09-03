#pragma once

class FieldComponent;

class BallComponent : public LogicComponent
{
	URHO3D_OBJECT(BallComponent, LogicComponent)

public:
	// Construct.
	BallComponent(Context* context);

	// Register object factory and URHO3D_ATTRIBUTEs.
	static void RegisterObject(Context* context);
	virtual void DelayedStart();

	void Draw();
	void Move(float timeStep);

	void SetSpeed(float s);
	float GetSpeed();
	void SetDir(double d);
	double GetDir();

	void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);
	void Update(float timeStep);

private:
	void InitializeAllDelegates();				// Register all delegates
	void DestroyAllDelegates();					// Unsubscribe from all events

private:
	DebugRenderer* m_pDebugRenderer;
	FieldComponent* m_pFieldComponent;

	float m_Radius;
	Color m_Color;
	float m_Speed;
	float m_Slowdown;
	double m_Dir;
};

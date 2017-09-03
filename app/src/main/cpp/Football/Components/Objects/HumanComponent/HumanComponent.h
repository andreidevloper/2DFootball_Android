#pragma once

enum htype
{
	tReferee,
	tAssReferee,
	tGoalkeeper,
	tForward,
	tHalfBack,
	tQuarterBack,
	tFullBack,
	tGirl
};

class HumanComponent : public LogicComponent
{
	URHO3D_OBJECT(HumanComponent, LogicComponent)

public:
	// Construct.
	HumanComponent(Context* context);

	// Register object factory and URHO3D_ATTRIBUTEs.
	static void RegisterObject(Context* context);
	virtual void DelayedStart();

	virtual void VMove(float timeStep) = 0;
	virtual void VDraw(float tiemStep) { }

	// Getter/Setters
	void SetSpeed(float s) { m_Speed = s; }
	float GetSpeed() { return m_Speed; }

	void SetDir(float d) { m_Dir = d; }
	float GetDir() { return m_Dir; }

	float GetEnergy() { return m_Energy; }
	void SetEnergy(float energy) { m_Energy = energy; }

	htype GetHumanType() { return m_HumanType; }

private:
	void InitializeAllDelegates();				// Register all delegates
	void DestroyAllDelegates();					// Unsubscribe from all events

protected:
	float m_Radius;
	float m_Speed;
	float m_Energy;
	float m_Dir;
	htype m_HumanType;
};

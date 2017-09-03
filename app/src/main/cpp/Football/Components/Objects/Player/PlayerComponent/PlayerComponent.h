#pragma once

#include "../../HumanComponent/HumanComponent.h"

class FootballLogic;
class FieldComponent;
class BallComponent;

enum TeamNumber
{
	Invalid = 0,
	Left,
	Right
};

class PlayerComponent : public HumanComponent
{
	URHO3D_OBJECT(PlayerComponent, HumanComponent)

public:
	// Construct.
	PlayerComponent(Context* context);
	PlayerComponent(TeamNumber teamNumber, int playerNumber, Context* context);
	
	// Register object factory and URHO3D_ATTRIBUTEs.
	static void RegisterObject(Context* context);
	virtual void DelayedStart();
	
	
	void VDraw(float tiemStep);
	virtual void VMove(float timeStep);
	virtual void VKick();
	void AvoidCollision(float timeStep);
	void MoveToBench();
	bool Partner(Vector3& result);
	
	void SetTeamNumber(TeamNumber teamNumber, int playerNumber) { m_TeamNumber = teamNumber; m_Number = playerNumber; }
	void SetTeam(Vector<Node*>* team) { m_pTeamContainer = team; }
	bool IsInGame() const { return m_bIsInGame; }
	void SetInGame(bool value) { m_bIsInGame = value; }

protected:
	FootballLogic* m_pFootballLogic;
	// For caching
	FieldComponent* m_pFieldComponent;
	BallComponent* m_pBallComponent;
	Vector<Node*>* m_pTeamContainer;

	TeamNumber m_TeamNumber;
	int m_Number;
	bool m_bIsInGame;
	IntRect m_Area;

	float m_KickDelay;
};

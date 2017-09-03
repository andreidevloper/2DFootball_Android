#pragma once

#include "GameEngine/GameLogic/BaseGameLogic.h"
#include "Components/Objects/Player/PlayerComponent/PlayerComponent.h"

class PlayerFactory;

// Состояния игры.
enum GameState
{
	GS_INTRO = 0,
	GS_FIRST_KICK,
	GS_GAMEPLAY,
	GS_GOAL,
	GS_RESTART_GAME,
	GS_STOP
};

class FootballLogic : public BaseGameLogic
{
	URHO3D_OBJECT(FootballLogic, BaseGameLogic)
public:
	FootballLogic(Context *context);
	virtual ~FootballLogic();

	virtual bool VInitialize();
	virtual void VChangeState(BaseGameState newState);
	virtual void VShutdown();

	virtual void VPreUpdate(float timeStep);
	virtual void VOnUpdate(float timeStep);

	virtual void VAddView(SharedPtr<IGameView> pView, NodeId actorId = INVALID_NODE_ID);
	virtual bool VLoadGameDelegate(String pLevelData);

	// Score
	int GetScore(TeamNumber team) const { return team == TeamNumber::Left ? m_LeftTeamScore : m_RightTeamScore; }
	void SetScore(TeamNumber team, int score) { team == TeamNumber::Left ? m_LeftTeamScore = score : m_RightTeamScore = score; m_bIsScoreTextDirty = true; }
	void IncreaseScore(TeamNumber team) { team == TeamNumber::Left ? m_LeftTeamScore++ : m_RightTeamScore++; m_bIsScoreTextDirty = true; }
	bool IsScoreTextDirty() const { return m_bIsScoreTextDirty; }
	void SetFalseSTDFlag() { m_bIsScoreTextDirty = false; }

	// Time
	void IncreaseTime(float value) { m_Time += value; m_bIsTimeTextDirty = true;}
	void SetTime(float value) { m_Time = value; m_bIsTimeTextDirty = true; }
	float GetTime() const { return m_Time; }
	bool IsTimeTextDirty() const { return m_bIsTimeTextDirty; }
	void SetFalseTTDFlag() { m_bIsTimeTextDirty = false; }

	// Energy
	void SetLeftTeamEnergy(float value) { m_LeftTeamEnergy = value; m_bIsEnergyTextDirty = true; }
	void SetRightTeamEnergy(float value) { m_RightTeamEnergy = value; m_bIsEnergyTextDirty = true; }
	float GetLeftTeamEnergy() const { return m_LeftTeamEnergy; }
	float GetRightTeamEnergy() const { return m_RightTeamEnergy; }
	bool IsEnergyTextDirty() const { return m_bIsEnergyTextDirty; }
	void SetFalseETDFlag() { m_bIsEnergyTextDirty = false; }

	// Comment
	String GetCommentText() const { return m_CommentText; }
	bool IsCommentTextDirty() const { return m_bIsCommentTextDirty; }
	void SetFalseCTDFlag() { m_bIsCommentTextDirty = false; }

	// Energy
	float CalcLeftTeamEnergy();
	float CalcRightTeamEnergy();

	void SetCommentText(String text);

    Node* GetReferee() { return m_Referee[0]; }
	const Vector<Node*>& GetReferees() { return m_Referee; }
    const Vector<Node*>& GetFirstTeam() { return m_TeamFirst; }
	const Vector<Node*>& GetSecondTeam() { return m_TeamSecond; }

    GameState GetGameState() { return m_GameState; }
    void SetNeededGameState(GameState gameState) { m_NeededGameState = gameState; }

protected:
	virtual void VInitializeComponents();

	void HandleStopSimulationDelegate(StringHash eventType, VariantMap& eventData);
	void HandleIntroSimulationDelegate(StringHash eventType, VariantMap& eventData);
	void HandleFirstKickDelegate(StringHash eventType, VariantMap& eventData);
    void HandleCommentDelegate(StringHash eventType, VariantMap& eventData);

	void HandlePlayerCheckBoxDelegate(StringHash eventType, VariantMap& eventData);
	void HandleSimulationValueDelegate(StringHash eventType, VariantMap& eventData);
	void HandleEnergyValueDelegate(StringHash eventType, VariantMap& eventData);

private:
	void CreateField();
	void CreateBall();
	void CreateTeams();
	void CreateUI();

	void StartNewGame();
	void StartGame();
	void Goal();
	void RestartGame();
	bool CheckLeftGoal();
	bool CheckRightGoal();

	void MoveAll(float timeStep);
	void SetPosition0();
	void SetPosition1();
	void SetPosition2();

private:
	Vector<Node*> m_TeamFirst;
	Vector<Node*> m_TeamSecond;
	Vector<Node*> m_Referee;
	Vector<Node*> m_AssReferee;

	PlayerFactory* m_pPlayerFactory;
	Node* m_pBall;

	bool m_bIsShowEnabled;
	float m_ShowTime;

	bool m_bIsGameStarted;
	float m_SimulationSlowDownTime;

	bool m_bIsScoreTextDirty;
	bool m_bIsTimeTextDirty;
	bool m_bIsEnergyTextDirty;
	bool m_bIsCommentTextDirty;

	float m_LeftTeamEnergy;
	float m_RightTeamEnergy;
	float m_Time;

	int m_LeftTeamScore;
	int m_RightTeamScore;

	String m_CommentText;

	GameState m_GameState;
	GameState m_NeededGameState;
};
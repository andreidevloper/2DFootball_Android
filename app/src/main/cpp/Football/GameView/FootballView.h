#pragma once

class FootballLogic;
#include "GameEngine/UserInterface/HumanView/HumanView.h"

class FootballView : public HumanView
{
	URHO3D_OBJECT(FootballView, HumanView)
public:
	FootballView(Context *context, Renderer* renderer);

	virtual bool VOnRestore();
	virtual void VOnUpdate(float timeStep);
	virtual void VShutdown();

	// Handle input message from devices
	virtual bool VOnMsgProc(AppMsg message);

	// Loading game environment.
	virtual bool VLoadGameDelegate(SharedPtr<File> level);

	// Get type from game view. Can be different, depend on realization.
	virtual GameViewType VGetType() { return GameView_Human; }

protected:
	void HandleIntroSimulationDelegate(StringHash eventType, VariantMap& eventData);
	void HandleFirstKickDelegate(StringHash eventType, VariantMap& eventData);
	void HandleRestartGameDelegate(StringHash eventType, VariantMap& eventData);

private:
	void CreateLight();

private:
    FootballLogic* m_pFootballLogic;
	Vector<Node*> m_Light;

	SharedPtr<IScreenElement> m_pMainMenuUI;
	SharedPtr<IScreenElement> m_pCommentUI;
};

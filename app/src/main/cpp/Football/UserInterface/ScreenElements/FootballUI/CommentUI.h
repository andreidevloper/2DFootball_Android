#pragma once

#include <GameLogic/FootballLogic.h>
#include "GameEngine/UserInterface/UserInterface.h"

class CommentUI : public BaseUI
{
	URHO3D_OBJECT(CommentUI, BaseUI);

public:
	CommentUI(Context* context);
	virtual void VOnUpdate(float timeStep);
	virtual bool VOnRestore();
	virtual void VSetVisible(bool visible);
	virtual bool VOnLostDevice();

protected:
	void CreateCommentWindow();
    void HandleCommentCloseButton(StringHash eventType, VariantMap& eventData);

protected:
	FootballLogic* m_pFootballLogic;
	UI* m_pUI;

	bool m_bIsInitialized;

	// Comment box
    SharedPtr<Window> m_pCommentWindow;
    Button* m_pCommentCloseButton;
    Text* m_pCommentText;
};

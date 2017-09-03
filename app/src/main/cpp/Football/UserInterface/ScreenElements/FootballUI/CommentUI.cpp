#include "FootballStd.h"

#include <GameEngine/UserInterface/UserInterface.h>
#include "Events/FootballEvents.h"
#include "CommentUI.h"

CommentUI::CommentUI(Context* context) : BaseUI(context)
{
	m_pUI = g_pApp->GetSubsystem<UI>();
	m_bIsInitialized = false;
	m_pCommentWindow = nullptr;
	m_bIsVisible = false;
	m_pFootballLogic = (FootballLogic*)g_pApp->GetGameLogic();
}

void CommentUI::VOnUpdate(float timeStep)
{
	if (m_pFootballLogic->IsCommentTextDirty())
	{
		String comment = m_pFootballLogic->GetCommentText();
		m_pCommentText->SetText(comment);
		m_pFootballLogic->SetFalseCTDFlag();
	}
}

bool CommentUI::VOnRestore()
{
	if (!m_bIsInitialized)
	{
		CommentUI::CreateCommentWindow();
		return true;
	}
	else
	{
		if (m_pCommentWindow)
		{
			m_pCommentWindow->SetVisible(true);
		}
	}

	return false;
}

void CommentUI::VSetVisible(bool value)
{
	if (m_pCommentWindow)
	{
		m_pCommentWindow->SetVisible(value);
	}
}

bool CommentUI::VOnLostDevice()
{
	m_pCommentWindow->SetVisible(false);
	return false;
}

void CommentUI::CreateCommentWindow()
{
	if (!m_pCommentWindow)
	{
		// Comment box
		XMLFile* file = g_pApp->GetResCache()->GetResource<XMLFile>("UI/CommentBox.xml");
		SharedPtr<UIElement> element = m_pUI->LoadLayout(file);

		m_pCommentWindow = StaticCast<Window>(element);
		m_pCommentCloseButton = (Button*)element->GetChild(String("Close"), true);
		SubscribeToEvent(m_pCommentCloseButton, E_RELEASED, URHO3D_HANDLER(CommentUI, HandleCommentCloseButton));

		m_pCommentText = (Text*)element->GetChild(String("Comment_Text"), true);
		m_pUI->GetRoot()->AddChild(element);

        m_bIsInitialized = true;
	}
}


void CommentUI::HandleCommentCloseButton(StringHash eventType, VariantMap& eventData)
{
	m_pCommentWindow->SetVisible(false);
}




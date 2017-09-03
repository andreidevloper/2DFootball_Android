#include "GameEngineStd.h"
#include "UserInterface.h"

BaseUI::BaseUI(Context* context) : IScreenElement(context)
{
	m_bIsVisible = true;
}

bool BaseUI::VIsVisible() const
{
	return m_bIsVisible;
}


void BaseUI::VSetVisible(bool visible)
{ 
	m_bIsVisible = visible; 
}
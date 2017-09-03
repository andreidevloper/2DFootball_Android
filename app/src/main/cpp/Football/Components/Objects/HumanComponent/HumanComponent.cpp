#include "FootballStd.h"
#include "HumanComponent.h"

HumanComponent::HumanComponent(Context* context) : LogicComponent(context)
{
	m_Radius = 1.0f;
	m_Speed = 0.0f;
	m_Energy = 0.0f;
	m_Dir = 0;
}

void HumanComponent::RegisterObject(Context* context)
{

}


void HumanComponent::InitializeAllDelegates()
{

}

void HumanComponent::DestroyAllDelegates()
{

}

void HumanComponent::DelayedStart()
{

}

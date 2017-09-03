#include "FootballStd.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent(Context* context) : LogicComponent(context)
{
	SetUpdateEventMask(USE_UPDATE);
	m_pInput = GetSubsystem<Input>();
}

void CameraComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<CameraComponent>();
}

void CameraComponent::Update(float timeStep)
{
	int wheel = m_pInput->GetMouseMoveWheel();
	if (wheel != 0)
	{
		auto pos = node_->GetPosition();
		pos.z_ += wheel * 4.0f;

		if (pos.z_ > -5.0f)
			pos.z_ = -5.0f;

		node_->SetPosition(pos);
	}

	if (m_pInput->GetMouseButtonPress(MOUSEB_RIGHT))
	{
		node_->SetPosition(CAMERA_DEFAULT_POS);
	}
}

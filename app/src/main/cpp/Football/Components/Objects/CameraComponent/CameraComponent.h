#pragma once

class CameraComponent : public LogicComponent
{
	URHO3D_OBJECT(CameraComponent, LogicComponent);

public:
	CameraComponent(Context* context);
	static void RegisterObject(Context* context);
	void Update(float timeStep);

protected:
	// for cache
	Input* m_pInput;

};

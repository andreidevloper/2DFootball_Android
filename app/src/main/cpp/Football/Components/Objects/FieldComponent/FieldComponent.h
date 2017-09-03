#pragma once

class FieldComponent : public LogicComponent
{
	URHO3D_OBJECT(FieldComponent, LogicComponent)

public:
	// Construct.
	FieldComponent(Context* context);

	// Register object factory and URHO3D_ATTRIBUTEs.
	static void RegisterObject(Context* context);
	void Start();

	bool IsInside(const Vector3& pos);
	bool IsOutSide(const Vector3& pos);

	bool IsOutLeft(const Vector3& pos);
	bool IsOutRight(const Vector3& pos);

	bool IsGoalRight(const Vector3& pos);
	bool IsGoalLeft(const Vector3& pos);

	Vector2 GetCenter() const { return m_Center; }
	float GetLeftSide() const { return m_FieldLeftSide; }
	float GetRightSide() const { return m_FieldRightSide; }
	float GetUpSide() const { return m_FieldUpSide; }
	float GetDownSide() const { return m_FieldDownSide; }
	float GetWidth() { return m_FieldRightSide - m_FieldLeftSide; }
	float GetHeight() { return m_FieldUpSide - m_FieldDownSide; }
	IntRect GetArea() const { return IntRect((int)m_FieldLeftSide, (int)m_FieldUpSide, (int)m_FieldRightSide, (int)m_FieldDownSide); }

	float GoalWidth() const { return m_GoalWidth; }
	Vector2 GetGoalArea() const { return Vector2(m_GoalAreaWidth, m_GoalAreaHeight); }

private:
	void InitializeAllDelegates();				// Register all delegates
	void DestroyAllDelegates();					// Unsubscribe from all events

private:
	float m_FieldLeftSide; // Tur kur zilie varti (-x axis)
	float m_FieldRightSide;// Tur kur sarkanie varti (+x axis)

	float m_FieldDownSide; 		// -y axis
	float m_FieldUpSide; 		// +y axis
	
	float m_GoalWidth;
	float m_GoalAreaHeight;
	float m_GoalAreaWidth;

	Vector2 m_Center;
};

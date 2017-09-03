#include "FootballStd.h"

float Distance(const Vector2& point1, const Vector2& point2)
{
	return (point1 - point2).Length();
}

float Direction(const Vector2& point1, const Vector2& point2)
{
	Vector2 delta = point1 - point2;

	if (delta.x_ == 0.0f)
	{
		return (float)(delta.y_ < 0.0f ? PI / 2 : 3 * PI / 2);
	}
	else
	{
		float result = (float)atan((float)delta.y_ / (float)delta.x_);
		result += (float)(delta.x_ > 0 ? PI : 0);
		return result;
	}
}

float Disperse(float direction, float delta)
{
	int k = Random(2) == 1 ? -1 : 1;
	float p = Random(101.0f) / 100.0f;
	return direction + k * p * delta;
}
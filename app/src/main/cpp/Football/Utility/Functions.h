#pragma once

#define SHOW_TIME 5.0f
#define CAMERA_DEFAULT_POS Vector3(0.0f, 0.477247f, -88.0428f)
#define PLAYER_DEFAULT_Z_COORD -0.527356f
#define PI 3.1415926535897932385

extern float Distance(const Vector2& point1, const Vector2& point2);
extern float Direction(const Vector2& point1, const Vector2& point2);
extern float Disperse(float direction, float delta);
#include "pch.h"
#include "Camera.h"

Vector2f Camera::Get() const {
	return pos;
}

void Camera::Add(float x, float y) {
	pos.x += x;
	pos.y += y;
}

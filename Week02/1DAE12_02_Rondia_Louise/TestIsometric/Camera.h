#pragma once
class Camera
{
public:
	Vector2f Get() const;
	void Add(float x, float y); // overload elapsedsec et dir
private:
	Vector2f pos{};
};


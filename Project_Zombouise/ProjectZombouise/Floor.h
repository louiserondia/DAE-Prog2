#pragma once
class FloorRenderable;

class Floor final
{
public:
	explicit Floor() = default;
	explicit Floor(int token, FloorRenderable* renderable);

private:
	int m_Type{};
	FloorRenderable* m_pRenderable{};
};


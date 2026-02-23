#pragma once
#include <vector>
#include <functional>
#include "Map.h"

class Renderer
{
public:

	Renderer() = default;
	Renderer(int width, int height, std::vector<Renderable> stack);

	void DrawMap(const Rectf& viewPort) const;
	void PrintStack() const;
	void SortStack();

private:

	int m_Width{};
	int m_Height{};
	std::vector<Renderable> m_Stack{};

	// i make a function to send to the std sort, the cmp function is initialized in the constructor
	std::function<bool(const Renderable&, const Renderable&)> m_CompareRenderables;

};


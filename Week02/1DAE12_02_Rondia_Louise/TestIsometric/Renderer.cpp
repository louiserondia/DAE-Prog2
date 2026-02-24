#include "pch.h"
#include "Renderer.h"

Renderer::Renderer(int width, int height, std::vector<Renderable> stack) :
	m_Width(width), m_Height(height), m_Stack(stack)
{
	m_CompareRenderables = [this](const Renderable& lhs, const Renderable& rhs) {
		return lhs.GetDistToMaxWidth(m_Width) < rhs.GetDistToMaxWidth(m_Width);
		};
}

void Renderer::PrintStack() const {
	std::cout << "---- STACK ----\n";
	//for (const Renderable& item : stack) {
		//std::cout << "[ " << item.coord.x << ", " << item.coord.y << " ] (" << item.GetDistToMaxWidth(item.coord, 4) << ") ";
	//}

	std::cout << std::endl;
	std::cout << std::endl;
}

void Renderer::DrawMap(const Rectf& viewPort) const {
	for (int idx{}; idx < m_Stack.size(); ++idx) {
		const Renderable& renderable{ m_Stack[idx] };


		// make a camera, use the transform to move it and renderable doesn't know about the viewport
		renderable.Draw(viewPort);
	}
}

void Renderer::SortStack() {
	std::sort(m_Stack.rbegin(), m_Stack.rend(), m_CompareRenderables);
}

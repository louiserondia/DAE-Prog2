#pragma once
#include <vector>
#include <functional>
#include "Map.h"
#include "Camera.h"
#include "CharacterRenderable.h"

class Renderer
{
public:

	Renderer() = default;
	Renderer(std::vector< std::unique_ptr<Renderable>> stack);

	void SetDimensions(int width, int height);
	void DrawMap(const Rectf& viewPort, const Camera& camera, const Vector2i &dir) const;
	void SortStack();
	void PushBack(const Renderable& character);
	CharacterRenderable* CreateCharacter();

private:

	int m_Width{};
	int m_Height{};
	std::vector<std::unique_ptr<Renderable>> m_Stack{};
	CharacterRenderable* m_Character{};

	// make function to create the character

	// i make a function to send to the std sort, the cmp function is initialized in the constructor
	std::function<bool(const std::unique_ptr<Renderable>&, const std::unique_ptr <Renderable>&)> m_CompareRenderables;

};


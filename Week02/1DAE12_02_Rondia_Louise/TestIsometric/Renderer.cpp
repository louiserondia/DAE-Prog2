#include "pch.h"
#include "Renderer.h"
#include "utils.h"
#include "TextureManager.h"

Renderer::Renderer(std::vector<std::unique_ptr<Renderable>> stack) :
	m_Stack(std::move(stack))
{
}

void Renderer::SetDimensions(int width, int height) {
	//, m_Map.GetWidth(), m_Map.GetHeight()
	m_Width = width;
	m_Height = height;
	
	m_CompareRenderables = 
		[w = m_Width](const std::unique_ptr<Renderable>& lhs, const std::unique_ptr<Renderable>& rhs) {
		if (lhs.get()->name == "lion") {
			std::cout << "w : " << w << std::endl;
			std::cout << "lhs : " << lhs.get()->GetDistToBottomRight(w) 
					<< ", rhs : " << rhs.get()->GetDistToBottomRight(w) << std::endl;
		}
		// c broken ici
		// if floor, always smaller
		return lhs.get()->GetDistToBottomRight(w) < rhs.get()->GetDistToBottomRight(w);
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

Color4f GetColor(const Vector2i& dir) {

	if (dir.x == -1 && !dir.y) return Color4f{ 1.f, 1.f, 0.f, 1.f }; // W
	if (dir.x == -1 && dir.y == 1) return Color4f{ 1.f, 0.f, 0.f, 1.f }; // NW
	if (!dir.x && dir.y == 1) return Color4f{ 1.f, 0.f, 0.5f, 1.f }; // N
	if (dir.x == 1 && dir.y == 1) return Color4f{ 1.f, 0.f, 1.f, 1.f }; // NE
	if (dir.x == 1 && !dir.y) return Color4f{ 0.f, 0.f, 1.f, 1.f }; // E
	if (dir.x == 1 && dir.y == -1) return Color4f{ 0.f, 1.f, 1.f, 1.f }; // SE
	if (!dir.x && dir.y == -1) return Color4f{ 0.f, 1.f, 1.f, 1.f }; // S
	if (dir.x == -1 && dir.y == -1) return Color4f{ 0.f, 1.f, 0.f, 1.f }; // SW
	return Color4f{ 0.f, 0.f, 0.f, 1.f };
}

Vector2i ScreenCoordFromPos(const Vector2f& pos, float w, float h) {
	float halfW = w * 0.5f;
	float halfH = h * 0.5f;

	float gx = (pos.x / halfW - pos.y / halfH) * 0.5f;
	float gy = (pos.x / halfW + pos.y / halfH) * 0.5f;

	return Vector2i{
		static_cast<int>(std::floor(gx)),
		static_cast<int>(std::floor(gy))
	};
}

void Renderer::DrawMap(const Rectf& viewPort, const Camera& camera, const Vector2i& dir) const {
	const Vector2f cam{ camera.Get() };
	const Vector2i camCoord{ ScreenCoordFromPos(cam, 128.f, 64.f) };

	for (int idx{}; idx < m_Stack.size(); ++idx) {
		glPushMatrix();
		{
			const Renderable& renderable{ *m_Stack[idx] };

			//glRotatef(angle, 0.f, 0.f, 1.f);
			//glScalef(1.5, 1.f, 1.f); // scale avec le zoom

			// cam is in the middle of the screen
			glTranslatef(-cam.x + viewPort.width / 2, -cam.y + viewPort.height / 2, 0.f);
			renderable.Draw();
		}
		glPopMatrix();
	}
	utils::SetColor(GetColor(dir));
	utils::FillRect(viewPort.width / 2 - 5.f, viewPort.height / 2 - 5.f, 10.f, 10.f);
}

void Renderer::SortStack() {
	std::sort(m_Stack.rbegin(), m_Stack.rend(), m_CompareRenderables);
}

void Renderer::PushBack(const Renderable& character) { // characterrenderable instead 
	m_Stack.push_back(std::make_unique<Renderable>(character));
}

//mettre dans utils
Vector2f ScreenPosFromCoord(const Vector2i& coord, float w, float h) {
	return Vector2f{
		(w / 2) * static_cast<float>(coord.x + coord.y),
		(h / 2) * static_cast<float>(coord.y - coord.x) - h / 2
	};
}

CharacterRenderable* Renderer::CreateCharacter() {
	const Vector2i coord{};
	const Vector2f pos{ ScreenPosFromCoord(coord, 128.f, 64.f) };

	std::unique_ptr<CharacterRenderable> character =
		std::make_unique<CharacterRenderable>(CharacterRenderable{
			pos,
			TextureManager::GetInstance()->GetTexture(TextureManager::Type::character),
			coord,
			"lion"
			});
	m_Character = character.get();
	m_Stack.push_back(std::move(character));
	return m_Character;
}


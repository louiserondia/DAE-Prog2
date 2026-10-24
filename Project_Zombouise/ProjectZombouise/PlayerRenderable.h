#pragma once
#include "Renderable.h"
#include "WalkingRenderable.h"
#include <map>

class Player;

#define TEX_SRC_IDLE_W 64.f
#define TEX_SRC_IDLE_H 147.f
#define TEX_SRC_PUSH_W 160.f
#define TEX_SRC_PUSH_H 184.f
#define TEX_SRC_STOMP_W 98.f
#define TEX_SRC_PUSH_OFFSET_Y 45.f


class PlayerRenderable final : public WalkingRenderable
{

public:

	explicit PlayerRenderable(
		const Vector2f& pos,
		std::pair<Texture*, Rectf> texturePair,
		int dist,
		int priority,
		const std::string& str);

	virtual ~PlayerRenderable() override = default;

	virtual void	Reset(const Vector2i& coord) override;
	virtual void	Draw() const override;

	void			SetFpsForFrame();
	void			UpdateFrameBounds();
	virtual void	UpdateAnimFromKeys(const bool* keys, const Vector2f& dir = Vector2f{}) override;
	void			SetOwner(Player* owner);

	void			Push();
	void			Idle();
	void			Stomp();

private:

	void			InitAnimFrames();
	Player*			m_pOwner{};
};

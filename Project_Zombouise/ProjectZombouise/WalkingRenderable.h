#pragma once
#include "Renderable.h"
#include <map>

class WalkingRenderable : public Renderable
{

public:
	enum class AnimType
	{
		idle,
		walk,
		dead
	};

	explicit WalkingRenderable(
		const Vector2f& pos,
		std::pair<Texture*, Rectf> texturePair,
		int dist,
		int priority,
		const std::string& str
	);
	
	virtual ~WalkingRenderable() override;
	WalkingRenderable(const WalkingRenderable& other) = delete;
	WalkingRenderable& operator=(const WalkingRenderable& other) = delete;
	WalkingRenderable(WalkingRenderable&& other) noexcept = default;
	WalkingRenderable& operator=(WalkingRenderable&& other) noexcept = default;

	virtual void	Reset(const Vector2i& coord);
	virtual void	Draw() const override;
	virtual void	Update(float elapsedSec);
	virtual void	UpdateAnimFromKeys(const bool* keys, const Vector2f& dir = Vector2f{});
	void			UpdateAnimFromDir(const Vector2f& dir);

	void			SetPos(const Vector2f& pos);
	void			AddToPos(const Vector2f& pos);
	void			AddToPos(float x, float y);
	Rectf			GetFrameDst() const;
	bool			IsHalfwayThroughAnimation() const;
	virtual void	UpdateFrameBounds() = 0;
	virtual void	Die();
	char			GetCurAnimFrameType() { return m_CurAnimFrameStr[0]; }

	Rectf			GetDstBounds() const;
	std::string		GetAnimFrameStr() const;


protected:

	struct AnimFrame {
		int row{};
		int col{};
		int nrFrames{};
	};

	virtual void	InitAnimFrames() = 0;
	void			UpdateAnimation(float elapsedSec = 0.f);
	void			SetAnimFrameStrFromFov(const bool* keys, const Vector2f& dir);
	void			SetAnimFrameStrFromKeys(const bool* keys);
	void			SetToNewAnimState(char type);

	static void		DrawDebug(const Vector2f& pos, const Rectf& frame);

	int		m_Fps{ 2 };
	float	m_FrameTimer{};
	Rectf	m_FrameSrc{ 0.f, 0.f, 128.f, 294.f };
	Rectf	m_FrameDst{ 0.f, 0.f, 64.f, 294.f / 2 };
	bool	m_HasArmsUp{};
	bool	m_IsDead{};

	float	m_HalfSec{};

	std::string m_CurAnimFrameStr{};
	std::map<std::string, AnimFrame> m_AnimFrames{};
	
private:

	// everything is used in the children as getters and setters somehow 
	// so strangefully enough everything is protected

};


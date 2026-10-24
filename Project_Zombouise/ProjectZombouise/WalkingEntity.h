#pragma once
#include "Vector2i.h"
#include "Tile.h"
#include <unordered_map>

class WalkingRenderable;
class Map;

#define P_CROUCH_SPEED 0.4f
#define P_WALK_SPEED 0.6f
#define Z_WALK_SPEED 0.5f
#define P_RUN_SPEED 1.f
#define Z_RUN_SPEED 0.9f

class WalkingEntity
{
public:
	using tile_map_t = std::unordered_map <Vector2i, Tile>;

	explicit WalkingEntity() = default;
	virtual ~WalkingEntity() = default;
	WalkingEntity(const WalkingEntity& other) = default;
	WalkingEntity& operator=(const WalkingEntity& other) = default;
	WalkingEntity(WalkingEntity&& other) noexcept = default;
	WalkingEntity& operator=(WalkingEntity&& other) noexcept = default;

	virtual void	Reset();
	virtual void	Update(float elapsedSec, const Map& map);

	Vector2i		GetCoord() const;
	Vector2f		GetDir() const;
	bool			IsCollision(const Vector2f& pos, const Vector2f& mvt, const tile_map_t& map);
	Vector2f		CheckCollision(int precision, Vector2f pos, Vector2f mvt, const tile_map_t& map);
	static Vector2f GetSlideMvt(const Vector2f& normal, const Vector2f& mvt);

	bool			IsAttacking() const { return m_IsAttacking; }
	virtual void	TryAttack(WalkingEntity* enemy);
	virtual void	Pushed(const Vector2f& dir);
	virtual void	GetDamage(int amount, WalkingEntity *attacker);
	void			SetPushed(const Vector2f& dir);

	virtual WalkingRenderable* GetRenderable() const = 0;
	// returns a generic Renderable but is not owner

protected:

	virtual void	UpdatePos(float elapsedSec, const tile_map_t& map) = 0;
	void			UpdateAttackTimer(float elapsedSec);
	void			UpdateFrozenTimer(float elapsedSec);

	void			SetMaxAttackCooldown(float Timer) { m_AttackCooldown = Timer; }
	virtual bool	CanAttack(const WalkingEntity* enemy) const;
	virtual void	Attack(WalkingEntity* enemy);

	float			m_Speed{ 0.8f };
	float			m_MaxSpeed{ 300.f };
	Vector2i		m_Coord{};
	Vector2f		m_Dir{};
	Vector2f		m_PushedDir{};

	int				m_HP{ 1 };
	bool			m_IsFrozen{};
	bool			m_IsAttacking{};
	float			m_AttackCooldown{};
	bool			m_IsPushed{};
	float			m_PushTimer{};

private:

	void			UpdateDir();
	void			UpdatePushTimer(float elapsedSec);

	const float		m_MaxAttackCooldown{ 0.8f };
	const float		m_MaxAttackTimer{ 0.3f }; //statoc
	float			m_AttackTimer{};
	float			m_FrozenTimer{};

	// walking entity doesnt own the renderable, 
	// they are strongly typed and owned by the children
	// then pushed in the stack of Renderable * for the Renderer
};


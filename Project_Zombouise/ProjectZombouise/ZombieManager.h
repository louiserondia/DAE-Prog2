#pragma once
#include <vector>
#include "Map.h"
#include "Player.h"
#include "Renderer.h"
class Zombie;

class ZombieManager final
{
public:

	explicit ZombieManager();
	~ZombieManager();
	ZombieManager(const ZombieManager& rhs);
	ZombieManager& operator=(const ZombieManager& rhs);
	ZombieManager(ZombieManager&& rhs) noexcept;
	ZombieManager& operator=(ZombieManager&& rhs) noexcept;
	
	void	AddZombiesToRenderQueue(Renderer &renderer);
	void	Reset();
	void	Update(float elapsedSec, const Map& map, Player& player, Renderer& renderer);
	void	SpawnZombie(Renderer& renderer);
	void	OnPlayerTileChange();
	Zombie* GetClosestToPlayer(const Player& player);

private:

	static const int		m_NumZombies{ 100 };
	static const int		m_MaxZombies{ 150 };
	std::vector<Zombie*>	m_Zombies{};
	float					m_SpawnCooldown{};

};


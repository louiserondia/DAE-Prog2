#include "pch.h"
#include "ZombieManager.h"
#include "Zombie.h"
#include "ZombieRenderable.h"
#include "Renderer.h"

// INIT & CLEAN

ZombieManager::ZombieManager()
{
	m_Zombies.reserve(m_MaxZombies);
	for (int idx{}; idx < m_NumZombies; ++idx)
	{
		m_Zombies.push_back(new Zombie());
	}
}

ZombieManager::~ZombieManager()
{
	for (Zombie* zombie : m_Zombies)
	{
		delete zombie;
	}
	m_Zombies.clear();
}

ZombieManager::ZombieManager(const ZombieManager& rhs) :
	m_SpawnCooldown(rhs.m_SpawnCooldown)
{
	m_Zombies.reserve(rhs.m_Zombies.size());

	for (const Zombie* zombie : rhs.m_Zombies)
	{
		m_Zombies.push_back(new Zombie{ *zombie });
	}
}

ZombieManager& ZombieManager::operator=(const ZombieManager& rhs)
{
	m_SpawnCooldown = rhs.m_SpawnCooldown;
	m_Zombies.reserve(rhs.m_Zombies.size());

	for (const Zombie* zombie : rhs.m_Zombies)
	{
		m_Zombies.push_back(new Zombie{ *zombie });
	}
	return *this;
}

ZombieManager::ZombieManager(ZombieManager&& rhs) noexcept :
	m_SpawnCooldown(rhs.m_SpawnCooldown),
	m_Zombies(std::move(rhs.m_Zombies))
{
}

ZombieManager& ZombieManager::operator=(ZombieManager&& rhs) noexcept
{
	if (&rhs != this)
	{
		m_SpawnCooldown = rhs.m_SpawnCooldown;
		m_Zombies = std::move(rhs.m_Zombies);
	}
	return *this;
}

void ZombieManager::Reset()
{
	for (Zombie* zombie : m_Zombies)
	{
		zombie->Reset();
	}
}

// UPDATE

void ZombieManager::Update(float elapsedSec, const Map& map, Player& player, Renderer& renderer)
{
	for (Zombie* zombie : m_Zombies)
	{
		if (player.DistFrom(zombie->GetCoord()) < Renderer::GetMaxRenderDist())
		{
			zombie->Update(elapsedSec, map, player);
		}
	}
	SpawnZombie(renderer);
	m_SpawnCooldown += elapsedSec;
}

void ZombieManager::AddZombiesToRenderQueue(Renderer& renderer)
{
	for (const Zombie* zombie : m_Zombies)
	{
		renderer.GetRenderQueue().push_back(zombie->GetRenderable());
	}
}

void ZombieManager::SpawnZombie(Renderer& renderer)
{
	const float max{ 10.f };
	if (m_Zombies.size() < m_MaxZombies && m_SpawnCooldown >= max)
	{
		m_SpawnCooldown = 0.f;
		m_Zombies.push_back(new Zombie());
		renderer.GetRenderQueue().push_back(m_Zombies.back()->GetRenderable());
	}
}

void ZombieManager::OnPlayerTileChange()
{
	for (Zombie* zombie : m_Zombies)
	{
		zombie->OnPlayerTileChange();
	}
}

// UTILS

Zombie* ZombieManager::GetClosestToPlayer(const Player& player)
{
	int closest{};
	Vector2f prev{ 9999.f, 9999.f };
	const Vector2f pos{ player.GetRenderable()->GetPos() };

	for (int idx{}; idx < m_Zombies.size(); ++idx)
	{
		if (m_Zombies[idx]->GetState() == Zombie::State::dead || !m_Zombies[idx]->IsInFov())
		{
			continue;
		}
		const Vector2f zombiePos{ m_Zombies[idx]->GetRenderable()->GetPos() };
		Vector2f diff{ zombiePos - pos };

		if (diff.Length() < prev.Length())
		{
			prev = diff;
			closest = idx;
		}
	}
	return m_Zombies[closest];
}

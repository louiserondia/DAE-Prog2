#pragma once
#include "Texture.h"
#include <iostream>

class Card
{
public:

	enum class Suit {
		clubs = 1,
		diamonds,
		hearts,
		spades
	};

	Card(Suit suit, int rank);
	~Card();

	int GetRank() const;
	std::string GetRankStr() const;
	std::string GetSuitStr() const;
	void Print() const;
	void Display(float x, float y) const;
	void Display(const Vector2f& pos) const;
	void Display(const Rectf& rect) const;

private:

	int m_Rank{};
	Suit m_Suit{};
	bool m_IsPlayed{};
	Texture* m_Texture{};
};


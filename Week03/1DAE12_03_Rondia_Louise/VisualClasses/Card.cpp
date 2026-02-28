#include "pch.h"
#include "Card.h"
#include <string>

Card::Card(Suit suit, int rank) : m_Suit(suit), m_Rank(rank)
{
	const std::string lead{ m_Rank < 10 ? "0" : "" };
	const std::string path{std::to_string(static_cast<int>(m_Suit)) + lead + std::to_string(m_Rank) + ".png"};
	m_Texture = new Texture{path};
}

Card::~Card() {
	delete m_Texture;
}

int Card::GetRank() const {
	return m_Rank;
}

std::string Card::GetRankStr() const {
	if (m_Rank < 11)
		return std::to_string(m_Rank);
	else if (m_Rank == 11)
		return "Jack";
	else if (m_Rank == 12)
		return "Queen";
	else if (m_Rank == 13)
		return "King";
	else if (m_Rank == 14)
		return "Ace";
}

std::string Card::GetSuitStr() const {
	std::string str{};
	switch (m_Suit)
	{
	case Card::Suit::clubs:
		str = "Clubs";
		break;
	case Card::Suit::diamonds:
		str = "Diamonds";
		break;
	case Card::Suit::hearts:
		str = "Hearts";
		break;
	case Card::Suit::spades:
		str = "Spades";
		break;
	default:
		break;
	}
	return str;
}

void Card::Print() const {
	std::cout << GetRankStr() << " of " << GetSuitStr() << std::endl;
}

void Card::Display(float x, float y) const {
	Display(Rectf{ x, y, m_Texture->GetWidth(), m_Texture->GetHeight() });
}

void Card::Display(const Vector2f& pos) const {
	Display(Rectf{ pos.x, pos.y, m_Texture->GetWidth(), m_Texture->GetHeight() });

}

void Card::Display(const Rectf& rect) const {
	m_Texture->Draw(rect);
}


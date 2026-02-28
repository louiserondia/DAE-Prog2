#include "pch.h"
#include "Movie.h"
#include "pch.h"
#include "Movie.h"

// initialization list overwrites the initialization in .h
Movie::Movie(const std::string& name) : m_Name(name) {}

void Movie::SetName(const std::string name)
{
	m_Name = name;
}

const std::string& Movie::GetName() const
{
	return m_Name;
}
int Movie::GetYear() const
{
	return m_Year;
}
int Movie::GetRating() const
{
	return m_Rating;
}

void Movie::RandomizeRating()
{
	m_Rating = rand() % (m_MaximumRating + 1);
}

void Movie::Print() const
{
	std::cout << "TITLE: " << m_Name;
	std::cout << "\n\tYear: " << m_Year;
	std::cout << "\n\tRating: " << this->GenerateRatingBar() << "\n";  // const methods can only call other const methods
}

void Movie::Update(float deltaTime)
{
	m_RefreshTimer -= deltaTime;

	if (m_RefreshTimer <= 0.f)
	{
		const int
			maximum{ 5 },
			minimum{ 2 };
		RandomizeRating();
		m_RefreshTimer += static_cast<float>((rand() % (maximum - minimum)) + minimum);
	}
}

std::string Movie::GenerateRatingBar() const
{
	const int numStars{ std::min(m_Rating, m_MaximumRating) };
	const std::string stars(numStars, '*');  // see initializing vector with default value ; similar behavior
	const std::string spaces(m_MaximumRating - numStars, ' ');

	return "[" + stars + spaces + "]";
}
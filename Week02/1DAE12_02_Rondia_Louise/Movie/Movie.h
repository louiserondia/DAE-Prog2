#pragma once
#include <iostream>

class Movie
{
public:
	void RandomizeRating();
	void Print() const;
	std::string GenerateRatingBar() const;

private:
	std::string name{};
	int year{};
	int rating{};
	const int maxRating{ 10 };
};


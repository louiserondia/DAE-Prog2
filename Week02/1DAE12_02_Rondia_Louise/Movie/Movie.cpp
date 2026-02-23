#include "pch.h"
#include "Movie.h"


void Movie::RandomizeRating() {
	rating = rand() % (maxRating + 1);
}

void Movie::Print() const {
	std::cout << "hello\n";
}

std::string Movie::GenerateRatingBar() const {
	const std::string stars(rating, '*');
	const std::string spaces(maxRating - rating, ' ');
	
	return "[ " + stars + spaces + " ]";
}

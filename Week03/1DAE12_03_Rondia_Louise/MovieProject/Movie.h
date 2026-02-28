#pragma once
#include <iostream>
#include <vector>

class Movie
{
public:

	// constructor is made to make the object in a working state, if user gives wrong parameters
	// (for invariants for instance)
	// it needs to not work, the user can't have the object working.
	// initialize is for instance for a struct where there is no invariants, then it doesn't need a constructor

	Movie() = default;
	Movie(const std::string& name);
	~Movie() = default;

	void SetName(const std::string name);

	const std::string& GetName() const;
	int GetYear() const;
	int GetRating() const;

	void Print() const;

	void RandomizeRating();
	void Update(float deltaTime);

private:

	std::string m_Name{};
	int m_Year{};
	int m_Rating{};
	float m_RefreshTimer{};
	const int m_MaximumRating{ 10 };  // const per instance

	std::string GenerateRatingBar() const;

};
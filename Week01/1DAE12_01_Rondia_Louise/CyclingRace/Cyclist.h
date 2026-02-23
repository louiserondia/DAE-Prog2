#pragma once
#include <iostream>


struct Cyclist
{
	std::string name{};
	int			color{ 97 };
	int			progression{};
	const int	bicycleWidth{ 9 };

	void Ride();
	void Print(float roadLength) const;
};
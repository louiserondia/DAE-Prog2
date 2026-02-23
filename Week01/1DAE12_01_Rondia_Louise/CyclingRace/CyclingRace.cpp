// CyclingRace.cpp : This file contains the 'main' function. Program execution begins and ends there.
// 1GD12 Rondia Louise

#include "Cyclist.h"

#include <iostream>
#include <windows.h>
#include <stdlib.h>

int main()
{
	const int roadLength{ 40 };
	std::cout << "Hello World!\n";
	Cyclist louise{ "Louise", 31 };
	Cyclist guy{ "Guy", 32 };
	for (int idx{}; idx < 10; ++idx) {
		louise.Ride();
		louise.Print(roadLength);
		guy.Ride();
		guy.Print(roadLength);

		if (louise.progression >= roadLength || guy.progression >= roadLength) {
			break;
		}

		Sleep(200);
		system("CLS");
	}
	std::cout << "We have a winner !! " << (louise.progression > guy.progression ? louise.name : guy.name) << std::endl;
}

#include "Cyclist.h"
#include <string>

void Cyclist::Ride() {
	progression += rand() % 6 + 1;
}

void Cyclist::Print(float roadLength) const {
	std::string res{ "\033[" };

	res += std::to_string(color) + "m";
	for (int idx{}; idx < progression; ++idx) {
		res += "^";
	}
	res += name;
	for (int idx{}; idx < roadLength - progression - name.length(); ++idx) {
		res += "^";
	}
	std::cout << res + "\033[0m" << std::endl;
}

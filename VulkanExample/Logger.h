#include <vector>
#include <string>
#include <iostream>

#pragma once

class Logger {
public:
	static void logInfo(const std::vector<std::string>& vs) {
		for (const auto elem : vs)
		{
			std::cout << elem << ';';
		}

		std::cout << "\n";
	}

	static void logInfo(const std::string& s) {
		std::cout << s << "\n";
	}
};
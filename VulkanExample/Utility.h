#pragma once

#include <vector>
#include <string>

class Utility {
public:
	static std::vector<const char*> transformVectorStringToCStyle(const std::vector<std::string>& vs) {
        std::vector<const char*> transformed;

        transformed.reserve(vs.size());
        for (size_t i = 0; i < vs.size(); ++i)
            transformed.push_back(vs[i].c_str());

        return transformed;
	}
};
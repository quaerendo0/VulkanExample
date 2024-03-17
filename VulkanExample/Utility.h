#pragma once

#include <vector>
#include <string>
#include <memory>

class PureCUtility {
public:
	static std::unique_ptr<std::vector<const char*>> transformVectorStringToVectorConstChar(const std::vector<std::string>& vs) {
        auto transformed = std::make_unique<std::vector<const char*>>();

        transformed->reserve(vs.size());
        for (const auto& str : vs) 
        {
            char* copiedStr = new char[str.size() + 1];
            strncpy_s(copiedStr, str.size() + 1, str.data(), str.size());
            transformed->push_back(copiedStr);
        }

        return transformed;
	}
};
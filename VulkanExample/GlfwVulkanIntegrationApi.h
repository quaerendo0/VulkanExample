#include <vector>
#include <string>
#include <GLFW/glfw3.h>

#pragma once

class GlfwVulkanIntegrationApi {
public:
    static std::vector<std::string> getReqiredVulkanExtensionsForGlfw() {

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<std::string> output;

        output.reserve(glfwExtensionCount); //ниабязательно, прост чуууууть чуть быстрее ухо будет

        for (int i = 0; i < glfwExtensionCount; ++i)
            output.push_back(glfwExtensions[i]);

        return output;
    }
};
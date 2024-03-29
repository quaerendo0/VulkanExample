#include <vector>
#include <string>
#include <GLFW/glfw3.h>

#pragma once

class GlfwVulkanIntegrationApi {
public:
    static std::vector<const char *> getReqiredVulkanExtensionsForGlfw() {

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> output;

        output.reserve(glfwExtensionCount); //�������������, ����� �������� ���� ������� ��� �����

        for (uint32_t i = 0; i < glfwExtensionCount; ++i)
            output.push_back(glfwExtensions[i]);

        return output;
    }
};
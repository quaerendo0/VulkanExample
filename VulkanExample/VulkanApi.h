#include <vector>
#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include "GlfwVulkanIntegrationApi.h"
#include "Utility.h"
#include "Logger.h"

#pragma once

class VulkanApi {
private:
    std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    std::vector<const char*> enabledExtensions;

    VkApplicationInfo vkApplicationInfo;
    VkInstanceCreateInfo vkInstanceCreateInfo;
    VkInstance vkInstance;

    static VkApplicationInfo generateVkInstance() {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        return appInfo;
    }

    VkInstanceCreateInfo generateVkInstanceInfo(const bool enableValidation) {
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &vkApplicationInfo;

        createInfo.enabledExtensionCount = enabledExtensions.size();
        createInfo.ppEnabledExtensionNames = enabledExtensions.data();

        if (enableValidation) {
            createInfo.enabledLayerCount = validationLayers.size();
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        Logger::logInfo("Extensions to enable:");
        Logger::logInfo(createInfo.ppEnabledExtensionNames, enabledExtensions.size());
        Logger::logInfo("-----");
        Logger::logInfo("Validation layers:");
        Logger::logInfo(createInfo.ppEnabledLayerNames, validationLayers.size());

        return createInfo;
    }

    VkInstance createInstance() const {
        VkInstance vki;

        if (vkCreateInstance(&vkInstanceCreateInfo, nullptr, &vki) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
        return vki;
    }

    std::vector<std::string> generateExtensionsToEnableList()
    {
        return GlfwVulkanIntegrationApi::getReqiredVulkanExtensionsForGlfw();
    }

    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const auto& layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerProperties.layerName, layerName)) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

public:

    VulkanApi(bool enableValidation)
    {
        auto reqExt = GlfwVulkanIntegrationApi::getReqiredVulkanExtensionsForGlfw();
        auto requiredExtensions = Utility::transformVectorStringToCStyle(reqExt);
        enabledExtensions.insert(enabledExtensions.begin(), requiredExtensions.begin(), requiredExtensions.end());

        if (enableValidation && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        vkApplicationInfo = generateVkInstance();
        vkInstanceCreateInfo = generateVkInstanceInfo(enableValidation);
        vkInstance = createInstance();
    }

    ~VulkanApi() {
        vkDestroyInstance(vkInstance, nullptr);
    }
};
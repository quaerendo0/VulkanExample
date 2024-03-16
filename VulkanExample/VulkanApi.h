#include <vector>
#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include "GlfwVulkanIntegrationApi.h"
#include "Utility.h"
#include "Logger.h"

#pragma once

class VulkanApi {
private:

    const std::vector<std::string> defaultValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

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

    VkInstanceCreateInfo generateVkInstanceInfo(const VkApplicationInfo& appInfo, const std::vector<std::string>& extensionsToEnable, const bool enableValidation) {
        Logger::logInfo("Extensions to enable:");
        Logger::logInfo(extensionsToEnable);
        Logger::logInfo("-----");
        Logger::logInfo("Validation layers:");
        Logger::logInfo(defaultValidationLayers);


        VkInstanceCreateInfo createInfo{};
        if (enableValidation) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(defaultValidationLayers.size());
            createInfo.ppEnabledLayerNames = Utility::transformVectorStringToCStyle(defaultValidationLayers).data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.ppEnabledExtensionNames = Utility::transformVectorStringToCStyle(extensionsToEnable).data();

        return createInfo;
    }

    VkInstance createInstance(const VkInstanceCreateInfo& createInfo) const {
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

        for (const auto& layerName : defaultValidationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (layerProperties.layerName == layerName) {
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
        if (enableValidation && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        vkApplicationInfo = generateVkInstance();
        auto extensions = generateExtensionsToEnableList();
        vkInstanceCreateInfo = generateVkInstanceInfo(vkApplicationInfo, extensions, enableValidation);
        vkInstance = createInstance(vkInstanceCreateInfo);
    }

    ~VulkanApi() {
        vkDestroyInstance(vkInstance, nullptr);
    }
};
#include <vector>
#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include "GlfwVulkanIntegrationApi.h"
#include "Utility.h"
#include "Logger.h"
#include "debugCallbacks.h"
#include "VulkanDebug.h"

#pragma once

class VulkanApi 
{
private:

    VkInstance vkInstance; 
    std::unique_ptr<VulkanDebug> vulkanDebug;

    static VkApplicationInfo generateVkInstance() 
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        return appInfo;
    }

    VkInstanceCreateInfo generateVkInstanceInfo(
        const VkApplicationInfo& appInfo, 
        const std::vector<const char*>& enabledExtensions, 
        const std::vector<const char*>& validationLayers, 
        const bool enableValidation) 
    {
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

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

    VkInstance createInstance(const VkInstanceCreateInfo& createInfo) const 
    {
        VkInstance vki;

        if (vkCreateInstance(&createInfo, nullptr, &vki) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
        return vki;
    }

    std::vector<const char *> getExtensionsToEnable(bool enableValidation)
    {
        auto exts = GlfwVulkanIntegrationApi::getReqiredVulkanExtensionsForGlfw();

        if (enableValidation) {
            exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return exts;
    }

    std::vector<const char*> getLayersToEnable(bool enableValidation)
    {
        std::vector<const char*> layers;
        if (enableValidation) 
        {
            layers.push_back("VK_LAYER_KHRONOS_validation");
        }

        return layers;
    }

    bool checkValidationLayerSupport(std::vector<const char*> validationLayers) {
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
        auto layers = getLayersToEnable(enableValidation);
        auto extensions = getExtensionsToEnable(enableValidation);

        if (enableValidation && !checkValidationLayerSupport(layers)) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        auto vkApplicationInfo = generateVkInstance();
        auto vkInstanceCreateInfo = generateVkInstanceInfo(vkApplicationInfo, extensions, layers, true);
        vkInstance = createInstance(vkInstanceCreateInfo);
        vulkanDebug = std::make_unique<VulkanDebug>(vkInstance);
    }

    ~VulkanApi() {
        vulkanDebug->DestroyDebugUtils(vkInstance);
        vkDestroyInstance(vkInstance, nullptr);
    }
};
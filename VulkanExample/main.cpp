#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include "VulkanApi.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif




class HelloTriangleApplication {
public:
    void run() {
        createWindow();
        vulkanApi = new VulkanApi(enableValidationLayers);
        mainLoop();
        cleanup();
    }

private:
    VulkanApi* vulkanApi;
    GLFWwindow* window;

    void createWindow() {
        glfwInit(); // init glfw lib, glfw creates windows
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // no opengl needed, we will use vulcan
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // no resizing needed
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        delete vulkanApi;
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
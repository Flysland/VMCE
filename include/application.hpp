////////////////////////
//
//  Created: Sun Jun 02 2024
//  File: application.hpp
//
////////////////////////

#pragma once

#include "vmce.hpp"
#include <singleton.hpp>

int main();

namespace vmce
{
    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *create_info,
        const VkAllocationCallbacks *allocator,
        VkDebugUtilsMessengerEXT *callback
    );

    void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT callback,
        const VkAllocationCallbacks *allocator
    );

    class Application : public Singleton<Application>
    {
        friend int ::main();

        public:
            Application();

        private:
            GLFWwindow *_window;
            VkInstance _instance;
            VkDebugUtilsMessengerEXT _callback;
            VkPhysicalDevice _physical_device;

            void run();
            void cleanup();
            void mainLoop();

            void initWindow();

            void initVulkan();
            void createInstance();
            void setupDebuggerMessenger();
            void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &create_info);
            bool checkValidationLayersSupport();
            std::vector<const char *> getRequiredExtensions();
            void pickPhysicalDevice();
            bool isDeviceSuitable(VkPhysicalDevice device);

            static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                VkDebugUtilsMessageTypeFlagsEXT message_type,
                const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                void *user_data
            );
    };
}

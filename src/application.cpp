////////////////////////
//
//  Created: Sun Jun 09 2024
//  File: application.cpp
//
////////////////////////

#include "application.hpp"

namespace vmce
{
    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *create_info,
        const VkAllocationCallbacks *allocator,
        VkDebugUtilsMessengerEXT *callback
    ) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

        if (func)
            return func(instance, create_info, allocator, callback);
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT callback,
        const VkAllocationCallbacks *allocator
    ) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

        if (func)
            func(instance, callback, allocator);
    }

    void Application::run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    void Application::cleanup()
    {
        DestroyDebugUtilsMessengerEXT(_instance, _callback, nullptr);
        vkDestroyInstance(_instance, nullptr);
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    void Application::mainLoop()
    {
        while (!glfwWindowShouldClose(_window))
            glfwPollEvents();
    }

    void Application::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        _window = glfwCreateWindow(800, 600, "VMCE Application", nullptr, nullptr);
    }

    void Application::initVulkan()
    {
        createInstance();
        setupDebuggerMessenger();
    }

    void Application::setupDebuggerMessenger()
    {
        if (!enable_validation_layers)
            return;

        VkDebugUtilsMessengerCreateInfoEXT create_info = VkDebugUtilsMessengerCreateInfoEXT();
        populateDebugMessengerCreateInfo(create_info);

        if (CreateDebugUtilsMessengerEXT(_instance, &create_info, nullptr, &_callback) != VK_SUCCESS)
            throw std::runtime_error("Failed to create the messenger");
    }

    void Application::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &create_info)
    {
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        create_info.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        create_info.pfnUserCallback = debugCallback;
        create_info.pUserData = nullptr;
    }

    void Application::createInstance()
    {
        if (enable_validation_layers && !checkValidationLayersSupport())
            throw std::runtime_error("Validation layers is activated but not supported");

        VkApplicationInfo app_info = VkApplicationInfo();
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = "VMCE Application";
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "No Engine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_0;
        app_info.pNext = nullptr;

        std::vector<const char *> extensions = getRequiredExtensions();

        VkInstanceCreateInfo create_info = VkInstanceCreateInfo();
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;
        create_info.enabledExtensionCount = extensions.size();
        create_info.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debug_create_info = VkDebugUtilsMessengerCreateInfoEXT();

        if (enable_validation_layers) {
            populateDebugMessengerCreateInfo(debug_create_info);
            create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
            create_info.ppEnabledLayerNames = validation_layers.data();
            create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debug_create_info;

            std::cout << "Validation layers activated:" << std::endl;
            for (const char *layer_name: validation_layers)
                std::cout << "- " << layer_name << std::endl;
            std::cout << std::endl;
        } else {
            create_info.enabledLayerCount = 0;
            create_info.pNext = nullptr;
        }

        uint32_t vk_extensions_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &vk_extensions_count, nullptr);
        std::vector<VkExtensionProperties> extensions_properties = std::vector<VkExtensionProperties>(vk_extensions_count);
        vkEnumerateInstanceExtensionProperties(nullptr, &vk_extensions_count, extensions_properties.data());

        std::cout << "Available extensions:" << std::endl;
        for (const auto &extension: extensions_properties)
            std::cout << "- " << extension.extensionName << std::endl;
        std::cout << std::endl;

        std::cout << "Enabled extensions:" << std::endl;
        for (const char *extension: extensions)
            std::cout << "- " << extension << std::endl;
        std::cout << std::endl;

        if (vkCreateInstance(&create_info, nullptr, &_instance) != VK_SUCCESS)
            throw std::runtime_error("Failed to create instance");
    }

    bool Application::checkValidationLayersSupport()
    {
        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        std::vector<VkLayerProperties> available_layers = std::vector<VkLayerProperties>(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

        std::cout << "Validation layers available:" << std::endl;
        for (const auto &layer_properties: available_layers)
            std::cout << "- " << layer_properties.layerName << std::endl;
        std::cout << std::endl;

        for (const char *layer_name: validation_layers) {
            bool layer_found = false;

            for (const auto &layer_properties: available_layers) {
                if (strcmp(layer_name, layer_properties.layerName) == 0) {
                    layer_found = true;
                    break;
                }
            }

            if (!layer_found) {
                std::cout << "Layer " << layer_name << " not available" << std::endl;
                return false;
            }
        }

        return true;
    }

    std::vector<const char *> Application::getRequiredExtensions()
    {
        uint32_t glfw_extension_count = 0;
        const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        std::vector<const char *> extensions = std::vector<const char *>(glfw_extensions, glfw_extensions + glfw_extension_count);

        if (enable_validation_layers)
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        return extensions;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Application::debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
        __attribute__((unused)) void *user_data
    ) {
        std::cerr << "Validation Layer [";
        switch (message_severity) {
            case (VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT):
                std::cerr << "INFO";
                break;
            case (VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT):
                std::cerr << "VERBOSE";
                break;
            case (VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT):
                std::cerr << "WARNING";
                break;
            case (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT):
                std::cerr << "ERROR";
                break;
            default:
                break;
        }

        std::cerr << "] [";

        switch (message_type) {
            case (VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT):
                std::cerr << "DEVICE ADDRESS BINDING";
                break;
            case (VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT):
                std::cerr << "FLAG BITS MAX";
                break;
            case (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT):
                std::cerr << "GENERAL";
                break;
            case (VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT):
                std::cerr << "PERFORMANCE";
                break;
            case (VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT):
                std::cerr << "VALIDATION";
                break;
            default:
                break;
        }

        std::cerr << "]: " << callback_data->pMessage << std::endl;
        return VK_FALSE;
    }
}

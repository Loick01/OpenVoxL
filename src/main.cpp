#include <exception>
#include <iostream>

// #include <glm/glm.hpp>
// #include <glm/vec3.hpp>

// #include <imgui.h>
// #include <imgui_impl_glfw.h>
// #include <imgui_impl_opengl3.h>

#include "core/application.hpp"

int main()
{
    Application app;
    bool isRunning = true;
    
    try {
        while(isRunning){
            isRunning = app.Run();
        }
    } catch (const std::exception e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return 0;
}
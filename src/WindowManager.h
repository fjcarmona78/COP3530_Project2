#ifndef WindowManagerMANAGER_H
#define WindowManagerMANAGER_H

#include <GLFW/glfw3.h>
#include <stdexcept>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

class WindowManager {
    private:
        const int width = 800;
        const int height = 600; 
        GLFWwindow* window = nullptr;

    public:
        WindowManager();
        ~WindowManager();
        bool shouldClose() {
            return glfwWindowShouldClose(window);
        }
        void update();
        void render();
};

#endif // WindowManagerMANAGER_H
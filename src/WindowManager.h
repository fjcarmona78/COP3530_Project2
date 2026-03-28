#ifndef WindowManagerMANAGER_H
#define WindowManagerMANAGER_H

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <stack>
#include <iostream>
#include <chrono>
#include <cmath>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/implot.h"

#include "Movie.h"
#include "BSTree.h"

class WindowManager {
    private:
        const float width = 1200;
        const float height = 720; 
        const float COLUMN_WIDTH = width/4.0;
        const float PADDING = 4.0;
        const float COLUMN_HEIGHT = height - 2*PADDING;
        const int COLUMN_FLAGS =  ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus;
        const char *comboItems[3] = {"Movie ID", "Popularity Rank", "Revenue"};

        GLFWwindow* window = nullptr;
        bool isSplay;

        std::vector<float> graphDataX;
        std::vector<float> graphDataY;

        double meanX = 0;
        double meanY = 0;
        double stddevx = 0;
        double stddevy = 0;
        double r = 0;

        char searchBuffer[24] = "\0";
        int comboOption = 0;

        std::vector<double> times;
        BSTMovie* treeRank = nullptr;
        BSTMovie* treeID = nullptr;

        void search(int32_t input);
    public:
        std::vector<Movie *> moviesDisplayed;
        WindowManager(bool isSplay, BSTMovie* treeRank, BSTMovie* treeID);
        ~WindowManager();
        bool shouldClose() {
            return glfwWindowShouldClose(window);
        }
        void insertGraphPoint(int32_t revenue, double popularity) {
            graphDataY.push_back((float) popularity);
            graphDataX.push_back((float) revenue);
        }
        void update();
        void render();
        void calculateStats();
};

#endif // WindowManagerMANAGER_H
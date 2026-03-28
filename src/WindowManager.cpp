#include "WindowManager.h"

void WindowManager::search(int32_t input) {
    if (comboOption == 0) {
        auto start = std::chrono::high_resolution_clock::now();
        moviesDisplayed.push_back(treeID->searchByMovieID(input));
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time = end - start;
        times.push_back(time.count());
    }
    else if (comboOption == 1) {
        auto start = std::chrono::high_resolution_clock::now();
        moviesDisplayed.push_back(treeRank->searchByRank(input));
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time = end - start;
        times.push_back(time.count());
    }
    else {
        auto start = std::chrono::high_resolution_clock::now();
        moviesDisplayed = treeID->searchByRevenue(input);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time = end - start;
        times.push_back(time.count());
    }

    if (!moviesDisplayed.empty() &&  moviesDisplayed[0] == nullptr) {
        moviesDisplayed.clear();
    }
}

WindowManager::WindowManager(bool isSplay, BSTMovie* treeRank, BSTMovie* treeID) {
    this->isSplay = isSplay;
    this->treeID = treeID;
    this->treeRank = treeRank;
    // GLFW INIT
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    window = glfwCreateWindow(width, height, "Cinerank Analytics", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create WindowManager!");
    }

    glfwMakeContextCurrent(window);


    // IMGUI INIT
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // IMPLOT INIT
    ImPlot::CreateContext();

    // IMGUI + GLFW
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

WindowManager::~WindowManager() {
    ImPlot::DestroyContext();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowManager::update() {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void WindowManager::render() {

    // -----PLOT COLUMN --------
    ImGui::SetNextWindowPos({PADDING, PADDING});
    ImGui::SetNextWindowSize({COLUMN_WIDTH - PADDING, COLUMN_HEIGHT});
    if (ImGui::Begin("plot column", nullptr, COLUMN_FLAGS)) {
        if (ImPlot::BeginPlot("Scatter plot")) {
            ImPlot::PlotScatter("popularity vs. revenue (M)", graphDataX.data(), graphDataY.data(), graphDataX.size());
        }ImPlot::EndPlot();

        ImGui::Separator();

        ImGui::Text("Mean Revenue: %f", meanX);
        ImGui::Text("Mean Popularity: %f", meanY);
        ImGui::Text("Std. deviation revenue: %f", stddevx);
        ImGui::Text("Std. deviation popularity: %f", stddevy);
        ImGui::Text("R: %f", r);
        ImGui::Text("R^2: %f", r*r);

    }ImGui::End();

    ImGui::SameLine();
    
    // ------------ SEARCH COLUMN ---------------
    ImGui::SetNextWindowPos({COLUMN_WIDTH + PADDING, PADDING});
    ImGui::SetNextWindowSize({2*COLUMN_WIDTH, (float) (.1*COLUMN_HEIGHT)});
    if (ImGui::Begin("search column", nullptr, COLUMN_FLAGS)) {
        ImGui::Text("Enter Search:");
        ImGui::SameLine();

        ImGui::SetNextItemWidth(2*COLUMN_WIDTH - 3*PADDING - 100);
        if (ImGui::InputText("##searchButton", searchBuffer, IM_ARRAYSIZE(searchBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            moviesDisplayed.clear();
            std::string searchResult(searchBuffer);
            if (searchResult == "List") {
                if (!isSplay) {
                    moviesDisplayed = treeRank->levelOrderTraversal();
                }
                else {
                    moviesDisplayed = treeID->levelOrderTraversal();
                }
            }
            else if (searchResult == "popular") {
                moviesDisplayed.push_back(treeRank->getMostPopularMovie());
            }
            else if (searchResult == "revenue") {
                moviesDisplayed.push_back(treeRank->getHighestRevenueMovie());
            }
            else {
                try {
                    search(std::stoll(searchBuffer));
                }
                catch (...) {
                    std::cerr << "INVALID INPUT IN SEARCH BAR" << std::endl;
                    std::cout << searchBuffer << std::endl;
                }
            }
        }
        
        ImGui::Spacing();
        ImGui::Spacing();
        
        ImGui::Dummy({50, 0});
        ImGui::SameLine();
        ImGui::Text("Search by: ");
        ImGui::SameLine();
        ImGui::Combo("##SearchCombo", &comboOption, comboItems, IM_ARRAYSIZE(comboItems));
    }ImGui::End(); 
    
    // --------------- SEARCH RESULTS -------------------------
    ImGui::SetNextWindowPos({COLUMN_WIDTH + PADDING, 2 * PADDING + (float) (.1 *COLUMN_HEIGHT)});
    ImGui::SetNextWindowSize({2*COLUMN_WIDTH, (float) (.9 * COLUMN_HEIGHT - PADDING)});
    if (ImGui::Begin("search results", nullptr, COLUMN_FLAGS | ImGuiWindowFlags_AlwaysVerticalScrollbar) && !moviesDisplayed.empty()) {
        for (auto movie : moviesDisplayed) {
            if (!movie) continue;
            ImGui::Separator();
            ImGui::Text("ID: %i", movie->movieID);
            ImGui::TextWrapped("Title: %s", movie->title.c_str());
            ImGui::TextWrapped("Genre: %s", movie->genre.c_str());
            ImGui::TextWrapped("Original Language: %s", movie->originalLanguage.c_str());
            ImGui::TextWrapped("Overview: %s", movie->overview.c_str());
            ImGui::Text("Popularity: %f", movie->popularity);
            ImGui::TextWrapped("Production Companies: %s", movie->productionCompanies.c_str());
            ImGui::Text("Release Date: %s", movie->releaseDate.c_str());
            ImGui::Text("Budget: %i", movie->budget);
            ImGui::Text("Revenue: %i", movie->revenue);
            ImGui::Text("Runtime: %d minutes", movie->runtime);
            ImGui::Text("Status: %s", movie->status.c_str());
            ImGui::Text("Tagline: %s", movie->tagline.c_str());
            ImGui::Text("Voter Average: %f", movie->voterAverage);
            ImGui::Text("Voter Count: %i", movie->voterCount);
            ImGui::TextWrapped("Credits: %s", movie->credits.c_str());
            ImGui::TextWrapped("Keywords: %s", movie->keywords.c_str());
            ImGui::TextWrapped("Poster Path: %s", movie->posterPath.c_str());
            ImGui::TextWrapped("Backdrop Path: %s", movie->backdropPath.c_str());
            ImGui::TextWrapped("Recommendations: %s", movie->recommendations.c_str());
            ImGui::Text("Popularity Rank: %i", movie->popularityRank); 
            ImGui::Separator();
        }
    } 
    else {
        ImGui::Text("Movie not found! :(");
    }ImGui::End();

    ImGui::SameLine();

    // ------------------- TIME LOG COLUMN ------------------
    ImGui::SetNextWindowPos({2*PADDING + 3*COLUMN_WIDTH, PADDING});
    ImGui::SetNextWindowSize({COLUMN_WIDTH - 3 * PADDING, (float) (.05 * COLUMN_HEIGHT)});
    if (ImGui::Begin("log column", nullptr, COLUMN_FLAGS)) {
        ImGui::Dummy({90, 0});
        ImGui::SameLine();
        ImGui::Text("Search Times");
    }ImGui::End();

    ImGui::SetNextWindowPos({2*PADDING + 3*COLUMN_WIDTH, 2 * PADDING + (float) (.05 * COLUMN_HEIGHT)});
    ImGui::SetNextWindowSize({COLUMN_WIDTH - 3 * PADDING, (float) (.95 * COLUMN_HEIGHT) - PADDING});

    if (ImGui::Begin("log results", nullptr, COLUMN_FLAGS | ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
        for (size_t i = 0; i < times.size(); i++) {
            ImGui::Text("Search %lli took %f ms", i+1, times[i]);
        }
    }ImGui::End();

    ImGui::Render();

    glViewport(0, 0, width, height);
    glClearColor(0.102, 0.098, 0.110, 1.000);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

void WindowManager::calculateStats() {
    for (float x : graphDataX) {
        meanX += x;
    }

    for (float y : graphDataY) {
        meanY += y;
    }

    meanX /= graphDataX.size();
    meanY /= graphDataY.size();

    for (float x : graphDataX) {
        stddevx += (x - meanX) * (x - meanX);
    }

    for (float y : graphDataY) {
        stddevy += (y - meanY) * (y - meanY);
    }

    stddevx /= graphDataX.size();
    stddevy /= graphDataY.size();

    stddevx = std::sqrt(stddevx);
    stddevy = std::sqrt(stddevy);

    for (size_t i = 0; i < graphDataX.size(); i++) {
        r += (graphDataX[i] - meanX) * (graphDataY[i] - meanY);
    }

    r /= graphDataX.size() * stddevx * stddevy;
}

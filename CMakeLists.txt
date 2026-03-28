cmake_minimum_required(VERSION 3.22)
project(Project2_CineRank_Analytics)

set(CMAKE_CXX_STANDARD 14)

if (MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall) 
endif()

##pull in desired version of catch through cmake automatically, make it available
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.2 # if you run into issues, try updating your editor or downgrading to a lower version of catch v3
)
FetchContent_MakeAvailable(Catch2)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9
)

FetchContent_MakeAvailable(glfw)

include_directories(src src/imgui src/imgui/backends)

add_executable(Main
        src/main.cpp
        src/imgui/imgui.cpp
        src/imgui/imgui_draw.cpp
        src/imgui/imgui_tables.cpp
        src/imgui/imgui_widgets.cpp
        src/imgui/backends/imgui_impl_opengl3.cpp
        src/imgui/backends/imgui_impl_glfw.cpp
        src/Movie.h
        src/SplayTree.cpp
        src/SplayTree.h
        src/Parser.cpp
        src/Parser.h
        src/RBTree.cpp
        src/RBTree.h 
        src/WindowManager.cpp
        )
        
target_link_libraries(Main PRIVATE glfw opengl32)

# These tests can use the Catch2-provided main
add_executable(Tests
        test/test.cpp
        src/RBTree.cpp
        src/RBTree.h  
        src/SplayTree.cpp
        src/SplayTree.h
        src/Movie.h 
        )
        
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain) #link catch to test.cpp file
# the name here must match that of your testing executable (the one that has test.cpp)

# comment everything below out if you are using CLion
include(CTest)
include(Catch)
catch_discover_tests(Tests) # must be named the same as your test executable

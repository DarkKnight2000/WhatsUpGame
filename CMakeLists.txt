cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
include(FetchContent)

project(whatsup)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 6.0)

# Raylib
set(CMAKE_BUILD_TYPE "Release")
add_subdirectory("E:/Documents/C_projects/raylib-6.0" "raylib")

# # Box2D
# FetchContent_Declare(
# 	box2d
# 	GIT_REPOSITORY https://github.com/erincatto/box2d.git
# 	GIT_TAG main
# 	GIT_SHALLOW TRUE
# 	GIT_PROGRESS TRUE
# )
# FetchContent_MakeAvailable(box2d)

# Our Project

add_executable(${PROJECT_NAME} src/main.cpp src/static.cpp src/levels_data.cpp)
#set(raylib_VERBOSE 1)
target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_link_libraries(${PROJECT_NAME} raylib)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    target_link_options(${PROJECT_NAME} PRIVATE 
        "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/res@/res"
        "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/shell.html"
    )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

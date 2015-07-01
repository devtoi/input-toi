add_executable(${CMAKE_PROJECT_NAME} main.cpp)
# Link libraries to the executable
target_link_libraries(${CMAKE_PROJECT_NAME} Utility Input ${SDL2Libraries})
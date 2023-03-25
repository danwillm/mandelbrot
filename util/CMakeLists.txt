add_library(util shader.h shader.cpp)

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
target_link_libraries(util PRIVATE GLEW::GLEW glm::glm)

target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})